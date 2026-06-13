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

#ifndef __LIBASM_TEXT_CP1600__
#define __LIBASM_TEXT_CP1600__

#include "text_common.h"

namespace libasm {
namespace text {
namespace cp1600 {
// clang-format off

extern const char TEXT_CP1600_LIST[] PROGMEM;
extern const char TEXT_CPU_CP1600[]  PROGMEM;

// CP1600
using common::TEXT_ADCR;
extern const char TEXT_ADDAT[] PROGMEM;
using common::TEXT_ADDI;
using common::TEXT_ADDR;
using common::TEXT_ADD;
extern const char TEXT_ANDAT[] PROGMEM;
using common::TEXT_ANDI;
using common::TEXT_ANDR;
using common::TEXT_AND;
using common::TEXT_BC;
using common::TEXT_BEQ;
extern const char TEXT_BESC[]  PROGMEM;
extern const char TEXT_BEXT[]  PROGMEM;
using common::TEXT_BGE;
using common::TEXT_BGT;
using common::TEXT_BLE;
extern const char TEXT_BLGE[]  PROGMEM;
extern const char TEXT_BLLT[]  PROGMEM;
using common::TEXT_BLT;
using common::TEXT_BMI;
using common::TEXT_BNC;
extern const char TEXT_BNEQ[]  PROGMEM;
extern const char TEXT_BNOV[]  PROGMEM;
extern const char TEXT_BNZE[]  PROGMEM;
extern const char TEXT_BOV[]   PROGMEM;
using common::TEXT_BPL;
extern const char TEXT_BUSC[]  PROGMEM;
using common::TEXT_B;
extern const char TEXT_BZE[]   PROGMEM;
using common::TEXT_CLRC;
extern const char TEXT_CLRR[]  PROGMEM;
extern const char TEXT_CMPAT[] PROGMEM;
using common::TEXT_CMPI;
using common::TEXT_CMPR;
using common::TEXT_CMP;
using common::TEXT_COMR;
extern const char TEXT_DECR[]  PROGMEM;
using common::TEXT_DIS;
extern const char TEXT_EIS[]   PROGMEM;
extern const char TEXT_GSWD[]  PROGMEM;
using common::TEXT_HLT;
extern const char TEXT_INCR[]  PROGMEM;
extern const char TEXT_JD[]    PROGMEM;
using common::TEXT_JE;
extern const char TEXT_J[]     PROGMEM;
using common::TEXT_JR;
extern const char TEXT_JSRD[]  PROGMEM;
extern const char TEXT_JSRE[]  PROGMEM;
using common::TEXT_JSR;
extern const char TEXT_MOVR[]  PROGMEM;
extern const char TEXT_MVIAT[] PROGMEM;
extern const char TEXT_MVII[]  PROGMEM;
using common::TEXT_MVI;
extern const char TEXT_MVOAT[] PROGMEM;
extern const char TEXT_MVO[]   PROGMEM;
extern const char TEXT_MVOI[]  PROGMEM;
using common::TEXT_NEGR;
extern const char TEXT_NOPP[]  PROGMEM;
using common::TEXT_NOP;
extern const char TEXT_PSHR[]  PROGMEM;
extern const char TEXT_PULR[]  PROGMEM;
using common::TEXT_RLC;
using common::TEXT_RRC;
extern const char TEXT_RSWD[]  PROGMEM;
extern const char TEXT_SARC[]  PROGMEM;
using common::TEXT_SAR;
extern const char TEXT_SDBD[]  PROGMEM;
using common::TEXT_SETC;
using common::TEXT_SIN;
extern const char TEXT_SLLC[]  PROGMEM;
using common::TEXT_SLL;
extern const char TEXT_SLR[]   PROGMEM;
extern const char TEXT_SUBAT[] PROGMEM;
using common::TEXT_SUBI;
using common::TEXT_SUBR;
using common::TEXT_SUB;
using common::TEXT_SWAP;
extern const char TEXT_TCI[]   PROGMEM;
extern const char TEXT_TSTR[]  PROGMEM;
extern const char TEXT_XORAT[] PROGMEM;
extern const char TEXT_XORI[]  PROGMEM;
extern const char TEXT_XORR[]  PROGMEM;
using common::TEXT_XOR;

using common::TEXT_REG_PC;
using common::TEXT_REG_SP;

// clang-format on
}  // namespace cp1600
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_CP1600__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
