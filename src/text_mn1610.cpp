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

constexpr char TEXT_MN1610_LIST[] = "MN1610, MN1613, MN1613A";
constexpr char TEXT_CPU_MN1610[]  = "MN1610";
constexpr char TEXT_CPU_MN1613[]  = "MN1613";
constexpr char TEXT_CPU_MN1613A[] = "MN1613A";

// MN1610
// constexpr char TEXT_A[]    = "A";
// constexpr char TEXT_AI[]   = "AI";
// constexpr char TEXT_AND[]  = "AND";
// constexpr char TEXT_B[]    = "B";
constexpr char TEXT_BAL[]  = "BAL";
constexpr char TEXT_BSWP[] = "BSWP";
// constexpr char TEXT_C[]    = "C";
// constexpr char TEXT_CB[]   = "CB";
// constexpr char TEXT_CLR[]  = "CLR";
constexpr char TEXT_DMS[]  = "DMS";
constexpr char TEXT_DSWP[] = "DSWP";
// constexpr char TEXT_EOR[]  = "EOR";
constexpr char TEXT_H[]    = "H";
constexpr char TEXT_IMS[]  = "IMS";
constexpr char TEXT_L[]    = "L";
constexpr char TEXT_LAD[]  = "LAD";
constexpr char TEXT_LPSW[] = "LPSW";
constexpr char TEXT_MV[]   = "MV";
constexpr char TEXT_MVB[]  = "MVB";
// constexpr char TEXT_MVI[]  = "MVI";
// constexpr char TEXT_OR[]   = "OR";
// constexpr char TEXT_POP[]  = "POP";
// constexpr char TEXT_PUSH[] = "PUSH";
constexpr char TEXT_RBIT[] = "RBIT";
constexpr char TEXT_RD[]   = "RD";
// constexpr char TEXT_RET[]  = "RET";
// constexpr char TEXT_S[]    = "S";
constexpr char TEXT_SBIT[] = "SBIT";
constexpr char TEXT_SI[]   = "SI";
// constexpr char TEXT_SL[]   = "SL";
// constexpr char TEXT_SR[]   = "SR";
// constexpr char TEXT_ST[]   = "ST";
constexpr char TEXT_TBIT[] = "TBIT";
// constexpr char TEXT_TST[]  = "TST";
constexpr char TEXT_WT[]   = "WT";

// MN1613
constexpr char TEXT_AD[]   = "AD";
// constexpr char TEXT_ANDI[] = "ANDI";
// constexpr char TEXT_ANDR[] = "ANDR";
constexpr char TEXT_AWI[]  = "AWI";
constexpr char TEXT_AWR[]  = "AWR";
constexpr char TEXT_BALD[] = "BALD";
constexpr char TEXT_BALL[] = "BALL";
constexpr char TEXT_BALR[] = "BALR";
// constexpr char TEXT_BD[]   = "BD";
// constexpr char TEXT_BL[]   = "BL";
constexpr char TEXT_BLK[]  = "BLK";
// constexpr char TEXT_BR[]   = "BR";
constexpr char TEXT_BSWR[] = "BSWR";
constexpr char TEXT_CBI[]  = "CBI";
constexpr char TEXT_CBR[]  = "CBR";
constexpr char TEXT_CPYB[] = "CPYB";
constexpr char TEXT_CPYH[] = "CPYH";
constexpr char TEXT_CPYS[] = "CPYS";
constexpr char TEXT_CWI[]  = "CWI";
constexpr char TEXT_CWR[]  = "CWR";
constexpr char TEXT_D[]    = "D";
// constexpr char TEXT_DAA[]  = "DAA";
// constexpr char TEXT_DAS[]  = "DAS";
constexpr char TEXT_DEBP[] = "DEBP";
constexpr char TEXT_DSWR[] = "DSWR";
// constexpr char TEXT_EORI[] = "EORI";
// constexpr char TEXT_EORR[] = "EORR";
constexpr char TEXT_FA[]   = "FA";
constexpr char TEXT_FD[]   = "FD";
// constexpr char TEXT_FIX[]  = "FIX";
// constexpr char TEXT_FLT[]  = "FLT";
constexpr char TEXT_FM[]   = "FM";
constexpr char TEXT_FS[]   = "FS";
constexpr char TEXT_LADI[] = "LADI";
constexpr char TEXT_LADR[] = "LADR";
constexpr char TEXT_LB[]   = "LB";
// constexpr char TEXT_LD[]   = "LD";
// constexpr char TEXT_LR[]   = "LR";
constexpr char TEXT_LS[]   = "LS";
constexpr char TEXT_M[]    = "M";
constexpr char TEXT_MVBR[] = "MVBR";
constexpr char TEXT_MVWI[] = "MVWI";
constexpr char TEXT_MVWR[] = "MVWR";
// constexpr char TEXT_NEG[]  = "NEG";
// constexpr char TEXT_ORI[]  = "ORI";
// constexpr char TEXT_ORR[]  = "ORR";
constexpr char TEXT_POPM[] = "POPM";
// constexpr char TEXT_PSHM[] = "PSHM";
constexpr char TEXT_RDR[]  = "RDR";
constexpr char TEXT_RETL[] = "RETL";
// constexpr char TEXT_SD[]   = "SD";
// constexpr char TEXT_SETB[] = "SETB";
constexpr char TEXT_SETH[] = "SETH";
// constexpr char TEXT_SETS[] = "SETS";
constexpr char TEXT_SRBT[] = "SRBT";
// constexpr char TEXT_STB[]  = "STB";
// constexpr char TEXT_STD[]  = "STD";
// constexpr char TEXT_STR[]  = "STR";
// constexpr char TEXT_STS[]  = "STS";
// constexpr char TEXT_SWI[]  = "SWI";
constexpr char TEXT_SWR[]  = "SWR";
constexpr char TEXT_TRST[] = "TRST";
// constexpr char TEXT_TSET[] = "TSET";
constexpr char TEXT_WTR[]  = "WTR";

constexpr char TEXT_REG_CSBR[] = "CSBR";
// constexpr char TEXT_REG_DR0[]  = "DR0";
constexpr char TEXT_REG_IC[]   = "IC";
constexpr char TEXT_REG_ICB[]  = "ICB";
constexpr char TEXT_REG_IISR[] = "IISR";
constexpr char TEXT_REG_NPP[]  = "NPP";
constexpr char TEXT_REG_OSR0[] = "OSR0";
constexpr char TEXT_REG_OSR1[] = "OSR1";
constexpr char TEXT_REG_OSR2[] = "OSR2";
constexpr char TEXT_REG_OSR3[] = "OSR3";
// constexpr char TEXT_REG_R0[] = "R0";
// constexpr char TEXT_REG_R1[] = "R1";
// constexpr char TEXT_REG_R2[] = "R2";
// constexpr char TEXT_REG_R3[] = "R3";
// constexpr char TEXT_REG_R4[] = "R4";
constexpr char TEXT_REG_SBRB[] = "SBRB";
constexpr char TEXT_REG_SCR[]  = "SCR";
constexpr char TEXT_REG_SIR[]  = "SIR";
constexpr char TEXT_REG_SOR[]  = "SOR";
// constexpr char TEXT_REG_SP[] = "SP";
constexpr char TEXT_REG_SSBR[] = "SSBR";
constexpr char TEXT_REG_SSR[]  = "SSR";
constexpr char TEXT_REG_STR[]  = "STR";
constexpr char TEXT_REG_TCR[]  = "TCR";
constexpr char TEXT_REG_TIR[]  = "TIR";
constexpr char TEXT_REG_TSR[]  = "TSR";
constexpr char TEXT_REG_TSR0[] = "TSR0";
constexpr char TEXT_REG_TSR1[] = "TSR1";
// constexpr char TEXT_REG_X0[]   = "X0";
// constexpr char TEXT_REG_X1[]   = "X1";

// constexpr char TEXT_CC_C[]  = "C";
constexpr char TEXT_CC_CE[]   = "CE";
constexpr char TEXT_CC_E[]    = "E";
constexpr char TEXT_CC_ENZ[]  = "ENZ";
constexpr char TEXT_CC_EZ[]   = "EZ";
constexpr char TEXT_CC_LM[]   = "LM";
constexpr char TEXT_CC_LMZ[]  = "LMZ";
constexpr char TEXT_CC_LP[]   = "LP";
constexpr char TEXT_CC_LPZ[]  = "LPZ";
// constexpr char TEXT_CC_M[]  = "M";
constexpr char TEXT_CC_MZ[]   = "MZ";
// constexpr char TEXT_CC_NE[] = "NE";
constexpr char TEXT_CC_NONE[] = "";
// constexpr char TEXT_CC_NZ[] = "NZ";
constexpr char TEXT_CC_ONZ[]  = "ONZ";
constexpr char TEXT_CC_OZ[]   = "OZ";
// constexpr char TEXT_CC_P[]  = "P";
constexpr char TEXT_CC_PZ[]   = "PZ";
constexpr char TEXT_CC_RE[]   = "RE";
constexpr char TEXT_CC_SE[]   = "SE";
constexpr char TEXT_CC_SKP[]  = "SKP";
// constexpr char TEXT_CC_Z[]  = "Z";

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
