/*
 * Copyright 2020 Tadashi G. Takaoka
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

#ifndef _VALUE_FORMATTER_H__
#define _VALUE_FORMATTER_H__

#include <stdint.h>

#include "str_buffer.h"

namespace libasm {

/**
 * Hexadecimal number formatter
 */
struct HexFormatter {
    /** Format unsigned |val| as |width| bit hexadecimal. */
    virtual StrBuffer &format(StrBuffer &out, uint32_t val, uint8_t width) const;

    static const /*PROGMEM*/ char ZERO_X[] PROGMEM;
    static const /*PROGMEM*/ char DOLLAR[] PROGMEM;
    static const /*PROGMEM*/ char X_DASH[] PROGMEM;
    static const /*PROGMEM*/ char H_DASH[] PROGMEM;

protected:
    /**
     * Helper function to format unsigned |val| as |width| bit hexadecimal in reverse digit order.
     */
    static StrBuffer &outHex(StrBuffer &out, uint32_t val, uint8_t width);
};

/**
 * C-Style hexadecimal number is "0xFF"
 */
struct CStyleHexFormatter final : HexFormatter {
    StrBuffer &format(StrBuffer &out, uint32_t val, uint8_t width) const override;
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

struct ValueFormatter final {
    ValueFormatter(const HexFormatter &hexFormatter = DEFAULT_HEX) : _hexFormatter(hexFormatter) {}

    /**
     * Convert |val| as |bits| decimal integer.  Treat |val| as signed integer when |bits| is
     * negative.  Leading zero will be suppressed.
     */
    StrBuffer &formatDec(StrBuffer &out, uint32_t val, int8_t bits = 0) const;

    /**
     * Convert |val| as |bits| hexadecimal integer.  Treat |val| as signed integer when |bits| is
     * negative. Use uppercase letter to represent digit larger than 9 when |upperHex| is true.  Use
     * base 10 and zero suppress when |relax| is true and |val| is less than 32 in absolute value.
     */
    StrBuffer &formatHex(StrBuffer &out, uint32_t val, int8_t bits = 0, bool upperHex = false,
            bool relax = false) const;

private:
    const HexFormatter &_hexFormatter;

    static const HexFormatter DEFAULT_HEX;

    uint32_t makePositive(StrBuffer &out, uint32_t val, int8_t bits) const;
    static StrBuffer &outDec(StrBuffer &out, uint32_t val);
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
