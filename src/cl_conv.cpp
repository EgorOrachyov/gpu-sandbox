/**
 * @file cl_conv.cpp
 * @author Egor Orachev
 * @date 3/31/2023
 */

#include "cl_conv.hpp"

#include <utility>

namespace gpusandbox {

    cl_conv::cl_conv(std::string key, std::shared_ptr<cl_backend> backend, float factor, int size, float* kernel) {
        m_backend = std::move(backend);
        m_key     = std::move(key);
        m_factor  = factor;
        m_size    = size;

        m_kernel_source.resize(size * size);
        std::memcpy(m_kernel_source.data(), kernel, m_kernel_source.size() * sizeof(float));
    }

    bool cl_conv::prepare() {
        std::string source =
                "__kernel void filter(read_only image2d_t  input,\n"
                "                     write_only image2d_t output,\n"
                "                     sampler_t            sampler,\n"
                "                     int                  w,\n"
                "                     int                  h,\n"
                "                     float                factor,\n"
                "                     __constant float*    ker) {\n"
                "    int i = get_global_id(0);\n"
                "    int j = get_global_id(1);\n"
                "\n"
                "    if (i < h && j < w) {\n"
                "        float3 result = (float3) (0, 0, 0);\n"
                "\n"
                "        int k = 0;\n"
                "\n"
                "        for (int y = i - HALF_SIZE; y <= i + HALF_SIZE; y++) {\n"
                "            for (int x = j - HALF_SIZE; x <= j + HALF_SIZE; x++, k++) {\n"
                "                result += read_imagef(input, sampler, (int2) (x, y)).rgb * ker[k];\n"
                "            }\n"
                "        }\n"
                "\n"
                "        result /= factor;\n"
                "        result = clamp(result, (float3) (0, 0, 0), (float3) (1, 1, 1));\n"
                "        write_imagef(output, (int2) (j, i), (float4) (result, 1.0f));\n"
                "    }\n"
                "}";

        if (!m_backend->get_or_build_program(m_key, source, m_program, "-DHALF_SIZE=" + std::to_string(m_size / 2))) {
            return false;
        }

        m_kernel = cl::Buffer(m_backend->context(),
                              CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR,
                              m_kernel_source.size() * sizeof(float),
                              m_kernel_source.data());

        return true;
    }

    bool cl_conv::operator()(int w, int h, cl::Image2D& input, cl::Image2D& output, cl::Sampler& sampler) {
        auto kernel = cl::Kernel(m_program, "filter");
        kernel.setArg(0, input);
        kernel.setArg(1, output);
        kernel.setArg(2, sampler);
        kernel.setArg(3, w);
        kernel.setArg(4, h);
        kernel.setArg(5, m_factor);
        kernel.setArg(6, m_kernel);

        cl::NDRange global(h, w);
        m_backend->queue().enqueueNDRangeKernel(kernel, cl::NullRange, global);
        m_backend->queue().finish();
        return true;
    }

    cl_conv2::cl_conv2(std::string key, std::shared_ptr<cl_backend> backend, float factor, int size, float* kernel1, float* kernel2) {
        m_backend = std::move(backend);
        m_key     = std::move(key);
        m_factor  = factor;
        m_size    = size;

        m_kernel_source1.resize(size * size);
        std::memcpy(m_kernel_source1.data(), kernel1, m_kernel_source1.size() * sizeof(float));

        m_kernel_source2.resize(size * size);
        std::memcpy(m_kernel_source2.data(), kernel2, m_kernel_source2.size() * sizeof(float));
    }

    bool cl_conv2::prepare() {
        std::string source =
                "__kernel void filter(read_only image2d_t  input,\n"
                "                     write_only image2d_t output,\n"
                "                     sampler_t            sampler,\n"
                "                     int                  w,\n"
                "                     int                  h,\n"
                "                     float                factor,\n"
                "                     __constant float*    ker1,\n"
                "                     __constant float*    ker2) {\n"
                "    int i = get_global_id(0);\n"
                "    int j = get_global_id(1);\n"
                "\n"
                "    if (i < h && j < w) {\n"
                "        float3 result1 = (float3) (0, 0, 0);\n"
                "        float3 result2 = (float3) (0, 0, 0);\n"
                "\n"
                "        int k = 0;\n"
                "\n"
                "        for (int y = i - HALF_SIZE; y <= i + HALF_SIZE; y++) {\n"
                "            for (int x = j - HALF_SIZE; x <= j + HALF_SIZE; x++, k++) {\n"
                "                float3 color = read_imagef(input, sampler, (int2) (x, y)).rgb;\n"
                "                result1 += color * ker1[k];\n"
                "                result2 += color * ker2[k];\n"
                "            }\n"
                "        }\n"
                "\n"
                "        result1 /= factor;\n"
                "        result2 /= factor;\n"
                "\n"
                "        float3 result = sqrt(result1 * result1 + result2 * result2);\n"
                "        result        = clamp(result, (float3) (0, 0, 0), (float3) (1, 1, 1));\n"
                "\n"
                "        write_imagef(output, (int2) (j, i), (float4) (result, 1.0f));\n"
                "    }\n"
                "}";

        if (!m_backend->get_or_build_program(m_key, source, m_program, "-DHALF_SIZE=" + std::to_string(m_size / 2))) {
            return false;
        }

        m_kernel1 = cl::Buffer(m_backend->context(),
                               CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR,
                               m_kernel_source1.size() * sizeof(float),
                               m_kernel_source1.data());

        m_kernel2 = cl::Buffer(m_backend->context(),
                               CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR,
                               m_kernel_source2.size() * sizeof(float),
                               m_kernel_source2.data());

        return true;
    }

    bool cl_conv2::operator()(int w, int h, cl::Image2D& input, cl::Image2D& output, cl::Sampler& sampler) {
        auto kernel = cl::Kernel(m_program, "filter");
        kernel.setArg(0, input);
        kernel.setArg(1, output);
        kernel.setArg(2, sampler);
        kernel.setArg(3, w);
        kernel.setArg(4, h);
        kernel.setArg(5, m_factor);
        kernel.setArg(6, m_kernel1);
        kernel.setArg(7, m_kernel2);

        cl::NDRange global(h, w);
        m_backend->queue().enqueueNDRangeKernel(kernel, cl::NullRange, global);
        m_backend->queue().finish();
        return true;
    }

}// namespace gpusandbox