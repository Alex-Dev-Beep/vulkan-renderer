#include <debug/debug_messenger.hpp>
#include <debug/validation_layers.hpp>
#include <images/image_views.hpp>
#include <app/app.hpp>

#include <stdexcept>
#include <iostream>

namespace renderer {
    void debugMessenger::setupDebugMessenger() {
        std::cout << "Setting up the debug messenger..." << std::endl;
        if (!enableValidationLayers) return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        renderer::validationLayers::populateDebugMessengerCreateInfo(createInfo);

        if (renderer::validationLayers::CreateDebugUtilsMessengerEXT(vulkan::instance.get(), &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
            throw std::runtime_error("Failed to set up debug messenger!");
        }

        std::cout << "Sucesfully set up the debug messenger." << std::endl;
    }

    VkDebugUtilsMessengerEXT debugMessenger::get() const {
        return debugMessenger;
    }
}