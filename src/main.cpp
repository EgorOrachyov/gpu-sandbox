/**
 * @file main.cpp
 * @author Egor Orachev
 * @date 3/29/2023
 */

#include "filter.hpp"
#include "image.hpp"
#include "timer.hpp"

#include "cpu_filter_blur.hpp"
#include "cpu_filter_flip.hpp"
#include "cpu_filter_gray.hpp"
#include "cpu_filter_motion.hpp"
#include "cpu_filter_sobel.hpp"

#ifdef GPUSANDBOX_BUILD_OPENCL
    #include "cl_backend.hpp"
    #include "cl_filter.hpp"
    #include "cl_filter_blur.hpp"
    #include "cl_filter_gray.hpp"
    #include "cl_filter_motion.hpp"
    #include "cl_filter_sobel.hpp"
#endif

#include <cxxopts.hpp>

#include <iostream>
#include <map>

int main(int argc, const char* const* argv) {
    cxxopts::Options options("gpusandbox", "A sandbox application to test gpu algorithms");
    options.add_option("", cxxopts::Option("h,help", "display help info about application", cxxopts::value<bool>()->default_value("false")));
    options.add_option("", cxxopts::Option("l,list", "list all available filters to apply", cxxopts::value<bool>()->default_value("false")));
    options.add_option("", cxxopts::Option("platform", "num of platform to select for gpu", cxxopts::value<int>()->default_value("0")));
    options.add_option("", cxxopts::Option("device", "num of device to select for gpu", cxxopts::value<int>()->default_value("0")));
    options.add_option("", cxxopts::Option("storage", "type of gpu storage to use for data (either buffer or image)", cxxopts::value<std::string>()->default_value("buffer")));
    options.add_option("", cxxopts::Option("input", "path to the image to filter", cxxopts::value<std::string>()->default_value("<none>")));
    options.add_option("", cxxopts::Option("output", "path to save result filtered image", cxxopts::value<std::string>()->default_value("out.bmp")));
    options.add_option("", cxxopts::Option("filter", "filter name to apply", cxxopts::value<std::string>()->default_value("<none>")));
    options.add_option("", cxxopts::Option("conv_size", "size of kernel for convolution", cxxopts::value<int>()->default_value("3")));

    cxxopts::ParseResult args;

    try {
        args = options.parse(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "failed to parse input arguments " << e.what();
        return 1;
    }

    // show help and exit, ok
    if (args["help"].as<bool>()) {
        std::cout << options.help();
        return 0;
    }

    // register filters
    std::map<std::string, std::unique_ptr<gpusandbox::filter>> filters;
    filters["cpu_flip_x"] = std::make_unique<gpusandbox::cpu_filter_flip_x>();
    filters["cpu_flip_y"] = std::make_unique<gpusandbox::cpu_filter_flip_y>();
    filters["cpu_blur"]   = std::make_unique<gpusandbox::cpu_filter_blur>();
    filters["cpu_motion"] = std::make_unique<gpusandbox::cpu_filter_motion>();
    filters["cpu_sobel"]  = std::make_unique<gpusandbox::cpu_filter_sobel>();
    filters["cpu_gray"]   = std::make_unique<gpusandbox::cpu_filter_gray>();
#ifdef GPUSANDBOX_BUILD_OPENCL
    filters["cl_blur"]   = std::make_unique<gpusandbox::cl_filter_blur>();
    filters["cl_motion"] = std::make_unique<gpusandbox::cl_filter_motion>();
    filters["cl_sobel"]  = std::make_unique<gpusandbox::cl_filter_sobel>();
    filters["cl_gray"]   = std::make_unique<gpusandbox::cl_filter_gray>();
#endif

    // show filters available
    if (args["list"].as<bool>()) {
        for (auto& kv : filters) std::cout << kv.first << "\n";
        return 0;
    }

    // state
    gpusandbox::filter*                filter = filters[args["filter"].as<std::string>()].get();
    std::shared_ptr<gpusandbox::image> input  = std::make_shared<gpusandbox::image>();
    std::shared_ptr<gpusandbox::image> output = std::make_shared<gpusandbox::image>();

    // check filter
    if (!filter) {
        std::cerr << "no filter with name '" << args["filter"].as<std::string>() << "'";
        return 1;
    }

    // load image to filter
    if (!input->load(args["input"].as<std::string>())) {
        return 1;
    }

    // allocate image to save result
    output->resize(input->width(), input->height());

#ifdef GPUSANDBOX_BUILD_OPENCL
    std::shared_ptr<gpusandbox::cl_backend> cl_backend = std::make_shared<gpusandbox::cl_backend>();

    if (!cl_backend->init(args["platform"].as<int>(), args["device"].as<int>())) {
        std::cerr << "failed to init cl backend";
        return 1;
    }
#endif

    gpusandbox::timer timer_prepare;
    gpusandbox::timer timer_exec;
    gpusandbox::timer timer_readback;
    gpusandbox::timer timer_total;

    timer_total.start();
    {
        filter->set_args(&args);
        filter->set_input(input);
        filter->set_output(output);

#ifdef GPUSANDBOX_BUILD_OPENCL
        if (auto* cl_filter = dynamic_cast<gpusandbox::cl_filter*>(filter)) {
            cl_filter->set_backend(cl_backend);
        }
#endif

        timer_prepare.start();
        {
            if (!filter->prepare()) {
                std::cerr << "failed to prepare image "
                          << args["input"].as<std::string>() << " with "
                          << " '" << args["filter"].as<std::string>() << "'";
                return 1;
            }
        }
        timer_prepare.stop();

        timer_exec.start();
        {
            if (!filter->execute()) {
                std::cerr << "failed to filter image "
                          << args["input"].as<std::string>() << " with "
                          << " '" << args["filter"].as<std::string>() << "'";
                return 1;
            }
        }
        timer_exec.stop();

        timer_readback.start();
        {
            if (!filter->readback()) {
                std::cerr << "failed to readback image "
                          << args["input"].as<std::string>() << " with "
                          << " '" << args["filter"].as<std::string>() << "'";
                return 1;
            }
        }
        timer_readback.stop();
    }
    timer_total.stop();

    std::cout << "Filter image " << args["input"].as<std::string>() << " with "
              << " '" << args["filter"].as<std::string>() << "'\n";
    std::cout << " time total:    " << timer_total.elapsed_ms() << " ms\n";
    std::cout << " time prepare:  " << timer_prepare.elapsed_ms() << " ms\n";
    std::cout << " time exec:     " << timer_exec.elapsed_ms() << " ms\n";
    std::cout << " time readback: " << timer_readback.elapsed_ms() << " ms\n";

    // save filtered image
    if (!output->save(args["output"].as<std::string>())) {
        return 1;
    }

    return 0;
}