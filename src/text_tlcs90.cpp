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
// clang-format off

constexpr char TEXT_CPU_TLCS90[] = "TLCS90";

// constexpr char TEXT_ADC[]  = "ADC";
// constexpr char TEXT_ADD[]  = "ADD";
// constexpr char TEXT_AND[]  = "AND";
// constexpr char TEXT_BIT[]  = "BIT";
// constexpr char TEXT_CALL[] = "CALL";
// constexpr char TEXT_CALR[] = "CALR";
// constexpr char TEXT_CCF[]  = "CCF";
// constexpr char TEXT_CP[]   = "CP";
// constexpr char TEXT_CPD[]  = "CPD";
// constexpr char TEXT_CPDR[] = "CPDR";
// constexpr char TEXT_CPI[]  = "CPI";
// constexpr char TEXT_CPIR[] = "CPIR";
// constexpr char TEXT_CPL[]  = "CPL";
// constexpr char TEXT_DAA[]  = "DAA";
// constexpr char TEXT_DEC[]  = "DEC";
// constexpr char TEXT_DECW[] = "DECW";
// constexpr char TEXT_DECX[] = "DECX";
// constexpr char TEXT_DI[]   = "DI";
// constexpr char TEXT_DIV[]  = "DIV";
// constexpr char TEXT_DJNZ[] = "DJNZ";
// constexpr char TEXT_EI[]   = "EI";
// constexpr char TEXT_EX[]   = "EX";
// constexpr char TEXT_EXX[]  = "EXX";
// constexpr char TEXT_HALT[] = "HALT";
// constexpr char TEXT_INC[]  = "INC";
// constexpr char TEXT_INCW[] = "INCW";
// constexpr char TEXT_INCX[] = "INCX";
// constexpr char TEXT_JP[]   = "JP";
// constexpr char TEXT_JR[]   = "JR";
// constexpr char TEXT_JRL[]  = "JRL";
// constexpr char TEXT_LD[]   = "LD";
// constexpr char TEXT_LDA[]  = "LDA";
// constexpr char TEXT_LDAR[] = "LDAR";
// constexpr char TEXT_LDD[]  = "LDD";
// constexpr char TEXT_LDDR[] = "LDDR";
// constexpr char TEXT_LDI[]  = "LDI";
// constexpr char TEXT_LDIR[] = "LDIR";
// constexpr char TEXT_LDW[]  = "LDW";
// constexpr char TEXT_MUL[]  = "MUL";
// constexpr char TEXT_NEG[]  = "NEG";
// constexpr char TEXT_NOP[]  = "NOP";
// constexpr char TEXT_OR[]   = "OR";
// constexpr char TEXT_POP[]  = "POP";
// constexpr char TEXT_PUSH[] = "PUSH";
// constexpr char TEXT_RCF[]  = "RCF";
// constexpr char TEXT_RES[]  = "RES";
// constexpr char TEXT_RET[]  = "RET";
// constexpr char TEXT_RETI[] = "RETI";
// constexpr char TEXT_RL[]   = "RL";
// constexpr char TEXT_RLA[]  = "RLA";
// constexpr char TEXT_RLC[]  = "RLC";
// constexpr char TEXT_RLCA[] = "RLCA";
// constexpr char TEXT_RLD[]  = "RLD";
// constexpr char TEXT_RR[]   = "RR";
// constexpr char TEXT_RRA[]  = "RRA";
// constexpr char TEXT_RRC[]  = "RRC";
// constexpr char TEXT_RRCA[] = "RRCA";
// constexpr char TEXT_RRD[]  = "RRD";
// constexpr char TEXT_SBC[]  = "SBC";
// constexpr char TEXT_SCF[]  = "SCF";
// constexpr char TEXT_SET[]  = "SET";
// constexpr char TEXT_SLA[]  = "SLA";
constexpr char TEXT_SLAA[] = "SLAA";
// constexpr char TEXT_SLL[]  = "SLL";
constexpr char TEXT_SLLA[] = "SLLA";
// constexpr char TEXT_SRA[]  = "SRA";
constexpr char TEXT_SRAA[] = "SRAA";
// constexpr char TEXT_SRL[]  = "SRL";
constexpr char TEXT_SRLA[] = "SRLA";
// constexpr char TEXT_SUB[]  = "SUB";
// constexpr char TEXT_SWI[]  = "SWI";
// constexpr char TEXT_TSET[] = "TSET";
// constexpr char TEXT_XOR[]  = "XOR";

// constexpr char TEXT_REG_A[]  = "A";
// constexpr char TEXT_REG_AF[] = "AF";
// constexpr char TEXT_REG_B[]  = "B";
// constexpr char TEXT_REG_BC[] = "BC";
// constexpr char TEXT_REG_C[]  = "C";
// constexpr char TEXT_REG_D[]  = "D";
// constexpr char TEXT_REG_DE[] = "DE";
// constexpr char TEXT_REG_E[]  = "E";
// constexpr char TEXT_REG_H[]  = "H";
// constexpr char TEXT_REG_HL[] = "HL";
// constexpr char TEXT_REG_IX[] = "IX";
// constexpr char TEXT_REG_IY[] = "IY";
// constexpr char TEXT_REG_L[]  = "L";
// constexpr char TEXT_REG_SP[] = "SP";

// constexpr char TEXT_CC_C[]   = "C";
// constexpr char TEXT_CC_EQ[]  = "EQ";
// constexpr char TEXT_CC_F[]   = "F";
// constexpr char TEXT_CC_GE[]  = "GE";
// constexpr char TEXT_CC_GT[]  = "GT";
// constexpr char TEXT_CC_LE[]  = "LE";
// constexpr char TEXT_CC_LT[]  = "LT";
// constexpr char TEXT_CC_M[]   = "M";
// constexpr char TEXT_CC_MI[]  = "MI";
// constexpr char TEXT_CC_NC[]  = "NC";
// constexpr char TEXT_CC_NE[]  = "NE";
// constexpr char TEXT_CC_NOV[] = "NOV";
// constexpr char TEXT_CC_NZ[]  = "NZ";
// constexpr char TEXT_CC_OV[]  = "OV";
// constexpr char TEXT_CC_P[]   = "P";
// constexpr char TEXT_CC_PE[]  = "PE";
// constexpr char TEXT_CC_PL[]  = "PL";
// constexpr char TEXT_CC_PO[]  = "PO";
// constexpr char TEXT_CC_T[]   = "";
// constexpr char TEXT_CC_UGE[] = "UGE";
// constexpr char TEXT_CC_UGT[] = "UGT";
// constexpr char TEXT_CC_ULE[] = "ULE";
// constexpr char TEXT_CC_ULT[] = "ULT";
// constexpr char TEXT_CC_Z[]   = "Z";

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
