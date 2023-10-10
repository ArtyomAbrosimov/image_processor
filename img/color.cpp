#include "color.h"

Color::Color(uint8_t new_blue, uint8_t new_green, uint8_t new_red) {
    SetColor(new_blue, new_green, new_red);
}

void Color::SetColor(uint8_t new_blue, uint8_t new_green, uint8_t new_red) {
    red = new_red;
    green = new_green;
    blue = new_blue;
}
