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

#ifndef __LIBASM_TEXT_Z8__
#define __LIBASM_TEXT_Z8__

#include "text_common.h"

namespace libasm {
namespace text {
namespace z8 {
// clang-format off

extern const char TEXT_Z8_LIST[]    PROGMEM;
extern const char TEXT_CPU_Z8[]     PROGMEM;
extern const char TEXT_CPU_Z86[]    PROGMEM;
extern const char TEXT_CPU_Z86C[]   PROGMEM;
extern const char TEXT_CPU_Z88[]    PROGMEM;
extern const char TEXT_CPU_SUPER8[] PROGMEM;

// Z8
using common::TEXT_ADC;
using common::TEXT_ADD;
using common::TEXT_AND;
using common::TEXT_CALL;
using common::TEXT_CCF;
using common::TEXT_CLR;
using common::TEXT_COM;
using common::TEXT_CP;
using common::TEXT_DA;
using common::TEXT_DEC;
using common::TEXT_DECW;
using common::TEXT_DI;
using common::TEXT_DJNZ;
using common::TEXT_EI;
using common::TEXT_INC;
using common::TEXT_INCW;
using common::TEXT_IRET;
using common::TEXT_JP;
using common::TEXT_JR;
using common::TEXT_LD;
using common::TEXT_LDC;
extern const char TEXT_LDCI[] PROGMEM;
using common::TEXT_LDE;
extern const char TEXT_LDEI[] PROGMEM;
using common::TEXT_NOP;
using common::TEXT_OR;
using common::TEXT_POP;
using common::TEXT_PUSH;
using common::TEXT_RCF;
using common::TEXT_RET;
using common::TEXT_RL;
using common::TEXT_RLC;
using common::TEXT_RR;
using common::TEXT_RRC;
using common::TEXT_SBC;
using common::TEXT_SCF;
using common::TEXT_SRA;
extern const char TEXT_SRP[]  PROGMEM;
using common::TEXT_SUB;
using common::TEXT_SWAP;
extern const char TEXT_TCM[]  PROGMEM;
extern const char TEXT_TM[]   PROGMEM;
using common::TEXT_XOR;

// Z86C
using common::TEXT_HALT;
using common::TEXT_STOP;

// SUPER8
using common::TEXT_BAND;
extern const char TEXT_BCP[]    PROGMEM;
extern const char TEXT_BITC[]   PROGMEM;
extern const char TEXT_BITR[]   PROGMEM;
extern const char TEXT_BITS[]   PROGMEM;
using common::TEXT_BOR;
extern const char TEXT_BTJRF[]  PROGMEM;
extern const char TEXT_BTJRT[]  PROGMEM;
extern const char TEXT_BXOR[]   PROGMEM;
extern const char TEXT_CPIJE[]  PROGMEM;
extern const char TEXT_CPIJNE[] PROGMEM;
using common::TEXT_DIV;
using common::TEXT_ENTER;
using common::TEXT_EXIT;
using common::TEXT_LDB;
extern const char TEXT_LDCD[]   PROGMEM;
extern const char TEXT_LDCPD[]  PROGMEM;
extern const char TEXT_LDCPI[]  PROGMEM;
using common::TEXT_LDED;
extern const char TEXT_LDEPD[]  PROGMEM;
extern const char TEXT_LDEPI[]  PROGMEM;
using common::TEXT_LDW;
using common::TEXT_MULT;
extern const char TEXT_NEXT[]   PROGMEM;
extern const char TEXT_POPUD[]  PROGMEM;
extern const char TEXT_POPUI[]  PROGMEM;
extern const char TEXT_PUSHUD[] PROGMEM;
extern const char TEXT_PUSHUI[] PROGMEM;
extern const char TEXT_SB0[]    PROGMEM;
extern const char TEXT_SB1[]    PROGMEM;
extern const char TEXT_SRP0[]   PROGMEM;
extern const char TEXT_SRP1[]   PROGMEM;
extern const char TEXT_WFI[]    PROGMEM;

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

using common::TEXT_REG_RR;

// clang-format on
}  // namespace z8
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_Z8__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
