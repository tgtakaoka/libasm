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
const char TEXT_LDRE[]    PROGMEM = "LDRE";
const char TEXT_LDRS[]    PROGMEM = "LDRS";
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
const char TEXT_SETRC[]   PROGMEM = "SETRC";
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

// SH-2A instruction mnemonics
const char TEXT_BAND_B[]    PROGMEM = "BAND.B";
const char TEXT_BANDNOT_B[] PROGMEM = "BANDNOT.B";
const char TEXT_BLD_B[]     PROGMEM = "BLD.B";
const char TEXT_BLDNOT_B[]  PROGMEM = "BLDNOT.B";
const char TEXT_BOR_B[]     PROGMEM = "BOR.B";
const char TEXT_BORNOT_B[]  PROGMEM = "BORNOT.B";
const char TEXT_BST_B[]     PROGMEM = "BST.B";
const char TEXT_BXOR_B[]    PROGMEM = "BXOR.B";
const char TEXT_CLIPS_B[]   PROGMEM = "CLIPS.B";
const char TEXT_CLIPS_W[]   PROGMEM = "CLIPS.W";
const char TEXT_CLIPU_B[]   PROGMEM = "CLIPU.B";
const char TEXT_CLIPU_W[]   PROGMEM = "CLIPU.W";
const char TEXT_JSR_N[]     PROGMEM = "JSR/N";
const char TEXT_LDBANK[]    PROGMEM = "LDBANK";
const char TEXT_MOVI20[]    PROGMEM = "MOVI20";
const char TEXT_MOVI20S[]   PROGMEM = "MOVI20S";
const char TEXT_MOVML_L[]   PROGMEM = "MOVML.L";
const char TEXT_MOVMU_L[]   PROGMEM = "MOVMU.L";
const char TEXT_MOVRT[]     PROGMEM = "MOVRT";
const char TEXT_MOVU_B[]    PROGMEM = "MOVU.B";
const char TEXT_MOVU_W[]    PROGMEM = "MOVU.W";
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
const char TEXT_FSTS[]    PROGMEM = "FSTS";
const char TEXT_FTRC[]    PROGMEM = "FTRC";

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

// SH-DSP register names
const char TEXT_REG_Y0[]   PROGMEM = "Y0";
const char TEXT_REG_Y1[]   PROGMEM = "Y1";

// SH-2A register names
const char TEXT_REG_DR2[]   PROGMEM = "DR2";
const char TEXT_REG_DR4[]   PROGMEM = "DR4";
const char TEXT_REG_DR6[]   PROGMEM = "DR6";
const char TEXT_REG_DR8[]   PROGMEM = "DR8";
const char TEXT_REG_DR10[]  PROGMEM = "DR10";
const char TEXT_REG_DR12[]  PROGMEM = "DR12";
const char TEXT_REG_DR14[]  PROGMEM = "DR14";
const char TEXT_REG_TBR[]   PROGMEM = "TBR";

// SH-2E FPU register names
const char TEXT_REG_FR0[]   PROGMEM = "FR0";
const char TEXT_REG_FR1[]   PROGMEM = "FR1";
const char TEXT_REG_FR2[]   PROGMEM = "FR2";
const char TEXT_REG_FR3[]   PROGMEM = "FR3";
const char TEXT_REG_FR4[]   PROGMEM = "FR4";
const char TEXT_REG_FR5[]   PROGMEM = "FR5";
const char TEXT_REG_FR6[]   PROGMEM = "FR6";
const char TEXT_REG_FR7[]   PROGMEM = "FR7";
const char TEXT_REG_FR8[]   PROGMEM = "FR8";
const char TEXT_REG_FR9[]   PROGMEM = "FR9";
const char TEXT_REG_FR10[]  PROGMEM = "FR10";
const char TEXT_REG_FR11[]  PROGMEM = "FR11";
const char TEXT_REG_FR12[]  PROGMEM = "FR12";
const char TEXT_REG_FR13[]  PROGMEM = "FR13";
const char TEXT_REG_FR14[]  PROGMEM = "FR14";
const char TEXT_REG_FR15[]  PROGMEM = "FR15";
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
