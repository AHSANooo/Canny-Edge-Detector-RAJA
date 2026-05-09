#include <iostream>
#include <vector>
#include "grayscale.hpp"
#include "gaussian.hpp"
#include "sobel.hpp"
#include "nms.hpp"
#include "hysteresis.hpp"

// RAJA, Umpire, and CHAI includes will be necessary in future phases.
#include "RAJA/RAJA.hpp"
#include "umpire/Umpire.hpp"
#include "chai/ManagedArray.hpp"

int main(int argc, char** argv) {
    std::cout << "Starting Canny Edge Detector (Sequential Baseline)" << std::endl;

    int width = 512;
    int height = 512;
    size_t num_pixels = width * height;

    std::vector<unsigned char> input_rgb(num_pixels * 3, 200);
    std::vector<unsigned char> gray(num_pixels);
    std::vector<float> blurred(num_pixels);
    std::vector<float> mag(num_pixels);
    std::vector<float> dir(num_pixels);
    std::vector<float> nms(num_pixels);
    std::vector<unsigned char> edges(num_pixels);

    // 1. Grayscale
    convertToGrayscale(input_rgb, gray, width, height);
    
    // 2. Gaussian Blur
    applyGaussianBlur(gray, blurred, width, height);
    
    // 3. Sobel
    applySobel(blurred, mag, dir, width, height);
    
    // 4. Non-Maximum Suppression
    applyNonMaximumSuppression(mag, dir, nms, width, height);
    
    // 5. Hysteresis
    applyHysteresis(nms, edges, width, height, 50.0f, 20.0f);

    std::cout << "CED Baseline processing complete!" << std::endl;
    return 0;
}
