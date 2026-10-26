/*
 * Copyright 2026 Tadashi G. Takaoka
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

#ifndef __LIBASM_TEXT_TLCS900__
#define __LIBASM_TEXT_TLCS900__

#include "text_common.h"

namespace libasm {
namespace text {
namespace tlcs900 {

extern const char TEXT_TLCS900_LIST[];
extern const char TEXT_CPU_TLCS900[];    // +NORMAL, +MAX, +LDX
extern const char TEXT_CPU_TLCS900L[];   // -NORMAL, -MAX, +MIN, +LDX
extern const char TEXT_CPU_TLCS900H[];   // -NORMAL, -MAX, +LDX
extern const char TEXT_CPU_TLCS900H2[];  // -NORMAL, -MAX, -LDX

extern const char OPT_BOOL_MAXIMUM_MODE[];
extern const char OPT_DESC_MAXIMUM_MODE[];

// clang-format off
using common::TEXT_ADC;
extern const char TEXT_ADCW[];
using common::TEXT_ADD;
using common::TEXT_ADDW;
using common::TEXT_AND;
using common::TEXT_ANDW;
extern const char TEXT_ANDCF[];
using common::TEXT_BIT;
extern const char TEXT_BS1B[];
extern const char TEXT_BS1F[];
using common::TEXT_CALL;
using common::TEXT_CALR;
using common::TEXT_CCF;
extern const char TEXT_CHG[];
using common::TEXT_CP;
using common::TEXT_CPW;
using common::TEXT_CPD;
using common::TEXT_CPDR;
using common::TEXT_CPI;
using common::TEXT_CPIR;
using common::TEXT_CPL;
using common::TEXT_DAA;
using common::TEXT_DEC;
using common::TEXT_DECW;
using common::TEXT_DECF;
using common::TEXT_DI;
using common::TEXT_DIV;
using common::TEXT_DIVS;
using common::TEXT_DJNZ;
using common::TEXT_EI;
using common::TEXT_EX;
using common::TEXT_EXTS;
extern const char TEXT_EXTZ[];
using common::TEXT_HALT;
using common::TEXT_INC;
using common::TEXT_INCW;
using common::TEXT_INCF;
using common::TEXT_JP;
using common::TEXT_JR;
using common::TEXT_JRL;
using common::TEXT_LD;
using common::TEXT_LDA;
using common::TEXT_LDAR;
using common::TEXT_LDC;
extern const char TEXT_LDCF[];
using common::TEXT_LDD;
extern const char TEXT_LDDW[];
using common::TEXT_LDDR;
extern const char TEXT_LDDRW[];
using common::TEXT_LDF;
using common::TEXT_LDI;
extern const char TEXT_LDIW[];
using common::TEXT_LDIR;
extern const char TEXT_LDIRW[];
using common::TEXT_LDW;
using common::TEXT_LDX;
using common::TEXT_LINK;
extern const char TEXT_MAX[];
using common::TEXT_MAXMODE;
extern const char TEXT_MDEC1[];
extern const char TEXT_MDEC2[];
extern const char TEXT_MDEC4[];
extern const char TEXT_MIN[];
extern const char TEXT_MINC1[];
extern const char TEXT_MINC2[];
extern const char TEXT_MINC4[];
extern const char TEXT_MIRR[];
using common::TEXT_MUL;
extern const char TEXT_MULA[];
using common::TEXT_MULS;
using common::TEXT_NEG;
using common::TEXT_NOP;
extern const char TEXT_NORMAL[];
using common::TEXT_OR;
using common::TEXT_ORW;
extern const char TEXT_ORCF[];
extern const char TEXT_PAA[];
using common::TEXT_POP;
extern const char TEXT_POPW[];
using common::TEXT_PUSH;
extern const char TEXT_PUSHW[];
using common::TEXT_RCF;
using common::TEXT_RES;
using common::TEXT_RET;
using common::TEXT_RETD;
using common::TEXT_RETI;
using common::TEXT_RL;
extern const char TEXT_RLW[];
using common::TEXT_RLC;
extern const char TEXT_RLCW[];
using common::TEXT_RLD;
using common::TEXT_RR;
extern const char TEXT_RRW[];
using common::TEXT_RRC;
extern const char TEXT_RRCW[];
using common::TEXT_RRD;
using common::TEXT_SBC;
extern const char TEXT_SBCW[];
using common::TEXT_SCC;
using common::TEXT_SCF;
using common::TEXT_SET;
using common::TEXT_SLA;
extern const char TEXT_SLAW[];
using common::TEXT_SLL;
extern const char TEXT_SLLW[];
using common::TEXT_SRA;
extern const char TEXT_SRAW[];
using common::TEXT_SRL;
extern const char TEXT_SRLW[];
extern const char TEXT_STCF[];
using common::TEXT_SUB;
using common::TEXT_SUBW;
using common::TEXT_SWI;
using common::TEXT_TSET;
using common::TEXT_UNLK;
using common::TEXT_XOR;
using common::TEXT_XORW;
extern const char TEXT_XORCF[];
extern const char TEXT_ZCF[];

using common::TEXT_REG_W;
using common::TEXT_REG_A;
using common::TEXT_REG_B;
using common::TEXT_REG_C;
using common::TEXT_REG_D;
using common::TEXT_REG_E;
using common::TEXT_REG_H;
using common::TEXT_REG_L;
extern const char TEXT_REG_RW[];
extern const char TEXT_REG_RA[];
extern const char TEXT_REG_RB[];
using common::TEXT_REG_RC;
extern const char TEXT_REG_RD[];
using common::TEXT_REG_RE;
using common::TEXT_REG_RH;
using common::TEXT_REG_RL;
extern const char TEXT_REG_QW[];
extern const char TEXT_REG_QA[];
extern const char TEXT_REG_QB[];
extern const char TEXT_REG_QC[];
extern const char TEXT_REG_QD[];
extern const char TEXT_REG_QE[];
extern const char TEXT_REG_QF[];
extern const char TEXT_REG_QH[];
using common::TEXT_REG_QL;
using common::TEXT_REG_IXH;
using common::TEXT_REG_IXL;
using common::TEXT_REG_IYH;
using common::TEXT_REG_IYL;
extern const char TEXT_REG_IZH[];
extern const char TEXT_REG_IZL[];
using common::TEXT_REG_SPH;
using common::TEXT_REG_SPL;
extern const char TEXT_REG_QIXH[];
extern const char TEXT_REG_QIXL[];
extern const char TEXT_REG_QIYH[];
extern const char TEXT_REG_QIYL[];
extern const char TEXT_REG_QIZH[];
extern const char TEXT_REG_QIZL[];
extern const char TEXT_REG_QSPH[];
extern const char TEXT_REG_QSPL[];
extern const char TEXT_REG_WA[];
using common::TEXT_REG_BC;
using common::TEXT_REG_DE;
using common::TEXT_REG_HL;
using common::TEXT_REG_IX;
using common::TEXT_REG_IY;
extern const char TEXT_REG_IZ[];
using common::TEXT_REG_SP;
extern const char TEXT_REG_QWA[];
extern const char TEXT_REG_QBC[];
extern const char TEXT_REG_QDE[];
extern const char TEXT_REG_QHL[];
extern const char TEXT_REG_QIX[];
extern const char TEXT_REG_QIY[];
extern const char TEXT_REG_QIZ[];
extern const char TEXT_REG_QSP[];
extern const char TEXT_REG_RWA[];
extern const char TEXT_REG_RBC[];
extern const char TEXT_REG_RDE[];
extern const char TEXT_REG_RHL[];
extern const char TEXT_REG_XWA[];
extern const char TEXT_REG_XBC[];
extern const char TEXT_REG_XDE[];
extern const char TEXT_REG_XHL[];
extern const char TEXT_REG_XIX[];
extern const char TEXT_REG_XIY[];
extern const char TEXT_REG_XIZ[];
extern const char TEXT_REG_XSP[];
using common::TEXT_REG_F;
using common::TEXT_REG_SR;
using common::TEXT_REG_PC;
using common::TEXT_REG_NSP;
extern const char TEXT_REG_XNSP[];
extern const char TEXT_REG_INTNEST[];

using common::TEXT_CC_C;
using common::TEXT_CC_EQ;
using common::TEXT_CC_F;
using common::TEXT_CC_GE;
using common::TEXT_CC_GT;
using common::TEXT_CC_LE;
using common::TEXT_CC_LT;
using common::TEXT_CC_MI;
using common::TEXT_CC_M;
using common::TEXT_CC_NC;
using common::TEXT_CC_NE;
using common::TEXT_CC_NOV;
using common::TEXT_CC_NZ;
using common::TEXT_CC_OV;
using common::TEXT_CC_PE;
using common::TEXT_CC_PL;
using common::TEXT_CC_PO;
using common::TEXT_CC_P;
using common::TEXT_CC_T;
using common::TEXT_CC_UGE;
using common::TEXT_CC_UGT;
using common::TEXT_CC_ULE;
using common::TEXT_CC_ULT;
using common::TEXT_CC_Z;
// clang-format on

}  // namespace tlcs900
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_TLCS900__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
