#ifndef NMS_HPP
#define NMS_HPP

#include <vector>

void applyNonMaximumSuppression(const std::vector<float>& mag, const std::vector<float>& dir, std::vector<float>& output_nms, int width, int height);

#endif
