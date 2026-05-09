#include "nms.hpp"
#include <cmath>

std::vector<float> applyNonMaximumSuppression(const std::vector<float>& mag, const std::vector<float>& dir, int width, int height) {
    int num_pixels = width * height;
    std::vector<float> output_nms(num_pixels, 0.0f);

    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            float angle = dir[y * width + x];
            
            // Normalize angle to [0, 180)
            if (angle < 0.0f) {
                angle += 180.0f;
            }

            float q_val = mag[y * width + x];
            float neighbor_1 = 0.0f;
            float neighbor_2 = 0.0f;

            // Quantize to 0, 45, 90, 135 degrees
            if ((angle >= 0.0f && angle < 22.5f) || (angle >= 157.5f && angle <= 180.0f)) {
                // 0 degrees: East and West
                neighbor_1 = mag[y * width + (x + 1)];
                neighbor_2 = mag[y * width + (x - 1)];
            } else if (angle >= 22.5f && angle < 67.5f) {
                // 45 degrees: North-East and South-West
                neighbor_1 = mag[(y - 1) * width + (x + 1)];
                neighbor_2 = mag[(y + 1) * width + (x - 1)];
            } else if (angle >= 67.5f && angle < 112.5f) {
                // 90 degrees: North and South
                neighbor_1 = mag[(y - 1) * width + x];
                neighbor_2 = mag[(y + 1) * width + x];
            } else if (angle >= 112.5f && angle < 157.5f) {
                // 135 degrees: North-West and South-East
                neighbor_1 = mag[(y - 1) * width + (x - 1)];
                neighbor_2 = mag[(y + 1) * width + (x + 1)];
            }

            // Suppress non-maximums
            if (q_val >= neighbor_1 && q_val >= neighbor_2) {
                output_nms[y * width + x] = q_val;
            } else {
                output_nms[y * width + x] = 0.0f;
            }
        }
    }

    return output_nms;
}
