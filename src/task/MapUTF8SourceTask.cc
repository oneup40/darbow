// Copyright 2018 oneup

#include "error/UnicodeError.hpp"
#include "task/MapUTF8SourceTask.hpp"

#include <iomanip>

namespace darbow {

namespace {

/*
 * The basic source character set consists of 96 characters: the space character, the control
 * characters representing horizontal tab, vertical tab, form feed, and new-line, plus the following
 * 91 graphical characters:
 * a b c d e f g h i j k l m n o p q r s t u v w x y z
 * A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
 * 0 1 2 3 4 5 6 7 8 9
 * _ { } [ ] # ( ) < > % : ; . ? * + - / ^ & | ~ ! = , \ " ’
 */

constexpr const std::array<bool, 128> is_ascii_basic = {
    /* 00 */
    false, false, false, false, false, false, false, false,
    false, true, true, true, true, false, false, false,
    false, false, false, false, false, false, false, false,
    false, false, false, false, false, false, false, false,

    /* 20 */
    true, true, true, true, false, true, true, true,
    true, true, true, true, true, true, true, true,
    true, true, true, true, true, true, true, true,
    true, true, true, true, true, true, true, true,

    /* 40 */
    false, true, true, true, true, true, true, true,
    true, true, true, true, true, true, true, true,
    true, true, true, true, true, true, true, true,
    true, true, true, true, true, true, true, true,

    /* 60 */
    false, true, true, true, true, true, true, true,
    true, true, true, true, true, true, true, true,
    true, true, true, true, true, true, true, true,
    true, true, true, true, true, true, true, false,
};

constexpr bool is_basic(char32_t ch) {
    if (ch >= 0x80) { return false; }
    return is_ascii_basic[ch];
}

std::string make_universal(char32_t ch) {
    std::stringstream oss;
    if (ch <= 0xffff) {
        oss << "\\u" << std::hex << std::setw(4) << std::setfill('0') << uint32_t(ch);
    } else {
        oss << "\\U" << std::hex << std::setw(8) << std::setfill('0') << uint32_t(ch);
    }

    return oss.str();
}

}   // namespace (anonymous)

std::error_condition MapUTF8SourceTask::Run(const std::string &input, std::string *output) const {
    /*
     * 1. Physical source file characters are mapped, in an implementation-defined manner, to the
     * basic source character set (introducing new-line characters for end-of-line indicators) if
     * necessary. The set of physical source file characters accepted is implementation-defined.
     * Any source file character not in the basic source character set (5.3) is replaced by the
     * universal-character-name that designates that character. An implementation may use any
     * internal encoding, so long as an actual extended character encountered in the source file,
     * and the same extended character expressed in the source file as a universal-character-name
     * (e.g., using the \uXXXX notation), are handled equivalently except where this replacement is
     * reverted (5.4) in a raw string literal.
     */

    std::size_t index = 0;

    while (index < input.size()) {
        char32_t c = static_cast<unsigned char>(input[index++]);

        if (is_basic(c)) {
            (*output) += c;
        } else {
            if (c >= 0x80) {
                unsigned nchars = 0;

                while (c & 0x80u) {
                    ++nchars;
                    c <<= 1;
                }

                --nchars;

                if (!nchars || index + nchars >= input.size()) {
                    return UnicodeError::InvalidUTF8Sequence;
                }

                c >>= nchars;
                for (auto i = 0u; i < nchars; ++i) {
                    c <<= 6;
                    c |= static_cast<unsigned char>(input[index + i]) & 0x3fu;
                }

                index += nchars;
            }

            (*output) += make_universal(c);
        }

        ++index;
    }

    return {};
}

}   // namespace darbow
