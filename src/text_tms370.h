/*
 * Copyright 2025 Tadashi G. Takaoka
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

#ifndef __LIBASM_TEXT_TMS370__
#define __LIBASM_TEXT_TMS370__

#include "text_common.h"

namespace libasm {
namespace text {
namespace tms370 {
// clang-format off

extern const char TEXT_CPU_TMS370[]  PROGMEM;
extern const char TEXT_CPU_TMS370C[] PROGMEM;

using common::TEXT_ADC;
using common::TEXT_ADD;
using common::TEXT_AND;
using common::TEXT_BR;
using common::TEXT_BTJO;
using common::TEXT_BTJZ;
using common::TEXT_CALL;
extern const char TEXT_CALLR[] PROGMEM;
using common::TEXT_CLR;
using common::TEXT_CLRC;
using common::TEXT_CMP;
extern const char TEXT_COMPL[] PROGMEM;
using common::TEXT_DAC;
using common::TEXT_DEC;
using common::TEXT_DINT;
using common::TEXT_DIV;
using common::TEXT_DJNZ;
using common::TEXT_DSB;
using common::TEXT_EINT;
extern const char TEXT_EINTH[] PROGMEM;
extern const char TEXT_EINTL[] PROGMEM;
using common::TEXT_IDLE;
using common::TEXT_INC;
using common::TEXT_INCW;
using common::TEXT_INV;
using common::TEXT_JC;
using common::TEXT_JEQ;
using common::TEXT_JG;
using common::TEXT_JGE;
using common::TEXT_JGT;
using common::TEXT_JHS;
using common::TEXT_JL;
using common::TEXT_JLE;
extern const char TEXT_JLO[]   PROGMEM;
using common::TEXT_JLT;
using common::TEXT_JMP;
using common::TEXT_JMPL;
using common::TEXT_JN;
using common::TEXT_JNC;
using common::TEXT_JNE;
using common::TEXT_JNV;
using common::TEXT_JNZ;
using common::TEXT_JP;
using common::TEXT_JPZ;
using common::TEXT_JV;
using common::TEXT_JZ;
using common::TEXT_LDSP;
extern const char TEXT_LDST[]  PROGMEM;
using common::TEXT_MOV;
using common::TEXT_MOVW;
using common::TEXT_MPY;
using common::TEXT_NOP;
using common::TEXT_OR;
using common::TEXT_POP;
using common::TEXT_PUSH;
using common::TEXT_RL;
using common::TEXT_RLC;
using common::TEXT_RR;
using common::TEXT_RRC;
using common::TEXT_RTI;
using common::TEXT_RTS;
using common::TEXT_SBB;
using common::TEXT_SETC;
using common::TEXT_STSP;
using common::TEXT_SUB;
using common::TEXT_SWAP;
using common::TEXT_TRAP;
using common::TEXT_TST;
using common::TEXT_XCHB;
using common::TEXT_XOR;

using common::TEXT_REG_A;
using common::TEXT_REG_B;
using common::TEXT_REG_SP;
using common::TEXT_REG_ST;

// clang-format on
}  // namespace tms370
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_TMS370__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
