#include "write.h"

read_and_write::Write::Write(std::string path) {
    path_ = path;
}

void read_and_write::Write::WriteBMPHeader(unsigned char* bmp_header, size_t size) {
    bmp_header[magic::FORMAT_OFFSET_FIRST] = magic::BMP_FORMAT[magic::FORMAT_OFFSET_FIRST];
    bmp_header[magic::FORMAT_OFFSET_SECOND] = magic::BMP_FORMAT[magic::FORMAT_OFFSET_SECOND];
    NumbToChars(size, bmp_header + magic::SIZE_OF_FILE_OFFSET);
    bmp_header[magic::HEADER_SIZE_OFFSET] = magic::BMP_HEADER_SIZE + magic::DIB_HEADER_SIZE;
}

void read_and_write::Write::WriteDIBHeader(unsigned char* dib_header, size_t width, size_t height) {
    dib_header[magic::DIB_SIZE_OFFSET] = magic::DIB_HEADER_SIZE;
    NumbToChars(width, dib_header + magic::WIDTH_OFFSET);
    NumbToChars(height, dib_header + magic::HEIGHT_OFFSET);
    dib_header[magic::NUMB_OF_COLOR_PLANE_OFFSET] = magic::NUMB_OF_COLOR_PLANE;
    dib_header[magic::CNT_OF_BITS_IN_PIX_OFFSET] = magic::CNT_OF_BITS_IN_PIX;
    dib_header[magic::BITMAP_DATA_OFFSET] = (width + GetPaddingSize(width)) * height;
}

void read_and_write::Write::WriteImg(const Img& img) {
    std::ofstream out_file;
    out_file.open(path_, std::ios::out | std::ios::binary);
    if (!out_file.is_open()) {
        throw std::invalid_argument("Can't open file " + path_);
    }
    const size_t size_of_file = magic::DIB_HEADER_SIZE + magic::BMP_HEADER_SIZE +
                                img.GetWidth() * img.GetHeight() * magic::CNT_OF_PIX +
                                GetPaddingSize(img.GetWidth()) * img.GetHeight();

    unsigned char bmp_header[magic::BMP_HEADER_SIZE] = {};
    std::fill(bmp_header, bmp_header + magic::BMP_HEADER_SIZE, 0);
    WriteBMPHeader(bmp_header, size_of_file);

    unsigned char dib_header[magic::DIB_HEADER_SIZE] = {};
    std::fill(dib_header, dib_header + magic::DIB_HEADER_SIZE, 0);
    WriteDIBHeader(dib_header, img.GetWidth(), img.GetHeight());

    out_file.write(reinterpret_cast<char*>(bmp_header), magic::BMP_HEADER_SIZE);
    out_file.write(reinterpret_cast<char*>(dib_header), magic::DIB_HEADER_SIZE);

    unsigned char pix_bmp[magic::CNT_OF_PIX] = {0, 0, 0};
    for (size_t i = 0; i < img.GetHeight(); ++i) {
        for (size_t j = 0; j < img.GetWidth(); ++j) {
            unsigned char blue = static_cast<unsigned char>(img.GetColor(j, i).blue);
            unsigned char green = static_cast<unsigned char>(img.GetColor(j, i).green);
            unsigned char red = static_cast<unsigned char>(img.GetColor(j, i).red);
            unsigned char pix[] = {blue, green, red};
            out_file.write(reinterpret_cast<char*>(pix), magic::CNT_OF_PIX);
        }
        out_file.write(reinterpret_cast<char*>(pix_bmp), static_cast<std::streamsize>(GetPaddingSize(img.GetWidth())));
    }
    out_file.close();
}

void read_and_write::Write::NumbToChars(size_t numb, unsigned char* chars) {
    *chars = numb;
    for (size_t i = 0; i < magic::BYTES_OFFSETS.size(); ++i) {
        *(chars + i + 1) = numb >> magic::BYTES_OFFSETS[i];
    }
}
