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

namespace libasm {

class ValueFormatter {
public:
    /*
     * Convert |val| as |bitWidth| integer of |radix|.
     * Treat |val| as signed integer when |radix| is negative.
     * Suppress leading zero when |bitWidth| is negative.
     * Use base 10 and zero suppress when |relax| is true
     *  and |val| is less than |base|.
     */
    virtual char *output(
        char *p, uint32_t val, int8_t radix,
        bool relax, int8_t bitWidth) const;
    void setUppercase(bool uppercase) { _uppercase = uppercase; }

protected:
    bool _uppercase;

    /*
     * Convert non-negative |val| as |bitWidth| integer of |base|.
     * Suppress leading zero when |bitWidth| is negative.
     */
    char *outputNumber(
        char *p, uint32_t val, uint8_t base, int8_t bitWidth) const;
    /*
     * Convert |val| as |bitWIdth| integer of |radix|.
     * Treat |val| as signed integer when |radix| is negative.
     * Use base 10 and zero suppress when |val| is less than |base|.
     * Otherwise return |nullptr|.
     */
    char *outputRelaxed(
        char *p, uint32_t val, int8_t radix, int8_t &bitWidth) const;
};

class MotoValueFormatter : public ValueFormatter {
public:
    char *output(
        char *p, uint32_t val, int8_t radix,
        bool relax, int8_t bitWidth) const override;
};

class IntelValueFormatter : public ValueFormatter {
public:
    char *output(
        char *p, uint32_t val, int8_t radix,
        bool relax, int8_t bitWidth) const override;
};

} // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
