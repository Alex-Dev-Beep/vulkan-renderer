
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
        // Framebuffers
        for (auto framebuffer : vulkan::swapchain.getFramebuffers()) {
            vkDestroyFramebuffer(
                vulkan::logicalDevice.get(),
                framebuffer,
                nullptr
            );
        }

        // Depth image
        vkDestroyImageView(
            vulkan::logicalDevice.get(),
            vulkan::resources.getDepthImage().imageView,
            nullptr
        );

        vkDestroyImage(
            vulkan::logicalDevice.get(),
            vulkan::resources.getDepthImage().image,
            nullptr
        );

        vkFreeMemory(
            vulkan::logicalDevice.get(),
            vulkan::resources.getDepthImage().imageMemory,
            nullptr
        );

        // Color image
        vkDestroyImageView(
            vulkan::logicalDevice.get(),
            vulkan::resources.getColorImage().imageView,
            nullptr
        );

        vkDestroyImage(
            vulkan::logicalDevice.get(),
            vulkan::resources.getColorImage().image,
            nullptr
        );

        vkFreeMemory(
            vulkan::logicalDevice.get(),
            vulkan::resources.getColorImage().imageMemory,
            nullptr
        );

        // Swapchain image views
        for (auto imageView : vulkan::swapchain.getImageViews()) {
            vkDestroyImageView(
                vulkan::logicalDevice.get(),
                imageView,
                nullptr
            );
        }

        // Swapchain
        vkDestroySwapchainKHR(
            vulkan::logicalDevice.get(),
            vulkan::swapchain.get(),
            nullptr
        );
    }

    void cleanup() {

        // Wait for GPU
        vkDeviceWaitIdle(vulkan::logicalDevice.get());

        // ImGui
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        // Swapchain resources
        cleanupSwapChain();

        // Graphics pipeline
        vkDestroyPipeline(
            vulkan::logicalDevice.get(),
            vulkan::graphicsPipeline.get(),
            nullptr
        );

        vkDestroyPipelineLayout(
            vulkan::logicalDevice.get(),
            vulkan::graphicsPipeline.getLayout(),
            nullptr
        );

        // Render pass
        vkDestroyRenderPass(
            vulkan::logicalDevice.get(),
            vulkan::renderPass.get(),
            nullptr
        );

        // Uniform buffers
        for (size_t i = 0; i < vulkan::MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroyBuffer(
                vulkan::logicalDevice.get(),
                vulkan::uniformBuffers.getUniformBuffers()[i],
                nullptr
            );

            vkFreeMemory(
                vulkan::logicalDevice.get(),
                vulkan::uniformBuffers.getUniformBuffersMemory()[i],
                nullptr
            );
        }

        // Textures
        for (const renderer::texture& tex : renderer::textures) {

            vkDestroySampler(
                vulkan::logicalDevice.get(),
                tex.textureSampler,
                nullptr
            );

            vkDestroyImageView(
                vulkan::logicalDevice.get(),
                tex.textureImageView,
                nullptr
            );

            vkDestroyImage(
                vulkan::logicalDevice.get(),
                tex.textureImage,
                nullptr
            );

            vkFreeMemory(
                vulkan::logicalDevice.get(),
                tex.textureImageMemory,
                nullptr
            );
        }

        // Meshes
        for (const renderer::mesh& model : renderer::meshes) {

            vkDestroyBuffer(
                vulkan::logicalDevice.get(),
                model.indexBuffer,
                nullptr
            );

            vkFreeMemory(
                vulkan::logicalDevice.get(),
                model.indexBufferMemory,
                nullptr
            );

            vkDestroyBuffer(
                vulkan::logicalDevice.get(),
                model.vertexBuffer,
                nullptr
            );

            vkFreeMemory(
                vulkan::logicalDevice.get(),
                model.vertexBufferMemory,
                nullptr
            );
        }

        // Descriptor pools
        vkDestroyDescriptorPool(
            vulkan::logicalDevice.get(),
            vulkan::descriptorPool.getImguiPool(),
            nullptr
        );

        vkDestroyDescriptorPool(
            vulkan::logicalDevice.get(),
            vulkan::descriptorPool.get(),
            nullptr
        );

        // Descriptor set layout
        vkDestroyDescriptorSetLayout(
            vulkan::logicalDevice.get(),
            vulkan::descriptorSetLayout.get(),
            nullptr
        );

        // Synchronization objects
        for (size_t i = 0; i < vulkan::MAX_FRAMES_IN_FLIGHT; i++) {

            vkDestroySemaphore(
                vulkan::logicalDevice.get(),
                vulkan::renderFinishedSemaphores[i],
                nullptr
            );

            vkDestroySemaphore(
                vulkan::logicalDevice.get(),
                vulkan::imageAvailableSemaphores[i],
                nullptr
            );

            vkDestroyFence(
                vulkan::logicalDevice.get(),
                vulkan::inFlightFences[i],
                nullptr
            );
        }

        // Command pool (also destroys allocated command buffers)
        vkDestroyCommandPool(
            vulkan::logicalDevice.get(),
            vulkan::commandPool.get(),
            nullptr
        );

        // Logical device
        vkDestroyDevice(
            vulkan::logicalDevice.get(),
            nullptr
        );

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
