#include "uniform_buffers.hpp"
#include "app.hpp"

namespace renderer {
    void uniformBuffers::createUniformBuffers() {
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);

        vulkan::uniformBuffers.vkUniformBuffers.resize(vulkan::MAX_FRAMES_IN_FLIGHT);
        vulkan::uniformBuffers.uniformBuffersMemory.resize(vulkan::MAX_FRAMES_IN_FLIGHT);
        vulkan::uniformBuffers.uniformBuffersMapped.resize(vulkan::MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < vulkan::MAX_FRAMES_IN_FLIGHT; i++) {
            vulkan::buffer.createBuffer(
                bufferSize, 
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                vkUniformBuffers[i], 
                uniformBuffersMemory[i]
            );

            vkMapMemory(
                vulkan::logicalDevice.get(), 
                vulkan::uniformBuffers.uniformBuffersMemory[i], 
                0, 
                bufferSize, 
                0, 
                &vulkan::uniformBuffers.uniformBuffersMapped[i]
            );
        }
    }

    std::vector<VkBuffer> uniformBuffers::getUniformBuffers() const {
        return vkUniformBuffers;
    }

    std::vector<VkDeviceMemory> uniformBuffers::getUniformBuffersMemory() const {
        return uniformBuffersMemory;
    }

    std::vector<void*> uniformBuffers::getUniformBuffersMapped() const {
        return uniformBuffersMapped;
    }
}