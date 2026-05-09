#include "hysteresis.hpp"

std::vector<unsigned char> applyHysteresis(const std::vector<float>& input_nms, int width, int height, float high_thresh, float low_thresh) {
    int num_pixels = width * height;
    std::vector<unsigned char> output_edges(num_pixels, 0);
    std::vector<unsigned char> state(num_pixels, 0); // 0 = Suppressed, 1 = Weak, 2 = Strong
    std::vector<int> strong_stack;

    // 1. Initial Pass: Classify pixels and seed the DFS stack
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            int i = y * width + x;
            float val = input_nms[i];
            
            if (val >= high_thresh) {
                state[i] = 2; // Strong edge
                strong_stack.push_back(i);
            } else if (val >= low_thresh) {
                state[i] = 1; // Weak edge
            }
        }
    }

    // Neighbor offsets for 8-connectivity dynamically resolved in a 1D mapping
    int neighbor_offsets[8] = {
        -width - 1, -width, -width + 1,
        -1,                 1,
         width - 1,  width,  width + 1
    };

    // 2. DFS Pass: Track edges by connecting weak components to strong components
    while (!strong_stack.empty()) {
        int idx = strong_stack.back();
        strong_stack.pop_back();

        // Final output confirmation
        output_edges[idx] = 255;

        // Decode bounds to restrict neighbor exploration safely
        int x = idx % width;
        int y = idx / width;

        if (y > 0 && y < height - 1 && x > 0 && x < width - 1) {
            for (int i = 0; i < 8; ++i) {
                int n_idx = idx + neighbor_offsets[i];
                if (state[n_idx] == 1) { // If Weak neighbor
                    state[n_idx] = 2;    // Promote to Strong
                    strong_stack.push_back(n_idx);
                }
            }
        }
    }

    // Unconnected Weak components remaining implicitly map as 0 natively
    return output_edges;
}
