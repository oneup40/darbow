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
if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
    add_compile_options(-O0 -g)
else ()
    add_compile_options(-O3)
endif ()

include_directories(inc)

add_library(darbow STATIC inc/task/CompilationTask.hpp inc/task/PipelineCompilationTask.hpp src/task/ReadSourceCompilationTask.cc inc/task/ReadSourceCompilationTask.hpp src/task/MapUTF8SourceTask.cc inc/task/MapUTF8SourceTask.hpp inc/error/UnicodeError.hpp inc/task/LineSplicerTask.hpp src/task/LineSplicerTask.cc src/task/CommentReplacerTask.cc inc/task/CommentReplacerTask.hpp src/error/ParserError.cc inc/error/ParserError.hpp src/error/UnicodeError.cc)

add_subdirectory(test)
