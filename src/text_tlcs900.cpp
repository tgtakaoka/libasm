/*
 * Copyright 2025 Tadashi G. Takaoka
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

#include "text_tlcs900.h"

namespace libasm {
namespace text {
namespace tlcs900 {

constexpr char TEXT_TLCS900_LIST[] PROGMEM = "TLCS900, TLCS900L, TLCS900H, TLCS900H2";
constexpr char TEXT_CPU_TLCS900[]  PROGMEM = "TLCS900";
constexpr char TEXT_CPU_TLCS900H[] PROGMEM = "TLCS900H";
constexpr char TEXT_CPU_TLCS900L[] PROGMEM = "TLCS900L";
constexpr char TEXT_CPU_TLCS900H2[] PROGMEM = "TLCS900H2";

// clang-format off
// constexpr char TEXT_ADC[]   PROGMEM = "ADC";
// constexpr char TEXT_ADD[]   PROGMEM = "ADD";
constexpr char TEXT_ANDCF[] PROGMEM = "ANDCF";
// constexpr char TEXT_AND[]   PROGMEM = "AND";
// constexpr char TEXT_BIT[]   PROGMEM = "BIT";
constexpr char TEXT_BS1B[]  PROGMEM = "BS1B";
constexpr char TEXT_BS1F[]  PROGMEM = "BS1F";
// constexpr char TEXT_CALL[]  PROGMEM = "CALL";
// constexpr char TEXT_CALR[]  PROGMEM = "CALR";
// constexpr char TEXT_CCF[]   PROGMEM = "CCF";
constexpr char TEXT_CHG[]   PROGMEM = "CHG";
// constexpr char TEXT_CPD[]   PROGMEM = "CPD";
// constexpr char TEXT_CPDR[]  PROGMEM = "CPDR";
// constexpr char TEXT_CPI[]   PROGMEM = "CPI";
// constexpr char TEXT_CPIR[]  PROGMEM = "CPIR";
// constexpr char TEXT_CPL[]   PROGMEM = "CPL";
// constexpr char TEXT_CP[]    PROGMEM = "CP";
// constexpr char TEXT_DAA[]   PROGMEM = "DAA";
// constexpr char TEXT_DECF[]  PROGMEM = "DECF";
// constexpr char TEXT_DEC[]   PROGMEM = "DEC";
// constexpr char TEXT_DI[]    PROGMEM = "DI";
// constexpr char TEXT_DIV[]   PROGMEM = "DIV";
// constexpr char TEXT_DIVS[]  PROGMEM = "DIVS";
// constexpr char TEXT_DJNZ[]  PROGMEM = "DJNZ";
// constexpr char TEXT_EI[]    PROGMEM = "EI";
// constexpr char TEXT_EX[]    PROGMEM = "EX";
// constexpr char TEXT_EXTS[]  PROGMEM = "EXTS";
constexpr char TEXT_EXTZ[]  PROGMEM = "EXTZ";
// constexpr char TEXT_HALT[]  PROGMEM = "HALT";
// constexpr char TEXT_INCF[]  PROGMEM = "INCF";
// constexpr char TEXT_INC[]   PROGMEM = "INC";
// constexpr char TEXT_JP[]    PROGMEM = "JP";
// constexpr char TEXT_JRL[]   PROGMEM = "JRL";
// constexpr char TEXT_JR[]    PROGMEM = "JR";
// constexpr char TEXT_LDA[]   PROGMEM = "LDA";
// constexpr char TEXT_LDAR[]  PROGMEM = "LDAR";
constexpr char TEXT_LDCF[]  PROGMEM = "LDCF";
// constexpr char TEXT_LDC[]   PROGMEM = "LDC";
// constexpr char TEXT_LDD[]   PROGMEM = "LDD";
// constexpr char TEXT_LDDR[]  PROGMEM = "LDDR";
// constexpr char TEXT_LDF[]   PROGMEM = "LDF";
// constexpr char TEXT_LDI[]   PROGMEM = "LDI";
// constexpr char TEXT_LDIR[]  PROGMEM = "LDIR";
// constexpr char TEXT_LD[]    PROGMEM = "LD";
// constexpr char TEXT_LDX[]   PROGMEM = "LDX";
// constexpr char TEXT_LINK[]  PROGMEM = "LINK";
constexpr char TEXT_MAX[]   PROGMEM = "MAX";
constexpr char TEXT_MDEC1[] PROGMEM = "MDEC1";
constexpr char TEXT_MDEC2[] PROGMEM = "MDEC2";
constexpr char TEXT_MDEC4[] PROGMEM = "MDEC4";
constexpr char TEXT_MINC1[] PROGMEM = "MINC1";
constexpr char TEXT_MINC2[] PROGMEM = "MINC2";
constexpr char TEXT_MINC4[] PROGMEM = "MINC4";
constexpr char TEXT_MIN[]   PROGMEM = "MIN";
constexpr char TEXT_MIRR[]  PROGMEM = "MIRR";
constexpr char TEXT_MULA[]  PROGMEM = "MULA";
// constexpr char TEXT_MUL[]   PROGMEM = "MUL";
// constexpr char TEXT_MULS[]  PROGMEM = "MULS";
// constexpr char TEXT_NEG[]   PROGMEM = "NEG";
// constexpr char TEXT_NOP[]   PROGMEM = "NOP";
constexpr char TEXT_NORMAL[] PROGMEM = "NORMAL";
constexpr char TEXT_ORCF[]  PROGMEM = "ORCF";
// constexpr char TEXT_OR[]    PROGMEM = "OR";
constexpr char TEXT_PAA[]   PROGMEM = "PAA";
// constexpr char TEXT_POP[]   PROGMEM = "POP";
// constexpr char TEXT_PUSH[]  PROGMEM = "PUSH";
// constexpr char TEXT_RCF[]   PROGMEM = "RCF";
// constexpr char TEXT_RES[]   PROGMEM = "RES";
// constexpr char TEXT_RETD[]  PROGMEM = "RETD";
// constexpr char TEXT_RETI[]  PROGMEM = "RETI";
// constexpr char TEXT_RET[]   PROGMEM = "RET";
// constexpr char TEXT_RLC[]   PROGMEM = "RLC";
// constexpr char TEXT_RLD[]   PROGMEM = "RLD";
// constexpr char TEXT_RL[]    PROGMEM = "RL";
// constexpr char TEXT_RRC[]   PROGMEM = "RRC";
// constexpr char TEXT_RRD[]   PROGMEM = "RRD";
// constexpr char TEXT_RR[]    PROGMEM = "RR";
// constexpr char TEXT_SBC[]   PROGMEM = "SBC";
// constexpr char TEXT_SCC[]   PROGMEM = "SCC";
// constexpr char TEXT_SCF[]   PROGMEM = "SCF";
// constexpr char TEXT_SET[]   PROGMEM = "SET";
// constexpr char TEXT_SLA[]   PROGMEM = "SLA";
// constexpr char TEXT_SLL[]   PROGMEM = "SLL";
// constexpr char TEXT_SRA[]   PROGMEM = "SRA";
// constexpr char TEXT_SRL[]   PROGMEM = "SRL";
constexpr char TEXT_STCF[]  PROGMEM = "STCF";
// constexpr char TEXT_SUB[]   PROGMEM = "SUB";
// constexpr char TEXT_SWI[]   PROGMEM = "SWI";
// constexpr char TEXT_TSET[]  PROGMEM = "TSET";
// constexpr char TEXT_UNLK[]  PROGMEM = "UNLK";
constexpr char TEXT_XORCF[] PROGMEM = "XORCF";
// constexpr char TEXT_XOR[]   PROGMEM = "XOR";
constexpr char TEXT_ZCF[]   PROGMEM = "ZCF";

// constexpr char TEXT_REG_W[]   PROGMEM = "W";
// constexpr char TEXT_REG_A[]   PROGMEM = "A";
// constexpr char TEXT_REG_B[]   PROGMEM = "B";
// constexpr char TEXT_REG_C[]   PROGMEM = "C";
// constexpr char TEXT_REG_D[]   PROGMEM = "D";
// constexpr char TEXT_REG_E[]   PROGMEM = "E";
// constexpr char TEXT_REG_H[]   PROGMEM = "H";
// constexpr char TEXT_REG_L[]   PROGMEM = "L";
constexpr char TEXT_REG_RW[]  PROGMEM = "RW";
constexpr char TEXT_REG_RA[]  PROGMEM = "RA";
constexpr char TEXT_REG_RB[]  PROGMEM = "RB";
// constexpr char TEXT_REG_RC[]  PROGMEM = "RC";
constexpr char TEXT_REG_RD[]  PROGMEM = "RD";
// constexpr char TEXT_REG_RE[]  PROGMEM = "RE";
// constexpr char TEXT_REG_RH[]  PROGMEM = "RH";
// constexpr char TEXT_REG_RL[]  PROGMEM = "RL";
constexpr char TEXT_REG_QW[]  PROGMEM = "QW";
constexpr char TEXT_REG_QA[]  PROGMEM = "QA";
constexpr char TEXT_REG_QB[]  PROGMEM = "QB";
constexpr char TEXT_REG_QC[]  PROGMEM = "QC";
constexpr char TEXT_REG_QD[]  PROGMEM = "QD";
constexpr char TEXT_REG_QE[]  PROGMEM = "QE";
constexpr char TEXT_REG_QF[]  PROGMEM = "QF";
constexpr char TEXT_REG_QH[]  PROGMEM = "QH";
// constexpr char TEXT_REG_QL[]  PROGMEM = "QL";
// constexpr char TEXT_REG_IXH[] PROGMEM = "IXH";
// constexpr char TEXT_REG_IXL[] PROGMEM = "IXL";
// constexpr char TEXT_REG_IYH[] PROGMEM = "IYH";
// constexpr char TEXT_REG_IYL[] PROGMEM = "IYL";
constexpr char TEXT_REG_IZH[] PROGMEM = "IZH";
constexpr char TEXT_REG_IZL[] PROGMEM = "IZL";
constexpr char TEXT_REG_SPH[] PROGMEM = "SPH";
constexpr char TEXT_REG_SPL[] PROGMEM = "SPL";
constexpr char TEXT_REG_QIXH[] PROGMEM = "QIXH";
constexpr char TEXT_REG_QIXL[] PROGMEM = "QIXL";
constexpr char TEXT_REG_QIYH[] PROGMEM = "QIYH";
constexpr char TEXT_REG_QIYL[] PROGMEM = "QIYL";
constexpr char TEXT_REG_QIZH[] PROGMEM = "QIZH";
constexpr char TEXT_REG_QIZL[] PROGMEM = "QIZL";
constexpr char TEXT_REG_QSPH[] PROGMEM = "QSPH";
constexpr char TEXT_REG_QSPL[] PROGMEM = "QSPL";
constexpr char TEXT_REG_WA[]  PROGMEM = "WA";
// constexpr char TEXT_REG_BC[]  PROGMEM = "BC";
// constexpr char TEXT_REG_DE[]  PROGMEM = "DE";
// constexpr char TEXT_REG_HL[]  PROGMEM = "HL";
// constexpr char TEXT_REG_IX[]  PROGMEM = "IX";
// constexpr char TEXT_REG_IY[]  PROGMEM = "IY";
constexpr char TEXT_REG_IZ[]  PROGMEM = "IZ";
// constexpr char TEXT_REG_SP[]  PROGMEM = "SP";
constexpr char TEXT_REG_QWA[] PROGMEM = "QWA";
constexpr char TEXT_REG_QBC[] PROGMEM = "QBC";
constexpr char TEXT_REG_QDE[] PROGMEM = "QDE";
constexpr char TEXT_REG_QHL[] PROGMEM = "QHL";
constexpr char TEXT_REG_QIX[] PROGMEM = "QIX";
constexpr char TEXT_REG_QIY[] PROGMEM = "QIY";
constexpr char TEXT_REG_QIZ[] PROGMEM = "QIZ";
constexpr char TEXT_REG_QSP[] PROGMEM = "QSP";
constexpr char TEXT_REG_RWA[] PROGMEM = "RWA";
constexpr char TEXT_REG_RBC[] PROGMEM = "RBC";
constexpr char TEXT_REG_RDE[] PROGMEM = "RDE";
constexpr char TEXT_REG_RHL[] PROGMEM = "RHL";
constexpr char TEXT_REG_XWA[] PROGMEM = "XWA";
constexpr char TEXT_REG_XBC[] PROGMEM = "XBC";
constexpr char TEXT_REG_XDE[] PROGMEM = "XDE";
constexpr char TEXT_REG_XHL[] PROGMEM = "XHL";
constexpr char TEXT_REG_XIX[] PROGMEM = "XIX";
constexpr char TEXT_REG_XIY[] PROGMEM = "XIY";
constexpr char TEXT_REG_XIZ[] PROGMEM = "XIZ";
constexpr char TEXT_REG_XSP[] PROGMEM = "XSP";
// constexpr char TEXT_REG_F[]   PROGMEM = "F";
// constexpr char TEXT_REG_SR[]  PROGMEM = "SR";
// constexpr char TEXT_REG_PC[]  PROGMEM = "PC";
constexpr char TEXT_REG_NXSP[] PROGMEM = "NXSP";

// constexpr char TEXT_CC_C[]   PROGMEM = "C";
// constexpr char TEXT_CC_EQ[]  PROGMEM = "EQ";
// constexpr char TEXT_CC_F[]   PROGMEM = "F";
// constexpr char TEXT_CC_GE[]  PROGMEM = "GE";
// constexpr char TEXT_CC_GT[]  PROGMEM = "GT";
// constexpr char TEXT_CC_LE[]  PROGMEM = "LE";
// constexpr char TEXT_CC_LT[]  PROGMEM = "LT";
// constexpr char TEXT_CC_MI[]  PROGMEM = "MI";
// constexpr char TEXT_CC_M[]   PROGMEM = "M";
// constexpr char TEXT_CC_NC[]  PROGMEM = "NC";
// constexpr char TEXT_CC_NE[]  PROGMEM = "NE";
// constexpr char TEXT_CC_NOV[] PROGMEM = "NOV";
// constexpr char TEXT_CC_NZ[]  PROGMEM = "NZ";
// constexpr char TEXT_CC_OV[]  PROGMEM = "OV";
// constexpr char TEXT_CC_PE[]  PROGMEM = "PE";
// constexpr char TEXT_CC_PL[]  PROGMEM = "PL";
// constexpr char TEXT_CC_PO[]  PROGMEM = "PO";
// constexpr char TEXT_CC_P[]   PROGMEM = "P";
// constexpr char TEXT_CC_UGE[] PROGMEM = "UGE";
// constexpr char TEXT_CC_UGT[] PROGMEM = "UGT";
// constexpr char TEXT_CC_ULE[] PROGMEM = "ULE";
// constexpr char TEXT_CC_ULT[] PROGMEM = "ULT";
// constexpr char TEXT_CC_Z[]   PROGMEM = "Z";
// clang-format on

}  // namespace tlcs900
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
