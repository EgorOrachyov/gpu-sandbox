/**
 * @file cpu_filter_sobel.hpp
 * @author Egor Orachev
 * @date 3/30/2023
 */

#ifndef GPUSANDBOX_CPU_FILTER_SOBEL_HPP
#define GPUSANDBOX_CPU_FILTER_SOBEL_HPP

#include "filter.hpp"

namespace gpusandbox {

    /**
     * @brief Naive cpu sobel filter
     */
    class cpu_filter_sobel : public filter {
    public:
        ~cpu_filter_sobel() override = default;
        bool prepare() override;
        bool execute(const image& input, image& output, cxxopts::ParseResult& args) override;
    };

}// namespace gpusandbox

#endif//GPUSANDBOX_CPU_FILTER_SOBEL_HPP