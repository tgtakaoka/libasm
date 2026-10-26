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
constexpr char TEXT_H16_LIST[] = "H16";
constexpr char TEXT_CPU_H16[]  = "H16";

// H16-specific instruction mnemonics (sorted; commented-out entries have
// been promoted to text_common.{h,cpp}).  Class-qualified mnemonics are
// stored stem-only; the ":X" class is carried as an Entry attribute.
constexpr char TEXT_BFEXT[]  = "BFEXT";
constexpr char TEXT_BFINS[]  = "BFINS";
constexpr char TEXT_BFMOV[]  = "BFMOV";
constexpr char TEXT_BFSCH[]  = "BFSCH";
constexpr char TEXT_CGBN[]   = "CGBN";
constexpr char TEXT_DCBN[]   = "DCBN";
constexpr char TEXT_DIVXS[]  = "DIVXS";
constexpr char TEXT_DNEG[]   = "DNEG";
constexpr char TEXT_DSUB[]   = "DSUB";
constexpr char TEXT_ICBN[]   = "ICBN";
constexpr char TEXT_MOVA[]   = "MOVA";
constexpr char TEXT_MOVF[]   = "MOVF";
constexpr char TEXT_MOVFP[]  = "MOVFP";
// constexpr char TEXT_MOVFPE[] = "MOVFPE";
constexpr char TEXT_MOVS[]   = "MOVS";
constexpr char TEXT_MOVTP[]  = "MOVTP";
constexpr char TEXT_MULXS[]  = "MULXS";
constexpr char TEXT_PGBN[]   = "PGBN";
constexpr char TEXT_SCB[]    = "SCB";
constexpr char TEXT_SCMP[]   = "SCMP";
constexpr char TEXT_SMOV[]   = "SMOV";
constexpr char TEXT_SSCH[]   = "SSCH";
constexpr char TEXT_SSTR[]   = "SSTR";

// Condition code names not in common
constexpr char TEXT_CC_CC[]  = "CC";
constexpr char TEXT_CC_CS[]  = "CS";
constexpr char TEXT_CC_HI[]  = "HI";
constexpr char TEXT_CC_HS[]  = "HS";
constexpr char TEXT_CC_LO[]  = "LO";
constexpr char TEXT_CC_LS[]  = "LS";
constexpr char TEXT_CC_VC[]  = "VC";
constexpr char TEXT_CC_VS[]  = "VS";

// Control register names (TEXT_REG_PC and TEXT_REG_USP promoted to common).
constexpr char TEXT_REG_BMR[]  = "BMR";
constexpr char TEXT_REG_BSP[]  = "BSP";
constexpr char TEXT_REG_CBNR[] = "CBNR";
constexpr char TEXT_REG_EBR[]  = "EBR";
constexpr char TEXT_REG_GBNR[] = "GBNR";
constexpr char TEXT_REG_IBR[]  = "IBR";
constexpr char TEXT_REG_RBR[]  = "RBR";
constexpr char TEXT_REG_VBNR[] = "VBNR";
// constexpr char TEXT_REG_PC[]  = "PC";
// constexpr char TEXT_REG_USP[] = "USP";

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
