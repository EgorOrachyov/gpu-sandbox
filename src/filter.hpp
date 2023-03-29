/**
 * @file filter.hpp
 * @author Egor Orachev
 * @date 3/29/2023
 */

#ifndef GPUSANDBOX_FILTER_HPP
#define GPUSANDBOX_FILTER_HPP

#include "image.hpp"

#include <cxxopts.hpp>

#include <string>

namespace gpusandbox {

    /**
     * @brief An interface to any image filter
     */
    class filter {
    public:
        virtual ~filter()                                                                   = default;
        virtual bool prepare()                                                              = 0;
        virtual bool execute(const image& input, image& output, cxxopts::ParseResult& args) = 0;
    };

}// namespace gpusandbox

#endif//GPUSANDBOX_FILTER_HPP
