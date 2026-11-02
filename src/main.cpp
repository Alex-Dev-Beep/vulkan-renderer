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
#include <cassert>

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
#include "cleanup.hpp"

class App {
public:
    void run() {
        initVulkan();
        mainLoop();
        renderer::cleanup();
    }

private:

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

        vulkan::uniformBuffers.createUniformBuffers();
        vulkan::descriptorPool.createDescriptorPool();
        vulkan::descriptorSets.createDescriptorSets();
        vulkan::commandBuffer.createCommandBuffers();
        createSyncObjects();
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(renderer::mainWindow.get())) {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(vulkan::logicalDevice.get());
    }

    void recreateSwapChain() {
        int width = 0, height = 0;
        glfwGetFramebufferSize(renderer::mainWindow.get(), &width, &height);
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(renderer::mainWindow.get(), &width, &height);
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(vulkan::logicalDevice.get());

        renderer::cleanupSwapChain();

        vulkan::swapchain.createSwapChain();
        vulkan::imageViews.createImageViews();
        vulkan::resources.createColorResources();
        vulkan::resources.createDepthResources();
        vulkan::framebuffers.createFramebuffers();
    }

    bool hasStencilComponent(VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

    void createSyncObjects() {
        vulkan::imageAvailableSemaphores.resize(vulkan::MAX_FRAMES_IN_FLIGHT);
        vulkan::renderFinishedSemaphores.resize(vulkan::MAX_FRAMES_IN_FLIGHT);
        vulkan::inFlightFences.resize(vulkan::MAX_FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < vulkan::MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(vulkan::logicalDevice.get(), &semaphoreInfo, nullptr, &vulkan::imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(vulkan::logicalDevice.get(), &semaphoreInfo, nullptr, &vulkan::renderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(vulkan::logicalDevice.get(), &fenceInfo, nullptr, &vulkan::inFlightFences[i]) != VK_SUCCESS) {
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

        memcpy(vulkan::uniformBuffers.getUniformBuffersMapped()[currentImage], &ubo, sizeof(ubo));
    }

    void drawFrame() {
        vkWaitForFences(vulkan::logicalDevice.get(), 1, &vulkan::inFlightFences[vulkan::currentFrame], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(vulkan::logicalDevice.get(), vulkan::swapchain.get(), UINT64_MAX, vulkan::imageAvailableSemaphores[vulkan::currentFrame], VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return;
        } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        updateUniformBuffer(vulkan::currentFrame);

        vkResetFences(vulkan::logicalDevice.get(), 1, &vulkan::inFlightFences[vulkan::currentFrame]);

        vkResetCommandBuffer(vulkan::commandBuffer.commandBuffers[vulkan::currentFrame], /*VkCommandBufferResetFlagBits*/ 0);
        vulkan::commandBuffer.recordCommandBuffer(vulkan::commandBuffer.commandBuffers[vulkan::currentFrame], imageIndex);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {vulkan::imageAvailableSemaphores[vulkan::currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &vulkan::commandBuffer.commandBuffers[vulkan::currentFrame];

        VkSemaphore signalSemaphores[] = {vulkan::renderFinishedSemaphores[vulkan::currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(vulkan::queues.graphicsQueue, 1, &submitInfo, vulkan::inFlightFences[vulkan::currentFrame]) != VK_SUCCESS) {
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

        vulkan::currentFrame = (vulkan::currentFrame + 1) % vulkan::MAX_FRAMES_IN_FLIGHT;
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
