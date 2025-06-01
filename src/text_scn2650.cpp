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
constexpr char TEXT_CPU_SCN2650[] PROGMEM = "SCN2650";
constexpr char TEXT_CPU_2650[]    PROGMEM = "2650";

// constexpr char TEXT_null[] PROGMEM = "";

// SCN2650
// constexpr char TEXT_ADDA[] PROGMEM = "ADDA";
// constexpr char TEXT_ADDI[] PROGMEM = "ADDI";
// constexpr char TEXT_ADDR[] PROGMEM = "ADDR";
constexpr char TEXT_ADDZ[] PROGMEM = "ADDZ";
// constexpr char TEXT_ANDA[] PROGMEM = "ANDA";
// constexpr char TEXT_ANDI[] PROGMEM = "ANDI";
// constexpr char TEXT_ANDR[] PROGMEM = "ANDR";
constexpr char TEXT_ANDZ[] PROGMEM = "ANDZ";
constexpr char TEXT_BCFA[] PROGMEM = "BCFA";
constexpr char TEXT_BCFR[] PROGMEM = "BCFR";
constexpr char TEXT_BCTA[] PROGMEM = "BCTA";
constexpr char TEXT_BCTR[] PROGMEM = "BCTR";
constexpr char TEXT_BDRA[] PROGMEM = "BDRA";
constexpr char TEXT_BDRR[] PROGMEM = "BDRR";
constexpr char TEXT_BIRA[] PROGMEM = "BIRA";
constexpr char TEXT_BIRR[] PROGMEM = "BIRR";
constexpr char TEXT_BRNA[] PROGMEM = "BRNA";
constexpr char TEXT_BRNR[] PROGMEM = "BRNR";
constexpr char TEXT_BSFA[] PROGMEM = "BSFA";
constexpr char TEXT_BSFR[] PROGMEM = "BSFR";
constexpr char TEXT_BSNA[] PROGMEM = "BSNA";
constexpr char TEXT_BSNR[] PROGMEM = "BSNR";
constexpr char TEXT_BSTA[] PROGMEM = "BSTA";
constexpr char TEXT_BSTR[] PROGMEM = "BSTR";
constexpr char TEXT_BSXA[] PROGMEM = "BSXA";
constexpr char TEXT_BXA[]  PROGMEM = "BXA";
// constexpr char TEXT_COMA[] PROGMEM = "COMA";
constexpr char TEXT_COMI[] PROGMEM = "COMI";
constexpr char TEXT_COMR[] PROGMEM = "COMR";
constexpr char TEXT_COMZ[] PROGMEM = "COMZ";
constexpr char TEXT_CPSL[] PROGMEM = "CPSL";
constexpr char TEXT_CPSU[] PROGMEM = "CPSU";
constexpr char TEXT_DAR[]  PROGMEM = "DAR";
// constexpr char TEXT_EORA[] PROGMEM = "EORA";
// constexpr char TEXT_EORI[] PROGMEM = "EORI";
// constexpr char TEXT_EORR[] PROGMEM = "EORR";
constexpr char TEXT_EORZ[] PROGMEM = "EORZ";
// constexpr char TEXT_HALT[] PROGMEM = "HALT";
constexpr char TEXT_IORA[] PROGMEM = "IORA";
constexpr char TEXT_IORI[] PROGMEM = "IORI";
constexpr char TEXT_IORR[] PROGMEM = "IORR";
constexpr char TEXT_IORZ[] PROGMEM = "IORZ";
constexpr char TEXT_LODA[] PROGMEM = "LODA";
constexpr char TEXT_LODI[] PROGMEM = "LODI";
constexpr char TEXT_LODR[] PROGMEM = "LODR";
constexpr char TEXT_LODZ[] PROGMEM = "LODZ";
constexpr char TEXT_LPSL[] PROGMEM = "LPSL";
constexpr char TEXT_LPSU[] PROGMEM = "LPSU";
// constexpr char TEXT_NOP[]  PROGMEM = "NOP";
constexpr char TEXT_PPSL[] PROGMEM = "PPSL";
constexpr char TEXT_PPSU[] PROGMEM = "PPSU";
constexpr char TEXT_REDC[] PROGMEM = "REDC";
constexpr char TEXT_REDD[] PROGMEM = "REDD";
constexpr char TEXT_REDE[] PROGMEM = "REDE";
// constexpr char TEXT_RETC[] PROGMEM = "RETC";
// constexpr char TEXT_RETE[] PROGMEM = "RETE";
// constexpr char TEXT_RRL[]  PROGMEM = "RRL";
constexpr char TEXT_RRR[]  PROGMEM = "RRR";
constexpr char TEXT_SPSL[] PROGMEM = "SPSL";
constexpr char TEXT_SPSU[] PROGMEM = "SPSU";
constexpr char TEXT_STRA[] PROGMEM = "STRA";
constexpr char TEXT_STRI[] PROGMEM = "STRI";
constexpr char TEXT_STRR[] PROGMEM = "STRR";
constexpr char TEXT_STRZ[] PROGMEM = "STRZ";
// constexpr char TEXT_SUBA[] PROGMEM = "SUBA";
// constexpr char TEXT_SUBI[] PROGMEM = "SUBI";
// constexpr char TEXT_SUBR[] PROGMEM = "SUBR";
constexpr char TEXT_SUBZ[] PROGMEM = "SUBZ";
constexpr char TEXT_TMI[]  PROGMEM = "TMI";
constexpr char TEXT_TPSL[] PROGMEM = "TPSL";
constexpr char TEXT_TPSU[] PROGMEM = "TPSU";
constexpr char TEXT_WRTC[] PROGMEM = "WRTC";
constexpr char TEXT_WRTD[] PROGMEM = "WRTD";
constexpr char TEXT_WRTE[] PROGMEM = "WRTE";
constexpr char TEXT_ZBRR[] PROGMEM = "ZBRR";
constexpr char TEXT_ZBSR[] PROGMEM = "ZBSR";

// constexpr char TEXT_REG_R0[]  PROGMEM = "R0";
// constexpr char TEXT_REG_R1[]  PROGMEM = "R1";
// constexpr char TEXT_REG_R2[]  PROGMEM = "R2";
// constexpr char TEXT_REG_R3[]  PROGMEM = "R3";

// constexpr char TEXT_CC_EQ[] PROGMEM = "EQ";
// constexpr char TEXT_CC_GT[] PROGMEM = "GT";
// constexpr char TEXT_CC_LT[] PROGMEM = "LT";
constexpr char TEXT_CC_UN[] PROGMEM = "UN";
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
