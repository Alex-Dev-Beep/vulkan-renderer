
#include <buffers/vertex_buffer.hpp>
#include <app/app.hpp>
#include <mesh/model.hpp>

#include <cstring>
#include <stdexcept>

namespace renderer {

    void createVertexBuffer(mesh& model) {
        VkDeviceSize bufferSize = sizeof(model.vertices[0]) * model.vertices.size();

        VkBuffer stagingBuffer;
        VmaAllocation stagingAllocation;

        vulkan::buffer.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
            stagingBuffer,
            stagingAllocation
        );

        void* data = nullptr;

        VkResult result = vmaMapMemory(
            vulkan::memoryAllocator.get(),
            stagingAllocation,
            &data
        );

        if (result != VK_SUCCESS) {
            vulkan::buffer.destroyBuffer(stagingBuffer, stagingAllocation);
            throw std::runtime_error("Failed to map staging buffer!");
        }

        std::memcpy(data, model.vertices.data(), static_cast<size_t>(bufferSize));

        vmaUnmapMemory(
            vulkan::memoryAllocator.get(),
            stagingAllocation
        );

        vulkan::buffer.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT |
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            0,
            model.vertexBuffer,
            model.vertexBufferAllocation
        );

        vulkan::buffer.copyBuffer(
            stagingBuffer,
            model.vertexBuffer,
            bufferSize
        );

        vulkan::buffer.destroyBuffer(
            stagingBuffer,
            stagingAllocation
        );
    }

}
