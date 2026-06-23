#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <vector>

int main() {
    int w, h, channels;
    unsigned char *img = stbi_load("assets/sprites/ui/idle_down.png", &w, &h, &channels, 4);
    if (!img) {
        std::cerr << "Failed to load image\n";
        return 1;
    }
    std::cout << "Size: " << w << "x" << h << "\n";

    std::vector<int> cols;
    for (int x = 0; x < w; ++x) {
        bool has_pixel = false;
        for (int y = 0; y < h; ++y) {
            unsigned char alpha = img[(y * w + x) * 4 + 3];
            if (alpha > 0) {
                has_pixel = true;
                break;
            }
        }
        if (has_pixel) cols.push_back(x);
    }

    if (cols.empty()) {
        std::cout << "Empty\n";
    } else {
        int start = cols[0];
        int prev = start;
        int count = 0;
        for (size_t i = 1; i < cols.size(); ++i) {
            if (cols[i] != prev + 1) {
                std::cout << "Segment: " << start << " to " << prev << " (width " << (prev - start + 1) << ")\n";
                start = cols[i];
                count++;
            }
            prev = cols[i];
        }
        std::cout << "Segment: " << start << " to " << prev << " (width " << (prev - start + 1) << ")\n";
        count++;
        std::cout << "Total segments: " << count << "\n";
    }
    stbi_image_free(img);
    return 0;
}
