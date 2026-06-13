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

constexpr char TEXT_H8500_LIST[] PROGMEM = "H8/500, H8/520";
constexpr char TEXT_CPU_H8500[]  PROGMEM = "H8/500";
constexpr char TEXT_CPU_H8520[]  PROGMEM = "H8/520";

constexpr char OPT_BOOL_FP_ALIAS[] PROGMEM = "fp-alias";
constexpr char OPT_DESC_FP_ALIAS[] PROGMEM = "emit FP alias for R6 in indirect addressing";
constexpr char OPT_BOOL_SP_ALIAS[] PROGMEM = "sp-alias";
constexpr char OPT_DESC_SP_ALIAS[] PROGMEM = "emit SP alias for R7 in indirect addressing";

// constexpr char TEXT_MAXMODE[] PROGMEM = "MAXMODE";

// H8/500-specific instruction mnemonics
constexpr char TEXT_ADD_G[]   PROGMEM = "ADD:G";
constexpr char TEXT_ADD_Q[]   PROGMEM = "ADD:Q";
constexpr char TEXT_CMP_E[]   PROGMEM = "CMP:E";
constexpr char TEXT_CMP_G[]   PROGMEM = "CMP:G";
constexpr char TEXT_CMP_I[]   PROGMEM = "CMP:I";
// constexpr char TEXT_DADD[]    PROGMEM = "DADD";
constexpr char TEXT_DSUB[]    PROGMEM = "DSUB";
constexpr char TEXT_MOV_E[]   PROGMEM = "MOV:E";
constexpr char TEXT_MOV_F[]   PROGMEM = "MOV:F";
constexpr char TEXT_MOV_G[]   PROGMEM = "MOV:G";
constexpr char TEXT_MOV_I[]   PROGMEM = "MOV:I";
constexpr char TEXT_MOV_L[]   PROGMEM = "MOV:L";
constexpr char TEXT_MOV_S[]   PROGMEM = "MOV:S";
constexpr char TEXT_PJMP[]    PROGMEM = "PJMP";
constexpr char TEXT_PJSR[]    PROGMEM = "PJSR";
constexpr char TEXT_PRTD[]    PROGMEM = "PRTD";
constexpr char TEXT_PRTS[]    PROGMEM = "PRTS";
// constexpr char TEXT_RTD[]     PROGMEM = "RTD";
constexpr char TEXT_SCB_EQ[]  PROGMEM = "SCB/EQ";
constexpr char TEXT_SCB_F[]   PROGMEM = "SCB/F";
constexpr char TEXT_SCB_NE[]  PROGMEM = "SCB/NE";
constexpr char TEXT_TRAP_VS[] PROGMEM = "TRAP/VS";

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
