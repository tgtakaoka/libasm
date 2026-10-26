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

extern const char TEXT_SUPERH_LIST[];
extern const char TEXT_CPU_SH1[];
extern const char TEXT_CPU_SH2[];
extern const char TEXT_CPU_SH_DSP[];
extern const char TEXT_CPU_SH2E[];
extern const char TEXT_CPU_SH2A[];
extern const char TEXT_FPU_NONE[];
extern const char TEXT_FPU_SH2E[];
extern const char TEXT_FPU_SH2A[];

// Instructions reused from common
using common::TEXT_ADD;
using common::TEXT_ADDC;
using common::TEXT_AND;
using common::TEXT_BF;
using common::TEXT_BRA;
using common::TEXT_BSR;
using common::TEXT_BST;
using common::TEXT_BT;
using common::TEXT_CLRMAC;
using common::TEXT_EXTS;
using common::TEXT_JMP;
using common::TEXT_JSR;
using common::TEXT_LDC;
using common::TEXT_LDS;
using common::TEXT_MOV;
using common::TEXT_MUL;
using common::TEXT_MULS;
using common::TEXT_MULU;
using common::TEXT_NEG;
using common::TEXT_NOP;
using common::TEXT_NOT;
using common::TEXT_OR;
using common::TEXT_ROTL;
using common::TEXT_ROTR;
using common::TEXT_RTE;
using common::TEXT_RTS;
using common::TEXT_SHAL;
using common::TEXT_SHAR;
using common::TEXT_SHLL;
using common::TEXT_SHLR;
using common::TEXT_SLEEP;
using common::TEXT_STC;
using common::TEXT_STS;
using common::TEXT_SUB;
using common::TEXT_SUBC;
using common::TEXT_SWAP;
using common::TEXT_TRAPA;
using common::TEXT_TST;
using common::TEXT_XOR;

// SH-specific instruction mnemonics
extern const char TEXT_ADDV[];
extern const char TEXT_BF_S[];      // "BF/S"
extern const char TEXT_BRAF[];
extern const char TEXT_BSRF[];
extern const char TEXT_BT_S[];      // "BT/S"
extern const char TEXT_CLRT[];
extern const char TEXT_CMP_EQ[];    // "CMP/EQ"
extern const char TEXT_CMP_GE[];    // "CMP/GE"
extern const char TEXT_CMP_GT[];    // "CMP/GT"
extern const char TEXT_CMP_HI[];    // "CMP/HI"
extern const char TEXT_CMP_HS[];    // "CMP/HS"
extern const char TEXT_CMP_PL[];    // "CMP/PL"
extern const char TEXT_CMP_PZ[];    // "CMP/PZ"
extern const char TEXT_CMP_STR[];   // "CMP/STR"
extern const char TEXT_DIV0S[];
extern const char TEXT_DIV0U[];
extern const char TEXT_DIV1[];
extern const char TEXT_DT[];
extern const char TEXT_LDRE[];
extern const char TEXT_LDRS[];
extern const char TEXT_MOVA[];
extern const char TEXT_MOVT[];
extern const char TEXT_NEGC[];
extern const char TEXT_ROTCL[];
extern const char TEXT_ROTCR[];
extern const char TEXT_SETRC[];
extern const char TEXT_SETT[];
extern const char TEXT_SHLL2[];
extern const char TEXT_SHLL8[];
extern const char TEXT_SHLL16[];
extern const char TEXT_SHLR2[];
extern const char TEXT_SHLR8[];
extern const char TEXT_SHLR16[];
extern const char TEXT_SUBV[];
extern const char TEXT_XTRCT[];

// SH-2A instruction mnemonics
using common::TEXT_BCLR;
using common::TEXT_BSET;
using common::TEXT_BLD;
using common::TEXT_DIVS;
using common::TEXT_DIVU;
extern const char TEXT_JSR_N[];     // "JSR/N"
extern const char TEXT_LDBANK[];
extern const char TEXT_MOVI20[];
extern const char TEXT_MOVI20S[];
extern const char TEXT_MOVRT[];
extern const char TEXT_MULR[];
extern const char TEXT_NOTT[];
extern const char TEXT_PREF[];
extern const char TEXT_RESBANK[];
extern const char TEXT_RTS_N[];     // "RTS/N"
extern const char TEXT_RTV_N[];     // "RTV/N"
extern const char TEXT_SHAD[];
extern const char TEXT_SHLD[];
extern const char TEXT_STBANK[];

// SH-2A FPU additions
extern const char TEXT_FCNVDS[];
extern const char TEXT_FCNVSD[];

// SH-2E FPU instruction mnemonics
using common::TEXT_FABS;
using common::TEXT_FADD;
extern const char TEXT_FCMP_EQ[];   // "FCMP/EQ"
extern const char TEXT_FCMP_GT[];   // "FCMP/GT"
using common::TEXT_FDIV;
extern const char TEXT_FLDI0[];
extern const char TEXT_FLDI1[];
extern const char TEXT_FLDS[];
using common::TEXT_FLOAT;
extern const char TEXT_FMAC[];
extern const char TEXT_FMOV[];
extern const char TEXT_FMOV_S[];    // "FMOV.S"
extern const char TEXT_MAC_W[];     // "MAC.W" (SH-1/2, 16x16)
extern const char TEXT_MAC_L[];     // "MAC.L" (SH-2, 32x32)
extern const char TEXT_CLIPS[];
extern const char TEXT_CLIPU[];
extern const char TEXT_DMULS[];
extern const char TEXT_DMULU[];
extern const char TEXT_MOVML[];
extern const char TEXT_MOVMU[];
using common::TEXT_EXTU;
using common::TEXT_TAS;
using common::TEXT_FMUL;
using common::TEXT_FNEG;
extern const char TEXT_FSTS[];
using common::TEXT_FSUB;
extern const char TEXT_FTRC[];

// Register names
using common::TEXT_REG_SR;
using common::TEXT_REG_SP;  // alias for R15

extern const char TEXT_REG_GBR[];
using common::TEXT_REG_VBR;
using common::TEXT_REG_MACH;
using common::TEXT_REG_MACL;
using common::TEXT_REG_PC;
extern const char TEXT_REG_PR[];

// SH-DSP register names
using common::TEXT_REG_DSR;
using common::TEXT_REG_A0;
using common::TEXT_REG_X0;
using common::TEXT_REG_X1;
extern const char TEXT_REG_Y0[];
extern const char TEXT_REG_Y1[];
using common::TEXT_REG_MOD;
using common::TEXT_REG_RS;
using common::TEXT_REG_RE;

// SH-2A register names
extern const char TEXT_REG_TBR[];

// SH-2E FPU register names
extern const char TEXT_REG_FPUL[];
extern const char TEXT_REG_FPSCR[];

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
