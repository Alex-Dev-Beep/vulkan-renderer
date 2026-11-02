#include <commands/command_pool.hpp>
#include <device/logical_device.hpp>
#include <app/app.hpp>

#include <stdexcept>

namespace renderer {
    void commandPool::createCommandPool() {
        QueueFamilyIndices queueFamilyIndices = vulkan::physicalDevice.findQueueFamilies(vulkan::physicalDevice.get());

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

        if (vkCreateCommandPool(vulkan::logicalDevice.get(), &poolInfo, nullptr, &vkCommandPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create graphics command pool!");
        }
    }

    VkCommandPool commandPool::get() const {
        return vkCommandPool;
    }
}