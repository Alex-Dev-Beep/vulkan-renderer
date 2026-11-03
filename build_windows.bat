@echo off
setlocal

cmake -S . -B build ^
    -DCMAKE_BUILD_TYPE=Debug ^
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

if errorlevel 1 exit /b 1

cmake --build build --config Debug --parallel

if errorlevel 1 exit /b 1

glslc assets/shaders/shader.frag -o assets/shaders/frag.spv
if errorlevel 1 exit /b 1

glslc assets/shaders/shader.vert -o assets/shaders/vert.spv
if errorlevel 1 exit /b 1

endlocal