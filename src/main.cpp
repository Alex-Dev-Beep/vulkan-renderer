#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include <iostream>
#include <stdexcept>
#include <chrono>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <array>
#include <cassert>

#include "validation_layers.hpp"
#include "window.hpp"
#include "instance.hpp"
#include "debug_messenger.hpp"
#include "app.hpp"
#include "surface.hpp"
#include "swapchain.hpp"
#include "image_views.hpp"
#include "render_pass.hpp"
#include "vertex.hpp"
#include "texture.hpp"
#include "model.hpp"

const std::string MODEL_PATH = "assets/models/viking_room.obj";

const int MAX_FRAMES_IN_FLIGHT = 2;

struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

class App {
public:
    void run() {
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;

    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    uint32_t currentFrame = 0;

    void initVulkan() {
        vulkan::instance.createInstance();
        vulkan::debugMessenger.setupDebugMessenger();
        vulkan::surface.createSurface();
        vulkan::physicalDevice.pickPhysicalDevice();
        vulkan::logicalDevice.createLogicalDevice();
        vulkan::swapchain.createSwapChain();
        vulkan::imageViews.createImageViews();
        vulkan::renderPass.createRenderPass();
        vulkan::descriptorSetLayout.createDescriptorSetLayout();
        vulkan::graphicsPipeline.createGraphicsPipeline();
        vulkan::commandPool.createCommandPool();
        vulkan::resources.createColorResources();
        vulkan::resources.createDepthResources();
        vulkan::framebuffers.createFramebuffers();

        renderer::texture vikingRoomTexture;
        vikingRoomTexture.path = "assets/textures/viking_room.png";

        renderer::createTexture(vikingRoomTexture);
        
        renderer::mesh vikingRoom;
        vikingRoom.path = "assets/models/viking_room.obj";
    
        renderer::createModel(vikingRoom);

        renderer::meshes.push_back(vikingRoom);

        createUniformBuffers();
        createDescriptorPool();
        createDescriptorSets();
        createCommandBuffers();
        createSyncObjects();
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(renderer::mainWindow.get())) {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(vulkan::logicalDevice.get());
    }

    void cleanupSwapChain() {
        vkDestroyImageView(vulkan::logicalDevice.get(), vulkan::resources.getDepthImage().imageView, nullptr);
        vkDestroyImage(vulkan::logicalDevice.get(), vulkan::resources.getDepthImage().image, nullptr);
        vkFreeMemory(vulkan::logicalDevice.get(), vulkan::resources.getDepthImage().imageMemory, nullptr);

        vkDestroyImageView(vulkan::logicalDevice.get(), vulkan::resources.getColorImage().imageView, nullptr);
        vkDestroyImage(vulkan::logicalDevice.get(), vulkan::resources.getColorImage().image, nullptr);
        vkFreeMemory(vulkan::logicalDevice.get(), vulkan::resources.getColorImage().imageMemory, nullptr);

        for (auto framebuffer : vulkan::swapchain.getFramebuffers()) {
            vkDestroyFramebuffer(vulkan::logicalDevice.get(), framebuffer, nullptr);
        }

        for (auto imageView : vulkan::swapchain.getImageViews()) {
            vkDestroyImageView(vulkan::logicalDevice.get(), imageView, nullptr);
        }

        vkDestroySwapchainKHR(vulkan::logicalDevice.get(), vulkan::swapchain.get(), nullptr);
    }

    void cleanup() {
        cleanupSwapChain();

        vkDestroyPipeline(vulkan::logicalDevice.get(), vulkan::graphicsPipeline.get(), nullptr);
        vkDestroyPipelineLayout(vulkan::logicalDevice.get(), vulkan::graphicsPipeline.getLayout(), nullptr);
        vkDestroyRenderPass(vulkan::logicalDevice.get(), vulkan::renderPass.get(), nullptr);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroyBuffer(vulkan::logicalDevice.get(), uniformBuffers[i], nullptr);
            vkFreeMemory(vulkan::logicalDevice.get(), uniformBuffersMemory[i], nullptr);
        }

        vkDestroyDescriptorPool(vulkan::logicalDevice.get(), descriptorPool, nullptr);

        for (const renderer::texture& tex : renderer::textures) {

            vkDestroySampler(vulkan::logicalDevice.get(), tex.textureSampler, nullptr);
            vkDestroyImageView(vulkan::logicalDevice.get(), tex.textureImageView, nullptr);

            vkDestroyImage(vulkan::logicalDevice.get(), tex.textureImage, nullptr);
            vkFreeMemory(vulkan::logicalDevice.get(), tex.textureImageMemory, nullptr);

        }

        vkDestroyDescriptorSetLayout(vulkan::logicalDevice.get(), vulkan::descriptorSetLayout.get(), nullptr);

        for (const renderer::mesh& model : renderer::meshes) {

            vkDestroyBuffer(vulkan::logicalDevice.get(), model.indexBuffer, nullptr);
            vkFreeMemory(vulkan::logicalDevice.get(), model.indexBufferMemory, nullptr);

            vkDestroyBuffer(vulkan::logicalDevice.get(), model.vertexBuffer, nullptr);
            vkFreeMemory(vulkan::logicalDevice.get(), model.vertexBufferMemory, nullptr);

        }

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(vulkan::logicalDevice.get(), renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(vulkan::logicalDevice.get(), imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(vulkan::logicalDevice.get(), inFlightFences[i], nullptr);
        }

        vkDestroyCommandPool(vulkan::logicalDevice.get(), vulkan::commandPool.get(), nullptr);

        vkDestroyDevice(vulkan::logicalDevice.get(), nullptr);

        if (enableValidationLayers) {
            renderer::validationLayers::DestroyDebugUtilsMessengerEXT(vulkan::instance.get(), vulkan::debugMessenger.get(), nullptr);
        }

        vkDestroySurfaceKHR(vulkan::instance.get(), vulkan::surface.get(), nullptr);

        glfwDestroyWindow(renderer::mainWindow.get());

        glfwTerminate();
    }

    void recreateSwapChain() {
        int width = 0, height = 0;
        glfwGetFramebufferSize(renderer::mainWindow.get(), &width, &height);
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(renderer::mainWindow.get(), &width, &height);
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(vulkan::logicalDevice.get());

        cleanupSwapChain();

        vulkan::swapchain.createSwapChain();
        vulkan::imageViews.createImageViews();
        vulkan::resources.createColorResources();
        vulkan::resources.createDepthResources();
        vulkan::framebuffers.createFramebuffers();
    }

    bool hasStencilComponent(VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

    void createUniformBuffers() {
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);

        uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
        uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
        uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vulkan::buffer.createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);

            vkMapMemory(vulkan::logicalDevice.get(), uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
        }
    }

    void createDescriptorPool() {
        std::array<VkDescriptorPoolSize, 2> poolSizes{};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        if (vkCreateDescriptorPool(vulkan::logicalDevice.get(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }
    }

    void createDescriptorSets() {
        std::vector<VkDescriptorSetLayout> layouts(
            MAX_FRAMES_IN_FLIGHT,
            vulkan::descriptorSetLayout.get()
        );

        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount =
            static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
        allocInfo.pSetLayouts = layouts.data();

        descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);

        if (vkAllocateDescriptorSets(
            vulkan::logicalDevice.get(),
            &allocInfo,
            descriptorSets.data()
        ) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        const auto& textures = renderer::textures;

        std::vector<VkDescriptorImageInfo> imageInfos;
        imageInfos.reserve(textures.size());

        for (const renderer::texture& tex : textures) {
            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = tex.textureImageView;
            imageInfo.sampler = tex.textureSampler;

            imageInfos.push_back(imageInfo);
        }

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = uniformBuffers[i];
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);

            std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

            // Uniform buffer
            descriptorWrites[0].sType =
                VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[0].dstSet = descriptorSets[i];
            descriptorWrites[0].dstBinding = 0;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType =
                VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pBufferInfo = &bufferInfo;

            // Texture array
            descriptorWrites[1].sType =
                VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[1].dstSet = descriptorSets[i];
            descriptorWrites[1].dstBinding = 1;
            descriptorWrites[1].dstArrayElement = 0;
            descriptorWrites[1].descriptorType =
                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[1].descriptorCount =
                static_cast<uint32_t>(imageInfos.size());
            descriptorWrites[1].pImageInfo = imageInfos.data();

            vkUpdateDescriptorSets(
                vulkan::logicalDevice.get(),
                static_cast<uint32_t>(descriptorWrites.size()),
                descriptorWrites.data(),
                0,
                nullptr
            );
        }
    }

    void createCommandBuffers() {
        commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = vulkan::commandPool.get();
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

        if (vkAllocateCommandBuffers(vulkan::logicalDevice.get(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }



    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = vulkan::renderPass.get();
        renderPassInfo.framebuffer = vulkan::swapchain.getFramebuffers()[imageIndex];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = vulkan::swapchain.getExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(
            commandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            vulkan::graphicsPipeline.get()
        );

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(vulkan::swapchain.getExtent().width);
        viewport.height = static_cast<float>(vulkan::swapchain.getExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = vulkan::swapchain.getExtent();

        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        for (const auto& mesh : renderer::meshes) {
            VkBuffer vertexBuffers[] = {mesh.vertexBuffer};
            VkDeviceSize offsets[] = {0};

            vkCmdBindVertexBuffers(
                commandBuffer, 0, 1, vertexBuffers, offsets
            );

            

            vkCmdBindIndexBuffer(
                commandBuffer,
                mesh.indexBuffer,
                0,
                VK_INDEX_TYPE_UINT32
            );

            vkCmdBindDescriptorSets(
                commandBuffer, 
                VK_PIPELINE_BIND_POINT_GRAPHICS, 
                vulkan::graphicsPipeline.getLayout(), 
                0, 
                1,
                &descriptorSets[currentFrame], 
                0, 
                nullptr
            );

            vkCmdDrawIndexed(
                commandBuffer,
                static_cast<uint32_t>(mesh.indices.size()),
                1,
                0,
                0,
                0
            );
        }

        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }

    void createSyncObjects() {
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(vulkan::logicalDevice.get(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(vulkan::logicalDevice.get(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(vulkan::logicalDevice.get(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create synchronization objects for a frame!");
            }
        }
    }

    void updateUniformBuffer(uint32_t currentImage) {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        UniformBufferObject ubo{};
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f), vulkan::swapchain.getExtent().width / (float) vulkan::swapchain.getExtent().height, 0.1f, 10.0f);
        ubo.proj[1][1] *= -1;

        memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
    }

    void drawFrame() {
        vkWaitForFences(vulkan::logicalDevice.get(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(vulkan::logicalDevice.get(), vulkan::swapchain.get(), UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return;
        } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        updateUniformBuffer(currentFrame);

        vkResetFences(vulkan::logicalDevice.get(), 1, &inFlightFences[currentFrame]);

        vkResetCommandBuffer(commandBuffers[currentFrame], /*VkCommandBufferResetFlagBits*/ 0);
        recordCommandBuffer(commandBuffers[currentFrame], imageIndex);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

        VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(vulkan::queues.graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {vulkan::swapchain.get()};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &imageIndex;

        result = vkQueuePresentKHR(vulkan::queues.presentQueue, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
            renderer::mainWindow.wasFramebufferResized()
        ) {
            renderer::mainWindow.resetFramebufferResized();
            recreateSwapChain();
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }
};

int main() {
    App app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
