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
constexpr char TEXT_CPU_SCMP[]    PROGMEM = "SC/MP";
constexpr char TEXT_CPU_INS8060[] PROGMEM = "INS8060";

// constexpr char TEXT_null[] PROGMEM = "";

// constexpr char TEXT_ADD[]  PROGMEM = "ADD";
constexpr char TEXT_ADE[]  PROGMEM = "ADE";
// constexpr char TEXT_ADI[]  PROGMEM = "ADI";
// constexpr char TEXT_AND[]  PROGMEM = "AND";
constexpr char TEXT_ANE[]  PROGMEM = "ANE";
// constexpr char TEXT_ANI[]  PROGMEM = "ANI";
constexpr char TEXT_CAD[]  PROGMEM = "CAD";
constexpr char TEXT_CAE[]  PROGMEM = "CAE";
constexpr char TEXT_CAI[]  PROGMEM = "CAI";
constexpr char TEXT_CAS[]  PROGMEM = "CAS";
constexpr char TEXT_CCL[]  PROGMEM = "CCL";
constexpr char TEXT_CSA[]  PROGMEM = "CSA";
// constexpr char TEXT_DAD[]  PROGMEM = "DAD";
constexpr char TEXT_DAE[]  PROGMEM = "DAE";
constexpr char TEXT_DAI[]  PROGMEM = "DAI";
// constexpr char TEXT_DINT[] PROGMEM = "DINT";
// constexpr char TEXT_DLD[]  PROGMEM = "DLD";
constexpr char TEXT_DLY[]  PROGMEM = "DLY";
// constexpr char TEXT_HALT[] PROGMEM = "HALT";
// constexpr char TEXT_IDL[]  PROGMEM = "IDL";
constexpr char TEXT_IEN[]  PROGMEM = "IEN";
// constexpr char TEXT_ILD[]  PROGMEM = "ILD";
// constexpr char TEXT_JMP[]  PROGMEM = "JMP";
// constexpr char TEXT_JNZ[]  PROGMEM = "JNZ";
// constexpr char TEXT_JP[]   PROGMEM = "JP";
// constexpr char TEXT_JZ[]   PROGMEM = "JZ";
// constexpr char TEXT_LD[]   PROGMEM = "LD";
// constexpr char TEXT_LDE[]  PROGMEM = "LDE";
// constexpr char TEXT_LDI[]  PROGMEM = "LDI";
// constexpr char TEXT_NOP[]  PROGMEM = "NOP";
// constexpr char TEXT_OR[]   PROGMEM = "OR";
constexpr char TEXT_ORE[]  PROGMEM = "ORE";
// constexpr char TEXT_ORI[]  PROGMEM = "ORI";
// constexpr char TEXT_RR[]   PROGMEM = "RR";
// constexpr char TEXT_RRL[]  PROGMEM = "RRL";
constexpr char TEXT_SCL[]  PROGMEM = "SCL";
constexpr char TEXT_SIO[]  PROGMEM = "SIO";
// constexpr char TEXT_SR[]   PROGMEM = "SR";
// constexpr char TEXT_SRL[]  PROGMEM = "SRL";
// constexpr char TEXT_ST[]   PROGMEM = "ST";
constexpr char TEXT_XAE[]  PROGMEM = "XAE";
// constexpr char TEXT_XOR[]  PROGMEM = "XOR";
constexpr char TEXT_XPAH[] PROGMEM = "XPAH";
constexpr char TEXT_XPAL[] PROGMEM = "XPAL";
constexpr char TEXT_XPPC[] PROGMEM = "XPPC";
constexpr char TEXT_XRE[]  PROGMEM = "XRE";
// constexpr char TEXT_XRI[]  PROGMEM = "XRI";

// constexpr char TEXT_REG_E[]  PROGMEM  = "E";
// constexpr char TEXT_REG_P0[] PROGMEM = "P0";
// constexpr char TEXT_REG_P1[] PROGMEM = "P1";
// constexpr char TEXT_REG_P2[] PROGMEM = "P2";
// constexpr char TEXT_REG_P3[] PROGMEM = "P3";
// constexpr char TEXT_REG_PC[] PROGMEM = "PC";
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
