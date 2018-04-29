PROJECT(darbow)
cmake_minimum_required(VERSION 3.0)

set(HOST_PLATFORM "Linux" CACHE STRING "Platform that compiler itself will run on")
set_property(CACHE HOST_PLATFORM PROPERTY STRINGS Linux)

if (${HOST_PLATFORM} STREQUAL Linux)
    add_definitions(-DHOST_PLATFORM_DEFINED)
    add_definitions(-DHOST_PLATFORM_LINUX)
elseif (${HOST_PLATFORM})
    message(FATAL_ERROR "Unsupported HOST_PLATFORM ${HOST_PLATFORM}")
else ()
    message(FATAL_ERROR "HOST_PLATFORM not configured")
endif ()

add_compile_options(-std=c++17 -Wall -Wextra -Wpedantic -Werror)

include_directories(inc)

add_library(darbow STATIC inc/CompilationTask.hpp inc/PipelineCompilationTask.hpp src/ReadSourceCompilationTask.cc inc/ReadSourceCompilationTask.hpp)

add_subdirectory(test)
