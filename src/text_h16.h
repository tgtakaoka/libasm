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

#ifndef __LIBASM_TEXT_H16__
#define __LIBASM_TEXT_H16__

#include "text_common.h"

namespace libasm {
namespace text {
namespace h16 {
// clang-format off

extern const char TEXT_H16_LIST[] PROGMEM;
extern const char TEXT_CPU_H16[]  PROGMEM;

// Instructions reused from common
using common::TEXT_ADD;
using common::TEXT_ADDS;
using common::TEXT_ADDX;
using common::TEXT_AND;
using common::TEXT_ANDC;
using common::TEXT_BCC;
using common::TEXT_BCLR;
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
using common::TEXT_BNOT;
using common::TEXT_BPL;
using common::TEXT_BRA;
using common::TEXT_BSR;
using common::TEXT_BSET;
using common::TEXT_BT;
using common::TEXT_BTST;
using common::TEXT_BVC;
using common::TEXT_BVS;
using common::TEXT_CLR;
using common::TEXT_CMP;
using common::TEXT_CMPS;
using common::TEXT_DADD;
using common::TEXT_DIVXU;
using common::TEXT_EXTS;
using common::TEXT_EXTU;
using common::TEXT_JMP;
using common::TEXT_JSR;
using common::TEXT_LDC;
using common::TEXT_LDM;
using common::TEXT_LINK;
using common::TEXT_MOV;
using common::TEXT_MOVFPE;
using common::TEXT_MOVTPE;
using common::TEXT_MULXU;
using common::TEXT_NEG;
using common::TEXT_NEGX;
using common::TEXT_NOP;
using common::TEXT_NOT;
using common::TEXT_OR;
using common::TEXT_ORC;
using common::TEXT_RESET;
using common::TEXT_ROTL;
using common::TEXT_ROTR;
using common::TEXT_ROTXL;
using common::TEXT_ROTXR;
using common::TEXT_RTD;
using common::TEXT_RTE;
using common::TEXT_RTR;
using common::TEXT_RTS;
using common::TEXT_SET;
using common::TEXT_SHAL;
using common::TEXT_SHAR;
using common::TEXT_SHLL;
using common::TEXT_SHLR;
using common::TEXT_SLEEP;
using common::TEXT_STC;
using common::TEXT_STM;
using common::TEXT_SUB;
using common::TEXT_SUBS;
using common::TEXT_SUBX;
using common::TEXT_SWAP;
using common::TEXT_TAS;
using common::TEXT_TRAP;
using common::TEXT_TRAPA;
using common::TEXT_TST;
using common::TEXT_UNLK;
using common::TEXT_XCH;
using common::TEXT_XOR;
using common::TEXT_XORC;

// Condition code names (used for Bcc:G, SET etc.)
using common::TEXT_CC_EQ;   // "EQ"
using common::TEXT_CC_F;    // "F"
using common::TEXT_CC_GE;   // "GE"
using common::TEXT_CC_GT;   // "GT"
using common::TEXT_CC_LE;   // "LE"
using common::TEXT_CC_LT;   // "LT"
using common::TEXT_CC_MI;   // "MI"
using common::TEXT_CC_NE;   // "NE"
using common::TEXT_CC_PL;   // "PL"
using common::TEXT_CC_T;    // "T"

// CR register name constants (used by the CR_TABLE NameEntry lookup).
using common::TEXT_REG_CCR;
using common::TEXT_REG_PC;
using common::TEXT_REG_SR;
using common::TEXT_REG_USP;

// H16-specific instructions.  Class-qualified mnemonics (ADD:G, MOV:RQ, BHI:G,
// ...) are stored stem-only; the ":X" class is an Entry attribute (InsnClass).
extern const char TEXT_BFEXT[]   PROGMEM;
extern const char TEXT_BFINS[]   PROGMEM;
extern const char TEXT_BFMOV[]   PROGMEM;
extern const char TEXT_BFSCH[]   PROGMEM;
extern const char TEXT_CGBN[]    PROGMEM;
extern const char TEXT_DCBN[]    PROGMEM;
extern const char TEXT_DIVXS[]   PROGMEM;
extern const char TEXT_DNEG[]    PROGMEM;
extern const char TEXT_DSUB[]    PROGMEM;
extern const char TEXT_ICBN[]    PROGMEM;
extern const char TEXT_MOVA[]    PROGMEM;
extern const char TEXT_MOVF[]    PROGMEM;
extern const char TEXT_MOVFP[]   PROGMEM;
extern const char TEXT_MOVS[]    PROGMEM;
extern const char TEXT_MOVTP[]   PROGMEM;
extern const char TEXT_MULXS[]   PROGMEM;
extern const char TEXT_PGBN[]    PROGMEM;
extern const char TEXT_SCB[]     PROGMEM;
extern const char TEXT_SCMP[]    PROGMEM;
extern const char TEXT_SMOV[]    PROGMEM;
extern const char TEXT_SSCH[]    PROGMEM;
extern const char TEXT_SSTR[]    PROGMEM;

// Additional condition code names
extern const char TEXT_CC_CC[]   PROGMEM;  // "CC"
extern const char TEXT_CC_CS[]   PROGMEM;  // "CS"
extern const char TEXT_CC_HI[]   PROGMEM;  // "HI"
extern const char TEXT_CC_HS[]   PROGMEM;  // "HS" (alias for CC)
extern const char TEXT_CC_LO[]   PROGMEM;  // "LO" (alias for CS)
extern const char TEXT_CC_LS[]   PROGMEM;  // "LS"
extern const char TEXT_CC_VC[]   PROGMEM;  // "VC"
extern const char TEXT_CC_VS[]   PROGMEM;  // "VS"

// Control register names
extern const char TEXT_REG_BMR[]  PROGMEM;
extern const char TEXT_REG_BSP[]  PROGMEM;
extern const char TEXT_REG_CBNR[] PROGMEM;
extern const char TEXT_REG_EBR[]  PROGMEM;
extern const char TEXT_REG_GBNR[] PROGMEM;
extern const char TEXT_REG_IBR[]  PROGMEM;
extern const char TEXT_REG_RBR[]  PROGMEM;
extern const char TEXT_REG_VBNR[] PROGMEM;

// clang-format on
}  // namespace h16
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_H16__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
