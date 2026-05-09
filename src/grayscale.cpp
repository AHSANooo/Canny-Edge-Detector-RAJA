#include "grayscale.hpp"

std::vector<float> convertToGrayscale(const std::vector<unsigned char>& input_rgb, int width, int height) {
    int num_pixels = width * height;
    std::vector<float> output_gray(num_pixels);

    // Clean, decoupled for-loop structured for later transformation into RAJA::forall
    for (int i = 0; i < num_pixels; ++i) {
        unsigned char r = input_rgb[3 * i];
        unsigned char g = input_rgb[3 * i + 1];
        unsigned char b = input_rgb[3 * i + 2];
        
        // Exact luminosity formula (using floating-point for accuracy in gradient math later)
        output_gray[i] = 0.299f * r + 0.587f * g + 0.114f * b;
    }

    return output_gray;
}

