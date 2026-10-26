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

#ifndef __LIBASM_TEXT_TMS9900__
#define __LIBASM_TEXT_TMS9900__

#include "text_common.h"

namespace libasm {
namespace text {
namespace tms9900 {
// clang-format off

extern const char TEXT_TMS9900_LIST[];
extern const char TEXT_CPU_9900[];
extern const char TEXT_CPU_99105[];
extern const char TEXT_CPU_99110[];
extern const char TEXT_CPU_9980[];
extern const char TEXT_CPU_9995[];

// TMS9900
using common::TEXT_A;
extern const char TEXT_AB[];
using common::TEXT_ABS;
using common::TEXT_AI;
using common::TEXT_ANDI;
using common::TEXT_B;
using common::TEXT_BL;
extern const char TEXT_BLWP[];
using common::TEXT_C;
using common::TEXT_CB;
using common::TEXT_CI;
extern const char TEXT_CKOF[];
extern const char TEXT_CKON[];
using common::TEXT_CLR;
extern const char TEXT_COC[];
extern const char TEXT_CZC[];
using common::TEXT_DEC;
extern const char TEXT_DECT[];
using common::TEXT_DIV;
using common::TEXT_IDLE;
using common::TEXT_INC;
extern const char TEXT_INCT[];
using common::TEXT_INV;
using common::TEXT_JEQ;
using common::TEXT_JGT;
using common::TEXT_JH;
extern const char TEXT_JHE[];
using common::TEXT_JL;
using common::TEXT_JLE;
using common::TEXT_JLT;
using common::TEXT_JMP;
using common::TEXT_JNC;
using common::TEXT_JNE;
using common::TEXT_JNO;
extern const char TEXT_JOC[];
extern const char TEXT_JOP[];
extern const char TEXT_LDCR[];
using common::TEXT_LI;
extern const char TEXT_LIMI[];
extern const char TEXT_LREX[];
extern const char TEXT_LWPI[];
using common::TEXT_MOV;
using common::TEXT_MOVB;
using common::TEXT_MPY;
using common::TEXT_NEG;
using common::TEXT_NOP;
using common::TEXT_ORI;
extern const char TEXT_RSET[];
extern const char TEXT_RT[];
extern const char TEXT_RTWP[];
using common::TEXT_S;
extern const char TEXT_SB[];
extern const char TEXT_SBO[];
extern const char TEXT_SBZ[];
using common::TEXT_SETO;
using common::TEXT_SLA;
extern const char TEXT_SOC[];
extern const char TEXT_SOCB[];
using common::TEXT_SRA;
extern const char TEXT_SRC[];
using common::TEXT_SRL;
extern const char TEXT_STCR[];
using common::TEXT_STST;
extern const char TEXT_STWP[];
extern const char TEXT_SWPB[];
extern const char TEXT_SZC[];
extern const char TEXT_SZCB[];
extern const char TEXT_TB[];
extern const char TEXT_X[];
extern const char TEXT_XOP[];
using common::TEXT_XOR;

// TMS9995
using common::TEXT_DIVS;
using common::TEXT_LST;
extern const char TEXT_LWP[];
using common::TEXT_MPYS;

// TMS999105
using common::TEXT_AM;
extern const char TEXT_BIND[];
extern const char TEXT_BLSK[];
extern const char TEXT_EVAD[];
extern const char TEXT_SLAM[];
using common::TEXT_SM;
extern const char TEXT_SRAM[];
extern const char TEXT_TCMB[];
extern const char TEXT_TMB[];
extern const char TEXT_TSMB[];

// TMS99110
extern const char TEXT_AR[];
extern const char TEXT_CER[];
extern const char TEXT_CIR[];
extern const char TEXT_CR[];
extern const char TEXT_CRE[];
extern const char TEXT_CRI[];
extern const char TEXT_DR[];
using common::TEXT_LDD;
using common::TEXT_LDS;
using common::TEXT_LR;
extern const char TEXT_MM[];
extern const char TEXT_MR[];
using common::TEXT_NEGR;
using common::TEXT_SR;
using common::TEXT_STR;

extern const char TEXT_SINGLE[];

// clang-format on
}  // namespace tms9900
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_TMS9900__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
