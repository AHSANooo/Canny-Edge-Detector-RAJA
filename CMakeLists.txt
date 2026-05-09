cmake_minimum_required(VERSION 3.14)
project(CannyEdgeDetector LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configure CUDA architectures if specified
if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
    set(CMAKE_CUDA_ARCHITECTURES 61)
endif()

# Setup BLT
include(third_party/blt/SetupBLT.cmake)

# Include RAJA, Umpire, and Chai
add_subdirectory(third_party/umpire)
add_subdirectory(third_party/raja)
add_subdirectory(third_party/chai)

# Set backend flags based on what CMake detects or user provides
# RAJA uses RAJA_ENABLE_CUDA, RAJA_ENABLE_OPENMP etc.
blt_add_executable(
    NAME canny_raja_baseline
    SOURCES 
        src/main.cpp
        src/pgm_io.cpp
        src/gaussian.cpp
        src/derivative.cpp
        src/nms.cpp
        src/hysteresis.cpp
    INCLUDES include/
    DEPENDS 
        raja 
        umpire 
        chai
)
