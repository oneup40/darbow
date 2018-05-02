// Copyright 2018 oneup

#include "error/ParserError.hpp"
#include "task/CommentReplacerTask.hpp"

#include <cassert>

namespace darbow {

static inline std::error_condition ReplaceMultilineComment(const std::string &input, std::string *output, unsigned *index) {
    assert(*index + 1 < input.length() && input[*index] == '/' && input[*index + 1] == '*');

    *output += ' ';
    *index += 2;

    bool found = false;
    while (*index + 1 < input.length()) {
        if (input[*index] == '*' && input[*index + 1] == '/') {
            found = true;
            *index += 2;
            break;
        }

        *index += 1;
    }

    std::error_condition err;
    if (!found) {
        err = ParserError::UnterminatedComment;
    }

    return err;
}

static inline std::error_condition ReplaceSinglelineComment(const std::string &input, std::string *output, unsigned *index) {
    assert(*index + 1 < input.length() && input[*index] == '/' && input[*index + 1] == '/');

    *output += ' ';
    *index += 2;

    // At this phase of compilation we are guaranteed to have a final newline!
    while (input[*index] != '\n') {
        *index += 1;
        assert(*index < input.size());
    }

    return {};
}

std::error_condition CommentReplacerTask::Run(const std::string &input, std::string *output) const {
    /* Each comment is replaced by one space character
     *
     * [lex.phases], para 1
     */

    /* The characters / * start a comment, which terminates with the characters * /. These comments
     * do not nest. The characters / / start a comment, which terminates immediately before the next
     * new-line character. If there is a form-feed or a vertical-tab character in such a comment,
     * only white-space characters shall appear between it and the new-line that terminates the
     * comment; no diagnostic is required. [ Note: The comment characters / /, / *, and * / have no
     * special meaning within a / / comment and are treated just like other characters. Similarly,
     * the comment characters / / and / * have no special meaning within a / * comment. end note ]
     *
     * [lex.comment], para 1
     * (note: spaces were inserted in between comment characters in quote above)
     */

    std::error_condition err;
    auto index = 0u;

    while (!err && index + 1 < input.length()) {
        if (input[index] == '/' && input[index+1] == '*') {
            err = ReplaceMultilineComment(input, output, &index);
        } else if (input[index] == '/' && input[index+1] == '/') {
            err = ReplaceSinglelineComment(input, output, &index);
        } else {
            *output += input[index];
            ++index;
        }
    }

    while (!err && index < input.length()) {
        *output += input[index++];
    }

    return err;
}

}   // namespace darbow
