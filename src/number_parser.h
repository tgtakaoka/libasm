/*
 * Copyright 2024 Tadashi G. Takaoka
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __LIBASM_NUMBER_PARSER_H__
#define __LIBASM_NUMBER_PARSER_H__

#include "error_reporter.h"
#include "str_scanner.h"
#include "value.h"

namespace libasm {

/**
 * Parse text |scan| as a number.
 *
 * - Returns OK when |scan| is recognized as a valid number, and updates |scan| at the end of a
 *   number.
 * - Returns ILLEGAL_CONSTANT when |scan| seems a number but not ended as expected. |scan| is
 *   updated at the error.
 * - Returns NOT_AN_EXPECTED when |scan| doesn't look like a number. |scan| is unchanged.
 */
struct NumberParser {
    virtual Error parseNumber(StrScanner &scan, Value &val, Radix defaultRadix) const = 0;

    /**
     * Search |scan| as a |radix| based number with optional |suffix|.
     *
     * - Returns OK when |scan| looks a valid number, and |scan| is updated to the end of a number
     *   including |suffix|.
     * - Returns NOT_AN_EXPECTED when |scan| doesn't look like a number, or lacks |suffix|.
     */
    static Error scanNumberEnd(StrScanner &scan, Radix radix, char suffix = 0);
};

/**
 * Base for character prefixed number parser.
 */
struct PrefixNumberParser : NumberParser {
    PrefixNumberParser(char hex, char bin, char oct, char dec)
        : _hex(hex), _bin(bin), _oct(oct), _dec(dec) {}

    Error parseNumber(StrScanner &scan, Value &val, Radix defaultRadix) const override;

protected:
    const char _hex;
    const char _bin;
    const char _oct;
    const char _dec;

    virtual Radix hasPrefix(StrScanner &scan) const;
};

/**
`* C-style numbers are
 * - Decimal:     "[1-9][0-9]*"
 * - Hexadecimal: "0[xX][0-9A-Fa-f]+"
 * - Octal:       "0[0-7]*"
 * - Binary:      "0[bB][01]+"
 */
struct CStyleNumberParser final : NumberParser, Singleton<CStyleNumberParser> {
    Error parseNumber(StrScanner &scan, Value &val, Radix defaultRadix) const override;

private:
    Radix hasPrefix(StrScanner &scan, Radix defaultRadix) const;
};

/**
 * Intel style numbers are
 * - Decimal:     "[0-9]+"
 * - Hexadecimal: "([0-9]|0[A-Fa-f])[0-9A-Fa-f]*[hH]"
 * - Octal:       "[0-7]+[oOqQ]"
 * - Binary:      "[01]+[bB]"
 */
struct IntelNumberParser final : NumberParser, Singleton<IntelNumberParser> {
    Error parseNumber(StrScanner &scan, Value &val, Radix defaultRadix) const override;
    Error parseNumber(StrScanner &scan, Value &val, Radix radix, StrScanner &end) const;

    static Radix hasSuffix(StrScanner &scan);
};

/**
 * Motorola style numbers are
 * - Decimal:     "{&}[0-9]+"
 * - Hexadecimal: "$[0-9A-Fa-f]+"
 * - Octal:       "@[0-7]+"
 * - Binary:      "%[01]+"
 */
struct MotorolaNumberParser final : PrefixNumberParser, Singleton<MotorolaNumberParser> {
    MotorolaNumberParser() : PrefixNumberParser('$', '%', '@', '&') {}
};

/**
 * Zilog style numbers are
 * - Decimal:     "[0-9]+"
 * - Hexadecimal: "%[0-9A-Fa-f]+"
 * - Octal:       "%(8)[0-7]+"
 * - Binary:      "%(2)[01]+"
 */
struct ZilogNumberParser final : NumberParser, Singleton<ZilogNumberParser> {
    Error parseNumber(StrScanner &scan, Value &val, Radix defaultRadix) const override;

private:
    Radix hasPrefix(StrScanner &scan) const;
};

/**
 * Texas Instrument style numbers is:
 * - '>hh' as hexadecimal.
 * - '?bb' as binary.
 */
struct TexasNumberParser final : PrefixNumberParser, Singleton<TexasNumberParser> {
    TexasNumberParser() : PrefixNumberParser('>', '?', 0, 0) {}
};

/**
 * IBM style numbers are
 * - Decimal:     "[0-9]+|[dD]'[0-9]+'"
 * - Hexadecimal: "[hH]'[0-9A-Fa-f]+'"
 * - Octal:       "[oO]'[0-7]+'"
 * - Binary:      "[bB]'[01]+'"
 */
struct IbmNumberParser : PrefixNumberParser {
    IbmNumberParser(char hex, char bin, char oct, char dec)
        : PrefixNumberParser(hex, bin, oct, dec) {}
    Error parseNumber(StrScanner &scan, Value &val, Radix defaultRadix) const override;
};

/** RCA style numbers are the same as IBM plus '#hh' for hexadecimal. */
struct RcaNumberParser final : IbmNumberParser, Singleton<RcaNumberParser> {
    RcaNumberParser() : IbmNumberParser('X', 'B', 0, 'D') {}
    Error parseNumber(StrScanner &scan, Value &val, Radix defaultRadix) const override;
};

/**
 * Fairchild style numbers are the same as IBM plus '$hh' for hexadecimal.
 */
struct FairchildNumberParser final : IbmNumberParser, Singleton<FairchildNumberParser> {
    FairchildNumberParser() : IbmNumberParser('H', 'B', 'O', 'D') {}
    Error parseNumber(StrScanner &scan, Value &val, Radix defaultRadix) const override;
};

struct PanasonicNumberParser final : IbmNumberParser, Singleton<PanasonicNumberParser> {
    PanasonicNumberParser() : IbmNumberParser('X', 0, 0, 0) {}
};

/** A numbers are the same as IBM except H'hh' for hexadecimal. */
struct SigneticsNumberParser final : IbmNumberParser, Singleton<SigneticsNumberParser> {
    SigneticsNumberParser() : IbmNumberParser('H', 'B', 'O', 'D') {}
};

/**
 * National style numbers are
 * - Decimal:     "[0-9]+|[dD]'[0-9]+'?"
 * - Hexadecimal: "[hH]'[0-9A-Fa-f]+'?"
 * - Octal:       "[oO]'[0-7]+'?"
 * - Binary:      "[bB]'[01]+'?"
 */
struct NationalNumberParser : IbmNumberParser {
    NationalNumberParser(char hex, char bin, char oct, char dec)
        : IbmNumberParser(hex, bin, oct, dec) {}
    Error parseNumber(StrScanner &scan, Value &val, Radix defaultRadix) const override;

private:
    Radix hasPrefix(StrScanner &scan) const override;
};

struct Ins80xxNumberParser final : NationalNumberParser, Singleton<Ins80xxNumberParser> {
    Ins80xxNumberParser() : NationalNumberParser('X', 0, 0, 0) {}
};

struct Ns32000NumberParser final : NationalNumberParser, Singleton<Ns32000NumberParser> {
    Ns32000NumberParser() : NationalNumberParser(/* 'X','H' */ 0, 'B', /* 'O' or */ 'Q', 'D') {}
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
