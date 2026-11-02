#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <mesh/model.hpp>

namespace renderer {
    void createVertexBuffer(mesh& model);
}