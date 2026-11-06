#include <surface/surface.hpp>
#include <app/app.hpp>
#include <window/window.hpp>

#include <stdexcept>
#include <iostream>

namespace renderer {
    
    VkSurfaceKHR surface::get() const {
        return surface;
    }

    void surface::createSurface() {
        std::cout << "Creating surface..." << std::endl;
        if (glfwCreateWindowSurface(vulkan::instance.get(), renderer::mainWindow.get(), nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create window surface!");
        }

        std::cout << "Succesfully created surface." << std::endl;
    }
}