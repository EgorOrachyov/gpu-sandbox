/**
 * @file cl_filter_gray.cpp
 * @author Egor Orachev
 * @date 3/30/2023
 */

#include "cl_filter_gray.hpp"

namespace gpusandbox {

    bool cl_filter_gray::prepare() {
        if (!cl_filter::prepare()) {
            return false;
        }

        std::string kernel_1 =
                "__kernel void filter1(__global const float4* input,\n"
                "                     __global float4* output,\n"
                "                     int w,\n"
                "                     int h) {\n"
                "   uint i = get_global_id(0);\n"
                "   uint j = get_global_id(1);\n"
                "\n"
                "   if (i < h && i < w) {\n"
                "       float3 weights = (float3)(0.299f, 0.587f, 0.114f);\n"
                "       float gray = dot(input[i * w + j].rgb, weights);\n"
                "       output[i * w + j] = (float4)(gray, gray, gray, 1.0f);\n"
                "   }\n"
                "}\n";

        std::string kernel_2 =
                "__kernel void filter2(read_only image2d_t input,\n"
                "                     write_only image2d_t output,"
                "                     sampler_t sampler,\n"
                "                     int w,\n"
                "                     int h) {\n"
                "   uint i = get_global_id(0);\n"
                "   uint j = get_global_id(1);\n"
                "\n"
                "   if (i < h && i < w) {\n"
                "       float3 weights = (float3)(0.299f, 0.587f, 0.114f);\n"
                "       float3 color = read_imagef(input, sampler, (int2)(j, i)).rgb;"
                "       float gray = dot(color, weights);\n"
                "       write_imagef(output, (int2)(j, i), (float4)(gray, gray, gray, 1.0f));\n"
                "   }\n"
                "}\n";

        if (!m_backend->get_or_build_program("gray_buff", kernel_1, m_gray_buff)) {
            return false;
        }
        if (!m_backend->get_or_build_program("gray_img", kernel_2, m_gray_img)) {
            return false;
        }

        return true;
    }

    bool cl_filter_gray::execute() {
        if ((*m_args)["storage"].as<std::string>() == "buffer") {
            cl::Kernel kernel = cl::Kernel(m_gray_buff, "filter1");
            kernel.setArg(0, m_cl_input_buf);
            kernel.setArg(1, m_cl_output_buf);
            kernel.setArg(2, m_input->width());
            kernel.setArg(3, m_input->height());

            cl::NDRange global(m_input->height(), m_input->width());
            m_backend->queue().enqueueNDRangeKernel(kernel, cl::NullRange, global);
            m_backend->queue().finish();
            return true;
        }
        if ((*m_args)["storage"].as<std::string>() == "image") {
            cl::Kernel kernel = cl::Kernel(m_gray_img, "filter2");
            kernel.setArg(0, m_cl_input_img);
            kernel.setArg(1, m_cl_output_img);
            kernel.setArg(2, m_cl_sampler);
            kernel.setArg(3, m_input->width());
            kernel.setArg(4, m_input->height());

            cl::NDRange global(m_input->height(), m_input->width());
            m_backend->queue().enqueueNDRangeKernel(kernel, cl::NullRange, global);
            m_backend->queue().finish();
            return true;
        }

        return false;
    }

}// namespace gpusandbox
