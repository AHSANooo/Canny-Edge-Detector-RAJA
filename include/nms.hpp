#ifndef NMS_HPP
#define NMS_HPP

#include <vector>

std::vector<float> applyNonMaximumSuppression(const std::vector<float>& mag, const std::vector<float>& dir, int width, int height);

#endif
