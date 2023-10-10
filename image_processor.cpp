#include "image_processor.h"

int main(int argc, char** argv) {
    read_and_write::Read input_file = read_and_write::Read(argv[1]);
    Img input_img = input_file.ReadImg();
    Img output_img(input_img);
    int no_read = 0;
    for (size_t i = 3; i < argc; ++i) {
        if (!no_read) {
            if (std::string(argv[i]) == "-crop") {
                if (i + 2 >= argc) {
                    throw std::invalid_argument("Not enough parameters for filter");
                }
                filter::Crop f(atoll(argv[i + 1]), atoll(argv[i + 2]));
                output_img = f.App(output_img);
                no_read = 2;
            } else if (std::string(argv[i]) == "-gs") {
                filter::Grayscale f;
                output_img = f.App(output_img);
            } else if (std::string(argv[i]) == "-neg") {
                filter::Negative f;
                output_img = f.App(output_img);
            } else if (std::string(argv[i]) == "-sharp") {
                filter::Sharpening f;
                output_img = f.App(output_img);
            } else if (std::string(argv[i]) == "-edge") {
                if (i + 1 >= argc) {
                    throw std::invalid_argument("Not enough parameters for filter");
                }
                filter::EdgeDetection f(std::stod(argv[i + 1]));
                output_img = f.App(output_img);
                no_read = 1;
            } else if (std::string(argv[i]) == "-blur") {
                if (i + 1 >= argc) {
                    throw std::invalid_argument("Not enough parameters for filter");
                }
                filter::GaussianBlur f(std::stod(argv[i + 1]));
                output_img = f.App(output_img);
                no_read = 1;
            } else if (std::string(argv[i]) == "-pix") {
                if (i + 1 >= argc) {
                    throw std::invalid_argument("Not enough parameters for filter");
                }
                filter::Pixelization f(atoll(argv[i + 1]));
                output_img = f.App(output_img);
                no_read = 1;
            } else {
                throw std::invalid_argument("Incorrect filter call");
            }
        } else {
            --no_read;
        }
    }
    read_and_write::Write output = read_and_write::Write(argv[2]);
    output.WriteImg(output_img);
    return 0;
}
