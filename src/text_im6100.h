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

#ifndef __LIBASM_TEXT_IM6100__
#define __LIBASM_TEXT_IM6100__

#include "text_common.h"

namespace libasm {
namespace text {
namespace im6100 {
// clang-format off

extern const char TEXT_IM6100_LIST[] PROGMEM;
extern const char TEXT_CPU_IM6100[] PROGMEM;
extern const char TEXT_CPU_HD6120[] PROGMEM;
extern const char TEXT_CPU_6100[]   PROGMEM;
extern const char TEXT_CPU_6120[]   PROGMEM;

// IM6100
extern const char TEXT_ACL[]  PROGMEM;
extern const char TEXT_AND[]  PROGMEM;
extern const char TEXT_BSW[]  PROGMEM;
extern const char TEXT_CAF[]  PROGMEM;
extern const char TEXT_CAM[]  PROGMEM;
extern const char TEXT_CIA[]  PROGMEM;
extern const char TEXT_CLA[]  PROGMEM;
extern const char TEXT_CLL[]  PROGMEM;
using common::TEXT_CMA;
extern const char TEXT_CML[]  PROGMEM;
extern const char TEXT_CPU[]  PROGMEM;
extern const char TEXT_DCA[]  PROGMEM;
extern const char TEXT_END[]  PROGMEM;
extern const char TEXT_GLK[]  PROGMEM;
extern const char TEXT_GTF[]  PROGMEM;
extern const char TEXT_HLT[]  PROGMEM;
extern const char TEXT_IAC[]  PROGMEM;
extern const char TEXT_IOF[]  PROGMEM;
extern const char TEXT_ION[]  PROGMEM;
extern const char TEXT_IOT[]  PROGMEM;
extern const char TEXT_ISZ[]  PROGMEM;
extern const char TEXT_JMP[]  PROGMEM;
extern const char TEXT_JMS[]  PROGMEM;
extern const char TEXT_LAS[]  PROGMEM;
extern const char TEXT_MQA[]  PROGMEM;
extern const char TEXT_MQL[]  PROGMEM;
extern const char TEXT_NOP[]  PROGMEM;
extern const char TEXT_ORG[]  PROGMEM;
extern const char TEXT_OSR[]  PROGMEM;
using common::TEXT_RAL;
using common::TEXT_RAR;
extern const char TEXT_RTF[]  PROGMEM;
using common::TEXT_RTL;
using common::TEXT_RTR;
using common::TEXT_SGT;
extern const char TEXT_SKON[] PROGMEM;
using common::TEXT_SKP;
extern const char TEXT_SMA[]  PROGMEM;
extern const char TEXT_SNA[]  PROGMEM;
extern const char TEXT_SNL[]  PROGMEM;
extern const char TEXT_SPA[]  PROGMEM;
extern const char TEXT_SRQ[]  PROGMEM;
extern const char TEXT_STA[]  PROGMEM;
extern const char TEXT_STL[]  PROGMEM;
extern const char TEXT_SWP[]  PROGMEM;
extern const char TEXT_SZA[]  PROGMEM;
extern const char TEXT_SZL[]  PROGMEM;
extern const char TEXT_TAD[]  PROGMEM;

// HD6120
extern const char TEXT_CDF[]  PROGMEM;
extern const char TEXT_CIF[]  PROGMEM;
extern const char TEXT_GCF[]  PROGMEM;
extern const char TEXT_LSP1[] PROGMEM;
extern const char TEXT_LSP2[] PROGMEM;
extern const char TEXT_PAC1[] PROGMEM;
extern const char TEXT_PAC2[] PROGMEM;
extern const char TEXT_POP1[] PROGMEM;
extern const char TEXT_POP2[] PROGMEM;
extern const char TEXT_PPC1[] PROGMEM;
extern const char TEXT_PPC2[] PROGMEM;
extern const char TEXT_PR0[]  PROGMEM;
extern const char TEXT_PR1[]  PROGMEM;
extern const char TEXT_PR2[]  PROGMEM;
extern const char TEXT_PR3[]  PROGMEM;
extern const char TEXT_R3L[]  PROGMEM;
extern const char TEXT_RDF[]  PROGMEM;
extern const char TEXT_RIB[]  PROGMEM;
extern const char TEXT_RIF[]  PROGMEM;
extern const char TEXT_RMF[]  PROGMEM;
extern const char TEXT_RSP1[] PROGMEM;
extern const char TEXT_RSP2[] PROGMEM;
extern const char TEXT_RTN1[] PROGMEM;
extern const char TEXT_RTN2[] PROGMEM;
extern const char TEXT_WSR[]  PROGMEM;

// clang-format on
}  // namespace im6100
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_IM6100__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
