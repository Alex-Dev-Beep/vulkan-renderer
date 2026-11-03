#!/bin/bash
set -e

cmake -S . -B build \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

cmake --build build --config Debug --parallel

glslc assets/shaders/shader.frag -o assets/shaders/frag.spv

glslc assets/shaders/shader.vert -o assets/shaders/vert.spv