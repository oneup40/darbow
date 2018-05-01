// Copyright 2018 oneup

#include "catch.hpp"

#include "error/UnicodeError.hpp"
#include "task/LineUnsplicerTask.hpp"

using namespace darbow;

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

TEST_CASE("Unsplice lines", "[line-unsplicer]") {
    LineUnsplicerTask task;
    std::string output;

    SECTION("Basic no spliced lines") {
        std::string input("abcdefg\n");
        auto err = task.Run(input, &output);
        REQUIRE(!err);
        REQUIRE(output == "abcdefg\n");
    }

    SECTION("Basic spliced line") {
        std::string input("abc\\\ndef\n");
        auto err = task.Run(input, &output);
        REQUIRE(!err);
        REQUIRE(output == "abcdef\n");
    }

    SECTION("double backslash") {
        std::string input("abc\\\\\ndef\n");
        auto err = task.Run(input, &output);
        REQUIRE(!err);
        REQUIRE(output == "abc\\def\n");
    }

    SECTION("double newline") {
        std::string input("abc\\\n\ndef\n");
        auto err = task.Run(input, &output);
        REQUIRE(!err);
        REQUIRE(output == "abc\ndef\n");
    }

    SECTION("double backslash double newline") {
        std::string input("abc\\\\\n\ndef\n");
        auto err = task.Run(input, &output);
        REQUIRE(!err);
        REQUIRE(output == "abc\\\ndef\n");
    }

    SECTION("no trailing newline") {
        std::string input("abcdef");
        auto err = task.Run(input, &output);
        REQUIRE(!err);
        REQUIRE(output == "abcdef\n");
    }

    SECTION("ends with splice") {
        std::string input("abcdef\\\n");
        auto err = task.Run(input, &output);
        REQUIRE(!err);
        REQUIRE(output == "abcdef\n");
    }

    SECTION("ends with backslash") {
        std::string input("abcdef\\");
        auto err = task.Run(input, &output);
        REQUIRE(!err);
        REQUIRE(output == "abcdef\\\n");
    }
}
