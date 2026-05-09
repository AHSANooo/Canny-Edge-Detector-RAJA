#include "gaussian.hpp"

void applyGaussianBlur(const std::vector<unsigned char>& input_gray, std::vector<float>& output_blurred, int width, int height) {
    // 5x5 Gaussian kernel approximation for baseline
    // Simplified for demonstration
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            output_blurred[y * width + x] = static_cast<float>(input_gray[y * width + x]); // replace with actual convolution
        }
    }
}
