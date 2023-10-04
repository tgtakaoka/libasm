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

#include "text_tlcs90.h"

namespace libasm {
namespace text {
namespace tlcs90 {

constexpr char TEXT_CPU_TLCS90[] PROGMEM = "TLCS90";

// clang-format off
// constexpr char TEXT_ADC[]  PROGMEM = "ADC";
// constexpr char TEXT_ADD[]  PROGMEM = "ADD";
// constexpr char TEXT_AND[]  PROGMEM = "AND";
// constexpr char TEXT_BIT[]  PROGMEM = "BIT";
// constexpr char TEXT_CALL[] PROGMEM = "CALL";
// constexpr char TEXT_CALR[] PROGMEM = "CALR";
// constexpr char TEXT_CCF[]  PROGMEM = "CCF";
// constexpr char TEXT_CP[]   PROGMEM = "CP";
// constexpr char TEXT_CPD[]  PROGMEM = "CPD";
// constexpr char TEXT_CPDR[] PROGMEM = "CPDR";
// constexpr char TEXT_CPI[]  PROGMEM = "CPI";
// constexpr char TEXT_CPIR[] PROGMEM = "CPIR";
// constexpr char TEXT_CPL[]  PROGMEM = "CPL";
// constexpr char TEXT_DAA[]  PROGMEM = "DAA";
// constexpr char TEXT_DEC[]  PROGMEM = "DEC";
// constexpr char TEXT_DECW[] PROGMEM = "DECW";
// constexpr char TEXT_DECX[] PROGMEM = "DECX";
// constexpr char TEXT_DI[]   PROGMEM = "DI";
// constexpr char TEXT_DIV[]  PROGMEM = "DIV";
// constexpr char TEXT_DJNZ[] PROGMEM = "DJNZ";
// constexpr char TEXT_EI[]   PROGMEM = "EI";
// constexpr char TEXT_EX[]   PROGMEM = "EX";
// constexpr char TEXT_EXX[]  PROGMEM = "EXX";
// constexpr char TEXT_HALT[] PROGMEM = "HALT";
// constexpr char TEXT_INC[]  PROGMEM = "INC";
// constexpr char TEXT_INCW[] PROGMEM = "INCW";
// constexpr char TEXT_INCX[] PROGMEM = "INCX";
// constexpr char TEXT_JP[]   PROGMEM = "JP";
// constexpr char TEXT_JR[]   PROGMEM = "JR";
constexpr char TEXT_JRL[]  PROGMEM = "JRL";
// constexpr char TEXT_LD[]   PROGMEM = "LD";
// constexpr char TEXT_LDA[]  PROGMEM = "LDA";
// constexpr char TEXT_LDAR[] PROGMEM = "LDAR";
// constexpr char TEXT_LDD[]  PROGMEM = "LDD";
// constexpr char TEXT_LDDR[] PROGMEM = "LDDR";
// constexpr char TEXT_LDI[]  PROGMEM = "LDI";
// constexpr char TEXT_LDIR[] PROGMEM = "LDIR";
// constexpr char TEXT_LDW[]  PROGMEM = "LDW";
// constexpr char TEXT_MUL[]  PROGMEM = "MUL";
// constexpr char TEXT_NEG[]  PROGMEM = "NEG";
// constexpr char TEXT_NOP[]  PROGMEM = "NOP";
// constexpr char TEXT_OR[]   PROGMEM = "OR";
// constexpr char TEXT_POP[]  PROGMEM = "POP";
// constexpr char TEXT_PUSH[] PROGMEM = "PUSH";
// constexpr char TEXT_RCF[]  PROGMEM = "RCF";
// constexpr char TEXT_RES[]  PROGMEM = "RES";
// constexpr char TEXT_RET[]  PROGMEM = "RET";
// constexpr char TEXT_RETI[] PROGMEM = "RETI";
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
// constexpr char TEXT_SBC[]  PROGMEM = "SBC";
// constexpr char TEXT_SCF[]  PROGMEM = "SCF";
// constexpr char TEXT_SET[]  PROGMEM = "SET";
// constexpr char TEXT_SLA[]  PROGMEM = "SLA";
constexpr char TEXT_SLAA[] PROGMEM = "SLAA";
// constexpr char TEXT_SLL[]  PROGMEM = "SLL";
constexpr char TEXT_SLLA[] PROGMEM = "SLLA";
// constexpr char TEXT_SRA[]  PROGMEM = "SRA";
constexpr char TEXT_SRAA[] PROGMEM = "SRAA";
// constexpr char TEXT_SRL[]  PROGMEM = "SRL";
constexpr char TEXT_SRLA[] PROGMEM = "SRLA";
// constexpr char TEXT_SUB[]  PROGMEM = "SUB";
// constexpr char TEXT_SWI[]  PROGMEM = "SWI";
// constexpr char TEXT_TSET[] PROGMEM = "TSET";
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

// constexpr char TEXT_CC_F[]   PROGMEM = "F";
// constexpr char TEXT_CC_LT[]  PROGMEM = "LT";
// constexpr char TEXT_CC_LE[]  PROGMEM = "LE";
// constexpr char TEXT_CC_ULE[] PROGMEM = "ULE";
// constexpr char TEXT_CC_OV[]  PROGMEM = "OV";
// constexpr char TEXT_CC_MI[]  PROGMEM = "MI";
// constexpr char TEXT_CC_Z[]   PROGMEM = "Z";
// constexpr char TEXT_CC_C[]   PROGMEM = "C";
// constexpr char TEXT_CC_T[]   PROGMEM = "";
// constexpr char TEXT_CC_GE[]  PROGMEM = "GE";
// constexpr char TEXT_CC_GT[]  PROGMEM = "GT";
// constexpr char TEXT_CC_UGT[] PROGMEM = "UGT";
// constexpr char TEXT_CC_NOV[] PROGMEM = "NOV";
// constexpr char TEXT_CC_PL[]  PROGMEM = "PL";
// constexpr char TEXT_CC_NZ[]  PROGMEM = "NZ";
// constexpr char TEXT_CC_NC[]  PROGMEM = "NC";
// constexpr char TEXT_CC_PE[]  PROGMEM = "PE";
// constexpr char TEXT_CC_M[]   PROGMEM = "M";
// constexpr char TEXT_CC_EQ[]  PROGMEM = "EQ";
// constexpr char TEXT_CC_ULT[] PROGMEM = "ULT";
// constexpr char TEXT_CC_PO[]  PROGMEM = "PO";
// constexpr char TEXT_CC_P[]   PROGMEM = "P";
// constexpr char TEXT_CC_NE[]  PROGMEM = "NE";
// constexpr char TEXT_CC_UGE[] PROGMEM = "UGE";
// clang-format on

}  // namespace tlcs90
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
