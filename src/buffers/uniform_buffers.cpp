
#include <buffers/uniform_buffers.hpp>
#include <app/app.hpp>
#include <window/window.hpp>

#include <cstring>
#include <chrono>
#include <stdexcept>
#include <iostream>
#include <cmath>

#include <GLFW/glfw3.h>

namespace renderer {

    void uniformBuffers::createUniformBuffers() {
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);

        vkUniformBuffers.resize(vulkan::MAX_FRAMES_IN_FLIGHT);
        uniformBuffersAllocation.resize(vulkan::MAX_FRAMES_IN_FLIGHT);
        uniformBuffersMapped.resize(vulkan::MAX_FRAMES_IN_FLIGHT);

        std::cout << "[UBO] Creating uniform buffers...\n";

        for (size_t i = 0; i < vulkan::MAX_FRAMES_IN_FLIGHT; i++) {
            vulkan::buffer.createBuffer(
                bufferSize,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT,
                vkUniformBuffers[i],
                uniformBuffersAllocation[i]
            );

            VkResult result = vmaMapMemory(
                vulkan::memoryAllocator.get(),
                uniformBuffersAllocation[i],
                &uniformBuffersMapped[i]
            );

            if (result != VK_SUCCESS) {
                throw std::runtime_error(
                    "Failed to map uniform buffer memory!"
                );
            }

            std::cout << "[UBO] Buffer " << i << " mapped successfully\n";
        }

        std::cout << "[UBO] All buffers created successfully\n";
    }

    
    void uniformBuffers::updateUniformBuffer(uint32_t currentImage) {
        using clock = std::chrono::steady_clock;

        static auto lastFrame = clock::now();
        const auto currentTime = clock::now();

        float deltaTime =
            std::chrono::duration<float>(currentTime - lastFrame).count();

        lastFrame = currentTime;
        deltaTime = glm::clamp(deltaTime, 0.0f, 0.1f);

        static glm::vec3 cameraPosition{2.0f, 2.0f, 2.0f};

        static float yaw = -135.0f;
        static float pitch = -35.264f;

        static bool wasRightPressed = false;

        static double lastMouseX = 0.0;
        static double lastMouseY = 0.0;

        static auto lastDebugPrint = clock::now();

        GLFWwindow* window = renderer::mainWindow.get();

        if (!window)
            throw std::runtime_error("[CAMERA] GLFW window is null!");

        const bool rightPressed =
            glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

        const bool justPressed = rightPressed && !wasRightPressed;
        const bool justReleased = !rightPressed && wasRightPressed;

        wasRightPressed = rightPressed;

        if (justPressed) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            glfwGetCursorPos(window, &lastMouseX, &lastMouseY);

            std::cout << "[CAMERA] Mouse captured\n";
        }

        if (justReleased) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

            std::cout << "[CAMERA] Mouse released\n";
        }

        const glm::vec3 worldUp{0.0f, 1.0f, 0.0f};

        glm::vec3 cameraFront;

        cameraFront.x =
            cos(glm::radians(yaw)) *
            cos(glm::radians(pitch));

        cameraFront.y =
            sin(glm::radians(pitch));

        cameraFront.z =
            sin(glm::radians(yaw)) *
            cos(glm::radians(pitch));

        cameraFront = glm::normalize(cameraFront);

        if (rightPressed) {

            double mouseX, mouseY;

            glfwGetCursorPos(window, &mouseX, &mouseY);

            const float xOffset =
                static_cast<float>(mouseX - lastMouseX);

            const float yOffset =
                static_cast<float>(lastMouseY - mouseY);

            lastMouseX = mouseX;
            lastMouseY = mouseY;

            constexpr float sensitivity = 0.1f;

            yaw += xOffset * sensitivity;
            pitch += yOffset * sensitivity;

            pitch = glm::clamp(pitch, -89.0f, 89.0f);

            cameraFront.x =
                cos(glm::radians(yaw)) *
                cos(glm::radians(pitch));

            cameraFront.y =
                sin(glm::radians(pitch));

            cameraFront.z =
                sin(glm::radians(yaw)) *
                cos(glm::radians(pitch));

            cameraFront = glm::normalize(cameraFront);

            const glm::vec3 cameraRight = glm::normalize(
                glm::cross(cameraFront, worldUp)
            );

            float cameraSpeed = 3.0f;

            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                cameraSpeed *= 2.5f;

            const float velocity = cameraSpeed * deltaTime;

            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                cameraPosition += cameraFront * velocity;

            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                cameraPosition -= cameraFront * velocity;

            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                cameraPosition -= cameraRight * velocity;

            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                cameraPosition += cameraRight * velocity;

            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
                cameraPosition += worldUp * velocity;

            if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
                cameraPosition -= worldUp * velocity;

            
            if (currentTime - lastDebugPrint >=
                std::chrono::milliseconds(250)) {

                std::cout
                    << "\n========== CAMERA DEBUG ==========\n"
                    << "Yaw: " << yaw << '\n'
                    << "Pitch: " << pitch << '\n'

                    << "Front: "
                    << cameraFront.x << ", "
                    << cameraFront.y << ", "
                    << cameraFront.z << '\n'

                    << "Position: "
                    << cameraPosition.x << ", "
                    << cameraPosition.y << ", "
                    << cameraPosition.z << '\n'

                    << "Delta time: " << deltaTime << '\n'
                    << "==================================\n";

                lastDebugPrint = currentTime;
            }
        }

        
        UniformBufferObject ubo{};

        static auto startTime = clock::now();

        const float time =
            std::chrono::duration<float>(
                currentTime - startTime
            ).count();

        ubo.model = glm::rotate(
            glm::mat4(1.0f),
            0 * glm::radians(90.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        );

        ubo.view = glm::lookAt(
            cameraPosition,
            cameraPosition + cameraFront,
            worldUp
        );

        const auto extent = vulkan::swapchain.getExtent();

        if (extent.width == 0 || extent.height == 0)
            return;

        ubo.proj = glm::perspective(
            glm::radians(45.0f),
            extent.width / static_cast<float>(extent.height),
            0.1f,
            100.0f
        );

        ubo.proj[1][1] *= -1;

        if (currentImage >= uniformBuffersMapped.size())
            throw std::runtime_error("[UBO] Invalid current image index!");

        if (!uniformBuffersMapped[currentImage])
            throw std::runtime_error("[UBO] Uniform buffer is not mapped!");

        std::memcpy(
            uniformBuffersMapped[currentImage],
            &ubo,
            sizeof(ubo)
        );
    }


    std::vector<VkBuffer> uniformBuffers::getUniformBuffers() const {
        return vkUniformBuffers;
    }

    std::vector<VmaAllocation>
    uniformBuffers::getUniformBuffersAllocation() const {
        return uniformBuffersAllocation;
    }

    std::vector<void*>
    uniformBuffers::getUniformBuffersMapped() const {
        return uniformBuffersMapped;
    }

}
