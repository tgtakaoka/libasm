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

constexpr char TEXT_PDP8_LIST[]  = "IM6100, HD6120";
constexpr char TEXT_CPU_6100[]   = "6100";
constexpr char TEXT_CPU_6120[]   = "6120";
constexpr char TEXT_CPU_HD6120[] = "HD6120";
constexpr char TEXT_CPU_IM6100[] = "IM6100";

// IM6100
constexpr char TEXT_ACL[]  = "ACL";
// constexpr char TEXT_AND[] = "AND";
constexpr char TEXT_BSW[]  = "BSW";
constexpr char TEXT_CAF[]  = "CAF";
constexpr char TEXT_CAM[]  = "CAM";
constexpr char TEXT_CIA[]  = "CIA";
constexpr char TEXT_CLA[]  = "CLA";
constexpr char TEXT_CLL[]  = "CLL";
// constexpr char TEXT_CMA[] = "CMA";
constexpr char TEXT_CML[]  = "CML";
// constexpr char TEXT_CPU[] = "CPU";
constexpr char TEXT_DCA[]  = "DCA";
constexpr char TEXT_END[]  = "END";
constexpr char TEXT_GLK[]  = "GLK";
constexpr char TEXT_GTF[]  = "GTF";
// constexpr char TEXT_HLT[] = "HLT";
constexpr char TEXT_IAC[]  = "IAC";
constexpr char TEXT_IOF[]  = "IOF";
constexpr char TEXT_ION[]  = "ION";
// constexpr char TEXT_IOT[] = "IOT";
constexpr char TEXT_ISZ[]  = "ISZ";
// constexpr char TEXT_JMP[] = "JMP";
constexpr char TEXT_JMS[]  = "JMS";
constexpr char TEXT_LAS[]  = "LAS";
constexpr char TEXT_MQA[]  = "MQA";
constexpr char TEXT_MQL[]  = "MQL";
// constexpr char TEXT_NOP[] = "NOP";
// constexpr char TEXT_ORG[] = "ORG";
constexpr char TEXT_OSR[]  = "OSR";
// constexpr char TEXT_RAL[] = "RAL";
// constexpr char TEXT_RAR[] = "RAR";
constexpr char TEXT_RTF[]  = "RTF";
// constexpr char TEXT_RTL[] = "RTL";
// constexpr char TEXT_RTR[] = "RTR";
// constexpr char TEXT_SGT[] = "SGT";
constexpr char TEXT_SKON[] = "SKON";
// constexpr char TEXT_SKP[] = "SKP";
constexpr char TEXT_SMA[]  = "SMA";
constexpr char TEXT_SNA[]  = "SNA";
constexpr char TEXT_SNL[]  = "SNL";
constexpr char TEXT_SPA[]  = "SPA";
constexpr char TEXT_SRQ[]  = "SRQ";
// constexpr char TEXT_STA[] = "STA";
constexpr char TEXT_STL[]  = "STL";
constexpr char TEXT_SWP[]  = "SWP";
constexpr char TEXT_SZA[]  = "SZA";
constexpr char TEXT_SZL[]  = "SZL";
constexpr char TEXT_TAD[]  = "TAD";

// HD6120
constexpr char TEXT_CDF[]  = "CDF";
constexpr char TEXT_CIF[]  = "CIF";
constexpr char TEXT_GCF[]  = "GCF";
constexpr char TEXT_LSP1[] = "LSP1";
constexpr char TEXT_LSP2[] = "LSP2";
constexpr char TEXT_PAC1[] = "PAC1";
constexpr char TEXT_PAC2[] = "PAC2";
constexpr char TEXT_POP1[] = "POP1";
constexpr char TEXT_POP2[] = "POP2";
constexpr char TEXT_PPC1[] = "PPC1";
constexpr char TEXT_PPC2[] = "PPC2";
constexpr char TEXT_PR0[]  = "PR0";
constexpr char TEXT_PR1[]  = "PR1";
constexpr char TEXT_PR2[]  = "PR2";
constexpr char TEXT_PR3[]  = "PR3";
constexpr char TEXT_R3L[]  = "R3L";
constexpr char TEXT_RDF[]  = "RDF";
constexpr char TEXT_RIB[]  = "RIB";
constexpr char TEXT_RIF[]  = "RIF";
constexpr char TEXT_RMF[]  = "RMF";
constexpr char TEXT_RSP1[] = "RSP1";
constexpr char TEXT_RSP2[] = "RSP2";
constexpr char TEXT_RTN1[] = "RTN1";
constexpr char TEXT_RTN2[] = "RTN2";
constexpr char TEXT_WSR[]  = "WSR";

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
