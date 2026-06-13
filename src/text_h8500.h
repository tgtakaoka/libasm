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

#ifndef __LIBASM_TEXT_H8500__
#define __LIBASM_TEXT_H8500__

#include "text_common.h"
#include "text_h8300.h"

namespace libasm {
namespace text {
namespace h8500 {
// clang-format off

extern const char TEXT_H8500_LIST[] PROGMEM;
extern const char TEXT_CPU_H8500[]  PROGMEM;
extern const char TEXT_CPU_H8520[]  PROGMEM;

extern const char OPT_BOOL_FP_ALIAS[] PROGMEM;
extern const char OPT_DESC_FP_ALIAS[] PROGMEM;
extern const char OPT_BOOL_SP_ALIAS[] PROGMEM;
extern const char OPT_DESC_SP_ALIAS[] PROGMEM;

using common::TEXT_MAXMODE;

// Instructions reused from common
using common::TEXT_ADD;
using common::TEXT_ADDS;
using common::TEXT_ADDX;
using common::TEXT_AND;
using common::TEXT_BCLR;
using common::TEXT_BCC;
using common::TEXT_BCS;
using common::TEXT_BEQ;
using common::TEXT_BF;
using common::TEXT_BGE;
using common::TEXT_BGT;
using common::TEXT_BHI;
using common::TEXT_BHS;
using common::TEXT_BLE;
using common::TEXT_BLO;
using common::TEXT_BLS;
using common::TEXT_BLT;
using common::TEXT_BMI;
using common::TEXT_BNE;
using common::TEXT_BPL;
using common::TEXT_BRA;
using common::TEXT_BRN;
using common::TEXT_BSR;
using common::TEXT_BSET;
using common::TEXT_BT;
using common::TEXT_BTST;
using common::TEXT_BVC;
using common::TEXT_BVS;
using common::TEXT_CLR;
using common::TEXT_EXTS;
using common::TEXT_JMP;
using common::TEXT_JSR;
using common::TEXT_LDC;
using common::TEXT_LDM;
using common::TEXT_LINK;
using common::TEXT_MOV;
using common::TEXT_NEG;
using common::TEXT_NOP;
using common::TEXT_NOT;
using common::TEXT_OR;
using common::TEXT_RTE;
using common::TEXT_RTS;
using common::TEXT_SHLL;
using common::TEXT_STC;
using common::TEXT_STM;
using common::TEXT_SUB;
using common::TEXT_SUBS;
using common::TEXT_SUBX;
using common::TEXT_SWAP;
using common::TEXT_TAS;
using common::TEXT_TST;
using common::TEXT_UNLK;
using common::TEXT_XCH;
using common::TEXT_XOR;

// Instructions reused from h8300
using h8300::TEXT_ANDC;
using h8300::TEXT_BNOT;
using h8300::TEXT_DIVXU;
using h8300::TEXT_EXTU;
using h8300::TEXT_MOVFPE;
using h8300::TEXT_MOVTPE;
using h8300::TEXT_MULXU;
using h8300::TEXT_ORC;
using h8300::TEXT_ROTL;
using h8300::TEXT_ROTR;
using h8300::TEXT_ROTXL;
using h8300::TEXT_ROTXR;
using h8300::TEXT_SHAL;
using h8300::TEXT_SHAR;
using h8300::TEXT_SHLR;
using h8300::TEXT_SLEEP;
using h8300::TEXT_TRAPA;
using h8300::TEXT_XORC;

// H8/500-specific instruction mnemonics
extern const char TEXT_ADD_G[]   PROGMEM;  // "ADD:G"
extern const char TEXT_ADD_Q[]   PROGMEM;  // "ADD:Q"
extern const char TEXT_CMP_E[]   PROGMEM;  // "CMP:E"
extern const char TEXT_CMP_G[]   PROGMEM;  // "CMP:G"
extern const char TEXT_CMP_I[]   PROGMEM;  // "CMP:I"
using common::TEXT_DADD;
extern const char TEXT_DSUB[]    PROGMEM;
extern const char TEXT_MOV_E[]   PROGMEM;  // "MOV:E"
extern const char TEXT_MOV_F[]   PROGMEM;  // "MOV:F"
extern const char TEXT_MOV_G[]   PROGMEM;  // "MOV:G"
extern const char TEXT_MOV_I[]   PROGMEM;  // "MOV:I"
extern const char TEXT_MOV_L[]   PROGMEM;  // "MOV:L"
extern const char TEXT_MOV_S[]   PROGMEM;  // "MOV:S"
extern const char TEXT_PJMP[]    PROGMEM;
extern const char TEXT_PJSR[]    PROGMEM;
extern const char TEXT_PRTD[]    PROGMEM;
extern const char TEXT_PRTS[]    PROGMEM;
using common::TEXT_RTD;
extern const char TEXT_SCB_EQ[]  PROGMEM;  // "SCB/EQ"
extern const char TEXT_SCB_F[]   PROGMEM;  // "SCB/F"
extern const char TEXT_SCB_NE[]  PROGMEM;  // "SCB/NE"
extern const char TEXT_TRAP_VS[] PROGMEM;  // "TRAP/VS"

// Register names from common
using common::TEXT_REG_CCR;
using common::TEXT_REG_DP;
using common::TEXT_REG_R0;
using common::TEXT_REG_R1;
using common::TEXT_REG_R2;
using common::TEXT_REG_R3;
using common::TEXT_REG_R4;
using common::TEXT_REG_R5;
using common::TEXT_REG_R6;
using common::TEXT_REG_R7;
using common::TEXT_REG_SP;
using common::TEXT_REG_SR;

// H8/500-specific register names
extern const char TEXT_REG_BR[] PROGMEM;
extern const char TEXT_REG_CP[] PROGMEM;
extern const char TEXT_REG_EP[] PROGMEM;
using common::TEXT_REG_FP;
extern const char TEXT_REG_TP[] PROGMEM;

// clang-format on
}  // namespace h8500
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_H8500__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
