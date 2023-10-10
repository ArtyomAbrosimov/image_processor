#pragma once

#include "color.h"

#include <stdexcept>
#include <string>
#include <vector>

class Img {
public:
    Img() = default;
    Img(size_t width, size_t height);
    explicit Img(const std::vector<std::vector<Color>>& image);
    Img(const Img& image);
    ~Img() = default;

    size_t GetWidth() const;
    size_t GetHeight() const;
    const Color& GetColor(size_t width, size_t height) const;

    void SetColor(size_t width, size_t height, Color color);

private:
    std::string path_;
    std::vector<std::vector<Color>> image_;
    size_t width_ = 0;
    size_t height_ = 0;

    void CheckSize(size_t width, size_t height) const;
};
