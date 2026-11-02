#include "image_views.hpp"
#include "app.hpp"
#include "swapchain.hpp"

#include <stdexcept>

namespace renderer {
    void imageViews::createImageViews() {
        vulkan::swapchain.getImageViews().resize(vulkan::swapchain.getImages().size());

        for (uint32_t i = 0; i < vulkan::swapchain.getImages().size(); i++) {
            vulkan::swapchain.getImageViews()[i] = createImageView(vulkan::swapchain.getImages()[i], vulkan::swapchain.getImageFormat(), VK_IMAGE_ASPECT_COLOR_BIT, 1);
        }
    }

    VkImageView imageViews::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels) {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = mipLevels;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VkImageView imageView;
        if (vkCreateImageView(vulkan::logicalDevice.get(), &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image view!");
        }

        return imageView;
    }
}