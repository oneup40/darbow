// Copyright 2018 oneup

#include "catch.hpp"

#include "error/UnicodeError.hpp"
#include "task/MapUTF8SourceTask.hpp"

using namespace darbow;

TEST_CASE("Map UTF-8 source", "[map-utf8-source]") {
    MapUTF8SourceTask task;
    std::string output;

    SECTION("Basic 7-bit ASCII") {
        std::string input("abcdefg");
        auto err = task.Run(input, &output);
        REQUIRE(!err);
        REQUIRE(output == "abcdefg");
    }

    SECTION("7-bit ASCII non-basic") {
        std::string input("abc@def`ghi$jkl");
        auto err = task.Run(input, &output);
        REQUIRE(!err);
        REQUIRE(output == "abc\\u0040def\\u0060ghi\\u0024jkl");
    }

    SECTION("0x80 - 0x7ff") {
        std::string input("\xc2\x80\xdf\xbf");
        auto err = task.Run(input, &output);
        REQUIRE(!err);
        REQUIRE(output == "\\u0080\\u07ff");
    }

    SECTION("0x800 - 0xffff") {
        std::string input("\xe0\xa0\x80\xef\xbf\xbf");
        auto err = task.Run(input, &output);
        REQUIRE(!err);
        REQUIRE(output == "\\u0800\\uffff");
    }

    SECTION("0x10000 - 0x10ffff") {
        std::string input("\xf0\x90\x80\x80\xf4\x8f\xbf\xbf");
        auto err = task.Run(input, &output);
        REQUIRE(!err);
        REQUIRE(output == "\\U00010000\\U0010ffff");
    }

    SECTION("5 byte sequences not allowed") {
        std::string input("\xf8\x8f\x8f\x8f\x8f");
        auto err = task.Run(input, &output);
        REQUIRE(err == UnicodeError::InvalidUTF8Sequence);
    }

    SECTION("6 byte sequences not allowed") {
        std::string input("\xfc\x8f\x8f\x8f\x8f\x8f");
        auto err = task.Run(input, &output);
        REQUIRE(err == UnicodeError::InvalidUTF8Sequence);
    }

    SECTION("7 byte sequences not allowed") {
        std::string input("\xfe\x8f\x8f\x8f\x8f\x8f\x8f");
        auto err = task.Run(input, &output);
        REQUIRE(err == UnicodeError::InvalidUTF8Sequence);
    }

    SECTION("8 byte sequences not allowed") {
        std::string input("\xff\x8f\x8f\x8f\x8f\x8f\x8f\x8f");
        auto err = task.Run(input, &output);
        REQUIRE(err == UnicodeError::InvalidUTF8Sequence);
    }

    SECTION("Truncated multi-byte sequence not allowed") {
        std::string input("\xc0");
        auto err = task.Run(input, &output);
        REQUIRE(err == UnicodeError::InvalidUTF8Sequence);
    }
}
