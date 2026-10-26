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

#ifndef __LIBASM_TEXT_MOS6502_H__
#define __LIBASM_TEXT_MOS6502_H__

#include "text_common.h"

namespace libasm {
namespace text {
namespace mos6502 {
// clang-format off

extern const char TEXT_MOS6502_LIST[];
extern const char TEXT_CPU_6502[];
extern const char TEXT_CPU_65816[];
extern const char TEXT_CPU_65C02[];
extern const char TEXT_CPU_65C816[];
extern const char TEXT_CPU_65C816S[];
extern const char TEXT_CPU_65SC02[];
extern const char TEXT_CPU_G65SC02[];
extern const char TEXT_CPU_MOS6502[];
extern const char TEXT_CPU_R65C02[];
extern const char TEXT_CPU_W65C02S[];

// MOS6502 instructions
using common::TEXT_ADC;
using common::TEXT_AND;
using common::TEXT_ASL;
using common::TEXT_BCC;
using common::TEXT_BCS;
using common::TEXT_BEQ;
using common::TEXT_BIT;
using common::TEXT_BMI;
using common::TEXT_BNE;
using common::TEXT_BPL;
extern const char TEXT_BRK[];
using common::TEXT_BVC;
using common::TEXT_BVS;
using common::TEXT_CLC;
using common::TEXT_CLD;
using common::TEXT_CLI;
using common::TEXT_CLV;
using common::TEXT_CMP;
using common::TEXT_CPX;
using common::TEXT_CPY;
using common::TEXT_DEC;
using common::TEXT_DEX;
using common::TEXT_DEY;
using common::TEXT_EOR;
using common::TEXT_INC;
using common::TEXT_INX;
using common::TEXT_INY;
using common::TEXT_JMP;
using common::TEXT_JSR;
using common::TEXT_LDA;
using common::TEXT_LDX;
using common::TEXT_LDY;
using common::TEXT_LSR;
using common::TEXT_NOP;
using common::TEXT_ORA;
extern const char TEXT_PHA[];
extern const char TEXT_PHP[];
extern const char TEXT_PLA[];
extern const char TEXT_PLP[];
using common::TEXT_ROL;
using common::TEXT_ROR;
using common::TEXT_RTI;
using common::TEXT_RTS;
using common::TEXT_SBC;
using common::TEXT_SEC;
extern const char TEXT_SED[];
using common::TEXT_SEI;
using common::TEXT_STA;
using common::TEXT_STX;
using common::TEXT_STY;
using common::TEXT_TAX;
extern const char TEXT_TAY[];
using common::TEXT_TSX;
using common::TEXT_TXA;
using common::TEXT_TXS;
extern const char TEXT_TYA[];

// G65SC02 instructions
using common::TEXT_BRA;
extern const char TEXT_PHX[];
extern const char TEXT_PHY[];
extern const char TEXT_PLX[];
extern const char TEXT_PLY[];
using common::TEXT_STZ;
extern const char TEXT_TRB[];
extern const char TEXT_TSB[];

// R65C02 instructions
extern const char TEXT_BBR0[];
extern const char TEXT_BBR1[];
extern const char TEXT_BBR2[];
extern const char TEXT_BBR3[];
extern const char TEXT_BBR4[];
extern const char TEXT_BBR5[];
extern const char TEXT_BBR6[];
extern const char TEXT_BBR7[];
extern const char TEXT_BBS0[];
extern const char TEXT_BBS1[];
extern const char TEXT_BBS2[];
extern const char TEXT_BBS3[];
extern const char TEXT_BBS4[];
extern const char TEXT_BBS5[];
extern const char TEXT_BBS6[];
extern const char TEXT_BBS7[];
extern const char TEXT_RMB0[];
extern const char TEXT_RMB1[];
extern const char TEXT_RMB2[];
extern const char TEXT_RMB3[];
extern const char TEXT_RMB4[];
extern const char TEXT_RMB5[];
extern const char TEXT_RMB6[];
extern const char TEXT_RMB7[];
extern const char TEXT_SMB0[];
extern const char TEXT_SMB1[];
extern const char TEXT_SMB2[];
extern const char TEXT_SMB3[];
extern const char TEXT_SMB4[];
extern const char TEXT_SMB5[];
extern const char TEXT_SMB6[];
extern const char TEXT_SMB7[];

// W65C02 instructions
extern const char TEXT_STP[];
using common::TEXT_WAI;

// W65C816 instructions
extern const char TEXT_ADCL[];
extern const char TEXT_ANDL[];
extern const char TEXT_BRL[];
using common::TEXT_CMPL;
extern const char TEXT_COP[];
extern const char TEXT_EORL[];
extern const char TEXT_JML[];
using common::TEXT_JMPL;
extern const char TEXT_JSL[];
extern const char TEXT_LDAL[];
extern const char TEXT_MVN[];
extern const char TEXT_MVP[];
extern const char TEXT_ORAL[];
using common::TEXT_PEA;
extern const char TEXT_PEI[];
extern const char TEXT_PER[];
extern const char TEXT_PHB[];
extern const char TEXT_PHD[];
extern const char TEXT_PHK[];
extern const char TEXT_PLB[];
extern const char TEXT_PLD[];
extern const char TEXT_PSD[];
using common::TEXT_REP;
using common::TEXT_RTL;
extern const char TEXT_SBCL[];
using common::TEXT_SEP;
extern const char TEXT_STAL[];
extern const char TEXT_TCD[];
extern const char TEXT_TCS[];
extern const char TEXT_TDC[];
extern const char TEXT_TSC[];
using common::TEXT_TXY;
using common::TEXT_TYX;
extern const char TEXT_WDM[];
extern const char TEXT_XBA[];
extern const char TEXT_XCE[];

using common::TEXT_REG_A;
using common::TEXT_REG_S;
using common::TEXT_REG_X;
using common::TEXT_REG_Y;

// clang-format on
}  // namespace mos6502
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_MOS6502_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
