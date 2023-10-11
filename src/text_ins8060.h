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

#ifndef __TEXT_INS8060__
#define __TEXT_INS8060__

#include "text_common.h"

namespace libasm {
namespace text {
namespace ins8060 {

// clang-format off
extern const char TEXT_CPU_SCMP[]    PROGMEM;
extern const char TEXT_CPU_INS8060[] PROGMEM;

using common::TEXT_ADD;
extern const char TEXT_ADE[]  PROGMEM;
using common::TEXT_ADI;
using common::TEXT_AND;
extern const char TEXT_ANE[]  PROGMEM;
using common::TEXT_ANI;
extern const char TEXT_CAD[]  PROGMEM;
extern const char TEXT_CAE[]  PROGMEM;
extern const char TEXT_CAI[]  PROGMEM;
extern const char TEXT_CAS[]  PROGMEM;
extern const char TEXT_CCL[]  PROGMEM;
extern const char TEXT_CSA[]  PROGMEM;
using common::TEXT_DAD;
extern const char TEXT_DAE[]  PROGMEM;
extern const char TEXT_DAI[]  PROGMEM;
using common::TEXT_DINT;
using common::TEXT_DLD;
extern const char TEXT_DLY[]  PROGMEM;
using common::TEXT_HALT;
using common::TEXT_IDL;
extern const char TEXT_IEN[]  PROGMEM;
using common::TEXT_ILD;
using common::TEXT_JMP;
using common::TEXT_JNZ;
using common::TEXT_JP;
using common::TEXT_JZ;
using common::TEXT_LD;
using common::TEXT_LDE;
using common::TEXT_LDI;
using common::TEXT_NOP;
using common::TEXT_OR;
extern const char TEXT_ORE[]  PROGMEM;
using common::TEXT_ORI;
using common::TEXT_RR;
using common::TEXT_RRL;
extern const char TEXT_SCL[]  PROGMEM;
extern const char TEXT_SIO[]  PROGMEM;
using common::TEXT_SR;
using common::TEXT_SRL;
using common::TEXT_ST;
extern const char TEXT_XAE[]  PROGMEM;
using common::TEXT_XOR;
extern const char TEXT_XPAH[] PROGMEM;
extern const char TEXT_XPAL[] PROGMEM;
extern const char TEXT_XPPC[] PROGMEM;
extern const char TEXT_XRE[]  PROGMEM;
using common::TEXT_XRI;

using common::TEXT_REG_E;
using common::TEXT_REG_P0;
using common::TEXT_REG_P1;
using common::TEXT_REG_P2;
using common::TEXT_REG_P3;
using common::TEXT_REG_PC;
// clang-format on

}  // namespace ins8060
}  // namespace text
}  // namespace libasm

#endif  // __TEXT_INS8060__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
