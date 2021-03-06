#pragma once

// Copyright 2018 oneup

#include "task/CompilationTask.hpp"

#include <memory>
#include <string>

namespace darbow {

class ReadSourceCompilationTask : public CompilationTask<std::string, std::string> {
public:
    std::error_condition Run(const std::string &filename, std::string *source) const;
};

}   // namespace darbow
