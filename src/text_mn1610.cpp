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
const char TEXT_CPU_LIST[]    PROGMEM = "MN1610, MN1613, MN1613A";
const char TEXT_CPU_MN1610[]  PROGMEM = "MN1610";
const char TEXT_CPU_MN1613[]  PROGMEM = "MN1613";
const char TEXT_CPU_MN1613A[] PROGMEM = "MN1613A";

// MN1610
// const char TEXT_AI[]   PROGMEM = "AI";
// const char TEXT_AND[]  PROGMEM = "AND";
// const char TEXT_A[]    PROGMEM = "A";
const char TEXT_BAL[]  PROGMEM = "BAL";
// const char TEXT_B[]    PROGMEM = "B";
const char TEXT_BSWP[] PROGMEM = "BSWP";
// const char TEXT_CB[]   PROGMEM = "CB";
// const char TEXT_C[]    PROGMEM = "C";
const char TEXT_DMS[]  PROGMEM = "DMS";
const char TEXT_DSWP[] PROGMEM = "DSWP";
// const char TEXT_EOR[]  PROGMEM = "EOR";
const char TEXT_H[]    PROGMEM = "H";
const char TEXT_IMS[]  PROGMEM = "IMS";
const char TEXT_LAD[]  PROGMEM = "LAD";
const char TEXT_L[]    PROGMEM = "L";
const char TEXT_LPSW[] PROGMEM = "LPSW";
const char TEXT_MVB[]  PROGMEM = "MVB";
// const char TEXT_MVI[]  PROGMEM = "MVI";
const char TEXT_MV[]   PROGMEM = "MV";
// const char TEXT_OR[]   PROGMEM = "OR";
// const char TEXT_POP[]  PROGMEM = "POP";
// const char TEXT_PUSH[] PROGMEM = "PUSH";
const char TEXT_RBIT[] PROGMEM = "RBIT";
const char TEXT_RD[]   PROGMEM = "RD";
// const char TEXT_RET[]  PROGMEM = "RET";
const char TEXT_SBIT[] PROGMEM = "SBIT";
const char TEXT_SI[]   PROGMEM = "SI";
// const char TEXT_SL[]   PROGMEM = "SL";
// const char TEXT_S[]    PROGMEM = "S";
// const char TEXT_SR[]   PROGMEM = "SR";
// const char TEXT_ST[]   PROGMEM = "ST";
const char TEXT_TBIT[] PROGMEM = "TBIT";
const char TEXT_WT[]   PROGMEM = "WT";

// MN1613
const char TEXT_LB[]   PROGMEM = "LB";
// const char TEXT_SETB[] PROGMEM = "SETB";
const char TEXT_LS[]   PROGMEM = "LS";
const char TEXT_SETS[] PROGMEM = "SETS";
// const char TEXT_STB[]  PROGMEM = "STB";
const char TEXT_CPYB[] PROGMEM = "CPYB";
// const char TEXT_STS[]  PROGMEM = "STS";
const char TEXT_CPYS[] PROGMEM = "CPYS";
const char TEXT_POPM[] PROGMEM = "POPM";
const char TEXT_PSHM[] PROGMEM = "PSHM";
const char TEXT_TRST[] PROGMEM = "TRST";
// const char TEXT_TSET[] PROGMEM = "TSET";
// const char TEXT_FLT[]  PROGMEM = "FLT";
const char TEXT_FIX[]  PROGMEM = "FIX";
// const char TEXT_NEG[]  PROGMEM = "NEG";
const char TEXT_BD[]   PROGMEM = "BD";
const char TEXT_BALD[] PROGMEM = "BALD";
// const char TEXT_BR[]   PROGMEM = "BR";
// const char TEXT_BL[]   PROGMEM = "BL";
const char TEXT_BALR[] PROGMEM = "BALR";
const char TEXT_BALL[] PROGMEM = "BALL";
// const char TEXT_LD[]   PROGMEM = "LD";
// const char TEXT_STD[]  PROGMEM = "STD";
const char TEXT_RDR[]  PROGMEM = "RDR";
// const char TEXT_LR[]   PROGMEM = "LR";
const char TEXT_WTR[]  PROGMEM = "WTR";
// const char TEXT_STR[]  PROGMEM = "STR";
const char TEXT_RETL[] PROGMEM = "RETL";
const char TEXT_BLK[]  PROGMEM = "BLK";
const char TEXT_SRBT[] PROGMEM = "SRBT";
const char TEXT_SETH[] PROGMEM = "SETH";
const char TEXT_DEBP[] PROGMEM = "DEBP";
const char TEXT_CPYH[] PROGMEM = "CPYH";
// const char TEXT_SD[]   PROGMEM = "SD";
const char TEXT_AD[]   PROGMEM = "AD";
const char TEXT_CBR[]  PROGMEM = "CBR";
const char TEXT_CWR[]  PROGMEM = "CWR";
// const char TEXT_DAS[]  PROGMEM = "DAS";
const char TEXT_CBI[]  PROGMEM = "CBI";
const char TEXT_CWI[]  PROGMEM = "CWI";
const char TEXT_SWR[]  PROGMEM = "SWR";
const char TEXT_AWR[]  PROGMEM = "AWR";
// const char TEXT_DAA[]  PROGMEM = "DAA";
// const char TEXT_SWI[]  PROGMEM = "SWI";
const char TEXT_AWI[]  PROGMEM = "AWI";
const char TEXT_FD[]   PROGMEM = "FD";
const char TEXT_FM[]   PROGMEM = "FM";
// const char TEXT_EORR[] PROGMEM = "EORR";
// const char TEXT_EORI[] PROGMEM = "EORI";
// const char TEXT_ORR[]  PROGMEM = "ORR";
// const char TEXT_ORI[]  PROGMEM = "ORI";
const char TEXT_LADR[] PROGMEM = "LADR";
const char TEXT_FS[]   PROGMEM = "FS";
const char TEXT_FA[]   PROGMEM = "FA";
const char TEXT_LADI[] PROGMEM = "LADI";
// const char TEXT_ANDR[] PROGMEM = "ANDR";
// const char TEXT_ANDI[] PROGMEM = "ANDI";
const char TEXT_DSWR[] PROGMEM = "DSWR";
const char TEXT_BSWR[] PROGMEM = "BSWR";
const char TEXT_D[]    PROGMEM = "D";
const char TEXT_MVBR[] PROGMEM = "MVBR";
const char TEXT_MVWR[] PROGMEM = "MVWR";
const char TEXT_M[]    PROGMEM = "M";
const char TEXT_MVWI[] PROGMEM = "MVWI";

// const char TEXT_REG_R0[]   PROGMEM = "R0";
// const char TEXT_REG_R1[]   PROGMEM = "R1";
// const char TEXT_REG_R2[]   PROGMEM = "R2";
// const char TEXT_REG_R3[]   PROGMEM = "R3";
// const char TEXT_REG_R4[]   PROGMEM = "R4";
// const char TEXT_REG_SP[]   PROGMEM = "SP";
const char TEXT_REG_STR[]  PROGMEM = "STR";
const char TEXT_REG_IC[]   PROGMEM = "IC";
const char TEXT_REG_DR0[]  PROGMEM = "DR0";
const char TEXT_REG_X0[]   PROGMEM = "X0";
const char TEXT_REG_X1[]   PROGMEM = "X1";
const char TEXT_REG_CSBR[] PROGMEM = "CSBR";
const char TEXT_REG_SSBR[] PROGMEM = "SSBR";
const char TEXT_REG_TSR0[] PROGMEM = "TSR0";
const char TEXT_REG_TSR1[] PROGMEM = "TSR1";
const char TEXT_REG_OSR0[] PROGMEM = "OSR0";
const char TEXT_REG_OSR1[] PROGMEM = "OSR1";
const char TEXT_REG_OSR2[] PROGMEM = "OSR2";
const char TEXT_REG_OSR3[] PROGMEM = "OSR3";
const char TEXT_REG_TCR[]  PROGMEM = "TCR";
const char TEXT_REG_TIR[]  PROGMEM = "TIR";
const char TEXT_REG_TSR[]  PROGMEM = "TSR";
const char TEXT_REG_SCR[]  PROGMEM = "SCR";
const char TEXT_REG_SSR[]  PROGMEM = "SSR";
const char TEXT_REG_SIR[]  PROGMEM = "SIR";
const char TEXT_REG_SOR[]  PROGMEM = "SOR";
const char TEXT_REG_IISR[] PROGMEM = "IISR";
const char TEXT_REG_SBRB[] PROGMEM = "SBRB";
const char TEXT_REG_ICB[]  PROGMEM = "ICB";
const char TEXT_REG_NPP[]  PROGMEM = "NPP";

const char TEXT_CC_SKP[] PROGMEM = "SKP";
// const char TEXT_CC_M[]   PROGMEM = "M";
const char TEXT_CC_PZ[]  PROGMEM = "PZ";
// const char TEXT_CC_Z[]   PROGMEM = "Z";
// const char TEXT_CC_NZ[]  PROGMEM = "NZ";
const char TEXT_CC_MZ[]  PROGMEM = "MZ";
// const char TEXT_CC_P[]   PROGMEM = "P";
const char TEXT_CC_EZ[]  PROGMEM = "EZ";
const char TEXT_CC_ENZ[] PROGMEM = "ENZ";
const char TEXT_CC_OZ[]  PROGMEM = "OZ";
const char TEXT_CC_ONZ[] PROGMEM = "ONZ";
const char TEXT_CC_LMZ[] PROGMEM = "LMZ";
const char TEXT_CC_LP[]  PROGMEM = "LP";
const char TEXT_CC_LPZ[] PROGMEM = "LPZ";
const char TEXT_CC_LM[]  PROGMEM = "LM";
const char TEXT_CC_E[]   PROGMEM = "E";
// const char TEXT_CC_NE[]  PROGMEM = "NE";
// const char TEXT_CC_C[]   PROGMEM = "C";
const char TEXT_CC_RE[]  PROGMEM = "RE";
const char TEXT_CC_SE[]  PROGMEM = "SE";
const char TEXT_CC_CE[]  PROGMEM = "CE";
const char TEXT_CC_NONE[] PROGMEM = "";
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
