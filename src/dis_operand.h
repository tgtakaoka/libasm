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

#ifndef _DIS_OPERAND_H__
#define _DIS_OPERAND_H__

#include <stdint.h>

namespace libasm {

class DisOperand {
public:
    virtual char *output(
        char *p, uint32_t val, int8_t radix,
        bool relax, uint8_t bitWidth) const;
    void setUppercase(bool uppercase) { _uppercase = uppercase; }

protected:
    bool _uppercase;

    char *outputNumber(
        char *p, uint32_t val, int8_t radix, int8_t bitWidth) const;
    char *outputRelaxed(
        char *p, uint32_t val, int8_t radix, uint8_t bitWidth) const;
};

class DisMotoOperand : public DisOperand {
public:
    char *output(
        char *p, uint32_t val, int8_t radix,
        bool relax, uint8_t bitWidth) const override;
};

class DisIntelOperand : public DisOperand {
public:
    char *output(
        char *p, uint32_t val, int8_t radix,
        bool relax, uint8_t bitWidth) const override;
};

} // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
