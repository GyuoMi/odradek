#!/bin/bash

# Check if libnoise is installed
if [ ! -d "libnoise" ]; then
    echo "Cloning libnoise repository..."
    git clone https://github.com/qknight/libnoise.git
fi

# Build libnoise
echo "Building libnoise..."
cd libnoise
mkdir build
cd build
cmake ..
make
cd ../..

# Build the terrain visualization program
echo "Building terrain visualization..."
make all

echo "Build complete. Run './terrain' to execute the program."
