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

#ifndef __LIBASM_TEXT_Z8000__
#define __LIBASM_TEXT_Z8000__

#include "text_common.h"

namespace libasm {
namespace text {
namespace z8000 {
// clang-format off

extern const char TEXT_Z8000_LIST[];
extern const char TEXT_CPU_Z8001[];
extern const char TEXT_CPU_Z8002[];

using common::TEXT_ADC;
using common::TEXT_ADCB;
using common::TEXT_ADD;
using common::TEXT_ADDB;
using common::TEXT_ADDL;
using common::TEXT_AND;
using common::TEXT_ANDB;
using common::TEXT_BIT;
using common::TEXT_BITB;
using common::TEXT_CALL;
using common::TEXT_CALR;
using common::TEXT_CLR;
using common::TEXT_CLRB;
using common::TEXT_COM;
using common::TEXT_COMB;
extern const char TEXT_COMFLG[];
using common::TEXT_CP;
extern const char TEXT_CPB[];
using common::TEXT_CPD;
extern const char TEXT_CPDB[];
using common::TEXT_CPDR;
extern const char TEXT_CPDRB[];
using common::TEXT_CPI;
extern const char TEXT_CPIB[];
using common::TEXT_CPIR;
extern const char TEXT_CPIRB[];
using common::TEXT_CPL;
extern const char TEXT_CPSD[];
extern const char TEXT_CPSDB[];
extern const char TEXT_CPSDR[];
extern const char TEXT_CPSDRB[];
extern const char TEXT_CPSI[];
extern const char TEXT_CPSIB[];
extern const char TEXT_CPSIR[];
extern const char TEXT_CPSIRB[];
extern const char TEXT_DAB[];
extern const char TEXT_DBJNZ[];
using common::TEXT_DEC;
using common::TEXT_DECB;
using common::TEXT_DI;
using common::TEXT_DIV;
using common::TEXT_DIVL;
using common::TEXT_DJNZ;
using common::TEXT_EI;
using common::TEXT_EX;
extern const char TEXT_EXB[];
using common::TEXT_EXTS;
using common::TEXT_EXTSB;
extern const char TEXT_EXTSL[];
using common::TEXT_HALT;
using common::TEXT_IN;
extern const char TEXT_INB[];
using common::TEXT_INC;
using common::TEXT_INCB;
using common::TEXT_IND;
extern const char TEXT_INDB[];
using common::TEXT_INDR;
extern const char TEXT_INDRB[];
using common::TEXT_INI;
extern const char TEXT_INIB[];
using common::TEXT_INIR;
extern const char TEXT_INIRB[];
using common::TEXT_IRET;
using common::TEXT_JP;
using common::TEXT_JR;
using common::TEXT_LD;
using common::TEXT_LDA;
using common::TEXT_LDAR;
using common::TEXT_LDB;
using common::TEXT_LDCTL;
extern const char TEXT_LDCTLB[];
using common::TEXT_LDD;
extern const char TEXT_LDDB[];
using common::TEXT_LDDR;
extern const char TEXT_LDDRB[];
using common::TEXT_LDI;
extern const char TEXT_LDIB[];
using common::TEXT_LDIR;
extern const char TEXT_LDIRB[];
extern const char TEXT_LDK[];
extern const char TEXT_LDL[];
using common::TEXT_LDM;
extern const char TEXT_LDPS[];
extern const char TEXT_LDR[];
extern const char TEXT_LDRB[];
extern const char TEXT_LDRL[];
extern const char TEXT_MBIT[];
extern const char TEXT_MREQ[];
extern const char TEXT_MRES[];
extern const char TEXT_MSET[];
using common::TEXT_MULT;
extern const char TEXT_MULTL[];
using common::TEXT_NEG;
using common::TEXT_NEGB;
using common::TEXT_NOP;
using common::TEXT_OR;
using common::TEXT_ORB;
using common::TEXT_OTDR;
extern const char TEXT_OTDRB[];
using common::TEXT_OTIR;
extern const char TEXT_OTIRB[];
using common::TEXT_OUT;
extern const char TEXT_OUTB[];
using common::TEXT_OUTD;
extern const char TEXT_OUTDB[];
using common::TEXT_OUTI;
extern const char TEXT_OUTIB[];
using common::TEXT_POP;
extern const char TEXT_POPL[];
using common::TEXT_PUSH;
extern const char TEXT_PUSHL[];
using common::TEXT_RES;
extern const char TEXT_RESB[];
extern const char TEXT_RESFLG[];
using common::TEXT_RET;
using common::TEXT_RL;
extern const char TEXT_RLB[];
using common::TEXT_RLC;
extern const char TEXT_RLCB[];
extern const char TEXT_RLDB[];
using common::TEXT_RR;
extern const char TEXT_RRB[];
using common::TEXT_RRC;
extern const char TEXT_RRCB[];
extern const char TEXT_RRDB[];
using common::TEXT_SBC;
using common::TEXT_SBCB;
using common::TEXT_SC;
extern const char TEXT_SDA[];
extern const char TEXT_SDAB[];
extern const char TEXT_SDAL[];
extern const char TEXT_SDL[];
extern const char TEXT_SDLB[];
extern const char TEXT_SDLL[];
using common::TEXT_SET;
using common::TEXT_SETB;
extern const char TEXT_SETFLG[];
using common::TEXT_SIN;
extern const char TEXT_SINB[];
extern const char TEXT_SIND[];
extern const char TEXT_SINDB[];
extern const char TEXT_SINDR[];
extern const char TEXT_SINDRB[];
extern const char TEXT_SINI[];
extern const char TEXT_SINIB[];
extern const char TEXT_SINIR[];
extern const char TEXT_SINIRB[];
using common::TEXT_SLA;
extern const char TEXT_SLAB[];
extern const char TEXT_SLAL[];
using common::TEXT_SLL;
extern const char TEXT_SLLB[];
extern const char TEXT_SLLL[];
extern const char TEXT_SOTDR[];
extern const char TEXT_SOTDRB[];
extern const char TEXT_SOTIR[];
extern const char TEXT_SOTIRB[];
extern const char TEXT_SOUT[];
extern const char TEXT_SOUTB[];
extern const char TEXT_SOUTD[];
extern const char TEXT_SOUTDB[];
extern const char TEXT_SOUTI[];
extern const char TEXT_SOUTIB[];
using common::TEXT_SRA;
extern const char TEXT_SRAB[];
extern const char TEXT_SRAL[];
using common::TEXT_SRL;
extern const char TEXT_SRLB[];
extern const char TEXT_SRLL[];
using common::TEXT_SUB;
using common::TEXT_SUBB;
using common::TEXT_SUBL;
extern const char TEXT_TCC[];
extern const char TEXT_TCCB[];
using common::TEXT_TEST;
extern const char TEXT_TESTB[];
extern const char TEXT_TESTL[];
extern const char TEXT_TRDB[];
extern const char TEXT_TRDRB[];
extern const char TEXT_TRIB[];
extern const char TEXT_TRIRB[];
extern const char TEXT_TRTDB[];
extern const char TEXT_TRTDRB[];
extern const char TEXT_TRTIB[];
extern const char TEXT_TRTIRB[];
using common::TEXT_TSET;
extern const char TEXT_TSETB[];
using common::TEXT_XOR;
using common::TEXT_XORB;

extern const char TEXT_REG_FCW[];
extern const char TEXT_REG_FLAGS[];
using common::TEXT_REG_NSP;
extern const char TEXT_REG_NSPOFF[];
extern const char TEXT_REG_NSPSEG[];
extern const char TEXT_REG_PSAP[];
extern const char TEXT_REG_PSAPOFF[];
extern const char TEXT_REG_PSAPSEG[];
extern const char TEXT_REG_REFRESH[];
using common::TEXT_REG_RH;
using common::TEXT_REG_RL;
extern const char TEXT_REG_RQ[];
using common::TEXT_REG_RR;
using common::TEXT_REG_SP;

using common::TEXT_CC_C;
using common::TEXT_CC_EQ;
using common::TEXT_CC_F;
using common::TEXT_CC_GE;
using common::TEXT_CC_GT;
using common::TEXT_CC_LE;
using common::TEXT_CC_LT;
using common::TEXT_CC_MI;
using common::TEXT_CC_NC;
using common::TEXT_CC_NE;
using common::TEXT_CC_NOV;
using common::TEXT_CC_NZ;
using common::TEXT_CC_OV;
using common::TEXT_CC_PL;
using common::TEXT_CC_UGE;
using common::TEXT_CC_UGT;
using common::TEXT_CC_ULE;
using common::TEXT_CC_ULT;
using common::TEXT_CC_Z;

extern const char TEXT_FLAG_C[];
extern const char TEXT_FLAG_P[];
extern const char TEXT_FLAG_S[];
extern const char TEXT_FLAG_V[];
extern const char TEXT_FLAG_Z[];

extern const char TEXT_INTR_NVI[];
extern const char TEXT_INTR_VI[];

// clang-format on
}  // namespace z8000
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_Z8000__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
