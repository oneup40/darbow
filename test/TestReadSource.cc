// Copyright 2018 oneup

#include "CompilationTask.hpp"
#include "ReadSourceCompilationTask.hpp"

#include <iostream>

class OutputCompilation : public darbow::CompilationFinisher<std::string> {
public:
    std::error_condition Run(const std::string &source) const override {
        std::cout << source;
        return {};
    }
};

int main(int argc, char **argv) {
    for (auto i = 1; i < argc; ++i) {
        std::string filename = argv[i];

        auto err = darbow::run_pipeline(filename, darbow::ReadSourceCompilationTask(),
                                        OutputCompilation());

        if (err) {
            std::cerr << "Error: " << err.message() << std::endl;
        }
    }
}
