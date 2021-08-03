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

class ValueFormatter {
public:
    ValueFormatter(char curSym = '.', bool cstyle = true)
        : _curSym(curSym), _cstyle(cstyle), _uppercase(false) {}

    void setCStyleHex(bool enable) { _cstyle = enable; }
    void setUppercase(bool uppercase) { _uppercase = uppercase; }
    void setCurrentOriginSymbol(char c) { _curSym = c; }
    char currentOriginSymbol() const { return _curSym; }

    /*
     * Convert |val| as |bits| decimal integer.  Treat |val| as signed
     * integer when |bits| is negative.  Leading zero will be
     * suppressed.
     */

    StrBuffer &formatDec(StrBuffer &out, uint32_t val, int8_t bits) const;
    /*
     * Convert |val| as |bits| hexadecimal integer.  Treat |val| as
     * signed integer when |bits| is negative. Yse base 10 and zero
     * suppress when |relax| is true and |val| is less than 32 in
     * absolute value.
     */
    StrBuffer &formatHex(StrBuffer &out, uint32_t val, int8_t bits, bool relax = true) const;

protected:
    char _curSym;
    bool _cstyle;
    bool _uppercase;

    uint32_t makePositive(StrBuffer &out, uint32_t val, int8_t bits) const
            __attribute__((noinline));
    virtual StrBuffer &formatPositiveHex(StrBuffer &out, uint32_t val, int8_t bits) const;
    StrBuffer &outHex(StrBuffer &out, uint32_t val, int8_t bits) const;
    StrBuffer &outDec(StrBuffer &out, uint32_t val) const __attribute__((noinline));
};

class MotoValueFormatter : public ValueFormatter {
public:
    MotoValueFormatter() : ValueFormatter('*', false) {}

protected:
    StrBuffer &formatPositiveHex(StrBuffer &out, uint32_t val, int8_t bits) const override;
};

class IntelValueFormatter : public ValueFormatter {
public:
    IntelValueFormatter() : ValueFormatter('$', false) {}

protected:
    StrBuffer &formatPositiveHex(StrBuffer &out, uint32_t val, int8_t bits) const override;
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
