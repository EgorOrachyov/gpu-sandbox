/**
 * @file cl_conv.hpp
 * @author Egor Orachev
 * @date 3/31/2023
 */

#ifndef GPUSANDBOX_CL_CONV_HPP
#define GPUSANDBOX_CL_CONV_HPP

#include "cl_backend.hpp"
#include "image.hpp"

#include <memory>
#include <string>
#include <vector>

namespace gpusandbox {

    /**
     * @brief OpenCL convolution with 2-d kernel
     */
    class cl_conv {
    public:
        cl_conv(std::string key, std::shared_ptr<cl_backend> backend, float factor, int size, float* kernel);
        bool prepare();
        bool operator()(int w, int h, cl::Image2D& input, cl::Image2D& output, cl::Sampler& sampler);

    private:
        std::shared_ptr<cl_backend> m_backend;
        std::vector<float>          m_kernel_source;
        std::string                 m_key;
        cl::Buffer                  m_kernel;
        cl::Program                 m_program;
        float                       m_factor = 1.0f;
        int                         m_size   = 0;
    };

    /**
     * @brief OpenCL convolution with two 2-d kernel
     */
    class cl_conv2 {
    public:
        cl_conv2(std::string key, std::shared_ptr<cl_backend> backend, float factor, int size, float* kernel1, float* kernel2);
        bool prepare();
        bool operator()(int w, int h, cl::Image2D& input, cl::Image2D& output, cl::Sampler& sampler);

    private:
        std::shared_ptr<cl_backend> m_backend;
        std::vector<float>          m_kernel_source1;
        std::vector<float>          m_kernel_source2;
        std::string                 m_key;
        cl::Buffer                  m_kernel1;
        cl::Buffer                  m_kernel2;
        cl::Program                 m_program;
        float                       m_factor = 1.0f;
        int                         m_size   = 0;
    };

}// namespace gpusandbox

#endif//GPUSANDBOX_CL_CONV_HPP
