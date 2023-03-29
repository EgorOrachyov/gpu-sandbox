/**
 * @file image.hpp
 * @author Egor Orachev
 * @date 3/29/2023
 */

#ifndef GPUSANDBOX_IMAGE_HPP
#define GPUSANDBOX_IMAGE_HPP

#include "color.hpp"

#include <filesystem>
#include <vector>

namespace gpusandbox {

    /**
     * @brief 2d-image holding pixel data in float format with a float per pixel
     */
    class image {
    public:
        bool load(const std::string& image);
        bool save(const std::string& image);
        void resize(int width, int height);

        [[nodiscard]] const std::vector<color>& data() const { return m_data; }
        [[nodiscard]] std::vector<color>&       data() { return m_data; }
        [[nodiscard]] int                       width() const { return m_width; }
        [[nodiscard]] int                       height() const { return m_height; }

    private:
        std::vector<color> m_data;
        int                m_width  = 0;
        int                m_height = 0;
    };

}// namespace gpusandbox

#endif//GPUSANDBOX_IMAGE_HPP
