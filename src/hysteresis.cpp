#include "hysteresis.hpp"

void applyHysteresis(const std::vector<float>& input_nms, std::vector<unsigned char>& output_edges, int width, int height, float high_thresh, float low_thresh) {
    for (int i = 0; i < width * height; ++i) {
        if (input_nms[i] >= high_thresh) {
            output_edges[i] = 255;
        } else if (input_nms[i] < low_thresh) {
            output_edges[i] = 0;
        } else {
            output_edges[i] = 127; // weak edge
        }
    }
}
