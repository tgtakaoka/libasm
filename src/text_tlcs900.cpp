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

constexpr char TEXT_TLCS900_LIST[] = "TLCS900, TLCS900L, TLCS900H, TLCS900H2";
constexpr char TEXT_CPU_TLCS900[] = "TLCS900";
constexpr char TEXT_CPU_TLCS900L[] = "TLCS900L";
constexpr char TEXT_CPU_TLCS900H[] = "TLCS900H";
constexpr char TEXT_CPU_TLCS900H2[] = "TLCS900H2";

constexpr char OPT_BOOL_MAXIMUM_MODE[] = "maximum-mode";
constexpr char OPT_DESC_MAXIMUM_MODE[] = "enable maximum mode";

// clang-format off
// constexpr char TEXT_ADC[]   = "ADC";
constexpr char TEXT_ADCW[] = "ADCW";
// constexpr char TEXT_ADD[]   = "ADD";
// constexpr char TEXT_ADDW[] = "ADDW";
constexpr char TEXT_ANDCF[] = "ANDCF";
// constexpr char TEXT_AND[]   = "AND";
// constexpr char TEXT_ANDW[] = "ANDW";
// constexpr char TEXT_BIT[]   = "BIT";
constexpr char TEXT_BS1B[]  = "BS1B";
constexpr char TEXT_BS1F[]  = "BS1F";
// constexpr char TEXT_CALL[]  = "CALL";
// constexpr char TEXT_CALR[]  = "CALR";
// constexpr char TEXT_CCF[]   = "CCF";
constexpr char TEXT_CHG[]   = "CHG";
// constexpr char TEXT_CPD[]   = "CPD";
// constexpr char TEXT_CPDR[]  = "CPDR";
// constexpr char TEXT_CPI[]   = "CPI";
// constexpr char TEXT_CPIR[]  = "CPIR";
// constexpr char TEXT_CPL[]   = "CPL";
// constexpr char TEXT_CP[]    = "CP";
// constexpr char TEXT_CPW[] = "CPW";
// constexpr char TEXT_DAA[]   = "DAA";
// constexpr char TEXT_DECF[]  = "DECF";
// constexpr char TEXT_DEC[]   = "DEC";
// constexpr char TEXT_DECW[] = "DECW";
// constexpr char TEXT_DI[]    = "DI";
// constexpr char TEXT_DIV[]   = "DIV";
// constexpr char TEXT_DIVS[]  = "DIVS";
// constexpr char TEXT_DJNZ[]  = "DJNZ";
// constexpr char TEXT_EI[]    = "EI";
// constexpr char TEXT_EX[]    = "EX";
// constexpr char TEXT_EXTS[]  = "EXTS";
constexpr char TEXT_EXTZ[]  = "EXTZ";
// constexpr char TEXT_HALT[]  = "HALT";
// constexpr char TEXT_INCF[]  = "INCF";
// constexpr char TEXT_INC[]   = "INC";
// constexpr char TEXT_INCW[] = "INCW";
// constexpr char TEXT_JP[]    = "JP";
// constexpr char TEXT_JRL[]   = "JRL";
// constexpr char TEXT_JR[]    = "JR";
// constexpr char TEXT_LDA[]   = "LDA";
// constexpr char TEXT_LDAR[]  = "LDAR";
constexpr char TEXT_LDCF[]  = "LDCF";
// constexpr char TEXT_LDC[]   = "LDC";
// constexpr char TEXT_LDD[]   = "LDD";
constexpr char TEXT_LDDW[] = "LDDW";
// constexpr char TEXT_LDDR[]  = "LDDR";
constexpr char TEXT_LDDRW[] = "LDDRW";
// constexpr char TEXT_LDF[]   = "LDF";
// constexpr char TEXT_LDI[]   = "LDI";
constexpr char TEXT_LDIW[] = "LDIW";
// constexpr char TEXT_LDIR[]  = "LDIR";
constexpr char TEXT_LDIRW[] = "LDIRW";
// constexpr char TEXT_LD[]    = "LD";
// constexpr char TEXT_LDW[]   = "LDW";
// constexpr char TEXT_LDX[]   = "LDX";
// constexpr char TEXT_LINK[]  = "LINK";
constexpr char TEXT_MAX[]     = "MAX";
// constexpr char TEXT_MAXMODE[] = "MAXMODE";
constexpr char TEXT_MDEC1[]   = "MDEC1";
constexpr char TEXT_MDEC2[] = "MDEC2";
constexpr char TEXT_MDEC4[] = "MDEC4";
constexpr char TEXT_MINC1[] = "MINC1";
constexpr char TEXT_MINC2[] = "MINC2";
constexpr char TEXT_MINC4[] = "MINC4";
constexpr char TEXT_MIN[]   = "MIN";
constexpr char TEXT_MIRR[]  = "MIRR";
constexpr char TEXT_MULA[]  = "MULA";
// constexpr char TEXT_MUL[]   = "MUL";
// constexpr char TEXT_MULS[]  = "MULS";
// constexpr char TEXT_NEG[]   = "NEG";
// constexpr char TEXT_NOP[]   = "NOP";
constexpr char TEXT_NORMAL[] = "NORMAL";
constexpr char TEXT_ORCF[]  = "ORCF";
// constexpr char TEXT_OR[]    = "OR";
// constexpr char TEXT_ORW[] = "ORW";
constexpr char TEXT_PAA[]   = "PAA";
// constexpr char TEXT_POP[]   = "POP";
constexpr char TEXT_POPW[] = "POPW";
// constexpr char TEXT_PUSH[]  = "PUSH";
constexpr char TEXT_PUSHW[]  = "PUSHW";
// constexpr char TEXT_RCF[]   = "RCF";
// constexpr char TEXT_RES[]   = "RES";
// constexpr char TEXT_RETD[]  = "RETD";
// constexpr char TEXT_RETI[]  = "RETI";
// constexpr char TEXT_RET[]   = "RET";
// constexpr char TEXT_RLC[]   = "RLC";
constexpr char TEXT_RLCW[] = "RLCW";
// constexpr char TEXT_RLD[]   = "RLD";
// constexpr char TEXT_RL[]    = "RL";
constexpr char TEXT_RLW[] = "RLW";
// constexpr char TEXT_RRC[]   = "RRC";
constexpr char TEXT_RRCW[] = "RRCW";
// constexpr char TEXT_RRD[]   = "RRD";
// constexpr char TEXT_RR[]    = "RR";
constexpr char TEXT_RRW[] = "RRW";
// constexpr char TEXT_SBC[]   = "SBC";
constexpr char TEXT_SBCW[] = "SBCW";
// constexpr char TEXT_SCC[]   = "SCC";
// constexpr char TEXT_SCF[]   = "SCF";
// constexpr char TEXT_SET[]   = "SET";
// constexpr char TEXT_SLA[]   = "SLA";
constexpr char TEXT_SLAW[] = "SLAW";
// constexpr char TEXT_SLL[]   = "SLL";
constexpr char TEXT_SLLW[] = "SLLW";
// constexpr char TEXT_SRA[]   = "SRA";
constexpr char TEXT_SRAW[] = "SRAW";
// constexpr char TEXT_SRL[]   = "SRL";
constexpr char TEXT_SRLW[] = "SRLW";
constexpr char TEXT_STCF[]  = "STCF";
// constexpr char TEXT_SUB[]   = "SUB";
// constexpr char TEXT_SUBW[] = "SUBW";
// constexpr char TEXT_SWI[]   = "SWI";
// constexpr char TEXT_TSET[]  = "TSET";
// constexpr char TEXT_UNLK[]  = "UNLK";
constexpr char TEXT_XORCF[] = "XORCF";
// constexpr char TEXT_XOR[]   = "XOR";
// constexpr char TEXT_XORW[] = "XORW";
constexpr char TEXT_ZCF[]   = "ZCF";

// constexpr char TEXT_REG_W[]   = "W";
// constexpr char TEXT_REG_A[]   = "A";
// constexpr char TEXT_REG_B[]   = "B";
// constexpr char TEXT_REG_C[]   = "C";
// constexpr char TEXT_REG_D[]   = "D";
// constexpr char TEXT_REG_E[]   = "E";
// constexpr char TEXT_REG_H[]   = "H";
// constexpr char TEXT_REG_L[]   = "L";
constexpr char TEXT_REG_RW[]  = "RW";
constexpr char TEXT_REG_RA[]  = "RA";
constexpr char TEXT_REG_RB[]  = "RB";
// constexpr char TEXT_REG_RC[]  = "RC";
constexpr char TEXT_REG_RD[]  = "RD";
// constexpr char TEXT_REG_RE[]  = "RE";
// constexpr char TEXT_REG_RH[]  = "RH";
// constexpr char TEXT_REG_RL[]  = "RL";
constexpr char TEXT_REG_QW[]  = "QW";
constexpr char TEXT_REG_QA[]  = "QA";
constexpr char TEXT_REG_QB[]  = "QB";
constexpr char TEXT_REG_QC[]  = "QC";
constexpr char TEXT_REG_QD[]  = "QD";
constexpr char TEXT_REG_QE[]  = "QE";
constexpr char TEXT_REG_QF[]  = "QF";
constexpr char TEXT_REG_QH[]  = "QH";
// constexpr char TEXT_REG_QL[]  = "QL";
// constexpr char TEXT_REG_IXH[] = "IXH";
// constexpr char TEXT_REG_IXL[] = "IXL";
// constexpr char TEXT_REG_IYH[] = "IYH";
// constexpr char TEXT_REG_IYL[] = "IYL";
constexpr char TEXT_REG_IZH[] = "IZH";
constexpr char TEXT_REG_IZL[] = "IZL";
// constexpr char TEXT_REG_SPH[] = "SPH";
// constexpr char TEXT_REG_SPL[] = "SPL";
constexpr char TEXT_REG_QIXH[] = "QIXH";
constexpr char TEXT_REG_QIXL[] = "QIXL";
constexpr char TEXT_REG_QIYH[] = "QIYH";
constexpr char TEXT_REG_QIYL[] = "QIYL";
constexpr char TEXT_REG_QIZH[] = "QIZH";
constexpr char TEXT_REG_QIZL[] = "QIZL";
constexpr char TEXT_REG_QSPH[] = "QSPH";
constexpr char TEXT_REG_QSPL[] = "QSPL";
constexpr char TEXT_REG_WA[]  = "WA";
// constexpr char TEXT_REG_BC[]  = "BC";
// constexpr char TEXT_REG_DE[]  = "DE";
// constexpr char TEXT_REG_HL[]  = "HL";
// constexpr char TEXT_REG_IX[]  = "IX";
// constexpr char TEXT_REG_IY[]  = "IY";
constexpr char TEXT_REG_IZ[]  = "IZ";
// constexpr char TEXT_REG_SP[]  = "SP";
constexpr char TEXT_REG_QWA[] = "QWA";
constexpr char TEXT_REG_QBC[] = "QBC";
constexpr char TEXT_REG_QDE[] = "QDE";
constexpr char TEXT_REG_QHL[] = "QHL";
constexpr char TEXT_REG_QIX[] = "QIX";
constexpr char TEXT_REG_QIY[] = "QIY";
constexpr char TEXT_REG_QIZ[] = "QIZ";
constexpr char TEXT_REG_QSP[] = "QSP";
constexpr char TEXT_REG_RWA[] = "RWA";
constexpr char TEXT_REG_RBC[] = "RBC";
constexpr char TEXT_REG_RDE[] = "RDE";
constexpr char TEXT_REG_RHL[] = "RHL";
constexpr char TEXT_REG_XWA[] = "XWA";
constexpr char TEXT_REG_XBC[] = "XBC";
constexpr char TEXT_REG_XDE[] = "XDE";
constexpr char TEXT_REG_XHL[] = "XHL";
constexpr char TEXT_REG_XIX[] = "XIX";
constexpr char TEXT_REG_XIY[] = "XIY";
constexpr char TEXT_REG_XIZ[] = "XIZ";
constexpr char TEXT_REG_XSP[] = "XSP";
// constexpr char TEXT_REG_F[]   = "F";
// constexpr char TEXT_REG_SR[]  = "SR";
// constexpr char TEXT_REG_PC[]  = "PC";
// constexpr char TEXT_REG_NSP[] = "NSP";
constexpr char TEXT_REG_XNSP[] = "XNSP";
constexpr char TEXT_REG_INTNEST[] = "INTNEST";

// constexpr char TEXT_CC_C[]   = "C";
// constexpr char TEXT_CC_EQ[]  = "EQ";
// constexpr char TEXT_CC_F[]   = "F";
// constexpr char TEXT_CC_GE[]  = "GE";
// constexpr char TEXT_CC_GT[]  = "GT";
// constexpr char TEXT_CC_LE[]  = "LE";
// constexpr char TEXT_CC_LT[]  = "LT";
// constexpr char TEXT_CC_MI[]  = "MI";
// constexpr char TEXT_CC_M[]   = "M";
// constexpr char TEXT_CC_NC[]  = "NC";
// constexpr char TEXT_CC_NE[]  = "NE";
// constexpr char TEXT_CC_NOV[] = "NOV";
// constexpr char TEXT_CC_NZ[]  = "NZ";
// constexpr char TEXT_CC_OV[]  = "OV";
// constexpr char TEXT_CC_PE[]  = "PE";
// constexpr char TEXT_CC_PL[]  = "PL";
// constexpr char TEXT_CC_PO[]  = "PO";
// constexpr char TEXT_CC_P[]   = "P";
// constexpr char TEXT_CC_UGE[] = "UGE";
// constexpr char TEXT_CC_UGT[] = "UGT";
// constexpr char TEXT_CC_ULE[] = "ULE";
// constexpr char TEXT_CC_ULT[] = "ULT";
// constexpr char TEXT_CC_T[]   = "T";
// constexpr char TEXT_CC_Z[]   = "Z";
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
