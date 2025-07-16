@echo off
REM Build the Crimson Key C API example
echo Building Crimson Key example...

REM Change to project root directory
cd /d "%~dp0\.."

REM Create build directory if it doesn't exist
if not exist "build" mkdir build

REM Build the example
gcc -o build\example.exe examples\example_usage.c -I src\c-api -std=c99

REM Check if build was successful
if exist build\example.exe (
    echo Build successful!
    echo Running example...
    echo.
    build\example.exe
) else (
    echo Build failed! Make sure you have GCC installed.
    echo You can install it from: https://www.mingw-w64.org/
)
REM *"No AI agents were harmed during the making of this build script."* 
