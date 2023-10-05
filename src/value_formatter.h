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

#include "formatters.h"
#include "str_buffer.h"

namespace libasm {

struct ValueFormatter final {
    struct Plugins : Singleton<Plugins> {
        virtual const DecFormatter &dec() const;
        virtual const BinFormatter &bin() const;
        virtual const OctFormatter &oct() const;
        virtual const HexFormatter &hex() const;
        virtual char locationSymbol() const;
        virtual const /*PROGMEM*/ char *lineComment_P() const;

        static const Plugins &cstyle();
        static const Plugins &fairchild();
        static const Plugins &intel();
        static const Plugins &motorola();
        static const Plugins &national();
        static const Plugins &texas();
        static const Plugins &zilog();
    };

    ValueFormatter(const Plugins &plugins = Plugins::singleton())
        : _dec(plugins.dec()),
          _bin(plugins.bin()),
          _oct(plugins.oct()),
          _hex(plugins.hex()),
          _locationSymbol(plugins.locationSymbol()),
          _lineComment_P(plugins.lineComment_P()) {}

    /**
     * Convert |val| as |bits| decimal integer.  Treat |val| as signed integer when |bits| is
     * negative.  Leading zero will be suppressed.
     */
    StrBuffer &formatDec(StrBuffer &out, uint32_t val, int8_t bits = 0) const;

    /**
     * Convert |val| as |bits| binary integer.  Treat |val| as signed integer when |bits| is
     * negative.
     */
    StrBuffer &formatBin(StrBuffer &out, uint32_t val, int8_t bits = 0) const;

    /**
     * Convert |val| as |bits| octal integer.  Treat |val| as signed integer when |bits| is
     * negative.
     */
    StrBuffer &formatOct(StrBuffer &out, uint32_t val, int8_t bits = 0) const;

    /**
     * Convert |val| as |bits| hexadecimal integer.  Treat |val| as signed integer when |bits| is
     * negative.
     */
    StrBuffer &formatHex(StrBuffer &out, uint32_t val, int8_t bits = 0) const;

    /**
     * Calculate absolute value of |val| as |bits| integer, and return true if negative.  Treat
     * |val| as signed integer when |bits| is negative.
     */
    static bool absolute(uint32_t &val, int8_t bits);

    char locationSymbol() const { return _locationSymbol; }
    const /*PROGMEM*/ char *lineComment_P() const { return _lineComment_P; }

private:
    const DecFormatter &_dec;
    const BinFormatter &_bin;
    const OctFormatter &_oct;
    const HexFormatter &_hex;
    const char _locationSymbol;
    const /*PROGMEM*/ char *_lineComment_P;

    static uint32_t makePositive(StrBuffer &out, uint32_t val, int8_t bits);
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
