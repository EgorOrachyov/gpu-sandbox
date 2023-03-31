/**
 * @file cl_filter_sobel.hpp
 * @author Egor Orachev
 * @date 3/31/2023
 */

#ifndef GPUSANDBOX_CL_FILTER_SOBEL_HPP
#define GPUSANDBOX_CL_FILTER_SOBEL_HPP

#include "cl_conv.hpp"
#include "cl_filter.hpp"

namespace gpusandbox {

    /**
     * @brief OpenCL sobel filter
     */
    class cl_filter_sobel : public cl_filter {
    public:
        ~cl_filter_sobel() override = default;
        bool prepare() override;
        bool execute() override;

    private:
        std::unique_ptr<cl_conv2> m_conv;
    };

}// namespace gpusandbox

#endif//GPUSANDBOX_CL_FILTER_SOBEL_HPP
