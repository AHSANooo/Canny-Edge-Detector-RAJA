#ifndef HYSTERESIS_HPP
#define HYSTERESIS_HPP

#include <vector>

std::vector<unsigned char> applyHysteresis(const std::vector<float>& input_nms, int width, int height, float high_thresh, float low_thresh);

#endif
