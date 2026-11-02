#include "renderer.hpp"
#include "app.hpp"
#include "window.hpp"
#include "uniform_buffers.hpp"

#include <stdexcept>

namespace renderer {
    void drawFrame() {
        vkWaitForFences(vulkan::logicalDevice.get(), 1, &vulkan::inFlightFences[vulkan::currentFrame], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(vulkan::logicalDevice.get(), vulkan::swapchain.get(), UINT64_MAX, vulkan::imageAvailableSemaphores[vulkan::currentFrame], VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            vulkan::swapchain.recreateSwapChain();
            return;
        } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        vulkan::uniformBuffers.updateUniformBuffer(vulkan::currentFrame);

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
            vulkan::swapchain.recreateSwapChain();
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }

        vulkan::currentFrame = (vulkan::currentFrame + 1) % vulkan::MAX_FRAMES_IN_FLIGHT;
    }
}