#pragma once

#include "magic.h"
#include "read.h"
#include "../img/img.h"

#include <algorithm>
#include <fstream>
#include <string>

namespace read_and_write {
class Write {
public:
    explicit Write(std::string path);
    void WriteImg(const Img& img);

private:
    void WriteBMPHeader(unsigned char* bmp_header, size_t size);
    void WriteDIBHeader(unsigned char* dib_header, size_t width, size_t height);
    void NumbToChars(size_t numb, unsigned char* chars);
    std::string path_;
};
}  // namespace read_and_write
