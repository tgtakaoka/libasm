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

extern const char TEXT_CPU_LIST[]    PROGMEM;
extern const char TEXT_CPU_8096[]    PROGMEM;
extern const char TEXT_CPU_80196[]   PROGMEM;
extern const char TEXT_CPU_80196KB[] PROGMEM;

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
extern const char TEXT_CLRVT[] PROGMEM;
using common::TEXT_CMP;
using common::TEXT_CMPB;
using common::TEXT_CPU;
using common::TEXT_DEC;
using common::TEXT_DECB;
using common::TEXT_DI;
using common::TEXT_DIV;
using common::TEXT_DIVB;
using common::TEXT_DIVU;
extern const char TEXT_DIVUB[] PROGMEM;
using common::TEXT_DJNZ;
using common::TEXT_EI;
using common::TEXT_EXT;
using common::TEXT_EXTB;
using common::TEXT_INC;
using common::TEXT_INCB;
using common::TEXT_JBC;
extern const char TEXT_JBS[]   PROGMEM;
using common::TEXT_JC;
using common::TEXT_JE;
using common::TEXT_JGE;
using common::TEXT_JGT;
using common::TEXT_JH;
using common::TEXT_JLE;
using common::TEXT_JLT;
using common::TEXT_JNC;
using common::TEXT_JNE;
extern const char TEXT_JNH[]   PROGMEM;
extern const char TEXT_JNST[]  PROGMEM;
using common::TEXT_JNV;
extern const char TEXT_JNVT[]  PROGMEM;
extern const char TEXT_JST[]   PROGMEM;
using common::TEXT_JV;
extern const char TEXT_JVT[]   PROGMEM;
using common::TEXT_LCALL;
using common::TEXT_LD;
using common::TEXT_LDB;
extern const char TEXT_LDBSE[] PROGMEM;
extern const char TEXT_LDBZE[] PROGMEM;
using common::TEXT_LJMP;
using common::TEXT_MUL;
using common::TEXT_MULB;
using common::TEXT_MULU;
extern const char TEXT_MULUB[] PROGMEM;
using common::TEXT_NEG;
using common::TEXT_NEGB;
using common::TEXT_NOP;
extern const char TEXT_NORML[] PROGMEM;
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
using common::TEXT_RST;
extern const char TEXT_SCALL[] PROGMEM;
using common::TEXT_SETC;
using common::TEXT_SHL;
extern const char TEXT_SHLB[]  PROGMEM;
extern const char TEXT_SHLL[]  PROGMEM;
using common::TEXT_SHR;
extern const char TEXT_SHRA[]  PROGMEM;
extern const char TEXT_SHRAB[] PROGMEM;
extern const char TEXT_SHRAL[] PROGMEM;
extern const char TEXT_SHRB[]  PROGMEM;
extern const char TEXT_SHRL[]  PROGMEM;
using common::TEXT_SJMP;
extern const char TEXT_SKIP[]  PROGMEM;
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
extern const char TEXT_BMOV[]  PROGMEM;
using common::TEXT_CMPL;
extern const char TEXT_DJNZW[] PROGMEM;
extern const char TEXT_IDLPD[] PROGMEM;
using common::TEXT_POPA;
using common::TEXT_PUSHA;

// i80C196KC
extern const char TEXT_BMOVI[] PROGMEM;
extern const char TEXT_DPTS[]  PROGMEM;
extern const char TEXT_EPTS[]  PROGMEM;
extern const char TEXT_TIJMP[] PROGMEM;
using common::TEXT_XCH;
using common::TEXT_XCHB;

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
