#pragma once

// Copyright 2018 oneup

#include "task/CompilationTask.hpp"
#include "MappedString.hpp"

#include <string>

namespace darbow {

class MapUTF8SourceTask : public CompilationTask<std::string, std::string> {
public:
    std::error_condition Run(const std::string &input, std::string *output) const;
};

}   // namespace darbow
