/**
 * @file cl_filter_gray.hpp
 * @author Egor Orachev
 * @date 3/30/2023
 */

#ifndef GPUSANDBOX_CL_FILTER_GRAY_HPP
#define GPUSANDBOX_CL_FILTER_GRAY_HPP

#include "cl_filter.hpp"

namespace gpusandbox {

    /**
     * @brief OpenCL grayscale filter implementation
     */
    class cl_filter_gray : public cl_filter {
    public:
        ~cl_filter_gray() override = default;
        bool prepare() override;
        bool execute() override;

    private:
        cl::Program m_gray_buff;
        cl::Program m_gray_img;
    };

}// namespace gpusandbox

#endif//GPUSANDBOX_CL_FILTER_GRAY_HPP
