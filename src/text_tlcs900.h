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

extern const char TEXT_TLCS900_LIST[] PROGMEM;
extern const char TEXT_CPU_TLCS900[]  PROGMEM; // +NORMAL, +MAX
extern const char TEXT_CPU_TLCS900H[] PROGMEM; // -NORMAL, -MAX
extern const char TEXT_CPU_TLCS900L[] PROGMEM; // -NORMAL, -MAX, +MIN
extern const char TEXT_CPU_TLCS900H2[] PROGMEM; // -NORMAL, -MAX, -LDX

// clang-format off
using common::TEXT_ADC;
using common::TEXT_ADD;
using common::TEXT_AND;
extern const char TEXT_ANDCF[]  PROGMEM;
using common::TEXT_BIT;
extern const char TEXT_BS1B[]   PROGMEM;
extern const char TEXT_BS1F[]   PROGMEM;
using common::TEXT_CALL;
using common::TEXT_CALR;
using common::TEXT_CCF;
extern const char TEXT_CHG[]    PROGMEM;
using common::TEXT_CP;
using common::TEXT_CPD;
using common::TEXT_CPDR;
using common::TEXT_CPI;
using common::TEXT_CPIR;
using common::TEXT_CPL;
using common::TEXT_DAA;
using common::TEXT_DEC;
using common::TEXT_DECF;
using common::TEXT_DI;
using common::TEXT_DIV;
using common::TEXT_DIVS;
using common::TEXT_DJNZ;
using common::TEXT_EI;
using common::TEXT_EX;
using common::TEXT_EXTS;
extern const char TEXT_EXTZ[]   PROGMEM;
using common::TEXT_HALT;
using common::TEXT_INC;
using common::TEXT_INCF;
using common::TEXT_JP;
using common::TEXT_JR;
using common::TEXT_JRL;
using common::TEXT_LD;
using common::TEXT_LDA;
using common::TEXT_LDAR;
using common::TEXT_LDC;
extern const char TEXT_LDCF[]   PROGMEM;
using common::TEXT_LDD;
using common::TEXT_LDDR;
using common::TEXT_LDF;
using common::TEXT_LDI;
using common::TEXT_LDIR;
using common::TEXT_LDX;
using common::TEXT_LINK;
extern const char TEXT_MAX[]    PROGMEM;
extern const char TEXT_MDEC1[]  PROGMEM;
extern const char TEXT_MDEC2[]  PROGMEM;
extern const char TEXT_MDEC4[]  PROGMEM;
extern const char TEXT_MIN[]    PROGMEM;
extern const char TEXT_MINC1[]  PROGMEM;
extern const char TEXT_MINC2[]  PROGMEM;
extern const char TEXT_MINC4[]  PROGMEM;
extern const char TEXT_MIRR[]   PROGMEM;
using common::TEXT_MUL;
extern const char TEXT_MULA[]   PROGMEM;
using common::TEXT_MULS;
using common::TEXT_NEG;
using common::TEXT_NOP;
extern const char TEXT_NORMAL[] PROGMEM;
using common::TEXT_OR;
extern const char TEXT_ORCF[]   PROGMEM;
extern const char TEXT_PAA[]    PROGMEM;
using common::TEXT_POP;
using common::TEXT_PUSH;
using common::TEXT_RCF;
using common::TEXT_RES;
using common::TEXT_RET;
using common::TEXT_RETD;
using common::TEXT_RETI;
using common::TEXT_RL;
using common::TEXT_RLC;
using common::TEXT_RLD;
using common::TEXT_RR;
using common::TEXT_RRC;
using common::TEXT_RRD;
using common::TEXT_SBC;
using common::TEXT_SCC;
using common::TEXT_SCF;
using common::TEXT_SET;
using common::TEXT_SLA;
using common::TEXT_SLL;
using common::TEXT_SRA;
using common::TEXT_SRL;
extern const char TEXT_STCF[]   PROGMEM;
using common::TEXT_SUB;
using common::TEXT_SWI;
using common::TEXT_TSET;
using common::TEXT_UNLK;
using common::TEXT_XOR;
extern const char TEXT_XORCF[]  PROGMEM;
extern const char TEXT_ZCF[]    PROGMEM;

using common::TEXT_REG_W;
using common::TEXT_REG_A;
using common::TEXT_REG_B;
using common::TEXT_REG_C;
using common::TEXT_REG_D;
using common::TEXT_REG_E;
using common::TEXT_REG_H;
using common::TEXT_REG_L;
extern const char TEXT_REG_RW[]  PROGMEM;
extern const char TEXT_REG_RA[]  PROGMEM;
extern const char TEXT_REG_RB[]  PROGMEM;
using common::TEXT_REG_RC;
extern const char TEXT_REG_RD[]  PROGMEM;
using common::TEXT_REG_RE;
using common::TEXT_REG_RH;
using common::TEXT_REG_RL;
extern const char TEXT_REG_QW[]  PROGMEM;
extern const char TEXT_REG_QA[]  PROGMEM;
extern const char TEXT_REG_QB[]  PROGMEM;
extern const char TEXT_REG_QC[]  PROGMEM;
extern const char TEXT_REG_QD[]  PROGMEM;
extern const char TEXT_REG_QE[]  PROGMEM;
extern const char TEXT_REG_QF[]  PROGMEM;
extern const char TEXT_REG_QH[]  PROGMEM;
using common::TEXT_REG_QL;
using common::TEXT_REG_IXH;
using common::TEXT_REG_IXL;
using common::TEXT_REG_IYH;
using common::TEXT_REG_IYL;
extern const char TEXT_REG_IZH[] PROGMEM;
extern const char TEXT_REG_IZL[] PROGMEM;
extern const char TEXT_REG_SPH[] PROGMEM;
extern const char TEXT_REG_SPL[] PROGMEM;
extern const char TEXT_REG_QIXH[] PROGMEM;
extern const char TEXT_REG_QIXL[] PROGMEM;
extern const char TEXT_REG_QIYH[] PROGMEM;
extern const char TEXT_REG_QIYL[] PROGMEM;
extern const char TEXT_REG_QIZH[] PROGMEM;
extern const char TEXT_REG_QIZL[] PROGMEM;
extern const char TEXT_REG_QSPH[] PROGMEM;
extern const char TEXT_REG_QSPL[] PROGMEM;
extern const char TEXT_REG_WA[]  PROGMEM;
using common::TEXT_REG_BC;
using common::TEXT_REG_DE;
using common::TEXT_REG_HL;
using common::TEXT_REG_IX;
using common::TEXT_REG_IY;
extern const char TEXT_REG_IZ[]  PROGMEM;
using common::TEXT_REG_SP;
extern const char TEXT_REG_QWA[] PROGMEM;
extern const char TEXT_REG_QBC[] PROGMEM;
extern const char TEXT_REG_QDE[] PROGMEM;
extern const char TEXT_REG_QHL[] PROGMEM;
extern const char TEXT_REG_QIX[] PROGMEM;
extern const char TEXT_REG_QIY[] PROGMEM;
extern const char TEXT_REG_QIZ[] PROGMEM;
extern const char TEXT_REG_QSP[] PROGMEM;
extern const char TEXT_REG_RWA[] PROGMEM;
extern const char TEXT_REG_RBC[] PROGMEM;
extern const char TEXT_REG_RDE[] PROGMEM;
extern const char TEXT_REG_RHL[] PROGMEM;
extern const char TEXT_REG_XWA[] PROGMEM;
extern const char TEXT_REG_XBC[] PROGMEM;
extern const char TEXT_REG_XDE[] PROGMEM;
extern const char TEXT_REG_XHL[] PROGMEM;
extern const char TEXT_REG_XIX[] PROGMEM;
extern const char TEXT_REG_XIY[] PROGMEM;
extern const char TEXT_REG_XIZ[] PROGMEM;
extern const char TEXT_REG_XSP[] PROGMEM;
using common::TEXT_REG_F;
using common::TEXT_REG_SR;
using common::TEXT_REG_PC;
extern const char TEXT_REG_NXSP[] PROGMEM;

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
