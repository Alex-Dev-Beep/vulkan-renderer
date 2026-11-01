#include "swapchain.hpp"
#include "app.hpp"
#include <cassert>
namespace renderer {
    renderer::SwapChainSupportDetails swapchain::querySwapChainSupport(VkPhysicalDevice device) {
        assert(vulkan::instance.get() != VK_NULL_HANDLE);
        assert(vulkan::surface.get() != VK_NULL_HANDLE);
        assert(device != VK_NULL_HANDLE);
        renderer::SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, vulkan::surface.get(), &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, vulkan::surface.get(), &formatCount, nullptr);

        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, vulkan::surface.get(), &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, vulkan::surface.get(), &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, vulkan::surface.get(), &presentModeCount, details.presentModes.data());
        }

        return details;
    }
}