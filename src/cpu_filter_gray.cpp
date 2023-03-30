/**
 * @file cpu_filter_gray.cpp
 * @author Egor Orachev
 * @date 3/30/2023
 */

#include "cpu_filter_gray.hpp"

#include <cmath>

namespace gpusandbox {

    bool cpu_filter_gray::prepare() {
        return true;
    }
    bool cpu_filter_gray::execute() {
        int          w      = m_input->width();
        int          h      = m_input->height();
        const color* s_data = m_input->data().data();
        color*       d_data = m_output->data().data();

        color weight{0.299f, 0.587f, 0.114f, 0};

        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                int offset = i * w + j;

                color source = s_data[offset];
                color result;

                float gray = weight.r * source.r + weight.g * source.g + weight.b * source.b;

                result.r = gray;
                result.g = gray;
                result.b = gray;
                result.a = 1.0f;

                d_data[offset] = result;
            }
        }

        return true;
    }

}// namespace gpusandbox