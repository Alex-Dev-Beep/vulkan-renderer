#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdint>
#include <vector>
#include <string>

#include <vk_mem_alloc.h>

namespace renderer {
    class texture {
    public:
        uint32_t mipLevels{};

        VkImage textureImage{};
        VkImageView textureImageView{};
        VkSampler textureSampler{};

        VmaAllocation textureImageAllocation{};

        std::string path;
    };

    inline std::vector<texture> textures;
    void createTextureImage(texture& tex);
    void createTextureImageView(texture& tex);
    void createTextureSampler(texture& tex);
    void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
    void createTexture(texture& tex);
}