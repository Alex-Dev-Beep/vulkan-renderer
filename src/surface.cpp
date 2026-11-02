#include "surface.hpp"
#include "objects.hpp"
#include "window.hpp"

#include <stdexcept>

namespace renderer {
    
    VkSurfaceKHR surface::get() const {
        return surface;
    }

    void surface::createSurface() {
        if (glfwCreateWindowSurface(vulkan::instance.get(), renderer::mainWindow.get(), nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface!");
        }
    }
}