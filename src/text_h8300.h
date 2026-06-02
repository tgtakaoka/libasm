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

#ifndef __LIBASM_TEXT_H8300__
#define __LIBASM_TEXT_H8300__

#include "text_common.h"

namespace libasm {
namespace text {
namespace h8300 {
// clang-format off

extern const char TEXT_H8300_LIST[] PROGMEM;
extern const char TEXT_CPU_H8300[]  PROGMEM;

using common::TEXT_ADD;
using common::TEXT_ADDS;
using common::TEXT_ADDX;
using common::TEXT_AND;
extern const char TEXT_ANDC[]   PROGMEM;
using common::TEXT_BAND;
using common::TEXT_BCC;
using common::TEXT_BCLR;
using common::TEXT_BCS;
using common::TEXT_BEQ;
using common::TEXT_BF;
using common::TEXT_BGE;
using common::TEXT_BGT;
using common::TEXT_BHI;
using common::TEXT_BHS;
using common::TEXT_BIAND;
extern const char TEXT_BILD[]   PROGMEM;
using common::TEXT_BIOR;
extern const char TEXT_BIST[]   PROGMEM;
extern const char TEXT_BIXOR[]  PROGMEM;
extern const char TEXT_BLD[]    PROGMEM;
using common::TEXT_BLE;
using common::TEXT_BLO;
using common::TEXT_BLS;
using common::TEXT_BLT;
using common::TEXT_BMI;
using common::TEXT_BNE;
extern const char TEXT_BNOT[]   PROGMEM;
using common::TEXT_BOR;
using common::TEXT_BPL;
using common::TEXT_BRA;
using common::TEXT_BRN;
using common::TEXT_BSET;
using common::TEXT_BSR;
extern const char TEXT_BST[]    PROGMEM;
using common::TEXT_BT;
using common::TEXT_BTST;
using common::TEXT_BVC;
using common::TEXT_BVS;
using common::TEXT_BXOR;
using common::TEXT_CMP;
using common::TEXT_DAA;
using common::TEXT_DAS;
using common::TEXT_DEC;
extern const char TEXT_DIVXU[]  PROGMEM;
extern const char TEXT_EEPMOV[] PROGMEM;
using common::TEXT_INC;
using common::TEXT_JMP;
using common::TEXT_JSR;
using common::TEXT_LDC;
using common::TEXT_MOV;
extern const char TEXT_MOVFPE[] PROGMEM;
extern const char TEXT_MOVTPE[] PROGMEM;
extern const char TEXT_MULXU[]  PROGMEM;
using common::TEXT_NEG;
using common::TEXT_NOP;
using common::TEXT_NOT;
using common::TEXT_OR;
extern const char TEXT_ORC[]    PROGMEM;
using common::TEXT_POP;
using common::TEXT_PUSH;
extern const char TEXT_ROTL[]   PROGMEM;
extern const char TEXT_ROTR[]   PROGMEM;
extern const char TEXT_ROTXL[]  PROGMEM;
extern const char TEXT_ROTXR[]  PROGMEM;
using common::TEXT_RTE;
using common::TEXT_RTS;
extern const char TEXT_SHAL[]   PROGMEM;
extern const char TEXT_SHAR[]   PROGMEM;
using common::TEXT_SHLL;
extern const char TEXT_SHLR[]   PROGMEM;
extern const char TEXT_SLEEP[]  PROGMEM;
using common::TEXT_STC;
using common::TEXT_SUB;
using common::TEXT_SUBS;
using common::TEXT_SUBX;
using common::TEXT_XOR;
extern const char TEXT_XORC[]   PROGMEM;

using common::TEXT_REG_CCR;
using common::TEXT_REG_R0;
extern const char TEXT_REG_R0H[] PROGMEM;
extern const char TEXT_REG_R0L[] PROGMEM;
using common::TEXT_REG_R1;
extern const char TEXT_REG_R1H[] PROGMEM;
extern const char TEXT_REG_R1L[] PROGMEM;
using common::TEXT_REG_R2;
extern const char TEXT_REG_R2H[] PROGMEM;
extern const char TEXT_REG_R2L[] PROGMEM;
using common::TEXT_REG_R3;
extern const char TEXT_REG_R3H[] PROGMEM;
extern const char TEXT_REG_R3L[] PROGMEM;
using common::TEXT_REG_R4;
extern const char TEXT_REG_R4H[] PROGMEM;
extern const char TEXT_REG_R4L[] PROGMEM;
using common::TEXT_REG_R5;
extern const char TEXT_REG_R5H[] PROGMEM;
extern const char TEXT_REG_R5L[] PROGMEM;
using common::TEXT_REG_R6;
extern const char TEXT_REG_R6H[] PROGMEM;
extern const char TEXT_REG_R6L[] PROGMEM;
using common::TEXT_REG_R7;
extern const char TEXT_REG_R7H[] PROGMEM;
extern const char TEXT_REG_R7L[] PROGMEM;
using common::TEXT_REG_SP;

// Options
extern const char OPT_BOOL_SP_ALIAS[] PROGMEM;
extern const char OPT_DESC_SP_ALIAS[] PROGMEM;

// clang-format on
}  // namespace h8300
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_H8300__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
