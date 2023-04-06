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

#include "text_ins8060.h"

namespace libasm {
namespace text {
namespace ins8060 {

// clang-format off
const char TEXT_CPU_SCMP[]    PROGMEM = "SC/MP";
const char TEXT_CPU_INS8060[] PROGMEM = "INS8060";

// const char TEXT_null[] PROGMEM = "";

// const char TEXT_ADD[]  PROGMEM = "ADD";
const char TEXT_ADE[]  PROGMEM = "ADE";
// const char TEXT_ADI[]  PROGMEM = "ADI";
// const char TEXT_AND[]  PROGMEM = "AND";
const char TEXT_ANE[]  PROGMEM = "ANE";
// const char TEXT_ANI[]  PROGMEM = "ANI";
const char TEXT_CAD[]  PROGMEM = "CAD";
const char TEXT_CAE[]  PROGMEM = "CAE";
const char TEXT_CAI[]  PROGMEM = "CAI";
const char TEXT_CAS[]  PROGMEM = "CAS";
const char TEXT_CCL[]  PROGMEM = "CCL";
const char TEXT_CSA[]  PROGMEM = "CSA";
// const char TEXT_DAD[]  PROGMEM = "DAD";
const char TEXT_DAE[]  PROGMEM = "DAE";
const char TEXT_DAI[]  PROGMEM = "DAI";
// const char TEXT_DINT[] PROGMEM = "DINT";
// const char TEXT_DLD[]  PROGMEM = "DLD";
const char TEXT_DLY[]  PROGMEM = "DLY";
// const char TEXT_HALT[] PROGMEM = "HALT";
// const char TEXT_IDL[]  PROGMEM = "IDL";
const char TEXT_IEN[]  PROGMEM = "IEN";
// const char TEXT_ILD[]  PROGMEM = "ILD";
// const char TEXT_JMP[]  PROGMEM = "JMP";
// const char TEXT_JNZ[]  PROGMEM = "JNZ";
// const char TEXT_JP[]   PROGMEM = "JP";
// const char TEXT_JZ[]   PROGMEM = "JZ";
// const char TEXT_LD[]   PROGMEM = "LD";
// const char TEXT_LDE[]  PROGMEM = "LDE";
// const char TEXT_LDI[]  PROGMEM = "LDI";
// const char TEXT_NOP[]  PROGMEM = "NOP";
// const char TEXT_OR[]   PROGMEM = "OR";
const char TEXT_ORE[]  PROGMEM = "ORE";
// const char TEXT_ORI[]  PROGMEM = "ORI";
// const char TEXT_RR[]   PROGMEM = "RR";
// const char TEXT_RRL[]  PROGMEM = "RRL";
const char TEXT_SCL[]  PROGMEM = "SCL";
const char TEXT_SIO[]  PROGMEM = "SIO";
// const char TEXT_SR[]   PROGMEM = "SR";
// const char TEXT_SRL[]  PROGMEM = "SRL";
// const char TEXT_ST[]   PROGMEM = "ST";
const char TEXT_XAE[]  PROGMEM = "XAE";
// const char TEXT_XOR[]  PROGMEM = "XOR";
const char TEXT_XPAH[] PROGMEM = "XPAH";
const char TEXT_XPAL[] PROGMEM = "XPAL";
const char TEXT_XPPC[] PROGMEM = "XPPC";
const char TEXT_XRE[]  PROGMEM = "XRE";
// const char TEXT_XRI[]  PROGMEM = "XRI";

// const char TEXT_REG_E[]  PROGMEM  = "E";
// const char TEXT_REG_P0[] PROGMEM = "P0";
// const char TEXT_REG_P1[] PROGMEM = "P1";
// const char TEXT_REG_P2[] PROGMEM = "P2";
// const char TEXT_REG_P3[] PROGMEM = "P3";
// const char TEXT_REG_PC[] PROGMEM = "PC";
// clang-format on

}  // namespace ins8060
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
