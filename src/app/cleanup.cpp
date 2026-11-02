#include <app/cleanup.hpp>
#include <app/app.hpp>
#include <texture/texture.hpp>
#include <model_loading/model.hpp>
#include <debug/validation_layers.hpp>
#include <window/window.hpp>

namespace renderer {
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

        for (size_t i = 0; i < vulkan::MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroyBuffer(vulkan::logicalDevice.get(), vulkan::uniformBuffers.getUniformBuffers()[i], nullptr);
            vkFreeMemory(vulkan::logicalDevice.get(), vulkan::uniformBuffers.getUniformBuffersMemory()[i], nullptr);
        }

        vkDestroyDescriptorPool(vulkan::logicalDevice.get(), vulkan::descriptorPool.get(), nullptr);

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

        for (size_t i = 0; i < vulkan::MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(vulkan::logicalDevice.get(), vulkan::renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(vulkan::logicalDevice.get(), vulkan::imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(vulkan::logicalDevice.get(), vulkan::inFlightFences[i], nullptr);
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
}