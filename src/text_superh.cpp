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
const char TEXT_SUPERH_LIST[] = "SH-1, SH-2, SH-DSP, SH-2E, SH-2A";
const char TEXT_CPU_SH1[]     = "SH-1";
const char TEXT_CPU_SH2[]     = "SH-2";
const char TEXT_CPU_SH_DSP[]  = "SH-DSP";
const char TEXT_CPU_SH2E[]    = "SH-2E";
const char TEXT_CPU_SH2A[]    = "SH-2A";
const char TEXT_FPU_NONE[]    = "none";
const char TEXT_FPU_SH2E[]    = "SH-2E";
const char TEXT_FPU_SH2A[]    = "SH-2A";

const char TEXT_ADDV[]    = "ADDV";
const char TEXT_BF_S[]    = "BF/S";
const char TEXT_BRAF[]    = "BRAF";
const char TEXT_BSRF[]    = "BSRF";
const char TEXT_BT_S[]    = "BT/S";
const char TEXT_CLRT[]    = "CLRT";
const char TEXT_CMP_EQ[]  = "CMP/EQ";
const char TEXT_CMP_GE[]  = "CMP/GE";
const char TEXT_CMP_GT[]  = "CMP/GT";
const char TEXT_CMP_HI[]  = "CMP/HI";
const char TEXT_CMP_HS[]  = "CMP/HS";
const char TEXT_CMP_PL[]  = "CMP/PL";
const char TEXT_CMP_PZ[]  = "CMP/PZ";
const char TEXT_CMP_STR[] = "CMP/STR";
const char TEXT_DIV0S[]   = "DIV0S";
const char TEXT_DIV0U[]   = "DIV0U";
const char TEXT_DIV1[]    = "DIV1";
const char TEXT_DT[]      = "DT";
const char TEXT_LDRE[]    = "LDRE";
const char TEXT_LDRS[]    = "LDRS";
const char TEXT_MOVA[]    = "MOVA";
const char TEXT_MOVT[]    = "MOVT";
const char TEXT_NEGC[]    = "NEGC";
const char TEXT_ROTCL[]   = "ROTCL";
const char TEXT_ROTCR[]   = "ROTCR";
const char TEXT_SETRC[]   = "SETRC";
const char TEXT_SETT[]    = "SETT";
const char TEXT_SHLL2[]   = "SHLL2";
const char TEXT_SHLL8[]   = "SHLL8";
const char TEXT_SHLL16[]  = "SHLL16";
const char TEXT_SHLR2[]   = "SHLR2";
const char TEXT_SHLR8[]   = "SHLR8";
const char TEXT_SHLR16[]  = "SHLR16";
const char TEXT_SUBV[]    = "SUBV";
const char TEXT_XTRCT[]   = "XTRCT";

// SH-2A instruction mnemonics
const char TEXT_JSR_N[]     = "JSR/N";
const char TEXT_LDBANK[]    = "LDBANK";
const char TEXT_MOVI20[]    = "MOVI20";
const char TEXT_MOVI20S[]   = "MOVI20S";
const char TEXT_MOVRT[]     = "MOVRT";
const char TEXT_MULR[]      = "MULR";
const char TEXT_NOTT[]      = "NOTT";
const char TEXT_PREF[]      = "PREF";
const char TEXT_RESBANK[]   = "RESBANK";
const char TEXT_RTS_N[]     = "RTS/N";
const char TEXT_RTV_N[]     = "RTV/N";
const char TEXT_SHAD[]      = "SHAD";
const char TEXT_SHLD[]      = "SHLD";
const char TEXT_STBANK[]    = "STBANK";

// SH-2A FPU additions
const char TEXT_FCNVDS[]    = "FCNVDS";
const char TEXT_FCNVSD[]    = "FCNVSD";

// SH-2E FPU instruction mnemonics
const char TEXT_FCMP_EQ[] = "FCMP/EQ";
const char TEXT_FCMP_GT[] = "FCMP/GT";
const char TEXT_FLDI0[]   = "FLDI0";
const char TEXT_FLDI1[]   = "FLDI1";
const char TEXT_FLDS[]    = "FLDS";
const char TEXT_FMAC[]    = "FMAC";
const char TEXT_FMOV[]    = "FMOV";
const char TEXT_FMOV_S[]  = "FMOV.S";
const char TEXT_MAC_W[]   = "MAC.W";
const char TEXT_MAC_L[]   = "MAC.L";
const char TEXT_CLIPS[]   = "CLIPS";
const char TEXT_CLIPU[]   = "CLIPU";
const char TEXT_DMULS[]   = "DMULS";
const char TEXT_DMULU[]   = "DMULU";
const char TEXT_MOVML[]   = "MOVML";
const char TEXT_MOVMU[]   = "MOVMU";
const char TEXT_FSTS[]    = "FSTS";
const char TEXT_FTRC[]    = "FTRC";

const char TEXT_REG_GBR[]  = "GBR";
const char TEXT_REG_PR[]   = "PR";

// SH-DSP register names
const char TEXT_REG_Y0[]   = "Y0";
const char TEXT_REG_Y1[]   = "Y1";

// SH-2A register names
const char TEXT_REG_TBR[]   = "TBR";

// SH-2E FPU register names
const char TEXT_REG_FPUL[]  = "FPUL";
const char TEXT_REG_FPSCR[] = "FPSCR";

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
