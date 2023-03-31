/**
 * @file cl_filter_motion.cpp
 * @author Egor Orachev
 * @date 3/31/2023
 */

#include "cl_filter_motion.hpp"

namespace gpusandbox {

    bool cl_filter_motion::prepare() {
        if (!cl_filter::prepare()) {
            return false;
        }

        float kernel[9][9] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                              0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

        m_conv = std::make_unique<cl_conv>("motion", m_backend, 9.0f, 9, (float*) kernel);

        return m_conv->prepare();
    }

    bool cl_filter_motion::execute() {
        if ((*m_args)["storage"].as<std::string>() == "image") {
            return (*m_conv)(m_input->width(), m_input->height(), m_cl_input_img, m_cl_output_img, m_cl_sampler);
        }

        return false;
    }

}// namespace gpusandbox