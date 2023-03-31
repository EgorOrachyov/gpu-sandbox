/**
 * @file cl_filter.hpp
 * @author Egor Orachev
 * @date 3/30/2023
 */

#ifndef GPUSANDBOX_CL_FILTER_HPP
#define GPUSANDBOX_CL_FILTER_HPP

#include "cl_backend.hpp"
#include "filter.hpp"

#include <memory>

namespace gpusandbox {

    /**
     * @brief A base class for opencl based filter
     */
    class cl_filter : public filter {
    public:
        ~cl_filter() override = default;
        bool prepare() override;
        bool readback() override;

        void set_backend(std::shared_ptr<cl_backend> backend) { m_backend = std::move(backend); }

    protected:
        std::shared_ptr<cl_backend> m_backend;
        cl::Buffer                  m_cl_input_buf;
        cl::Buffer                  m_cl_output_buf;
        cl::Image2D                 m_cl_input_img;
        cl::Image2D                 m_cl_output_img;
        cl::Sampler                 m_cl_sampler;
    };

}// namespace gpusandbox

#endif//GPUSANDBOX_CL_FILTER_HPP
