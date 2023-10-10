#include "magic.h"
#include "read.h"

read_and_write::Read::Read(const std::string &path) {
    path_ = path;
}

size_t read_and_write::Read::CharsToNumber(unsigned char *chars) {
    size_t res = *chars;
    for (size_t i = 0; i < magic::BYTES_OFFSETS.size(); ++i) {
        res += *(chars + i + 1) << magic::BYTES_OFFSETS[i];
    }
    return res;
}

const size_t read_and_write::GetPaddingSize(size_t width) {
    return (magic::MAX_PADDING - (width * magic::CNT_OF_PIX % magic::MAX_PADDING)) % magic::MAX_PADDING;
}

Img read_and_write::Read::ReadImg() {
    unsigned char in_file_header[magic::BMP_HEADER_SIZE];
    unsigned char in_info_file[magic::DIB_HEADER_SIZE];
    std::ifstream in_file;
    in_file.open(path_, std::ios::in | std::ios::binary);

    if (!in_file.is_open()) {
        throw std::invalid_argument("Can't open file " + path_);
    }
    if (errno == EACCES) {
        throw std::invalid_argument("No permission to file " + path_);
    }
    in_file.read(reinterpret_cast<char *>(in_file_header), magic::BMP_HEADER_SIZE);
    if (in_file_header[0] != magic::BMP_FORMAT[0] || in_file_header[1] != magic::BMP_FORMAT[1]) {
        throw std::invalid_argument("Format of " + path_ + " is not .bmp\n");
    }
    try {
        in_file.read(reinterpret_cast<char *>(in_info_file), magic::DIB_HEADER_SIZE);
        uint32_t width = CharsToNumber(in_info_file + magic::WIDTH_OFFSET);
        uint32_t height = CharsToNumber(in_info_file + magic::HEIGHT_OFFSET);
        Img img(width, height);
        std::vector<std::vector<Color>> image(height);
        std::vector<Color> tmp(width);
        for (size_t i = 0; i < height; ++i) {
            for (size_t j = 0; j < width; ++j) {
                unsigned char pix[magic::CNT_OF_PIX];
                in_file.read(reinterpret_cast<char *>(pix), magic::CNT_OF_PIX);
                tmp[j].SetColor(static_cast<uint8_t>(pix[0]), static_cast<uint8_t>(pix[1]),
                                static_cast<uint8_t>(pix[2]));
            }
            image[i] = tmp;
            in_file.ignore(static_cast<std::streamsize>(GetPaddingSize(width)));
        }
        img = Img(image);
        in_file.close();
        return img;
    } catch (std::exception &e) {
        throw "Error in reading: " + static_cast<std::string>(e.what());
    }
}
