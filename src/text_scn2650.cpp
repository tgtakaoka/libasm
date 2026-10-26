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

constexpr char TEXT_CPU_SCN2650[] = "SCN2650";
constexpr char TEXT_CPU_2650[]    = "2650";

// constexpr char TEXT_null[] = "";

// SCN2650
// constexpr char TEXT_ADDA[] = "ADDA";
// constexpr char TEXT_ADDI[] = "ADDI";
// constexpr char TEXT_ADDR[] = "ADDR";
constexpr char TEXT_ADDZ[] = "ADDZ";
// constexpr char TEXT_ANDA[] = "ANDA";
// constexpr char TEXT_ANDI[] = "ANDI";
// constexpr char TEXT_ANDR[] = "ANDR";
constexpr char TEXT_ANDZ[] = "ANDZ";
constexpr char TEXT_BCFA[] = "BCFA";
constexpr char TEXT_BCFR[] = "BCFR";
constexpr char TEXT_BCTA[] = "BCTA";
constexpr char TEXT_BCTR[] = "BCTR";
constexpr char TEXT_BDRA[] = "BDRA";
constexpr char TEXT_BDRR[] = "BDRR";
constexpr char TEXT_BIRA[] = "BIRA";
constexpr char TEXT_BIRR[] = "BIRR";
constexpr char TEXT_BRNA[] = "BRNA";
constexpr char TEXT_BRNR[] = "BRNR";
constexpr char TEXT_BSFA[] = "BSFA";
constexpr char TEXT_BSFR[] = "BSFR";
constexpr char TEXT_BSNA[] = "BSNA";
constexpr char TEXT_BSNR[] = "BSNR";
constexpr char TEXT_BSTA[] = "BSTA";
constexpr char TEXT_BSTR[] = "BSTR";
constexpr char TEXT_BSXA[] = "BSXA";
constexpr char TEXT_BXA[]  = "BXA";
// constexpr char TEXT_COMA[] = "COMA";
constexpr char TEXT_COMI[] = "COMI";
// constexpr char TEXT_COMR[] = "COMR";
constexpr char TEXT_COMZ[] = "COMZ";
constexpr char TEXT_CPSL[] = "CPSL";
constexpr char TEXT_CPSU[] = "CPSU";
constexpr char TEXT_DAR[]  = "DAR";
// constexpr char TEXT_EORA[] = "EORA";
// constexpr char TEXT_EORI[] = "EORI";
// constexpr char TEXT_EORR[] = "EORR";
constexpr char TEXT_EORZ[] = "EORZ";
// constexpr char TEXT_HALT[] = "HALT";
constexpr char TEXT_IORA[] = "IORA";
constexpr char TEXT_IORI[] = "IORI";
constexpr char TEXT_IORR[] = "IORR";
constexpr char TEXT_IORZ[] = "IORZ";
constexpr char TEXT_LODA[] = "LODA";
constexpr char TEXT_LODI[] = "LODI";
constexpr char TEXT_LODR[] = "LODR";
constexpr char TEXT_LODZ[] = "LODZ";
constexpr char TEXT_LPSL[] = "LPSL";
constexpr char TEXT_LPSU[] = "LPSU";
// constexpr char TEXT_NOP[]  = "NOP";
constexpr char TEXT_PPSL[] = "PPSL";
constexpr char TEXT_PPSU[] = "PPSU";
constexpr char TEXT_REDC[] = "REDC";
constexpr char TEXT_REDD[] = "REDD";
constexpr char TEXT_REDE[] = "REDE";
// constexpr char TEXT_RETC[] = "RETC";
// constexpr char TEXT_RETE[] = "RETE";
// constexpr char TEXT_RRL[]  = "RRL";
constexpr char TEXT_RRR[]  = "RRR";
constexpr char TEXT_SPSL[] = "SPSL";
constexpr char TEXT_SPSU[] = "SPSU";
constexpr char TEXT_STRA[] = "STRA";
constexpr char TEXT_STRI[] = "STRI";
constexpr char TEXT_STRR[] = "STRR";
constexpr char TEXT_STRZ[] = "STRZ";
// constexpr char TEXT_SUBA[] = "SUBA";
// constexpr char TEXT_SUBI[] = "SUBI";
// constexpr char TEXT_SUBR[] = "SUBR";
constexpr char TEXT_SUBZ[] = "SUBZ";
constexpr char TEXT_TMI[]  = "TMI";
constexpr char TEXT_TPSL[] = "TPSL";
constexpr char TEXT_TPSU[] = "TPSU";
constexpr char TEXT_WRTC[] = "WRTC";
constexpr char TEXT_WRTD[] = "WRTD";
constexpr char TEXT_WRTE[] = "WRTE";
constexpr char TEXT_ZBRR[] = "ZBRR";
constexpr char TEXT_ZBSR[] = "ZBSR";

// constexpr char TEXT_REG_R0[]  = "R0";
// constexpr char TEXT_REG_R1[]  = "R1";
// constexpr char TEXT_REG_R2[]  = "R2";
// constexpr char TEXT_REG_R3[]  = "R3";

// constexpr char TEXT_CC_EQ[] = "EQ";
// constexpr char TEXT_CC_GT[] = "GT";
// constexpr char TEXT_CC_LT[] = "LT";
constexpr char TEXT_CC_UN[] = "UN";

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
