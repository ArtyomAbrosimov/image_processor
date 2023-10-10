#pragma once

#include "img.h"
#include "magic.h"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <vector>

namespace filter {
class Filter {
public:
    Filter() = default;
    virtual ~Filter() = default;
    virtual Img App(const Img& img) const = 0;

protected:
    std::vector<int> AppMatrix(const std::vector<std::vector<int>>& matrix, const Img& img, size_t x, size_t y) const;
};

class Crop : public Filter {
public:
    Crop(size_t width, size_t height);
    Img App(const Img& img) const override;

private:
    size_t width_;
    size_t height_;
};

class Grayscale : public Filter {
public:
    Img App(const Img& img) const override;
};

class Negative : public Filter {
public:
    Img App(const Img& img) const override;
};

class EdgeDetection : public Filter {
public:
    explicit EdgeDetection(double threshold);
    Img App(const Img& img) const override;

private:
    double threshold_;
};

class Sharpening : public Filter {
public:
    Img App(const Img& img) const override;
};

class GaussianBlur : public Filter {
public:
    explicit GaussianBlur(double sigma);
    Img App(const Img& img) const override;

private:
    std::vector<double> one_dim_;
    int one_dim_size_;
};

class Pixelization : public Filter {
public:
    explicit Pixelization(size_t pixel_size);
    Img App(const Img& img) const override;

private:
    size_t pixel_size_;
};
}  // namespace filter
