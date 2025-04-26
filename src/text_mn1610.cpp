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

#include "text_mn1610.h"

namespace libasm {
namespace text {
namespace mn1610 {

// clang-format off
constexpr char TEXT_MN1610_LIST[] PROGMEM = "MN1610, MN1613, MN1613A";
constexpr char TEXT_CPU_MN1610[]  PROGMEM = "MN1610";
constexpr char TEXT_CPU_MN1613[]  PROGMEM = "MN1613";
constexpr char TEXT_CPU_MN1613A[] PROGMEM = "MN1613A";

// MN1610
// constexpr char TEXT_AI[]   PROGMEM = "AI";
// constexpr char TEXT_AND[]  PROGMEM = "AND";
// constexpr char TEXT_A[]    PROGMEM = "A";
constexpr char TEXT_BAL[]  PROGMEM = "BAL";
// constexpr char TEXT_B[]    PROGMEM = "B";
constexpr char TEXT_BSWP[] PROGMEM = "BSWP";
// constexpr char TEXT_CB[]   PROGMEM = "CB";
// constexpr char TEXT_C[]    PROGMEM = "C";
// constexpr char TEXT_CLR[]  PROGMEM = "CLR";
constexpr char TEXT_DMS[]  PROGMEM = "DMS";
constexpr char TEXT_DSWP[] PROGMEM = "DSWP";
// constexpr char TEXT_EOR[]  PROGMEM = "EOR";
constexpr char TEXT_H[]    PROGMEM = "H";
constexpr char TEXT_IMS[]  PROGMEM = "IMS";
constexpr char TEXT_LAD[]  PROGMEM = "LAD";
constexpr char TEXT_L[]    PROGMEM = "L";
constexpr char TEXT_LPSW[] PROGMEM = "LPSW";
constexpr char TEXT_MVB[]  PROGMEM = "MVB";
// constexpr char TEXT_MVI[]  PROGMEM = "MVI";
constexpr char TEXT_MV[]   PROGMEM = "MV";
// constexpr char TEXT_OR[]   PROGMEM = "OR";
// constexpr char TEXT_POP[]  PROGMEM = "POP";
// constexpr char TEXT_PUSH[] PROGMEM = "PUSH";
constexpr char TEXT_RBIT[] PROGMEM = "RBIT";
constexpr char TEXT_RD[]   PROGMEM = "RD";
// constexpr char TEXT_RET[]  PROGMEM = "RET";
constexpr char TEXT_SBIT[] PROGMEM = "SBIT";
constexpr char TEXT_SI[]   PROGMEM = "SI";
// constexpr char TEXT_SL[]   PROGMEM = "SL";
// constexpr char TEXT_S[]    PROGMEM = "S";
// constexpr char TEXT_SR[]   PROGMEM = "SR";
// constexpr char TEXT_ST[]   PROGMEM = "ST";
constexpr char TEXT_TBIT[] PROGMEM = "TBIT";
// constexpr char TEXT_TST[]  PROGMEM = "TST";
constexpr char TEXT_WT[]   PROGMEM = "WT";

// MN1613
constexpr char TEXT_LB[]   PROGMEM = "LB";
// constexpr char TEXT_SETB[] PROGMEM = "SETB";
constexpr char TEXT_LS[]   PROGMEM = "LS";
constexpr char TEXT_SETS[] PROGMEM = "SETS";
// constexpr char TEXT_STB[]  PROGMEM = "STB";
constexpr char TEXT_CPYB[] PROGMEM = "CPYB";
// constexpr char TEXT_STS[]  PROGMEM = "STS";
constexpr char TEXT_CPYS[] PROGMEM = "CPYS";
constexpr char TEXT_POPM[] PROGMEM = "POPM";
// constexpr char TEXT_PSHM[] PROGMEM = "PSHM";
constexpr char TEXT_TRST[] PROGMEM = "TRST";
// constexpr char TEXT_TSET[] PROGMEM = "TSET";
// constexpr char TEXT_FLT[]  PROGMEM = "FLT";
constexpr char TEXT_FIX[]  PROGMEM = "FIX";
// constexpr char TEXT_NEG[]  PROGMEM = "NEG";
constexpr char TEXT_BD[]   PROGMEM = "BD";
constexpr char TEXT_BALD[] PROGMEM = "BALD";
// constexpr char TEXT_BR[]   PROGMEM = "BR";
// constexpr char TEXT_BL[]   PROGMEM = "BL";
constexpr char TEXT_BALR[] PROGMEM = "BALR";
constexpr char TEXT_BALL[] PROGMEM = "BALL";
// constexpr char TEXT_LD[]   PROGMEM = "LD";
// constexpr char TEXT_STD[]  PROGMEM = "STD";
constexpr char TEXT_RDR[]  PROGMEM = "RDR";
// constexpr char TEXT_LR[]   PROGMEM = "LR";
constexpr char TEXT_WTR[]  PROGMEM = "WTR";
// constexpr char TEXT_STR[]  PROGMEM = "STR";
constexpr char TEXT_RETL[] PROGMEM = "RETL";
constexpr char TEXT_BLK[]  PROGMEM = "BLK";
constexpr char TEXT_SRBT[] PROGMEM = "SRBT";
constexpr char TEXT_SETH[] PROGMEM = "SETH";
constexpr char TEXT_DEBP[] PROGMEM = "DEBP";
constexpr char TEXT_CPYH[] PROGMEM = "CPYH";
// constexpr char TEXT_SD[]   PROGMEM = "SD";
constexpr char TEXT_AD[]   PROGMEM = "AD";
constexpr char TEXT_CBR[]  PROGMEM = "CBR";
constexpr char TEXT_CWR[]  PROGMEM = "CWR";
// constexpr char TEXT_DAS[]  PROGMEM = "DAS";
constexpr char TEXT_CBI[]  PROGMEM = "CBI";
constexpr char TEXT_CWI[]  PROGMEM = "CWI";
constexpr char TEXT_SWR[]  PROGMEM = "SWR";
constexpr char TEXT_AWR[]  PROGMEM = "AWR";
// constexpr char TEXT_DAA[]  PROGMEM = "DAA";
// constexpr char TEXT_SWI[]  PROGMEM = "SWI";
constexpr char TEXT_AWI[]  PROGMEM = "AWI";
constexpr char TEXT_FD[]   PROGMEM = "FD";
constexpr char TEXT_FM[]   PROGMEM = "FM";
// constexpr char TEXT_EORR[] PROGMEM = "EORR";
// constexpr char TEXT_EORI[] PROGMEM = "EORI";
// constexpr char TEXT_ORR[]  PROGMEM = "ORR";
// constexpr char TEXT_ORI[]  PROGMEM = "ORI";
constexpr char TEXT_LADR[] PROGMEM = "LADR";
constexpr char TEXT_FS[]   PROGMEM = "FS";
constexpr char TEXT_FA[]   PROGMEM = "FA";
constexpr char TEXT_LADI[] PROGMEM = "LADI";
// constexpr char TEXT_ANDR[] PROGMEM = "ANDR";
// constexpr char TEXT_ANDI[] PROGMEM = "ANDI";
constexpr char TEXT_DSWR[] PROGMEM = "DSWR";
constexpr char TEXT_BSWR[] PROGMEM = "BSWR";
constexpr char TEXT_D[]    PROGMEM = "D";
constexpr char TEXT_MVBR[] PROGMEM = "MVBR";
constexpr char TEXT_MVWR[] PROGMEM = "MVWR";
constexpr char TEXT_M[]    PROGMEM = "M";
constexpr char TEXT_MVWI[] PROGMEM = "MVWI";

// constexpr char TEXT_REG_R0[]   PROGMEM = "R0";
// constexpr char TEXT_REG_R1[]   PROGMEM = "R1";
// constexpr char TEXT_REG_R2[]   PROGMEM = "R2";
// constexpr char TEXT_REG_R3[]   PROGMEM = "R3";
// constexpr char TEXT_REG_R4[]   PROGMEM = "R4";
// constexpr char TEXT_REG_SP[]   PROGMEM = "SP";
constexpr char TEXT_REG_STR[]  PROGMEM = "STR";
constexpr char TEXT_REG_IC[]   PROGMEM = "IC";
constexpr char TEXT_REG_DR0[]  PROGMEM = "DR0";
constexpr char TEXT_REG_X0[]   PROGMEM = "X0";
constexpr char TEXT_REG_X1[]   PROGMEM = "X1";
constexpr char TEXT_REG_CSBR[] PROGMEM = "CSBR";
constexpr char TEXT_REG_SSBR[] PROGMEM = "SSBR";
constexpr char TEXT_REG_TSR0[] PROGMEM = "TSR0";
constexpr char TEXT_REG_TSR1[] PROGMEM = "TSR1";
constexpr char TEXT_REG_OSR0[] PROGMEM = "OSR0";
constexpr char TEXT_REG_OSR1[] PROGMEM = "OSR1";
constexpr char TEXT_REG_OSR2[] PROGMEM = "OSR2";
constexpr char TEXT_REG_OSR3[] PROGMEM = "OSR3";
constexpr char TEXT_REG_TCR[]  PROGMEM = "TCR";
constexpr char TEXT_REG_TIR[]  PROGMEM = "TIR";
constexpr char TEXT_REG_TSR[]  PROGMEM = "TSR";
constexpr char TEXT_REG_SCR[]  PROGMEM = "SCR";
constexpr char TEXT_REG_SSR[]  PROGMEM = "SSR";
constexpr char TEXT_REG_SIR[]  PROGMEM = "SIR";
constexpr char TEXT_REG_SOR[]  PROGMEM = "SOR";
constexpr char TEXT_REG_IISR[] PROGMEM = "IISR";
constexpr char TEXT_REG_SBRB[] PROGMEM = "SBRB";
constexpr char TEXT_REG_ICB[]  PROGMEM = "ICB";
constexpr char TEXT_REG_NPP[]  PROGMEM = "NPP";

constexpr char TEXT_CC_SKP[] PROGMEM = "SKP";
// constexpr char TEXT_CC_M[]   PROGMEM = "M";
constexpr char TEXT_CC_PZ[]  PROGMEM = "PZ";
// constexpr char TEXT_CC_Z[]   PROGMEM = "Z";
// constexpr char TEXT_CC_NZ[]  PROGMEM = "NZ";
constexpr char TEXT_CC_MZ[]  PROGMEM = "MZ";
// constexpr char TEXT_CC_P[]   PROGMEM = "P";
constexpr char TEXT_CC_EZ[]  PROGMEM = "EZ";
constexpr char TEXT_CC_ENZ[] PROGMEM = "ENZ";
constexpr char TEXT_CC_OZ[]  PROGMEM = "OZ";
constexpr char TEXT_CC_ONZ[] PROGMEM = "ONZ";
constexpr char TEXT_CC_LMZ[] PROGMEM = "LMZ";
constexpr char TEXT_CC_LP[]  PROGMEM = "LP";
constexpr char TEXT_CC_LPZ[] PROGMEM = "LPZ";
constexpr char TEXT_CC_LM[]  PROGMEM = "LM";
constexpr char TEXT_CC_E[]   PROGMEM = "E";
// constexpr char TEXT_CC_NE[]  PROGMEM = "NE";
// constexpr char TEXT_CC_C[]   PROGMEM = "C";
constexpr char TEXT_CC_RE[]  PROGMEM = "RE";
constexpr char TEXT_CC_SE[]  PROGMEM = "SE";
constexpr char TEXT_CC_CE[]  PROGMEM = "CE";
constexpr char TEXT_CC_NONE[] PROGMEM = "";
// clang-format on

}  // namespace mn1610
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
