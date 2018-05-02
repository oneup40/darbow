// Copyright 2018 oneup

#include "catch.hpp"

#include "error/ParserError.hpp"
#include "task/CommentReplacerTask.hpp"

using namespace darbow;

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


TEST_CASE("Comment replacer", "[comment-replacer]") {
    CommentReplacerTask task;
    std::string output;

    SECTION("Basic no comments") {
        std::string input("abcdefg\n");
        auto err = task.Run(input, &output);
        REQUIRE(!err);
        REQUIRE(output == "abcdefg\n");
    }

    SECTION("Basic multi-line comment") {
        std::string input("abc/*def*/ghi\n");
        auto err = task.Run(input, &output);
        REQUIRE(!err);
        REQUIRE(output == "abc ghi\n");
    }

    SECTION("Basic single-line comment") {
        std::string input("abc//def\nghi\n");
        auto err = task.Run(input, &output);
        REQUIRE(!err);
        REQUIRE(output == "abc \nghi\n");
    }

    SECTION("Comment in middle of spaces") {
        std::string input("abc /*def*/ ghi\n");
        auto err = task.Run(input, &output);
        REQUIRE(!err);
        REQUIRE(output == "abc   ghi\n");
    }

    SECTION("Trailing multi-line comment") {
        std::string input("abcdef/*\n");
        auto err = task.Run(input, &output);
        REQUIRE(err == ParserError::UnterminatedComment);
    }

    SECTION("Half-unterminated multi-line comment") {
        std::string input("abcdef/*ghi*\n");
        auto err = task.Run(input, &output);
        REQUIRE(err == ParserError::UnterminatedComment);
    }

    SECTION("Overlapping multi-line comment termination") {
        std::string input("abc/*/def\n");
        auto err = task.Run(input, &output);
        REQUIRE(err == ParserError::UnterminatedComment);
    }

    SECTION("Multi-line comment in single-line comment") {
        std::string input("abc//def/*\n");
        auto err = task.Run(input, &output);
        REQUIRE(!err);
        REQUIRE(output == "abc \n");
    }

    SECTION("Single-line comment in multi-line comment") {
        std::string input("abc/*def//ghi*/jkl\n");
        auto err = task.Run(input, &output);
        REQUIRE(!err);
        REQUIRE(output == "abc jkl\n");
    }

    SECTION("Multi-line comment terminator overlapping single-line") {
        std::string input("abc/*def*//ghi\n");
        auto err = task.Run(input, &output);
        REQUIRE(!err);
        REQUIRE(output == "abc /ghi\n");
    }

    SECTION("Multi-line comment spanning multiple lines") {
        std::string input("abc/*\ndef\nghi\njkl\n*/mno\n");
        auto err = task.Run(input, &output);
        REQUIRE(!err);
        REQUIRE(output == "abc mno\n");
    }
}
