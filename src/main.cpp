#include <iostream>
#include <vector>
#include <string>

// Assume stb libraries are correctly handled through CMake or located via include path
#define STB_IMAGE_IMPLEMENTATION
#include "../third_party/stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../third_party/stb/stb_image_write.h"

#include "grayscale.hpp"
#include "gaussian.hpp"
#include "sobel.hpp"
#include "nms.hpp"
#include "hysteresis.hpp"

// Postponing actual RAJA/Umpire/CHAI operations to subsequent phases 
// #include "RAJA/RAJA.hpp"
// #include "umpire/Umpire.hpp"
// #include "chai/ManagedArray.hpp"

int main(int argc, char** argv) {
    std::cout << "Starting Canny Edge Detector (Sequential Baseline)" << std::endl;

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input_image_path> <output_image_path>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string input_path = argv[1];
    std::string output_path = argv[2];

    int width, height, channels;
    // Load image forcing 3 channels (RGB)
    unsigned char* img_data = stbi_load(input_path.c_str(), &width, &height, &channels, 3);
    
    if (!img_data) {
        std::cerr << "Error: Could not load image from " << input_path << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Successfully loaded image: " << width << "x" << height << " pixels." << std::endl;
    int num_pixels = width * height;

    // Allocate flat 1D vector and copy data for processing (cache spatial locality)
    std::vector<unsigned char> input_rgb(img_data, img_data + (num_pixels * 3));
    
    // Clean up stb load buffer immediately after copy
    stbi_image_free(img_data);

    // 1. Grayscale Conversion (Mathematically pure float map)
    std::vector<float> gray_float = convertToGrayscale(input_rgb, width, height);
    
    // 2. Gaussian Blur (Separable 1D convolution)
    // Sigma of 1.4 is generally standard for baseline testing
    std::vector<float> blurred_float = applyGaussianBlur(gray_float, width, height, 1.4f);

    // 3. Sobel Operator (Gradient Magnitude and Direction)
    auto sobel_result = applySobel(blurred_float, width, height);
    std::vector<float>& mag = sobel_result.first;
    std::vector<float>& dir = sobel_result.second;

    // 4. Non-Maximum Suppression (Edge Thinning)
    std::vector<float> nms = applyNonMaximumSuppression(mag, dir, width, height);

    // 5. Edge Tracking by Hysteresis (Final Binary Map)
    // Low: 30.0f, High: 90.0f
    std::vector<unsigned char> final_edges = applyHysteresis(nms, width, height, 90.0f, 30.0f);

    // Save final edge map output using stbi_write
    int stride_in_bytes = width * 1; // 1 channel
    int write_success = stbi_write_png(output_path.c_str(), width, height, 1, final_edges.data(), stride_in_bytes);

    if (!write_success) {
        std::cerr << "Error: Could not write edge map to " << output_path << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Successfully wrote final Edge Map to " << output_path << std::endl;
    std::cout << "CED Baseline Phase 1 (Sequential) completely functional and mapped!" << std::endl;

    return EXIT_SUCCESS;
}
