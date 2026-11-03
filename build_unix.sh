#!/usr/bin/env bash
set -euo pipefail

if command -v ninja >/dev/null 2>&1; then
    GEN="Ninja"
else
    GEN="Unix Makefiles"
fi

cmake -S . -B build \
    -G "$GEN" \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

cmake --build build --config Debug --parallel

glslc assets/shaders/shader.frag -o assets/shaders/frag.spv
glslc assets/shaders/shader.vert -o assets/shaders/vert.spv