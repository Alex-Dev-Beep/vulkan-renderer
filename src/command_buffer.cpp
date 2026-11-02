#include "command_buffer.hpp"
#include "app.hpp"

#include <stdexcept>

namespace renderer {
    VkCommandBuffer commandBuffer::beginSingleTimeCommands() {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = vulkan::commandPool.get();
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer{};

        VkResult result = vkAllocateCommandBuffers(
            vulkan::logicalDevice.get(),
            &allocInfo,
            &commandBuffer
        );

        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate command buffer");
        }

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        result = vkBeginCommandBuffer(commandBuffer, &beginInfo);

        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to begin command buffer");
        }

        return commandBuffer;
    }

    void commandBuffer::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(vulkan::queues.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(vulkan::queues.graphicsQueue);

        vkFreeCommandBuffers(vulkan::logicalDevice.get(), vulkan::commandPool.get(), 1, &commandBuffer);
    }
}