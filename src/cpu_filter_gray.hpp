/**
 * @file cpu_filter_gray.hpp
 * @author Egor Orachev
 * @date 3/30/2023
 */

#ifndef GPUSANDBOX_CPU_FILTER_GRAY_HPP
#define GPUSANDBOX_CPU_FILTER_GRAY_HPP

#include "filter.hpp"

namespace gpusandbox {

    /**
     * @brief Grayscale filter
     */
    class cpu_filter_gray : public filter {
    public:
        ~cpu_filter_gray() override = default;
        bool execute() override;
    };

}// namespace gpusandbox

#endif//GPUSANDBOX_CPU_FILTER_GRAY_HPP
