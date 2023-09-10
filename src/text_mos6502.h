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

#ifndef __TEXT_MOS6502_H__
#define __TEXT_MOS6502_H__

#include "text_common.h"

namespace libasm {
namespace text {
namespace mos6502 {

// clang-format off
extern const char TEXT_CPU_LIST[]    PROGMEM;
extern const char TEXT_CPU_6502[]    PROGMEM;
extern const char TEXT_CPU_65SC02[]  PROGMEM;
extern const char TEXT_CPU_65C02[]   PROGMEM;
extern const char TEXT_CPU_W65C02S[] PROGMEM;
extern const char TEXT_CPU_65816[]   PROGMEM;
extern const char TEXT_CPU_MOS6502[] PROGMEM;
extern const char TEXT_CPU_R65C02[]  PROGMEM;
extern const char TEXT_CPU_G65SC02[] PROGMEM;
extern const char TEXT_CPU_65C816[]  PROGMEM;
extern const char TEXT_CPU_65C816S[] PROGMEM;

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
extern const char TEXT_BRK[] PROGMEM;
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
extern const char TEXT_PHA[] PROGMEM;
extern const char TEXT_PHP[] PROGMEM;
extern const char TEXT_PLA[] PROGMEM;
extern const char TEXT_PLP[] PROGMEM;
using common::TEXT_ROL;
using common::TEXT_ROR;
using common::TEXT_RTI;
using common::TEXT_RTS;
using common::TEXT_SBC;
using common::TEXT_SEC;
extern const char TEXT_SED[] PROGMEM;
using common::TEXT_SEI;
using common::TEXT_STA;
using common::TEXT_STX;
using common::TEXT_STY;
using common::TEXT_TAX;
extern const char TEXT_TAY[] PROGMEM;
using common::TEXT_TSX;
using common::TEXT_TXA;
using common::TEXT_TXS;
extern const char TEXT_TYA[] PROGMEM;

// G65SC02 instructions

using common::TEXT_BRA;
extern const char TEXT_PHX[] PROGMEM;
extern const char TEXT_PHY[] PROGMEM;
extern const char TEXT_PLX[] PROGMEM;
extern const char TEXT_PLY[] PROGMEM;
extern const char TEXT_STZ[] PROGMEM;
extern const char TEXT_TRB[] PROGMEM;
extern const char TEXT_TSB[] PROGMEM;

// R65C02 instructions

extern const char TEXT_BBR0[] PROGMEM;
extern const char TEXT_BBR1[] PROGMEM;
extern const char TEXT_BBR2[] PROGMEM;
extern const char TEXT_BBR3[] PROGMEM;
extern const char TEXT_BBR4[] PROGMEM;
extern const char TEXT_BBR5[] PROGMEM;
extern const char TEXT_BBR6[] PROGMEM;
extern const char TEXT_BBR7[] PROGMEM;
extern const char TEXT_BBS0[] PROGMEM;
extern const char TEXT_BBS1[] PROGMEM;
extern const char TEXT_BBS2[] PROGMEM;
extern const char TEXT_BBS3[] PROGMEM;
extern const char TEXT_BBS4[] PROGMEM;
extern const char TEXT_BBS5[] PROGMEM;
extern const char TEXT_BBS6[] PROGMEM;
extern const char TEXT_BBS7[] PROGMEM;
extern const char TEXT_RMB0[] PROGMEM;
extern const char TEXT_RMB1[] PROGMEM;
extern const char TEXT_RMB2[] PROGMEM;
extern const char TEXT_RMB3[] PROGMEM;
extern const char TEXT_RMB4[] PROGMEM;
extern const char TEXT_RMB5[] PROGMEM;
extern const char TEXT_RMB6[] PROGMEM;
extern const char TEXT_RMB7[] PROGMEM;
extern const char TEXT_SMB0[] PROGMEM;
extern const char TEXT_SMB1[] PROGMEM;
extern const char TEXT_SMB2[] PROGMEM;
extern const char TEXT_SMB3[] PROGMEM;
extern const char TEXT_SMB4[] PROGMEM;
extern const char TEXT_SMB5[] PROGMEM;
extern const char TEXT_SMB6[] PROGMEM;
extern const char TEXT_SMB7[] PROGMEM;

// W65C02 instructions

extern const char TEXT_STP[] PROGMEM;
using common::TEXT_WAI;

// W65C816 instructions

extern const char TEXT_BRL[] PROGMEM;
extern const char TEXT_COP[] PROGMEM;
extern const char TEXT_JML[] PROGMEM;
extern const char TEXT_JSL[] PROGMEM;
extern const char TEXT_MVN[] PROGMEM;
extern const char TEXT_MVP[] PROGMEM;
using common::TEXT_PEA;
extern const char TEXT_PEI[] PROGMEM;
extern const char TEXT_PER[] PROGMEM;
extern const char TEXT_PHB[] PROGMEM;
extern const char TEXT_PHD[] PROGMEM;
extern const char TEXT_PHK[] PROGMEM;
extern const char TEXT_PLB[] PROGMEM;
extern const char TEXT_PLD[] PROGMEM;
extern const char TEXT_PSD[] PROGMEM;
using common::TEXT_REP;
extern const char TEXT_RTL[] PROGMEM;
using common::TEXT_SEP;
extern const char TEXT_TCD[] PROGMEM;
extern const char TEXT_TCS[] PROGMEM;
extern const char TEXT_TDC[] PROGMEM;
extern const char TEXT_TSC[] PROGMEM;
extern const char TEXT_TXY[] PROGMEM;
extern const char TEXT_TYX[] PROGMEM;
extern const char TEXT_WDM[] PROGMEM;
extern const char TEXT_XBA[] PROGMEM;
extern const char TEXT_XCE[] PROGMEM;

extern const char TEXT_JMPL[] PROGMEM;
extern const char TEXT_ADCL[] PROGMEM;
extern const char TEXT_ANDL[] PROGMEM;
using common::TEXT_CMPL;
extern const char TEXT_EORL[] PROGMEM;
extern const char TEXT_LDAL[] PROGMEM;
extern const char TEXT_ORAL[] PROGMEM;
extern const char TEXT_SBCL[] PROGMEM;
extern const char TEXT_STAL[] PROGMEM;

extern const char OPT_BOOL_LONGA[] PROGMEM;
extern const char OPT_DESC_LONGA[] PROGMEM;
extern const char OPT_BOOL_LONGI[] PROGMEM;
extern const char OPT_DESC_LONGI[] PROGMEM;

using common::TEXT_REG_A;
using common::TEXT_REG_X;
using common::TEXT_REG_Y;
using common::TEXT_REG_S;

// clang-format on

}  // namespace mos6502
}  // namespace text
}  // namespace libasm

#endif  // __TEXT_MOS6502_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
