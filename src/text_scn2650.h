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
extern const char TEXT_CPU_SCN2650[] PROGMEM;
extern const char TEXT_CPU_2650[]    PROGMEM;

// SCN2650
using common::TEXT_ADDA;
using common::TEXT_ADDI;
using common::TEXT_ADDR;
extern const char TEXT_ADDZ[] PROGMEM;
using common::TEXT_ANDA;
using common::TEXT_ANDI;
using common::TEXT_ANDR;
extern const char TEXT_ANDZ[] PROGMEM;
extern const char TEXT_BCFA[] PROGMEM;
extern const char TEXT_BCFR[] PROGMEM;
extern const char TEXT_BCTA[] PROGMEM;
extern const char TEXT_BCTR[] PROGMEM;
extern const char TEXT_BDRA[] PROGMEM;
extern const char TEXT_BDRR[] PROGMEM;
extern const char TEXT_BIRA[] PROGMEM;
extern const char TEXT_BIRR[] PROGMEM;
extern const char TEXT_BRNA[] PROGMEM;
extern const char TEXT_BRNR[] PROGMEM;
extern const char TEXT_BSFA[] PROGMEM;
extern const char TEXT_BSFR[] PROGMEM;
extern const char TEXT_BSNA[] PROGMEM;
extern const char TEXT_BSNR[] PROGMEM;
extern const char TEXT_BSTA[] PROGMEM;
extern const char TEXT_BSTR[] PROGMEM;
extern const char TEXT_BSXA[] PROGMEM;
extern const char TEXT_BXA[]  PROGMEM;
using common::TEXT_COMA;
extern const char TEXT_COMI[] PROGMEM;
extern const char TEXT_COMR[] PROGMEM;
extern const char TEXT_COMZ[] PROGMEM;
extern const char TEXT_CPSL[] PROGMEM;
extern const char TEXT_CPSU[] PROGMEM;
extern const char TEXT_DAR[]  PROGMEM;
using common::TEXT_EORA;
using common::TEXT_EORI;
using common::TEXT_EORR;
extern const char TEXT_EORZ[] PROGMEM;
using common::TEXT_HALT;
extern const char TEXT_IORA[] PROGMEM;
extern const char TEXT_IORI[] PROGMEM;
extern const char TEXT_IORR[] PROGMEM;
extern const char TEXT_IORZ[] PROGMEM;
extern const char TEXT_LODA[] PROGMEM;
extern const char TEXT_LODI[] PROGMEM;
extern const char TEXT_LODR[] PROGMEM;
extern const char TEXT_LODZ[] PROGMEM;
extern const char TEXT_LPSL[] PROGMEM;
extern const char TEXT_LPSU[] PROGMEM;
using common::TEXT_NOP;
extern const char TEXT_PPSL[] PROGMEM;
extern const char TEXT_PPSU[] PROGMEM;
extern const char TEXT_REDC[] PROGMEM;
extern const char TEXT_REDD[] PROGMEM;
extern const char TEXT_REDE[] PROGMEM;
using common::TEXT_RETC;
using common::TEXT_RETE;
using common::TEXT_RRL;
extern const char TEXT_RRR[]  PROGMEM;
extern const char TEXT_SPSL[] PROGMEM;
extern const char TEXT_SPSU[] PROGMEM;
extern const char TEXT_STRA[] PROGMEM;
extern const char TEXT_STRI[] PROGMEM;
extern const char TEXT_STRR[] PROGMEM;
extern const char TEXT_STRZ[] PROGMEM;
using common::TEXT_SUBA;
using common::TEXT_SUBI;
using common::TEXT_SUBR;
extern const char TEXT_SUBZ[] PROGMEM;
extern const char TEXT_TMI[]  PROGMEM;
extern const char TEXT_TPSL[] PROGMEM;
extern const char TEXT_TPSU[] PROGMEM;
extern const char TEXT_WRTC[] PROGMEM;
extern const char TEXT_WRTD[] PROGMEM;
extern const char TEXT_WRTE[] PROGMEM;
extern const char TEXT_ZBRR[] PROGMEM;
extern const char TEXT_ZBSR[] PROGMEM;

using common::TEXT_REG_R0;
using common::TEXT_REG_R1;
using common::TEXT_REG_R2;
using common::TEXT_REG_R3;

using common::TEXT_CC_EQ;
using common::TEXT_CC_GT;
using common::TEXT_CC_LT;
extern const char TEXT_CC_UN[] PROGMEM;
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
