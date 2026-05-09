#include "grayscale.hpp"

void convertToGrayscale(const std::vector<unsigned char>& input_rgb, std::vector<unsigned char>& output_gray, int width, int height) {
    for (int i = 0; i < width * height; ++i) {
        // Simple luminance formula: Y = 0.299*R + 0.587*G + 0.114*B
        unsigned char r = input_rgb[3 * i];
        unsigned char g = input_rgb[3 * i + 1];
        unsigned char b = input_rgb[3 * i + 2];
        output_gray[i] = static_cast<unsigned char>(0.299 * r + 0.587 * g + 0.114 * b);
    }
}
