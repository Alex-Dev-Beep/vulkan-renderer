@echo off
setlocal

REM ============================================================
REM Verify that the required toolchain paths exist
REM ============================================================
if not exist "C:\mingw64\bin\gcc.exe" (
    echo [ERROR] GCC not found at: C:\mingw64\bin\gcc.exe
    echo         Please check your MinGW installation path.
    exit /b 1
)

if not exist "C:\mingw64\bin\g++.exe" (
    echo [ERROR] G++ not found at: C:\mingw64\bin\g++.exe
    echo         Please check your MinGW installation path.
    exit /b 1
)

where glslc >nul 2>&1
if errorlevel 1 (
    echo [ERROR] glslc not found in PATH.
    echo         Make sure the Vulkan SDK is installed and glslc is on your PATH.
    exit /b 1
)

if not exist "assets" (
    echo [ERROR] The "assets" directory was not found.
    echo         Expected it at: %CD%\assets
    exit /b 1
)

REM ============================================================
REM Configure CMake to use MinGW Makefiles and GCC/G++
REM ============================================================
cmake -S . -B build ^
    -G "MinGW Makefiles" ^
    -DCMAKE_C_COMPILER="C:/mingw64/bin/gcc.exe" ^
    -DCMAKE_CXX_COMPILER="C:/mingw64/bin/g++.exe" ^
    -DCMAKE_BUILD_TYPE=Debug ^
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

if errorlevel 1 (
    echo [ERROR] CMake configuration failed.
    echo         Check the output above for details.
    exit /b 1
)

REM ============================================================
REM Build the project
REM ============================================================
cmake --build build --config Debug --parallel

if errorlevel 1 (
    echo [ERROR] Build failed.
    echo         Check the output above for details.
    exit /b 1
)

REM ============================================================
REM Copy assets into the build directory
REM ============================================================
xcopy /E /I /Y "assets" "build\assets"

if errorlevel 1 (
    echo [ERROR] Failed to copy the "assets" directory into "build".
    exit /b 1
)

REM ============================================================
REM Compile shaders with glslc
REM ============================================================
glslc build/assets/shaders/shader.frag -o build/assets/shaders/frag.spv

if errorlevel 1 (
    echo [ERROR] Failed to compile shader.frag
    exit /b 1
)

glslc build/assets/shaders/shader.vert -o build/assets/shaders/vert.spv

if errorlevel 1 (
    echo [ERROR] Failed to compile shader.vert
    exit /b 1
)

echo [OK] Build completed successfully.
endlocal