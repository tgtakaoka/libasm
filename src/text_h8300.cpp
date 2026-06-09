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

#include "text_h8300.h"

namespace libasm {
namespace text {
namespace h8300 {
// clang-format off

constexpr char TEXT_H8300_LIST[]   PROGMEM = "H8/300, H8/300H, H8S/2000";
constexpr char TEXT_CPU_H8300[]    PROGMEM = "H8/300";
constexpr char TEXT_CPU_H8300H[]   PROGMEM = "H8/300H";
constexpr char TEXT_CPU_H8S2000[]  PROGMEM = "H8S/2000";

// constexpr char TEXT_ADD[]   PROGMEM = "ADD";
constexpr char TEXT_ADDS[]  PROGMEM = "ADDS";
constexpr char TEXT_ADDX[]  PROGMEM = "ADDX";
// constexpr char TEXT_AND[]   PROGMEM = "AND";
constexpr char TEXT_ANDC[]   PROGMEM = "ANDC";
// constexpr char TEXT_BAND[]  PROGMEM = "BAND";
// constexpr char TEXT_BCC[]   PROGMEM = "BCC";
// constexpr char TEXT_BCLR[]  PROGMEM = "BCLR";
// constexpr char TEXT_BCS[]   PROGMEM = "BCS";
// constexpr char TEXT_BEQ[]   PROGMEM = "BEQ";
constexpr char TEXT_BF[]    PROGMEM = "BF";
// constexpr char TEXT_BGE[]   PROGMEM = "BGE";
// constexpr char TEXT_BGT[]   PROGMEM = "BGT";
// constexpr char TEXT_BHI[]   PROGMEM = "BHI";
// constexpr char TEXT_BHS[]   PROGMEM = "BHS";
constexpr char TEXT_BIAND[] PROGMEM = "BIAND";
constexpr char TEXT_BILD[]   PROGMEM = "BILD";
constexpr char TEXT_BIOR[]  PROGMEM = "BIOR";
constexpr char TEXT_BIST[]   PROGMEM = "BIST";
constexpr char TEXT_BIXOR[]  PROGMEM = "BIXOR";
constexpr char TEXT_BLD[]    PROGMEM = "BLD";
// constexpr char TEXT_BLE[]   PROGMEM = "BLE";
// constexpr char TEXT_BLO[]   PROGMEM = "BLO";
// constexpr char TEXT_BLS[]   PROGMEM = "BLS";
// constexpr char TEXT_BLT[]   PROGMEM = "BLT";
// constexpr char TEXT_BMI[]   PROGMEM = "BMI";
// constexpr char TEXT_BNE[]   PROGMEM = "BNE";
constexpr char TEXT_BNOT[]   PROGMEM = "BNOT";
// constexpr char TEXT_BOR[]   PROGMEM = "BOR";
// constexpr char TEXT_BPL[]   PROGMEM = "BPL";
// constexpr char TEXT_BRA[]   PROGMEM = "BRA";
// constexpr char TEXT_BRN[]   PROGMEM = "BRN";
// constexpr char TEXT_BSET[]  PROGMEM = "BSET";
// constexpr char TEXT_BSR[]   PROGMEM = "BSR";
constexpr char TEXT_BST[]    PROGMEM = "BST";
// constexpr char TEXT_BT[]    PROGMEM = "BT";
// constexpr char TEXT_BT[]    PROGMEM = "BT";
// constexpr char TEXT_BTST[]  PROGMEM = "BTST";
// constexpr char TEXT_BVC[]   PROGMEM = "BVC";
// constexpr char TEXT_BVS[]   PROGMEM = "BVS";
constexpr char TEXT_BXOR[]  PROGMEM = "BXOR";
// constexpr char TEXT_CMP[]   PROGMEM = "CMP";
// constexpr char TEXT_DAA[]   PROGMEM = "DAA";
// constexpr char TEXT_DAS[]   PROGMEM = "DAS";
// constexpr char TEXT_DEC[]   PROGMEM = "DEC";
constexpr char TEXT_DIVXU[]  PROGMEM = "DIVXU";
constexpr char TEXT_DIVXS[]  PROGMEM = "DIVXS";
constexpr char TEXT_EEPMOV[] PROGMEM = "EEPMOV";
// constexpr char TEXT_EXTS[]   PROGMEM = "EXTS";
constexpr char TEXT_EXTU[]   PROGMEM = "EXTU";
// constexpr char TEXT_INC[]   PROGMEM = "INC";
// constexpr char TEXT_JMP[]   PROGMEM = "JMP";
// constexpr char TEXT_JSR[]   PROGMEM = "JSR";
// constexpr char TEXT_LDC[]   PROGMEM = "LDC";
// constexpr char TEXT_MOV[]   PROGMEM = "MOV";
constexpr char TEXT_MOVFPE[] PROGMEM = "MOVFPE";
constexpr char TEXT_MOVTPE[] PROGMEM = "MOVTPE";
constexpr char TEXT_MULXU[]  PROGMEM = "MULXU";
constexpr char TEXT_MULXS[]  PROGMEM = "MULXS";
// constexpr char TEXT_NEG[]   PROGMEM = "NEG";
// constexpr char TEXT_NOP[]   PROGMEM = "NOP";
// constexpr char TEXT_NOT[]   PROGMEM = "NOT";
// constexpr char TEXT_OR[]    PROGMEM = "OR";
constexpr char TEXT_ORC[]    PROGMEM = "ORC";
// constexpr char TEXT_POP[]   PROGMEM = "POP";
// constexpr char TEXT_PUSH[]  PROGMEM = "PUSH";
constexpr char TEXT_ROTL[]   PROGMEM = "ROTL";
constexpr char TEXT_ROTR[]   PROGMEM = "ROTR";
constexpr char TEXT_ROTXL[]  PROGMEM = "ROTXL";
constexpr char TEXT_ROTXR[]  PROGMEM = "ROTXR";
constexpr char TEXT_RTE[]   PROGMEM = "RTE";
// constexpr char TEXT_RTS[]   PROGMEM = "RTS";
constexpr char TEXT_SHAL[]   PROGMEM = "SHAL";
constexpr char TEXT_SHAR[]   PROGMEM = "SHAR";
constexpr char TEXT_SHLL[]  PROGMEM = "SHLL";
constexpr char TEXT_SHLR[]   PROGMEM = "SHLR";
constexpr char TEXT_SLEEP[]  PROGMEM = "SLEEP";
// constexpr char TEXT_STC[]   PROGMEM = "STC";
constexpr char TEXT_STM[]   PROGMEM = "STM";
// constexpr char TEXT_SUB[]   PROGMEM = "SUB";
constexpr char TEXT_SUBS[]  PROGMEM = "SUBS";
constexpr char TEXT_SUBX[]  PROGMEM = "SUBX";
constexpr char TEXT_TAS[]   PROGMEM = "TAS";
constexpr char TEXT_TRAPA[] PROGMEM = "TRAPA";
// constexpr char TEXT_XOR[]   PROGMEM = "XOR";
constexpr char TEXT_XORC[]   PROGMEM = "XORC";

// constexpr char TEXT_REG_CCR[] PROGMEM = "CCR";
// constexpr char TEXT_REG_R0[]  PROGMEM = "R0";
constexpr char TEXT_REG_R0H[] PROGMEM = "R0H";
constexpr char TEXT_REG_R0L[] PROGMEM = "R0L";
// constexpr char TEXT_REG_R1[]  PROGMEM = "R1";
constexpr char TEXT_REG_R1H[] PROGMEM = "R1H";
constexpr char TEXT_REG_R1L[] PROGMEM = "R1L";
// constexpr char TEXT_REG_R2[]  PROGMEM = "R2";
constexpr char TEXT_REG_R2H[] PROGMEM = "R2H";
constexpr char TEXT_REG_R2L[] PROGMEM = "R2L";
// constexpr char TEXT_REG_R3[]  PROGMEM = "R3";
constexpr char TEXT_REG_R3H[] PROGMEM = "R3H";
constexpr char TEXT_REG_R3L[] PROGMEM = "R3L";
// constexpr char TEXT_REG_R4[]  PROGMEM = "R4";
constexpr char TEXT_REG_R4H[] PROGMEM = "R4H";
constexpr char TEXT_REG_R4L[] PROGMEM = "R4L";
// constexpr char TEXT_REG_R5[]  PROGMEM = "R5";
constexpr char TEXT_REG_R5H[] PROGMEM = "R5H";
constexpr char TEXT_REG_R5L[] PROGMEM = "R5L";
// constexpr char TEXT_REG_R6[]  PROGMEM = "R6";
constexpr char TEXT_REG_R6H[] PROGMEM = "R6H";
constexpr char TEXT_REG_R6L[] PROGMEM = "R6L";
// constexpr char TEXT_REG_R7[]  PROGMEM = "R7";
constexpr char TEXT_REG_R7H[] PROGMEM = "R7H";
constexpr char TEXT_REG_R7L[] PROGMEM = "R7L";
// constexpr char TEXT_REG_SP[]  PROGMEM = "SP";

constexpr char TEXT_REG_E0[]  PROGMEM = "E0";
constexpr char TEXT_REG_E1[]  PROGMEM = "E1";
constexpr char TEXT_REG_E2[]  PROGMEM = "E2";
constexpr char TEXT_REG_E3[]  PROGMEM = "E3";
constexpr char TEXT_REG_E4[]  PROGMEM = "E4";
constexpr char TEXT_REG_E5[]  PROGMEM = "E5";
constexpr char TEXT_REG_E6[]  PROGMEM = "E6";
constexpr char TEXT_REG_E7[]  PROGMEM = "E7";

constexpr char TEXT_REG_ER0[] PROGMEM = "ER0";
constexpr char TEXT_REG_ER1[] PROGMEM = "ER1";
constexpr char TEXT_REG_ER2[] PROGMEM = "ER2";
constexpr char TEXT_REG_ER3[] PROGMEM = "ER3";
constexpr char TEXT_REG_ER4[] PROGMEM = "ER4";
constexpr char TEXT_REG_ER5[] PROGMEM = "ER5";
constexpr char TEXT_REG_ER6[] PROGMEM = "ER6";
constexpr char TEXT_REG_ER7[] PROGMEM = "ER7";

constexpr char TEXT_REG_EXR[] PROGMEM = "EXR";

// Options
constexpr char OPT_BOOL_ADVANCED_MODE[] PROGMEM = "advanced-mode";
constexpr char OPT_DESC_ADVANCED_MODE[] PROGMEM = "H8/300H 24-bit advanced mode";
constexpr char OPT_BOOL_SP_ALIAS[] PROGMEM = "sp-alias";
constexpr char OPT_DESC_SP_ALIAS[] PROGMEM = "emit SP alias for ER7/R7 in indirect addressing";

// clang-format on
}  // namespace h8300
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
