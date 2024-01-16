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

#ifndef __LIBASM_TEXT_I8051__
#define __LIBASM_TEXT_I8051__

#include "text_common.h"

namespace libasm {
namespace text {
namespace i8051 {

// clang-format off
extern const char TEXT_CPU_I8051[] PROGMEM;
extern const char TEXT_CPU_8051[]  PROGMEM;

extern const char TEXT_ACALL[] PROGMEM;
using common::TEXT_ADD;
using common::TEXT_ADDC;
extern const char TEXT_AJMP[]  PROGMEM;
using common::TEXT_ANL;
extern const char TEXT_CJNE[]  PROGMEM;
using common::TEXT_CLR;
using common::TEXT_CPL;
using common::TEXT_CPU;
using common::TEXT_DA;
using common::TEXT_DEC;
using common::TEXT_DIV;
using common::TEXT_DJNZ;
using common::TEXT_INC;
using common::TEXT_JB;
using common::TEXT_JBC;
using common::TEXT_JC;
using common::TEXT_JMP;
using common::TEXT_JNB;
using common::TEXT_JNC;
using common::TEXT_JNZ;
using common::TEXT_JZ;
using common::TEXT_LCALL;
using common::TEXT_LJMP;
using common::TEXT_MOV;
extern const char TEXT_MOVC[]  PROGMEM;
using common::TEXT_MOVX;
using common::TEXT_MUL;
using common::TEXT_NOP;
using common::TEXT_ORL;
using common::TEXT_POP;
using common::TEXT_PUSH;
using common::TEXT_RET;
using common::TEXT_RETI;
using common::TEXT_RL;
using common::TEXT_RLC;
using common::TEXT_RR;
using common::TEXT_RRC;
using common::TEXT_SETB;
using common::TEXT_SJMP;
using common::TEXT_SUBB;
using common::TEXT_SWAP;
using common::TEXT_XCH;
using common::TEXT_XCHD;
using common::TEXT_XRL;

using common::TEXT_REG_A;
using common::TEXT_REG_C;
extern const char TEXT_REG_AB[]   PROGMEM;
using common::TEXT_REG_R0;
using common::TEXT_REG_R1;
using common::TEXT_REG_R2;
using common::TEXT_REG_R3;
using common::TEXT_REG_R4;
using common::TEXT_REG_R5;
using common::TEXT_REG_R6;
using common::TEXT_REG_R7;
using common::TEXT_REG_PC;
extern const char TEXT_REG_DPTR[] PROGMEM;
// clang-format on

}  // namespace i8051
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_I8051__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
