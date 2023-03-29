/**
 * @file main.cpp
 * @author Egor Orachev
 * @date 3/29/2023
 */

#include <cxxopts.hpp>

#include <iostream>

int main(int argc, const char* const* argv) {
    cxxopts::Options options("gpusandbox", "A sandbox application to test gpu algorithms");
    options.add_option("", cxxopts::Option("h,help", "display help info about application", cxxopts::value<bool>()->default_value("false")));

    cxxopts::ParseResult args;

    try {
        args = options.parse(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "failed to parse input arguments " << e.what();
        return 1;
    }

    if (args["help"].as<bool>()) {
        std::cout << options.help();
        return 0;
    }

    return 0;
}