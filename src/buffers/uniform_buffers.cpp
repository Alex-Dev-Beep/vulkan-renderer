#include <buffers/uniform_buffers.hpp>
#include <app/app.hpp>

#include <cstring>
#include <chrono>
#include <stdexcept>

namespace renderer {
    void uniformBuffers::createUniformBuffers() {
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);

        vkUniformBuffers.resize(vulkan::MAX_FRAMES_IN_FLIGHT);
        uniformBuffersAllocation.resize(vulkan::MAX_FRAMES_IN_FLIGHT);
        uniformBuffersMapped.resize(vulkan::MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < vulkan::MAX_FRAMES_IN_FLIGHT; i++) {
            vulkan::buffer.createBuffer(
                bufferSize,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT,
                vkUniformBuffers[i],
                uniformBuffersAllocation[i]
            );

            VkResult result = vmaMapMemory(
                vulkan::memoryAllocator.get(),
                uniformBuffersAllocation[i],
                &uniformBuffersMapped[i]
            );

            if (result != VK_SUCCESS) {
                throw std::runtime_error("failed to map uniform buffer memory!");
            }
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

    std::vector<VmaAllocation> uniformBuffers::getUniformBuffersAllocation() const {
        return uniformBuffersAllocation;
    }

    std::vector<void*> uniformBuffers::getUniformBuffersMapped() const {
        return uniformBuffersMapped;
    }
}