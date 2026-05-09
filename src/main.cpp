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
    
    // -- Stages 2-5 placeholdered for upcoming commits --
    // std::vector<float> blurred(num_pixels);
    // applyGaussianBlur(gray, blurred, width, height);    
    // std::vector<float> mag(num_pixels), dir(num_pixels);
    // applySobel(blurred, mag, dir, width, height);    
    // std::vector<float> nms(num_pixels);
    // applyNonMaximumSuppression(mag, dir, nms, width, height);    
    // std::vector<unsigned char> edges(num_pixels);
    // applyHysteresis(nms, edges, width, height, 50.0f, 20.0f);

    // For Initial Visual Testing: Cast float vector back to uint8 to save Grayscale
    std::vector<unsigned char> output_gray_img(num_pixels);
    for (int i = 0; i < num_pixels; ++i) {
        output_gray_img[i] = static_cast<unsigned char>(gray_float[i]);
    }

    // Save outputs using stbi_write
    int stride_in_bytes = width * 1; // 1 channel
    int write_success = stbi_write_png(output_path.c_str(), width, height, 1, output_gray_img.data(), stride_in_bytes);

    if (!write_success) {
        std::cerr << "Error: Could not write image to " << output_path << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Successfully wrote grayscale processing result to " << output_path << std::endl;
    std::cout << "CED Baseline processing complete!" << std::endl;

    return EXIT_SUCCESS;
}
