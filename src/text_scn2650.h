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

#ifndef __TEXT_SCN2650__
#define __TEXT_SCN2650__

#include "config_host.h"

namespace libasm {
namespace scn2650 {

// clang-format off
static const char TEXT_CPU_LIST[] = "SCN2650";
static constexpr const char *TEXT_CPU_SCN2650 = TEXT_CPU_LIST;
static constexpr const char *TEXT_CPU_2650 = TEXT_CPU_LIST + 3;
static constexpr const char *TEXT_null = TEXT_CPU_2650 + 4;

// SCN2650
static const char TEXT_ADDA[] PROGMEM = "ADDA";
static const char TEXT_ADDI[] PROGMEM = "ADDI";
static const char TEXT_ADDR[] PROGMEM = "ADDR";
static const char TEXT_ADDZ[] PROGMEM = "ADDZ";
static const char TEXT_ANDA[] PROGMEM = "ANDA";
static const char TEXT_ANDI[] PROGMEM = "ANDI";
static const char TEXT_ANDR[] PROGMEM = "ANDR";
static const char TEXT_ANDZ[] PROGMEM = "ANDZ";
static const char TEXT_BCFA[] PROGMEM = "BCFA";
static const char TEXT_BCFR[] PROGMEM = "BCFR";
static const char TEXT_BCTA[] PROGMEM = "BCTA";
static const char TEXT_BCTR[] PROGMEM = "BCTR";
static const char TEXT_BDRA[] PROGMEM = "BDRA";
static const char TEXT_BDRR[] PROGMEM = "BDRR";
static const char TEXT_BIRA[] PROGMEM = "BIRA";
static const char TEXT_BIRR[] PROGMEM = "BIRR";
static const char TEXT_BRNA[] PROGMEM = "BRNA";
static const char TEXT_BRNR[] PROGMEM = "BRNR";
static const char TEXT_BSFA[] PROGMEM = "BSFA";
static const char TEXT_BSFR[] PROGMEM = "BSFR";
static const char TEXT_BSNA[] PROGMEM = "BSNA";
static const char TEXT_BSNR[] PROGMEM = "BSNR";
static const char TEXT_BSTA[] PROGMEM = "BSTA";
static const char TEXT_BSTR[] PROGMEM = "BSTR";
static const char TEXT_BSXA[] PROGMEM = "BSXA";
static const char TEXT_BXA[]  PROGMEM = "BXA";
static const char TEXT_COMA[] PROGMEM = "COMA";
static const char TEXT_COMI[] PROGMEM = "COMI";
static const char TEXT_COMR[] PROGMEM = "COMR";
static const char TEXT_COMZ[] PROGMEM = "COMZ";
static const char TEXT_CPSL[] PROGMEM = "CPSL";
static const char TEXT_CPSU[] PROGMEM = "CPSU";
static const char TEXT_DAR[]  PROGMEM = "DAR";
static const char TEXT_EORA[] PROGMEM = "EORA";
static const char TEXT_EORI[] PROGMEM = "EORI";
static const char TEXT_EORR[] PROGMEM = "EORR";
static const char TEXT_EORZ[] PROGMEM = "EORZ";
static const char TEXT_HALT[] PROGMEM = "HALT";
static const char TEXT_IORA[] PROGMEM = "IORA";
static const char TEXT_IORI[] PROGMEM = "IORI";
static const char TEXT_IORR[] PROGMEM = "IORR";
static const char TEXT_IORZ[] PROGMEM = "IORZ";
static const char TEXT_LODA[] PROGMEM = "LODA";
static const char TEXT_LODI[] PROGMEM = "LODI";
static const char TEXT_LODR[] PROGMEM = "LODR";
static const char TEXT_LODZ[] PROGMEM = "LODZ";
static const char TEXT_LPSL[] PROGMEM = "LPSL";
static const char TEXT_LPSU[] PROGMEM = "LPSU";
static const char TEXT_NOP[]  PROGMEM = "NOP";
static const char TEXT_PPSL[] PROGMEM = "PPSL";
static const char TEXT_PPSU[] PROGMEM = "PPSU";
static const char TEXT_REDC[] PROGMEM = "REDC";
static const char TEXT_REDD[] PROGMEM = "REDD";
static const char TEXT_REDE[] PROGMEM = "REDE";
static const char TEXT_RETC[] PROGMEM = "RETC";
static const char TEXT_RETE[] PROGMEM = "RETE";
static const char TEXT_RRL[]  PROGMEM = "RRL";
static const char TEXT_RRR[]  PROGMEM = "RRR";
static const char TEXT_SPSL[] PROGMEM = "SPSL";
static const char TEXT_SPSU[] PROGMEM = "SPSU";
static const char TEXT_STRA[] PROGMEM = "STRA";
static const char TEXT_STRI[] PROGMEM = "STRI";
static const char TEXT_STRR[] PROGMEM = "STRR";
static const char TEXT_STRZ[] PROGMEM = "STRZ";
static const char TEXT_SUBA[] PROGMEM = "SUBA";
static const char TEXT_SUBI[] PROGMEM = "SUBI";
static const char TEXT_SUBR[] PROGMEM = "SUBR";
static const char TEXT_SUBZ[] PROGMEM = "SUBZ";
static const char TEXT_TMI[]  PROGMEM = "TMI";
static const char TEXT_TPSL[] PROGMEM = "TPSL";
static const char TEXT_TPSU[] PROGMEM = "TPSU";
static const char TEXT_WRTC[] PROGMEM = "WRTC";
static const char TEXT_WRTD[] PROGMEM = "WRTD";
static const char TEXT_WRTE[] PROGMEM = "WRTE";
static const char TEXT_ZBRR[] PROGMEM = "ZBRR";
static const char TEXT_ZBSR[] PROGMEM = "ZBSR";

// clang-format off

} // namespace scn2650
} // namespace libasm

#endif // __TEXT_SCN2650__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
