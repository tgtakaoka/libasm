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

constexpr char TEXT_MOS6502_LIST[] = "MOS6502, R65C02, G65SC02, W65C02S, W65C816S";
constexpr char TEXT_CPU_6502[]    = "6502";
constexpr char TEXT_CPU_65816[]   = "65816";
constexpr char TEXT_CPU_65C02[]   = "65C02";
constexpr char TEXT_CPU_65C816[]  = "65C816";
constexpr char TEXT_CPU_65C816S[] = "65C816S";
constexpr char TEXT_CPU_65SC02[]  = "65SC02";
constexpr char TEXT_CPU_G65SC02[] = "G65SC02";
constexpr char TEXT_CPU_MOS6502[] = "MOS6502";
constexpr char TEXT_CPU_R65C02[]  = "R65C02";
constexpr char TEXT_CPU_W65C02S[] = "W65C02S";

// MOS6502
// constexpr char TEXT_ADC[] = "ADC";
// constexpr char TEXT_AND[] = "AND";
// constexpr char TEXT_ASL[] = "ASL";
// constexpr char TEXT_BCC[] = "BCC";
// constexpr char TEXT_BCS[] = "BCS";
// constexpr char TEXT_BEQ[] = "BEQ";
// constexpr char TEXT_BIT[] = "BIT";
// constexpr char TEXT_BMI[] = "BMI";
// constexpr char TEXT_BNE[] = "BNE";
// constexpr char TEXT_BPL[] = "BPL";
constexpr char TEXT_BRK[] = "BRK";
// constexpr char TEXT_BVC[] = "BVC";
// constexpr char TEXT_BVS[] = "BVS";
// constexpr char TEXT_CLC[] = "CLC";
// constexpr char TEXT_CLD[] = "CLD";
// constexpr char TEXT_CLI[] = "CLI";
// constexpr char TEXT_CLV[] = "CLV";
// constexpr char TEXT_CMP[] = "CMP";
// constexpr char TEXT_CPX[] = "CPX";
// constexpr char TEXT_CPY[] = "CPY";
// constexpr char TEXT_DEC[] = "DEC";
// constexpr char TEXT_DEX[] = "DEX";
// constexpr char TEXT_DEY[] = "DEY";
// constexpr char TEXT_EOR[] = "EOR";
// constexpr char TEXT_INC[] = "INC";
// constexpr char TEXT_INX[] = "INX";
// constexpr char TEXT_INY[] = "INY";
// constexpr char TEXT_JMP[] = "JMP";
// constexpr char TEXT_JSR[] = "JSR";
// constexpr char TEXT_LDA[] = "LDA";
// constexpr char TEXT_LDX[] = "LDX";
// constexpr char TEXT_LDY[] = "LDY";
// constexpr char TEXT_LSR[] = "LSR";
// constexpr char TEXT_NOP[] = "NOP";
// constexpr char TEXT_ORA[] = "ORA";
constexpr char TEXT_PHA[] = "PHA";
constexpr char TEXT_PHP[] = "PHP";
constexpr char TEXT_PLA[] = "PLA";
constexpr char TEXT_PLP[] = "PLP";
// constexpr char TEXT_ROL[] = "ROL";
// constexpr char TEXT_ROR[] = "ROR";
// constexpr char TEXT_RTI[] = "RTI";
// constexpr char TEXT_RTS[] = "RTS";
// constexpr char TEXT_SBC[] = "SBC";
// constexpr char TEXT_SEC[] = "SEC";
constexpr char TEXT_SED[] = "SED";
// constexpr char TEXT_SEI[] = "SEI";
// constexpr char TEXT_STA[] = "STA";
// constexpr char TEXT_STX[] = "STX";
// constexpr char TEXT_STY[] = "STY";
// constexpr char TEXT_TAX[] = "TAX";
constexpr char TEXT_TAY[] = "TAY";
// constexpr char TEXT_TSX[] = "TSX";
// constexpr char TEXT_TXA[] = "TXA";
// constexpr char TEXT_TXS[] = "TXS";
constexpr char TEXT_TYA[] = "TYA";

// G65SC02
// constexpr char TEXT_BRA[] = "BRA";
constexpr char TEXT_PHX[] = "PHX";
constexpr char TEXT_PHY[] = "PHY";
constexpr char TEXT_PLX[] = "PLX";
constexpr char TEXT_PLY[] = "PLY";
// constexpr char TEXT_STZ[] = "STZ";
constexpr char TEXT_TRB[] = "TRB";
constexpr char TEXT_TSB[] = "TSB";

// R65C02
constexpr char TEXT_BBR0[] = "BBR0";
constexpr char TEXT_BBR1[] = "BBR1";
constexpr char TEXT_BBR2[] = "BBR2";
constexpr char TEXT_BBR3[] = "BBR3";
constexpr char TEXT_BBR4[] = "BBR4";
constexpr char TEXT_BBR5[] = "BBR5";
constexpr char TEXT_BBR6[] = "BBR6";
constexpr char TEXT_BBR7[] = "BBR7";
constexpr char TEXT_BBS0[] = "BBS0";
constexpr char TEXT_BBS1[] = "BBS1";
constexpr char TEXT_BBS2[] = "BBS2";
constexpr char TEXT_BBS3[] = "BBS3";
constexpr char TEXT_BBS4[] = "BBS4";
constexpr char TEXT_BBS5[] = "BBS5";
constexpr char TEXT_BBS6[] = "BBS6";
constexpr char TEXT_BBS7[] = "BBS7";
constexpr char TEXT_RMB0[] = "RMB0";
constexpr char TEXT_RMB1[] = "RMB1";
constexpr char TEXT_RMB2[] = "RMB2";
constexpr char TEXT_RMB3[] = "RMB3";
constexpr char TEXT_RMB4[] = "RMB4";
constexpr char TEXT_RMB5[] = "RMB5";
constexpr char TEXT_RMB6[] = "RMB6";
constexpr char TEXT_RMB7[] = "RMB7";
constexpr char TEXT_SMB0[] = "SMB0";
constexpr char TEXT_SMB1[] = "SMB1";
constexpr char TEXT_SMB2[] = "SMB2";
constexpr char TEXT_SMB3[] = "SMB3";
constexpr char TEXT_SMB4[] = "SMB4";
constexpr char TEXT_SMB5[] = "SMB5";
constexpr char TEXT_SMB6[] = "SMB6";
constexpr char TEXT_SMB7[] = "SMB7";

// W65C02
constexpr char TEXT_STP[] = "STP";
// constexpr char TEXT_WAI[] = "WAI";

// W65C816
constexpr char TEXT_ADCL[] = "ADCL";
constexpr char TEXT_ANDL[] = "ANDL";
constexpr char TEXT_BRL[]  = "BRL";
// constexpr char TEXT_CMPL[] = "CMPL";
constexpr char TEXT_COP[]  = "COP";
constexpr char TEXT_EORL[] = "EORL";
constexpr char TEXT_JML[]  = "JML";
// constexpr char TEXT_JMPL[] = "JMPL";
constexpr char TEXT_JSL[]  = "JSL";
constexpr char TEXT_LDAL[] = "LDAL";
constexpr char TEXT_MVN[]  = "MVN";
constexpr char TEXT_MVP[]  = "MVP";
constexpr char TEXT_ORAL[] = "ORAL";
// constexpr char TEXT_PEA[]  = "PEA";
constexpr char TEXT_PEI[]  = "PEI";
constexpr char TEXT_PER[]  = "PER";
constexpr char TEXT_PHB[]  = "PHB";
constexpr char TEXT_PHD[]  = "PHD";
constexpr char TEXT_PHK[]  = "PHK";
constexpr char TEXT_PLB[]  = "PLB";
constexpr char TEXT_PLD[]  = "PLD";
constexpr char TEXT_PSD[]  = "PSD";
// constexpr char TEXT_REP[]  = "REP";
// constexpr char TEXT_RTL[]  = "RTL";
constexpr char TEXT_SBCL[] = "SBCL";
// constexpr char TEXT_SEP[]  = "SEP";
constexpr char TEXT_STAL[] = "STAL";
constexpr char TEXT_TCD[]  = "TCD";
constexpr char TEXT_TCS[]  = "TCS";
constexpr char TEXT_TDC[]  = "TDC";
constexpr char TEXT_TSC[]  = "TSC";
// constexpr char TEXT_TXY[]  = "TXY";
// constexpr char TEXT_TYX[]  = "TYX";
constexpr char TEXT_WDM[]  = "WDM";
constexpr char TEXT_XBA[]  = "XBA";
constexpr char TEXT_XCE[]  = "XCE";

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
