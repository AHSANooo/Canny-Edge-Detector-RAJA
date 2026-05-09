#ifndef GAUSSIAN_HPP
#define GAUSSIAN_HPP

#include <vector>

std::vector<float> applyGaussianBlur(const std::vector<float>& input_gray, int width, int height, float sigma);

#endif
