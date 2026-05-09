# Parallel Canny Edge Detector (RAJA Portability Suite)

This repository contains a modular, performance-portable implementation of the Canny Edge Detector (CED). It is built using the Mike Heath sequential baseline and is structured to be transitioned into a parallelized architecture using the RAJA Portability Suite, Umpire, and Chai. 

*Note: The use of AI assistance for this Digital Image Processing task has been explicitly approved by the instructor.*

## Prerequisites

To compile and run this project, your environment must meet the following minimum requirements:
* **OS:** Ubuntu (or compatible Linux distribution)
* **Compiler:** GCC/G++ version 12 (Crucial for CUDA toolkit compatibility)
* **CMake:** Version 3.23 or higher
* **GPU Toolkit:** NVIDIA CUDA Toolkit (v11.0+)

## Setup and Building

1. **Clone the repository:**
   ```bash
   git clone git@github.com:AHSANooo/Canny-Edge-Detector-RAJA.git
   cd Canny-Edge-Detector-RAJA

   Initialize Submodules:
    If the third_party libraries (RAJA, Umpire, Chai, blt) are empty, pull them down:
    Bash

    git submodule update --init --recursive

    Configure the Build System:
    Generate the CMake cache. We explicitly force GCC-12 to prevent compiler mismatch errors with NVCC, and disable tests/examples to speed up compilation.
    Bash

cmake -B build -S . -DCMAKE_C_COMPILER=gcc-12 -DCMAKE_CXX_COMPILER=g++-12 -DENABLE_CUDA=ON -DCMAKE_CUDA_ARCHITECTURES=61 -DENABLE_TESTS=OFF -DRAJA_ENABLE_EXAMPLES=OFF -DRAJA_ENABLE_EXERCISES=OFF -DUMPIRE_ENABLE_EXAMPLES=OFF -DCHAI_ENABLE_EXAMPLES=OFF -DCMAKE_CXX_FLAGS="-DFMT_USE_USER_DEFINED_LITERALS=0 -DFMT_USE_NONTYPE_TEMPLATE_ARGS=0" -DCMAKE_CUDA_FLAGS="-DFMT_USE_USER_DEFINED_LITERALS=0 -DFMT_USE_NONTYPE_TEMPLATE_ARGS=0"

    Compile:
    Bash

    cmake --build build -j$(nproc)

Running the Code

Once compiled, the executable canny_baseline will be located in the build/ directory. The program requires two arguments: the path to your input image and the desired path for the output edge map.

    Place your test image in the data/ directory (e.g., data/my_test_image.jpg).

    Run the executable:
    Bash

    ./build/canny_baseline data/my_test_image.jpg output_edges.png

Check the root directory for output_edges.png to view the final mathematical structural contours.