// Copyright 2018 oneup

#include "task/ReadSourceCompilationTask.hpp"

#include <fstream>

namespace darbow {

std::error_condition
ReadSourceCompilationTask::Run(const std::string &filename, std::string *source) const {
    std::ifstream fin(filename);
    if (!fin.good()) {
        return make_error_condition(std::errc::no_such_file_or_directory);
    }

    fin.seekg(0, std::ios_base::end);
    auto maybe_total = fin.tellg();
    if (maybe_total < 0) {
        return make_error_condition(std::errc::io_error);
    }

    std::size_t total = maybe_total;

    fin.seekg(0);

    source->resize(total);
    fin.read(&(*source)[0], total);

    maybe_total = fin.gcount();
    if (maybe_total < 0) {
        source->clear();
        return make_error_condition(std::errc::io_error);
    }

    total = maybe_total;
    source->resize(total);

    return {};
}

}   // namespace darbow
