/**
 * @file filter.hpp
 * @author Egor Orachev
 * @date 3/29/2023
 */

#ifndef GPUSANDBOX_FILTER_HPP
#define GPUSANDBOX_FILTER_HPP

#include "image.hpp"

#include <cxxopts.hpp>

#include <memory>
#include <string>
#include <utility>

namespace gpusandbox {

    /**
     * @brief A base class for any image filter
     */
    class filter {
    public:
        virtual ~filter() = default;
        virtual bool prepare() { return true; }
        virtual bool execute() { return true; }
        virtual bool readback() { return true; }

        void set_args(cxxopts::ParseResult* args) { m_args = args; }
        void set_input(std::shared_ptr<image> input) { m_input = std::move(input); }
        void set_output(std::shared_ptr<image> output) { m_output = std::move(output); }

    protected:
        cxxopts::ParseResult*  m_args = nullptr;
        std::shared_ptr<image> m_input;
        std::shared_ptr<image> m_output = nullptr;
    };

}// namespace gpusandbox

#endif//GPUSANDBOX_FILTER_HPP
