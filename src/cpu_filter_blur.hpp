/**
 * @file cpu_filter_blur.hpp
 * @author Egor Orachev
 * @date 3/30/2023
 */

#ifndef GPUSANDBOX_CPU_FILTER_BLUR_HPP
#define GPUSANDBOX_CPU_FILTER_BLUR_HPP

#include "filter.hpp"

namespace gpusandbox {

    /**
     * @brief Naive cpu blur filter
     */
    class cpu_filter_blur : public filter {
    public:
        ~cpu_filter_blur() override = default;
        bool prepare() override;
        bool execute() override;
    };


}// namespace gpusandbox

#endif//GPUSANDBOX_CPU_FILTER_BLUR_HPP
