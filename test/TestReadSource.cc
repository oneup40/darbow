// Copyright 2018 oneup

#include "CompilationTask.hpp"
#include "ReadSourceCompilationTask.hpp"

#include <iostream>

class OutputCompilationTask : public darbow::CompilationTask<std::string, int> {
public:
    std::error_condition Run(const std::string &source, int *dummy) const override {
        *dummy = 0;
        std::cout << source;
        return {};
    }
};

int main(int argc, char **argv) {
    for (auto i = 1; i < argc; ++i) {
        std::string filename = argv[i];
        int dummy;

        auto err = darbow::run_pipeline(filename, &dummy, darbow::ReadSourceCompilationTask(),
                                        OutputCompilationTask());

        if (err) {
            std::cerr << "Error: " << err.message() << std::endl;
        }
    }
}
