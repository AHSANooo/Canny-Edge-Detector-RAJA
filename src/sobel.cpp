#include "sobel.hpp"
#include <cmath>

std::pair<std::vector<float>, std::vector<float>> applySobel(const std::vector<float>& input_blurred, int width, int height) {
    int num_pixels = width * height;
    std::vector<float> output_mag(num_pixels, 0.0f);
    std::vector<float> output_dir(num_pixels, 0.0f);

    // Sobel kernels:
    // Gx:
    // -1  0  1
    // -2  0  2
    // -1  0  1
    // Gy:
    //  1  2  1
    //  0  0  0
    // -1 -2 -1

    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            float gx = -1.0f * input_blurred[(y - 1) * width + (x - 1)] + 1.0f * input_blurred[(y - 1) * width + (x + 1)]
                     - 2.0f * input_blurred[y * width + (x - 1)]       + 2.0f * input_blurred[y * width + (x + 1)]
                     - 1.0f * input_blurred[(y + 1) * width + (x - 1)] + 1.0f * input_blurred[(y + 1) * width + (x + 1)];

            float gy =  1.0f * input_blurred[(y - 1) * width + (x - 1)] + 2.0f * input_blurred[(y - 1) * width + x] + 1.0f * input_blurred[(y - 1) * width + (x + 1)]
                     - 1.0f * input_blurred[(y + 1) * width + (x - 1)] - 2.0f * input_blurred[(y + 1) * width + x] - 1.0f * input_blurred[(y + 1) * width + (x + 1)];

            output_mag[y * width + x] = std::sqrt(gx * gx + gy * gy);
            
            // atan2 returns [-pi, pi]. Convert to degrees.
            float angle = std::atan2(gy, gx) * (180.0f / M_PI);
            output_dir[y * width + x] = angle;
        }
    }

    return {output_mag, output_dir};
}
