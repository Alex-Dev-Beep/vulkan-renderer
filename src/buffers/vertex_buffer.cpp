#include "vertex_buffer.hpp"
#include <app/app.hpp>
#include <model_loading/model.hpp>

#include <cstring>

namespace renderer {
    void createVertexBuffer(mesh& model) {
        VkDeviceSize bufferSize = sizeof(model.vertices[0]) * model.vertices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        vulkan::buffer.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(vulkan::logicalDevice.get(), stagingBufferMemory, 0, bufferSize, 0, &data);
            memcpy(data, model.vertices.data(), (size_t) bufferSize);
        vkUnmapMemory(vulkan::logicalDevice.get(), stagingBufferMemory);

        vulkan::buffer.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, model.vertexBuffer, model.vertexBufferMemory);

        vulkan::buffer.copyBuffer(stagingBuffer, model.vertexBuffer, bufferSize);

        vkDestroyBuffer(vulkan::logicalDevice.get(), stagingBuffer, nullptr);
        vkFreeMemory(vulkan::logicalDevice.get(), stagingBufferMemory, nullptr);
    }
}