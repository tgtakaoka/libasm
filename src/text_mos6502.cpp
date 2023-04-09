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

#include "text_mos6502.h"

namespace libasm {
namespace text {
namespace mos6502 {

// clang-format off
const char TEXT_CPU_LIST[]    PROGMEM = "MOS6502, R65C02, G65SC02, W65C02S, W65C816S";
const char TEXT_CPU_6502[]    PROGMEM = "6502";
const char TEXT_CPU_65SC02[]  PROGMEM = "65SC02";
const char TEXT_CPU_65C02[]   PROGMEM = "65C02";
const char TEXT_CPU_W65C02S[] PROGMEM = "W65C02S";
const char TEXT_CPU_65816[]   PROGMEM = "65816";
const char TEXT_CPU_MOS6502[] PROGMEM = "MOS6502";
const char TEXT_CPU_R65C02[]  PROGMEM = "R65C02";
const char TEXT_CPU_G65SC02[] PROGMEM = "G65SC02";
const char TEXT_CPU_65C816[]  PROGMEM = "65C816";
const char TEXT_CPU_65C816S[] PROGMEM = "65C816S";

// MOS6502 instructions
// const char TEXT_ADC[] PROGMEM = "ADC";
// const char TEXT_AND[] PROGMEM = "AND";
// const char TEXT_ASL[] PROGMEM = "ASL";
// const char TEXT_BCC[] PROGMEM = "BCC";
// const char TEXT_BCS[] PROGMEM = "BCS";
// const char TEXT_BEQ[] PROGMEM = "BEQ";
// const char TEXT_BIT[] PROGMEM = "BIT";
// const char TEXT_BMI[] PROGMEM = "BMI";
// const char TEXT_BNE[] PROGMEM = "BNE";
// const char TEXT_BPL[] PROGMEM = "BPL";
const char TEXT_BRK[] PROGMEM = "BRK";
// const char TEXT_BVC[] PROGMEM = "BVC";
// const char TEXT_BVS[] PROGMEM = "BVS";
// const char TEXT_CLC[] PROGMEM = "CLC";
// const char TEXT_CLD[] PROGMEM = "CLD";
// const char TEXT_CLI[] PROGMEM = "CLI";
// const char TEXT_CLV[] PROGMEM = "CLV";
// const char TEXT_CMP[] PROGMEM = "CMP";
// const char TEXT_CPX[] PROGMEM = "CPX";
// const char TEXT_CPY[] PROGMEM = "CPY";
// const char TEXT_DEC[] PROGMEM = "DEC";
// const char TEXT_DEX[] PROGMEM = "DEX";
// const char TEXT_DEY[] PROGMEM = "DEY";
// const char TEXT_EOR[] PROGMEM = "EOR";
// const char TEXT_INC[] PROGMEM = "INC";
// const char TEXT_INX[] PROGMEM = "INX";
// const char TEXT_INY[] PROGMEM = "INY";
// const char TEXT_JMP[] PROGMEM = "JMP";
// const char TEXT_JSR[] PROGMEM = "JSR";
// const char TEXT_LDA[] PROGMEM = "LDA";
// const char TEXT_LDX[] PROGMEM = "LDX";
// const char TEXT_LDY[] PROGMEM = "LDY";
// const char TEXT_LSR[] PROGMEM = "LSR";
// const char TEXT_NOP[] PROGMEM = "NOP";
// const char TEXT_ORA[] PROGMEM = "ORA";
const char TEXT_PHA[] PROGMEM = "PHA";
const char TEXT_PHP[] PROGMEM = "PHP";
const char TEXT_PLA[] PROGMEM = "PLA";
const char TEXT_PLP[] PROGMEM = "PLP";
// const char TEXT_ROL[] PROGMEM = "ROL";
// const char TEXT_ROR[] PROGMEM = "ROR";
// const char TEXT_RTI[] PROGMEM = "RTI";
// const char TEXT_RTS[] PROGMEM = "RTS";
// const char TEXT_SBC[] PROGMEM = "SBC";
// const char TEXT_SEC[] PROGMEM = "SEC";
const char TEXT_SED[] PROGMEM = "SED";
// const char TEXT_SEI[] PROGMEM = "SEI";
// const char TEXT_STA[] PROGMEM = "STA";
// const char TEXT_STX[] PROGMEM = "STX";
// const char TEXT_STY[] PROGMEM = "STY";
// const char TEXT_TAX[] PROGMEM = "TAX";
const char TEXT_TAY[] PROGMEM = "TAY";
// const char TEXT_TSX[] PROGMEM = "TSX";
// const char TEXT_TXA[] PROGMEM = "TXA";
// const char TEXT_TXS[] PROGMEM = "TXS";
const char TEXT_TYA[] PROGMEM = "TYA";

// G65SC02 instructions

// const char TEXT_BRA[] PROGMEM = "BRA";
const char TEXT_PHX[] PROGMEM = "PHX";
const char TEXT_PHY[] PROGMEM = "PHY";
const char TEXT_PLX[] PROGMEM = "PLX";
const char TEXT_PLY[] PROGMEM = "PLY";
const char TEXT_STZ[] PROGMEM = "STZ";
const char TEXT_TRB[] PROGMEM = "TRB";
const char TEXT_TSB[] PROGMEM = "TSB";

// R65C02 instructions

const char TEXT_BBR0[] PROGMEM = "BBR0";
const char TEXT_BBR1[] PROGMEM = "BBR1";
const char TEXT_BBR2[] PROGMEM = "BBR2";
const char TEXT_BBR3[] PROGMEM = "BBR3";
const char TEXT_BBR4[] PROGMEM = "BBR4";
const char TEXT_BBR5[] PROGMEM = "BBR5";
const char TEXT_BBR6[] PROGMEM = "BBR6";
const char TEXT_BBR7[] PROGMEM = "BBR7";
const char TEXT_BBS0[] PROGMEM = "BBS0";
const char TEXT_BBS1[] PROGMEM = "BBS1";
const char TEXT_BBS2[] PROGMEM = "BBS2";
const char TEXT_BBS3[] PROGMEM = "BBS3";
const char TEXT_BBS4[] PROGMEM = "BBS4";
const char TEXT_BBS5[] PROGMEM = "BBS5";
const char TEXT_BBS6[] PROGMEM = "BBS6";
const char TEXT_BBS7[] PROGMEM = "BBS7";
const char TEXT_RMB0[] PROGMEM = "RMB0";
const char TEXT_RMB1[] PROGMEM = "RMB1";
const char TEXT_RMB2[] PROGMEM = "RMB2";
const char TEXT_RMB3[] PROGMEM = "RMB3";
const char TEXT_RMB4[] PROGMEM = "RMB4";
const char TEXT_RMB5[] PROGMEM = "RMB5";
const char TEXT_RMB6[] PROGMEM = "RMB6";
const char TEXT_RMB7[] PROGMEM = "RMB7";
const char TEXT_SMB0[] PROGMEM = "SMB0";
const char TEXT_SMB1[] PROGMEM = "SMB1";
const char TEXT_SMB2[] PROGMEM = "SMB2";
const char TEXT_SMB3[] PROGMEM = "SMB3";
const char TEXT_SMB4[] PROGMEM = "SMB4";
const char TEXT_SMB5[] PROGMEM = "SMB5";
const char TEXT_SMB6[] PROGMEM = "SMB6";
const char TEXT_SMB7[] PROGMEM = "SMB7";

// W65C02 instructions

const char TEXT_STP[] PROGMEM  = "STP";
// const char TEXT_WAI[] PROGMEM  = "WAI";

// W65C816 instructions

const char TEXT_BRL[] PROGMEM  = "BRL";
const char TEXT_COP[] PROGMEM  = "COP";
const char TEXT_JML[] PROGMEM  = "JML";
const char TEXT_JSL[] PROGMEM  = "JSL";
const char TEXT_MVN[] PROGMEM  = "MVN";
const char TEXT_MVP[] PROGMEM  = "MVP";
// const char TEXT_PEA[] PROGMEM  = "PEA";
const char TEXT_PEI[] PROGMEM  = "PEI";
const char TEXT_PER[] PROGMEM  = "PER";
const char TEXT_PHB[] PROGMEM  = "PHB";
const char TEXT_PHD[] PROGMEM  = "PHD";
const char TEXT_PHK[] PROGMEM  = "PHK";
const char TEXT_PLB[] PROGMEM  = "PLB";
const char TEXT_PLD[] PROGMEM  = "PLD";
const char TEXT_PSD[] PROGMEM  = "PSD";
// const char TEXT_REP[] PROGMEM  = "REP";
const char TEXT_RTL[] PROGMEM  = "RTL";
// const char TEXT_SEP[] PROGMEM  = "SEP";
const char TEXT_TCD[] PROGMEM  = "TCD";
const char TEXT_TCS[] PROGMEM  = "TCS";
const char TEXT_TDC[] PROGMEM  = "TDC";
const char TEXT_TSC[] PROGMEM  = "TSC";
const char TEXT_TXY[] PROGMEM  = "TXY";
const char TEXT_TYX[] PROGMEM  = "TYX";
const char TEXT_WDM[] PROGMEM  = "WDM";
const char TEXT_XBA[] PROGMEM  = "XBA";
const char TEXT_XCE[] PROGMEM  = "XCE";

const char TEXT_JMPL[] PROGMEM = "JMPL";
const char TEXT_ADCL[] PROGMEM = "ADCL";
const char TEXT_ANDL[] PROGMEM = "ANDL";
// const char TEXT_CMPL[] PROGMEM = "CMPL";
const char TEXT_EORL[] PROGMEM = "EORL";
const char TEXT_LDAL[] PROGMEM = "LDAL";
const char TEXT_ORAL[] PROGMEM = "ORAL";
const char TEXT_SBCL[] PROGMEM = "SBCL";
const char TEXT_STAL[] PROGMEM = "STAL";

const char OPT_BOOL_LONGA[] PROGMEM = "longa";
const char OPT_DESC_LONGA[] PROGMEM = "enable 16-bit accumulator";
const char OPT_BOOL_LONGI[] PROGMEM = "longi";
const char OPT_DESC_LONGI[] PROGMEM = "enable 16-bit index registers";

// clang-format on

}  // namespace mos6502
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
