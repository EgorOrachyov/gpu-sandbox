/**
 * @file cl_backend.cpp
 * @author Egor Orachev
 * @date 3/30/2023
 */

#include "cl_backend.hpp"

#include <iostream>
#include <utility>

namespace gpusandbox {

    bool cl_backend::init(int platform_id, int device_id) {
        std::vector<cl::Platform> platforms;
        cl::Platform::get(&platforms);

        if (platform_id >= platforms.size()) {
            std::cerr << "no cl platform " << platform_id;
            return false;
        }

        m_platform = platforms[platform_id];

        std::vector<cl::Device> devices;
        m_platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);

        if (device_id >= devices.size()) {
            std::cerr << "no cl device " << device_id;
            return false;
        }

        m_device = devices[device_id];

        m_context = cl::Context(m_device);
        m_queue   = cl::CommandQueue(m_context, m_device);

        std::string vendor_name = m_device.getInfo<CL_DEVICE_VENDOR>();
        int         vendor_id   = m_device.getInfo<CL_DEVICE_VENDOR_ID>();

        std::cout << "init cl backend " << vendor_name << " id: " << vendor_id << "\n";

        return true;
    }

    bool cl_backend::get_or_build_program(const std::string& key, const std::string& source, cl::Program& program, const std::string& defines) {
        program = get_program(key);

        if (!program()) {
            return build_and_cache_program(key, source, program, defines);
        }

        return true;
    }

    bool cl_backend::build_and_cache_program(const std::string& key, const std::string& source, cl::Program& program, const std::string& defines) {
        program = cl::Program(m_context, source);

        auto options = "-cl-std=CL1.2 " + defines;

        if (program.build(options.c_str()) != CL_SUCCESS) {
            std::cerr << "failed to build "
                      << key << " with options "
                      << options << " due "
                      << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(m_device);
            return false;
        }

        cache_program(key, program);
        return true;
    }

    void cl_backend::cache_program(const std::string& key, cl::Program program) {
        m_program_cache[key] = std::move(program);
    }

    cl::Program cl_backend::get_program(const std::string& key) {
        return m_program_cache[key];
    }

}// namespace gpusandbox