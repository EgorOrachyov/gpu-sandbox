/**
 * @file timer.cpp
 * @author Egor Orachev
 * @date 3/29/2023
 */

#include "timer.hpp"

namespace gpusandbox {

    void timer::start() {
        m_begin = m_end = clock::now();
    }
    void timer::stop() {
        m_end = clock::now();
    }
    float timer::elapsed_ms() {
        auto duration = m_end - m_begin;
        auto count    = std::chrono::duration_cast<ns>(duration).count();
        return float(count) * 1e-6f;
    }

}// namespace gpusandbox