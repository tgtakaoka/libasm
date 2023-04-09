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

const char TEXT_CPU_TLCS90[] PROGMEM = "TLCS90";

// clang-format off
// const char TEXT_ADC[]  PROGMEM = "ADC";
// const char TEXT_ADD[]  PROGMEM = "ADD";
// const char TEXT_AND[]  PROGMEM = "AND";
// const char TEXT_BIT[]  PROGMEM = "BIT";
// const char TEXT_CALL[] PROGMEM = "CALL";
// const char TEXT_CALR[] PROGMEM = "CALR";
// const char TEXT_CCF[]  PROGMEM = "CCF";
// const char TEXT_CP[]   PROGMEM = "CP";
// const char TEXT_CPD[]  PROGMEM = "CPD";
// const char TEXT_CPDR[] PROGMEM = "CPDR";
// const char TEXT_CPI[]  PROGMEM = "CPI";
// const char TEXT_CPIR[] PROGMEM = "CPIR";
// const char TEXT_CPL[]  PROGMEM = "CPL";
// const char TEXT_DAA[]  PROGMEM = "DAA";
// const char TEXT_DEC[]  PROGMEM = "DEC";
// const char TEXT_DECW[] PROGMEM = "DECW";
// const char TEXT_DECX[] PROGMEM = "DECX";
// const char TEXT_DI[]   PROGMEM = "DI";
// const char TEXT_DIV[]  PROGMEM = "DIV";
// const char TEXT_DJNZ[] PROGMEM = "DJNZ";
// const char TEXT_EI[]   PROGMEM = "EI";
// const char TEXT_EX[]   PROGMEM = "EX";
// const char TEXT_EXX[]  PROGMEM = "EXX";
// const char TEXT_HALT[] PROGMEM = "HALT";
// const char TEXT_INC[]  PROGMEM = "INC";
// const char TEXT_INCW[] PROGMEM = "INCW";
// const char TEXT_INCX[] PROGMEM = "INCX";
// const char TEXT_JP[]   PROGMEM = "JP";
// const char TEXT_JR[]   PROGMEM = "JR";
const char TEXT_JRL[]  PROGMEM = "JRL";
// const char TEXT_LD[]   PROGMEM = "LD";
// const char TEXT_LDA[]  PROGMEM = "LDA";
// const char TEXT_LDAR[] PROGMEM = "LDAR";
// const char TEXT_LDD[]  PROGMEM = "LDD";
// const char TEXT_LDDR[] PROGMEM = "LDDR";
// const char TEXT_LDI[]  PROGMEM = "LDI";
// const char TEXT_LDIR[] PROGMEM = "LDIR";
// const char TEXT_LDW[]  PROGMEM = "LDW";
// const char TEXT_MUL[]  PROGMEM = "MUL";
// const char TEXT_NEG[]  PROGMEM = "NEG";
// const char TEXT_NOP[]  PROGMEM = "NOP";
// const char TEXT_OR[]   PROGMEM = "OR";
// const char TEXT_POP[]  PROGMEM = "POP";
// const char TEXT_PUSH[] PROGMEM = "PUSH";
// const char TEXT_RCF[]  PROGMEM = "RCF";
// const char TEXT_RES[]  PROGMEM = "RES";
// const char TEXT_RET[]  PROGMEM = "RET";
// const char TEXT_RETI[] PROGMEM = "RETI";
// const char TEXT_RL[]   PROGMEM = "RL";
// const char TEXT_RLA[]  PROGMEM = "RLA";
// const char TEXT_RLC[]  PROGMEM = "RLC";
// const char TEXT_RLCA[] PROGMEM = "RLCA";
// const char TEXT_RLD[]  PROGMEM = "RLD";
// const char TEXT_RR[]   PROGMEM = "RR";
// const char TEXT_RRA[]  PROGMEM = "RRA";
// const char TEXT_RRC[]  PROGMEM = "RRC";
// const char TEXT_RRCA[] PROGMEM = "RRCA";
// const char TEXT_RRD[]  PROGMEM = "RRD";
// const char TEXT_SBC[]  PROGMEM = "SBC";
// const char TEXT_SCF[]  PROGMEM = "SCF";
// const char TEXT_SET[]  PROGMEM = "SET";
// const char TEXT_SLA[]  PROGMEM = "SLA";
const char TEXT_SLAA[] PROGMEM = "SLAA";
// const char TEXT_SLL[]  PROGMEM = "SLL";
const char TEXT_SLLA[] PROGMEM = "SLLA";
// const char TEXT_SRA[]  PROGMEM = "SRA";
const char TEXT_SRAA[] PROGMEM = "SRAA";
// const char TEXT_SRL[]  PROGMEM = "SRL";
const char TEXT_SRLA[] PROGMEM = "SRLA";
// const char TEXT_SUB[]  PROGMEM = "SUB";
// const char TEXT_SWI[]  PROGMEM = "SWI";
// const char TEXT_TSET[] PROGMEM = "TSET";
// const char TEXT_XOR[]  PROGMEM = "XOR";

// const char TEXT_REG_BC[]  PROGMEM = "BC";
// const char TEXT_REG_DE[]  PROGMEM = "DE";
// const char TEXT_REG_HL[]  PROGMEM = "HL";
// const char TEXT_REG_IX[]  PROGMEM = "IX";
// const char TEXT_REG_IY[]  PROGMEM = "IY";
// const char TEXT_REG_SP[]  PROGMEM = "SP";
// const char TEXT_REG_AFP[] PROGMEM = "AF'";
// const char TEXT_REG_AF[]  PROGMEM = "AF";
// const char TEXT_REG_A[]   PROGMEM = "A";
// const char TEXT_REG_B[]   PROGMEM = "B";
// const char TEXT_REG_C[]   PROGMEM = "C";
// const char TEXT_REG_D[]   PROGMEM = "D";
// const char TEXT_REG_E[]   PROGMEM = "E";
// const char TEXT_REG_H[]   PROGMEM = "H";
// const char TEXT_REG_L[]   PROGMEM = "L";

// const char TEXT_CC_F[]   PROGMEM = "F";
// const char TEXT_CC_LT[]  PROGMEM = "LT";
// const char TEXT_CC_LE[]  PROGMEM = "LE";
// const char TEXT_CC_ULE[] PROGMEM = "ULE";
// const char TEXT_CC_OV[]  PROGMEM = "OV";
// const char TEXT_CC_MI[]  PROGMEM = "MI";
// const char TEXT_CC_Z[]   PROGMEM = "Z";
// const char TEXT_CC_C[]   PROGMEM = "C";
// const char TEXT_CC_T[]   PROGMEM = "";
// const char TEXT_CC_GE[]  PROGMEM = "GE";
// const char TEXT_CC_GT[]  PROGMEM = "GT";
// const char TEXT_CC_UGT[] PROGMEM = "UGT";
// const char TEXT_CC_NOV[] PROGMEM = "NOV";
// const char TEXT_CC_PL[]  PROGMEM = "PL";
// const char TEXT_CC_NZ[]  PROGMEM = "NZ";
// const char TEXT_CC_NC[]  PROGMEM = "NC";
// const char TEXT_CC_PE[]  PROGMEM = "PE";
// const char TEXT_CC_M[]   PROGMEM = "M";
// const char TEXT_CC_EQ[]  PROGMEM = "EQ";
// const char TEXT_CC_ULT[] PROGMEM = "ULT";
// const char TEXT_CC_PO[]  PROGMEM = "PO";
// const char TEXT_CC_P[]   PROGMEM = "P";
// const char TEXT_CC_NE[]  PROGMEM = "NE";
// const char TEXT_CC_UGE[] PROGMEM = "UGE";
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