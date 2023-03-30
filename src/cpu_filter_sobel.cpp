/**
 * @file cpu_filter_sobel.cpp
 * @author Egor Orachev
 * @date 3/30/2023
 */

#include "cpu_filter_sobel.hpp"

#include "cpu_conv.hpp"

#include <cmath>

namespace gpusandbox {

    bool cpu_filter_sobel::prepare() {
        return true;
    }
    bool cpu_filter_sobel::execute() {
        float filter_kernel1[3][3] = {1.0f, 0.0f, -1.0f,
                                      2.0f, 0.0f, -2.0f,
                                      1.0f, 0.0f, -1.0f};

        float filter_kernel2[3][3] = {1.0f, 2.0f, 1.0f,
                                      0.0f, 0.0f, 0.0f,
                                      -1.0f, -2.0f, -1.0f};

        auto merge = [](float x, float y) { return std::sqrt(x * x + y * y); };

        cpu_conv2<3> conv;
        conv(*m_input, *m_output, merge, filter_kernel1, filter_kernel2);
        return true;
    }

}// namespace gpusandbox