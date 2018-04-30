// Copyright 2018 oneup

#include "task/LineUnsplicerTask.hpp"

namespace darbow {

std::error_condition LineUnsplicerTask::Run(const std::string &input, std::string *output) const {
    /* 2. Each instance of a backslash character (\) immediately followed by a new-line character is
     * deleted, splicing physical source lines to form logical source lines. Only the last backslash
     * on any physical source line shall be eligible for being part of such a splice. Except for
     * splices reverted in a raw string literal, if a splice results in a character sequence that
     * matches the syntax of a universal-character-name, the behavior is undefined. A source file
     * that is not empty and that does not end in a new-line character, or that ends in a new-line
     * character immediately preceded by a backslash character before any such splicing takes place,
     * shall be processed as if an additional new-line character were appended to the file.
     *
     * [lex.phases], para 1
     */

    std::size_t index = 0;
    while (index + 1 < input.length()) {
        if (input[index] == '\\' && input[index + 1] == '\n') {
            index += 2;
        } else {
            (*output) += input[index];
            ++index;
        }
    }

    if (index < input.length()) {
        (*output) += input[index];
        ++index;
    }

    if (output->back() != '\n') {
        (*output) += '\n';
    }

    return {};
}

}   // namespace darbow
