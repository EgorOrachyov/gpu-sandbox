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
        virtual ~filter()      = default;
        virtual bool prepare() = 0;
        virtual bool execute() = 0;

        void set_args(cxxopts::ParseResult* args) { m_args = args; }
        void set_input(const image* input) { m_input = input; }
        void set_output(image* output) { m_output = output; }

    protected:
        cxxopts::ParseResult* m_args   = nullptr;
        const image*          m_input  = nullptr;
        image*                m_output = nullptr;
    };

}// namespace gpusandbox

#endif//GPUSANDBOX_FILTER_HPP
