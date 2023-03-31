/**
 * @file cl_filter_blur.hpp
 * @author Egor Orachev
 * @date 3/31/2023
 */

#ifndef GPUSANDBOX_CL_FILTER_BLUR_HPP
#define GPUSANDBOX_CL_FILTER_BLUR_HPP

#include "cl_conv.hpp"
#include "cl_filter.hpp"

namespace gpusandbox {

    class cl_filter_blur : public cl_filter {
    public:
        ~cl_filter_blur() override = default;
        bool prepare() override;
        bool execute() override;

    private:
        std::unique_ptr<cl_conv> m_conv;
    };

}// namespace gpusandbox

#endif//GPUSANDBOX_CL_FILTER_BLUR_HPP
