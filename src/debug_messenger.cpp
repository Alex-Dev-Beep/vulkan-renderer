#include "debug_messenger.hpp"
#include "validation_layers.hpp"
#include "instance.hpp"
#include "app.hpp"

#include <stdexcept>

namespace renderer {
    void debugMessenger::setupDebugMessenger() {
        if (!enableValidationLayers) return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        renderer::validationLayers::populateDebugMessengerCreateInfo(createInfo);

        if (renderer::validationLayers::CreateDebugUtilsMessengerEXT(vulkan::instance.get(), &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
            throw std::runtime_error("failed to set up debug messenger!");
        }
    }

    VkDebugUtilsMessengerEXT debugMessenger::get() const {
        return debugMessenger;
    }
}