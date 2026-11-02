#include "window.hpp"

namespace renderer {
    window mainWindow{800, 720};

    window::window(int width, int height) {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        mainWindow = glfwCreateWindow(
            width,
            height,
            "Vulkan Renderer",
            nullptr,
            nullptr
        );

        glfwSetWindowUserPointer(mainWindow, this);
        glfwSetFramebufferSizeCallback(mainWindow, framebufferResizeCallback);
    }

    window::~window() {
        if (mainWindow) {
            glfwDestroyWindow(mainWindow);
        }

        glfwTerminate();
    }

    GLFWwindow* window::get() const {
        return mainWindow;
    }

    void window::framebufferResizeCallback(
        GLFWwindow* window,
        int width,
        int height
    ) {
        auto* app = static_cast<renderer::window*>(
            glfwGetWindowUserPointer(window)
        );

        app->framebufferResized = true;
    }
}