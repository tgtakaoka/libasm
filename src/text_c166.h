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

extern const char TEXT_C166_LIST[];
extern const char TEXT_CPU_80C166[];
extern const char TEXT_CPU_80C167CS[];
extern const char TEXT_CPU_80C167[];
extern const char TEXT_CPU_XC16X[];

using common::TEXT_ADDB;
using common::TEXT_ADDCB;
using common::TEXT_ADDC;
using common::TEXT_ADD;
using common::TEXT_ANDB;
using common::TEXT_AND;
extern const char TEXT_ASHR[];
extern const char TEXT_ATOMIC[];
using common::TEXT_BAND;
using common::TEXT_BCLR;
extern const char TEXT_BCMP[];
extern const char TEXT_BFLDH[];
extern const char TEXT_BFLDL[];
extern const char TEXT_BMOVN[];
using common::TEXT_BMOV;
using common::TEXT_BOR;
using common::TEXT_BSET;
using common::TEXT_BXOR;
extern const char TEXT_CALLA[];
extern const char TEXT_CALLA_M[];
extern const char TEXT_CALLA_P[];
using common::TEXT_CALLI;
using common::TEXT_CALLR;
using common::TEXT_CALLS;
using common::TEXT_CMPB;
extern const char TEXT_CMPD1[];
extern const char TEXT_CMPD2[];
extern const char TEXT_CMPI1[];
extern const char TEXT_CMPI2[];
using common::TEXT_CMP;
extern const char TEXT_COABS[];
extern const char TEXT_COADD2[];
extern const char TEXT_COADD[];
extern const char TEXT_COASHR[];
extern const char TEXT_COCMP[];
extern const char TEXT_COLOAD2[];
extern const char TEXT_COLOAD2_M[];
extern const char TEXT_COLOAD[];
extern const char TEXT_COLOAD_M[];
extern const char TEXT_COMAC[];
extern const char TEXT_COMAC_M[];
extern const char TEXT_COMACM[];
extern const char TEXT_COMACM_M[];
extern const char TEXT_COMACMR[];
extern const char TEXT_COMACMRSU[];
extern const char TEXT_COMACMRU[];
extern const char TEXT_COMACMRUS[];
extern const char TEXT_COMACMSU[];
extern const char TEXT_COMACMSU_M[];
extern const char TEXT_COMACMU[];
extern const char TEXT_COMACMU_M[];
extern const char TEXT_COMACMUS[];
extern const char TEXT_COMACMUS_M[];
extern const char TEXT_COMACR[];
extern const char TEXT_COMACRSU[];
extern const char TEXT_COMACRU[];
extern const char TEXT_COMACRUS[];
extern const char TEXT_COMACSU[];
extern const char TEXT_COMACSU_M[];
extern const char TEXT_COMACU[];
extern const char TEXT_COMACU_M[];
extern const char TEXT_COMACUS[];
extern const char TEXT_COMACUS_M[];
extern const char TEXT_COMAX[];
extern const char TEXT_COMIN[];
extern const char TEXT_COMOV[];
extern const char TEXT_COMUL[];
extern const char TEXT_COMUL_M[];
extern const char TEXT_COMULSU[];
extern const char TEXT_COMULSU_M[];
extern const char TEXT_COMULU[];
extern const char TEXT_COMULU_M[];
extern const char TEXT_COMULUS[];
extern const char TEXT_COMULUS_M[];
extern const char TEXT_CONEG[];
extern const char TEXT_CONOP[];
extern const char TEXT_CORND[];
extern const char TEXT_COSHL[];
extern const char TEXT_COSHR[];
extern const char TEXT_COSTORE[];
extern const char TEXT_COSUB2[];
extern const char TEXT_COSUB2R[];
extern const char TEXT_COSUB[];
extern const char TEXT_COSUBR[];
extern const char TEXT_CPLB[];
using common::TEXT_CPL;
extern const char TEXT_DISWDT[];
extern const char TEXT_DIVLU[];
using common::TEXT_DIVL;
using common::TEXT_DIV;
using common::TEXT_DIVU;
extern const char TEXT_EINIT[];
extern const char TEXT_ENWDT[];
extern const char TEXT_EXTP[];
extern const char TEXT_EXTPR[];
extern const char TEXT_EXTR[];
extern const char TEXT_EXTSR[];
using common::TEXT_EXTS;
using common::TEXT_IDLE;
using common::TEXT_JBC;
using common::TEXT_JB;
extern const char TEXT_JMPA[];
extern const char TEXT_JMPA_M[];
extern const char TEXT_JMPA_P[];
using common::TEXT_JMPI;
extern const char TEXT_JMPR[];
extern const char TEXT_JMPS[];
extern const char TEXT_JNBS[];
using common::TEXT_JNB;
extern const char TEXT_MOVBS[];
using common::TEXT_MOVB;
extern const char TEXT_MOVBZ[];
using common::TEXT_MOV;
using common::TEXT_MUL;
using common::TEXT_MULU;
using common::TEXT_NEGB;
using common::TEXT_NEG;
using common::TEXT_NOP;
using common::TEXT_ORB;
using common::TEXT_OR;
extern const char TEXT_PCALL[];
using common::TEXT_POP;
extern const char TEXT_PRIOR[];
using common::TEXT_PUSH;
extern const char TEXT_PWRDN[];
using common::TEXT_RETI;
extern const char TEXT_RETP[];
using common::TEXT_RETS;
using common::TEXT_RET;
extern const char TEXT_RND[];
extern const char TEXT_REPEAT[];
using common::TEXT_ROL;
using common::TEXT_ROR;
extern const char TEXT_SBRK[];
extern const char TEXT_SCXT[];
using common::TEXT_SHL;
using common::TEXT_SHR;
extern const char TEXT_SRST[];
extern const char TEXT_SRVWDT[];
using common::TEXT_SUBB;
using common::TEXT_SUBCB;
using common::TEXT_SUBC;
using common::TEXT_SUB;
using common::TEXT_TRAP;
using common::TEXT_XORB;
using common::TEXT_XOR;

using common::TEXT_CC_C;
using common::TEXT_CC_EQ;
extern const char TEXT_CC_[];
using common::TEXT_CC_NC;
extern const char TEXT_CC_NET[];
using common::TEXT_CC_NE;
extern const char TEXT_CC_N[];
extern const char TEXT_CC_NN[];
extern const char TEXT_CC_NUSR0[];
extern const char TEXT_CC_NUSR1[];
using common::TEXT_CC_NV;
using common::TEXT_CC_NZ;
extern const char TEXT_CC_SGE[];
extern const char TEXT_CC_SGT[];
extern const char TEXT_CC_SLE[];
extern const char TEXT_CC_SLT[];
extern const char TEXT_CC_UC[];
using common::TEXT_CC_UGE;
using common::TEXT_CC_UGT;
using common::TEXT_CC_ULE;
using common::TEXT_CC_ULT;
extern const char TEXT_CC_USR0[];
extern const char TEXT_CC_USR1[];
using common::TEXT_CC_V;
using common::TEXT_CC_Z;

extern const char TEXT_REG_CP[];
extern const char TEXT_REG_CPUCON1[];
extern const char TEXT_REG_CPUCON2[];
extern const char TEXT_REG_CSP[];
extern const char TEXT_REG_DPP0[];
extern const char TEXT_REG_DPP1[];
extern const char TEXT_REG_DPP2[];
extern const char TEXT_REG_DPP3[];
extern const char TEXT_REG_IDX0[];
extern const char TEXT_REG_IDX1[];
extern const char TEXT_REG_MAH[];
extern const char TEXT_REG_MAL[];
extern const char TEXT_REG_MAS[];
extern const char TEXT_REG_MCW[];
extern const char TEXT_REG_MDC[];
extern const char TEXT_REG_MDH[];
extern const char TEXT_REG_MDL[];
extern const char TEXT_REG_MRW[];
extern const char TEXT_REG_MSW[];
extern const char TEXT_REG_ONES[];
using common::TEXT_REG_PSW;
extern const char TEXT_REG_QR0[];
extern const char TEXT_REG_QR1[];
extern const char TEXT_REG_QX0[];
extern const char TEXT_REG_QX1[];
using common::TEXT_REG_SP;
extern const char TEXT_REG_SPSEG[];
extern const char TEXT_REG_STKOV[];
extern const char TEXT_REG_STKUN[];
extern const char TEXT_REG_TFR[];
extern const char TEXT_REG_VECSEG[];
extern const char TEXT_REG_ZEROS[];

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
