#include "filter.h"

std::vector<int> filter::Filter::AppMatrix(const std::vector<std::vector<int>>& matrix, const Img& img, size_t x,
                                           size_t y) const {
    int red = 0;
    int green = 0;
    int blue = 0;
    size_t center_x = matrix.size() / 2;
    size_t center_y = matrix.front().size() / 2;
    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < matrix.front().size(); ++j) {
            red += img.GetColor(std::clamp(static_cast<ssize_t>(y - center_y + j), static_cast<ssize_t>(0),
                                           static_cast<ssize_t>(img.GetWidth() - 1)),
                                std::clamp(static_cast<ssize_t>(x - center_x + i), static_cast<ssize_t>(0),
                                           static_cast<ssize_t>(img.GetHeight() - 1)))
                       .red *
                   matrix[i][j];
            green += img.GetColor(std::clamp(static_cast<ssize_t>(y - center_y + j), static_cast<ssize_t>(0),
                                             static_cast<ssize_t>(img.GetWidth() - 1)),
                                  std::clamp(static_cast<ssize_t>(x - center_x + i), static_cast<ssize_t>(0),
                                             static_cast<ssize_t>(img.GetHeight() - 1)))
                         .green *
                     matrix[i][j];
            blue += img.GetColor(std::clamp(static_cast<ssize_t>(y - center_y + j), static_cast<ssize_t>(0),
                                            static_cast<ssize_t>(img.GetWidth() - 1)),
                                 std::clamp(static_cast<ssize_t>(x - center_x + i), static_cast<ssize_t>(0),
                                            static_cast<ssize_t>(img.GetHeight() - 1)))
                        .blue *
                    matrix[i][j];
        }
    }
    red = std::clamp(red, 0, static_cast<int>(magic::MAX_COLOR));
    green = std::clamp(green, 0, static_cast<int>(magic::MAX_COLOR));
    blue = std::clamp(blue, 0, static_cast<int>(magic::MAX_COLOR));
    return {red, green, blue};
}

filter::Crop::Crop(size_t width, size_t height) : width_(width), height_(height) {
}

Img filter::Crop::App(const Img& img) const {
    size_t min_width = (width_ <= img.GetWidth() ? width_ : img.GetWidth());
    size_t min_height = (height_ <= img.GetHeight() ? height_ : img.GetHeight());
    Img new_image(min_width, min_height);
    size_t delta_height = img.GetHeight() - min_height;
    for (size_t i = 0; i < min_width; ++i) {
        for (size_t j = 0; j < min_height; ++j) {
            new_image.SetColor(i, j, img.GetColor(i, j + delta_height));
        }
    }
    return new_image;
}

Img filter::Grayscale::App(const Img& img) const {
    Img new_img(img.GetWidth(), img.GetHeight());
    for (size_t i = 0; i < img.GetWidth(); ++i) {
        for (size_t j = 0; j < img.GetHeight(); ++j) {
            Color color = img.GetColor(i, j);
            double double_color =
                magic::GRAY_RED * color.red + magic::GRAY_GREEN * color.green + magic::GRAY_BLUE * color.blue;
            new_img.SetColor(i, j,
                             {static_cast<uint8_t>(double_color), static_cast<uint8_t>(double_color),
                              static_cast<uint8_t>(double_color)});
        }
    }
    return new_img;
}

Img filter::Negative::App(const Img& img) const {
    Img new_img(img.GetWidth(), img.GetHeight());
    for (size_t i = 0; i < img.GetWidth(); ++i) {
        for (size_t j = 0; j < img.GetHeight(); ++j) {
            Color color = img.GetColor(i, j);
            uint8_t new_red = magic::MAX_COLOR - color.red;
            uint8_t new_green = magic::MAX_COLOR - color.green;
            uint8_t new_blue = magic::MAX_COLOR - color.blue;
            new_img.SetColor(i, j, {new_blue, new_green, new_red});
        }
    }
    return new_img;
}

Img filter::Sharpening::App(const Img& img) const {
    std::vector<std::vector<Color>> res;
    for (size_t i = 0; i < img.GetHeight(); ++i) {
        std::vector<Color> tmp(img.GetWidth());
        for (size_t j = 0; j < img.GetWidth(); ++j) {
            std::vector<int> colors_of_pix = AppMatrix(magic::SHARP_MATRIX, img, i, j);
            tmp[j] = {static_cast<uint8_t>(colors_of_pix[2]), static_cast<uint8_t>(colors_of_pix[1]),
                      static_cast<uint8_t>(colors_of_pix[0])};
        }
        res.push_back(tmp);
    }
    return Img{res};
}

filter::EdgeDetection::EdgeDetection(double threshold) : threshold_(threshold * magic::MAX_COLOR) {
}

Img filter::EdgeDetection::App(const Img& img) const {
    Img gray_img = filter::Grayscale().App(img);
    std::vector<std::vector<Color>> res;
    for (size_t i = 0; i < gray_img.GetHeight(); ++i) {
        std::vector<Color> tmp(gray_img.GetWidth());
        for (size_t j = 0; j < gray_img.GetWidth(); ++j) {
            std::vector<int> colors_of_pix = AppMatrix(magic::EDGE_MATRIX, gray_img, i, j);
            if (static_cast<double>(colors_of_pix.front()) > threshold_) {
                tmp[j] = {magic::MAX_COLOR, magic::MAX_COLOR, magic::MAX_COLOR};
            } else {
                tmp[j] = {0, 0, 0};
            }
        }
        res.push_back(tmp);
    }
    return Img{res};
}

filter::GaussianBlur::GaussianBlur(double sigma) {
    one_dim_size_ = std::ceil(sigma * magic::GAUSS_DIAMETER);
    one_dim_ = std::vector<double>(one_dim_size_);
    const size_t g_radius = one_dim_size_ / 2;
    const double pow_sigma = 2 * sigma * sigma;

    for (size_t i = 0; i < one_dim_size_; ++i) {
        one_dim_[i] = std::exp(-(static_cast<double>(i) - static_cast<double>(g_radius)) *
                               (static_cast<double>(i) - static_cast<double>(g_radius)) / pow_sigma);
    }

    for (size_t i = 0; i < one_dim_size_; ++i) {
        one_dim_[i] /= std::sqrt(pow_sigma * M_PI);
    }
}

Img filter::GaussianBlur::App(const Img& img) const {
    std::vector<std::vector<Color>> tmp(img.GetHeight(), std::vector<Color>(img.GetWidth()));
    const size_t g_radius = one_dim_size_ / 2;

    for (size_t i = 0; i < img.GetHeight(); ++i) {
        for (size_t j = 0; j < img.GetWidth(); ++j) {
            double r_sum = 0;
            double g_sum = 0;
            double b_sum = 0;
            for (size_t k = 0; k < one_dim_size_; ++k) {
                if (j + k - g_radius >= 0 && j + k - g_radius < img.GetWidth()) {
                    const Color& p = img.GetColor(j + k - g_radius, i);
                    const double& elem = one_dim_[k];
                    r_sum += p.red * elem;
                    g_sum += p.green * elem;
                    b_sum += p.blue * elem;
                }
            }
            tmp[i][j] = {std::clamp(static_cast<uint8_t>(b_sum), static_cast<uint8_t>(0), magic::MAX_COLOR),
                         std::clamp(static_cast<uint8_t>(g_sum), static_cast<uint8_t>(0), magic::MAX_COLOR),
                         std::clamp(static_cast<uint8_t>(r_sum), static_cast<uint8_t>(0), magic::MAX_COLOR)};
        }
    }
    Img image(tmp);

    for (size_t j = 0; j < img.GetWidth(); ++j) {
        for (size_t i = 0; i < img.GetHeight(); ++i) {
            double r_sum = 0;
            double g_sum = 0;
            double b_sum = 0;
            for (size_t k = 0; k < one_dim_size_; ++k) {
                if (i + k - g_radius >= 0 && i + k - g_radius < image.GetHeight()) {
                    const Color& p = image.GetColor(j, i + k - g_radius);
                    const double& elem = one_dim_[k];
                    r_sum += p.red * elem;
                    g_sum += p.green * elem;
                    b_sum += p.blue * elem;
                }
            }
            tmp[i][j] = {std::clamp(static_cast<uint8_t>(b_sum), static_cast<uint8_t>(0), magic::MAX_COLOR),
                         std::clamp(static_cast<uint8_t>(g_sum), static_cast<uint8_t>(0), magic::MAX_COLOR),
                         std::clamp(static_cast<uint8_t>(r_sum), static_cast<uint8_t>(0), magic::MAX_COLOR)};
        }
    }
    return Img(tmp);
}

filter::Pixelization::Pixelization(size_t pixel_size) : pixel_size_(pixel_size) {
}

Img filter::Pixelization::App(const Img& img) const {
    size_t width_offset = img.GetWidth() % pixel_size_ / 2;
    size_t height_offset = img.GetHeight() % pixel_size_ / 2;
    Img new_image(img.GetWidth(), img.GetHeight());
    for (size_t j = width_offset; j < img.GetWidth() + pixel_size_; j += pixel_size_) {
        for (size_t i = height_offset; i < img.GetHeight() + pixel_size_; i += pixel_size_) {
            size_t cnt_of_pix = 0;
            size_t red = 0;
            size_t green = 0;
            size_t blue = 0;
            for (size_t x = (static_cast<ssize_t>(j - pixel_size_) > 0 ? j - pixel_size_ : 0);
                 x < (j < img.GetWidth() ? j : img.GetWidth()); ++x) {
                for (size_t y = (static_cast<ssize_t>(i - pixel_size_) > 0 > 0 ? i - pixel_size_ : 0);
                     y < (i < img.GetHeight() ? i : img.GetHeight()); ++y) {
                    ++cnt_of_pix;
                    red += img.GetColor(x, y).red;
                    green += img.GetColor(x, y).green;
                    blue += img.GetColor(x, y).blue;
                }
            }
            red /= cnt_of_pix;
            green /= cnt_of_pix;
            blue /= cnt_of_pix;
            for (size_t x = (static_cast<ssize_t>(j - pixel_size_) > 0 > 0 ? j - pixel_size_ : 0);
                 x < (j < img.GetWidth() ? j : img.GetWidth()); ++x) {
                for (size_t y = (static_cast<ssize_t>(i - pixel_size_) > 0 > 0 ? i - pixel_size_ : 0);
                     y < (i < img.GetHeight() ? i : img.GetHeight()); ++y) {
                    new_image.SetColor(
                        x, y, {static_cast<uint8_t>(blue), static_cast<uint8_t>(green), static_cast<uint8_t>(red)});
                }
            }
        }
    }
    return new_image;
}
