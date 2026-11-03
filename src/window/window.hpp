#pragma once

#include <GLFW/glfw3.h>

namespace renderer {
    class window {
    public:
        window(int width, int height);
        ~window();

        GLFWwindow* get() const;

        bool wasFramebufferResized() const {
            return framebufferResized;
        }

        void resetFramebufferResized() {
            framebufferResized = false;
        }

        static void framebufferResizeCallback(
            GLFWwindow* window,
            int width,
            int height
        );

    private:
        GLFWwindow* mainWindow = nullptr;
        bool framebufferResized = false;
    };

    extern window mainWindow;
    inline float mainScale;
}