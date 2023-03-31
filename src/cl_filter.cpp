/**
 * @file cl_filter.cpp
 * @author Egor Orachev
 * @date 3/30/2023
 */

#include "cl_filter.hpp"

#include <cassert>
#include <iostream>

namespace gpusandbox {

    bool cl_filter::prepare() {
        assert(m_input);
        assert(m_output);
        assert(m_args);

        auto storage = (*m_args)["storage"].as<std::string>();

        if (storage == "buffer") {
            m_cl_input_buf = cl::Buffer(m_backend->context(),
                                        CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR,
                                        sizeof(color) * m_input->data().size(),
                                        m_input->data().data());

            m_cl_output_buf = cl::Buffer(m_backend->context(),
                                         CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY,
                                         sizeof(color) * m_output->data().size());

            return true;
        }
        if (storage == "image") {
            m_cl_input_img = cl::Image2D(m_backend->context(),
                                         CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR,
                                         cl::ImageFormat(CL_RGBA, CL_FLOAT),
                                         m_input->width(),
                                         m_input->height(),
                                         m_input->width() * sizeof(color),
                                         m_input->data().data());

            m_cl_output_img = cl::Image2D(m_backend->context(),
                                          CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY,
                                          cl::ImageFormat(CL_RGBA, CL_FLOAT),
                                          m_input->width(),
                                          m_input->height());

            m_cl_sampler = cl::Sampler(m_backend->context(),
                                       false,
                                       CL_ADDRESS_CLAMP_TO_EDGE,
                                       CL_FILTER_NEAREST);

            return true;
        }

        std::cerr << "invalid storage specified " << storage;

        return false;
    }

    bool cl_filter::readback() {
        assert(m_input);
        assert(m_output);
        assert(m_args);

        auto storage = (*m_args)["storage"].as<std::string>();

        if (storage == "buffer") {
            m_backend->queue().enqueueReadBuffer(m_cl_output_buf,
                                                 true,
                                                 0,
                                                 sizeof(color) * m_output->data().size(),
                                                 m_output->data().data());
            return true;
        }
        if (storage == "image") {
            std::array<size_t, 3> origin = {0, 0, 0};
            std::array<size_t, 3> region = {std::size_t(m_output->width()), std::size_t(m_output->height()), 1};

            m_backend->queue().enqueueReadImage(m_cl_output_img,
                                                true,
                                                origin,
                                                region,
                                                sizeof(color) * m_output->width(),
                                                0,
                                                m_output->data().data());
            return true;
        }

        std::cerr << "invalid storage specified " << storage;

        return false;
    }

}// namespace gpusandbox