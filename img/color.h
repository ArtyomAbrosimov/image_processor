#pragma once

#include <cstdint>
#include <errno.h>

struct Color {
    Color() = default;
    Color(uint8_t new_blue, uint8_t new_green, uint8_t new_red);

    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;

    void SetColor(uint8_t new_blue, uint8_t new_green, uint8_t new_red);
};
