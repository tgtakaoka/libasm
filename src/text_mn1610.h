/*
 * Copyright 2021 Tadashi G. Takaoka
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

#ifndef __LIBASM_TEXT_MN1610__
#define __LIBASM_TEXT_MN1610__

#include "text_common.h"

namespace libasm {
namespace text {
namespace mn1610 {
// clang-format off

extern const char TEXT_MN1610_LIST[] PROGMEM;
extern const char TEXT_CPU_MN1610[]  PROGMEM;
extern const char TEXT_CPU_MN1613[]  PROGMEM;
extern const char TEXT_CPU_MN1613A[] PROGMEM;

// MN1610
using common::TEXT_A;
using common::TEXT_AI;
using common::TEXT_AND;
using common::TEXT_B;
extern const char TEXT_BAL[]  PROGMEM;
extern const char TEXT_BSWP[] PROGMEM;
using common::TEXT_C;
using common::TEXT_CB;
using common::TEXT_CLR;
extern const char TEXT_DMS[]  PROGMEM;
extern const char TEXT_DSWP[] PROGMEM;
using common::TEXT_EOR;
extern const char TEXT_H[]    PROGMEM;
extern const char TEXT_IMS[]  PROGMEM;
extern const char TEXT_L[]    PROGMEM;
extern const char TEXT_LAD[]  PROGMEM;
extern const char TEXT_LPSW[] PROGMEM;
extern const char TEXT_MV[]   PROGMEM;
extern const char TEXT_MVB[]  PROGMEM;
using common::TEXT_MVI;
using common::TEXT_OR;
using common::TEXT_POP;
using common::TEXT_PUSH;
extern const char TEXT_RBIT[] PROGMEM;
extern const char TEXT_RD[]   PROGMEM;
using common::TEXT_RET;
using common::TEXT_S;
extern const char TEXT_SBIT[] PROGMEM;
extern const char TEXT_SI[]   PROGMEM;
using common::TEXT_SL;
using common::TEXT_SR;
using common::TEXT_ST;
extern const char TEXT_TBIT[] PROGMEM;
using common::TEXT_TST;
extern const char TEXT_WT[]   PROGMEM;

// MN1613
extern const char TEXT_AD[]   PROGMEM;
using common::TEXT_ANDI;
using common::TEXT_ANDR;
extern const char TEXT_AWI[]  PROGMEM;
extern const char TEXT_AWR[]  PROGMEM;
extern const char TEXT_BALD[] PROGMEM;
extern const char TEXT_BALL[] PROGMEM;
extern const char TEXT_BALR[] PROGMEM;
using common::TEXT_BD;
using common::TEXT_BL;
extern const char TEXT_BLK[]  PROGMEM;
using common::TEXT_BR;
extern const char TEXT_BSWR[] PROGMEM;
extern const char TEXT_CBI[]  PROGMEM;
extern const char TEXT_CBR[]  PROGMEM;
extern const char TEXT_CPYB[] PROGMEM;
extern const char TEXT_CPYH[] PROGMEM;
extern const char TEXT_CPYS[] PROGMEM;
extern const char TEXT_CWI[]  PROGMEM;
extern const char TEXT_CWR[]  PROGMEM;
extern const char TEXT_D[]    PROGMEM;
using common::TEXT_DAA;
using common::TEXT_DAS;
extern const char TEXT_DEBP[] PROGMEM;
extern const char TEXT_DSWR[] PROGMEM;
using common::TEXT_EORI;
using common::TEXT_EORR;
extern const char TEXT_FA[]   PROGMEM;
extern const char TEXT_FD[]   PROGMEM;
using common::TEXT_FIX;
using common::TEXT_FLT;
extern const char TEXT_FM[]   PROGMEM;
extern const char TEXT_FS[]   PROGMEM;
extern const char TEXT_LADI[] PROGMEM;
extern const char TEXT_LADR[] PROGMEM;
extern const char TEXT_LB[]   PROGMEM;
using common::TEXT_LD;
using common::TEXT_LR;
extern const char TEXT_LS[]   PROGMEM;
extern const char TEXT_M[]    PROGMEM;
extern const char TEXT_MVBR[] PROGMEM;
extern const char TEXT_MVWI[] PROGMEM;
extern const char TEXT_MVWR[] PROGMEM;
using common::TEXT_NEG;
using common::TEXT_ORI;
using common::TEXT_ORR;
extern const char TEXT_POPM[] PROGMEM;
using common::TEXT_PSHM;
extern const char TEXT_RDR[]  PROGMEM;
extern const char TEXT_RETL[] PROGMEM;
using common::TEXT_SD;
using common::TEXT_SETB;
extern const char TEXT_SETH[] PROGMEM;
using common::TEXT_SETS;
extern const char TEXT_SRBT[] PROGMEM;
using common::TEXT_STB;
using common::TEXT_STD;
using common::TEXT_STR;
using common::TEXT_STS;
using common::TEXT_SWI;
extern const char TEXT_SWR[]  PROGMEM;
extern const char TEXT_TRST[] PROGMEM;
using common::TEXT_TSET;
extern const char TEXT_WTR[]  PROGMEM;

extern const char TEXT_REG_CSBR[] PROGMEM;
extern const char TEXT_REG_DR0[]  PROGMEM;
extern const char TEXT_REG_IC[]   PROGMEM;
extern const char TEXT_REG_ICB[]  PROGMEM;
extern const char TEXT_REG_IISR[] PROGMEM;
extern const char TEXT_REG_NPP[]  PROGMEM;
extern const char TEXT_REG_OSR0[] PROGMEM;
extern const char TEXT_REG_OSR1[] PROGMEM;
extern const char TEXT_REG_OSR2[] PROGMEM;
extern const char TEXT_REG_OSR3[] PROGMEM;
using common::TEXT_REG_R0;
using common::TEXT_REG_R1;
using common::TEXT_REG_R2;
using common::TEXT_REG_R3;
using common::TEXT_REG_R4;
extern const char TEXT_REG_SBRB[] PROGMEM;
extern const char TEXT_REG_SCR[]  PROGMEM;
extern const char TEXT_REG_SIR[]  PROGMEM;
extern const char TEXT_REG_SOR[]  PROGMEM;
using common::TEXT_REG_SP;
extern const char TEXT_REG_SSBR[] PROGMEM;
extern const char TEXT_REG_SSR[]  PROGMEM;
extern const char TEXT_REG_STR[]  PROGMEM;
extern const char TEXT_REG_TCR[]  PROGMEM;
extern const char TEXT_REG_TIR[]  PROGMEM;
extern const char TEXT_REG_TSR[]  PROGMEM;
extern const char TEXT_REG_TSR0[] PROGMEM;
extern const char TEXT_REG_TSR1[] PROGMEM;
extern const char TEXT_REG_X0[]   PROGMEM;
extern const char TEXT_REG_X1[]   PROGMEM;

using common::TEXT_CC_C;
extern const char TEXT_CC_CE[]   PROGMEM;
extern const char TEXT_CC_E[]    PROGMEM;
extern const char TEXT_CC_ENZ[]  PROGMEM;
extern const char TEXT_CC_EZ[]   PROGMEM;
extern const char TEXT_CC_LM[]   PROGMEM;
extern const char TEXT_CC_LMZ[]  PROGMEM;
extern const char TEXT_CC_LP[]   PROGMEM;
extern const char TEXT_CC_LPZ[]  PROGMEM;
using common::TEXT_CC_M;
extern const char TEXT_CC_MZ[]   PROGMEM;
using common::TEXT_CC_NE;
extern const char TEXT_CC_NONE[] PROGMEM;
using common::TEXT_CC_NZ;
extern const char TEXT_CC_ONZ[]  PROGMEM;
extern const char TEXT_CC_OZ[]   PROGMEM;
using common::TEXT_CC_P;
extern const char TEXT_CC_PZ[]   PROGMEM;
extern const char TEXT_CC_RE[]   PROGMEM;
extern const char TEXT_CC_SE[]   PROGMEM;
extern const char TEXT_CC_SKP[]  PROGMEM;
using common::TEXT_CC_Z;

// clang-format on
}  // namespace mn1610
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_MN1610__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
