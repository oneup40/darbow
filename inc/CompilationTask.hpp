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

template<class Input>
class CompilationFinisher {
public:
    virtual std::error_condition Run(const Input &input) const = 0;

    virtual ~CompilationFinisher() = default;
};

template<class Input>
static inline std::error_condition
run_pipeline(const Input &input, const CompilationFinisher<Input> &task) {
    return task.Run(input);
}


template<class Input, class T>
static inline std::error_condition
run_pipeline(const Input &input, const CompilationTask<Input, T> &task,
             const CompilationFinisher<T> &finisher) {
    T result;
    auto err = task.Run(input, &result);

    if (!err) {
        err = run_pipeline(result, finisher);
    }

    return err;
}

template<class Input, class T, class... Tasks>
static inline std::error_condition
run_pipeline(const Input &input, const CompilationTask<Input, T> &task, const Tasks &... tasks,
             const CompilationFinisher<Input> &finisher) {
    T result;
    auto err = task.Run(input, &result);

    if (!err) {
        err = run_pipeline(result, tasks..., finisher);
    }

    return err;
}

}   // namespace darbow
