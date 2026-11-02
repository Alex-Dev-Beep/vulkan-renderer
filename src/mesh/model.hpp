#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdint>
#include <vector>
#include <string>

#include "vertex.hpp"

namespace renderer {
    class mesh {
        public:
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;
        VkDescriptorSet descriptorSet;

        std::string path;
    };

    void loadModel(mesh& model);
    void createModel(mesh& model);
    inline std::vector<mesh> meshes;
}