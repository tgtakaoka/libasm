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

constexpr char TEXT_TMS9900_LIST[] = "TMS9900, TMS9980, TMS9995, TMS99105, TMS99110";
constexpr char TEXT_CPU_9900[]  = "9900";
constexpr char TEXT_CPU_99105[] = "99105";
constexpr char TEXT_CPU_99110[] = "99110";
constexpr char TEXT_CPU_9980[]  = "9980";
constexpr char TEXT_CPU_9995[]  = "9995";

// TMS9900
// constexpr char TEXT_A[]    = "A";
constexpr char TEXT_AB[]   = "AB";
// constexpr char TEXT_ABS[]  = "ABS";
// constexpr char TEXT_AI[]   = "AI";
// constexpr char TEXT_ANDI[] = "ANDI";
// constexpr char TEXT_B[]    = "B";
// constexpr char TEXT_BL[]   = "BL";
constexpr char TEXT_BLWP[] = "BLWP";
// constexpr char TEXT_C[]    = "C";
// constexpr char TEXT_CB[]   = "CB";
// constexpr char TEXT_CI[]   = "CI";
constexpr char TEXT_CKOF[] = "CKOF";
constexpr char TEXT_CKON[] = "CKON";
// constexpr char TEXT_CLR[]  = "CLR";
constexpr char TEXT_COC[]  = "COC";
constexpr char TEXT_CZC[]  = "CZC";
// constexpr char TEXT_DEC[]  = "DEC";
constexpr char TEXT_DECT[] = "DECT";
// constexpr char TEXT_DIV[]  = "DIV";
// constexpr char TEXT_IDLE[] = "IDLE";
// constexpr char TEXT_INC[]  = "INC";
constexpr char TEXT_INCT[] = "INCT";
// constexpr char TEXT_INV[]  = "INV";
// constexpr char TEXT_JEQ[]  = "JEQ";
// constexpr char TEXT_JGT[]  = "JGT";
// constexpr char TEXT_JH[]   = "JH";
constexpr char TEXT_JHE[]  = "JHE";
// constexpr char TEXT_JL[]   = "JL";
// constexpr char TEXT_JLE[]  = "JLE";
// constexpr char TEXT_JLT[]  = "JLT";
// constexpr char TEXT_JMP[]  = "JMP";
// constexpr char TEXT_JNC[]  = "JNC";
// constexpr char TEXT_JNE[]  = "JNE";
// constexpr char TEXT_JNO[]  = "JNO";
constexpr char TEXT_JOC[]  = "JOC";
constexpr char TEXT_JOP[]  = "JOP";
constexpr char TEXT_LDCR[] = "LDCR";
// constexpr char TEXT_LI[]   = "LI";
constexpr char TEXT_LIMI[] = "LIMI";
constexpr char TEXT_LREX[] = "LREX";
constexpr char TEXT_LWPI[] = "LWPI";
// constexpr char TEXT_MOV[]  = "MOV";
// constexpr char TEXT_MOVB[] = "MOVB";
// constexpr char TEXT_MPY[]  = "MPY";
// constexpr char TEXT_NEG[]  = "NEG";
// constexpr char TEXT_ORI[]  = "ORI";
constexpr char TEXT_RSET[] = "RSET";
constexpr char TEXT_RT[]   = "RT";
constexpr char TEXT_RTWP[] = "RTWP";
// constexpr char TEXT_S[]    = "S";
constexpr char TEXT_SB[]   = "SB";
constexpr char TEXT_SBO[]  = "SBO";
constexpr char TEXT_SBZ[]  = "SBZ";
// constexpr char TEXT_SETO[] = "SETO";
// constexpr char TEXT_SLA[]  = "SLA";
constexpr char TEXT_SOC[]  = "SOC";
constexpr char TEXT_SOCB[] = "SOCB";
// constexpr char TEXT_SRA[]  = "SRA";
constexpr char TEXT_SRC[]  = "SRC";
// constexpr char TEXT_SRL[]  = "SRL";
constexpr char TEXT_STCR[] = "STCR";
// constexpr char TEXT_STST[] = "STST";
constexpr char TEXT_STWP[] = "STWP";
constexpr char TEXT_SWPB[] = "SWPB";
constexpr char TEXT_SZC[]  = "SZC";
constexpr char TEXT_SZCB[] = "SZCB";
constexpr char TEXT_TB[]   = "TB";
constexpr char TEXT_X[]    = "X";
constexpr char TEXT_XOP[]  = "XOP";
// constexpr char TEXT_XOR[]  = "XOR";

// TMS9995
// constexpr char TEXT_DIVS[] = "DIVS";
// constexpr char TEXT_LST[]  = "LST";
constexpr char TEXT_LWP[] = "LWP";
// constexpr char TEXT_MPYS[] = "MPYS";

// TMS999105
// constexpr char TEXT_AM[] = "AM";
constexpr char TEXT_BIND[] = "BIND";
constexpr char TEXT_BLSK[] = "BLSK";
constexpr char TEXT_EVAD[] = "EVAD";
constexpr char TEXT_SLAM[] = "SLAM";
// constexpr char TEXT_SM[] = "SM";
constexpr char TEXT_SRAM[] = "SRAM";
constexpr char TEXT_TCMB[] = "TCMB";
constexpr char TEXT_TMB[]  = "TMB";
constexpr char TEXT_TSMB[] = "TSMB";

// TMS99110
constexpr char TEXT_AR[]   = "AR";
constexpr char TEXT_CER[]  = "CER";
constexpr char TEXT_CIR[]  = "CIR";
constexpr char TEXT_CR[]   = "CR";
constexpr char TEXT_CRE[]  = "CRE";
constexpr char TEXT_CRI[]  = "CRI";
constexpr char TEXT_DR[]   = "DR";
// constexpr char TEXT_LDD[] = "LDD";
// constexpr char TEXT_LDS[] = "LDS";
// constexpr char TEXT_LR[]  = "LR";
constexpr char TEXT_MM[]   = "MM";
constexpr char TEXT_MR[]   = "MR";
// constexpr char TEXT_NEGR[] = "NEGR";
// constexpr char TEXT_SR[]  = "SR";
// constexpr char TEXT_STR[] = "STR";

constexpr char TEXT_SINGLE[] = "SINGLE";

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
