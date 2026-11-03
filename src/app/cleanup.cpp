
#include <app/cleanup.hpp>
#include <app/app.hpp>
#include <texture/texture.hpp>
#include <mesh/model.hpp>
#include <debug/validation_layers.hpp>
#include <window/window.hpp>

#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>

namespace renderer {

    void cleanupSwapChain() {
        VkDevice device = vulkan::logicalDevice.get();

        // Framebuffers
        for (auto framebuffer : vulkan::swapchain.getFramebuffers()) {
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }

        // Depth image
        // vulkan::resources.get()

        vkDestroyImageView(device, vulkan::resources.getDepthImage().imageView, nullptr);

        vmaDestroyImage(
            vulkan::memoryAllocator.get(),
            vulkan::resources.getDepthImage().image,
            vulkan::resources.getDepthImage().allocation
        );

        // Color image
        // vulkan::resources.getvulkan::resources.getColorImage()()

        vkDestroyImageView(device, vulkan::resources.getColorImage().imageView, nullptr);

        vmaDestroyImage(
            vulkan::memoryAllocator.get(),
            vulkan::resources.getColorImage().image,
            vulkan::resources.getColorImage().allocation
        );

        // Swapchain image views
        for (auto imageView : vulkan::swapchain.getImageViews()) {
            vkDestroyImageView(device, imageView, nullptr);
        }

        // Swapchain
        vkDestroySwapchainKHR(
            device,
            vulkan::swapchain.get(),
            nullptr
        );
    }

    void cleanup() {
        VkDevice device = vulkan::logicalDevice.get();
        VmaAllocator allocator = vulkan::memoryAllocator.get();

        // Wait for GPU
        vkDeviceWaitIdle(device);

        // ImGui
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        // Swapchain resources
        cleanupSwapChain();

        // Graphics pipeline
        vkDestroyPipeline(
            device,
            vulkan::graphicsPipeline.get(),
            nullptr
        );

        vkDestroyPipelineLayout(
            device,
            vulkan::graphicsPipeline.getLayout(),
            nullptr
        );

        // Render pass
        vkDestroyRenderPass(
            device,
            vulkan::renderPass.get(),
            nullptr
        );

        // Uniform buffers
        for (size_t i = 0; i < vulkan::MAX_FRAMES_IN_FLIGHT; i++) {
            vmaUnmapMemory(
                vulkan::memoryAllocator.get(),
                vulkan::uniformBuffers.getUniformBuffersAllocation()[i]
            );

            vulkan::buffer.destroyBuffer(
                vulkan::uniformBuffers.getUniformBuffers()[i],
                vulkan::uniformBuffers.getUniformBuffersAllocation()[i]
            );
        }

        // Textures
        for (const renderer::texture& tex : renderer::textures) {

            vkDestroySampler(
                device,
                tex.textureSampler,
                nullptr
            );

            vkDestroyImageView(
                device,
                tex.textureImageView,
                nullptr
            );

            vmaDestroyImage(
                allocator,
                tex.textureImage,
                tex.textureImageAllocation
            );
        }

        // Meshes
        for (const renderer::mesh& model : renderer::meshes) {

            vulkan::buffer.destroyBuffer(
                model.indexBuffer,
                model.indexBufferAllocation
            );

            vulkan::buffer.destroyBuffer(
                model.vertexBuffer,
                model.vertexBufferAllocation
            );
        }

        // Descriptor pools
        vkDestroyDescriptorPool(
            device,
            vulkan::descriptorPool.getImguiPool(),
            nullptr
        );

        vkDestroyDescriptorPool(
            device,
            vulkan::descriptorPool.get(),
            nullptr
        );

        // Descriptor set layout
        vkDestroyDescriptorSetLayout(
            device,
            vulkan::descriptorSetLayout.get(),
            nullptr
        );

        // Synchronization objects
        for (size_t i = 0; i < vulkan::MAX_FRAMES_IN_FLIGHT; i++) {

            vkDestroySemaphore(
                device,
                vulkan::renderFinishedSemaphores[i],
                nullptr
            );

            vkDestroySemaphore(
                device,
                vulkan::imageAvailableSemaphores[i],
                nullptr
            );

            vkDestroyFence(
                device,
                vulkan::inFlightFences[i],
                nullptr
            );
        }

        // Command pool
        vkDestroyCommandPool(
            device,
            vulkan::commandPool.get(),
            nullptr
        );

        // VMA allocator
        vmaDestroyAllocator(allocator);

        // Logical device
        vkDestroyDevice(device, nullptr);

        // Debug messenger
        if (enableValidationLayers) {
            renderer::validationLayers::DestroyDebugUtilsMessengerEXT(
                vulkan::instance.get(),
                vulkan::debugMessenger.get(),
                nullptr
            );
        }

        // Surface
        vkDestroySurfaceKHR(
            vulkan::instance.get(),
            vulkan::surface.get(),
            nullptr
        );

        // Instance
        vkDestroyInstance(
            vulkan::instance.get(),
            nullptr
        );

        // GLFW
        glfwDestroyWindow(renderer::mainWindow.get());
        glfwTerminate();
    }

}
