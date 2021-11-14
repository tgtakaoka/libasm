/*
 * Copyright 2021 Tadashi G. Takaoka
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

#ifndef __TEXT_MN1610__
#define __TEXT_MN1610__

#include "config_host.h"

namespace libasm {
namespace mn1610 {

// clang-format off
static const char TEXT_CPU_LIST[]   PROGMEM = "MN1610, MN1613, MN1613A";
static const char TEXT_CPU_MN1610[] PROGMEM = "MN1610";
static const char TEXT_CPU_MN1613[] PROGMEM = "MN1613";
static constexpr const char *TEXT_CPU_MN1613A = (TEXT_CPU_LIST + 16);
static constexpr const char *TEXT_CPU_1610    = (TEXT_CPU_MN1610 + 2);
static constexpr const char *TEXT_CPU_1613    = (TEXT_CPU_MN1613 + 2);
static constexpr const char *TEXT_CPU_1613A   = (TEXT_CPU_MN1613A + 2);

// MN1610
static const char TEXT_AI[]   PROGMEM = "AI";
static const char TEXT_AND[]  PROGMEM = "AND";
static const char TEXT_A[]    PROGMEM = "A";
static const char TEXT_BAL[]  PROGMEM = "BAL";
static const char TEXT_B[]    PROGMEM = "B";
static const char TEXT_BSWP[] PROGMEM = "BSWP";
static const char TEXT_CB[]   PROGMEM = "CB";
static const char TEXT_C[]    PROGMEM = "C";
static const char TEXT_DMS[]  PROGMEM = "DMS";
static const char TEXT_DSWP[] PROGMEM = "DSWP";
static const char TEXT_EOR[]  PROGMEM = "EOR";
static const char TEXT_H[]    PROGMEM = "H";
static const char TEXT_IMS[]  PROGMEM = "IMS";
static const char TEXT_LAD[]  PROGMEM = "LAD";
static const char TEXT_L[]    PROGMEM = "L";
static const char TEXT_LPSW[] PROGMEM = "LPSW";
static const char TEXT_MVB[]  PROGMEM = "MVB";
static const char TEXT_MVI[]  PROGMEM = "MVI";
static const char TEXT_MV[]   PROGMEM = "MV";
static const char TEXT_OR[]   PROGMEM = "OR";
static const char TEXT_POP[]  PROGMEM = "POP";
static const char TEXT_PUSH[] PROGMEM = "PUSH";
static const char TEXT_RBIT[] PROGMEM = "RBIT";
static const char TEXT_RD[]   PROGMEM = "RD";
static const char TEXT_RET[]  PROGMEM = "RET";
static const char TEXT_SBIT[] PROGMEM = "SBIT";
static const char TEXT_SI[]   PROGMEM = "SI";
static const char TEXT_SL[]   PROGMEM = "SL";
static const char TEXT_S[]    PROGMEM = "S";
static const char TEXT_SR[]   PROGMEM = "SR";
static const char TEXT_ST[]   PROGMEM = "ST";
static const char TEXT_TBIT[] PROGMEM = "TBIT";
static const char TEXT_WT[]   PROGMEM = "WT";

// MN1613
static const char TEXT_LB[]   PROGMEM = "LB";
static const char TEXT_SETB[] PROGMEM = "SETB";
static const char TEXT_LS[]   PROGMEM = "LS";
static const char TEXT_SETS[] PROGMEM = "SETS";
static const char TEXT_STB[]  PROGMEM = "STB";
static const char TEXT_CPYB[] PROGMEM = "CPYB";
static const char TEXT_STS[]  PROGMEM = "STS";
static const char TEXT_CPYS[] PROGMEM = "CPYS";
static const char TEXT_POPM[] PROGMEM = "POPM";
static const char TEXT_PSHM[] PROGMEM = "PSHM";
static const char TEXT_TRST[] PROGMEM = "TRST";
static const char TEXT_TSET[] PROGMEM = "TSET";
static const char TEXT_FLT[]  PROGMEM = "FLT";
static const char TEXT_FIX[]  PROGMEM = "FIX";
static const char TEXT_NEG[]  PROGMEM = "NEG";
static const char TEXT_BD[]   PROGMEM = "BD";
static const char TEXT_BALD[] PROGMEM = "BALD";
static const char TEXT_BR[]   PROGMEM = "BR";
static const char TEXT_BL[]   PROGMEM = "BL";
static const char TEXT_BALR[] PROGMEM = "BALR";
static const char TEXT_BALL[] PROGMEM = "BALL";
static const char TEXT_LD[]   PROGMEM = "LD";
static const char TEXT_STD[]  PROGMEM = "STD";
static const char TEXT_RDR[]  PROGMEM = "RDR";
static const char TEXT_LR[]   PROGMEM = "LR";
static const char TEXT_WTR[]  PROGMEM = "WTR";
static const char TEXT_STR[]  PROGMEM = "STR";
static const char TEXT_RETL[] PROGMEM = "RETL";
static const char TEXT_BLK[]  PROGMEM = "BLK";
static const char TEXT_SRBT[] PROGMEM = "SRBT";
static const char TEXT_SETH[] PROGMEM = "SETH";
static const char TEXT_DEBP[] PROGMEM = "DEBP";
static const char TEXT_CPYH[] PROGMEM = "CPYH";
static const char TEXT_SD[]   PROGMEM = "SD";
static const char TEXT_AD[]   PROGMEM = "AD";
static const char TEXT_CBR[]  PROGMEM = "CBR";
static const char TEXT_CWR[]  PROGMEM = "CWR";
static const char TEXT_DAS[]  PROGMEM = "DAS";
static const char TEXT_CBI[]  PROGMEM = "CBI";
static const char TEXT_CWI[]  PROGMEM = "CWI";
static const char TEXT_SWR[]  PROGMEM = "SWR";
static const char TEXT_AWR[]  PROGMEM = "AWR";
static const char TEXT_DAA[]  PROGMEM = "DAA";
static const char TEXT_SWI[]  PROGMEM = "SWI";
static const char TEXT_AWI[]  PROGMEM = "AWI";
static const char TEXT_FD[]   PROGMEM = "FD";
static const char TEXT_FM[]   PROGMEM = "FM";
static const char TEXT_EORR[] PROGMEM = "EORR";
static const char TEXT_EORI[] PROGMEM = "EORI";
static const char TEXT_ORR[]  PROGMEM = "ORR";
static const char TEXT_ORI[]  PROGMEM = "ORI";
static const char TEXT_LADR[] PROGMEM = "LADR";
static const char TEXT_FS[]   PROGMEM = "FS";
static const char TEXT_FA[]   PROGMEM = "FA";
static const char TEXT_LADI[] PROGMEM = "LADI";
static const char TEXT_ANDR[] PROGMEM = "ANDR";
static const char TEXT_ANDI[] PROGMEM = "ANDI";
static const char TEXT_DSWR[] PROGMEM = "DSWR";
static const char TEXT_BSWR[] PROGMEM = "BSWR";
static const char TEXT_D[]    PROGMEM = "D";
static const char TEXT_MVBR[] PROGMEM = "MVBR";
static const char TEXT_MVWR[] PROGMEM = "MVWR";
static const char TEXT_M[]    PROGMEM = "M";
static const char TEXT_MVWI[] PROGMEM = "MVWI";
// clang-format on

}  // namespace mn1610
}  // namespace libasm

#endif  // __TEXT_MN1610__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
