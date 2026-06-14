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

#include "text_h16.h"

namespace libasm {
namespace text {
namespace h16 {
// clang-format off

// CPU list and names
constexpr char TEXT_H16_LIST[]     PROGMEM = "HD641016";
constexpr char TEXT_CPU_HD641016[] PROGMEM = "HD641016";

// H16-specific instruction mnemonics (sorted)
constexpr char TEXT_ADD_G[]  PROGMEM = "ADD:G";
constexpr char TEXT_ADD_Q[]  PROGMEM = "ADD:Q";
constexpr char TEXT_ADD_R[]  PROGMEM = "ADD:R";
constexpr char TEXT_ADD_RQ[] PROGMEM = "ADD:RQ";
constexpr char TEXT_ANDC[]   PROGMEM = "ANDC";
constexpr char TEXT_BCC_G[]  PROGMEM = "BCC:G";
constexpr char TEXT_BCLR_X[] PROGMEM = "BCLR";  // duplicate for table use
constexpr char TEXT_BCS_G[]  PROGMEM = "BCS:G";
constexpr char TEXT_BEQ_G[]  PROGMEM = "BEQ:G";
constexpr char TEXT_BF_G[]   PROGMEM = "BF:G";
constexpr char TEXT_BGE_G[]  PROGMEM = "BGE:G";
constexpr char TEXT_BGT_G[]  PROGMEM = "BGT:G";
constexpr char TEXT_BHI_G[]  PROGMEM = "BHI:G";
constexpr char TEXT_BLE_G[]  PROGMEM = "BLE:G";
constexpr char TEXT_BLS_G[]  PROGMEM = "BLS:G";
constexpr char TEXT_BLT_G[]  PROGMEM = "BLT:G";
constexpr char TEXT_BMI_G[]  PROGMEM = "BMI:G";
constexpr char TEXT_BNE_G[]  PROGMEM = "BNE:G";
constexpr char TEXT_BNOT[]   PROGMEM = "BNOT";
constexpr char TEXT_BPL_G[]  PROGMEM = "BPL:G";
constexpr char TEXT_BT_G[]   PROGMEM = "BT:G";
constexpr char TEXT_BVC_G[]  PROGMEM = "BVC:G";
constexpr char TEXT_BVS_G[]  PROGMEM = "BVS:G";
constexpr char TEXT_CGBN[]   PROGMEM = "CGBN";
constexpr char TEXT_CMP_G[]  PROGMEM = "CMP:G";
constexpr char TEXT_CMP_Q[]  PROGMEM = "CMP:Q";
constexpr char TEXT_CMP_R[]  PROGMEM = "CMP:R";
constexpr char TEXT_CMP_RQ[] PROGMEM = "CMP:RQ";
constexpr char TEXT_CMPS[]   PROGMEM = "CMPS";
constexpr char TEXT_DADD[]   PROGMEM = "DADD";
constexpr char TEXT_DCBN[]   PROGMEM = "DCBN";
constexpr char TEXT_DIVXS[]  PROGMEM = "DIVXS";
constexpr char TEXT_DIVXU[]  PROGMEM = "DIVXU";
constexpr char TEXT_DNEG[]   PROGMEM = "DNEG";
constexpr char TEXT_DSUB[]   PROGMEM = "DSUB";
constexpr char TEXT_EXTU[]   PROGMEM = "EXTU";
constexpr char TEXT_ICBN[]   PROGMEM = "ICBN";
constexpr char TEXT_MOV_G[]  PROGMEM = "MOV:G";
constexpr char TEXT_MOV_Q[]  PROGMEM = "MOV:Q";
constexpr char TEXT_MOV_R[]  PROGMEM = "MOV:R";
constexpr char TEXT_MOV_RQ[] PROGMEM = "MOV:RQ";
constexpr char TEXT_MOVA[]   PROGMEM = "MOVA";
constexpr char TEXT_MOVF[]   PROGMEM = "MOVF";
constexpr char TEXT_MOVFP[]  PROGMEM = "MOVFP";
constexpr char TEXT_MOVFPE[] PROGMEM = "MOVFPE";
constexpr char TEXT_MOVS[]   PROGMEM = "MOVS";
constexpr char TEXT_MOVTP[]  PROGMEM = "MOVTP";
constexpr char TEXT_MOVTPE[] PROGMEM = "MOVTPE";
constexpr char TEXT_MULXS[]  PROGMEM = "MULXS";
constexpr char TEXT_MULXU[]  PROGMEM = "MULXU";
constexpr char TEXT_ORC[]    PROGMEM = "ORC";
constexpr char TEXT_PGBN[]   PROGMEM = "PGBN";
constexpr char TEXT_ROTL[]   PROGMEM = "ROTL";
constexpr char TEXT_ROTR[]   PROGMEM = "ROTR";
constexpr char TEXT_ROTXL[]  PROGMEM = "ROTXL";
constexpr char TEXT_ROTXR[]  PROGMEM = "ROTXR";
constexpr char TEXT_RTD[]    PROGMEM = "RTD";
constexpr char TEXT_RTR[]    PROGMEM = "RTR";
constexpr char TEXT_SCB[]    PROGMEM = "SCB";
constexpr char TEXT_SCMP[]   PROGMEM = "SCMP";
constexpr char TEXT_SET[]    PROGMEM = "SET";
constexpr char TEXT_SHAL[]   PROGMEM = "SHAL";
constexpr char TEXT_SHAR[]   PROGMEM = "SHAR";
constexpr char TEXT_SHLR[]   PROGMEM = "SHLR";
constexpr char TEXT_SLEEP[]  PROGMEM = "SLEEP";
constexpr char TEXT_SMOV[]   PROGMEM = "SMOV";
constexpr char TEXT_SSCH[]   PROGMEM = "SSCH";
constexpr char TEXT_SSTR[]   PROGMEM = "SSTR";
constexpr char TEXT_SUB_G[]  PROGMEM = "SUB:G";
constexpr char TEXT_SUB_R[]  PROGMEM = "SUB:R";
constexpr char TEXT_SUB_RQ[] PROGMEM = "SUB:RQ";
constexpr char TEXT_TRAP[]   PROGMEM = "TRAP";
constexpr char TEXT_TRAPA[]  PROGMEM = "TRAPA";
constexpr char TEXT_XORC[]   PROGMEM = "XORC";

// Condition code names not in common
constexpr char TEXT_CC_CC[]  PROGMEM = "CC";
constexpr char TEXT_CC_CS[]  PROGMEM = "CS";
constexpr char TEXT_CC_HI[]  PROGMEM = "HI";
constexpr char TEXT_CC_HS[]  PROGMEM = "HS";
constexpr char TEXT_CC_LO[]  PROGMEM = "LO";
constexpr char TEXT_CC_LS[]  PROGMEM = "LS";
constexpr char TEXT_CC_VC[]  PROGMEM = "VC";
constexpr char TEXT_CC_VS[]  PROGMEM = "VS";

// Control register names
constexpr char TEXT_REG_BMR[]  PROGMEM = "BMR";
constexpr char TEXT_REG_BSP[]  PROGMEM = "BSP";
constexpr char TEXT_REG_CBNR[] PROGMEM = "CBNR";
constexpr char TEXT_REG_EBR[]  PROGMEM = "EBR";
constexpr char TEXT_REG_GBNR[] PROGMEM = "GBNR";
constexpr char TEXT_REG_IBR[]  PROGMEM = "IBR";
constexpr char TEXT_REG_RBR[]  PROGMEM = "RBR";
constexpr char TEXT_REG_USP[]  PROGMEM = "USP";
constexpr char TEXT_REG_VBNR[] PROGMEM = "VBNR";

// PC register
constexpr char TEXT_REG_PC[]  PROGMEM = "PC";

// clang-format on
}  // namespace h16
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
