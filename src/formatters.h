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
#include "text_common.h"

namespace libasm {

/**
 * Decimal number formatter
 */
struct DecFormatter {
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
struct BinFormatter {
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
struct OctFormatter {
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
struct HexFormatter {
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
struct PrefixFormatter<FORMATTER, char> : FORMATTER {
    PrefixFormatter(char prefix) : FORMATTER(), _prefix(prefix) {}

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
struct SuffixFormatter<FORMATTER, char> : FORMATTER {
    SuffixFormatter(char suffix) : FORMATTER(), _suffix(suffix) {}

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
struct SurroundFormatter<FORMATTER, const /*PROGMEM*/ char *const, char> : FORMATTER {
    SurroundFormatter(const /*PROGMEM*/ char *const prefix_P, char suffix)
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
struct PrefixBinFormatter : PrefixFormatter<BinFormatter, char> {
    PrefixBinFormatter(char prefix) : PrefixFormatter(prefix) {}
};

struct PrefixStrBinFormatter : PrefixFormatter<BinFormatter, const char *const> {
    PrefixStrBinFormatter(const /*PROGMEM*/ char *const prefix_P) : PrefixFormatter(prefix_P) {}
};

/**
 * Octal number is prefixed with |prefix|.
 */
struct PrefixOctFormatter : PrefixFormatter<OctFormatter, char> {
    PrefixOctFormatter(char prefix) : PrefixFormatter(prefix) {}
};

struct PrefixStrOctFormatter : PrefixFormatter<OctFormatter, const char *const> {
    PrefixStrOctFormatter(const /*PROGMEM*/ char *prefix_P) : PrefixFormatter(prefix_P) {}
};

/**
 * Hexadecimal number is prefixed with |prefix|.
 */
struct PrefixHexFormatter : PrefixFormatter<HexFormatter, char> {
    PrefixHexFormatter(char prefix) : PrefixFormatter(prefix) {}
};

struct PrefixStrHexFormatter : PrefixFormatter<HexFormatter, const char *const> {
    PrefixStrHexFormatter(const /*PROGMEM*/ char *prefix_P) : PrefixFormatter(prefix_P) {}
};

/**
 * Binary number is suffixed with |suffix|.
 */

struct SuffixBinFormatter : SuffixFormatter<BinFormatter, char> {
    SuffixBinFormatter(char suffix) : SuffixFormatter(suffix) {}
};

/**
 * Octal number is suffixed with |suffix|.
 */
struct SuffixOctFormatter : SuffixFormatter<OctFormatter, char> {
    SuffixOctFormatter(char suffix) : SuffixFormatter(suffix) {}
};

/**
 * Hexadecimal number is suffixed with |suffix|. It is also prefixed with '0' when it starts with
 * non-digit letter.
 */
struct SuffixHexFormatter : SuffixFormatter<HexFormatter, char> {
    SuffixHexFormatter(char suffix) : SuffixFormatter(suffix) {}

    StrBuffer &format(StrBuffer &out, uint32_t val, uint8_t bits) const override;
};

/**
 * Binary number is surrounded by |prefix| and |suffix|.
 */
struct SurroundBinFormatter : SurroundFormatter<BinFormatter, const char *const, char> {
    SurroundBinFormatter(const /*PROGMEM*/ char *const prefix_P, const char suffix)
        : SurroundFormatter(prefix_P, suffix) {}
};

/**
 * Octal number is surrounded by |prefix| and |suffix|.
 */
struct SurroundOctFormatter : SurroundFormatter<OctFormatter, const char *const, char> {
    SurroundOctFormatter(const /*PROGMEM*/ char *const prefix_P, char suffix)
        : SurroundFormatter(prefix_P, suffix) {}
};

/**
 * Hexadecimal number is surrounded by |prefix| and |suffix|.
 */
struct SurroundHexFormatter : SurroundFormatter<HexFormatter, const char *const, char> {
    SurroundHexFormatter(const /*PROGMEM*/ char *const prefix_P, char suffix)
        : SurroundFormatter(prefix_P, suffix) {}
};

/**
 * C-Style binary number is "0b11"
 */
struct CStyleBinFormatter : PrefixStrBinFormatter, Singleton<CStyleBinFormatter> {
    CStyleBinFormatter() : PrefixStrBinFormatter(PSTR("0b")) {}
    StrBuffer &format(StrBuffer &out, uint32_t val, uint8_t bits) const override {
        return BinFormatter::format(out.rtext_P(_prefix_P), val, bits);
    }
};

/**
 * C-Style octal number is "0377"
 */
struct CStyleOctFormatter final : PrefixOctFormatter, Singleton<CStyleOctFormatter> {
    CStyleOctFormatter() : PrefixOctFormatter('0') {}
};

/**
 * C-Style hexadecimal number is "0xFF"
 */
struct CStyleHexFormatter final : PrefixStrHexFormatter, Singleton<CStyleHexFormatter> {
    CStyleHexFormatter() : PrefixStrHexFormatter(PSTR("0x")) {}
    StrBuffer &format(StrBuffer &out, uint32_t val, uint8_t bits) const override {
        return HexFormatter::format(out.rtext_P(_prefix_P), val, bits);
    }
};

struct IntelBinFormatter final : SuffixBinFormatter, Singleton<IntelBinFormatter> {
    IntelBinFormatter() : SuffixBinFormatter('B') {}
};

struct IntelOctFormatter final : SuffixOctFormatter, Singleton<IntelOctFormatter> {
    IntelOctFormatter() : SuffixOctFormatter('Q') {}
};

struct IntelHexFormatter final : SuffixHexFormatter, Singleton<IntelHexFormatter> {
    IntelHexFormatter() : SuffixHexFormatter('H') {}
};

struct MotorolaBinFormatter final : PrefixBinFormatter, Singleton<MotorolaBinFormatter> {
    MotorolaBinFormatter() : PrefixBinFormatter('%') {}
};

struct MotorolaOctFormatter final : PrefixOctFormatter, Singleton<MotorolaOctFormatter> {
    MotorolaOctFormatter() : PrefixOctFormatter('@') {}
};

struct MotorolaHexFormatter final : PrefixHexFormatter, Singleton<MotorolaHexFormatter> {
    MotorolaHexFormatter() : PrefixHexFormatter('$') {}
};

struct FairchildBinFormatter final : SurroundBinFormatter, Singleton<FairchildBinFormatter> {
    FairchildBinFormatter() : SurroundBinFormatter(text::common::PSTR_B_DASH, '\'') {}
};

struct FairchildOctFormatter final : SurroundOctFormatter, Singleton<FairchildOctFormatter> {
    FairchildOctFormatter() : SurroundOctFormatter(text::common::PSTR_O_DASH, '\'') {}
};

struct FairchildHexFormatter final : SurroundHexFormatter, Singleton<FairchildHexFormatter> {
    FairchildHexFormatter() : SurroundHexFormatter(text::common::PSTR_H_DASH, '\'') {}
};

struct NationalBinFormatter final : PrefixStrBinFormatter, Singleton<NationalBinFormatter> {
    NationalBinFormatter() : PrefixStrBinFormatter(text::common::PSTR_B_DASH) {}
};

struct NationalOctFormatter final : PrefixStrOctFormatter, Singleton<NationalOctFormatter> {
    NationalOctFormatter() : PrefixStrOctFormatter(text::common::PSTR_O_DASH) {}
};

struct NationalHexFormatter final : PrefixStrHexFormatter, Singleton<NationalHexFormatter> {
    NationalHexFormatter() : PrefixStrHexFormatter(text::common::PSTR_X_DASH) {}
};

struct TexasBinFormatter final : PrefixBinFormatter, Singleton<TexasBinFormatter> {
    TexasBinFormatter() : PrefixBinFormatter('?') {}
};

struct TexasHexFormatter final : PrefixHexFormatter, Singleton<TexasHexFormatter> {
    TexasHexFormatter() : PrefixHexFormatter('>') {}
};

struct ZilogBinFormatter final : PrefixStrBinFormatter, Singleton<ZilogBinFormatter> {
    ZilogBinFormatter() : PrefixStrBinFormatter(text::common::PSTR_PERCENT_2) {}
};

struct ZilogOctFormatter final : PrefixStrOctFormatter, Singleton<ZilogOctFormatter> {
    ZilogOctFormatter() : PrefixStrOctFormatter(text::common::PSTR_PERCENT_8) {}
};

struct ZilogHexFormatter final : PrefixHexFormatter, Singleton<ZilogHexFormatter> {
    ZilogHexFormatter() : PrefixHexFormatter('%') {}
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
