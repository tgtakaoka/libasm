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

#ifndef __LIBASM_TEXT_C166__
#define __LIBASM_TEXT_C166__

#include "text_common.h"

namespace libasm {
namespace text {
namespace c166 {
// clang-format off

extern const char TEXT_C166_LIST[]    PROGMEM;
extern const char TEXT_CPU_80C166[]   PROGMEM;
extern const char TEXT_CPU_80C167CS[] PROGMEM;
extern const char TEXT_CPU_80C167[]   PROGMEM;
extern const char TEXT_CPU_XC16X[]    PROGMEM;

using common::TEXT_ADDB;
using common::TEXT_ADDCB;
using common::TEXT_ADDC;
using common::TEXT_ADD;
using common::TEXT_ANDB;
using common::TEXT_AND;
extern const char TEXT_ASHR[]       PROGMEM;
extern const char TEXT_ATOMIC[]     PROGMEM;
using common::TEXT_BAND;
using common::TEXT_BCLR;
extern const char TEXT_BCMP[]       PROGMEM;
extern const char TEXT_BFLDH[]      PROGMEM;
extern const char TEXT_BFLDL[]      PROGMEM;
extern const char TEXT_BMOVN[]      PROGMEM;
using common::TEXT_BMOV;
using common::TEXT_BOR;
using common::TEXT_BSET;
using common::TEXT_BXOR;
extern const char TEXT_CALLA[]      PROGMEM;
extern const char TEXT_CALLA_M[]    PROGMEM;
extern const char TEXT_CALLA_P[]    PROGMEM;
using common::TEXT_CALLI;
using common::TEXT_CALLR;
using common::TEXT_CALLS;
using common::TEXT_CMPB;
extern const char TEXT_CMPD1[]      PROGMEM;
extern const char TEXT_CMPD2[]      PROGMEM;
extern const char TEXT_CMPI1[]      PROGMEM;
extern const char TEXT_CMPI2[]      PROGMEM;
using common::TEXT_CMP;
extern const char TEXT_COABS[]      PROGMEM;
extern const char TEXT_COADD2[]     PROGMEM;
extern const char TEXT_COADD[]      PROGMEM;
extern const char TEXT_COASHR[]     PROGMEM;
extern const char TEXT_COCMP[]      PROGMEM;
extern const char TEXT_COLOAD2[]    PROGMEM;
extern const char TEXT_COLOAD2_M[]  PROGMEM;
extern const char TEXT_COLOAD[]     PROGMEM;
extern const char TEXT_COLOAD_M[]   PROGMEM;
extern const char TEXT_COMAC[]      PROGMEM;
extern const char TEXT_COMAC_M[]    PROGMEM;
extern const char TEXT_COMACM[]     PROGMEM;
extern const char TEXT_COMACM_M[]   PROGMEM;
extern const char TEXT_COMACMR[]    PROGMEM;
extern const char TEXT_COMACMRSU[]  PROGMEM;
extern const char TEXT_COMACMRU[]   PROGMEM;
extern const char TEXT_COMACMRUS[]  PROGMEM;
extern const char TEXT_COMACMSU[]   PROGMEM;
extern const char TEXT_COMACMSU_M[] PROGMEM;
extern const char TEXT_COMACMU[]    PROGMEM;
extern const char TEXT_COMACMU_M[]  PROGMEM;
extern const char TEXT_COMACMUS[]   PROGMEM;
extern const char TEXT_COMACMUS_M[] PROGMEM;
extern const char TEXT_COMACR[]     PROGMEM;
extern const char TEXT_COMACRSU[]   PROGMEM;
extern const char TEXT_COMACRU[]    PROGMEM;
extern const char TEXT_COMACRUS[]   PROGMEM;
extern const char TEXT_COMACSU[]    PROGMEM;
extern const char TEXT_COMACSU_M[]  PROGMEM;
extern const char TEXT_COMACU[]     PROGMEM;
extern const char TEXT_COMACU_M[]   PROGMEM;
extern const char TEXT_COMACUS[]    PROGMEM;
extern const char TEXT_COMACUS_M[]  PROGMEM;
extern const char TEXT_COMAX[]      PROGMEM;
extern const char TEXT_COMIN[]      PROGMEM;
extern const char TEXT_COMOV[]      PROGMEM;
extern const char TEXT_COMUL[]      PROGMEM;
extern const char TEXT_COMUL_M[]    PROGMEM;
extern const char TEXT_COMULSU[]    PROGMEM;
extern const char TEXT_COMULSU_M[]  PROGMEM;
extern const char TEXT_COMULU[]     PROGMEM;
extern const char TEXT_COMULU_M[]   PROGMEM;
extern const char TEXT_COMULUS[]    PROGMEM;
extern const char TEXT_COMULUS_M[]  PROGMEM;
extern const char TEXT_CONEG[]      PROGMEM;
extern const char TEXT_CONOP[]      PROGMEM;
extern const char TEXT_CORND[]      PROGMEM;
extern const char TEXT_COSHL[]      PROGMEM;
extern const char TEXT_COSHR[]      PROGMEM;
extern const char TEXT_COSTORE[]    PROGMEM;
extern const char TEXT_COSUB2[]     PROGMEM;
extern const char TEXT_COSUB2R[]    PROGMEM;
extern const char TEXT_COSUB[]      PROGMEM;
extern const char TEXT_COSUBR[]     PROGMEM;
extern const char TEXT_CPLB[]       PROGMEM;
using common::TEXT_CPL;
extern const char TEXT_DISWDT[]     PROGMEM;
extern const char TEXT_DIVLU[]      PROGMEM;
using common::TEXT_DIVL;
using common::TEXT_DIV;
using common::TEXT_DIVU;
extern const char TEXT_EINIT[]      PROGMEM;
extern const char TEXT_ENWDT[]      PROGMEM;
extern const char TEXT_EXTP[]       PROGMEM;
extern const char TEXT_EXTPR[]      PROGMEM;
extern const char TEXT_EXTR[]       PROGMEM;
extern const char TEXT_EXTSR[]      PROGMEM;
using common::TEXT_EXTS;
using common::TEXT_IDLE;
using common::TEXT_JBC;
using common::TEXT_JB;
extern const char TEXT_JMPA[]       PROGMEM;
extern const char TEXT_JMPA_M[]     PROGMEM;
extern const char TEXT_JMPA_P[]     PROGMEM;
using common::TEXT_JMPI;
extern const char TEXT_JMPR[]       PROGMEM;
extern const char TEXT_JMPS[]       PROGMEM;
extern const char TEXT_JNBS[]       PROGMEM;
using common::TEXT_JNB;
extern const char TEXT_MOVBS[]      PROGMEM;
using common::TEXT_MOVB;
extern const char TEXT_MOVBZ[]      PROGMEM;
using common::TEXT_MOV;
using common::TEXT_MUL;
using common::TEXT_MULU;
using common::TEXT_NEGB;
using common::TEXT_NEG;
using common::TEXT_NOP;
using common::TEXT_ORB;
using common::TEXT_OR;
extern const char TEXT_PCALL[]      PROGMEM;
using common::TEXT_POP;
extern const char TEXT_PRIOR[]      PROGMEM;
using common::TEXT_PUSH;
extern const char TEXT_PWRDN[]      PROGMEM;
using common::TEXT_RETI;
extern const char TEXT_RETP[]       PROGMEM;
using common::TEXT_RETS;
using common::TEXT_RET;
extern const char TEXT_RND[]        PROGMEM;
extern const char TEXT_REPEAT[] PROGMEM;
using common::TEXT_ROL;
using common::TEXT_ROR;
extern const char TEXT_SBRK[]       PROGMEM;
extern const char TEXT_SCXT[]       PROGMEM;
using common::TEXT_SHL;
using common::TEXT_SHR;
extern const char TEXT_SRST[]       PROGMEM;
extern const char TEXT_SRVWDT[]     PROGMEM;
using common::TEXT_SUBB;
using common::TEXT_SUBCB;
using common::TEXT_SUBC;
using common::TEXT_SUB;
using common::TEXT_TRAP;
using common::TEXT_XORB;
using common::TEXT_XOR;

using common::TEXT_CC_C;
using common::TEXT_CC_EQ;
extern const char TEXT_CC_[]      PROGMEM;
using common::TEXT_CC_NC;
extern const char TEXT_CC_NET[]   PROGMEM;
using common::TEXT_CC_NE;
extern const char TEXT_CC_N[]     PROGMEM;
extern const char TEXT_CC_NN[]    PROGMEM;
extern const char TEXT_CC_NUSR0[] PROGMEM;
extern const char TEXT_CC_NUSR1[] PROGMEM;
using common::TEXT_CC_NV;
using common::TEXT_CC_NZ;
extern const char TEXT_CC_SGE[]   PROGMEM;
extern const char TEXT_CC_SGT[]   PROGMEM;
extern const char TEXT_CC_SLE[]   PROGMEM;
extern const char TEXT_CC_SLT[]   PROGMEM;
extern const char TEXT_CC_UC[]    PROGMEM;
using common::TEXT_CC_UGE;
using common::TEXT_CC_UGT;
using common::TEXT_CC_ULE;
using common::TEXT_CC_ULT;
extern const char TEXT_CC_USR0[]  PROGMEM;
extern const char TEXT_CC_USR1[]  PROGMEM;
using common::TEXT_CC_V;
using common::TEXT_CC_Z;

extern const char TEXT_REG_CP[]      PROGMEM;
extern const char TEXT_REG_CPUCON1[] PROGMEM;
extern const char TEXT_REG_CPUCON2[] PROGMEM;
extern const char TEXT_REG_CSP[]     PROGMEM;
extern const char TEXT_REG_DPP0[]    PROGMEM;
extern const char TEXT_REG_DPP1[]    PROGMEM;
extern const char TEXT_REG_DPP2[]    PROGMEM;
extern const char TEXT_REG_DPP3[]    PROGMEM;
extern const char TEXT_REG_IDX0[]    PROGMEM;
extern const char TEXT_REG_IDX1[]    PROGMEM;
extern const char TEXT_REG_MAH[]     PROGMEM;
extern const char TEXT_REG_MAL[]     PROGMEM;
extern const char TEXT_REG_MAS[]     PROGMEM;
extern const char TEXT_REG_MCW[]     PROGMEM;
extern const char TEXT_REG_MDC[]     PROGMEM;
extern const char TEXT_REG_MDH[]     PROGMEM;
extern const char TEXT_REG_MDL[]     PROGMEM;
extern const char TEXT_REG_MRW[]     PROGMEM;
extern const char TEXT_REG_MSW[]     PROGMEM;
extern const char TEXT_REG_ONES[]    PROGMEM;
using common::TEXT_REG_PSW;
extern const char TEXT_REG_QR0[]     PROGMEM;
extern const char TEXT_REG_QR1[]     PROGMEM;
extern const char TEXT_REG_QX0[]     PROGMEM;
extern const char TEXT_REG_QX1[]     PROGMEM;
using common::TEXT_REG_SP;
extern const char TEXT_REG_SPSEG[]   PROGMEM;
extern const char TEXT_REG_STKOV[]   PROGMEM;
extern const char TEXT_REG_STKUN[]   PROGMEM;
extern const char TEXT_REG_TFR[]     PROGMEM;
extern const char TEXT_REG_VECSEG[]  PROGMEM;
extern const char TEXT_REG_ZEROS[]   PROGMEM;

// clang-format on
}  // namespace c166
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_C166__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
