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
    /** Format unsigned |val| as |width| bit binary. */
    virtual StrBuffer &format(StrBuffer &out, uint32_t val, uint8_t width) const;

protected:
    /**
     * Helper function to format unsigned |val| as |width| bit binary in reverse digit order.
     */
    static StrBuffer &outBin(StrBuffer &out, uint32_t val, uint8_t width);
};

/**
 * Octal number formatter
 */
struct OctFormatter : Singleton<OctFormatter> {
    /** Format unsigned |val| as |width| bit hexadecimal. */
    virtual StrBuffer &format(StrBuffer &out, uint32_t val, uint8_t width) const;

protected:
    /**
     * Helper function to format unsigned |val| as |width| bit octal in reverse digit order.
     */
    static StrBuffer &outOct(StrBuffer &out, uint32_t val, uint8_t width);
};

/**
 * Hexadecimal number formatter
 */
struct HexFormatter : Singleton<HexFormatter> {
    /** Format unsigned |val| as |width| bit hexadecimal. */
    virtual StrBuffer &format(StrBuffer &out, uint32_t val, uint8_t width) const;

protected:
    /**
     * Helper function to format unsigned |val| as |width| bit hexadecimal in reverse digit order.
     */
    static StrBuffer &outHex(StrBuffer &out, uint32_t val, uint8_t width);
};

/**
 * Binary number is prefixed with |prefix|.
 */
struct PrefixBinFormatter final : BinFormatter {
    PrefixBinFormatter(const /*PROGMEM*/ char *prefix_P) : _prefix_P(prefix_P) {}

    StrBuffer &format(StrBuffer &out, uint32_t val, uint8_t width) const override;

protected:
    const char *const _prefix_P;
};

/**
 * Octal number is prefixed with |prefix|.
 */
struct PrefixOctFormatter final : OctFormatter {
    PrefixOctFormatter(const /*PROGMEM*/ char *prefix_P) : _prefix_P(prefix_P) {}

    StrBuffer &format(StrBuffer &out, uint32_t val, uint8_t width) const override;

protected:
    const char *const _prefix_P;
};

/**
 * Hexadecimal number is prefixed with |prefix|.
 */
struct PrefixHexFormatter final : HexFormatter {
    PrefixHexFormatter(const /*PROGMEM*/ char *prefix_P) : _prefix_P(prefix_P) {}

    StrBuffer &format(StrBuffer &out, uint32_t val, uint8_t width) const override;

protected:
    const char *const _prefix_P;
};

/**
 * Binary number is suffixed with |suffix|.
 */
struct SuffixBinFormatter final : BinFormatter {
    SuffixBinFormatter(char suffix) : _suffix(suffix) {}

    StrBuffer &format(StrBuffer &out, uint32_t val, uint8_t width) const override;

private:
    const char _suffix;
};

/**
 * Octal number is suffixed with |suffix|.
 */
struct SuffixOctFormatter final : OctFormatter {
    SuffixOctFormatter(char suffix) : _suffix(suffix) {}

    StrBuffer &format(StrBuffer &out, uint32_t val, uint8_t width) const override;

private:
    const char _suffix;
};

/**
 * Hexadecimal number is suffixed with |suffix|. It is also prefixed with '0' when it starts with
 * non-digit letter.
 */
struct SuffixHexFormatter final : HexFormatter {
    SuffixHexFormatter(char suffix) : _suffix(suffix) {}

    StrBuffer &format(StrBuffer &out, uint32_t val, uint8_t width) const override;

private:
    const char _suffix;
};

/**
 * Binary number is surrounded by |prefix| and |suffix|.
 */
struct SurroundBinFormatter final : BinFormatter {
    SurroundBinFormatter(const /*PROGMEM*/ char *prefix_P, char suffix)
        : _prefix_P(prefix_P), _suffix(suffix) {}

    StrBuffer &format(StrBuffer &out, uint32_t val, uint8_t width) const override;

private:
    const char *const _prefix_P;
    const char _suffix;
};

/**
 * Octal number is surrounded by |prefix| and |suffix|.
 */
struct SurroundOctFormatter final : OctFormatter {
    SurroundOctFormatter(const /*PROGMEM*/ char *prefix_P, char suffix)
        : _prefix_P(prefix_P), _suffix(suffix) {}

    StrBuffer &format(StrBuffer &out, uint32_t val, uint8_t width) const override;

private:
    const char *const _prefix_P;
    const char _suffix;
};

/**
 * Hexadecimal number is surrounded by |prefix| and |suffix|.
 */
struct SurroundHexFormatter final : HexFormatter {
    SurroundHexFormatter(const /*PROGMEM*/ char *prefix_P, char suffix)
        : _prefix_P(prefix_P), _suffix(suffix) {}

    StrBuffer &format(StrBuffer &out, uint32_t val, uint8_t width) const override;

private:
    const char *const _prefix_P;
    const char _suffix;
};

/**
 * C-Style binary number is "0b11"
 */
struct CStyleBinFormatter final : BinFormatter {
    StrBuffer &format(StrBuffer &out, uint32_t val, uint8_t width) const override;
};

/**
 * C-Style octal number is "0377"
 */
struct CStyleOctFormatter final : OctFormatter {
    StrBuffer &format(StrBuffer &out, uint32_t val, uint8_t width) const override;
};

/**
 * C-Style hexadecimal number is "0xFF"
 */
struct CStyleHexFormatter final : HexFormatter {
    StrBuffer &format(StrBuffer &out, uint32_t val, uint8_t width) const override;
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
