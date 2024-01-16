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

#ifndef __LIBASM_TEXT_INS8070__
#define __LIBASM_TEXT_INS8070__

#include "text_common.h"

namespace libasm {
namespace text {
namespace ins8070 {

// clang-format off
extern const char TEXT_CPU_INS8070[] PROGMEM;
extern const char TEXT_CPU_8070[]    PROGMEM;

using common::TEXT_ADD;
using common::TEXT_AND;
extern const char TEXT_BND[]  PROGMEM;
using common::TEXT_BNZ;
using common::TEXT_BP;
using common::TEXT_BRA;
using common::TEXT_BZ;
using common::TEXT_CALL;
using common::TEXT_DIV;
using common::TEXT_DLD;
using common::TEXT_ILD;
using common::TEXT_JMP;
using common::TEXT_JSR;
using common::TEXT_LD;
using common::TEXT_MPY;
using common::TEXT_NOP;
using common::TEXT_OR;
extern const char TEXT_PLI[]  PROGMEM;
using common::TEXT_POP;
using common::TEXT_PUSH;
using common::TEXT_RET;
using common::TEXT_RRL;
using common::TEXT_RR;
using common::TEXT_SL;
using common::TEXT_SRL;
using common::TEXT_SR;
extern const char TEXT_SSM[]  PROGMEM;
using common::TEXT_ST;
using common::TEXT_SUB;
using common::TEXT_XCH;
using common::TEXT_XOR;

using common::TEXT_REG_A;
using common::TEXT_REG_E;
extern const char TEXT_REG_EA[] PROGMEM;
using common::TEXT_REG_T;
using common::TEXT_REG_S;
using common::TEXT_REG_PC;
using common::TEXT_REG_SP;
using common::TEXT_REG_P2;
using common::TEXT_REG_P3;
// clang-format on

}  // namespace ins8070
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_INS8070__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
