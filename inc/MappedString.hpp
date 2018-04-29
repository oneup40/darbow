#pragma once

// Copyright 2018 oneup

#include <string>
#include <unordered_map>

namespace darbow {

class MappedString {
public:
    MappedString &operator+=(char32_t ch) {
        cooked_ += ch;
        return *this;
    }

private:
    std::u32string cooked_;
    std::unordered_map<std::size_t, std::string> raw_lookup_;
};

}   // namespace darbow
