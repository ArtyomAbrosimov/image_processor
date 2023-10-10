#pragma once

#include <stdint.h>
#include <vector>

namespace magic {
const uint8_t MAX_COLOR = 255;
const double GRAY_RED = 0.299;
const double GRAY_GREEN = 0.587;
const double GRAY_BLUE = 0.114;
const int GAUSS_DIAMETER = 6;
const std::vector<std::vector<int>> SHARP_MATRIX = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
const std::vector<std::vector<int>> EDGE_MATRIX = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};
}  // namespace magic
