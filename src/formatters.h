/*
 * Copyright 2023 Tadashi G. Takaoka
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

#ifndef _FORMATTERS_H__
#define _FORMATTERS_H__

#include <stdint.h>

#include "str_buffer.h"

namespace libasm {

/**
 * Decimal number formatter
 */
struct DecFormatter : Singleton<DecFormatter> {
    /** Format unsigned |val| as decimal. */
    virtual StrBuffer &format(StrBuffer &out, uint32_t val) const;

protected:
    /**
     * Helper function to format unsigned |val| as decimal in reverse digit order.
     */
    static StrBuffer &outDec(StrBuffer &out, uint32_t val);
};

/**
 * Binary number formatter
 */
struct BinFormatter : Singleton<BinFormatter> {
    /** Format unsigned |val| as |bits| bit binary. */
    virtual StrBuffer &format(StrBuffer &out, uint32_t val, uint8_t bits) const;

protected:
    /**
     * Helper function to format unsigned |val| as |bits| bit binary in reverse digit order.
     */
    static StrBuffer &outBin(StrBuffer &out, uint32_t val, uint8_t bits);
};

/**
 * Octal number formatter
 */
struct OctFormatter : Singleton<OctFormatter> {
    /** Format unsigned |val| as |bits| bit hexadecimal. */
    virtual StrBuffer &format(StrBuffer &out, uint32_t val, uint8_t bits) const;

protected:
    /**
     * Helper function to format unsigned |val| as |bits| bit octal in reverse digit order.
     */
    static StrBuffer &outOct(StrBuffer &out, uint32_t val, uint8_t bits);
};

/**
 * Hexadecimal number formatter
 */
struct HexFormatter : Singleton<HexFormatter> {
    /** Format unsigned |val| as |bits| bit hexadecimal. */
    virtual StrBuffer &format(StrBuffer &out, uint32_t val, uint8_t bits) const;

protected:
    /**
     * Helper function to format unsigned |val| as |bits| bit hexadecimal in reverse digit order.
     */
    static StrBuffer &outHex(StrBuffer &out, uint32_t val, uint8_t bits);
};

/**
 * Formatter base for prefixed with |prefix|.
 */
template <typename FORMATTER, typename PREFIX>
struct PrefixFormatter {};

template <typename FORMATTER>
struct PrefixFormatter<FORMATTER, const char> : FORMATTER {
    PrefixFormatter(const char prefix) : FORMATTER(), _prefix(prefix) {}

    StrBuffer &format(StrBuffer &out, uint32_t val, uint8_t bits) const override final {
        return FORMATTER::format(out.letter(_prefix), val, bits);
    }

private:
    const char _prefix;
};

template <typename FORMATTER>
struct PrefixFormatter<FORMATTER, const /*PROGMEM*/ char *const> : FORMATTER {
    PrefixFormatter(const /*PROGMEM*/ char *const prefix_P) : FORMATTER(), _prefix_P(prefix_P) {}

    StrBuffer &format(StrBuffer &out, uint32_t val, uint8_t bits) const override {
        return FORMATTER::format(out.text_P(_prefix_P), val, bits);
    }

protected:
    const /*PROGMEM*/ char *const _prefix_P;
};

/**
 * Formatter base for suffixed with |suffix|.
 */
template <typename FORMATTER, typename SUFFIX>
struct SuffixFormatter {};

template <typename FORMATTER>
struct SuffixFormatter<FORMATTER, const char> : FORMATTER {
    SuffixFormatter(const char suffix) : FORMATTER(), _suffix(suffix) {}

    StrBuffer &format(StrBuffer &out, uint32_t val, uint8_t bits) const override {
        return FORMATTER::format(out, val, bits).letter(_suffix);
    }

protected:
    const char _suffix;
};

/**
 * Formatter base for surrounded with |prefix| and |suffix|.
 */
template <typename FORMATTER, typename PREFIX, typename SUFFIX>
struct SurroundFormatter {};

template <typename FORMATTER>
struct SurroundFormatter<FORMATTER, const /*PROGMEM*/ char *const, const char> : FORMATTER {
    SurroundFormatter(const /*PROGMEM*/ char *const prefix_P, const char suffix)
        : FORMATTER(), _prefix_P(prefix_P), _suffix(suffix) {}

    StrBuffer &format(StrBuffer &out, uint32_t val, uint8_t bits) const override final {
        return FORMATTER::format(out.text_P(_prefix_P), val, bits).letter(_suffix);
    }

private:
    const /*PROGMEM*/ char *const _prefix_P;
    const char _suffix;
};

/**
 * Binary number is prefixed with |prefix|.
 */
struct PrefixBinFormatter final : PrefixFormatter<BinFormatter, const char> {
    PrefixBinFormatter(const char prefix) : PrefixFormatter(prefix) {}
};

struct PrefixStrBinFormatter final : PrefixFormatter<BinFormatter, const char *const> {
    PrefixStrBinFormatter(const /*PROGMEM*/ char *const prefix_P) : PrefixFormatter(prefix_P) {}
};

/**
 * Octal number is prefixed with |prefix|.
 */
struct PrefixOctFormatter final : PrefixFormatter<OctFormatter, const char> {
    PrefixOctFormatter(const char prefix) : PrefixFormatter(prefix) {}
};

struct PrefixStrOctFormatter final : PrefixFormatter<OctFormatter, const char *const> {
    PrefixStrOctFormatter(const /*PROGMEM*/ char *prefix_P) : PrefixFormatter(prefix_P) {}
};

/**
 * Hexadecimal number is prefixed with |prefix|.
 */
struct PrefixHexFormatter final : PrefixFormatter<HexFormatter, const char> {
    PrefixHexFormatter(const char prefix) : PrefixFormatter(prefix) {}
};

struct PrefixStrHexFormatter final : PrefixFormatter<HexFormatter, const char *const> {
    PrefixStrHexFormatter(const /*PROGMEM*/ char *prefix_P) : PrefixFormatter(prefix_P) {}
};

/**
 * Binary number is suffixed with |suffix|.
 */

struct SuffixBinFormatter final : SuffixFormatter<BinFormatter, const char> {
    SuffixBinFormatter(const char suffix) : SuffixFormatter(suffix) {}
};

/**
 * Octal number is suffixed with |suffix|.
 */
struct SuffixOctFormatter final : SuffixFormatter<OctFormatter, const char> {
    SuffixOctFormatter(const char suffix) : SuffixFormatter(suffix) {}
};

/**
 * Hexadecimal number is suffixed with |suffix|. It is also prefixed with '0' when it starts with
 * non-digit letter.
 */
struct SuffixHexFormatter final : SuffixFormatter<HexFormatter, const char> {
    SuffixHexFormatter(const char suffix) : SuffixFormatter(suffix) {}

    StrBuffer &format(StrBuffer &out, uint32_t val, uint8_t bits) const override;
};

/**
 * Binary number is surrounded by |prefix| and |suffix|.
 */
struct SurroundBinFormatter final : SurroundFormatter<BinFormatter, const char *const, const char> {
    SurroundBinFormatter(const /*PROGMEM*/ char *const prefix_P, const char suffix)
        : SurroundFormatter(prefix_P, suffix) {}
};

/**
 * Octal number is surrounded by |prefix| and |suffix|.
 */
struct SurroundOctFormatter final : SurroundFormatter<OctFormatter, const char *const, const char> {
    SurroundOctFormatter(const /*PROGMEM*/ char *const prefix_P, char suffix)
        : SurroundFormatter(prefix_P, suffix) {}
};

/**
 * Hexadecimal number is surrounded by |prefix| and |suffix|.
 */
struct SurroundHexFormatter final : SurroundFormatter<HexFormatter, const char *const, const char> {
    SurroundHexFormatter(const /*PROGMEM*/ char *const prefix_P, char suffix)
        : SurroundFormatter(prefix_P, suffix) {}
};

/**
 * C-Style binary number is "0b11"
 */
struct CStyleBinFormatter final : PrefixFormatter<BinFormatter, const char *const> {
    CStyleBinFormatter() : PrefixFormatter(PSTR("0b")) {}

    StrBuffer &format(StrBuffer &out, uint32_t val, uint8_t bits) const override {
        return BinFormatter::format(out.rtext_P(_prefix_P), val, bits);
    }
};

/**
 * C-Style octal number is "0377"
 */
struct CStyleOctFormatter final : PrefixFormatter<OctFormatter, const char> {
    CStyleOctFormatter() : PrefixFormatter('0') {}
};

/**
 * C-Style hexadecimal number is "0xFF"
 */
struct CStyleHexFormatter final : PrefixFormatter<HexFormatter, const char *const> {
    CStyleHexFormatter() : PrefixFormatter(PSTR("0x")) {}

    StrBuffer &format(StrBuffer &out, uint32_t val, uint8_t bits) const override {
        return HexFormatter::format(out.rtext_P(_prefix_P), val, bits);
    }
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
