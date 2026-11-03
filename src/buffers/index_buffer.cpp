#include "index_buffer.hpp"
#include <app/app.hpp>

#include <cstring>

namespace renderer {
    void createIndexBuffer(mesh& model) {
        VkDeviceSize bufferSize = sizeof(model.indices[0]) * model.indices.size();

        VkBuffer stagingBuffer;
        VmaAllocation stagingBufferAllocation;
        vulkan::buffer.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferAllocation);

        void* data;

        vmaMapMemory(vulkan::memoryAllocator.get(), stagingBufferAllocation, &data);
        memcpy(data, model.indices.data(), (size_t) bufferSize);
        vmaUnmapMemory(vulkan::memoryAllocator.get(), stagingBufferAllocation);

        vulkan::buffer.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, model.indexBuffer, model.indexBufferAllocation);
        vulkan::buffer.copyBuffer(stagingBuffer, model.indexBuffer, bufferSize);

        vmaDestroyBuffer(vulkan::memoryAllocator.get(), stagingBuffer, stagingBufferAllocation);
    }
};