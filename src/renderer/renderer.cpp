#include <renderer/renderer.hpp>
#include <app/app.hpp>
#include <window/window.hpp>
#include <buffers/uniform_buffers.hpp>

#include <stdexcept>

namespace renderer {
    void drawFrame() {
        // 1. Esperar el fence del frame slot
        vkWaitForFences(vulkan::logicalDevice.get(), 1,
                        &vulkan::inFlightFences[vulkan::currentFrame],
                        VK_TRUE, UINT64_MAX);

        // 2. Acquire — inicializamos imageIndex y pasamos el semáforo
        //    indexado por currentFrame (aún no conocemos imageIndex)
        uint32_t imageIndex = 0;
        VkResult result = vkAcquireNextImageKHR(
            vulkan::logicalDevice.get(),
            vulkan::swapchain.get(),
            UINT64_MAX,
            vulkan::imageAvailableSemaphores[vulkan::currentFrame],  // ✅ conocido antes
            VK_NULL_HANDLE,
            &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            vulkan::swapchain.recreateSwapChain();
            return;
        } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        vulkan::uniformBuffers.updateUniformBuffer(vulkan::currentFrame);

        vkResetFences(vulkan::logicalDevice.get(), 1,
                      &vulkan::inFlightFences[vulkan::currentFrame]);

        vkResetCommandBuffer(
            vulkan::commandBuffer.commandBuffers[vulkan::currentFrame], 0);
        vulkan::commandBuffer.recordCommandBuffer(
            vulkan::commandBuffer.commandBuffers[vulkan::currentFrame], imageIndex);

        // 3. Submit: espera el MISMO semáforo que señaló acquire
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {
            vulkan::imageAvailableSemaphores[vulkan::currentFrame]  // ✅ mismo que acquire
        };
        VkPipelineStageFlags waitStages[] = {
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
        };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers =
            &vulkan::commandBuffer.commandBuffers[vulkan::currentFrame];

        // 4. Signal: renderFinished indexado por IMAGEN (patrón a)
        VkSemaphore signalSemaphores[] = {
            vulkan::renderFinishedSemaphores[imageIndex]  // ✅ por imagen
        };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(vulkan::queues.graphicsQueue, 1, &submitInfo,
                          vulkan::inFlightFences[vulkan::currentFrame]) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        // 5. Present: espera el mismo renderFinished[imageIndex]
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;  // renderFinished[imageIndex] ✅

        VkSwapchainKHR swapChains[] = {vulkan::swapchain.get()};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;

        result = vkQueuePresentKHR(vulkan::queues.presentQueue, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
            renderer::mainWindow.wasFramebufferResized()) {
            renderer::mainWindow.resetFramebufferResized();
            vulkan::swapchain.recreateSwapChain();
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }

        vulkan::currentFrame = (vulkan::currentFrame + 1) % vulkan::MAX_FRAMES_IN_FLIGHT;
    }
}