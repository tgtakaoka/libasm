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

constexpr char TEXT_CPU_SCMP[]    = "SC/MP";
constexpr char TEXT_CPU_INS8060[] = "INS8060";

// constexpr char TEXT_null[] = "";

// constexpr char TEXT_ADD[]  = "ADD";
// constexpr char TEXT_ADE[]  = "ADE";
// constexpr char TEXT_ADI[]  = "ADI";
// constexpr char TEXT_AND[]  = "AND";
constexpr char TEXT_ANE[]  = "ANE";
// constexpr char TEXT_ANI[]  = "ANI";
constexpr char TEXT_CAD[]  = "CAD";
constexpr char TEXT_CAE[]  = "CAE";
constexpr char TEXT_CAI[]  = "CAI";
// constexpr char TEXT_CAS[]  = "CAS";
constexpr char TEXT_CCL[]  = "CCL";
constexpr char TEXT_CSA[]  = "CSA";
// constexpr char TEXT_DAD[]  = "DAD";
constexpr char TEXT_DAE[]  = "DAE";
constexpr char TEXT_DAI[]  = "DAI";
// constexpr char TEXT_DINT[] = "DINT";
// constexpr char TEXT_DLD[]  = "DLD";
constexpr char TEXT_DLY[]  = "DLY";
// constexpr char TEXT_HALT[] = "HALT";
// constexpr char TEXT_IDL[]  = "IDL";
constexpr char TEXT_IEN[]  = "IEN";
// constexpr char TEXT_ILD[]  = "ILD";
// constexpr char TEXT_JMP[]  = "JMP";
// constexpr char TEXT_JNZ[]  = "JNZ";
// constexpr char TEXT_JP[]   = "JP";
// constexpr char TEXT_JZ[]   = "JZ";
// constexpr char TEXT_LD[]   = "LD";
// constexpr char TEXT_LDE[]  = "LDE";
// constexpr char TEXT_LDI[]  = "LDI";
// constexpr char TEXT_NOP[]  = "NOP";
// constexpr char TEXT_OR[]   = "OR";
// constexpr char TEXT_ORE[]  = "ORE";
// constexpr char TEXT_ORI[]  = "ORI";
// constexpr char TEXT_RR[]   = "RR";
// constexpr char TEXT_RRL[]  = "RRL";
constexpr char TEXT_SCL[]  = "SCL";
constexpr char TEXT_SIO[]  = "SIO";
// constexpr char TEXT_SR[]   = "SR";
// constexpr char TEXT_SRL[]  = "SRL";
// constexpr char TEXT_ST[]   = "ST";
constexpr char TEXT_XAE[]  = "XAE";
// constexpr char TEXT_XOR[]  = "XOR";
constexpr char TEXT_XPAH[] = "XPAH";
constexpr char TEXT_XPAL[] = "XPAL";
constexpr char TEXT_XPPC[] = "XPPC";
constexpr char TEXT_XRE[]  = "XRE";
// constexpr char TEXT_XRI[]  = "XRI";

// constexpr char TEXT_REG_E[]  = "E";
// constexpr char TEXT_REG_P0[] = "P0";
// constexpr char TEXT_REG_P1[] = "P1";
// constexpr char TEXT_REG_P2[] = "P2";
// constexpr char TEXT_REG_P3[] = "P3";
// constexpr char TEXT_REG_PC[] = "PC";

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
