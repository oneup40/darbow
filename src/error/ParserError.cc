// Copyright 2018 oneup

#include "error/ParserError.hpp"

namespace darbow {

const static ParserErrorCategory gParserErrorCategory;

const ParserErrorCategory& parser_error_category() { return gParserErrorCategory; }

}   // namespace darbow
