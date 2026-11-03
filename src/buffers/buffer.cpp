#include <buffers/buffer.hpp>
#include <app/app.hpp>

#include <buffers/buffer.hpp>
#include <memory/memory.hpp>
#include <commands/command_buffer.hpp>

#include <stdexcept>
#include <iostream>
#include <vulkan/vulkan_core.h>

namespace renderer {
    void buffer::createBuffer(
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VmaAllocationCreateFlags flags,
        VkBuffer& buffer,
        VmaAllocation& allocation
    ) {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VmaAllocationCreateInfo allocInfo{};
        allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
        allocInfo.flags = flags;

        if (flags & VMA_ALLOCATION_CREATE_MAPPED_BIT) {
            allocInfo.flags |=
                VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
        }

        VkResult result = vmaCreateBuffer(
            vulkan::memoryAllocator.get(),
            &bufferInfo,
            &allocInfo,
            &buffer,
            &allocation,
            nullptr
        );

        vulkan::memoryAllocator.checkVmaResult(
            result,
            "vmaCreateBuffer"
        );
    }

    void buffer::destroyBuffer(VkBuffer buffer, VmaAllocation allocation) {
        vmaDestroyBuffer(
            vulkan::memoryAllocator.get(),
            buffer,
            allocation
        );
    }

    void buffer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
        VkCommandBuffer commandBuffer =
            vulkan::commandBuffer.beginSingleTimeCommands();

        VkBufferCopy copyRegion{};
        copyRegion.size = size;

        vkCmdCopyBuffer(
            commandBuffer,
            srcBuffer,
            dstBuffer,
            1,
            &copyRegion
        );

        vulkan::commandBuffer.endSingleTimeCommands(commandBuffer);
    }

}