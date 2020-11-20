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

#ifndef __TEXT_TMS9900__
#define __TEXT_TMS9900__

namespace libasm {
namespace tms9900 {

// clang-format off
static const char TEXT_CPU_LIST[] PROGMEM = "TMS9900, TMS9995, TMS99105";
static const char TEXT_CPU_9900[] PROGMEM = "9900";
static const char TEXT_CPU_9995[] PROGMEM = "9995";
static constexpr const char *TEXT_CPU_TMS   = TEXT_CPU_LIST;
static constexpr const char *TEXT_CPU_99105 = (TEXT_CPU_LIST + 21);

// TMS9900
static const char TEXT_A[]    PROGMEM = "A";
static const char TEXT_AB[]   PROGMEM = "AB";
static const char TEXT_ABS[]  PROGMEM = "ABS";
static const char TEXT_AI[]   PROGMEM = "AI";
static const char TEXT_ANDI[] PROGMEM = "ANDI";
static const char TEXT_B[]    PROGMEM = "B";
static const char TEXT_BL[]   PROGMEM = "BL";
static const char TEXT_BLWP[] PROGMEM = "BLWP";
static const char TEXT_C[]    PROGMEM = "C";
static const char TEXT_CB[]   PROGMEM = "CB";
static const char TEXT_CI[]   PROGMEM = "CI";
static const char TEXT_CKOF[] PROGMEM = "CKOF";
static const char TEXT_CKON[] PROGMEM = "CKON";
static const char TEXT_CLR[]  PROGMEM = "CLR";
static const char TEXT_COC[]  PROGMEM = "COC";
static const char TEXT_CZC[]  PROGMEM = "CZC";
static const char TEXT_DEC[]  PROGMEM = "DEC";
static const char TEXT_DECT[] PROGMEM = "DECT";
static const char TEXT_DIV[]  PROGMEM = "DIV";
static const char TEXT_IDLE[] PROGMEM = "IDLE";
static const char TEXT_INC[]  PROGMEM = "INC";
static const char TEXT_INCT[] PROGMEM = "INCT";
static const char TEXT_INV[]  PROGMEM = "INV";
static const char TEXT_JEQ[]  PROGMEM = "JEQ";
static const char TEXT_JGT[]  PROGMEM = "JGT";
static const char TEXT_JH[]   PROGMEM = "JH";
static const char TEXT_JHE[]  PROGMEM = "JHE";
static const char TEXT_JL[]   PROGMEM = "JL";
static const char TEXT_JLE[]  PROGMEM = "JLE";
static const char TEXT_JLT[]  PROGMEM = "JLT";
static const char TEXT_JMP[]  PROGMEM = "JMP";
static const char TEXT_JNC[]  PROGMEM = "JNC";
static const char TEXT_JNE[]  PROGMEM = "JNE";
static const char TEXT_JNO[]  PROGMEM = "JNO";
static const char TEXT_JOC[]  PROGMEM = "JOC";
static const char TEXT_JOP[]  PROGMEM = "JOP";
static const char TEXT_LDCR[] PROGMEM = "LDCR";
static const char TEXT_LI[]   PROGMEM = "LI";
static const char TEXT_LIMI[] PROGMEM = "LIMI";
static const char TEXT_LREX[] PROGMEM = "LREX";
static const char TEXT_LWPI[] PROGMEM = "LWPI";
static const char TEXT_MOV[]  PROGMEM = "MOV";
static const char TEXT_MOVB[] PROGMEM = "MOVB";
static const char TEXT_MPY[]  PROGMEM = "MPY";
static const char TEXT_NEG[]  PROGMEM = "NEG";
static const char TEXT_ORI[]  PROGMEM = "ORI";
static const char TEXT_RSET[] PROGMEM = "RSET";
static const char TEXT_RTWP[] PROGMEM = "RTWP";
static const char TEXT_S[]    PROGMEM = "S";
static const char TEXT_SB[]   PROGMEM = "SB";
static const char TEXT_SBO[]  PROGMEM = "SBO";
static const char TEXT_SBZ[]  PROGMEM = "SBZ";
static const char TEXT_SETO[] PROGMEM = "SETO";
static const char TEXT_SLA[]  PROGMEM = "SLA";
static const char TEXT_SRA[]  PROGMEM = "SRA";
static const char TEXT_SRC[]  PROGMEM = "SRC";
static const char TEXT_SRL[]  PROGMEM = "SRL";
static const char TEXT_SOC[]  PROGMEM = "SOC";
static const char TEXT_SOCB[] PROGMEM = "SOCB";
static const char TEXT_STCR[] PROGMEM = "STCR";
static const char TEXT_STST[] PROGMEM = "STST";
static const char TEXT_STWP[] PROGMEM = "STWP";
static const char TEXT_SWPB[] PROGMEM = "SWPB";
static const char TEXT_SZC[]  PROGMEM = "SZC";
static const char TEXT_SZCB[] PROGMEM = "SZCB";
static const char TEXT_TB[]   PROGMEM = "TB";
static const char TEXT_X[]    PROGMEM = "X";
static const char TEXT_XOP[]  PROGMEM = "XOP";
static const char TEXT_XOR[]  PROGMEM = "XOR";

// TMS9995
static const char TEXT_MPYS[] PROGMEM = "MPYS";
static const char TEXT_DIVS[] PROGMEM = "DIVS";
static const char TEXT_LST[]  PROGMEM = "LST";
static const char TEXT_LWP[]  PROGMEM = "LWP";

// TMS999105
static const char TEXT_AM[]   PROGMEM = "AM";
static const char TEXT_BIND[] PROGMEM = "BIND";
static const char TEXT_BLSK[] PROGMEM = "BLSK";
static const char TEXT_EVAD[] PROGMEM = "EVAD";
static const char TEXT_SLAM[] PROGMEM = "SLAM";
static const char TEXT_SM[]   PROGMEM = "SM";
static const char TEXT_SRAM[] PROGMEM = "SRAM";
static const char TEXT_TCMB[] PROGMEM = "TCMB";
static const char TEXT_TMB[]  PROGMEM = "TMB";
static const char TEXT_TSMB[] PROGMEM = "TSMB";
// clang-format on

}  // namespace tms9900
}  // namespace libasm

#endif  // __TEXT_TMS9900__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
