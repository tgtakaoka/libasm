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

#ifndef __TEXT_INS8070__
#define __TEXT_INS8070__

#include "config_host.h"

namespace libasm {
namespace ins8070 {

// clang-format off
static const char TEXT_CPU_INS8070[] PROGMEM = "INS8070";
static constexpr const char *TEXT_CPU_LIST = TEXT_CPU_INS8070;
static constexpr const char *TEXT_CPU_8070 = (TEXT_CPU_INS8070 + 3);

static const char TEXT_ADD[]  PROGMEM = "ADD";
static const char TEXT_AND[]  PROGMEM = "AND";
static const char TEXT_BND[]  PROGMEM = "BND";
static const char TEXT_BNZ[]  PROGMEM = "BNZ";
static const char TEXT_BP[]   PROGMEM = "BP";
static const char TEXT_BRA[]  PROGMEM = "BRA";
static const char TEXT_BZ[]   PROGMEM = "BZ";
static const char TEXT_CALL[] PROGMEM = "CALL";
static const char TEXT_DIV[]  PROGMEM = "DIV";
static const char TEXT_DLD[]  PROGMEM = "DLD";
static const char TEXT_ILD[]  PROGMEM = "ILD";
static const char TEXT_JMP[]  PROGMEM = "JMP";
static const char TEXT_JSR[]  PROGMEM = "JSR";
static const char TEXT_LD[]   PROGMEM = "LD";
static const char TEXT_MPY[]  PROGMEM = "MPY";
static const char TEXT_NOP[]  PROGMEM = "NOP";
static const char TEXT_OR[]   PROGMEM = "OR";
static const char TEXT_PLI[]  PROGMEM = "PLI";
static const char TEXT_POP[]  PROGMEM = "POP";
static const char TEXT_PUSH[] PROGMEM = "PUSH";
static const char TEXT_RET[]  PROGMEM = "RET";
static const char TEXT_RRL[]  PROGMEM = "RRL";
static const char TEXT_RR[]   PROGMEM = "RR";
static const char TEXT_SL[]   PROGMEM = "SL";
static const char TEXT_SRL[]  PROGMEM = "SRL";
static const char TEXT_SR[]   PROGMEM = "SR";
static const char TEXT_SSM[]  PROGMEM = "SSM";
static const char TEXT_ST[]   PROGMEM = "ST";
static const char TEXT_SUB[]  PROGMEM = "SUB";
static const char TEXT_XCH[]  PROGMEM = "XCH";
static const char TEXT_XOR[]  PROGMEM = "XOR";
// clang-format on

}  // namespace ins8070
}  // namespace libasm

#endif  // __TEXT_INS8070__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
