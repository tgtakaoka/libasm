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

extern const char TEXT_I960_LIST[] PROGMEM;
extern const char TEXT_CPU_80960[] PROGMEM;
extern const char OPT_BOOL_MODE_BITS[] PROGMEM;
extern const char OPT_DESC_MODE_BITS[] PROGMEM;
extern const char OPT_TEXT_IP_OPERAND[] PROGMEM;
extern const char OPT_DESC_IP_OPERAND[] PROGMEM;
extern const char OPT_BOOL_CANONICAL_MEM[] PROGMEM;
extern const char OPT_DESC_CANONICAL_MEM[] PROGMEM;
extern const char OPT_BOOL_FPU[] PROGMEM;
extern const char OPT_DESC_FPU[] PROGMEM;
extern const char TEXT_REG_PFP[] PROGMEM;
extern const char TEXT_REG_RIP[] PROGMEM;
extern const char TEXT_REG_IP[]  PROGMEM;
using common::TEXT_REG_FP;
using common::TEXT_REG_SP;

// i960
using common::TEXT_ADDC;
using common::TEXT_ADDI;
extern const char TEXT_ADDO[]     PROGMEM;
extern const char TEXT_ALTERBIT[] PROGMEM;
using common::TEXT_AND;
extern const char TEXT_ANDNOT[]   PROGMEM;
extern const char TEXT_ATADD[]    PROGMEM;
extern const char TEXT_ATMOD[]    PROGMEM;
using common::TEXT_B;
extern const char TEXT_BAL[]      PROGMEM;
extern const char TEXT_BALX[]     PROGMEM;
extern const char TEXT_BBC[]      PROGMEM;
extern const char TEXT_BBS[]      PROGMEM;
extern const char TEXT_BE[]       PROGMEM;
extern const char TEXT_BG[]       PROGMEM;
using common::TEXT_BGE;
using common::TEXT_BL;
using common::TEXT_BLE;
using common::TEXT_BNE;
extern const char TEXT_BNO[]      PROGMEM;
extern const char TEXT_BO[]       PROGMEM;
extern const char TEXT_BX[]       PROGMEM;
using common::TEXT_CALL;
extern const char TEXT_CALLS[]    PROGMEM;
extern const char TEXT_CALLX[]    PROGMEM;
extern const char TEXT_CHKBIT[]   PROGMEM;
extern const char TEXT_CLRBIT[]   PROGMEM;
extern const char TEXT_CMPDECI[]  PROGMEM;
extern const char TEXT_CMPDECO[]  PROGMEM;
using common::TEXT_CMPI;
extern const char TEXT_CMPIBE[]   PROGMEM;
extern const char TEXT_CMPIBG[]   PROGMEM;
extern const char TEXT_CMPIBGE[]  PROGMEM;
extern const char TEXT_CMPIBL[]   PROGMEM;
extern const char TEXT_CMPIBLE[]  PROGMEM;
extern const char TEXT_CMPIBNE[]  PROGMEM;
extern const char TEXT_CMPIBNO[]  PROGMEM;
extern const char TEXT_CMPIBO[]   PROGMEM;
extern const char TEXT_CMPINCI[]  PROGMEM;
extern const char TEXT_CMPINCO[]  PROGMEM;
extern const char TEXT_CMPO[]     PROGMEM;
extern const char TEXT_CMPOBE[]   PROGMEM;
extern const char TEXT_CMPOBG[]   PROGMEM;
extern const char TEXT_CMPOBGE[]  PROGMEM;
extern const char TEXT_CMPOBL[]   PROGMEM;
extern const char TEXT_CMPOBLE[]  PROGMEM;
extern const char TEXT_CMPOBNE[]  PROGMEM;
extern const char TEXT_CONCMPI[]  PROGMEM;
extern const char TEXT_CONCMPO[]  PROGMEM;
extern const char TEXT_DIVI[]     PROGMEM;
extern const char TEXT_DIVO[]     PROGMEM;
using common::TEXT_EDIV;
using common::TEXT_EMUL;
extern const char TEXT_EXTRACT[]  PROGMEM;
extern const char TEXT_FAULTE[]   PROGMEM;
extern const char TEXT_FAULTG[]   PROGMEM;
extern const char TEXT_FAULTGE[]  PROGMEM;
extern const char TEXT_FAULTL[]   PROGMEM;
extern const char TEXT_FAULTLE[]  PROGMEM;
extern const char TEXT_FAULTNE[]  PROGMEM;
extern const char TEXT_FAULTNO[]  PROGMEM;
extern const char TEXT_FAULTO[]   PROGMEM;
extern const char TEXT_FLUSHREG[] PROGMEM;
extern const char TEXT_FMARK[]    PROGMEM;
using common::TEXT_LD;
using common::TEXT_LDA;
extern const char TEXT_LDIB[]     PROGMEM;
extern const char TEXT_LDIS[]     PROGMEM;
extern const char TEXT_LDL[]      PROGMEM;
extern const char TEXT_LDOB[]     PROGMEM;
extern const char TEXT_LDOS[]     PROGMEM;
extern const char TEXT_LDQ[]      PROGMEM;
extern const char TEXT_LDT[]      PROGMEM;
using common::TEXT_MARK;
extern const char TEXT_MODAC[]    PROGMEM;
extern const char TEXT_MODI[]     PROGMEM;
extern const char TEXT_MODIFY[]   PROGMEM;
extern const char TEXT_MODPC[]    PROGMEM;
extern const char TEXT_MODTC[]    PROGMEM;
using common::TEXT_MOV;
extern const char TEXT_MOVL[]     PROGMEM;
extern const char TEXT_MOVQ[]     PROGMEM;
extern const char TEXT_MOVT[]     PROGMEM;
extern const char TEXT_MULI[]     PROGMEM;
extern const char TEXT_MULO[]     PROGMEM;
extern const char TEXT_NAND[]     PROGMEM;
extern const char TEXT_NOR[]      PROGMEM;
using common::TEXT_NOT;
extern const char TEXT_NOTAND[]   PROGMEM;
extern const char TEXT_NOTBIT[]   PROGMEM;
extern const char TEXT_NOTOR[]    PROGMEM;
using common::TEXT_OR;
extern const char TEXT_ORNOT[]    PROGMEM;
extern const char TEXT_REMI[]     PROGMEM;
extern const char TEXT_REMO[]     PROGMEM;
using common::TEXT_RET;
extern const char TEXT_ROTATE[]   PROGMEM;
extern const char TEXT_SCANBIT[]  PROGMEM;
extern const char TEXT_SCANBYTE[] PROGMEM;
extern const char TEXT_SETBIT[]   PROGMEM;
extern const char TEXT_SHLI[]     PROGMEM;
extern const char TEXT_SHLO[]     PROGMEM;
extern const char TEXT_SHRDI[]    PROGMEM;
extern const char TEXT_SHRI[]     PROGMEM;
extern const char TEXT_SHRO[]     PROGMEM;
extern const char TEXT_SPANBIT[]  PROGMEM;
using common::TEXT_ST;
extern const char TEXT_STIB[]     PROGMEM;
extern const char TEXT_STIS[]     PROGMEM;
extern const char TEXT_STL[]      PROGMEM;
extern const char TEXT_STOB[]     PROGMEM;
extern const char TEXT_STOS[]     PROGMEM;
extern const char TEXT_STQ[]      PROGMEM;
extern const char TEXT_STT[]      PROGMEM;
using common::TEXT_SUBC;
using common::TEXT_SUBI;
extern const char TEXT_SUBO[]     PROGMEM;
extern const char TEXT_SYNCF[]    PROGMEM;
extern const char TEXT_SYNLD[]    PROGMEM;
extern const char TEXT_SYNMOV[]   PROGMEM;
extern const char TEXT_SYNMOVL[]  PROGMEM;
extern const char TEXT_SYNMOVQ[]  PROGMEM;
extern const char TEXT_TESTE[]    PROGMEM;
extern const char TEXT_TESTG[]    PROGMEM;
extern const char TEXT_TESTGE[]   PROGMEM;
extern const char TEXT_TESTL[]    PROGMEM;
extern const char TEXT_TESTLE[]   PROGMEM;
extern const char TEXT_TESTNE[]   PROGMEM;
extern const char TEXT_TESTNO[]   PROGMEM;
extern const char TEXT_TESTO[]    PROGMEM;
extern const char TEXT_XNOR[]     PROGMEM;
using common::TEXT_XOR;


// 80960KB floating-point and decimal extensions
using common::TEXT_ADDR;
extern const char TEXT_ADDRL[]   PROGMEM;
extern const char TEXT_ATANR[]   PROGMEM;
extern const char TEXT_ATANRL[]  PROGMEM;
extern const char TEXT_CLASSR[]  PROGMEM;
extern const char TEXT_CLASSRL[] PROGMEM;
extern const char TEXT_CMPOR[]   PROGMEM;
extern const char TEXT_CMPORL[]  PROGMEM;
using common::TEXT_CMPR;
extern const char TEXT_CMPRL[]   PROGMEM;
extern const char TEXT_COSR[]    PROGMEM;
extern const char TEXT_COSRL[]   PROGMEM;
extern const char TEXT_CPYRSRE[] PROGMEM;
extern const char TEXT_CPYSRE[]  PROGMEM;
extern const char TEXT_CVTILR[]  PROGMEM;
extern const char TEXT_CVTIR[]   PROGMEM;
extern const char TEXT_CVTRI[]   PROGMEM;
extern const char TEXT_CVTRIL[]  PROGMEM;
extern const char TEXT_CVTZRI[]  PROGMEM;
extern const char TEXT_CVTZRIL[] PROGMEM;
extern const char TEXT_DADDC[]   PROGMEM;
extern const char TEXT_DIVR[]    PROGMEM;
extern const char TEXT_DIVRL[]   PROGMEM;
extern const char TEXT_DMOVT[]   PROGMEM;
extern const char TEXT_DSUBC[]   PROGMEM;
extern const char TEXT_EXPR[]    PROGMEM;
extern const char TEXT_EXPRL[]   PROGMEM;
extern const char TEXT_LOGBNR[]  PROGMEM;
extern const char TEXT_LOGBNRL[] PROGMEM;
extern const char TEXT_LOGEPR[]  PROGMEM;
extern const char TEXT_LOGEPRL[] PROGMEM;
extern const char TEXT_LOGR[]    PROGMEM;
extern const char TEXT_LOGRL[]   PROGMEM;
extern const char TEXT_MOVR[]    PROGMEM;
extern const char TEXT_MOVRE[]   PROGMEM;
extern const char TEXT_MOVRL[]   PROGMEM;
extern const char TEXT_MULR[]    PROGMEM;
extern const char TEXT_MULRL[]   PROGMEM;
extern const char TEXT_REMR[]    PROGMEM;
extern const char TEXT_REMRL[]   PROGMEM;
extern const char TEXT_ROUNDR[]  PROGMEM;
extern const char TEXT_ROUNDRL[] PROGMEM;
extern const char TEXT_SCALER[]  PROGMEM;
extern const char TEXT_SCALERL[] PROGMEM;
extern const char TEXT_SINR[]    PROGMEM;
extern const char TEXT_SINRL[]   PROGMEM;
extern const char TEXT_SQRTR[]   PROGMEM;
extern const char TEXT_SQRTRL[]  PROGMEM;
using common::TEXT_SUBR;
extern const char TEXT_SUBRL[]   PROGMEM;
extern const char TEXT_TANR[]    PROGMEM;
extern const char TEXT_TANRL[]   PROGMEM;

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
