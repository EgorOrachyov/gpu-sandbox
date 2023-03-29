/**
 * @file timer.hpp
 * @author Egor Orachev
 * @date 3/29/2023
 */

#ifndef GPUSANDBOX_TIMER_HPP
#define GPUSANDBOX_TIMER_HPP

#include <chrono>

namespace gpusandbox {

    /**
     * @brief Aux timer to measure execution time of a block of code
     */
    class timer {
    public:
        void  start();
        void  stop();
        float elapsed_ms();

    private:
        using clock      = std::chrono::steady_clock;
        using time_point = clock::time_point;
        using ns         = std::chrono::nanoseconds;

        time_point m_begin{};
        time_point m_end{};
    };

}// namespace gpusandbox

#endif//GPUSANDBOX_TIMER_HPP
