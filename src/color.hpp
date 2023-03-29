/**
 * @file color.hpp
 * @author Egor Orachev
 * @date 3/30/2023
 */

#ifndef GPUSANDBOX_COLOR_HPP
#define GPUSANDBOX_COLOR_HPP

namespace gpusandbox {

    /**
     * @brief 4-chanel float color representation
     */
    struct color {
        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;
        float a = 0.0f;
    };

}// namespace gpusandbox

#endif//GPUSANDBOX_COLOR_HPP
