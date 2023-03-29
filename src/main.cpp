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
#include "cpu_filter_motion.hpp"
#include "cpu_filter_sobel.hpp"

#include <cxxopts.hpp>

#include <iostream>
#include <unordered_map>

int main(int argc, const char* const* argv) {
    cxxopts::Options options("gpusandbox", "A sandbox application to test gpu algorithms");
    options.add_option("", cxxopts::Option("h,help", "display help info about application", cxxopts::value<bool>()->default_value("false")));
    options.add_option("", cxxopts::Option("platform", "num of platform to select for gpu", cxxopts::value<int>()->default_value("0")));
    options.add_option("", cxxopts::Option("device", "num of device to select for gpu", cxxopts::value<int>()->default_value("0")));
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

    // filters
    std::unordered_map<std::string, std::unique_ptr<gpusandbox::filter>> filters;
    filters["cpu_flip_x"] = std::make_unique<gpusandbox::cpu_filter_flip_x>();
    filters["cpu_flip_y"] = std::make_unique<gpusandbox::cpu_filter_flip_y>();
    filters["cpu_blur"]   = std::make_unique<gpusandbox::cpu_filter_blur>();
    filters["cpu_motion"] = std::make_unique<gpusandbox::cpu_filter_motion>();
    filters["cpu_sobel"]  = std::make_unique<gpusandbox::cpu_filter_sobel>();

    // state
    gpusandbox::filter* filter = filters[args["filter"].as<std::string>()].get();
    gpusandbox::image   input;
    gpusandbox::image   output;

    // check filter
    if (!filter) {
        std::cerr << "no filter with name '" << args["filter"].as<std::string>() << "'";
        return 1;
    }

    // load image to filter
    if (!input.load(args["input"].as<std::string>())) {
        return 1;
    }

    // allocate image to save result
    output.resize(input.width(), input.height());

    gpusandbox::timer timer_exec;
    gpusandbox::timer timer_prepare;
    gpusandbox::timer timer_total;

    timer_exec.start();

    if (!filter->execute(input, output, args)) {
        std::cerr << "failed to filter image "
                  << args["input"].as<std::string>() << " with "
                  << " '" << args["filter"].as<std::string>() << "'";
        return 1;
    }

    timer_exec.stop();

    std::cout << "Filter image " << args["input"].as<std::string>() << " with "
              << " '" << args["filter"].as<std::string>() << "'\n";
    std::cout << " time total:   " << timer_total.elapsed_ms() << " ms\n";
    std::cout << " time prepare: " << timer_prepare.elapsed_ms() << " ms\n";
    std::cout << " time exec:    " << timer_exec.elapsed_ms() << " ms\n";

    // save filtered image
    if (!output.save(args["output"].as<std::string>())) {
        return 1;
    }

    return 0;
}