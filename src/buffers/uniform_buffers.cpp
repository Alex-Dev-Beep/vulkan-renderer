#include <buffers/uniform_buffers.hpp>
#include <app/app.hpp>

#include <cstring>
#include <chrono>

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

    void uniformBuffers::updateUniformBuffer(uint32_t currentImage) {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        UniformBufferObject ubo{};
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f), vulkan::swapchain.getExtent().width / (float) vulkan::swapchain.getExtent().height, 0.1f, 10.0f);
        ubo.proj[1][1] *= -1;

        memcpy(vulkan::uniformBuffers.getUniformBuffersMapped()[currentImage], &ubo, sizeof(ubo));
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