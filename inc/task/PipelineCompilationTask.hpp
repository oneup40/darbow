#pragma once

// Copyright 2018 oneup

#include "task/CompilationTask.hpp"

namespace darbow {

namespace {

template<typename... Tasks>
struct PipelineTraits {
    using Tuple = std::tuple<Tasks...>;
    static constexpr size_t Size = std::tuple_size<Tuple>::value;
    using First = std::tuple_element_t<0, Tuple>;
    using Last = std::tuple_element_t<Size - 1, Tuple>;
    using Input = typename First::Input;
    using Output = typename Last::Output;
    using Base = CompilationTask<Input, Output>;

    //static constexpr Head&& get_head(Tuple &&tasks) { return std::get<0>(std::forward<Tuple>(tasks)); }

    //static constexpr First&& first_task(Tuple &&tasks) { return std::get<0>(std::forward<Tuple>(tasks)); }
    //static constexpr First&& first_task(Tuple &&tasks) { return std::get<0>(std::forward<Tuple>(tasks)); }
};

template<>
struct PipelineTraits<> {
};

template<typename... Tasks>
using PipelineTuple = typename PipelineTraits<Tasks...>::Tuple;

template<typename... Tasks>
inline constexpr std::size_t PipelineSize = PipelineTraits<Tasks...>::Size;

template<typename... Tasks>
using PipelineFirst = typename PipelineTraits<Tasks...>::First;

template<typename... Tasks>
using PipelineLast = typename PipelineTraits<Tasks...>::Last;

template<typename... Tasks>
using PipelineInput = typename PipelineTraits<Tasks...>::Input;

template<typename... Tasks>
using PipelineOutput = typename PipelineTraits<Tasks...>::Output;

template<typename... Tasks>
using PipelineBase = typename PipelineTraits<Tasks...>::Base;

template<typename Task>
std::error_condition
run_pipeline(const typename Task::Input &input, typename Task::Output *output, const Task &task) {
    return task.Run(input, output);
}

template<typename Task, typename... Tasks>
std::error_condition run_pipeline(const PipelineInput<Tasks...> &input,
                                  PipelineOutput<Tasks...> *output,
                                  const Task &task,
                                  const Tasks &... tasks) {
    typename Task::Output result;

    auto err = task.Run(input, &result);
    if (!err) {
        err = run_pipeline(result, output, tasks...);
    }

    return err;
}


}   // namespace (anonymous)

template<typename... Tasks>
class PipelineCompilationTask : public PipelineTraits<Tasks...>::Base {
public:
    using Traits = PipelineTraits<Tasks...>;
    using Input = typename Traits::Input;
    using Output = typename Traits::Output;

    explicit PipelineCompilationTask(Tasks &&... tasks) : tasks_(std::forward<Tasks>(tasks)...) {}

    std::error_condition Run(const Input &input, Output *output) const {
        return std::apply([&input, output](const auto &... tasks) {
            return run_pipeline(input, output, tasks...);
        }, tasks_);
    }

private:
    PipelineTuple<Tasks...> tasks_;
};

template<typename... Tasks>
PipelineCompilationTask<Tasks...> make_pipeline(Tasks &&... tasks) {
    return PipelineCompilationTask<Tasks...>(std::forward<Tasks>(tasks)...);
}

}   // namespace darbow
