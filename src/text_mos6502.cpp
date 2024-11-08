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
constexpr char TEXT_MOS6502_LIST[] PROGMEM = "MOS6502, R65C02, G65SC02, W65C02S, W65C816S";
constexpr char TEXT_CPU_6502[]    PROGMEM = "6502";
constexpr char TEXT_CPU_65SC02[]  PROGMEM = "65SC02";
constexpr char TEXT_CPU_65C02[]   PROGMEM = "65C02";
constexpr char TEXT_CPU_W65C02S[] PROGMEM = "W65C02S";
constexpr char TEXT_CPU_65816[]   PROGMEM = "65816";
constexpr char TEXT_CPU_MOS6502[] PROGMEM = "MOS6502";
constexpr char TEXT_CPU_R65C02[]  PROGMEM = "R65C02";
constexpr char TEXT_CPU_G65SC02[] PROGMEM = "G65SC02";
constexpr char TEXT_CPU_65C816[]  PROGMEM = "65C816";
constexpr char TEXT_CPU_65C816S[] PROGMEM = "65C816S";

// MOS6502 instructions
// constexpr char TEXT_ADC[] PROGMEM = "ADC";
// constexpr char TEXT_AND[] PROGMEM = "AND";
// constexpr char TEXT_ASL[] PROGMEM = "ASL";
// constexpr char TEXT_BCC[] PROGMEM = "BCC";
// constexpr char TEXT_BCS[] PROGMEM = "BCS";
// constexpr char TEXT_BEQ[] PROGMEM = "BEQ";
// constexpr char TEXT_BIT[] PROGMEM = "BIT";
// constexpr char TEXT_BMI[] PROGMEM = "BMI";
// constexpr char TEXT_BNE[] PROGMEM = "BNE";
// constexpr char TEXT_BPL[] PROGMEM = "BPL";
constexpr char TEXT_BRK[] PROGMEM = "BRK";
// constexpr char TEXT_BVC[] PROGMEM = "BVC";
// constexpr char TEXT_BVS[] PROGMEM = "BVS";
// constexpr char TEXT_CLC[] PROGMEM = "CLC";
// constexpr char TEXT_CLD[] PROGMEM = "CLD";
// constexpr char TEXT_CLI[] PROGMEM = "CLI";
// constexpr char TEXT_CLV[] PROGMEM = "CLV";
// constexpr char TEXT_CMP[] PROGMEM = "CMP";
// constexpr char TEXT_CPX[] PROGMEM = "CPX";
// constexpr char TEXT_CPY[] PROGMEM = "CPY";
// constexpr char TEXT_DEC[] PROGMEM = "DEC";
// constexpr char TEXT_DEX[] PROGMEM = "DEX";
// constexpr char TEXT_DEY[] PROGMEM = "DEY";
// constexpr char TEXT_EOR[] PROGMEM = "EOR";
// constexpr char TEXT_INC[] PROGMEM = "INC";
// constexpr char TEXT_INX[] PROGMEM = "INX";
// constexpr char TEXT_INY[] PROGMEM = "INY";
// constexpr char TEXT_JMP[] PROGMEM = "JMP";
// constexpr char TEXT_JSR[] PROGMEM = "JSR";
// constexpr char TEXT_LDA[] PROGMEM = "LDA";
// constexpr char TEXT_LDX[] PROGMEM = "LDX";
// constexpr char TEXT_LDY[] PROGMEM = "LDY";
// constexpr char TEXT_LSR[] PROGMEM = "LSR";
// constexpr char TEXT_NOP[] PROGMEM = "NOP";
// constexpr char TEXT_ORA[] PROGMEM = "ORA";
constexpr char TEXT_PHA[] PROGMEM = "PHA";
constexpr char TEXT_PHP[] PROGMEM = "PHP";
constexpr char TEXT_PLA[] PROGMEM = "PLA";
constexpr char TEXT_PLP[] PROGMEM = "PLP";
// constexpr char TEXT_ROL[] PROGMEM = "ROL";
// constexpr char TEXT_ROR[] PROGMEM = "ROR";
// constexpr char TEXT_RTI[] PROGMEM = "RTI";
// constexpr char TEXT_RTS[] PROGMEM = "RTS";
// constexpr char TEXT_SBC[] PROGMEM = "SBC";
// constexpr char TEXT_SEC[] PROGMEM = "SEC";
constexpr char TEXT_SED[] PROGMEM = "SED";
// constexpr char TEXT_SEI[] PROGMEM = "SEI";
// constexpr char TEXT_STA[] PROGMEM = "STA";
// constexpr char TEXT_STX[] PROGMEM = "STX";
// constexpr char TEXT_STY[] PROGMEM = "STY";
// constexpr char TEXT_TAX[] PROGMEM = "TAX";
constexpr char TEXT_TAY[] PROGMEM = "TAY";
// constexpr char TEXT_TSX[] PROGMEM = "TSX";
// constexpr char TEXT_TXA[] PROGMEM = "TXA";
// constexpr char TEXT_TXS[] PROGMEM = "TXS";
constexpr char TEXT_TYA[] PROGMEM = "TYA";

// G65SC02 instructions

// constexpr char TEXT_BRA[] PROGMEM = "BRA";
constexpr char TEXT_PHX[] PROGMEM = "PHX";
constexpr char TEXT_PHY[] PROGMEM = "PHY";
constexpr char TEXT_PLX[] PROGMEM = "PLX";
constexpr char TEXT_PLY[] PROGMEM = "PLY";
constexpr char TEXT_STZ[] PROGMEM = "STZ";
constexpr char TEXT_TRB[] PROGMEM = "TRB";
constexpr char TEXT_TSB[] PROGMEM = "TSB";

// R65C02 instructions

constexpr char TEXT_BBR0[] PROGMEM = "BBR0";
constexpr char TEXT_BBR1[] PROGMEM = "BBR1";
constexpr char TEXT_BBR2[] PROGMEM = "BBR2";
constexpr char TEXT_BBR3[] PROGMEM = "BBR3";
constexpr char TEXT_BBR4[] PROGMEM = "BBR4";
constexpr char TEXT_BBR5[] PROGMEM = "BBR5";
constexpr char TEXT_BBR6[] PROGMEM = "BBR6";
constexpr char TEXT_BBR7[] PROGMEM = "BBR7";
constexpr char TEXT_BBS0[] PROGMEM = "BBS0";
constexpr char TEXT_BBS1[] PROGMEM = "BBS1";
constexpr char TEXT_BBS2[] PROGMEM = "BBS2";
constexpr char TEXT_BBS3[] PROGMEM = "BBS3";
constexpr char TEXT_BBS4[] PROGMEM = "BBS4";
constexpr char TEXT_BBS5[] PROGMEM = "BBS5";
constexpr char TEXT_BBS6[] PROGMEM = "BBS6";
constexpr char TEXT_BBS7[] PROGMEM = "BBS7";
constexpr char TEXT_RMB0[] PROGMEM = "RMB0";
constexpr char TEXT_RMB1[] PROGMEM = "RMB1";
constexpr char TEXT_RMB2[] PROGMEM = "RMB2";
constexpr char TEXT_RMB3[] PROGMEM = "RMB3";
constexpr char TEXT_RMB4[] PROGMEM = "RMB4";
constexpr char TEXT_RMB5[] PROGMEM = "RMB5";
constexpr char TEXT_RMB6[] PROGMEM = "RMB6";
constexpr char TEXT_RMB7[] PROGMEM = "RMB7";
constexpr char TEXT_SMB0[] PROGMEM = "SMB0";
constexpr char TEXT_SMB1[] PROGMEM = "SMB1";
constexpr char TEXT_SMB2[] PROGMEM = "SMB2";
constexpr char TEXT_SMB3[] PROGMEM = "SMB3";
constexpr char TEXT_SMB4[] PROGMEM = "SMB4";
constexpr char TEXT_SMB5[] PROGMEM = "SMB5";
constexpr char TEXT_SMB6[] PROGMEM = "SMB6";
constexpr char TEXT_SMB7[] PROGMEM = "SMB7";

// W65C02 instructions

constexpr char TEXT_STP[] PROGMEM  = "STP";
// constexpr char TEXT_WAI[] PROGMEM  = "WAI";

// W65C816 instructions

constexpr char TEXT_BRL[] PROGMEM  = "BRL";
constexpr char TEXT_COP[] PROGMEM  = "COP";
constexpr char TEXT_JML[] PROGMEM  = "JML";
constexpr char TEXT_JSL[] PROGMEM  = "JSL";
constexpr char TEXT_MVN[] PROGMEM  = "MVN";
constexpr char TEXT_MVP[] PROGMEM  = "MVP";
// constexpr char TEXT_PEA[] PROGMEM  = "PEA";
constexpr char TEXT_PEI[] PROGMEM  = "PEI";
constexpr char TEXT_PER[] PROGMEM  = "PER";
constexpr char TEXT_PHB[] PROGMEM  = "PHB";
constexpr char TEXT_PHD[] PROGMEM  = "PHD";
constexpr char TEXT_PHK[] PROGMEM  = "PHK";
constexpr char TEXT_PLB[] PROGMEM  = "PLB";
constexpr char TEXT_PLD[] PROGMEM  = "PLD";
constexpr char TEXT_PSD[] PROGMEM  = "PSD";
// constexpr char TEXT_REP[] PROGMEM  = "REP";
// constexpr char TEXT_RTL[] PROGMEM  = "RTL";
// constexpr char TEXT_SEP[] PROGMEM  = "SEP";
constexpr char TEXT_TCD[] PROGMEM  = "TCD";
constexpr char TEXT_TCS[] PROGMEM  = "TCS";
constexpr char TEXT_TDC[] PROGMEM  = "TDC";
constexpr char TEXT_TSC[] PROGMEM  = "TSC";
constexpr char TEXT_TXY[] PROGMEM  = "TXY";
constexpr char TEXT_TYX[] PROGMEM  = "TYX";
constexpr char TEXT_WDM[] PROGMEM  = "WDM";
constexpr char TEXT_XBA[] PROGMEM  = "XBA";
constexpr char TEXT_XCE[] PROGMEM  = "XCE";

constexpr char TEXT_JMPL[] PROGMEM = "JMPL";
constexpr char TEXT_ADCL[] PROGMEM = "ADCL";
constexpr char TEXT_ANDL[] PROGMEM = "ANDL";
// constexpr char TEXT_CMPL[] PROGMEM = "CMPL";
constexpr char TEXT_EORL[] PROGMEM = "EORL";
constexpr char TEXT_LDAL[] PROGMEM = "LDAL";
constexpr char TEXT_ORAL[] PROGMEM = "ORAL";
constexpr char TEXT_SBCL[] PROGMEM = "SBCL";
constexpr char TEXT_STAL[] PROGMEM = "STAL";

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
