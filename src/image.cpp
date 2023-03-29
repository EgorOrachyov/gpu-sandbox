/**
 * @file image.cpp
 * @author Egor Orachev
 * @date 3/29/2023
 */

#include "image.hpp"

#include "stb_image.h"
#include "stb_image_write.h"

#include <cmath>
#include <filesystem>
#include <iostream>

namespace gpusandbox {

    bool image::load(const std::string& filepath) {
        int n_channels = 0;

        stbi_uc* image_data = stbi_load(filepath.c_str(), &m_width, &m_height, &n_channels, 4);

        if (!image_data) {
            std::cerr << "failed to load image " << filepath;
            return false;
        }

        int channels = 4;
        m_data.resize(m_height * m_width);

        int current_src = 0;
        int current_dst = 0;

        for (int i = 0; i < m_height; i++) {
            for (int j = 0; j < m_width; ++j) {
                m_data[current_dst].r = float(image_data[current_src + 0]) / 255.0f;
                m_data[current_dst].g = float(image_data[current_src + 1]) / 255.0f;
                m_data[current_dst].b = float(image_data[current_src + 2]) / 255.0f;
                m_data[current_dst].a = float(image_data[current_src + 3]) / 255.0f;

                current_src += channels;
                current_dst += 1;
            }
        }

        stbi_image_free(image_data);

        return true;
    }

    bool image::save(const std::string& filepath) {
        std::filesystem::path path(filepath);

        if (!path.has_extension()) {
            path = path.parent_path() / (path.filename().string() + ".bmp");
        }

        int                       channels = 4;
        std::string               path_str = path.string();
        std::vector<std::uint8_t> data;
        data.resize(m_height * m_width * channels);

        int current_src = 0;
        int current_dst = 0;

        for (int i = 0; i < m_height; i++) {
            for (int j = 0; j < m_width; ++j) {
                data[current_dst + 0] = std::uint8_t(std::floor(m_data[current_src].r * 255.0f));
                data[current_dst + 1] = std::uint8_t(std::floor(m_data[current_src].g * 255.0f));
                data[current_dst + 2] = std::uint8_t(std::floor(m_data[current_src].b * 255.0f));
                data[current_dst + 3] = std::uint8_t(std::floor(m_data[current_src].a * 255.0f));

                current_src += 1;
                current_dst += channels;
            }
        }

        if (!stbi_write_bmp(path_str.data(), m_width, m_height, channels, data.data())) {
            std::cerr << "failed to save image " << filepath;
            return false;
        }

        return true;
    }
    void image::resize(int width, int height) {
        m_width  = width;
        m_height = height;
        m_data.clear();
        m_data.resize(m_height * m_width);
    }

}// namespace gpusandbox