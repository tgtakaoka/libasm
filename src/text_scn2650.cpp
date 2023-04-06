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

#include "text_scn2650.h"

namespace libasm {
namespace text {
namespace scn2650 {

// clang-format off
const char TEXT_CPU_SCN2650[] PROGMEM = "SCN2650";
const char TEXT_CPU_2650[]    PROGMEM = "2650";

// const char TEXT_null[] PROGMEM = "";

// SCN2650
// const char TEXT_ADDA[] PROGMEM = "ADDA";
// const char TEXT_ADDI[] PROGMEM = "ADDI";
// const char TEXT_ADDR[] PROGMEM = "ADDR";
const char TEXT_ADDZ[] PROGMEM = "ADDZ";
// const char TEXT_ANDA[] PROGMEM = "ANDA";
// const char TEXT_ANDI[] PROGMEM = "ANDI";
// const char TEXT_ANDR[] PROGMEM = "ANDR";
const char TEXT_ANDZ[] PROGMEM = "ANDZ";
const char TEXT_BCFA[] PROGMEM = "BCFA";
const char TEXT_BCFR[] PROGMEM = "BCFR";
const char TEXT_BCTA[] PROGMEM = "BCTA";
const char TEXT_BCTR[] PROGMEM = "BCTR";
const char TEXT_BDRA[] PROGMEM = "BDRA";
const char TEXT_BDRR[] PROGMEM = "BDRR";
const char TEXT_BIRA[] PROGMEM = "BIRA";
const char TEXT_BIRR[] PROGMEM = "BIRR";
const char TEXT_BRNA[] PROGMEM = "BRNA";
const char TEXT_BRNR[] PROGMEM = "BRNR";
const char TEXT_BSFA[] PROGMEM = "BSFA";
const char TEXT_BSFR[] PROGMEM = "BSFR";
const char TEXT_BSNA[] PROGMEM = "BSNA";
const char TEXT_BSNR[] PROGMEM = "BSNR";
const char TEXT_BSTA[] PROGMEM = "BSTA";
const char TEXT_BSTR[] PROGMEM = "BSTR";
const char TEXT_BSXA[] PROGMEM = "BSXA";
const char TEXT_BXA[]  PROGMEM = "BXA";
// const char TEXT_COMA[] PROGMEM = "COMA";
const char TEXT_COMI[] PROGMEM = "COMI";
const char TEXT_COMR[] PROGMEM = "COMR";
const char TEXT_COMZ[] PROGMEM = "COMZ";
const char TEXT_CPSL[] PROGMEM = "CPSL";
const char TEXT_CPSU[] PROGMEM = "CPSU";
const char TEXT_DAR[]  PROGMEM = "DAR";
// const char TEXT_EORA[] PROGMEM = "EORA";
// const char TEXT_EORI[] PROGMEM = "EORI";
// const char TEXT_EORR[] PROGMEM = "EORR";
const char TEXT_EORZ[] PROGMEM = "EORZ";
// const char TEXT_HALT[] PROGMEM = "HALT";
const char TEXT_IORA[] PROGMEM = "IORA";
const char TEXT_IORI[] PROGMEM = "IORI";
const char TEXT_IORR[] PROGMEM = "IORR";
const char TEXT_IORZ[] PROGMEM = "IORZ";
const char TEXT_LODA[] PROGMEM = "LODA";
const char TEXT_LODI[] PROGMEM = "LODI";
const char TEXT_LODR[] PROGMEM = "LODR";
const char TEXT_LODZ[] PROGMEM = "LODZ";
const char TEXT_LPSL[] PROGMEM = "LPSL";
const char TEXT_LPSU[] PROGMEM = "LPSU";
// const char TEXT_NOP[]  PROGMEM = "NOP";
const char TEXT_PPSL[] PROGMEM = "PPSL";
const char TEXT_PPSU[] PROGMEM = "PPSU";
const char TEXT_REDC[] PROGMEM = "REDC";
const char TEXT_REDD[] PROGMEM = "REDD";
const char TEXT_REDE[] PROGMEM = "REDE";
const char TEXT_RETC[] PROGMEM = "RETC";
const char TEXT_RETE[] PROGMEM = "RETE";
// const char TEXT_RRL[]  PROGMEM = "RRL";
const char TEXT_RRR[]  PROGMEM = "RRR";
const char TEXT_SPSL[] PROGMEM = "SPSL";
const char TEXT_SPSU[] PROGMEM = "SPSU";
const char TEXT_STRA[] PROGMEM = "STRA";
const char TEXT_STRI[] PROGMEM = "STRI";
const char TEXT_STRR[] PROGMEM = "STRR";
const char TEXT_STRZ[] PROGMEM = "STRZ";
// const char TEXT_SUBA[] PROGMEM = "SUBA";
// const char TEXT_SUBI[] PROGMEM = "SUBI";
// const char TEXT_SUBR[] PROGMEM = "SUBR";
const char TEXT_SUBZ[] PROGMEM = "SUBZ";
const char TEXT_TMI[]  PROGMEM = "TMI";
const char TEXT_TPSL[] PROGMEM = "TPSL";
const char TEXT_TPSU[] PROGMEM = "TPSU";
const char TEXT_WRTC[] PROGMEM = "WRTC";
const char TEXT_WRTD[] PROGMEM = "WRTD";
const char TEXT_WRTE[] PROGMEM = "WRTE";
const char TEXT_ZBRR[] PROGMEM = "ZBRR";
const char TEXT_ZBSR[] PROGMEM = "ZBSR";

// const char TEXT_CC_EQ[] PROGMEM = "EQ";
// const char TEXT_CC_GT[] PROGMEM = "GT";
// const char TEXT_CC_LT[] PROGMEM = "LT";
const char TEXT_CC_UN[] PROGMEM = "UN";
// clang-format on

}  // namespace scn2650
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
