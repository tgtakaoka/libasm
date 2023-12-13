/*
 * Copyright 2023 Tadashi G. Takaoka
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

#ifndef __LIBASM_TEXT_TMS7000__
#define __LIBASM_TEXT_TMS7000__

#include "text_common.h"

namespace libasm {
namespace text {
namespace tms7000 {

// clang-format off
extern const char TEXT_CPU_TMS7000[]  PROGMEM;
extern const char TEXT_CPU_TMS70C00[] PROGMEM;

// TMS7000
using common::TEXT_ADC;
using common::TEXT_ADD;
using common::TEXT_AND;
extern const char TEXT_ANDP[]  PROGMEM;
extern const char TEXT_BTJO[]  PROGMEM;
extern const char TEXT_BTJOP[] PROGMEM;
extern const char TEXT_BTJZ[]  PROGMEM;
extern const char TEXT_BTJZP[] PROGMEM;
using common::TEXT_BR;
using common::TEXT_CALL;
using common::TEXT_CLR;
using common::TEXT_CLRC;
using common::TEXT_CMPA;
using common::TEXT_CMP;
extern const char TEXT_DAC[]   PROGMEM;
using common::TEXT_DEC;
using common::TEXT_DECD;
using common::TEXT_DINT;
using common::TEXT_DJNZ;
using common::TEXT_DSB;
using common::TEXT_EINT;
using common::TEXT_IDLE;
using common::TEXT_INC;
using common::TEXT_INV;
extern const char TEXT_JC[]    PROGMEM;
using common::TEXT_JEQ;
using common::TEXT_JGE;
using common::TEXT_JGT;
extern const char TEXT_JHS[]   PROGMEM;
using common::TEXT_JLT;
extern const char TEXT_JL[]    PROGMEM;
using common::TEXT_JMP;
extern const char TEXT_JNC[]   PROGMEM;
extern const char TEXT_JNE[]   PROGMEM;
extern const char TEXT_JN[]    PROGMEM;
extern const char TEXT_JNZ[]   PROGMEM;
extern const char TEXT_JP[]    PROGMEM;
extern const char TEXT_JPZ[]   PROGMEM;
extern const char TEXT_JZ[]    PROGMEM;
using common::TEXT_LDA;
extern const char TEXT_LDSP[]  PROGMEM;
using common::TEXT_MOVD;
using common::TEXT_MOV;
using common::TEXT_MOVP;
using common::TEXT_MPY;
using common::TEXT_NOP;
using common::TEXT_OR;
extern const char TEXT_ORP[]   PROGMEM;
using common::TEXT_POP;
using common::TEXT_PUSH;
using common::TEXT_RETI;
extern const char TEXT_RETS[]  PROGMEM;
using common::TEXT_RL;
using common::TEXT_RLC;
using common::TEXT_RR;
using common::TEXT_RRC;
using common::TEXT_SBB;
using common::TEXT_SETC;
using common::TEXT_STA;
extern const char TEXT_STSP[] PROGMEM;
using common::TEXT_SUB;
using common::TEXT_SWAP;
using common::TEXT_TSTA;
using common::TEXT_TSTB;
using common::TEXT_TRAP;
extern const char TEXT_XCHB[] PROGMEM;
using common::TEXT_XOR;
extern const char TEXT_XORP[] PROGMEM;

using common::TEXT_REG_A;
using common::TEXT_REG_B;
using common::TEXT_REG_ST;

// clang-format on

}  // namespace tms7000
}  // namespace text
}  // namespace libasm

#endif  // __TEXT_TMS7000__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
