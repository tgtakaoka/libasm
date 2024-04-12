/*
 * Copyright 2023 Tadashi G. Takaoka
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

#include "text_pdp8.h"

namespace libasm {
namespace text {
namespace pdp8 {
// clang-format off

constexpr char TEXT_PDP8_LIST[] PROGMEM = "IM6100, HD6120";
constexpr char TEXT_CPU_IM6100[] PROGMEM = "IM6100";
constexpr char TEXT_CPU_HD6120[] PROGMEM = "HD6120";
constexpr char TEXT_CPU_6100[]   PROGMEM = "6100";
constexpr char TEXT_CPU_6120[]   PROGMEM = "6120";

// IM6100
constexpr char TEXT_ACL[]  PROGMEM = "ACL";
// constexpr char TEXT_AND[]  PROGMEM = "AND";
constexpr char TEXT_BSW[]  PROGMEM = "BSW";
constexpr char TEXT_CAF[]  PROGMEM = "CAF";
constexpr char TEXT_CAM[]  PROGMEM = "CAM";
constexpr char TEXT_CIA[]  PROGMEM = "CIA";
constexpr char TEXT_CLA[]  PROGMEM = "CLA";
constexpr char TEXT_CLL[]  PROGMEM = "CLL";
// constexpr char TEXT_CMA[]  PROGMEM = "CMA";
constexpr char TEXT_CML[]  PROGMEM = "CML";
// constexpr char TEXT_CPU[]  PROGMEM = "CPU";
constexpr char TEXT_DCA[]  PROGMEM = "DCA";
constexpr char TEXT_END[]  PROGMEM = "END";
constexpr char TEXT_GLK[]  PROGMEM = "GLK";
constexpr char TEXT_GTF[]  PROGMEM = "GTF";
// constexpr char TEXT_HLT[]  PROGMEM = "HLT";
constexpr char TEXT_IAC[]  PROGMEM = "IAC";
constexpr char TEXT_IOF[]  PROGMEM = "IOF";
constexpr char TEXT_ION[]  PROGMEM = "ION";
// constexpr char TEXT_IOT[]  PROGMEM = "IOT";
constexpr char TEXT_ISZ[]  PROGMEM = "ISZ";
// constexpr char TEXT_JMP[]  PROGMEM = "JMP";
constexpr char TEXT_JMS[]  PROGMEM = "JMS";
constexpr char TEXT_LAS[]  PROGMEM = "LAS";
constexpr char TEXT_MQA[]  PROGMEM = "MQA";
constexpr char TEXT_MQL[]  PROGMEM = "MQL";
// constexpr char TEXT_NOP[]  PROGMEM = "NOP";
// constexpr char TEXT_ORG[]  PROGMEM = "ORG";
constexpr char TEXT_OSR[]  PROGMEM = "OSR";
// constexpr char TEXT_RAL[]  PROGMEM = "RAL";
// constexpr char TEXT_RAR[]  PROGMEM = "RAR";
constexpr char TEXT_RTF[]  PROGMEM = "RTF";
// constexpr char TEXT_RTL[]  PROGMEM = "RTL";
// constexpr char TEXT_RTR[]  PROGMEM = "RTR";
// constexpr char TEXT_SGT[]  PROGMEM = "SGT";
constexpr char TEXT_SKON[] PROGMEM = "SKON";
// constexpr char TEXT_SKP[]  PROGMEM = "SKP";
constexpr char TEXT_SMA[]  PROGMEM = "SMA";
constexpr char TEXT_SNA[]  PROGMEM = "SNA";
constexpr char TEXT_SNL[]  PROGMEM = "SNL";
constexpr char TEXT_SPA[]  PROGMEM = "SPA";
constexpr char TEXT_SRQ[]  PROGMEM = "SRQ";
// constexpr char TEXT_STA[]  PROGMEM = "STA";
constexpr char TEXT_STL[]  PROGMEM = "STL";
constexpr char TEXT_SWP[]  PROGMEM = "SWP";
constexpr char TEXT_SZA[]  PROGMEM = "SZA";
constexpr char TEXT_SZL[]  PROGMEM = "SZL";
constexpr char TEXT_TAD[]  PROGMEM = "TAD";

// HD6120
constexpr char TEXT_CDF[]  PROGMEM = "CDF";
constexpr char TEXT_CIF[]  PROGMEM = "CIF";
constexpr char TEXT_GCF[]  PROGMEM = "GCF";
constexpr char TEXT_LSP1[] PROGMEM = "LSP1";
constexpr char TEXT_LSP2[] PROGMEM = "LSP2";
constexpr char TEXT_PAC1[] PROGMEM = "PAC1";
constexpr char TEXT_PAC2[] PROGMEM = "PAC2";
constexpr char TEXT_POP1[] PROGMEM = "POP1";
constexpr char TEXT_POP2[] PROGMEM = "POP2";
constexpr char TEXT_PPC1[] PROGMEM = "PPC1";
constexpr char TEXT_PPC2[] PROGMEM = "PPC2";
constexpr char TEXT_PR0[]  PROGMEM = "PR0";
constexpr char TEXT_PR1[]  PROGMEM = "PR1";
constexpr char TEXT_PR2[]  PROGMEM = "PR2";
constexpr char TEXT_PR3[]  PROGMEM = "PR3";
constexpr char TEXT_R3L[]  PROGMEM = "R3L";
constexpr char TEXT_RDF[]  PROGMEM = "RDF";
constexpr char TEXT_RIB[]  PROGMEM = "RIB";
constexpr char TEXT_RIF[]  PROGMEM = "RIF";
constexpr char TEXT_RMF[]  PROGMEM = "RMF";
constexpr char TEXT_RSP1[] PROGMEM = "RSP1";
constexpr char TEXT_RSP2[] PROGMEM = "RSP2";
constexpr char TEXT_RTN1[] PROGMEM = "RTN1";
constexpr char TEXT_RTN2[] PROGMEM = "RTN2";
constexpr char TEXT_WSR[]  PROGMEM = "WSR";

// clang-format on
}  // namespace pdp8
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
