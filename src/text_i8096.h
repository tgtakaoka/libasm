/*
 * Copyright 2022 Tadashi G. Takaoka
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

#ifndef __LIBASM_TEXT_I8096__
#define __LIBASM_TEXT_I8096__

#include "text_common.h"

namespace libasm {
namespace text {
namespace i8096 {
// clang-format off

extern const char TEXT_CPU_LIST[];
extern const char TEXT_CPU_8096[];
extern const char TEXT_CPU_80196[];
extern const char TEXT_CPU_80296[];
extern const char TEXT_CPU_80196N[];
extern const char TEXT_CPU_80196KB[];

using common::TEXT_ADD;
using common::TEXT_ADDB;
using common::TEXT_ADDC;
using common::TEXT_ADDCB;
using common::TEXT_AND;
using common::TEXT_ANDB;
using common::TEXT_BR;
using common::TEXT_CLR;
using common::TEXT_CLRB;
using common::TEXT_CLRC;
extern const char TEXT_CLRVT[];
using common::TEXT_CMP;
using common::TEXT_CMPB;
using common::TEXT_CPU;
using common::TEXT_DEC;
using common::TEXT_DECB;
using common::TEXT_DI;
using common::TEXT_DIV;
using common::TEXT_DIVB;
using common::TEXT_DIVU;
extern const char TEXT_DIVUB[];
using common::TEXT_DJNZ;
using common::TEXT_EI;
using common::TEXT_EXT;
using common::TEXT_EXTB;
using common::TEXT_INC;
using common::TEXT_INCB;
using common::TEXT_JBC;
extern const char TEXT_JBS[];
using common::TEXT_JC;
using common::TEXT_JE;
using common::TEXT_JGE;
using common::TEXT_JGT;
using common::TEXT_JH;
using common::TEXT_JLE;
using common::TEXT_JLT;
using common::TEXT_JNC;
using common::TEXT_JNE;
extern const char TEXT_JNH[];
extern const char TEXT_JNST[];
using common::TEXT_JNV;
extern const char TEXT_JNVT[];
extern const char TEXT_JST[];
using common::TEXT_JV;
extern const char TEXT_JVT[];
using common::TEXT_LCALL;
using common::TEXT_LD;
using common::TEXT_LDB;
extern const char TEXT_LDBSE[];
extern const char TEXT_LDBZE[];
using common::TEXT_LJMP;
using common::TEXT_MUL;
using common::TEXT_MULB;
using common::TEXT_MULU;
extern const char TEXT_MULUB[];
using common::TEXT_NEG;
using common::TEXT_NEGB;
using common::TEXT_NOP;
extern const char TEXT_NORML[];
using common::TEXT_NOT;
using common::TEXT_NOTB;
using common::TEXT_OR;
using common::TEXT_ORB;
using common::TEXT_ORG;
using common::TEXT_POP;
using common::TEXT_POPF;
using common::TEXT_PUSH;
using common::TEXT_PUSHF;
using common::TEXT_RET;
using common::TEXT_RETI;
using common::TEXT_RST;
extern const char TEXT_SCALL[];
using common::TEXT_SETC;
using common::TEXT_SHL;
extern const char TEXT_SHLB[];
using common::TEXT_SHLL;
using common::TEXT_SHR;
extern const char TEXT_SHRA[];
extern const char TEXT_SHRAB[];
extern const char TEXT_SHRAL[];
extern const char TEXT_SHRB[];
extern const char TEXT_SHRL[];
using common::TEXT_SJMP;
extern const char TEXT_SKIP[];
using common::TEXT_ST;
using common::TEXT_STB;
using common::TEXT_SUB;
using common::TEXT_SUBB;
using common::TEXT_SUBC;
using common::TEXT_SUBCB;
using common::TEXT_TRAP;
using common::TEXT_XOR;
using common::TEXT_XORB;

// i80C196KB
using common::TEXT_BMOV;
using common::TEXT_CMPL;
extern const char TEXT_DJNZW[];
extern const char TEXT_IDLPD[];
using common::TEXT_POPA;
using common::TEXT_PUSHA;

// i80C196KC
extern const char TEXT_BMOVI[];
extern const char TEXT_DPTS[];
extern const char TEXT_EBMOVI[];
extern const char TEXT_EBR[];
extern const char TEXT_ECALL[];
extern const char TEXT_EJMP[];
extern const char TEXT_ELD[];
extern const char TEXT_ELDB[];
extern const char TEXT_EST[];
extern const char TEXT_ESTB[];
extern const char TEXT_EPTS[];
extern const char TEXT_TIJMP[];
using common::TEXT_XCH;
using common::TEXT_XCHB;

// i80296
using common::TEXT_MAC;
extern const char TEXT_MACR[];
extern const char TEXT_MACRZ[];
extern const char TEXT_MACZ[];
extern const char TEXT_MSAC[];
extern const char TEXT_MVAC[];
extern const char TEXT_RPT[];
extern const char TEXT_RPTC[];
extern const char TEXT_RPTE[];
extern const char TEXT_RPTGE[];
extern const char TEXT_RPTGT[];
extern const char TEXT_RPTH[];
extern const char TEXT_RPTI[];
extern const char TEXT_RPTIC[];
extern const char TEXT_RPTIE[];
extern const char TEXT_RPTIGE[];
extern const char TEXT_RPTIGT[];
extern const char TEXT_RPTIH[];
extern const char TEXT_RPTILE[];
extern const char TEXT_RPTILT[];
extern const char TEXT_RPTINC[];
extern const char TEXT_RPTINE[];
extern const char TEXT_RPTINH[];
extern const char TEXT_RPTINST[];
extern const char TEXT_RPTINV[];
extern const char TEXT_RPTINVT[];
extern const char TEXT_RPTIST[];
extern const char TEXT_RPTIV[];
extern const char TEXT_RPTIVT[];
extern const char TEXT_RPTLE[];
extern const char TEXT_RPTLT[];
extern const char TEXT_RPTNC[];
extern const char TEXT_RPTNE[];
extern const char TEXT_RPTNH[];
extern const char TEXT_RPTNST[];
extern const char TEXT_RPTNV[];
extern const char TEXT_RPTNVT[];
extern const char TEXT_RPTST[];
extern const char TEXT_RPTV[];
extern const char TEXT_RPTVT[];
extern const char TEXT_SMAC[];
extern const char TEXT_SMACR[];
extern const char TEXT_SMACRZ[];
extern const char TEXT_SMACZ[];

// clang-format on
}  // namespace i8096
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_I8096__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
