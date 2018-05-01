// Copyright 2018 oneup

#include "task/CompilationTask.hpp"
#include "task/LineSplicerTask.hpp"
#include "task/MapUTF8SourceTask.hpp"
#include "task/PipelineCompilationTask.hpp"
#include "task/ReadSourceCompilationTask.hpp"

#include <iostream>

int main(int argc, char **argv) {
    for (auto i = 1; i < argc; ++i) {
        std::string filename = argv[i];
        std::string source;

        auto pipeline = darbow::make_pipeline(
            darbow::ReadSourceCompilationTask(),
            darbow::MapUTF8SourceTask(),
            darbow::LineSplicerTask()
        );

        auto err = pipeline.Run(filename, &source);

        if (err) {
            std::cerr << "Error: " << err.message() << std::endl;
        } else {
            std::cout << source;
        }
    }
}
