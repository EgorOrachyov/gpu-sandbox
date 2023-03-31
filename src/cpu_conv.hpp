/**
 * @file cpu_apply_kernel.hpp
 * @author Egor Orachev
 * @date 3/29/2023
 */

#ifndef GPUSANDBOX_CPU_CONV_HPP
#define GPUSANDBOX_CPU_CONV_HPP

#include "image.hpp"

#include <cmath>

namespace gpusandbox {

    /**
     * @brief Naive cpu convolution using 2-d kernel
     *
     * @tparam size Size of kernel, must be odd
     */
    template<int size>
    class cpu_conv {
    public:
        static_assert(size % 2, "must be odd");

        void operator()(const image& input, image& output, float factor, const float kernel[size][size]) {
            int          w      = input.width();
            int          h      = input.height();
            const color* s_data = input.data().data();
            color*       d_data = output.data().data();

            for (int pixel_i = 0; pixel_i < h; pixel_i++) {
                for (int pixel_j = 0; pixel_j < w; pixel_j++) {
                    color result{};

                    for (int kernel_i = 0; kernel_i < size; kernel_i++) {
                        for (int kernel_j = 0; kernel_j < size; kernel_j++) {
                            int sample_i = pixel_i + kernel_i - size / 2;
                            int sample_j = pixel_j + kernel_j - size / 2;

                            if (0 <= sample_i && sample_i < h && 0 <= sample_j && sample_j < w) {
                                color source = s_data[sample_i * w + sample_j];

                                result.r += source.r * kernel[kernel_i][kernel_j];
                                result.g += source.g * kernel[kernel_i][kernel_j];
                                result.b += source.b * kernel[kernel_i][kernel_j];
                            }
                        }
                    }

                    result.r /= factor;
                    result.g /= factor;
                    result.b /= factor;

                    result.r = std::min(std::max(0.0f, result.r), 1.0f);
                    result.g = std::min(std::max(0.0f, result.g), 1.0f);
                    result.b = std::min(std::max(0.0f, result.b), 1.0f);
                    result.a = 1.0f;

                    d_data[pixel_i * w + pixel_j] = result;
                }
            }
        }
    };

    /**
     * @brief Naive cpu convolution using two 2-d kernel plus merge functor
     *
     * @tparam size Size of kernel, must be odd
     */
    template<int size>
    class cpu_conv2 {
    public:
        static_assert(size % 2, "must be odd");

        template<typename Functor>
        void operator()(const image& input, image& output, Functor f, const float kernel1[size][size], const float kernel2[size][size]) {
            int          w      = input.width();
            int          h      = input.height();
            const color* s_data = input.data().data();
            color*       d_data = output.data().data();

            for (int pixel_i = 0; pixel_i < h; pixel_i++) {
                for (int pixel_j = 0; pixel_j < w; pixel_j++) {
                    color result{};
                    color result1{};
                    color result2{};

                    for (int kernel_i = 0; kernel_i < size; kernel_i++) {
                        for (int kernel_j = 0; kernel_j < size; kernel_j++) {
                            int sample_i = pixel_i + kernel_i - size / 2;
                            int sample_j = pixel_j + kernel_j - size / 2;

                            if (0 <= sample_i && sample_i < h && 0 <= sample_j && sample_j < w) {
                                color source = s_data[sample_i * w + sample_j];

                                result1.r += source.r * kernel1[kernel_i][kernel_j];
                                result1.g += source.g * kernel1[kernel_i][kernel_j];
                                result1.b += source.b * kernel1[kernel_i][kernel_j];

                                result2.r += source.r * kernel2[kernel_i][kernel_j];
                                result2.g += source.g * kernel2[kernel_i][kernel_j];
                                result2.b += source.b * kernel2[kernel_i][kernel_j];
                            }
                        }
                    }

                    result.r = f(result1.r, result2.r);
                    result.g = f(result1.g, result2.g);
                    result.b = f(result1.b, result2.b);

                    result.r = std::min(std::max(0.0f, result.r), 1.0f);
                    result.g = std::min(std::max(0.0f, result.g), 1.0f);
                    result.b = std::min(std::max(0.0f, result.b), 1.0f);
                    result.a = 1.0f;

                    d_data[pixel_i * w + pixel_j] = result;
                }
            }
        }
    };

}// namespace gpusandbox

#endif//GPUSANDBOX_CPU_CONV_HPP
