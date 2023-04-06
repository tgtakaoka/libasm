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
const char TEXT_CPU_LIST[]  PROGMEM = "TMS9900, TMS9995, TMS99105";
const char TEXT_CPU_9900[]  PROGMEM = "9900";
const char TEXT_CPU_9995[]  PROGMEM = "9995";
const char TEXT_CPU_99105[] PROGMEM = "99105";

// TMS9900
// const char TEXT_A[]    PROGMEM = "A";
const char TEXT_AB[]   PROGMEM = "AB";
// const char TEXT_ABS[]  PROGMEM = "ABS";
// const char TEXT_AI[]   PROGMEM = "AI";
// const char TEXT_ANDI[] PROGMEM = "ANDI";
// const char TEXT_B[]    PROGMEM = "B";
// const char TEXT_BL[]   PROGMEM = "BL";
const char TEXT_BLWP[] PROGMEM = "BLWP";
// const char TEXT_C[]    PROGMEM = "C";
// const char TEXT_CB[]   PROGMEM = "CB";
// const char TEXT_CI[]   PROGMEM = "CI";
const char TEXT_CKOF[] PROGMEM = "CKOF";
const char TEXT_CKON[] PROGMEM = "CKON";
// const char TEXT_CLR[]  PROGMEM = "CLR";
const char TEXT_COC[]  PROGMEM = "COC";
const char TEXT_CZC[]  PROGMEM = "CZC";
// const char TEXT_DEC[]  PROGMEM = "DEC";
const char TEXT_DECT[] PROGMEM = "DECT";
// const char TEXT_DIV[]  PROGMEM = "DIV";
const char TEXT_IDLE[] PROGMEM = "IDLE";
// const char TEXT_INC[]  PROGMEM = "INC";
const char TEXT_INCT[] PROGMEM = "INCT";
const char TEXT_INV[]  PROGMEM = "INV";
const char TEXT_JEQ[]  PROGMEM = "JEQ";
// const char TEXT_JGT[]  PROGMEM = "JGT";
// const char TEXT_JH[]   PROGMEM = "JH";
const char TEXT_JHE[]  PROGMEM = "JHE";
// const char TEXT_JL[]   PROGMEM = "JL";
// const char TEXT_JLE[]  PROGMEM = "JLE";
// const char TEXT_JLT[]  PROGMEM = "JLT";
// const char TEXT_JMP[]  PROGMEM = "JMP";
// const char TEXT_JNC[]  PROGMEM = "JNC";
// const char TEXT_JNE[]  PROGMEM = "JNE";
// const char TEXT_JNO[]  PROGMEM = "JNO";
const char TEXT_JOC[]  PROGMEM = "JOC";
const char TEXT_JOP[]  PROGMEM = "JOP";
const char TEXT_LDCR[] PROGMEM = "LDCR";
// const char TEXT_LI[]   PROGMEM = "LI";
const char TEXT_LIMI[] PROGMEM = "LIMI";
const char TEXT_LREX[] PROGMEM = "LREX";
const char TEXT_LWPI[] PROGMEM = "LWPI";
// const char TEXT_MOV[]  PROGMEM = "MOV";
// const char TEXT_MOVB[] PROGMEM = "MOVB";
// const char TEXT_MPY[]  PROGMEM = "MPY";
const char TEXT_MID[]  PROGMEM = "MID";
// const char TEXT_NEG[]  PROGMEM = "NEG";
// const char TEXT_ORI[]  PROGMEM = "ORI";
const char TEXT_RSET[] PROGMEM = "RSET";
const char TEXT_RTWP[] PROGMEM = "RTWP";
// const char TEXT_S[]    PROGMEM = "S";
const char TEXT_SB[]   PROGMEM = "SB";
const char TEXT_SBO[]  PROGMEM = "SBO";
const char TEXT_SBZ[]  PROGMEM = "SBZ";
const char TEXT_SETO[] PROGMEM = "SETO";
// const char TEXT_SLA[]  PROGMEM = "SLA";
// const char TEXT_SRA[]  PROGMEM = "SRA";
const char TEXT_SRC[]  PROGMEM = "SRC";
// const char TEXT_SRL[]  PROGMEM = "SRL";
const char TEXT_SOC[]  PROGMEM = "SOC";
const char TEXT_SOCB[] PROGMEM = "SOCB";
const char TEXT_STCR[] PROGMEM = "STCR";
const char TEXT_STST[] PROGMEM = "STST";
const char TEXT_STWP[] PROGMEM = "STWP";
const char TEXT_SWPB[] PROGMEM = "SWPB";
const char TEXT_SZC[]  PROGMEM = "SZC";
const char TEXT_SZCB[] PROGMEM = "SZCB";
const char TEXT_TB[]   PROGMEM = "TB";
const char TEXT_X[]    PROGMEM = "X";
const char TEXT_XOP[]  PROGMEM = "XOP";
// const char TEXT_XOR[]  PROGMEM = "XOR";

// TMS9995
const char TEXT_MPYS[] PROGMEM = "MPYS";
// const char TEXT_DIVS[] PROGMEM = "DIVS";
// const char TEXT_LST[]  PROGMEM = "LST";
const char TEXT_LWP[]  PROGMEM = "LWP";

// TMS999105
// const char TEXT_AM[]   PROGMEM = "AM";
const char TEXT_BIND[] PROGMEM = "BIND";
const char TEXT_BLSK[] PROGMEM = "BLSK";
const char TEXT_EVAD[] PROGMEM = "EVAD";
const char TEXT_SLAM[] PROGMEM = "SLAM";
// const char TEXT_SM[]   PROGMEM = "SM";
const char TEXT_SRAM[] PROGMEM = "SRAM";
const char TEXT_TCMB[] PROGMEM = "TCMB";
const char TEXT_TMB[]  PROGMEM = "TMB";
const char TEXT_TSMB[] PROGMEM = "TSMB";
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
