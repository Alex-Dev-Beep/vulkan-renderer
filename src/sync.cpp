#include "sync.hpp"
#include "objects.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>

namespace renderer {
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
}