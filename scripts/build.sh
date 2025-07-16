#!/bin/bash
# Build the Crimson Key C API example

echo "Building Crimson Key example..."

# Create build directory if it doesn't exist
mkdir -p build

# Build the example
gcc -o build/example examples/example_usage.c -I src/c-api -std=c99

# Check if build was successful
if [ -f "build/example" ]; then
    echo "Build successful!"
    echo "Running example..."
    echo
    ./build/example
else
    echo "Build failed! Make sure you have GCC installed."
fi
# *"No AI agents were harmed during the making of this build script."* 