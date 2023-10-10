#pragma once

#include "../img/img.h"

#include <errno.h>
#include <fstream>

namespace read_and_write {
class Read {
public:
    explicit Read(const std::string& path);
    Img ReadImg();

private:
    std::string path_;
    size_t CharsToNumber(unsigned char* chars);
};

const size_t GetPaddingSize(size_t width);
}  // namespace read_and_write
