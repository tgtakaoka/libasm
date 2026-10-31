/*
 * Copyright 2025 Tadashi G. Takaoka
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

#include "text_h8500.h"

namespace libasm {
namespace text {
namespace h8500 {
// clang-format off

constexpr char TEXT_H8500_LIST[] = "H8/510, H8/520, H8/530";
constexpr char TEXT_CPU_H8500[]  = "H8/500";
constexpr char TEXT_CPU_H8510[]  = "H8/510";
constexpr char TEXT_CPU_H8520[]  = "H8/520";
constexpr char TEXT_CPU_H8530[]  = "H8/530";

constexpr char OPT_BOOL_MAX_MODE[] = "maximum-mode";
constexpr char OPT_DESC_MAX_MODE[] = "enable maximum mode";
constexpr char OPT_BOOL_FP_ALIAS[] = "fp-alias";
constexpr char OPT_DESC_FP_ALIAS[] = "emit FP alias for R6";
constexpr char OPT_BOOL_SP_ALIAS[] = "sp-alias";
constexpr char OPT_DESC_SP_ALIAS[] = "emit SP alias for R7 in indirect addressing";

// constexpr char TEXT_MAXMODE[] = "MAXMODE";

// H8/500-specific instruction mnemonics. MOV/ADD/CMP keep only the stem; the
// ":x" class is carried as an Entry attribute (see InsnClass).
// constexpr char TEXT_ANDC[]    = "ANDC";
// constexpr char TEXT_BNOT[]    = "BNOT";
// constexpr char TEXT_DADD[]    = "DADD";
// constexpr char TEXT_DIVXU[]   = "DIVXU";
constexpr char TEXT_DSUB[]    = "DSUB";
// constexpr char TEXT_EXTU[]    = "EXTU";
// constexpr char TEXT_MOVFPE[]  = "MOVFPE";
// constexpr char TEXT_MOVTPE[]  = "MOVTPE";
// constexpr char TEXT_MULXU[]   = "MULXU";
// constexpr char TEXT_ORC[]     = "ORC";
constexpr char TEXT_PJMP[]    = "PJMP";
constexpr char TEXT_PJSR[]    = "PJSR";
constexpr char TEXT_PRTD[]    = "PRTD";
constexpr char TEXT_PRTS[]    = "PRTS";
// constexpr char TEXT_ROTL[]    = "ROTL";
// constexpr char TEXT_ROTR[]    = "ROTR";
// constexpr char TEXT_ROTXL[]   = "ROTXL";
// constexpr char TEXT_ROTXR[]   = "ROTXR";
// constexpr char TEXT_RTD[]     = "RTD";
constexpr char TEXT_SCB_EQ[]  = "SCB/EQ";
constexpr char TEXT_SCB_F[]   = "SCB/F";
constexpr char TEXT_SCB_NE[]  = "SCB/NE";
// constexpr char TEXT_SHAL[]    = "SHAL";
// constexpr char TEXT_SHAR[]    = "SHAR";
// constexpr char TEXT_SHLR[]    = "SHLR";
// constexpr char TEXT_SLEEP[]   = "SLEEP";
constexpr char TEXT_TRAP_VS[] = "TRAP/VS";
// constexpr char TEXT_TRAPA[]   = "TRAPA";
// constexpr char TEXT_XORC[]    = "XORC";

// H8/500-specific register names
constexpr char TEXT_REG_BR[] = "BR";
constexpr char TEXT_REG_CP[] = "CP";
constexpr char TEXT_REG_EP[] = "EP";
// constexpr char TEXT_REG_FP[] = "FP";
constexpr char TEXT_REG_TP[] = "TP";

// clang-format on
}  // namespace h8500
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
