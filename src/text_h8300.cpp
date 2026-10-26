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

constexpr char TEXT_H8300_LIST[]   = "H8/300, H8/300H, H8S/2000, H8S/2600";
constexpr char TEXT_CPU_H8300[]    = "H8/300";
constexpr char TEXT_CPU_H8300H[]   = "H8/300H";
constexpr char TEXT_CPU_H8S2000[]  = "H8S/2000";
constexpr char TEXT_CPU_H8S2600[]  = "H8S/2600";

// constexpr char TEXT_ADD[]   = "ADD";
// constexpr char TEXT_ADDS[]  = "ADDS";
// constexpr char TEXT_ADDX[]  = "ADDX";
// constexpr char TEXT_AND[]   = "AND";
// constexpr char TEXT_ANDC[]   = "ANDC";
// constexpr char TEXT_BAND[]  = "BAND";
// constexpr char TEXT_BCC[]   = "BCC";
// constexpr char TEXT_BCLR[]  = "BCLR";
// constexpr char TEXT_BCS[]   = "BCS";
// constexpr char TEXT_BEQ[]   = "BEQ";
// constexpr char TEXT_BF[]    = "BF";
// constexpr char TEXT_BGE[]   = "BGE";
// constexpr char TEXT_BGT[]   = "BGT";
// constexpr char TEXT_BHI[]   = "BHI";
// constexpr char TEXT_BHS[]   = "BHS";
// constexpr char TEXT_BIAND[] = "BIAND";
constexpr char TEXT_BILD[]   = "BILD";
// constexpr char TEXT_BIOR[]  = "BIOR";
constexpr char TEXT_BIST[]   = "BIST";
constexpr char TEXT_BIXOR[]  = "BIXOR";
// constexpr char TEXT_BLD[]    = "BLD";
// constexpr char TEXT_BLE[]   = "BLE";
// constexpr char TEXT_BLO[]   = "BLO";
// constexpr char TEXT_BLS[]   = "BLS";
// constexpr char TEXT_BLT[]   = "BLT";
// constexpr char TEXT_BMI[]   = "BMI";
// constexpr char TEXT_BNE[]   = "BNE";
// constexpr char TEXT_BNOT[]   = "BNOT";
// constexpr char TEXT_BOR[]   = "BOR";
// constexpr char TEXT_BPL[]   = "BPL";
// constexpr char TEXT_BRA[]   = "BRA";
// constexpr char TEXT_BRN[]   = "BRN";
// constexpr char TEXT_BSET[]  = "BSET";
// constexpr char TEXT_BSR[]   = "BSR";
// constexpr char TEXT_BST[]    = "BST";
// constexpr char TEXT_BT[]    = "BT";
// constexpr char TEXT_BT[]    = "BT";
// constexpr char TEXT_BTST[]  = "BTST";
// constexpr char TEXT_BVC[]   = "BVC";
// constexpr char TEXT_BVS[]   = "BVS";
// constexpr char TEXT_BXOR[]  = "BXOR";
// constexpr char TEXT_CLRMAC[] = "CLRMAC";
// constexpr char TEXT_CMP[]   = "CMP";
// constexpr char TEXT_DAA[]   = "DAA";
// constexpr char TEXT_DAS[]   = "DAS";
// constexpr char TEXT_DEC[]   = "DEC";
// constexpr char TEXT_DIVXU[]  = "DIVXU";
constexpr char TEXT_DIVXS[]  = "DIVXS";
constexpr char TEXT_EEPMOV[] = "EEPMOV";
// constexpr char TEXT_EXTS[]   = "EXTS";
// constexpr char TEXT_EXTU[]   = "EXTU";
// constexpr char TEXT_INC[]   = "INC";
// constexpr char TEXT_JMP[]   = "JMP";
// constexpr char TEXT_JSR[]   = "JSR";
// constexpr char TEXT_LDC[]   = "LDC";
constexpr char TEXT_LDMAC[]  = "LDMAC";
// constexpr char TEXT_MAC[]   = "MAC";
// constexpr char TEXT_MOV[]   = "MOV";
// constexpr char TEXT_MOVFPE[] = "MOVFPE";
// constexpr char TEXT_MOVTPE[] = "MOVTPE";
// constexpr char TEXT_MULXU[]  = "MULXU";
constexpr char TEXT_MULXS[]  = "MULXS";
// constexpr char TEXT_NEG[]   = "NEG";
// constexpr char TEXT_NOP[]   = "NOP";
// constexpr char TEXT_NOT[]   = "NOT";
// constexpr char TEXT_OR[]    = "OR";
// constexpr char TEXT_ORC[]    = "ORC";
// constexpr char TEXT_POP[]   = "POP";
// constexpr char TEXT_PUSH[]  = "PUSH";
// constexpr char TEXT_ROTL[]   = "ROTL";
// constexpr char TEXT_ROTR[]   = "ROTR";
// constexpr char TEXT_ROTXL[]  = "ROTXL";
// constexpr char TEXT_ROTXR[]  = "ROTXR";
// constexpr char TEXT_RTE[]   = "RTE";
// constexpr char TEXT_RTS[]   = "RTS";
// constexpr char TEXT_SHAL[]   = "SHAL";
// constexpr char TEXT_SHAR[]   = "SHAR";
// constexpr char TEXT_SHLL[]  = "SHLL";
// constexpr char TEXT_SHLR[]   = "SHLR";
// constexpr char TEXT_SLEEP[]  = "SLEEP";
// constexpr char TEXT_STC[]   = "STC";
// constexpr char TEXT_STM[]   = "STM";
constexpr char TEXT_STMAC[]  = "STMAC";
// constexpr char TEXT_SUB[]   = "SUB";
// constexpr char TEXT_SUBS[]  = "SUBS";
// constexpr char TEXT_SUBX[]  = "SUBX";
// constexpr char TEXT_TAS[]   = "TAS";
// constexpr char TEXT_TRAPA[]  = "TRAPA";
// constexpr char TEXT_XOR[]   = "XOR";
// constexpr char TEXT_XORC[]   = "XORC";

// constexpr char TEXT_REG_CCR[] = "CCR";
// constexpr char TEXT_REG_R0[]  = "R0";
constexpr char TEXT_REG_R0H[] = "R0H";
constexpr char TEXT_REG_R0L[] = "R0L";
// constexpr char TEXT_REG_R1[]  = "R1";
constexpr char TEXT_REG_R1H[] = "R1H";
constexpr char TEXT_REG_R1L[] = "R1L";
// constexpr char TEXT_REG_R2[]  = "R2";
constexpr char TEXT_REG_R2H[] = "R2H";
constexpr char TEXT_REG_R2L[] = "R2L";
// constexpr char TEXT_REG_R3[]  = "R3";
constexpr char TEXT_REG_R3H[] = "R3H";
constexpr char TEXT_REG_R3L[] = "R3L";
// constexpr char TEXT_REG_R4[]  = "R4";
constexpr char TEXT_REG_R4H[] = "R4H";
constexpr char TEXT_REG_R4L[] = "R4L";
// constexpr char TEXT_REG_R5[]  = "R5";
constexpr char TEXT_REG_R5H[] = "R5H";
constexpr char TEXT_REG_R5L[] = "R5L";
// constexpr char TEXT_REG_R6[]  = "R6";
constexpr char TEXT_REG_R6H[] = "R6H";
constexpr char TEXT_REG_R6L[] = "R6L";
// constexpr char TEXT_REG_R7[]  = "R7";
constexpr char TEXT_REG_R7H[] = "R7H";
constexpr char TEXT_REG_R7L[] = "R7L";
// constexpr char TEXT_REG_SP[]  = "SP";

constexpr char TEXT_REG_E0[]  = "E0";
constexpr char TEXT_REG_E1[]  = "E1";
constexpr char TEXT_REG_E2[]  = "E2";
constexpr char TEXT_REG_E3[]  = "E3";
constexpr char TEXT_REG_E4[]  = "E4";
constexpr char TEXT_REG_E5[]  = "E5";
constexpr char TEXT_REG_E6[]  = "E6";
constexpr char TEXT_REG_E7[]  = "E7";

constexpr char TEXT_REG_ER0[] = "ER0";
constexpr char TEXT_REG_ER1[] = "ER1";
constexpr char TEXT_REG_ER2[] = "ER2";
constexpr char TEXT_REG_ER3[] = "ER3";
constexpr char TEXT_REG_ER4[] = "ER4";
constexpr char TEXT_REG_ER5[] = "ER5";
constexpr char TEXT_REG_ER6[] = "ER6";
constexpr char TEXT_REG_ER7[] = "ER7";

constexpr char TEXT_REG_EXR[]  = "EXR";
// constexpr char TEXT_REG_MACH[] = "MACH";
// constexpr char TEXT_REG_MACL[] = "MACL";

// Options
constexpr char OPT_BOOL_ADVANCED_MODE[] = "advanced-mode";
constexpr char OPT_DESC_ADVANCED_MODE[] = "H8/300H 24-bit advanced mode";
constexpr char OPT_BOOL_SP_ALIAS[] = "sp-alias";
constexpr char OPT_DESC_SP_ALIAS[] = "emit SP alias for ER7/R7 in indirect addressing";

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
