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

#include "config_host.h"

namespace libasm {
namespace ins8060 {

// clang-format off
static const char TEXT_CPU_SCMP[] PROGMEM = "SC/MP";
static const char TEXT_CPU_INS8060[] PROGMEM = "INS8060";
static constexpr const char *TEXT_CPU_LIST = TEXT_CPU_INS8060;
static constexpr const char *TEXT_CPU_8060 = (TEXT_CPU_INS8060 + 3);
static constexpr const char *TEXT_null = &TEXT_CPU_INS8060[7];

static const char TEXT_ADD[]  PROGMEM = "ADD";
static const char TEXT_ADE[]  PROGMEM = "ADE";
static const char TEXT_ADI[]  PROGMEM = "ADI";
static const char TEXT_AND[]  PROGMEM = "AND";
static const char TEXT_ANE[]  PROGMEM = "ANE";
static const char TEXT_ANI[]  PROGMEM = "ANI";
static const char TEXT_CAD[]  PROGMEM = "CAD";
static const char TEXT_CAE[]  PROGMEM = "CAE";
static const char TEXT_CAI[]  PROGMEM = "CAI";
static const char TEXT_CAS[]  PROGMEM = "CAS";
static const char TEXT_CCL[]  PROGMEM = "CCL";
static const char TEXT_CSA[]  PROGMEM = "CSA";
static const char TEXT_DAD[]  PROGMEM = "DAD";
static const char TEXT_DAE[]  PROGMEM = "DAE";
static const char TEXT_DAI[]  PROGMEM = "DAI";
static const char TEXT_DINT[] PROGMEM = "DINT";
static const char TEXT_DLD[]  PROGMEM = "DLD";
static const char TEXT_DLY[]  PROGMEM = "DLY";
static const char TEXT_HALT[] PROGMEM = "HALT";
static const char TEXT_IDL[]  PROGMEM = "IDL";
static const char TEXT_IEN[]  PROGMEM = "IEN";
static const char TEXT_ILD[]  PROGMEM = "ILD";
static const char TEXT_JMP[]  PROGMEM = "JMP";
static const char TEXT_JNZ[]  PROGMEM = "JNZ";
static const char TEXT_JP[]   PROGMEM = "JP";
static const char TEXT_JZ[]   PROGMEM = "JZ";
static const char TEXT_LD[]   PROGMEM = "LD";
static const char TEXT_LDE[]  PROGMEM = "LDE";
static const char TEXT_LDI[]  PROGMEM = "LDI";
static const char TEXT_NOP[]  PROGMEM = "NOP";
static const char TEXT_OR[]   PROGMEM = "OR";
static const char TEXT_ORE[]  PROGMEM = "ORE";
static const char TEXT_ORI[]  PROGMEM = "ORI";
static const char TEXT_RR[]   PROGMEM = "RR";
static const char TEXT_RRL[]  PROGMEM = "RRL";
static const char TEXT_SCL[]  PROGMEM = "SCL";
static const char TEXT_SIO[]  PROGMEM = "SIO";
static const char TEXT_SR[]   PROGMEM = "SR";
static const char TEXT_SRL[]  PROGMEM = "SRL";
static const char TEXT_ST[]   PROGMEM = "ST";
static const char TEXT_XAE[]  PROGMEM = "XAE";
static const char TEXT_XOR[]  PROGMEM = "XOR";
static const char TEXT_XPAH[] PROGMEM = "XPAH";
static const char TEXT_XPAL[] PROGMEM = "XPAL";
static const char TEXT_XPPC[] PROGMEM = "XPPC";
static const char TEXT_XRE[]  PROGMEM = "XRE";
static const char TEXT_XRI[]  PROGMEM = "XRI";
// clang-format on

}  // namespace ins8060
}  // namespace libasm

#endif  // __TEXT_INS8060__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
