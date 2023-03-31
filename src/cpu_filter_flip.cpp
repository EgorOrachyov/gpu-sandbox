/**
 * @file cpu_filter_flip.cpp
 * @author Egor Orachev
 * @date 3/29/2023
 */

#include "cpu_filter_flip.hpp"

namespace gpusandbox {

    bool cpu_filter_flip_x::execute() {
        int          w      = m_input->width();
        int          h      = m_input->height();
        const color* s_data = m_input->data().data();
        color*       d_data = m_output->data().data();

        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                int offset   = i * w + j;
                int d_j      = w - j - 1;
                int d_offset = i * w + d_j;

                d_data[d_offset] = s_data[offset];
            }
        }

        return true;
    }

    bool cpu_filter_flip_y::execute() {
        int          w      = m_input->width();
        int          h      = m_input->height();
        const color* s_data = m_input->data().data();
        color*       d_data = m_output->data().data();

        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                int offset   = i * w + j;
                int d_i      = h - i - 1;
                int d_offset = d_i * w + j;

                d_data[d_offset] = s_data[offset];
            }
        }

        return true;
    }

}// namespace gpusandbox