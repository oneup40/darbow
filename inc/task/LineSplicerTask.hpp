#pragma once

// Copyright 2018 oneup

#include "CompilationTask.hpp"

namespace darbow {

class LineSplicerTask : public CompilationTask<std::string, std::string> {
public:
    std::error_condition Run(const std::string &input, std::string *output) const;
};

}   // namespace darbow
