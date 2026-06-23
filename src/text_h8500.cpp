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

constexpr char TEXT_H8500_LIST[] PROGMEM = "H8/500, H8/520, H8/530";
constexpr char TEXT_CPU_H8500[]  PROGMEM = "H8/500";
constexpr char TEXT_CPU_H8520[]  PROGMEM = "H8/520";
constexpr char TEXT_CPU_H8530[]  PROGMEM = "H8/530";

constexpr char OPT_BOOL_MAX_MODE[] PROGMEM = "maximum-mode";
constexpr char OPT_DESC_MAX_MODE[] PROGMEM = "enable maximum mode";
constexpr char OPT_BOOL_FP_ALIAS[] PROGMEM = "fp-alias";
constexpr char OPT_DESC_FP_ALIAS[] PROGMEM = "emit FP alias for R6";
constexpr char OPT_BOOL_SP_ALIAS[] PROGMEM = "sp-alias";
constexpr char OPT_DESC_SP_ALIAS[] PROGMEM = "emit SP alias for R7 in indirect addressing";

// constexpr char TEXT_MAXMODE[] PROGMEM = "MAXMODE";

// H8/500-specific instruction mnemonics. MOV/ADD/CMP keep only the stem; the
// ":x" class is carried as an Entry attribute (see InsnClass).
// constexpr char TEXT_ANDC[]    PROGMEM = "ANDC";
// constexpr char TEXT_BNOT[]    PROGMEM = "BNOT";
// constexpr char TEXT_DADD[]    PROGMEM = "DADD";
// constexpr char TEXT_DIVXU[]   PROGMEM = "DIVXU";
constexpr char TEXT_DSUB[]    PROGMEM = "DSUB";
// constexpr char TEXT_EXTU[]    PROGMEM = "EXTU";
// constexpr char TEXT_MOVFPE[]  PROGMEM = "MOVFPE";
// constexpr char TEXT_MOVTPE[]  PROGMEM = "MOVTPE";
// constexpr char TEXT_MULXU[]   PROGMEM = "MULXU";
// constexpr char TEXT_ORC[]     PROGMEM = "ORC";
constexpr char TEXT_PJMP[]    PROGMEM = "PJMP";
constexpr char TEXT_PJSR[]    PROGMEM = "PJSR";
constexpr char TEXT_PRTD[]    PROGMEM = "PRTD";
constexpr char TEXT_PRTS[]    PROGMEM = "PRTS";
// constexpr char TEXT_ROTL[]    PROGMEM = "ROTL";
// constexpr char TEXT_ROTR[]    PROGMEM = "ROTR";
// constexpr char TEXT_ROTXL[]   PROGMEM = "ROTXL";
// constexpr char TEXT_ROTXR[]   PROGMEM = "ROTXR";
// constexpr char TEXT_RTD[]     PROGMEM = "RTD";
constexpr char TEXT_SCB_EQ[]  PROGMEM = "SCB/EQ";
constexpr char TEXT_SCB_F[]   PROGMEM = "SCB/F";
constexpr char TEXT_SCB_NE[]  PROGMEM = "SCB/NE";
// constexpr char TEXT_SHAL[]    PROGMEM = "SHAL";
// constexpr char TEXT_SHAR[]    PROGMEM = "SHAR";
// constexpr char TEXT_SHLR[]    PROGMEM = "SHLR";
// constexpr char TEXT_SLEEP[]   PROGMEM = "SLEEP";
constexpr char TEXT_TRAP_VS[] PROGMEM = "TRAP/VS";
// constexpr char TEXT_TRAPA[]   PROGMEM = "TRAPA";
// constexpr char TEXT_XORC[]    PROGMEM = "XORC";

// H8/500-specific register names
constexpr char TEXT_REG_BR[] PROGMEM = "BR";
constexpr char TEXT_REG_CP[] PROGMEM = "CP";
constexpr char TEXT_REG_EP[] PROGMEM = "EP";
// constexpr char TEXT_REG_FP[] PROGMEM = "FP";
constexpr char TEXT_REG_TP[] PROGMEM = "TP";

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
