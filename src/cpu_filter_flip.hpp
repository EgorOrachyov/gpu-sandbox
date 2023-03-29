/**
 * @file cpu_filter_flip.hpp
 * @author Egor Orachev
 * @date 3/29/2023
 */

#ifndef GPUSANDBOX_CPU_FILTER_FLIP_HPP
#define GPUSANDBOX_CPU_FILTER_FLIP_HPP

#include "filter.hpp"

namespace gpusandbox {

    /**
     * @brief Naive cpu flip along x axis from left to right filter
     */
    class cpu_filter_flip_x : public filter {
    public:
        ~cpu_filter_flip_x() override = default;
        bool prepare() override;
        bool execute(const image& input, image& output, cxxopts::ParseResult& args) override;
    };

    /**
     * @brief Naive cpu flip along y axis from bottom to top filter
     */
    class cpu_filter_flip_y : public filter {
    public:
        ~cpu_filter_flip_y() override = default;
        bool prepare() override;
        bool execute(const image& input, image& output, cxxopts::ParseResult& args) override;
    };

}// namespace gpusandbox

#endif//GPUSANDBOX_CPU_FILTER_FLIP_HPP