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

extern const char TEXT_MN1610_LIST[];
extern const char TEXT_CPU_MN1610[];
extern const char TEXT_CPU_MN1613[];
extern const char TEXT_CPU_MN1613A[];

// MN1610
using common::TEXT_A;
using common::TEXT_AI;
using common::TEXT_AND;
using common::TEXT_B;
extern const char TEXT_BAL[];
extern const char TEXT_BSWP[];
using common::TEXT_C;
using common::TEXT_CB;
using common::TEXT_CLR;
extern const char TEXT_DMS[];
extern const char TEXT_DSWP[];
using common::TEXT_EOR;
extern const char TEXT_H[];
extern const char TEXT_IMS[];
extern const char TEXT_L[];
extern const char TEXT_LAD[];
extern const char TEXT_LPSW[];
extern const char TEXT_MV[];
extern const char TEXT_MVB[];
using common::TEXT_MVI;
using common::TEXT_OR;
using common::TEXT_POP;
using common::TEXT_PUSH;
extern const char TEXT_RBIT[];
extern const char TEXT_RD[];
using common::TEXT_RET;
using common::TEXT_S;
extern const char TEXT_SBIT[];
extern const char TEXT_SI[];
using common::TEXT_SL;
using common::TEXT_SR;
using common::TEXT_ST;
extern const char TEXT_TBIT[];
using common::TEXT_TST;
extern const char TEXT_WT[];

// MN1613
extern const char TEXT_AD[];
using common::TEXT_ANDI;
using common::TEXT_ANDR;
extern const char TEXT_AWI[];
extern const char TEXT_AWR[];
extern const char TEXT_BALD[];
extern const char TEXT_BALL[];
extern const char TEXT_BALR[];
using common::TEXT_BD;
using common::TEXT_BL;
extern const char TEXT_BLK[];
using common::TEXT_BR;
extern const char TEXT_BSWR[];
extern const char TEXT_CBI[];
extern const char TEXT_CBR[];
extern const char TEXT_CPYB[];
extern const char TEXT_CPYH[];
extern const char TEXT_CPYS[];
extern const char TEXT_CWI[];
extern const char TEXT_CWR[];
extern const char TEXT_D[];
using common::TEXT_DAA;
using common::TEXT_DAS;
extern const char TEXT_DEBP[];
extern const char TEXT_DSWR[];
using common::TEXT_EORI;
using common::TEXT_EORR;
extern const char TEXT_FA[];
extern const char TEXT_FD[];
using common::TEXT_FIX;
using common::TEXT_FLT;
extern const char TEXT_FM[];
extern const char TEXT_FS[];
extern const char TEXT_LADI[];
extern const char TEXT_LADR[];
extern const char TEXT_LB[];
using common::TEXT_LD;
using common::TEXT_LR;
extern const char TEXT_LS[];
extern const char TEXT_M[];
extern const char TEXT_MVBR[];
extern const char TEXT_MVWI[];
extern const char TEXT_MVWR[];
using common::TEXT_NEG;
using common::TEXT_ORI;
using common::TEXT_ORR;
extern const char TEXT_POPM[];
using common::TEXT_PSHM;
extern const char TEXT_RDR[];
extern const char TEXT_RETL[];
using common::TEXT_SD;
using common::TEXT_SETB;
extern const char TEXT_SETH[];
using common::TEXT_SETS;
extern const char TEXT_SRBT[];
using common::TEXT_STB;
using common::TEXT_STD;
using common::TEXT_STR;
using common::TEXT_STS;
using common::TEXT_SWI;
extern const char TEXT_SWR[];
extern const char TEXT_TRST[];
using common::TEXT_TSET;
extern const char TEXT_WTR[];

extern const char TEXT_REG_CSBR[];
using common::TEXT_REG_DR0;
extern const char TEXT_REG_IC[];
extern const char TEXT_REG_ICB[];
extern const char TEXT_REG_IISR[];
extern const char TEXT_REG_NPP[];
extern const char TEXT_REG_OSR0[];
extern const char TEXT_REG_OSR1[];
extern const char TEXT_REG_OSR2[];
extern const char TEXT_REG_OSR3[];
using common::TEXT_REG_R0;
using common::TEXT_REG_R1;
using common::TEXT_REG_R2;
using common::TEXT_REG_R3;
using common::TEXT_REG_R4;
extern const char TEXT_REG_SBRB[];
extern const char TEXT_REG_SCR[];
extern const char TEXT_REG_SIR[];
extern const char TEXT_REG_SOR[];
using common::TEXT_REG_SP;
extern const char TEXT_REG_SSBR[];
extern const char TEXT_REG_SSR[];
extern const char TEXT_REG_STR[];
extern const char TEXT_REG_TCR[];
extern const char TEXT_REG_TIR[];
extern const char TEXT_REG_TSR[];
extern const char TEXT_REG_TSR0[];
extern const char TEXT_REG_TSR1[];
using common::TEXT_REG_X0;
using common::TEXT_REG_X1;

using common::TEXT_CC_C;
extern const char TEXT_CC_CE[];
extern const char TEXT_CC_E[];
extern const char TEXT_CC_ENZ[];
extern const char TEXT_CC_EZ[];
extern const char TEXT_CC_LM[];
extern const char TEXT_CC_LMZ[];
extern const char TEXT_CC_LP[];
extern const char TEXT_CC_LPZ[];
using common::TEXT_CC_M;
extern const char TEXT_CC_MZ[];
using common::TEXT_CC_NE;
extern const char TEXT_CC_NONE[];
using common::TEXT_CC_NZ;
extern const char TEXT_CC_ONZ[];
extern const char TEXT_CC_OZ[];
using common::TEXT_CC_P;
extern const char TEXT_CC_PZ[];
extern const char TEXT_CC_RE[];
extern const char TEXT_CC_SE[];
extern const char TEXT_CC_SKP[];
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
