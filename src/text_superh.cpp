/*
 * Copyright 2026 Tadashi G. Takaoka
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

#include "text_superh.h"

namespace libasm {
namespace text {
namespace superh {
// clang-format off

// clang-format off
const char TEXT_SUPERH_LIST[] PROGMEM = "SH-1, SH-2";
const char TEXT_CPU_SH1[]     PROGMEM = "SH-1";
const char TEXT_CPU_SH2[]     PROGMEM = "SH-2";

const char TEXT_ADDV[]    PROGMEM = "ADDV";
const char TEXT_AND_B[]   PROGMEM = "AND.B";
const char TEXT_BF_S[]    PROGMEM = "BF/S";
const char TEXT_BRAF[]    PROGMEM = "BRAF";
const char TEXT_BSRF[]    PROGMEM = "BSRF";
const char TEXT_BT_S[]    PROGMEM = "BT/S";
const char TEXT_CLRT[]    PROGMEM = "CLRT";
const char TEXT_CMP_EQ[]  PROGMEM = "CMP/EQ";
const char TEXT_CMP_GE[]  PROGMEM = "CMP/GE";
const char TEXT_CMP_GT[]  PROGMEM = "CMP/GT";
const char TEXT_CMP_HI[]  PROGMEM = "CMP/HI";
const char TEXT_CMP_HS[]  PROGMEM = "CMP/HS";
const char TEXT_CMP_PL[]  PROGMEM = "CMP/PL";
const char TEXT_CMP_PZ[]  PROGMEM = "CMP/PZ";
const char TEXT_CMP_STR[] PROGMEM = "CMP/STR";
const char TEXT_DIV0S[]   PROGMEM = "DIV0S";
const char TEXT_DIV0U[]   PROGMEM = "DIV0U";
const char TEXT_DIV1[]    PROGMEM = "DIV1";
const char TEXT_DMULS_L[] PROGMEM = "DMULS.L";
const char TEXT_DMULU_L[] PROGMEM = "DMULU.L";
const char TEXT_DT[]      PROGMEM = "DT";
const char TEXT_EXTS_B[]  PROGMEM = "EXTS.B";
const char TEXT_EXTS_W[]  PROGMEM = "EXTS.W";
const char TEXT_EXTU_B[]  PROGMEM = "EXTU.B";
const char TEXT_EXTU_W[]  PROGMEM = "EXTU.W";
const char TEXT_LDC_L[]   PROGMEM = "LDC.L";
const char TEXT_LDS_L[]   PROGMEM = "LDS.L";
const char TEXT_MAC_L[]   PROGMEM = "MAC.L";
const char TEXT_MAC_W[]   PROGMEM = "MAC.W";
const char TEXT_MOV_B[]   PROGMEM = "MOV.B";
const char TEXT_MOV_L[]   PROGMEM = "MOV.L";
const char TEXT_MOV_W[]   PROGMEM = "MOV.W";
const char TEXT_MOVA[]    PROGMEM = "MOVA";
const char TEXT_MOVT[]    PROGMEM = "MOVT";
const char TEXT_MUL_L[]   PROGMEM = "MUL.L";
const char TEXT_MULS_W[]  PROGMEM = "MULS.W";
const char TEXT_MULU_W[]  PROGMEM = "MULU.W";
const char TEXT_NEGC[]    PROGMEM = "NEGC";
const char TEXT_OR_B[]    PROGMEM = "OR.B";
const char TEXT_ROTCL[]   PROGMEM = "ROTCL";
const char TEXT_ROTCR[]   PROGMEM = "ROTCR";
const char TEXT_SETT[]    PROGMEM = "SETT";
const char TEXT_SHLL2[]   PROGMEM = "SHLL2";
const char TEXT_SHLL8[]   PROGMEM = "SHLL8";
const char TEXT_SHLL16[]  PROGMEM = "SHLL16";
const char TEXT_SHLR2[]   PROGMEM = "SHLR2";
const char TEXT_SHLR8[]   PROGMEM = "SHLR8";
const char TEXT_SHLR16[]  PROGMEM = "SHLR16";
const char TEXT_STC_L[]   PROGMEM = "STC.L";
const char TEXT_STS_L[]   PROGMEM = "STS.L";
const char TEXT_SUBV[]    PROGMEM = "SUBV";
const char TEXT_SWAP_B[]  PROGMEM = "SWAP.B";
const char TEXT_SWAP_W[]  PROGMEM = "SWAP.W";
const char TEXT_TAS_B[]   PROGMEM = "TAS.B";
const char TEXT_TST_B[]   PROGMEM = "TST.B";
const char TEXT_XOR_B[]   PROGMEM = "XOR.B";
const char TEXT_XTRCT[]   PROGMEM = "XTRCT";

const char TEXT_REG_R8[]   PROGMEM = "R8";
const char TEXT_REG_R9[]   PROGMEM = "R9";
const char TEXT_REG_R10[]  PROGMEM = "R10";
const char TEXT_REG_R11[]  PROGMEM = "R11";
const char TEXT_REG_R12[]  PROGMEM = "R12";
const char TEXT_REG_R13[]  PROGMEM = "R13";
const char TEXT_REG_R14[]  PROGMEM = "R14";
const char TEXT_REG_R15[]  PROGMEM = "R15";
const char TEXT_REG_GBR[]  PROGMEM = "GBR";
const char TEXT_REG_PR[]   PROGMEM = "PR";

// clang-format on
}  // namespace superh
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
