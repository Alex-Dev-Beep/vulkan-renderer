#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

namespace renderer {
    class uniformBuffers {
        public:
        void createUniformBuffers();
        void updateUniformBuffer(uint32_t currentImage);
        std::vector<VkBuffer> getUniformBuffers() const;
        std::vector<VkDeviceMemory> getUniformBuffersMemory() const;
        std::vector<void*> getUniformBuffersMapped() const;

        private:
        std::vector<VkBuffer> vkUniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;
        std::vector<void*> uniformBuffersMapped;
    };
}