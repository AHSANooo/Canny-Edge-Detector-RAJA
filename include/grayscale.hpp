#ifndef GRAYSCALE_HPP
#define GRAYSCALE_HPP

#include <vector>

void convertToGrayscale(const std::vector<unsigned char>& input_rgb, std::vector<unsigned char>& output_gray, int width, int height);

#endif
