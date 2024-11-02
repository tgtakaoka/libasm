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

#include "text_z80.h"

namespace libasm {
namespace text {
namespace z80 {

// clang-format off
constexpr char TEXT_CPU_Z80[]    PROGMEM = "Z80";

// constexpr char TEXT_ADC[]  PROGMEM = "ADC";
// constexpr char TEXT_ADD[]  PROGMEM = "ADD";
// constexpr char TEXT_AND[]  PROGMEM = "AND";
// constexpr char TEXT_BIT[]  PROGMEM = "BIT";
// constexpr char TEXT_CALL[] PROGMEM = "CALL";
// constexpr char TEXT_CCF[]  PROGMEM = "CCF";
// constexpr char TEXT_CP[]   PROGMEM = "CP";
// constexpr char TEXT_CPD[]  PROGMEM = "CPD";
// constexpr char TEXT_CPDR[] PROGMEM = "CPDR";
// constexpr char TEXT_CPI[]  PROGMEM = "CPI";
// constexpr char TEXT_CPIR[] PROGMEM = "CPIR";
// constexpr char TEXT_CPL[]  PROGMEM = "CPL";
// constexpr char TEXT_DAA[]  PROGMEM = "DAA";
// constexpr char TEXT_DEC[]  PROGMEM = "DEC";
// constexpr char TEXT_DI[]   PROGMEM = "DI";
// constexpr char TEXT_DJNZ[] PROGMEM = "DJNZ";
// constexpr char TEXT_EI[]   PROGMEM = "EI";
// constexpr char TEXT_EX[]   PROGMEM = "EX";
// constexpr char TEXT_EXX[]  PROGMEM = "EXX";
// constexpr char TEXT_HALT[] PROGMEM = "HALT";
// constexpr char TEXT_HLT[]  PROGMEM = "HLT";
constexpr char TEXT_IM[]   PROGMEM = "IM";
// constexpr char TEXT_IN[]   PROGMEM = "IN";
// constexpr char TEXT_INC[]  PROGMEM = "INC";
// constexpr char TEXT_IND[]  PROGMEM = "IND";
// constexpr char TEXT_INDR[] PROGMEM = "INDR";
// constexpr char TEXT_INI[]  PROGMEM = "INI";
// constexpr char TEXT_INIR[] PROGMEM = "INIR";
// constexpr char TEXT_JP[]   PROGMEM = "JP";
// constexpr char TEXT_JR[]   PROGMEM = "JR";
// constexpr char TEXT_LD[]   PROGMEM = "LD";
// constexpr char TEXT_LDD[]  PROGMEM = "LDD";
// constexpr char TEXT_LDDR[] PROGMEM = "LDDR";
// constexpr char TEXT_LDI[]  PROGMEM = "LDI";
// constexpr char TEXT_LDIR[] PROGMEM = "LDIR";
// constexpr char TEXT_NEG[]  PROGMEM = "NEG";
// constexpr char TEXT_NOP[]  PROGMEM = "NOP";
// constexpr char TEXT_OR[]   PROGMEM = "OR";
// constexpr char TEXT_OTDR[] PROGMEM = "OTDR";
// constexpr char TEXT_OTIR[] PROGMEM = "OTIR";
// constexpr char TEXT_OUT[]  PROGMEM = "OUT";
// constexpr char TEXT_OUTD[] PROGMEM = "OUTD";
// constexpr char TEXT_OUTI[] PROGMEM = "OUTI";
// constexpr char TEXT_POP[]  PROGMEM = "POP";
// constexpr char TEXT_PUSH[] PROGMEM = "PUSH";
// constexpr char TEXT_RES[]  PROGMEM = "RES";
// constexpr char TEXT_RET[]  PROGMEM = "RET";
// constexpr char TEXT_RETI[] PROGMEM = "RETI";
constexpr char TEXT_RETN[] PROGMEM = "RETN";
// constexpr char TEXT_RL[]   PROGMEM = "RL";
// constexpr char TEXT_RLA[]  PROGMEM = "RLA";
// constexpr char TEXT_RLC[]  PROGMEM = "RLC";
// constexpr char TEXT_RLCA[] PROGMEM = "RLCA";
// constexpr char TEXT_RLD[]  PROGMEM = "RLD";
// constexpr char TEXT_RR[]   PROGMEM = "RR";
// constexpr char TEXT_RRA[]  PROGMEM = "RRA";
// constexpr char TEXT_RRC[]  PROGMEM = "RRC";
// constexpr char TEXT_RRCA[] PROGMEM = "RRCA";
// constexpr char TEXT_RRD[]  PROGMEM = "RRD";
// constexpr char TEXT_RST[]  PROGMEM = "RST";
// constexpr char TEXT_SBC[]  PROGMEM = "SBC";
// constexpr char TEXT_SCF[]  PROGMEM = "SCF";
// constexpr char TEXT_SET[]  PROGMEM = "SET";
// constexpr char TEXT_SLA[]  PROGMEM = "SLA";
// constexpr char TEXT_SRA[]  PROGMEM = "SRA";
// constexpr char TEXT_SRL[]  PROGMEM = "SRL";
// constexpr char TEXT_SUB[]  PROGMEM = "SUB";
// constexpr char TEXT_XOR[]  PROGMEM = "XOR";

// constexpr char TEXT_REG_BC[]  PROGMEM = "BC";
// constexpr char TEXT_REG_DE[]  PROGMEM = "DE";
// constexpr char TEXT_REG_HL[]  PROGMEM = "HL";
// constexpr char TEXT_REG_IX[]  PROGMEM = "IX";
// constexpr char TEXT_REG_IY[]  PROGMEM = "IY";
// constexpr char TEXT_REG_SP[]  PROGMEM = "SP";
// constexpr char TEXT_REG_AFP[] PROGMEM = "AF'";
// constexpr char TEXT_REG_AF[]  PROGMEM = "AF";
// constexpr char TEXT_REG_A[]   PROGMEM = "A";
// constexpr char TEXT_REG_B[]   PROGMEM = "B";
// constexpr char TEXT_REG_C[]   PROGMEM = "C";
// constexpr char TEXT_REG_D[]   PROGMEM = "D";
// constexpr char TEXT_REG_E[]   PROGMEM = "E";
// constexpr char TEXT_REG_H[]   PROGMEM = "H";
// constexpr char TEXT_REG_L[]   PROGMEM = "L";
constexpr char TEXT_REG_R[]   PROGMEM = "R";
// constexpr char TEXT_REG_I[]   PROGMEM = "I";

// constexpr char TEXT_CC_NZ[] PROGMEM = "NZ";
// constexpr char TEXT_CC_Z[]  PROGMEM = "Z";
// constexpr char TEXT_CC_NC[] PROGMEM = "NC";
// constexpr char TEXT_CC_C[]  PROGMEM = "C";
// constexpr char TEXT_CC_PO[] PROGMEM = "PO";
// constexpr char TEXT_CC_PE[] PROGMEM = "PE";
// constexpr char TEXT_CC_P[]  PROGMEM = "P";
// constexpr char TEXT_CC_M[]  PROGMEM = "M";
// clang-format on

}  // namespace z80
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
