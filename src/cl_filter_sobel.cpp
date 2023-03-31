/**
 * @file cl_filter_sobel.cpp
 * @author Egor Orachev
 * @date 3/31/2023
 */

#include "cl_filter_sobel.hpp"

namespace gpusandbox {

    bool cl_filter_sobel::prepare() {
        if (!cl_filter::prepare()) {
            return false;
        }

        float filter_kernel1[3][3] = {1.0f, 0.0f, -1.0f,
                                      2.0f, 0.0f, -2.0f,
                                      1.0f, 0.0f, -1.0f};

        float filter_kernel2[3][3] = {1.0f, 2.0f, 1.0f,
                                      0.0f, 0.0f, 0.0f,
                                      -1.0f, -2.0f, -1.0f};

        m_conv = std::make_unique<cl_conv2>("sobel", m_backend, 1.0f, 3, (float*) filter_kernel1, (float*) filter_kernel2);
        return m_conv->prepare();
    }
    bool cl_filter_sobel::execute() {
        if ((*m_args)["storage"].as<std::string>() == "image") {
            return (*m_conv)(m_input->width(), m_input->height(), m_cl_input_img, m_cl_output_img, m_cl_sampler);
        }

        return false;
    }

}// namespace gpusandbox