/*
 * Copyright 2020 Tadashi G. Takaoka
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

#include "text_tms9900.h"

namespace libasm {
namespace text {
namespace tms9900 {

// clang-format off
constexpr char TEXT_TMS9900_LIST[] PROGMEM = "TMS9900, TMS9980, TMS9995, TMS99105, TMS99110";
constexpr char TEXT_CPU_9900[]  PROGMEM = "9900";
constexpr char TEXT_CPU_9980[]  PROGMEM = "9980";
constexpr char TEXT_CPU_9995[]  PROGMEM = "9995";
constexpr char TEXT_CPU_99105[] PROGMEM = "99105";
constexpr char TEXT_CPU_99110[] PROGMEM = "99110";

// TMS9900
// constexpr char TEXT_A[]    PROGMEM = "A";
constexpr char TEXT_AB[]   PROGMEM = "AB";
// constexpr char TEXT_ABS[]  PROGMEM = "ABS";
// constexpr char TEXT_AI[]   PROGMEM = "AI";
// constexpr char TEXT_ANDI[] PROGMEM = "ANDI";
// constexpr char TEXT_B[]    PROGMEM = "B";
// constexpr char TEXT_BL[]   PROGMEM = "BL";
constexpr char TEXT_BLWP[] PROGMEM = "BLWP";
// constexpr char TEXT_C[]    PROGMEM = "C";
// constexpr char TEXT_CB[]   PROGMEM = "CB";
// constexpr char TEXT_CI[]   PROGMEM = "CI";
constexpr char TEXT_CKOF[] PROGMEM = "CKOF";
constexpr char TEXT_CKON[] PROGMEM = "CKON";
// constexpr char TEXT_CLR[]  PROGMEM = "CLR";
constexpr char TEXT_COC[]  PROGMEM = "COC";
constexpr char TEXT_CZC[]  PROGMEM = "CZC";
// constexpr char TEXT_DEC[]  PROGMEM = "DEC";
constexpr char TEXT_DECT[] PROGMEM = "DECT";
// constexpr char TEXT_DIV[]  PROGMEM = "DIV";
// constexpr char TEXT_IDLE[] PROGMEM = "IDLE";
// constexpr char TEXT_INC[]  PROGMEM = "INC";
constexpr char TEXT_INCT[] PROGMEM = "INCT";
// constexpr char TEXT_INV[]  PROGMEM = "INV";
// constexpr char TEXT_JEQ[]  PROGMEM = "JEQ";
// constexpr char TEXT_JGT[]  PROGMEM = "JGT";
// constexpr char TEXT_JH[]   PROGMEM = "JH";
constexpr char TEXT_JHE[]  PROGMEM = "JHE";
// constexpr char TEXT_JL[]   PROGMEM = "JL";
// constexpr char TEXT_JLE[]  PROGMEM = "JLE";
// constexpr char TEXT_JLT[]  PROGMEM = "JLT";
// constexpr char TEXT_JMP[]  PROGMEM = "JMP";
// constexpr char TEXT_JNC[]  PROGMEM = "JNC";
// constexpr char TEXT_JNE[]  PROGMEM = "JNE";
// constexpr char TEXT_JNO[]  PROGMEM = "JNO";
constexpr char TEXT_JOC[]  PROGMEM = "JOC";
constexpr char TEXT_JOP[]  PROGMEM = "JOP";
constexpr char TEXT_LDCR[] PROGMEM = "LDCR";
// constexpr char TEXT_LI[]   PROGMEM = "LI";
constexpr char TEXT_LIMI[] PROGMEM = "LIMI";
constexpr char TEXT_LREX[] PROGMEM = "LREX";
constexpr char TEXT_LWPI[] PROGMEM = "LWPI";
// constexpr char TEXT_MOV[]  PROGMEM = "MOV";
// constexpr char TEXT_MOVB[] PROGMEM = "MOVB";
// constexpr char TEXT_MPY[]  PROGMEM = "MPY";
// constexpr char TEXT_NEG[]  PROGMEM = "NEG";
// constexpr char TEXT_ORI[]  PROGMEM = "ORI";
constexpr char TEXT_RSET[] PROGMEM = "RSET";
constexpr char TEXT_RTWP[] PROGMEM = "RTWP";
// constexpr char TEXT_S[]    PROGMEM = "S";
constexpr char TEXT_SB[]   PROGMEM = "SB";
constexpr char TEXT_SBO[]  PROGMEM = "SBO";
constexpr char TEXT_SBZ[]  PROGMEM = "SBZ";
constexpr char TEXT_SETO[] PROGMEM = "SETO";
// constexpr char TEXT_SLA[]  PROGMEM = "SLA";
// constexpr char TEXT_SRA[]  PROGMEM = "SRA";
constexpr char TEXT_SRC[]  PROGMEM = "SRC";
// constexpr char TEXT_SRL[]  PROGMEM = "SRL";
constexpr char TEXT_SOC[]  PROGMEM = "SOC";
constexpr char TEXT_SOCB[] PROGMEM = "SOCB";
constexpr char TEXT_STCR[] PROGMEM = "STCR";
// constexpr char TEXT_STST[] PROGMEM = "STST";
constexpr char TEXT_STWP[] PROGMEM = "STWP";
constexpr char TEXT_SWPB[] PROGMEM = "SWPB";
constexpr char TEXT_SZC[]  PROGMEM = "SZC";
constexpr char TEXT_SZCB[] PROGMEM = "SZCB";
constexpr char TEXT_TB[]   PROGMEM = "TB";
constexpr char TEXT_X[]    PROGMEM = "X";
constexpr char TEXT_XOP[]  PROGMEM = "XOP";
// constexpr char TEXT_XOR[]  PROGMEM = "XOR";

// TMS9995
constexpr char TEXT_MPYS[] PROGMEM = "MPYS";
// constexpr char TEXT_DIVS[] PROGMEM = "DIVS";
// constexpr char TEXT_LST[]  PROGMEM = "LST";
constexpr char TEXT_LWP[]  PROGMEM = "LWP";

// TMS999105
// constexpr char TEXT_AM[]   PROGMEM = "AM";
constexpr char TEXT_BIND[] PROGMEM = "BIND";
constexpr char TEXT_BLSK[] PROGMEM = "BLSK";
constexpr char TEXT_EVAD[] PROGMEM = "EVAD";
constexpr char TEXT_SLAM[] PROGMEM = "SLAM";
// constexpr char TEXT_SM[]   PROGMEM = "SM";
constexpr char TEXT_SRAM[] PROGMEM = "SRAM";
constexpr char TEXT_TCMB[] PROGMEM = "TCMB";
constexpr char TEXT_TMB[]  PROGMEM = "TMB";
constexpr char TEXT_TSMB[] PROGMEM = "TSMB";

// TMS99110
constexpr char TEXT_AR[]   PROGMEM = "AR";
constexpr char TEXT_CER[]  PROGMEM = "CER";
constexpr char TEXT_CIR[]  PROGMEM = "CIR";
constexpr char TEXT_CR[]   PROGMEM = "CR";
constexpr char TEXT_CRE[]  PROGMEM = "CRE";
constexpr char TEXT_CRI[]  PROGMEM = "CRI";
constexpr char TEXT_DR[]   PROGMEM = "DR";
// constexpr char TEXT_LDD[]  PROGMEM = "LDD";
// constexpr char TEXT_LDS[]  PROGMEM = "LDS";
// constexpr char TEXT_LR[]   PROGMEM = "LR";
constexpr char TEXT_MM[]   PROGMEM = "MM";
constexpr char TEXT_MR[]   PROGMEM = "MR";
constexpr char TEXT_NEGR[] PROGMEM = "NEGR";
// constexpr char TEXT_SR[]   PROGMEM = "SR";
// constexpr char TEXT_STR[]  PROGMEM = "STR";

constexpr char TEXT_SINGLE[] PROGMEM = "SINGLE";

// clang-format on
}  // namespace tms9900
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
