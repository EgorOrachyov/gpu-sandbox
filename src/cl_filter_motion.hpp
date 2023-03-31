/**
 * @file cl_filter_motion.hpp
 * @author Egor Orachev
 * @date 3/31/2023
 */

#ifndef GPUSANDBOX_CL_FILTER_MOTION_HPP
#define GPUSANDBOX_CL_FILTER_MOTION_HPP

#include "cl_conv.hpp"
#include "cl_filter.hpp"

namespace gpusandbox {

    /**
     * @brief OpenCL motion blur filter
     */
    class cl_filter_motion : public cl_filter {
    public:
        ~cl_filter_motion() override = default;
        bool prepare() override;
        bool execute() override;

    private:
        std::unique_ptr<cl_conv> m_conv;
    };

}// namespace gpusandbox

#endif//GPUSANDBOX_CL_FILTER_MOTION_HPP
