#include <window/window.hpp>
#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>

#include <iostream>

namespace renderer {
    window mainWindow{800, 720};

    window::window(int width, int height) {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        mainScale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()); 

        std::cout << mainScale << std::endl;

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