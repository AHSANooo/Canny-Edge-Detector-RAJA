#ifndef SOBEL_HPP
#define SOBEL_HPP

#include <vector>

void applySobel(const std::vector<float>& input_blurred, std::vector<float>& output_mag, std::vector<float>& output_dir, int width, int height);

#endif
