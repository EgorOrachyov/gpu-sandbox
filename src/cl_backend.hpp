/**
 * @file cl_backend.hpp
 * @author Egor Orachev
 * @date 3/30/2023
 */

#ifndef GPUSANDBOX_CL_BACKEND_HPP
#define GPUSANDBOX_CL_BACKEND_HPP

#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_TARGET_OPENCL_VERSION  120
#include <CL/cl2.hpp>

#include <string>
#include <unordered_map>

namespace gpusandbox {

    /**
     * @brief State of initialized opencl backend for computations
     */
    class cl_backend {
    public:
        bool init(int platform_id, int device_id);

        bool        get_or_build_program(const std::string& key, const std::string& source, cl::Program& program, const std::string& defines = "");
        bool        build_and_cache_program(const std::string& key, const std::string& source, cl::Program& program, const std::string& defines);
        void        cache_program(const std::string& key, cl::Program program);
        cl::Program get_program(const std::string& key);

        cl::Platform&                                 platform() { return m_platform; }
        cl::Device&                                   device() { return m_device; }
        cl::Context&                                  context() { return m_context; }
        cl::CommandQueue&                             queue() { return m_queue; }
        std::unordered_map<std::string, cl::Program>& program_cache() { return m_program_cache; }

    private:
        cl::Platform                                 m_platform;
        cl::Device                                   m_device;
        cl::Context                                  m_context;
        cl::CommandQueue                             m_queue;
        std::unordered_map<std::string, cl::Program> m_program_cache;
    };

}// namespace gpusandbox

#endif//GPUSANDBOX_CL_BACKEND_HPP
