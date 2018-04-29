#pragma once

// Copyright 2018 oneup

#include <functional>
#include <system_error>

namespace darbow {

template<class _Input, class _Output>
class CompilationTask {
public:
    using Input = _Input;
    using Output = _Output;
};

}   // namespace darbow
