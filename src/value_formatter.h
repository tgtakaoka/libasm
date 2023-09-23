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
    ValueFormatter(const HexFormatter &hexFormatter = HexFormatter::singleton())
        : _hexFormatter(hexFormatter) {}

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
