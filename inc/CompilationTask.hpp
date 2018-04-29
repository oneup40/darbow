#pragma once

// Copyright 2018 oneup

#include <functional>
#include <system_error>

namespace darbow {

template<class Input, class Output>
class CompilationTask {
public:
    virtual std::error_condition Run(const Input &input, Output *output) const = 0;

    virtual ~CompilationTask() = default;
};

template<class Input, class Output>
static inline std::error_condition
run_pipeline(const Input &input, Output *output, const CompilationTask<Input, Output> &task) {
    return task.Run(input, output);
}

template<class Input, class Output, class T, class... Tasks>
static inline std::error_condition
run_pipeline(const Input &input, Output *output, const CompilationTask<Input, T> &task,
             const Tasks &&... tasks) {
    T result;
    auto err = task.Run(input, &result);

    if (!err) {
        err = run_pipeline(result, output, std::forward<const Tasks>(tasks)...);
    }

    return err;
}

}   // namespace darbow
