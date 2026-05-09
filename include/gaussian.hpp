#ifndef GAUSSIAN_HPP
#define GAUSSIAN_HPP

#include <vector>

void applyGaussianBlur(const std::vector<unsigned char>& input_gray, std::vector<float>& output_blurred, int width, int height);

#endif
