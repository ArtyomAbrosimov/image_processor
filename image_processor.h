#pragma once

#include "img/filter.h"
#include "read_and_write/write.h"

#include <iostream>

const std::string INFO =
    "You need to enter by spaces:\n"
    "- path to input file;\n"
    "- path to output file;\n"
    "- filter names and parameters.\n"
    "\n"
    "Image processor has the following filters:\n"
    "- crop (-crop width height);\n"
    "- grayscale (-gs);\n"
    "- negative (-neg);\n"
    "- sharpening (-sharp);\n"
    "- edge detection (-edge threshold);\n"
    "- Gaussian blur (-blur sigma);\n"
    "- Pixelization (-pix size).";
