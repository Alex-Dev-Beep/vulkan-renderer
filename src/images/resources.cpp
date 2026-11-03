#include <images/resources.hpp>
#include <app/app.hpp>
#include <images/image.hpp>

#include <iostream>

namespace renderer {
    resourceImage resources::getColorImage() const {
        return colorImage;
    }

    resourceImage resources::getDepthImage() const {
        return depthImage;
    }

    void resources::createColorResources() {
        VkFormat colorFormat = vulkan::swapchain.getImageFormat();


        vulkan::image.createImage(vulkan::swapchain.getExtent().width, vulkan::swapchain.getExtent().height, 1, vulkan::msaaSamples, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, colorImage.image, colorImage.allocation);
        
        
        colorImage.imageView = vulkan::imageViews.createImageView(colorImage.image, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }

    void resources::createDepthResources() {
        VkFormat depthFormat = vulkan::renderPass.findDepthFormat();

        vulkan::image.createImage(vulkan::swapchain.getExtent().width, vulkan::swapchain.getExtent().height, 1, vulkan::msaaSamples, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage.image, depthImage.allocation);
        depthImage.imageView = vulkan::imageViews.createImageView(depthImage.image, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
    }
}