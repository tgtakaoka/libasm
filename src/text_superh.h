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

#ifndef __LIBASM_TEXT_SUPERH__
#define __LIBASM_TEXT_SUPERH__

#include "text_common.h"

namespace libasm {
namespace text {
namespace superh {
// clang-format off

extern const char TEXT_SUPERH_LIST[] PROGMEM;
extern const char TEXT_CPU_SH1[]     PROGMEM;
extern const char TEXT_CPU_SH2[]     PROGMEM;
extern const char TEXT_CPU_SH_DSP[]  PROGMEM;
extern const char TEXT_CPU_SH2E[]    PROGMEM;
extern const char TEXT_CPU_SH2A[]    PROGMEM;
extern const char TEXT_FPU_NONE[]    PROGMEM;
extern const char TEXT_FPU_SH2E[]    PROGMEM;
extern const char TEXT_FPU_SH2A[]    PROGMEM;

// Instructions reused from common
using common::TEXT_ADD;
using common::TEXT_ADDC;
using common::TEXT_AND;
using common::TEXT_BF;
using common::TEXT_BRA;
using common::TEXT_BSR;
using common::TEXT_BT;
using common::TEXT_EXTS;
using common::TEXT_JMP;
using common::TEXT_JSR;
using common::TEXT_LDC;
using common::TEXT_LDS;
using common::TEXT_MAC;
using common::TEXT_MUL;
using common::TEXT_MULS;
using common::TEXT_MULU;
using common::TEXT_NEG;
using common::TEXT_NOP;
using common::TEXT_NOT;
using common::TEXT_OR;
using common::TEXT_RTE;
using common::TEXT_RTS;
using common::TEXT_SHLL;
using common::TEXT_STC;
using common::TEXT_STS;
using common::TEXT_SUB;
using common::TEXT_SUBC;
using common::TEXT_SWAP;
using common::TEXT_TST;
using common::TEXT_XOR;

// SH-specific instruction mnemonics
extern const char TEXT_ADDC_B[]   PROGMEM;  // "ADDC"  (already in common as TEXT_ADDC)
extern const char TEXT_ADDV[]     PROGMEM;
extern const char TEXT_AND_B[]    PROGMEM;  // "AND.B"
extern const char TEXT_BF_S[]     PROGMEM;  // "BF/S"
extern const char TEXT_BRAF[]     PROGMEM;
extern const char TEXT_BSRF[]     PROGMEM;
extern const char TEXT_BT_S[]     PROGMEM;  // "BT/S"
extern const char TEXT_CLRMAC[]   PROGMEM;
extern const char TEXT_CLRT[]     PROGMEM;
extern const char TEXT_CMP_EQ[]   PROGMEM;  // "CMP/EQ"
extern const char TEXT_CMP_GE[]   PROGMEM;  // "CMP/GE"
extern const char TEXT_CMP_GT[]   PROGMEM;  // "CMP/GT"
extern const char TEXT_CMP_HI[]   PROGMEM;  // "CMP/HI"
extern const char TEXT_CMP_HS[]   PROGMEM;  // "CMP/HS"
extern const char TEXT_CMP_PL[]   PROGMEM;  // "CMP/PL"
extern const char TEXT_CMP_PZ[]   PROGMEM;  // "CMP/PZ"
extern const char TEXT_CMP_STR[]  PROGMEM;  // "CMP/STR"
extern const char TEXT_DIV0S[]    PROGMEM;
extern const char TEXT_DIV0U[]    PROGMEM;
extern const char TEXT_DIV1[]     PROGMEM;
extern const char TEXT_DMULS_L[]  PROGMEM;  // "DMULS.L"
extern const char TEXT_DMULU_L[]  PROGMEM;  // "DMULU.L"
extern const char TEXT_DT[]       PROGMEM;
extern const char TEXT_EXTS_B[]   PROGMEM;  // "EXTS.B"
extern const char TEXT_EXTS_W[]   PROGMEM;  // "EXTS.W"
extern const char TEXT_EXTU_B[]   PROGMEM;  // "EXTU.B"
extern const char TEXT_EXTU_W[]   PROGMEM;  // "EXTU.W"
extern const char TEXT_LDC_L[]    PROGMEM;  // "LDC.L"
extern const char TEXT_LDRE[]     PROGMEM;
extern const char TEXT_LDRS[]     PROGMEM;
extern const char TEXT_LDS_L[]    PROGMEM;  // "LDS.L"
extern const char TEXT_MAC_L[]    PROGMEM;  // "MAC.L"
extern const char TEXT_MAC_W[]    PROGMEM;  // "MAC.W"
extern const char TEXT_MOV[]      PROGMEM;
extern const char TEXT_MOV_B[]    PROGMEM;  // "MOV.B"
extern const char TEXT_MOV_L[]    PROGMEM;  // "MOV.L"
extern const char TEXT_MOV_W[]    PROGMEM;  // "MOV.W"
extern const char TEXT_MOVA[]     PROGMEM;
extern const char TEXT_MOVT[]     PROGMEM;
extern const char TEXT_MUL_L[]    PROGMEM;  // "MUL.L"
extern const char TEXT_MULS_W[]   PROGMEM;  // "MULS.W"
extern const char TEXT_MULU_W[]   PROGMEM;  // "MULU.W"
extern const char TEXT_NEGC[]     PROGMEM;
extern const char TEXT_OR_B[]     PROGMEM;  // "OR.B"
extern const char TEXT_ROTCL[]    PROGMEM;
extern const char TEXT_ROTCR[]    PROGMEM;
extern const char TEXT_ROTL[]     PROGMEM;
extern const char TEXT_ROTR[]     PROGMEM;
extern const char TEXT_SETRC[]    PROGMEM;
extern const char TEXT_SETT[]     PROGMEM;
extern const char TEXT_SHAL[]     PROGMEM;
extern const char TEXT_SHAR[]     PROGMEM;
extern const char TEXT_SHLR[]     PROGMEM;
extern const char TEXT_SHLL2[]    PROGMEM;
extern const char TEXT_SHLL8[]    PROGMEM;
extern const char TEXT_SHLL16[]   PROGMEM;
extern const char TEXT_SHLR2[]    PROGMEM;
extern const char TEXT_SHLR8[]    PROGMEM;
extern const char TEXT_SHLR16[]   PROGMEM;
extern const char TEXT_SLEEP[]    PROGMEM;
extern const char TEXT_STC_L[]    PROGMEM;  // "STC.L"
extern const char TEXT_STS_L[]    PROGMEM;  // "STS.L"
extern const char TEXT_SUBV[]     PROGMEM;
extern const char TEXT_SWAP_B[]   PROGMEM;  // "SWAP.B"
extern const char TEXT_SWAP_W[]   PROGMEM;  // "SWAP.W"
extern const char TEXT_TAS_B[]    PROGMEM;  // "TAS.B"
extern const char TEXT_TRAPA[]    PROGMEM;
extern const char TEXT_TST_B[]    PROGMEM;  // "TST.B"
extern const char TEXT_XOR_B[]    PROGMEM;  // "XOR.B"
extern const char TEXT_XTRCT[]    PROGMEM;

// SH-2A instruction mnemonics
extern const char TEXT_BCLR[]     PROGMEM;
extern const char TEXT_BSET[]     PROGMEM;
extern const char TEXT_BLD[]      PROGMEM;
extern const char TEXT_BST[]      PROGMEM;
extern const char TEXT_BAND_B[]   PROGMEM;  // "BAND.B"
extern const char TEXT_BANDNOT_B[] PROGMEM; // "BANDNOT.B"
extern const char TEXT_BLD_B[]    PROGMEM;  // "BLD.B"
extern const char TEXT_BLDNOT_B[] PROGMEM;  // "BLDNOT.B"
extern const char TEXT_BOR_B[]    PROGMEM;  // "BOR.B"
extern const char TEXT_BORNOT_B[] PROGMEM;  // "BORNOT.B"
extern const char TEXT_BST_B[]    PROGMEM;  // "BST.B"
extern const char TEXT_BXOR_B[]   PROGMEM;  // "BXOR.B"
extern const char TEXT_CLIPS_B[]  PROGMEM;  // "CLIPS.B"
extern const char TEXT_CLIPS_W[]  PROGMEM;  // "CLIPS.W"
extern const char TEXT_CLIPU_B[]  PROGMEM;  // "CLIPU.B"
extern const char TEXT_CLIPU_W[]  PROGMEM;  // "CLIPU.W"
extern const char TEXT_DIVS[]     PROGMEM;
extern const char TEXT_DIVU[]     PROGMEM;
extern const char TEXT_JSR_N[]    PROGMEM;  // "JSR/N"
extern const char TEXT_LDBANK[]   PROGMEM;
extern const char TEXT_MOVI20[]   PROGMEM;
extern const char TEXT_MOVI20S[]  PROGMEM;
extern const char TEXT_MOVML_L[]  PROGMEM;  // "MOVML.L"
extern const char TEXT_MOVMU_L[]  PROGMEM;  // "MOVMU.L"
extern const char TEXT_MOVRT[]    PROGMEM;
extern const char TEXT_MOVU_B[]   PROGMEM;  // "MOVU.B"
extern const char TEXT_MOVU_W[]   PROGMEM;  // "MOVU.W"
extern const char TEXT_MULR[]     PROGMEM;
extern const char TEXT_NOTT[]     PROGMEM;
extern const char TEXT_PREF[]     PROGMEM;
extern const char TEXT_RESBANK[]  PROGMEM;
extern const char TEXT_RTS_N[]    PROGMEM;  // "RTS/N"
extern const char TEXT_RTV_N[]    PROGMEM;  // "RTV/N"
extern const char TEXT_SHAD[]     PROGMEM;
extern const char TEXT_SHLD[]     PROGMEM;
extern const char TEXT_STBANK[]   PROGMEM;

// SH-2A FPU additions
extern const char TEXT_FCNVDS[]   PROGMEM;
extern const char TEXT_FCNVSD[]   PROGMEM;

// SH-2E FPU instruction mnemonics
extern const char TEXT_FABS[]     PROGMEM;
extern const char TEXT_FADD[]     PROGMEM;
extern const char TEXT_FCMP_EQ[]  PROGMEM;  // "FCMP/EQ"
extern const char TEXT_FCMP_GT[]  PROGMEM;  // "FCMP/GT"
extern const char TEXT_FDIV[]     PROGMEM;
extern const char TEXT_FLDI0[]    PROGMEM;
extern const char TEXT_FLDI1[]    PROGMEM;
extern const char TEXT_FLDS[]     PROGMEM;
extern const char TEXT_FLOAT[]    PROGMEM;
extern const char TEXT_FMAC[]     PROGMEM;
extern const char TEXT_FMOV[]     PROGMEM;
extern const char TEXT_FMOV_S[]   PROGMEM;  // "FMOV.S"
extern const char TEXT_FMUL[]     PROGMEM;
extern const char TEXT_FNEG[]     PROGMEM;
extern const char TEXT_FSTS[]     PROGMEM;
extern const char TEXT_FSUB[]     PROGMEM;
extern const char TEXT_FTRC[]     PROGMEM;

// Register names
using common::TEXT_REG_R0;
using common::TEXT_REG_R1;
using common::TEXT_REG_R2;
using common::TEXT_REG_R3;
using common::TEXT_REG_R4;
using common::TEXT_REG_R5;
using common::TEXT_REG_R6;
using common::TEXT_REG_R7;
using common::TEXT_REG_SR;

extern const char TEXT_REG_R8[]   PROGMEM;
extern const char TEXT_REG_R9[]   PROGMEM;
extern const char TEXT_REG_R10[]  PROGMEM;
extern const char TEXT_REG_R11[]  PROGMEM;
extern const char TEXT_REG_R12[]  PROGMEM;
extern const char TEXT_REG_R13[]  PROGMEM;
extern const char TEXT_REG_R14[]  PROGMEM;
extern const char TEXT_REG_R15[]  PROGMEM;
extern const char TEXT_REG_GBR[]  PROGMEM;
extern const char TEXT_REG_VBR[]  PROGMEM;
extern const char TEXT_REG_MACH[] PROGMEM;
extern const char TEXT_REG_MACL[] PROGMEM;
extern const char TEXT_REG_PC[]   PROGMEM;
extern const char TEXT_REG_PR[]   PROGMEM;

// SH-DSP register names
extern const char TEXT_REG_DSR[]  PROGMEM;
extern const char TEXT_REG_A0[]   PROGMEM;
extern const char TEXT_REG_X0[]   PROGMEM;
extern const char TEXT_REG_X1[]   PROGMEM;
extern const char TEXT_REG_Y0[]   PROGMEM;
extern const char TEXT_REG_Y1[]   PROGMEM;
extern const char TEXT_REG_MOD[]  PROGMEM;
extern const char TEXT_REG_RS[]   PROGMEM;
extern const char TEXT_REG_RE[]   PROGMEM;

// SH-2A register names
extern const char TEXT_REG_DR0[]   PROGMEM;
extern const char TEXT_REG_DR2[]   PROGMEM;
extern const char TEXT_REG_DR4[]   PROGMEM;
extern const char TEXT_REG_DR6[]   PROGMEM;
extern const char TEXT_REG_DR8[]   PROGMEM;
extern const char TEXT_REG_DR10[]  PROGMEM;
extern const char TEXT_REG_DR12[]  PROGMEM;
extern const char TEXT_REG_DR14[]  PROGMEM;
extern const char TEXT_REG_TBR[]   PROGMEM;

// SH-2E FPU register names
extern const char TEXT_REG_FR0[]   PROGMEM;
extern const char TEXT_REG_FR1[]   PROGMEM;
extern const char TEXT_REG_FR2[]   PROGMEM;
extern const char TEXT_REG_FR3[]   PROGMEM;
extern const char TEXT_REG_FR4[]   PROGMEM;
extern const char TEXT_REG_FR5[]   PROGMEM;
extern const char TEXT_REG_FR6[]   PROGMEM;
extern const char TEXT_REG_FR7[]   PROGMEM;
extern const char TEXT_REG_FR8[]   PROGMEM;
extern const char TEXT_REG_FR9[]   PROGMEM;
extern const char TEXT_REG_FR10[]  PROGMEM;
extern const char TEXT_REG_FR11[]  PROGMEM;
extern const char TEXT_REG_FR12[]  PROGMEM;
extern const char TEXT_REG_FR13[]  PROGMEM;
extern const char TEXT_REG_FR14[]  PROGMEM;
extern const char TEXT_REG_FR15[]  PROGMEM;
extern const char TEXT_REG_FPUL[]  PROGMEM;
extern const char TEXT_REG_FPSCR[] PROGMEM;

// clang-format on
}  // namespace superh
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_SUPERH__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
