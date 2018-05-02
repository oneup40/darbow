#pragma once

// Copyright 2018 oneup

#include <system_error>

namespace darbow {

enum class ParserError {
    UnterminatedComment = 1
};

}   // namespace darbow

template<>
struct std::is_error_condition_enum<darbow::ParserError> : public std::true_type {
};

namespace darbow {

class ParserErrorCategory : public std::error_category {
public:
    const char *name() const noexcept override { return "parser"; }

    std::string message(int ev) const override {
        switch (static_cast<ParserError>(ev)) {
            case ParserError::UnterminatedComment:
                return "unterminated comment";
            default:
                return "(unknown parser error)";
        }
    }
};

const ParserErrorCategory &parser_error_category();

static inline std::error_condition make_error_condition(ParserError e) {
    return {static_cast<int>(e), parser_error_category()};
}

}
