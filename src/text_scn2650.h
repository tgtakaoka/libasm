/*
 * Copyright 2022 Tadashi G. Takaoka
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

#ifndef __LIBASM_TEXT_SCN2650__
#define __LIBASM_TEXT_SCN2650__

#include "text_common.h"

namespace libasm {
namespace text {
namespace scn2650 {
// clang-format off

extern const char TEXT_CPU_SCN2650[];
extern const char TEXT_CPU_2650[];

// SCN2650
using common::TEXT_ADDA;
using common::TEXT_ADDI;
using common::TEXT_ADDR;
extern const char TEXT_ADDZ[];
using common::TEXT_ANDA;
using common::TEXT_ANDI;
using common::TEXT_ANDR;
extern const char TEXT_ANDZ[];
extern const char TEXT_BCFA[];
extern const char TEXT_BCFR[];
extern const char TEXT_BCTA[];
extern const char TEXT_BCTR[];
extern const char TEXT_BDRA[];
extern const char TEXT_BDRR[];
extern const char TEXT_BIRA[];
extern const char TEXT_BIRR[];
extern const char TEXT_BRNA[];
extern const char TEXT_BRNR[];
extern const char TEXT_BSFA[];
extern const char TEXT_BSFR[];
extern const char TEXT_BSNA[];
extern const char TEXT_BSNR[];
extern const char TEXT_BSTA[];
extern const char TEXT_BSTR[];
extern const char TEXT_BSXA[];
extern const char TEXT_BXA[];
using common::TEXT_COMA;
extern const char TEXT_COMI[];
using common::TEXT_COMR;
extern const char TEXT_COMZ[];
extern const char TEXT_CPSL[];
extern const char TEXT_CPSU[];
extern const char TEXT_DAR[];
using common::TEXT_EORA;
using common::TEXT_EORI;
using common::TEXT_EORR;
extern const char TEXT_EORZ[];
using common::TEXT_HALT;
extern const char TEXT_IORA[];
extern const char TEXT_IORI[];
extern const char TEXT_IORR[];
extern const char TEXT_IORZ[];
extern const char TEXT_LODA[];
extern const char TEXT_LODI[];
extern const char TEXT_LODR[];
extern const char TEXT_LODZ[];
extern const char TEXT_LPSL[];
extern const char TEXT_LPSU[];
using common::TEXT_NOP;
extern const char TEXT_PPSL[];
extern const char TEXT_PPSU[];
extern const char TEXT_REDC[];
extern const char TEXT_REDD[];
extern const char TEXT_REDE[];
using common::TEXT_RETC;
using common::TEXT_RETE;
using common::TEXT_RRL;
extern const char TEXT_RRR[];
extern const char TEXT_SPSL[];
extern const char TEXT_SPSU[];
extern const char TEXT_STRA[];
extern const char TEXT_STRI[];
extern const char TEXT_STRR[];
extern const char TEXT_STRZ[];
using common::TEXT_SUBA;
using common::TEXT_SUBI;
using common::TEXT_SUBR;
extern const char TEXT_SUBZ[];
extern const char TEXT_TMI[];
extern const char TEXT_TPSL[];
extern const char TEXT_TPSU[];
extern const char TEXT_WRTC[];
extern const char TEXT_WRTD[];
extern const char TEXT_WRTE[];
extern const char TEXT_ZBRR[];
extern const char TEXT_ZBSR[];

using common::TEXT_REG_R0;
using common::TEXT_REG_R1;
using common::TEXT_REG_R2;
using common::TEXT_REG_R3;

using common::TEXT_CC_EQ;
using common::TEXT_CC_GT;
using common::TEXT_CC_LT;
extern const char TEXT_CC_UN[];

// clang-format on
}  // namespace scn2650
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_SCN2650__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
