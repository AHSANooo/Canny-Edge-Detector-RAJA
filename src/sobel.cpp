#include "sobel.hpp"
#include <cmath>

void applySobel(const std::vector<float>& input_blurred, std::vector<float>& output_mag, std::vector<float>& output_dir, int width, int height) {
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            float gx = 0.0f; // Compute Gx
            float gy = 0.0f; // Compute Gy
            // magnitude & direction
            output_mag[y * width + x] = std::sqrt(gx*gx + gy*gy);
            output_dir[y * width + x] = std::atan2(gy, gx);
        }
    }
}
