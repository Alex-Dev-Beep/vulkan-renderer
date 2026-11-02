#include "framebuffers.hpp"
#include "objects.hpp"

#include <array>
#include <stdexcept>

namespace renderer {
    void framebuffers::createFramebuffers() {
        vulkan::swapchain.getFramebuffers().resize(vulkan::swapchain.getImageViews().size());

        for (size_t i = 0; i < vulkan::swapchain.getImageViews().size(); i++) {
            std::array<VkImageView, 3> attachments = {
                vulkan::resources.getColorImage().imageView,
                vulkan::resources.getDepthImage().imageView,
                vulkan::swapchain.getImageViews()[i]
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = vulkan::renderPass.get();
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = vulkan::swapchain.getExtent().width;
            framebufferInfo.height = vulkan::swapchain.getExtent().height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(vulkan::logicalDevice.get(), &framebufferInfo, nullptr, &vulkan::swapchain.getFramebuffers()[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
    }
}