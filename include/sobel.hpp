#ifndef SOBEL_HPP
#define SOBEL_HPP

#include <vector>
#include <utility>

std::pair<std::vector<float>, std::vector<float>> applySobel(const std::vector<float>& input_blurred, int width, int height);

#endif
