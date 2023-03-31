/**
 * @file cl_filter_blur.cpp
 * @author Egor Orachev
 * @date 3/31/2023
 */

#include "cl_filter_blur.hpp"

namespace gpusandbox {

    bool cl_filter_blur::prepare() {
        if (!cl_filter::prepare()) {
            return false;
        }

        int size = (*m_args)["conv_size"].as<int>();

        if (size == 3) {
            float filter_kernel[3][3] = {0.0f, 1.0f, 0.0f,
                                         1.0f, 1.0f, 1.0f,
                                         0.0f, 1.0f, 0.0f};

            m_conv = std::make_unique<cl_conv>("blur_3", m_backend, 5.0f, 3, (float*) filter_kernel);
            return m_conv->prepare();
        }
        if (size == 5) {
            float filter_kernel[5][5] = {0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                                         0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                                         1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
                                         0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                                         0.0f, 0.0f, 1.0f, 0.0f, 0.0f};

            m_conv = std::make_unique<cl_conv>("blur_3", m_backend, 13.0f, 5, (float*) filter_kernel);
            return m_conv->prepare();
        }
        if (size == 7) {
            float filter_kernel[7][7] = {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                                         0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
                                         0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                                         1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
                                         0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                                         0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
                                         0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f};

            m_conv = std::make_unique<cl_conv>("blur_3", m_backend, 25.0f, 7, (float*) filter_kernel);
            return m_conv->prepare();
        }

        std::cerr << "no filter size " << size;
        return false;
    }
    bool cl_filter_blur::execute() {
        if ((*m_args)["storage"].as<std::string>() == "image") {
            return (*m_conv)(m_input->width(), m_input->height(), m_cl_input_img, m_cl_output_img, m_cl_sampler);
        }

        return false;
    }

}// namespace gpusandbox