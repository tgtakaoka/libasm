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

constexpr char TEXT_H8500_LIST[] PROGMEM = "H8/500, H8/520, H8/532";
constexpr char TEXT_CPU_H8500[]  PROGMEM = "H8/500";
constexpr char TEXT_CPU_H8520[]  PROGMEM = "H8/520";
constexpr char TEXT_CPU_H8532[]  PROGMEM = "H8/532";

// H8/500-specific instruction mnemonics
constexpr char TEXT_ADD_G[]  PROGMEM = "ADD:G";
constexpr char TEXT_ADD_Q[]  PROGMEM = "ADD:Q";
constexpr char TEXT_ADDS[]   PROGMEM = "ADDS";
constexpr char TEXT_ADDX[]   PROGMEM = "ADDX";
constexpr char TEXT_ANDC[]   PROGMEM = "ANDC";
constexpr char TEXT_BF[]     PROGMEM = "BF";
constexpr char TEXT_BNOT[]   PROGMEM = "BNOT";
constexpr char TEXT_CMP_E[]  PROGMEM = "CMP:E";
constexpr char TEXT_CMP_G[]  PROGMEM = "CMP:G";
constexpr char TEXT_CMP_I[]  PROGMEM = "CMP:I";
constexpr char TEXT_DADD[]   PROGMEM = "DADD";
constexpr char TEXT_DIVXU[]  PROGMEM = "DIVXU";
constexpr char TEXT_DSUB[]   PROGMEM = "DSUB";
constexpr char TEXT_EXTU[]   PROGMEM = "EXTU";
constexpr char TEXT_LINK[]   PROGMEM = "LINK";
constexpr char TEXT_MOV_E[]  PROGMEM = "MOV:E";
constexpr char TEXT_MOV_F[]  PROGMEM = "MOV:F";
constexpr char TEXT_MOV_G[]  PROGMEM = "MOV:G";
constexpr char TEXT_MOV_I[]  PROGMEM = "MOV:I";
constexpr char TEXT_MOV_L[]  PROGMEM = "MOV:L";
constexpr char TEXT_MOV_S[]  PROGMEM = "MOV:S";
constexpr char TEXT_MOVFPE[] PROGMEM = "MOVFPE";
constexpr char TEXT_MOVTPE[] PROGMEM = "MOVTPE";
constexpr char TEXT_MULXU[]  PROGMEM = "MULXU";
constexpr char TEXT_ORC[]    PROGMEM = "ORC";
constexpr char TEXT_PJMP[]   PROGMEM = "PJMP";
constexpr char TEXT_PJSR[]   PROGMEM = "PJSR";
constexpr char TEXT_PRTD[]   PROGMEM = "PRTD";
constexpr char TEXT_PRTS[]   PROGMEM = "PRTS";
constexpr char TEXT_RTD[]    PROGMEM = "RTD";
constexpr char TEXT_RTE[]    PROGMEM = "RTE";
constexpr char TEXT_ROTL[]   PROGMEM = "ROTL";
constexpr char TEXT_ROTR[]   PROGMEM = "ROTR";
constexpr char TEXT_ROTXL[]  PROGMEM = "ROTXL";
constexpr char TEXT_ROTXR[]  PROGMEM = "ROTXR";
constexpr char TEXT_SCB_EQ[] PROGMEM = "SCB/EQ";
constexpr char TEXT_SCB_F[]  PROGMEM = "SCB/F";
constexpr char TEXT_SCB_NE[] PROGMEM = "SCB/NE";
constexpr char TEXT_SHAL[]   PROGMEM = "SHAL";
constexpr char TEXT_SHAR[]   PROGMEM = "SHAR";
constexpr char TEXT_SHLL[]   PROGMEM = "SHLL";
constexpr char TEXT_SHLR[]   PROGMEM = "SHLR";
constexpr char TEXT_SLEEP[]  PROGMEM = "SLEEP";
constexpr char TEXT_STM[]    PROGMEM = "STM";
constexpr char TEXT_SUBS[]   PROGMEM = "SUBS";
constexpr char TEXT_SUBX[]   PROGMEM = "SUBX";
constexpr char TEXT_TAS[]    PROGMEM = "TAS";
constexpr char TEXT_TRAP_VS[] PROGMEM = "TRAP/VS";
constexpr char TEXT_TRAPA[]  PROGMEM = "TRAPA";
constexpr char TEXT_UNLK[]   PROGMEM = "UNLK";
constexpr char TEXT_XORC[]   PROGMEM = "XORC";

// H8/500-specific register names
constexpr char TEXT_REG_BR[] PROGMEM = "BR";
constexpr char TEXT_REG_CP[] PROGMEM = "CP";
constexpr char TEXT_REG_EP[] PROGMEM = "EP";
constexpr char TEXT_REG_FP[] PROGMEM = "FP";
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
