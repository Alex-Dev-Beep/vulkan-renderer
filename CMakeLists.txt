
cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(Renderer)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package(Vulkan REQUIRED)

add_subdirectory(thirdparty/glfw)
add_subdirectory(thirdparty/glm)

add_executable(renderer
    src/main.cpp
    src/validation_layers.hpp
    src/validation_layers.cpp
    src/window.hpp
    src/window.cpp
    src/instance.hpp
    src/instance.cpp
    src/debug_messenger.cpp
    src/debug_messenger.hpp
    src/app.hpp
    src/surface.hpp
    src/surface.cpp
    src/physical_device.hpp
    src/physical_device.cpp
    src/logical_device.cpp
    src/logical_device.hpp
    src/swapchain.hpp
    src/swapchain.cpp
    src/msaaSamples.hpp
    src/msaaSamples.cpp
    src/image_views.hpp
    src/image_views.cpp
    src/render_pass.hpp
    src/render_pass.cpp
    src/descriptor_sets.cpp
    src/descriptor_sets.hpp
    src/graphics_pipeline.cpp
    src/graphics_pipeline.hpp
    src/vertex.hpp
    src/filesystem.hpp
    src/filesystem.cpp
    src/command_pool.cpp
    src/command_pool.hpp
)

target_link_libraries(renderer PRIVATE
    glfw
    Vulkan::Vulkan
)

target_include_directories(renderer PRIVATE
    thirdparty/model_loading
    thirdparty/stb
    thirdparty/glm
    thirdparty/glfw/include
    include
    src
)
