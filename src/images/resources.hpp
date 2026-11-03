
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vk_mem_alloc.h>

namespace renderer {

    class resourceImage {
    public:
        VkImage image{};
        VmaAllocation allocation{};
        VkImageView imageView{};
    };

    class resources {
    public:
        void createColorResources();
        void createDepthResources();

        resourceImage getColorImage() const;
        resourceImage getDepthImage() const;

    private:
        inline static resourceImage colorImage{};
        inline static resourceImage depthImage{};
    };

}
