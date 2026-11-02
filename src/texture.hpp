#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdint>
#include <vector>
#include <string>

namespace renderer {
    class texture {
        public:
        uint32_t mipLevels;
        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
        VkImageView textureImageView;
        VkSampler textureSampler;
        std::string path;
    };

    class textures {
        public:
        std::vector<texture> textures;
        void createTextureImage(texture& tex);
        void createTextureImageView(texture& tex);
        void createTextureSampler(texture& tex);
        void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
        void createTexture(texture& tex);
    };
}