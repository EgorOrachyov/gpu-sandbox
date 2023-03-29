/**
 * @file cpu_filter_motion.hpp
 * @author Egor Orachev
 * @date 3/30/2023
 */

#ifndef GPUSANDBOX_CPU_FILTER_MOTION_HPP
#define GPUSANDBOX_CPU_FILTER_MOTION_HPP

#include "filter.hpp"

namespace gpusandbox {

    /**
     * @brief Naive cpu motion filter along main diagonal
     */
    class cpu_filter_motion : public filter {
    public:
        ~cpu_filter_motion() override = default;
        bool prepare() override;
        bool execute(const image& input, image& output, cxxopts::ParseResult& args) override;
    };

}// namespace gpusandbox

#endif//GPUSANDBOX_CPU_FILTER_MOTION_HPP