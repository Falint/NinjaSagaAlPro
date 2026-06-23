#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

void find_diff(const std::string& base_path, const std::string& hover_path, int index) {
    int w1, h1, c1;
    int w2, h2, c2;
    unsigned char* img1 = stbi_load(base_path.c_str(), &w1, &h1, &c1, 4);
    unsigned char* img2 = stbi_load(hover_path.c_str(), &w2, &h2, &c2, 4);
    if (!img1 || !img2) {
        std::cerr << "Failed to load " << base_path << " or " << hover_path << "\n";
        return;
    }
    if (w1 != w2 || h1 != h2) {
        std::cerr << "Dimensions mismatch\n";
        stbi_image_free(img1);
        stbi_image_free(img2);
        return;
    }

    int min_x = w1, max_x = -1, min_y = h1, max_y = -1;
    for (int y = 0; y < h1; ++y) {
        for (int x = 0; x < w1; ++x) {
            int idx = (y * w1 + x) * 4;
            bool diff = false;
            for (int c = 0; c < 4; ++c) {
                if (img1[idx + c] != img2[idx + c]) {
                    diff = true;
                    break;
                }
            }
            if (diff) {
                min_x = std::min(min_x, x);
                max_x = std::max(max_x, x);
                min_y = std::min(min_y, y);
                max_y = std::max(max_y, y);
            }
        }
    }

    if (max_x == -1) {
        std::cout << "Frame " << index << ": No difference\n";
    } else {
        int width = max_x - min_x + 1;
        int height = max_y - min_y + 1;
        float rx = (float)min_x / w1;
        float ry = (float)min_y / h1;
        float rw = (float)width / w1;
        float rh = (float)height / h1;
        std::cout << "Frame " << index << " diff bounding box:\n";
        std::cout << "  Pixels: X=" << min_x << ", Y=" << min_y << ", W=" << width << ", H=" << height << "\n";
        std::cout << "  Ratios: X_Ratio=" << rx << "f, Y_Ratio=" << ry << "f, Width_Ratio=" << rw << "f, Height_Ratio=" << rh << "f\n";
    }

    stbi_image_free(img1);
    stbi_image_free(img2);
}

int main() {
    std::string base = "assets/sprites/ui/missionrank1.png";
    for (int i = 2; i <= 6; ++i) {
        std::string hover = "assets/sprites/ui/missionrank" + std::to_string(i) + ".png";
        find_diff(base, hover, i);
    }
    return 0;
}
