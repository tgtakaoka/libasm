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
constexpr char TEXT_H16_LIST[] PROGMEM = "H16";
constexpr char TEXT_CPU_H16[]  PROGMEM = "H16";

// H16-specific instruction mnemonics (sorted; commented-out entries have
// been promoted to text_common.{h,cpp}).  Class-qualified mnemonics are
// stored stem-only; the ":X" class is carried as an Entry attribute.
constexpr char TEXT_BFEXT[]  PROGMEM = "BFEXT";
constexpr char TEXT_BFINS[]  PROGMEM = "BFINS";
constexpr char TEXT_BFMOV[]  PROGMEM = "BFMOV";
constexpr char TEXT_BFSCH[]  PROGMEM = "BFSCH";
constexpr char TEXT_CGBN[]   PROGMEM = "CGBN";
constexpr char TEXT_DCBN[]   PROGMEM = "DCBN";
constexpr char TEXT_DIVXS[]  PROGMEM = "DIVXS";
constexpr char TEXT_DNEG[]   PROGMEM = "DNEG";
constexpr char TEXT_DSUB[]   PROGMEM = "DSUB";
constexpr char TEXT_ICBN[]   PROGMEM = "ICBN";
constexpr char TEXT_MOVA[]   PROGMEM = "MOVA";
constexpr char TEXT_MOVF[]   PROGMEM = "MOVF";
constexpr char TEXT_MOVFP[]  PROGMEM = "MOVFP";
// constexpr char TEXT_MOVFPE[] PROGMEM = "MOVFPE";
constexpr char TEXT_MOVS[]   PROGMEM = "MOVS";
constexpr char TEXT_MOVTP[]  PROGMEM = "MOVTP";
constexpr char TEXT_MULXS[]  PROGMEM = "MULXS";
constexpr char TEXT_PGBN[]   PROGMEM = "PGBN";
constexpr char TEXT_SCB[]    PROGMEM = "SCB";
constexpr char TEXT_SCMP[]   PROGMEM = "SCMP";
constexpr char TEXT_SMOV[]   PROGMEM = "SMOV";
constexpr char TEXT_SSCH[]   PROGMEM = "SSCH";
constexpr char TEXT_SSTR[]   PROGMEM = "SSTR";

// Condition code names not in common
constexpr char TEXT_CC_CC[]  PROGMEM = "CC";
constexpr char TEXT_CC_CS[]  PROGMEM = "CS";
constexpr char TEXT_CC_HI[]  PROGMEM = "HI";
constexpr char TEXT_CC_HS[]  PROGMEM = "HS";
constexpr char TEXT_CC_LO[]  PROGMEM = "LO";
constexpr char TEXT_CC_LS[]  PROGMEM = "LS";
constexpr char TEXT_CC_VC[]  PROGMEM = "VC";
constexpr char TEXT_CC_VS[]  PROGMEM = "VS";

// Control register names (TEXT_REG_PC and TEXT_REG_USP promoted to common).
constexpr char TEXT_REG_BMR[]  PROGMEM = "BMR";
constexpr char TEXT_REG_BSP[]  PROGMEM = "BSP";
constexpr char TEXT_REG_CBNR[] PROGMEM = "CBNR";
constexpr char TEXT_REG_EBR[]  PROGMEM = "EBR";
constexpr char TEXT_REG_GBNR[] PROGMEM = "GBNR";
constexpr char TEXT_REG_IBR[]  PROGMEM = "IBR";
constexpr char TEXT_REG_RBR[]  PROGMEM = "RBR";
constexpr char TEXT_REG_VBNR[] PROGMEM = "VBNR";
// constexpr char TEXT_REG_PC[]  PROGMEM = "PC";
// constexpr char TEXT_REG_USP[] PROGMEM = "USP";

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
