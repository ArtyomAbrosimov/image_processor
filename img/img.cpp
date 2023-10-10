#include "img.h"

Img::Img(size_t width, size_t height) {
    width_ = width;
    height_ = height;
    image_ = std::vector<std::vector<Color>>(height_, std::vector<Color>(width_));
}

Img::Img(const std::vector<std::vector<Color>>& image) {
    image_ = image;
    width_ = image.front().size();
    height_ = image.size();
}

Img::Img(const Img& image) {
    this->path_ = image.path_;
    this->image_ = image.image_;
    this->width_ = image.width_;
    this->height_ = image.height_;
}

size_t Img::GetWidth() const {
    return width_;
}

size_t Img::GetHeight() const {
    return height_;
}

const Color& Img::GetColor(size_t width, size_t height) const {
    CheckSize(width, height);
    return image_[height][width];
}

void Img::SetColor(size_t width, size_t height, Color color) {
    CheckSize(width, height);
    image_[height][width] = color;
}

void Img::CheckSize(size_t width, size_t height) const {
    if (width >= width_ || height >= height_) {
        throw std::invalid_argument("Point outside the image");
    }
}
