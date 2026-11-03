@echo off
setlocal

cmake -S . -B build ^
    -DCMAKE_BUILD_TYPE=Debug ^
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

if errorlevel 1 exit /b 1

cmake --build build --config Debug --parallel

if errorlevel 1 exit /b 1

REM
xcopy /E /I /Y "assets" "build\assets"
if errorlevel 1 exit /b 1

REM
glslc build/assets/shaders/shader.frag -o build/assets/shaders/frag.spv
if errorlevel 1 exit /b 1

glslc build/assets/shaders/shader.vert -o build/assets/shaders/vert.spv
if errorlevel 1 exit /b 1

endlocal