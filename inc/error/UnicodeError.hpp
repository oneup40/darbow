#pragma once

// Copyright 2018 oneup

#include <system_error>

namespace darbow {

enum class UnicodeError {
    InvalidUTF8Sequence = 1
};

}   // namespace darbow

template<>
struct std::is_error_condition_enum<darbow::UnicodeError> : public std::true_type {
};

namespace darbow {

class UnicodeErrorCategory : public std::error_category {
public:
    const char *name() const noexcept override { return "unicode"; }

    std::string message(int ev) const override {
        switch (static_cast<UnicodeError>(ev)) {
            case UnicodeError::InvalidUTF8Sequence:
                return "invalid utf-8 sequence";
            default:
                return "(unknown unicode error)";
        }
    }
};

const UnicodeErrorCategory &unicode_error_category();

static inline std::error_condition make_error_condition(UnicodeError e) {
    return {static_cast<int>(e), unicode_error_category()};
}

}
