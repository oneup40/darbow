// Copyright 2018 oneup

#include "error/UnicodeError.hpp"

namespace darbow {

const static UnicodeErrorCategory gUnicodeErrorCategory;

const UnicodeErrorCategory &unicode_error_category() { return gUnicodeErrorCategory; }

}   // namespace darbow
