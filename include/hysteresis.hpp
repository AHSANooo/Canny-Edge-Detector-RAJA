#ifndef HYSTERESIS_HPP
#define HYSTERESIS_HPP

#include <vector>

void applyHysteresis(const std::vector<float>& input_nms, std::vector<unsigned char>& output_edges, int width, int height, float high_thresh, float low_thresh);

#endif
