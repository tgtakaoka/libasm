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

extern const char TEXT_CP1600_LIST[];
extern const char TEXT_CPU_CP1600[];

// CP1600
using common::TEXT_ADCR;
extern const char TEXT_ADDAT[];
using common::TEXT_ADDI;
using common::TEXT_ADDR;
using common::TEXT_ADD;
extern const char TEXT_ANDAT[];
using common::TEXT_ANDI;
using common::TEXT_ANDR;
using common::TEXT_AND;
using common::TEXT_BC;
using common::TEXT_BEQ;
extern const char TEXT_BESC[];
extern const char TEXT_BEXT[];
using common::TEXT_BGE;
using common::TEXT_BGT;
using common::TEXT_BLE;
extern const char TEXT_BLGE[];
extern const char TEXT_BLLT[];
using common::TEXT_BLT;
using common::TEXT_BMI;
using common::TEXT_BNC;
extern const char TEXT_BNEQ[];
extern const char TEXT_BNOV[];
extern const char TEXT_BNZE[];
extern const char TEXT_BOV[];
using common::TEXT_BPL;
extern const char TEXT_BUSC[];
using common::TEXT_B;
extern const char TEXT_BZE[];
using common::TEXT_CLRC;
extern const char TEXT_CLRR[];
extern const char TEXT_CMPAT[];
using common::TEXT_CMPI;
using common::TEXT_CMPR;
using common::TEXT_CMP;
using common::TEXT_COMR;
extern const char TEXT_DECR[];
using common::TEXT_DIS;
extern const char TEXT_EIS[];
extern const char TEXT_GSWD[];
using common::TEXT_HLT;
extern const char TEXT_INCR[];
extern const char TEXT_JD[];
using common::TEXT_JE;
extern const char TEXT_J[];
using common::TEXT_JR;
extern const char TEXT_JSRD[];
extern const char TEXT_JSRE[];
using common::TEXT_JSR;
extern const char TEXT_MOVR[];
extern const char TEXT_MVIAT[];
extern const char TEXT_MVII[];
using common::TEXT_MVI;
extern const char TEXT_MVOAT[];
extern const char TEXT_MVO[];
extern const char TEXT_MVOI[];
using common::TEXT_NEGR;
extern const char TEXT_NOPP[];
using common::TEXT_NOP;
extern const char TEXT_PSHR[];
extern const char TEXT_PULR[];
using common::TEXT_RLC;
using common::TEXT_RRC;
extern const char TEXT_RSWD[];
extern const char TEXT_SARC[];
using common::TEXT_SAR;
extern const char TEXT_SDBD[];
using common::TEXT_SETC;
using common::TEXT_SIN;
extern const char TEXT_SLLC[];
using common::TEXT_SLL;
extern const char TEXT_SLR[];
extern const char TEXT_SUBAT[];
using common::TEXT_SUBI;
using common::TEXT_SUBR;
using common::TEXT_SUB;
using common::TEXT_SWAP;
extern const char TEXT_TCI[];
extern const char TEXT_TSTR[];
extern const char TEXT_XORAT[];
extern const char TEXT_XORI[];
extern const char TEXT_XORR[];
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
