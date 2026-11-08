@echo off
setlocal EnableExtensions

title Vulkan Renderer - Windows Release Build

REM ============================================================
REM Detect required tools
REM ============================================================

echo.
echo Detecting toolchain...

where gcc >nul 2>&1 || (
    echo [ERROR] GCC not found in PATH.
    exit /b 1
)

where g++ >nul 2>&1 || (
    echo [ERROR] G++ not found in PATH.
    exit /b 1
)

where mingw32-make >nul 2>&1 || (
    echo [ERROR] MinGW Make not found in PATH.
    echo Install it through MSYS2:
    echo pacman -S mingw-w64-ucrt-x86_64-make
    exit /b 1
)

where glslc >nul 2>&1 || (
    echo [ERROR] GLSLC not found in PATH.
    echo Make sure the Vulkan SDK is installed.
    exit /b 1
)

where cmake >nul 2>&1 || (
    echo [ERROR] CMake not found in PATH.
    exit /b 1
)

REM ============================================================
REM Get executable paths
REM ============================================================

for /f "delims=" %%i in ('where gcc') do (
    set "C_COMPILER=%%i"
    goto :gcc_found
)
:gcc_found

for /f "delims=" %%i in ('where g++') do (
    set "CXX_COMPILER=%%i"
    goto :gxx_found
)
:gxx_found

for /f "delims=" %%i in ('where mingw32-make') do (
    set "MAKE_PROGRAM=%%i"
    goto :make_found
)
:make_found

for /f "delims=" %%i in ('where glslc') do (
    set "GLSLC=%%i"
    goto :glslc_found
)
:glslc_found

echo GCC:   %C_COMPILER%
echo G++:   %CXX_COMPILER%
echo Make:  %MAKE_PROGRAM%
echo GLSLC: %GLSLC%

REM ============================================================
REM Verify project files
REM ============================================================

if not exist "CMakeLists.txt" (
    echo [ERROR] CMakeLists.txt not found.
    exit /b 1
)

if not exist "assets" (
    echo [ERROR] Assets directory not found.
    exit /b 1
)

REM ============================================================
REM Configure CMake - Release
REM ============================================================

echo.
echo Configuring Release build...

cmake -S . -B build ^
    -G "MinGW Makefiles" ^
    -DCMAKE_MAKE_PROGRAM="%MAKE_PROGRAM%" ^
    -DCMAKE_C_COMPILER="%C_COMPILER%" ^
    -DCMAKE_CXX_COMPILER="%CXX_COMPILER%" ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

if errorlevel 1 (
    echo [ERROR] CMake configuration failed.
    exit /b 1
)

REM ============================================================
REM Build project
REM ============================================================

echo.
echo Building Release...

cmake --build build --config Release --parallel

if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Build failed.
    exit /b 1
)

REM ============================================================
REM Copy assets
REM ============================================================

echo.
echo Copying assets...

if exist "build\assets" (
    rmdir /s /q "build\assets"
)

xcopy "assets" "build\assets" /E /I /Y >nul

if errorlevel 1 (
    echo [ERROR] Failed to copy assets.
    exit /b 1
)

REM ============================================================
REM Compile shaders
REM ============================================================

echo.
echo Compiling shaders...

"%GLSLC%" "build\assets\shaders\shader.frag" ^
    -o "build\assets\shaders\frag.spv"

if errorlevel 1 (
    echo [ERROR] Failed to compile fragment shader.
    exit /b 1
)

"%GLSLC%" "build\assets\shaders\shader.vert" ^
    -o "build\assets\shaders\vert.spv"

if errorlevel 1 (
    echo [ERROR] Failed to compile vertex shader.
    exit /b 1
)

REM ============================================================
REM Finished
REM ============================================================

echo.
echo ========================================
echo       RELEASE BUILD SUCCESSFUL
echo ========================================

endlocal
exit /b 0