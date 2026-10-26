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

#ifndef __LIBASM_TEXT_PDP8__
#define __LIBASM_TEXT_PDP8__

#include "text_common.h"

namespace libasm {
namespace text {
namespace pdp8 {
// clang-format off

extern const char TEXT_PDP8_LIST[];
extern const char TEXT_CPU_6100[];
extern const char TEXT_CPU_6120[];
extern const char TEXT_CPU_HD6120[];
extern const char TEXT_CPU_IM6100[];

// IM6100
extern const char TEXT_ACL[];
using common::TEXT_AND;
extern const char TEXT_BSW[];
extern const char TEXT_CAF[];
extern const char TEXT_CAM[];
extern const char TEXT_CIA[];
extern const char TEXT_CLA[];
extern const char TEXT_CLL[];
using common::TEXT_CMA;
extern const char TEXT_CML[];
using common::TEXT_CPU;
extern const char TEXT_DCA[];
extern const char TEXT_END[];
extern const char TEXT_GLK[];
extern const char TEXT_GTF[];
using common::TEXT_HLT;
extern const char TEXT_IAC[];
extern const char TEXT_IOF[];
extern const char TEXT_ION[];
using common::TEXT_IOT;
extern const char TEXT_ISZ[];
using common::TEXT_JMP;
extern const char TEXT_JMS[];
extern const char TEXT_LAS[];
extern const char TEXT_MQA[];
extern const char TEXT_MQL[];
using common::TEXT_NOP;
using common::TEXT_ORG;
extern const char TEXT_OSR[];
using common::TEXT_RAL;
using common::TEXT_RAR;
extern const char TEXT_RTF[];
using common::TEXT_RTL;
using common::TEXT_RTR;
using common::TEXT_SGT;
extern const char TEXT_SKON[];
using common::TEXT_SKP;
extern const char TEXT_SMA[];
extern const char TEXT_SNA[];
extern const char TEXT_SNL[];
extern const char TEXT_SPA[];
extern const char TEXT_SRQ[];
using common::TEXT_STA;
extern const char TEXT_STL[];
extern const char TEXT_SWP[];
extern const char TEXT_SZA[];
extern const char TEXT_SZL[];
extern const char TEXT_TAD[];

// HD6120
extern const char TEXT_CDF[];
extern const char TEXT_CIF[];
extern const char TEXT_GCF[];
extern const char TEXT_LSP1[];
extern const char TEXT_LSP2[];
extern const char TEXT_PAC1[];
extern const char TEXT_PAC2[];
extern const char TEXT_POP1[];
extern const char TEXT_POP2[];
extern const char TEXT_PPC1[];
extern const char TEXT_PPC2[];
extern const char TEXT_PR0[];
extern const char TEXT_PR1[];
extern const char TEXT_PR2[];
extern const char TEXT_PR3[];
extern const char TEXT_R3L[];
extern const char TEXT_RDF[];
extern const char TEXT_RIB[];
extern const char TEXT_RIF[];
extern const char TEXT_RMF[];
extern const char TEXT_RSP1[];
extern const char TEXT_RSP2[];
extern const char TEXT_RTN1[];
extern const char TEXT_RTN2[];
extern const char TEXT_WSR[];

// clang-format on
}  // namespace pdp8
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_PDP8__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
