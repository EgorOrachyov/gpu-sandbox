cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(gpusandbox LANGUAGES CXX)

######################################################################
## public options

option(GPUSANDBOX_BUILD_OPENCL "Build library with opencl backend" YES)

######################################################################
## Internal definitions

set(GPUSANDBOX_DEFINES)
set(GPUSANDBOX_DEBUG NO)
set(GPUSANDBOX_RELEASE NO)
set(GPUSANDBOX_TARGET_LINUX NO)
set(GPUSANDBOX_TARGET_WINDOWS NO)
set(GPUSANDBOX_TARGET_MACOSX NO)
set(GPUSANDBOX_ARCH)

####################################################################
## Define platform
##  - WINDOWS = Windows Desktop
##  - MACOSX  = MacOS X
##  - LINUX   = Linux

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(GPUSANDBOX_TARGET_WINDOWS YES)
    set(GPUSANDBOX_ARCH "win_x64")
    list(APPEND GPUSANDBOX_DEFINES GPUSANDBOX_TARGET_WINDOWS)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(GPUSANDBOX_TARGET_LINUX YES)
    set(GPUSANDBOX_ARCH "linux_x64")
    list(APPEND GPUSANDBOX_DEFINES GPUSANDBOX_TARGET_LINUX)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(GPUSANDBOX_TARGET_MACOSX YES)
    if (NOT GPUSANDBOX_ARCH)
        if (NOT CMAKE_OSX_ARCHITECTURES)
            if ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "arm64")
                set(GPUSANDBOX_ARCH "macos_arm64")
            else ()
                set(GPUSANDBOX_ARCH "macos_x64")
            endif ()
        else ()
            if ("${CMAKE_OSX_ARCHITECTURES}" STREQUAL "arm64")
                set(GPUSANDBOX_ARCH "macos_arm64")
            else ()
                set(GPUSANDBOX_ARCH "macos_x64")
            endif ()
        endif ()
    endif ()
    message(STATUS "Build macos binaries for ${GPUSANDBOX_ARCH} architecture")
    list(APPEND GPUSANDBOX_DEFINES GPUSANDBOX_TARGET_MACOSX)
else ()
    message(FATAL_ERROR "Unsupported target platform")
endif ()

####################################################################
## Define Build type

if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(GPUSANDBOX_DEBUG YES)
    list(APPEND GPUSANDBOX_DEFINES GPUSANDBOX_DEBUG)
    message(STATUS "Build gpusandbox in debug mode (specified)")
elseif (CMAKE_BUILD_TYPE MATCHES Release)
    set(GPUSANDBOX_RELEASE YES)
    list(APPEND GPUSANDBOX_DEFINES GPUSANDBOX_RELEASE)
    message(STATUS "Build gpusandbox in release mode (specified)")
elseif (CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
    set(GPUSANDBOX_RELEASE YES)
    list(APPEND GPUSANDBOX_DEFINES GPUSANDBOX_DEBUG_RELEASE)
    message(STATUS "Build gpusandbox in release mode with debug info (specified)")
else ()
    set(GPUSANDBOX_RELEASE YES)
    set(CMAKE_BUILD_TYPE Release)
    list(APPEND GPUSANDBOX_DEFINES GPUSANDBOX_RELEASE)
    message(STATUS "Build gpusandbox in release mode (default, not specified)")
endif ()

##################################################################
## Compiler and language specifics

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (GPUSANDBOX_TARGET_WINDOWS)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        list(APPEND GPUSANDBOX_DEFINES GPUSANDBOX_MSVC)
    else ()
        message(FATAL_ERROR "Supported only MSVC compiler for Windows platform")
    endif ()
endif ()

##################################################################
## Target options and defs functions
## (use this functions to configure build targets)

function(gpusandbox_target_compile_definitions target)
    foreach (DEFINITION ${GPUSANDBOX_DEFINES})
        target_compile_definitions(${target} PUBLIC ${DEFINITION})
    endforeach ()

    if (GPUSANDBOX_TARGET_WINDOWS)
        target_compile_definitions(${target}
                PUBLIC WIN32
                PUBLIC _WIN32
                PUBLIC _WINDOWS
                PUBLIC UNICODE
                PUBLIC _UNICODE
                PUBLIC _CRT_SECURE_NO_WARNINGS
                PUBLIC _SCL_SECURE_NO_WARNINGS)
    endif ()
endfunction()

function(gpusandbox_target_compile_options target)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set(CMAKE_CXX_FLAGS_RELEASE "/MT")
        set(CMAKE_CXX_FLAGS_DEBUG "/MTd")

        target_compile_options(${target}
                PUBLIC /W1
                PUBLIC /std:c++17
                PUBLIC /source-charset:utf-8)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(${target}
                PUBLIC -Wall
                PUBLIC -Wextra
                PUBLIC -Wpedantic)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(${target}
                PUBLIC -Wall
                PUBLIC -Wextra
                PUBLIC -Wpedantic
                PUBLIC -Wno-inconsistent-missing-override)
    endif ()
endfunction()

function(gpusandbox_target_link_options target)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_link_options(${target}
                PUBLIC /INCREMENTAL:NO
                PUBLIC /machine:X64)
    endif ()
endfunction()

######################################################################
## Dependencies config

if (GPUSANDBOX_BUILD_OPENCL)
    if (GPUSANDBOX_TARGET_MACOSX)
        message(STATUS "Add standard Apple OpenCL package")
        find_package(OpenCL REQUIRED)

        add_library(OpenCLHeaders INTERFACE)
        add_library(OpenCL::Headers ALIAS OpenCLHeaders)
        target_include_directories(OpenCLHeaders INTERFACE ${OpenCL_INCLUDE_DIRS})

        add_library(OpenCL INTERFACE)
        target_include_directories(OpenCL INTERFACE ${OpenCL_INCLUDE_DIRS})
        target_link_libraries(OpenCL INTERFACE ${OpenCL_LIBRARIES})
    else ()
        message(STATUS "Add Khronos OpenCL C API header files")
        add_subdirectory(deps/opencl-headers)

        message(STATUS "Add Khronos OpenCL ICD loaded library")
        set(OPENCL_ICD_LOADER_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
        set(OPENCL_ICD_LOADER_BUILD_TESTING OFF CACHE BOOL "" FORCE)
        add_subdirectory(deps/opencl-icd-loader)
    endif ()


    message(STATUS "Add Khronos OpenCL HPP bindings")
    add_library(HeadersCpp INTERFACE)
    add_library(OpenCL::HeadersCpp ALIAS HeadersCpp)
    target_include_directories(HeadersCpp INTERFACE deps/opencl-headers-hpp/include)
    target_link_libraries(HeadersCpp INTERFACE OpenCL::Headers)
endif ()

message(STATUS "Add cxxopts as arguments parser for example applications")
set(CXXOPTS_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CXXOPTS_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(CXXOPTS_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
set(CXXOPTS_ENABLE_WARNINGS OFF CACHE BOOL "" FORCE)
set(CXXOPTS_USE_UNICODE_HELP OFF CACHE BOOL "" FORCE)
add_subdirectory(deps/cxxopts)

message(STATUS "Add stb image for image loading")
add_subdirectory(deps/stb)

######################################################################
## Source code configuration

add_executable(gpusandbox
        src/main.cpp
        src/color.hpp
        src/filter.hpp
        src/image.cpp
        src/image.hpp
        src/timer.cpp
        src/timer.hpp
        src/cpu_filter_flip.cpp
        src/cpu_filter_flip.hpp
        src/cpu_filter_blur.cpp
        src/cpu_filter_blur.hpp
        src/cpu_filter_motion.cpp
        src/cpu_filter_motion.hpp
        src/cpu_filter_sobel.cpp
        src/cpu_filter_sobel.hpp
        src/cpu_conv.hpp
        )

target_include_directories(gpusandbox PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(gpusandbox PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)

set_target_properties(gpusandbox PROPERTIES OUTPUT_NAME "gpusandbox_${GPUSANDBOX_ARCH}")
target_compile_definitions(gpusandbox PRIVATE GPUSANDBOX_EXPORTS)

target_link_libraries(gpusandbox PRIVATE cxxopts)
target_link_libraries(gpusandbox PRIVATE stb_image)

if (GPUSANDBOX_BUILD_OPENCL)
    target_link_libraries(gpusandbox PUBLIC OpenCL)
    target_link_libraries(gpusandbox PUBLIC OpenCL::HeadersCpp)
    target_compile_definitions(gpusandbox PUBLIC CL_TARGET_OPENCL_VERSION=120)
    list(APPEND GPUSANDBOX_DEFINES GPUSANDBOX_BUILD_OPENCL)
endif ()

gpusandbox_target_compile_definitions(gpusandbox)
gpusandbox_target_compile_options(gpusandbox)
gpusandbox_target_link_options(gpusandbox)