#include "gaussian.hpp"
#include <cmath>
#include <algorithm>

std::vector<float> applyGaussianBlur(const std::vector<float>& input_gray, int width, int height, float sigma) {
    int radius = static_cast<int>(std::ceil(3.0f * sigma));
    int kernel_size = 2 * radius + 1;
    std::vector<float> kernel(kernel_size);
    
    // Dynamically generate the 1D Gaussian kernel
    float sum_weight = 0.0f;
    for (int i = -radius; i <= radius; ++i) {
        float weight = std::exp(-(i * i) / (2.0f * sigma * sigma)) / std::sqrt(2.0f * M_PI * sigma * sigma);
        kernel[i + radius] = weight;
        sum_weight += weight;
    }
    
    // Normalize the kernel
    for (int i = 0; i < kernel_size; ++i) {
        kernel[i] /= sum_weight;
    }

    int num_pixels = width * height;
    std::vector<float> intermediate(num_pixels, 0.0f);
    std::vector<float> output_blurred(num_pixels, 0.0f);

    // Horizontal Pass (separable convolution)
    // These outer loops are structured identically to be cleanly convertible into RAJA::forall statements
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float sum = 0.0f;
            for (int k = -radius; k <= radius; ++k) {
                // Gracefully clamp X boundaries (halo handling)
                int col = std::max(0, std::min(width - 1, x + k));
                sum += input_gray[y * width + col] * kernel[k + radius];
            }
            intermediate[y * width + x] = sum;
        }
    }

    // Vertical Pass (separable convolution)
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float sum = 0.0f;
            for (int k = -radius; k <= radius; ++k) {
                // Gracefully clamp Y boundaries (halo handling)
                int row = std::max(0, std::min(height - 1, y + k));
                sum += intermediate[row * width + x] * kernel[k + radius];
            }
            output_blurred[y * width + x] = sum;
        }
    }

    return output_blurred;
}
