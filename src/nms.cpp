#include "nms.hpp"

void applyNonMaximumSuppression(const std::vector<float>& mag, const std::vector<float>& dir, std::vector<float>& output_nms, int width, int height) {
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            output_nms[y * width + x] = mag[y * width + x]; // placeholder for actual suppression
        }
    }
}
