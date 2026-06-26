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
const char TEXT_SUPERH_LIST[] PROGMEM = "SH-1, SH-2, SH-DSP, SH-2E, SH-2A";
const char TEXT_CPU_SH1[]     PROGMEM = "SH-1";
const char TEXT_CPU_SH2[]     PROGMEM = "SH-2";
const char TEXT_CPU_SH_DSP[]  PROGMEM = "SH-DSP";
const char TEXT_CPU_SH2E[]    PROGMEM = "SH-2E";
const char TEXT_CPU_SH2A[]    PROGMEM = "SH-2A";
const char TEXT_FPU_NONE[]    PROGMEM = "none";
const char TEXT_FPU_SH2E[]    PROGMEM = "SH-2E";
const char TEXT_FPU_SH2A[]    PROGMEM = "SH-2A";

const char TEXT_ADDV[]    PROGMEM = "ADDV";
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
const char TEXT_DT[]      PROGMEM = "DT";
const char TEXT_LDRE[]    PROGMEM = "LDRE";
const char TEXT_LDRS[]    PROGMEM = "LDRS";
const char TEXT_MOVA[]    PROGMEM = "MOVA";
const char TEXT_MOVT[]    PROGMEM = "MOVT";
const char TEXT_NEGC[]    PROGMEM = "NEGC";
const char TEXT_ROTCL[]   PROGMEM = "ROTCL";
const char TEXT_ROTCR[]   PROGMEM = "ROTCR";
const char TEXT_SETRC[]   PROGMEM = "SETRC";
const char TEXT_SETT[]    PROGMEM = "SETT";
const char TEXT_SHLL2[]   PROGMEM = "SHLL2";
const char TEXT_SHLL8[]   PROGMEM = "SHLL8";
const char TEXT_SHLL16[]  PROGMEM = "SHLL16";
const char TEXT_SHLR2[]   PROGMEM = "SHLR2";
const char TEXT_SHLR8[]   PROGMEM = "SHLR8";
const char TEXT_SHLR16[]  PROGMEM = "SHLR16";
const char TEXT_SUBV[]    PROGMEM = "SUBV";
const char TEXT_XTRCT[]   PROGMEM = "XTRCT";

// SH-2A instruction mnemonics
const char TEXT_JSR_N[]     PROGMEM = "JSR/N";
const char TEXT_LDBANK[]    PROGMEM = "LDBANK";
const char TEXT_MOVI20[]    PROGMEM = "MOVI20";
const char TEXT_MOVI20S[]   PROGMEM = "MOVI20S";
const char TEXT_MOVRT[]     PROGMEM = "MOVRT";
const char TEXT_MULR[]      PROGMEM = "MULR";
const char TEXT_NOTT[]      PROGMEM = "NOTT";
const char TEXT_PREF[]      PROGMEM = "PREF";
const char TEXT_RESBANK[]   PROGMEM = "RESBANK";
const char TEXT_RTS_N[]     PROGMEM = "RTS/N";
const char TEXT_RTV_N[]     PROGMEM = "RTV/N";
const char TEXT_SHAD[]      PROGMEM = "SHAD";
const char TEXT_SHLD[]      PROGMEM = "SHLD";
const char TEXT_STBANK[]    PROGMEM = "STBANK";

// SH-2A FPU additions
const char TEXT_FCNVDS[]    PROGMEM = "FCNVDS";
const char TEXT_FCNVSD[]    PROGMEM = "FCNVSD";

// SH-2E FPU instruction mnemonics
const char TEXT_FCMP_EQ[] PROGMEM = "FCMP/EQ";
const char TEXT_FCMP_GT[] PROGMEM = "FCMP/GT";
const char TEXT_FLDI0[]   PROGMEM = "FLDI0";
const char TEXT_FLDI1[]   PROGMEM = "FLDI1";
const char TEXT_FLDS[]    PROGMEM = "FLDS";
const char TEXT_FMAC[]    PROGMEM = "FMAC";
const char TEXT_FMOV[]    PROGMEM = "FMOV";
const char TEXT_FMOV_S[]  PROGMEM = "FMOV.S";
const char TEXT_MAC_W[]   PROGMEM = "MAC.W";
const char TEXT_MAC_L[]   PROGMEM = "MAC.L";
const char TEXT_CLIPS[]   PROGMEM = "CLIPS";
const char TEXT_CLIPU[]   PROGMEM = "CLIPU";
const char TEXT_DMULS[]   PROGMEM = "DMULS";
const char TEXT_DMULU[]   PROGMEM = "DMULU";
const char TEXT_MOVML[]   PROGMEM = "MOVML";
const char TEXT_MOVMU[]   PROGMEM = "MOVMU";
const char TEXT_FSTS[]    PROGMEM = "FSTS";
const char TEXT_FTRC[]    PROGMEM = "FTRC";

const char TEXT_REG_GBR[]  PROGMEM = "GBR";
const char TEXT_REG_PR[]   PROGMEM = "PR";

// SH-DSP register names
const char TEXT_REG_Y0[]   PROGMEM = "Y0";
const char TEXT_REG_Y1[]   PROGMEM = "Y1";

// SH-2A register names
const char TEXT_REG_TBR[]   PROGMEM = "TBR";

// SH-2E FPU register names
const char TEXT_REG_FPUL[]  PROGMEM = "FPUL";
const char TEXT_REG_FPSCR[] PROGMEM = "FPSCR";

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
