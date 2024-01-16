/*
 * Copyright 2021 Tadashi G. Takaoka
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

#ifndef __LIBASM_TEXT_I8048__
#define __LIBASM_TEXT_I8048__

#include "text_common.h"

namespace libasm {
namespace text {
namespace i8048 {

// clang-format off
extern const char TEXT_I8048_LIST[]     PROGMEM;
extern const char TEXT_CPU_8039[]     PROGMEM;
extern const char TEXT_CPU_8048[]     PROGMEM;
extern const char TEXT_CPU_80C39[]    PROGMEM;
extern const char TEXT_CPU_80C48[]    PROGMEM;
extern const char TEXT_CPU_MSM80C39[] PROGMEM;
extern const char TEXT_CPU_MSM80C48[] PROGMEM;

using common::TEXT_ADD;
using common::TEXT_ADDC;
using common::TEXT_ANL;
extern const char TEXT_ANLD[] PROGMEM;
using common::TEXT_CALL;
using common::TEXT_CLR;
using common::TEXT_CPL;
using common::TEXT_DA;
using common::TEXT_DEC;
using common::TEXT_DIS;
using common::TEXT_DJNZ;
extern const char TEXT_EN[]   PROGMEM;
extern const char TEXT_ENT0[] PROGMEM;
using common::TEXT_IN;
using common::TEXT_INC;
using common::TEXT_INS;
extern const char TEXT_IRL[]  PROGMEM;
using common::TEXT_JB;
using common::TEXT_JC;
extern const char TEXT_JF0[]  PROGMEM;
extern const char TEXT_JF1[]  PROGMEM;
using common::TEXT_JMP;
extern const char TEXT_JMPP[] PROGMEM;
using common::TEXT_JNC;
extern const char TEXT_JNI[]  PROGMEM;
extern const char TEXT_JNT0[] PROGMEM;
extern const char TEXT_JNT1[] PROGMEM;
using common::TEXT_JNZ;
extern const char TEXT_JT0[]  PROGMEM;
extern const char TEXT_JT1[]  PROGMEM;
extern const char TEXT_JTF[]  PROGMEM;
using common::TEXT_JZ;
using common::TEXT_MOV;
using common::TEXT_MOVD;
using common::TEXT_MOVP;
extern const char TEXT_MOVP3[] PROGMEM;
using common::TEXT_MOVX;
using common::TEXT_NOP;
using common::TEXT_ORL;
extern const char TEXT_ORLD[] PROGMEM;
extern const char TEXT_OUTL[] PROGMEM;
using common::TEXT_RET;
extern const char TEXT_RETR[] PROGMEM;
using common::TEXT_RL;
using common::TEXT_RLC;
using common::TEXT_RR;
using common::TEXT_RRC;
extern const char TEXT_SEL[]  PROGMEM;
using common::TEXT_STOP;
extern const char TEXT_STRT[] PROGMEM;
using common::TEXT_SWAP;
using common::TEXT_XCH;
using common::TEXT_XCHD;
using common::TEXT_XRL;

// i80C48
using common::TEXT_HALT;

// MSM80C48
using common::TEXT_FLT;
extern const char TEXT_FLTT[] PROGMEM;
extern const char TEXT_FRES[] PROGMEM;
extern const char TEXT_HLTS[] PROGMEM;
extern const char TEXT_MOVP1[] PROGMEM;

// clang-format off
using common::TEXT_REG_A;
extern const char TEXT_REG_BUS[]  PROGMEM;
using common::TEXT_REG_C;
extern const char TEXT_REG_CLK[]  PROGMEM;
extern const char TEXT_REG_CNT[]  PROGMEM;
using common::TEXT_REG_F0;
using common::TEXT_REG_F1;
using common::TEXT_REG_I;
extern const char TEXT_REG_MB0[]  PROGMEM;
extern const char TEXT_REG_MB1[]  PROGMEM;
using common::TEXT_REG_P;
using common::TEXT_REG_P1;
using common::TEXT_REG_P2;
extern const char TEXT_REG_P4[]   PROGMEM;
extern const char TEXT_REG_P5[]   PROGMEM;
extern const char TEXT_REG_P6[]   PROGMEM;
extern const char TEXT_REG_P7[]   PROGMEM;
using common::TEXT_REG_PSW;
using common::TEXT_REG_R0;
using common::TEXT_REG_R1;
using common::TEXT_REG_R2;
using common::TEXT_REG_R3;
using common::TEXT_REG_R4;
using common::TEXT_REG_R5;
using common::TEXT_REG_R6;
using common::TEXT_REG_R7;
extern const char TEXT_REG_RB0[]  PROGMEM;
extern const char TEXT_REG_RB1[]  PROGMEM;
using common::TEXT_REG_T;
extern const char TEXT_REG_TCNT[] PROGMEM;
extern const char TEXT_REG_TCNTI[] PROGMEM;
// clang-format on

}  // namespace i8048
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_I8048__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
