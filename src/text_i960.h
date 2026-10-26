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

#ifndef __LIBASM_TEXT_I960__
#define __LIBASM_TEXT_I960__

#include "text_common.h"

namespace libasm {
namespace text {
namespace i960 {
// clang-format off

extern const char TEXT_I960_LIST[];
extern const char TEXT_CPU_80960[];
extern const char OPT_BOOL_MODE_BITS[];
extern const char OPT_DESC_MODE_BITS[];
extern const char OPT_TEXT_IP_OPERAND[];
extern const char OPT_DESC_IP_OPERAND[];
extern const char OPT_BOOL_CANONICAL_MEM[];
extern const char OPT_BOOL_FPU[];
extern const char OPT_DESC_FPU[];
extern const char TEXT_REG_PFP[];
extern const char TEXT_REG_RIP[];
extern const char TEXT_REG_IP[];
using common::TEXT_REG_FP;
using common::TEXT_REG_SP;

// i960
using common::TEXT_ADDC;
using common::TEXT_ADDI;
extern const char TEXT_ADDO[];
extern const char TEXT_ALTERBIT[];
using common::TEXT_AND;
extern const char TEXT_ANDNOT[];
extern const char TEXT_ATADD[];
extern const char TEXT_ATMOD[];
using common::TEXT_B;
extern const char TEXT_BAL[];
extern const char TEXT_BALX[];
extern const char TEXT_BBC[];
extern const char TEXT_BBS[];
extern const char TEXT_BE[];
extern const char TEXT_BG[];
using common::TEXT_BGE;
using common::TEXT_BL;
using common::TEXT_BLE;
using common::TEXT_BNE;
extern const char TEXT_BNO[];
extern const char TEXT_BO[];
extern const char TEXT_BX[];
using common::TEXT_CALL;
using common::TEXT_CALLS;
extern const char TEXT_CALLX[];
extern const char TEXT_CHKBIT[];
extern const char TEXT_CLRBIT[];
extern const char TEXT_CMPDECI[];
extern const char TEXT_CMPDECO[];
using common::TEXT_CMPI;
extern const char TEXT_CMPIBE[];
extern const char TEXT_CMPIBG[];
extern const char TEXT_CMPIBGE[];
extern const char TEXT_CMPIBL[];
extern const char TEXT_CMPIBLE[];
extern const char TEXT_CMPIBNE[];
extern const char TEXT_CMPIBNO[];
extern const char TEXT_CMPIBO[];
extern const char TEXT_CMPINCI[];
extern const char TEXT_CMPINCO[];
extern const char TEXT_CMPO[];
extern const char TEXT_CMPOBE[];
extern const char TEXT_CMPOBG[];
extern const char TEXT_CMPOBGE[];
extern const char TEXT_CMPOBL[];
extern const char TEXT_CMPOBLE[];
extern const char TEXT_CMPOBNE[];
extern const char TEXT_CONCMPI[];
extern const char TEXT_CONCMPO[];
extern const char TEXT_DIVI[];
extern const char TEXT_DIVO[];
using common::TEXT_EDIV;
using common::TEXT_EMUL;
extern const char TEXT_EXTRACT[];
extern const char TEXT_FAULTE[];
extern const char TEXT_FAULTG[];
extern const char TEXT_FAULTGE[];
extern const char TEXT_FAULTL[];
extern const char TEXT_FAULTLE[];
extern const char TEXT_FAULTNE[];
extern const char TEXT_FAULTNO[];
extern const char TEXT_FAULTO[];
extern const char TEXT_FLUSHREG[];
extern const char TEXT_FMARK[];
using common::TEXT_LD;
using common::TEXT_LDA;
extern const char TEXT_LDIB[];
extern const char TEXT_LDIS[];
extern const char TEXT_LDL[];
extern const char TEXT_LDOB[];
extern const char TEXT_LDOS[];
extern const char TEXT_LDQ[];
extern const char TEXT_LDT[];
using common::TEXT_MARK;
extern const char TEXT_MODAC[];
extern const char TEXT_MODI[];
extern const char TEXT_MODIFY[];
extern const char TEXT_MODPC[];
extern const char TEXT_MODTC[];
using common::TEXT_MOV;
extern const char TEXT_MOVL[];
extern const char TEXT_MOVQ[];
extern const char TEXT_MOVT[];
extern const char TEXT_MULI[];
extern const char TEXT_MULO[];
extern const char TEXT_NAND[];
extern const char TEXT_NOR[];
using common::TEXT_NOT;
extern const char TEXT_NOTAND[];
extern const char TEXT_NOTBIT[];
extern const char TEXT_NOTOR[];
using common::TEXT_OR;
extern const char TEXT_ORNOT[];
extern const char TEXT_REMI[];
extern const char TEXT_REMO[];
using common::TEXT_RET;
extern const char TEXT_ROTATE[];
extern const char TEXT_SCANBIT[];
extern const char TEXT_SCANBYTE[];
extern const char TEXT_SETBIT[];
extern const char TEXT_SHLI[];
extern const char TEXT_SHLO[];
extern const char TEXT_SHRDI[];
extern const char TEXT_SHRI[];
extern const char TEXT_SHRO[];
extern const char TEXT_SPANBIT[];
using common::TEXT_ST;
extern const char TEXT_STIB[];
extern const char TEXT_STIS[];
extern const char TEXT_STL[];
extern const char TEXT_STOB[];
extern const char TEXT_STOS[];
extern const char TEXT_STQ[];
extern const char TEXT_STT[];
using common::TEXT_SUBC;
using common::TEXT_SUBI;
extern const char TEXT_SUBO[];
extern const char TEXT_SYNCF[];
extern const char TEXT_SYNLD[];
extern const char TEXT_SYNMOV[];
extern const char TEXT_SYNMOVL[];
extern const char TEXT_SYNMOVQ[];
extern const char TEXT_TESTE[];
extern const char TEXT_TESTG[];
extern const char TEXT_TESTGE[];
extern const char TEXT_TESTL[];
extern const char TEXT_TESTLE[];
extern const char TEXT_TESTNE[];
extern const char TEXT_TESTNO[];
extern const char TEXT_TESTO[];
extern const char TEXT_XNOR[];
using common::TEXT_XOR;


// 80960KB floating-point and decimal extensions
using common::TEXT_ADDR;
extern const char TEXT_ADDRL[];
extern const char TEXT_ATANR[];
extern const char TEXT_ATANRL[];
extern const char TEXT_CLASSR[];
extern const char TEXT_CLASSRL[];
extern const char TEXT_CMPOR[];
extern const char TEXT_CMPORL[];
using common::TEXT_CMPR;
extern const char TEXT_CMPRL[];
extern const char TEXT_COSR[];
extern const char TEXT_COSRL[];
extern const char TEXT_CPYRSRE[];
extern const char TEXT_CPYSRE[];
extern const char TEXT_CVTILR[];
extern const char TEXT_CVTIR[];
extern const char TEXT_CVTRI[];
extern const char TEXT_CVTRIL[];
extern const char TEXT_CVTZRI[];
extern const char TEXT_CVTZRIL[];
extern const char TEXT_DADDC[];
extern const char TEXT_DIVR[];
extern const char TEXT_DIVRL[];
extern const char TEXT_DMOVT[];
extern const char TEXT_DSUBC[];
extern const char TEXT_EXPR[];
extern const char TEXT_EXPRL[];
extern const char TEXT_LOGBNR[];
extern const char TEXT_LOGBNRL[];
extern const char TEXT_LOGEPR[];
extern const char TEXT_LOGEPRL[];
extern const char TEXT_LOGR[];
extern const char TEXT_LOGRL[];
extern const char TEXT_MOVR[];
extern const char TEXT_MOVRE[];
extern const char TEXT_MOVRL[];
extern const char TEXT_MULR[];
extern const char TEXT_MULRL[];
extern const char TEXT_REMR[];
extern const char TEXT_REMRL[];
extern const char TEXT_ROUNDR[];
extern const char TEXT_ROUNDRL[];
extern const char TEXT_SCALER[];
extern const char TEXT_SCALERL[];
extern const char TEXT_SINR[];
extern const char TEXT_SINRL[];
extern const char TEXT_SQRTR[];
extern const char TEXT_SQRTRL[];
using common::TEXT_SUBR;
extern const char TEXT_SUBRL[];
extern const char TEXT_TANR[];
extern const char TEXT_TANRL[];

// clang-format on
}  // namespace i960
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_I960__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
