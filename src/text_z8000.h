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

extern const char TEXT_Z8000_LIST[] PROGMEM;
extern const char TEXT_CPU_Z8001[]  PROGMEM;
extern const char TEXT_CPU_Z8002[]  PROGMEM;

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
extern const char TEXT_COMFLG[] PROGMEM;
using common::TEXT_CP;
extern const char TEXT_CPB[]    PROGMEM;
using common::TEXT_CPD;
extern const char TEXT_CPDB[]   PROGMEM;
using common::TEXT_CPDR;
extern const char TEXT_CPDRB[]  PROGMEM;
using common::TEXT_CPI;
extern const char TEXT_CPIB[]   PROGMEM;
using common::TEXT_CPIR;
extern const char TEXT_CPIRB[]  PROGMEM;
using common::TEXT_CPL;
extern const char TEXT_CPSD[]   PROGMEM;
extern const char TEXT_CPSDB[]  PROGMEM;
extern const char TEXT_CPSDR[]  PROGMEM;
extern const char TEXT_CPSDRB[] PROGMEM;
extern const char TEXT_CPSI[]   PROGMEM;
extern const char TEXT_CPSIB[]  PROGMEM;
extern const char TEXT_CPSIR[]  PROGMEM;
extern const char TEXT_CPSIRB[] PROGMEM;
extern const char TEXT_DAB[]    PROGMEM;
extern const char TEXT_DBJNZ[]  PROGMEM;
using common::TEXT_DEC;
using common::TEXT_DECB;
using common::TEXT_DI;
using common::TEXT_DIV;
using common::TEXT_DIVL;
using common::TEXT_DJNZ;
using common::TEXT_EI;
using common::TEXT_EX;
extern const char TEXT_EXB[]    PROGMEM;
using common::TEXT_EXTS;
using common::TEXT_EXTSB;
extern const char TEXT_EXTSL[]  PROGMEM;
using common::TEXT_HALT;
using common::TEXT_IN;
extern const char TEXT_INB[]    PROGMEM;
using common::TEXT_INC;
using common::TEXT_INCB;
using common::TEXT_IND;
extern const char TEXT_INDB[]   PROGMEM;
using common::TEXT_INDR;
extern const char TEXT_INDRB[]  PROGMEM;
using common::TEXT_INI;
extern const char TEXT_INIB[]   PROGMEM;
using common::TEXT_INIR;
extern const char TEXT_INIRB[]  PROGMEM;
using common::TEXT_IRET;
using common::TEXT_JP;
using common::TEXT_JR;
using common::TEXT_LD;
using common::TEXT_LDA;
using common::TEXT_LDAR;
using common::TEXT_LDB;
using common::TEXT_LDCTL;
extern const char TEXT_LDCTLB[] PROGMEM;
using common::TEXT_LDD;
extern const char TEXT_LDDB[]   PROGMEM;
using common::TEXT_LDDR;
extern const char TEXT_LDDRB[]  PROGMEM;
using common::TEXT_LDI;
extern const char TEXT_LDIB[]   PROGMEM;
using common::TEXT_LDIR;
extern const char TEXT_LDIRB[]  PROGMEM;
extern const char TEXT_LDK[]    PROGMEM;
extern const char TEXT_LDL[]    PROGMEM;
using common::TEXT_LDM;
extern const char TEXT_LDPS[]   PROGMEM;
extern const char TEXT_LDR[]    PROGMEM;
extern const char TEXT_LDRB[]   PROGMEM;
extern const char TEXT_LDRL[]   PROGMEM;
extern const char TEXT_MBIT[]   PROGMEM;
extern const char TEXT_MREQ[]   PROGMEM;
extern const char TEXT_MRES[]   PROGMEM;
extern const char TEXT_MSET[]   PROGMEM;
using common::TEXT_MULT;
extern const char TEXT_MULTL[]  PROGMEM;
using common::TEXT_NEG;
using common::TEXT_NEGB;
using common::TEXT_NOP;
using common::TEXT_OR;
using common::TEXT_ORB;
using common::TEXT_OTDR;
extern const char TEXT_OTDRB[]  PROGMEM;
using common::TEXT_OTIR;
extern const char TEXT_OTIRB[]  PROGMEM;
using common::TEXT_OUT;
extern const char TEXT_OUTB[]   PROGMEM;
using common::TEXT_OUTD;
extern const char TEXT_OUTDB[]  PROGMEM;
using common::TEXT_OUTI;
extern const char TEXT_OUTIB[]  PROGMEM;
using common::TEXT_POP;
extern const char TEXT_POPL[]   PROGMEM;
using common::TEXT_PUSH;
extern const char TEXT_PUSHL[]  PROGMEM;
using common::TEXT_RES;
extern const char TEXT_RESB[]   PROGMEM;
extern const char TEXT_RESFLG[] PROGMEM;
using common::TEXT_RET;
using common::TEXT_RL;
extern const char TEXT_RLB[]    PROGMEM;
using common::TEXT_RLC;
extern const char TEXT_RLCB[]   PROGMEM;
extern const char TEXT_RLDB[]   PROGMEM;
using common::TEXT_RR;
extern const char TEXT_RRB[]    PROGMEM;
using common::TEXT_RRC;
extern const char TEXT_RRCB[]   PROGMEM;
extern const char TEXT_RRDB[]   PROGMEM;
using common::TEXT_SBC;
using common::TEXT_SBCB;
using common::TEXT_SC;
extern const char TEXT_SDA[]    PROGMEM;
extern const char TEXT_SDAB[]   PROGMEM;
extern const char TEXT_SDAL[]   PROGMEM;
extern const char TEXT_SDL[]    PROGMEM;
extern const char TEXT_SDLB[]   PROGMEM;
extern const char TEXT_SDLL[]   PROGMEM;
using common::TEXT_SET;
using common::TEXT_SETB;
extern const char TEXT_SETFLG[] PROGMEM;
extern const char TEXT_SIN[]    PROGMEM;
extern const char TEXT_SINB[]   PROGMEM;
extern const char TEXT_SIND[]   PROGMEM;
extern const char TEXT_SINDB[]  PROGMEM;
extern const char TEXT_SINDR[]  PROGMEM;
extern const char TEXT_SINDRB[] PROGMEM;
extern const char TEXT_SINI[]   PROGMEM;
extern const char TEXT_SINIB[]  PROGMEM;
extern const char TEXT_SINIR[]  PROGMEM;
extern const char TEXT_SINIRB[] PROGMEM;
using common::TEXT_SLA;
extern const char TEXT_SLAB[]   PROGMEM;
extern const char TEXT_SLAL[]   PROGMEM;
using common::TEXT_SLL;
extern const char TEXT_SLLB[]   PROGMEM;
extern const char TEXT_SLLL[]   PROGMEM;
extern const char TEXT_SOTDR[]  PROGMEM;
extern const char TEXT_SOTDRB[] PROGMEM;
extern const char TEXT_SOTIR[]  PROGMEM;
extern const char TEXT_SOTIRB[] PROGMEM;
extern const char TEXT_SOUT[]   PROGMEM;
extern const char TEXT_SOUTB[]  PROGMEM;
extern const char TEXT_SOUTD[]  PROGMEM;
extern const char TEXT_SOUTDB[] PROGMEM;
extern const char TEXT_SOUTI[]  PROGMEM;
extern const char TEXT_SOUTIB[] PROGMEM;
using common::TEXT_SRA;
extern const char TEXT_SRAB[]   PROGMEM;
extern const char TEXT_SRAL[]   PROGMEM;
using common::TEXT_SRL;
extern const char TEXT_SRLB[]   PROGMEM;
extern const char TEXT_SRLL[]   PROGMEM;
using common::TEXT_SUB;
using common::TEXT_SUBB;
using common::TEXT_SUBL;
extern const char TEXT_TCC[]    PROGMEM;
extern const char TEXT_TCCB[]   PROGMEM;
using common::TEXT_TEST;
extern const char TEXT_TESTB[]  PROGMEM;
extern const char TEXT_TESTL[]  PROGMEM;
extern const char TEXT_TRDB[]   PROGMEM;
extern const char TEXT_TRDRB[]  PROGMEM;
extern const char TEXT_TRIB[]   PROGMEM;
extern const char TEXT_TRIRB[]  PROGMEM;
extern const char TEXT_TRTDB[]  PROGMEM;
extern const char TEXT_TRTDRB[] PROGMEM;
extern const char TEXT_TRTIB[]  PROGMEM;
extern const char TEXT_TRTIRB[] PROGMEM;
using common::TEXT_TSET;
extern const char TEXT_TSETB[]  PROGMEM;
using common::TEXT_XOR;
using common::TEXT_XORB;

extern const char TEXT_REG_FCW[]     PROGMEM;
extern const char TEXT_REG_FLAGS[]   PROGMEM;
extern const char TEXT_REG_NSP[]     PROGMEM;
extern const char TEXT_REG_NSPOFF[]  PROGMEM;
extern const char TEXT_REG_NSPSEG[]  PROGMEM;
extern const char TEXT_REG_PSAP[]    PROGMEM;
extern const char TEXT_REG_PSAPOFF[] PROGMEM;
extern const char TEXT_REG_PSAPSEG[] PROGMEM;
extern const char TEXT_REG_REFRESH[] PROGMEM;
extern const char TEXT_REG_RH[]      PROGMEM;
extern const char TEXT_REG_RL[]      PROGMEM;
extern const char TEXT_REG_RQ[]      PROGMEM;
using common::TEXT_REG_RR;

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

extern const char TEXT_FLAG_C[] PROGMEM;
extern const char TEXT_FLAG_P[] PROGMEM;
extern const char TEXT_FLAG_S[] PROGMEM;
extern const char TEXT_FLAG_V[] PROGMEM;
extern const char TEXT_FLAG_Z[] PROGMEM;

extern const char TEXT_INTR_NVI[] PROGMEM;
extern const char TEXT_INTR_VI[]  PROGMEM;

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
