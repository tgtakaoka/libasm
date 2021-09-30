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

#include "config_host.h"

namespace libasm {
namespace mos6502 {

// clang-format off
static const char TEXT_CPU_LIST[] PROGMEM =
    "MOS6502, R65C02, G65SC02, W65C02S, W65C816S";
static const char TEXT_CPU_MOS6502[] PROGMEM = "MOS6502";
static constexpr const char *TEXT_CPU_6502 = TEXT_CPU_MOS6502 + 3;
static const char TEXT_CPU_R65C02[] PROGMEM  = "R65C02";
static constexpr const char *TEXT_CPU_65C02 = TEXT_CPU_R65C02 + 1;
static const char TEXT_CPU_G65SC02[] PROGMEM = "G65SC02";
static constexpr const char *TEXT_CPU_65SC02 = TEXT_CPU_G65SC02 + 1;
static const char TEXT_CPU_W65C02S[] PROGMEM = "W65C02S";
static constexpr const char *TEXT_CPU_W65C816S = TEXT_CPU_LIST + 35;
static const char TEXT_CPU_W65C816[]  PROGMEM = "W65C816";
static const char TEXT_CPU_W65816[]   PROGMEM = "W65816";
static constexpr const char *TEXT_CPU_65816 = TEXT_CPU_W65816 + 1;

// MOS6502 instructions
static const char TEXT_ADC[] PROGMEM = "ADC";
static const char TEXT_AND[] PROGMEM = "AND";
static const char TEXT_ASL[] PROGMEM = "ASL";
static const char TEXT_BCC[] PROGMEM = "BCC";
static const char TEXT_BCS[] PROGMEM = "BCS";
static const char TEXT_BEQ[] PROGMEM = "BEQ";
static const char TEXT_BIT[] PROGMEM = "BIT";
static const char TEXT_BMI[] PROGMEM = "BMI";
static const char TEXT_BNE[] PROGMEM = "BNE";
static const char TEXT_BPL[] PROGMEM = "BPL";
static const char TEXT_BRK[] PROGMEM = "BRK";
static const char TEXT_BVC[] PROGMEM = "BVC";
static const char TEXT_BVS[] PROGMEM = "BVS";
static const char TEXT_CLC[] PROGMEM = "CLC";
static const char TEXT_CLD[] PROGMEM = "CLD";
static const char TEXT_CLI[] PROGMEM = "CLI";
static const char TEXT_CLV[] PROGMEM = "CLV";
static const char TEXT_CMP[] PROGMEM = "CMP";
static const char TEXT_CPX[] PROGMEM = "CPX";
static const char TEXT_CPY[] PROGMEM = "CPY";
static const char TEXT_DEC[] PROGMEM = "DEC";
static const char TEXT_DEX[] PROGMEM = "DEX";
static const char TEXT_DEY[] PROGMEM = "DEY";
static const char TEXT_EOR[] PROGMEM = "EOR";
static const char TEXT_INC[] PROGMEM = "INC";
static const char TEXT_INX[] PROGMEM = "INX";
static const char TEXT_INY[] PROGMEM = "INY";
static const char TEXT_JMP[] PROGMEM = "JMP";
static const char TEXT_JSR[] PROGMEM = "JSR";
static const char TEXT_LDA[] PROGMEM = "LDA";
static const char TEXT_LDX[] PROGMEM = "LDX";
static const char TEXT_LDY[] PROGMEM = "LDY";
static const char TEXT_LSR[] PROGMEM = "LSR";
static const char TEXT_NOP[] PROGMEM = "NOP";
static const char TEXT_ORA[] PROGMEM = "ORA";
static const char TEXT_PHA[] PROGMEM = "PHA";
static const char TEXT_PHP[] PROGMEM = "PHP";
static const char TEXT_PLA[] PROGMEM = "PLA";
static const char TEXT_PLP[] PROGMEM = "PLP";
static const char TEXT_ROL[] PROGMEM = "ROL";
static const char TEXT_ROR[] PROGMEM = "ROR";
static const char TEXT_RTI[] PROGMEM = "RTI";
static const char TEXT_RTS[] PROGMEM = "RTS";
static const char TEXT_SBC[] PROGMEM = "SBC";
static const char TEXT_SEC[] PROGMEM = "SEC";
static const char TEXT_SED[] PROGMEM = "SED";
static const char TEXT_SEI[] PROGMEM = "SEI";
static const char TEXT_STA[] PROGMEM = "STA";
static const char TEXT_STX[] PROGMEM = "STX";
static const char TEXT_STY[] PROGMEM = "STY";
static const char TEXT_TAX[] PROGMEM = "TAX";
static const char TEXT_TAY[] PROGMEM = "TAY";
static const char TEXT_TSX[] PROGMEM = "TSX";
static const char TEXT_TXA[] PROGMEM = "TXA";
static const char TEXT_TXS[] PROGMEM = "TXS";
static const char TEXT_TYA[] PROGMEM = "TYA";

// G65SC02 instructions

static const char TEXT_BRA[] PROGMEM = "BRA";
static const char TEXT_PHX[] PROGMEM = "PHX";
static const char TEXT_PHY[] PROGMEM = "PHY";
static const char TEXT_PLX[] PROGMEM = "PLX";
static const char TEXT_PLY[] PROGMEM = "PLY";
static const char TEXT_STZ[] PROGMEM = "STZ";
static const char TEXT_TRB[] PROGMEM = "TRB";
static const char TEXT_TSB[] PROGMEM = "TSB";

// R65C02 instructions

static const char TEXT_BBR0[] PROGMEM = "BBR0";
static const char TEXT_BBR1[] PROGMEM = "BBR1";
static const char TEXT_BBR2[] PROGMEM = "BBR2";
static const char TEXT_BBR3[] PROGMEM = "BBR3";
static const char TEXT_BBR4[] PROGMEM = "BBR4";
static const char TEXT_BBR5[] PROGMEM = "BBR5";
static const char TEXT_BBR6[] PROGMEM = "BBR6";
static const char TEXT_BBR7[] PROGMEM = "BBR7";
static const char TEXT_BBS0[] PROGMEM = "BBS0";
static const char TEXT_BBS1[] PROGMEM = "BBS1";
static const char TEXT_BBS2[] PROGMEM = "BBS2";
static const char TEXT_BBS3[] PROGMEM = "BBS3";
static const char TEXT_BBS4[] PROGMEM = "BBS4";
static const char TEXT_BBS5[] PROGMEM = "BBS5";
static const char TEXT_BBS6[] PROGMEM = "BBS6";
static const char TEXT_BBS7[] PROGMEM = "BBS7";
static const char TEXT_RMB0[] PROGMEM = "RMB0";
static const char TEXT_RMB1[] PROGMEM = "RMB1";
static const char TEXT_RMB2[] PROGMEM = "RMB2";
static const char TEXT_RMB3[] PROGMEM = "RMB3";
static const char TEXT_RMB4[] PROGMEM = "RMB4";
static const char TEXT_RMB5[] PROGMEM = "RMB5";
static const char TEXT_RMB6[] PROGMEM = "RMB6";
static const char TEXT_RMB7[] PROGMEM = "RMB7";
static const char TEXT_SMB0[] PROGMEM = "SMB0";
static const char TEXT_SMB1[] PROGMEM = "SMB1";
static const char TEXT_SMB2[] PROGMEM = "SMB2";
static const char TEXT_SMB3[] PROGMEM = "SMB3";
static const char TEXT_SMB4[] PROGMEM = "SMB4";
static const char TEXT_SMB5[] PROGMEM = "SMB5";
static const char TEXT_SMB6[] PROGMEM = "SMB6";
static const char TEXT_SMB7[] PROGMEM = "SMB7";

// W65C02 instructions

static const char TEXT_STP[] PROGMEM  = "STP";
static const char TEXT_WAI[] PROGMEM  = "WAI";

// W65C816 instructions

static const char TEXT_BRL[] PROGMEM  = "BRL";
static const char TEXT_COP[] PROGMEM  = "COP";
static const char TEXT_JML[] PROGMEM  = "JML";
static const char TEXT_JSL[] PROGMEM  = "JSL";
static const char TEXT_MVN[] PROGMEM  = "MVN";
static const char TEXT_MVP[] PROGMEM  = "MVP";
static const char TEXT_PEA[] PROGMEM  = "PEA";
static const char TEXT_PEI[] PROGMEM  = "PEI";
static const char TEXT_PER[] PROGMEM  = "PER";
static const char TEXT_PHB[] PROGMEM  = "PHB";
static const char TEXT_PHD[] PROGMEM  = "PHD";
static const char TEXT_PHK[] PROGMEM  = "PHK";
static const char TEXT_PLB[] PROGMEM  = "PLB";
static const char TEXT_PLD[] PROGMEM  = "PLD";
static const char TEXT_PSD[] PROGMEM  = "PSD";
static const char TEXT_REP[] PROGMEM  = "REP";
static const char TEXT_RTL[] PROGMEM  = "RTL";
static const char TEXT_SEP[] PROGMEM  = "SEP";
static const char TEXT_TCD[] PROGMEM  = "TCD";
static const char TEXT_TCS[] PROGMEM  = "TCS";
static const char TEXT_TDC[] PROGMEM  = "TDC";
static const char TEXT_TSC[] PROGMEM  = "TSC";
static const char TEXT_TXY[] PROGMEM  = "TXY";
static const char TEXT_TYX[] PROGMEM  = "TYX";
static const char TEXT_WDM[] PROGMEM  = "WDM";
static const char TEXT_XBA[] PROGMEM  = "XBA";
static const char TEXT_XCE[] PROGMEM  = "XCE";

static const char TEXT_JMPL[] PROGMEM = "JMPL";
static const char TEXT_ADCL[] PROGMEM = "ADCL";
static const char TEXT_ANDL[] PROGMEM = "ANDL";
static const char TEXT_CMPL[] PROGMEM = "CMPL";
static const char TEXT_EORL[] PROGMEM = "EORL";
static const char TEXT_LDAL[] PROGMEM = "LDAL";
static const char TEXT_ORAL[] PROGMEM = "ORAL";
static const char TEXT_SBCL[] PROGMEM = "SBCL";
static const char TEXT_STAL[] PROGMEM = "STAL";
// clang-format on

}  // namespace mos6502
}  // namespace libasm

#endif  // __TEXT_MOS6502_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
