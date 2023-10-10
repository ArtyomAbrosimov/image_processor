#pragma once

#include <vector>

namespace magic {
const int BMP_HEADER_SIZE = 14;
const int DIB_HEADER_SIZE = 40;
const int WIDTH_OFFSET = 4;
const int HEIGHT_OFFSET = 8;
const int MAX_PADDING = 4;
const int CNT_OF_PIX = 3;
const int FORMAT_OFFSET_FIRST = 0;
const int FORMAT_OFFSET_SECOND = 1;
const int SIZE_OF_FILE_OFFSET = 2;
const int HEADER_SIZE_OFFSET = 10;
const int DIB_SIZE_OFFSET = 0;
const int NUMB_OF_COLOR_PLANE_OFFSET = 12;
const int NUMB_OF_COLOR_PLANE = 1;
const int CNT_OF_BITS_IN_PIX = 24;
const int CNT_OF_BITS_IN_PIX_OFFSET = 14;
const int BITMAP_DATA_OFFSET = 20;

const std::vector<char> BMP_FORMAT = {'B', 'M'};
const std::vector<int> BYTES_OFFSETS = {8, 16, 24};
}  // namespace magic
