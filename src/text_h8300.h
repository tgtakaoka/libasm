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

extern const char TEXT_H8300_LIST[];
extern const char TEXT_CPU_H8300[];
extern const char TEXT_CPU_H8300H[];
extern const char TEXT_CPU_H8S2000[];
extern const char TEXT_CPU_H8S2600[];

using common::TEXT_ADD;
using common::TEXT_ADDS;
using common::TEXT_ADDX;
using common::TEXT_AND;
using common::TEXT_ANDC;
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
extern const char TEXT_BILD[];
using common::TEXT_BIOR;
extern const char TEXT_BIST[];
extern const char TEXT_BIXOR[];
using common::TEXT_BLD;
using common::TEXT_BLE;
using common::TEXT_BLO;
using common::TEXT_BLS;
using common::TEXT_BLT;
using common::TEXT_BMI;
using common::TEXT_BNE;
using common::TEXT_BNOT;
using common::TEXT_BOR;
using common::TEXT_BPL;
using common::TEXT_BRA;
using common::TEXT_BRN;
using common::TEXT_BSET;
using common::TEXT_BSR;
using common::TEXT_BST;
using common::TEXT_BT;
using common::TEXT_BTST;
using common::TEXT_BVC;
using common::TEXT_BVS;
using common::TEXT_BXOR;
using common::TEXT_CLRMAC;
using common::TEXT_CMP;
using common::TEXT_DAA;
using common::TEXT_DAS;
using common::TEXT_DEC;
using common::TEXT_DIVXU;
extern const char TEXT_DIVXS[];
extern const char TEXT_EEPMOV[];
using common::TEXT_EXTS;
using common::TEXT_EXTU;
using common::TEXT_INC;
using common::TEXT_JMP;
using common::TEXT_JSR;
using common::TEXT_LDC;
using common::TEXT_LDM;
extern const char TEXT_LDMAC[];
using common::TEXT_MAC;
using common::TEXT_MOV;
using common::TEXT_MOVFPE;
using common::TEXT_MOVTPE;
using common::TEXT_MULXU;
extern const char TEXT_MULXS[];
using common::TEXT_NEG;
using common::TEXT_NOP;
using common::TEXT_NOT;
using common::TEXT_OR;
using common::TEXT_ORC;
using common::TEXT_POP;
using common::TEXT_PUSH;
using common::TEXT_ROTL;
using common::TEXT_ROTR;
using common::TEXT_ROTXL;
using common::TEXT_ROTXR;
using common::TEXT_RTE;
using common::TEXT_RTS;
using common::TEXT_SHAL;
using common::TEXT_SHAR;
using common::TEXT_SHLL;
using common::TEXT_SHLR;
using common::TEXT_SLEEP;
using common::TEXT_STC;
using common::TEXT_STM;
extern const char TEXT_STMAC[];
using common::TEXT_SUB;
using common::TEXT_SUBS;
using common::TEXT_SUBX;
using common::TEXT_TAS;
using common::TEXT_TRAPA;
using common::TEXT_XOR;
using common::TEXT_XORC;

using common::TEXT_REG_CCR;
using common::TEXT_REG_R0;
extern const char TEXT_REG_R0H[];
extern const char TEXT_REG_R0L[];
using common::TEXT_REG_R1;
extern const char TEXT_REG_R1H[];
extern const char TEXT_REG_R1L[];
using common::TEXT_REG_R2;
extern const char TEXT_REG_R2H[];
extern const char TEXT_REG_R2L[];
using common::TEXT_REG_R3;
extern const char TEXT_REG_R3H[];
extern const char TEXT_REG_R3L[];
using common::TEXT_REG_R4;
extern const char TEXT_REG_R4H[];
extern const char TEXT_REG_R4L[];
using common::TEXT_REG_R5;
extern const char TEXT_REG_R5H[];
extern const char TEXT_REG_R5L[];
using common::TEXT_REG_R6;
extern const char TEXT_REG_R6H[];
extern const char TEXT_REG_R6L[];
using common::TEXT_REG_R7;
extern const char TEXT_REG_R7H[];
extern const char TEXT_REG_R7L[];
using common::TEXT_REG_SP;

// H8/300H 16-bit upper-half registers (E0..E7)
extern const char TEXT_REG_E0[];
extern const char TEXT_REG_E1[];
extern const char TEXT_REG_E2[];
extern const char TEXT_REG_E3[];
extern const char TEXT_REG_E4[];
extern const char TEXT_REG_E5[];
extern const char TEXT_REG_E6[];
extern const char TEXT_REG_E7[];

// H8/300H 32-bit extended registers (ER0..ER7)
extern const char TEXT_REG_ER0[];
extern const char TEXT_REG_ER1[];
extern const char TEXT_REG_ER2[];
extern const char TEXT_REG_ER3[];
extern const char TEXT_REG_ER4[];
extern const char TEXT_REG_ER5[];
extern const char TEXT_REG_ER6[];
extern const char TEXT_REG_ER7[];

extern const char TEXT_REG_EXR[];
using common::TEXT_REG_MACH;
using common::TEXT_REG_MACL;

// Options
extern const char OPT_BOOL_ADVANCED_MODE[];
extern const char OPT_DESC_ADVANCED_MODE[];
extern const char OPT_BOOL_SP_ALIAS[];
extern const char OPT_DESC_SP_ALIAS[];

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
