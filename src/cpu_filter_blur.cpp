/**
 * @file cpu_filter_blur.cpp
 * @author Egor Orachev
 * @date 3/30/2023
 */

#include "cpu_filter_blur.hpp"

#include "cpu_conv.hpp"

#include <iostream>

namespace gpusandbox {

    bool cpu_filter_blur::prepare() {
        return true;
    }
    bool cpu_filter_blur::execute(const image& input, image& output, cxxopts::ParseResult& args) {
        int size = args["conv_size"].as<int>();

        if (size == 3) {
            float filter_kernel[3][3] = {0.0f, 1.0f, 0.0f,
                                         1.0f, 1.0f, 1.0f,
                                         0.0f, 1.0f, 0.0f};

            cpu_conv<3> conv;
            conv(input, output, 5.0f, filter_kernel);
            return true;
        }
        if (size == 5) {
            float filter_kernel[5][5] = {0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                                         0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                                         1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
                                         0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                                         0.0f, 0.0f, 1.0f, 0.0f, 0.0f};

            cpu_conv<5> conv;
            conv(input, output, 13.0f, filter_kernel);
            return true;
        }
        if (size == 7) {
            float filter_kernel[7][7] = {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                                         0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
                                         0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                                         1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
                                         0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                                         0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
                                         0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f};

            cpu_conv<7> conv;
            conv(input, output, 25.0f, filter_kernel);
            return true;
        }

        std::cerr << "no filter size " << size;
        return false;
    }
}// namespace gpusandbox