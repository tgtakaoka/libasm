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

#ifndef __TEXT_TMS9900__
#define __TEXT_TMS9900__

#include "text_common.h"

namespace libasm {
namespace text {
namespace tms9900 {

// clang-format off
extern const char TEXT_TMS9900_LIST[]  PROGMEM;
extern const char TEXT_CPU_9900[]  PROGMEM;
extern const char TEXT_CPU_9980[]  PROGMEM;
extern const char TEXT_CPU_9995[]  PROGMEM;
extern const char TEXT_CPU_99105[] PROGMEM;

// TMS9900
using common::TEXT_A;
extern const char TEXT_AB[]   PROGMEM;
using common::TEXT_ABS;
using common::TEXT_AI;
using common::TEXT_ANDI;
using common::TEXT_B;
using common::TEXT_BL;
extern const char TEXT_BLWP[] PROGMEM;
using common::TEXT_C;
using common::TEXT_CB;
using common::TEXT_CI;
extern const char TEXT_CKOF[] PROGMEM;
extern const char TEXT_CKON[] PROGMEM;
using common::TEXT_CLR;
extern const char TEXT_COC[]  PROGMEM;
extern const char TEXT_CZC[]  PROGMEM;
using common::TEXT_DEC;
extern const char TEXT_DECT[] PROGMEM;
using common::TEXT_DIV;
using common::TEXT_IDLE;
using common::TEXT_INC;
extern const char TEXT_INCT[] PROGMEM;
using common::TEXT_INV;
using common::TEXT_JEQ;
using common::TEXT_JGT;
using common::TEXT_JH;
extern const char TEXT_JHE[]  PROGMEM;
using common::TEXT_JL;
using common::TEXT_JLE;
using common::TEXT_JLT;
using common::TEXT_JMP;
using common::TEXT_JNC;
using common::TEXT_JNE;
using common::TEXT_JNO;
extern const char TEXT_JOC[]  PROGMEM;
extern const char TEXT_JOP[]  PROGMEM;
extern const char TEXT_LDCR[] PROGMEM;
using common::TEXT_LI;
extern const char TEXT_LIMI[] PROGMEM;
extern const char TEXT_LREX[] PROGMEM;
extern const char TEXT_LWPI[] PROGMEM;
using common::TEXT_MOV;
using common::TEXT_MOVB;
extern const char TEXT_MID[]  PROGMEM;
using common::TEXT_MPY;
using common::TEXT_NEG;
using common::TEXT_ORI;
extern const char TEXT_RSET[] PROGMEM;
extern const char TEXT_RTWP[] PROGMEM;
using common::TEXT_S;
extern const char TEXT_SB[]   PROGMEM;
extern const char TEXT_SBO[]  PROGMEM;
extern const char TEXT_SBZ[]  PROGMEM;
extern const char TEXT_SETO[] PROGMEM;
using common::TEXT_SLA;
using common::TEXT_SRA;
extern const char TEXT_SRC[]  PROGMEM;
using common::TEXT_SRL;
extern const char TEXT_SOC[]  PROGMEM;
extern const char TEXT_SOCB[] PROGMEM;
extern const char TEXT_STCR[] PROGMEM;
extern const char TEXT_STST[] PROGMEM;
extern const char TEXT_STWP[] PROGMEM;
extern const char TEXT_SWPB[] PROGMEM;
extern const char TEXT_SZC[]  PROGMEM;
extern const char TEXT_SZCB[] PROGMEM;
extern const char TEXT_TB[]   PROGMEM;
extern const char TEXT_X[]    PROGMEM;
extern const char TEXT_XOP[]  PROGMEM;
using common::TEXT_XOR;

// TMS9995
extern const char TEXT_MPYS[] PROGMEM;
using common::TEXT_DIVS;
using common::TEXT_LST;
extern const char TEXT_LWP[]  PROGMEM;

// TMS999105
using common::TEXT_AM;
extern const char TEXT_BIND[] PROGMEM;
extern const char TEXT_BLSK[] PROGMEM;
extern const char TEXT_EVAD[] PROGMEM;
extern const char TEXT_SLAM[] PROGMEM;
using common::TEXT_SM;
extern const char TEXT_SRAM[] PROGMEM;
extern const char TEXT_TCMB[] PROGMEM;
extern const char TEXT_TMB[]  PROGMEM;
extern const char TEXT_TSMB[] PROGMEM;
// clang-format on

}  // namespace tms9900
}  // namespace text
}  // namespace libasm

#endif  // __TEXT_TMS9900__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
