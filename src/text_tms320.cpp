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

#include "text_tms320.h"

namespace libasm {
namespace text {
namespace tms320 {
// clang-format off

constexpr char TEXT_TMS320_LIST[] = "TMS32010, TMS32015, TMS32020, TMS320C25, TMS320C26, TMS320C20X, TMS320C5X";
constexpr char TEXT_CPU_32010[]   = "32010";
constexpr char TEXT_CPU_32015[]   = "32015";
constexpr char TEXT_CPU_32020[]   = "32020";
constexpr char TEXT_CPU_320C20X[] = "320C20X";
constexpr char TEXT_CPU_320C25[]  = "320C25";
constexpr char TEXT_CPU_320C26[]  = "320C26";
constexpr char TEXT_CPU_320C5X[]  = "320C5X";

// TMS320
// constexpr char TEXT_ABS[]  = "ABS";
// constexpr char TEXT_ADD[]  = "ADD";
constexpr char TEXT_ADDH[] = "ADDH";
// constexpr char TEXT_ADDS[] = "ADDS";
// constexpr char TEXT_AND[]  = "AND";
constexpr char TEXT_APAC[] = "APAC";
// constexpr char TEXT_B[]    = "B";
constexpr char TEXT_BANZ[] = "BANZ";
constexpr char TEXT_BGEZ[] = "BGEZ";
constexpr char TEXT_BGZ[]  = "BGZ";
constexpr char TEXT_BIOZ[] = "BIOZ";
constexpr char TEXT_BLEZ[] = "BLEZ";
constexpr char TEXT_BLZ[]  = "BLZ";
// constexpr char TEXT_BNZ[]  = "BNZ";
// constexpr char TEXT_BV[]   = "BV";
// constexpr char TEXT_BZ[]   = "BZ";
constexpr char TEXT_CALA[] = "CALA";
// constexpr char TEXT_CALL[] = "CALL";
// constexpr char TEXT_DINT[] = "DINT";
constexpr char TEXT_DMOV[] = "DMOV";
// constexpr char TEXT_EINT[] = "EINT";
// constexpr char TEXT_IN[]   = "IN";
constexpr char TEXT_LAC[]  = "LAC";
constexpr char TEXT_LACK[] = "LACK";
// constexpr char TEXT_LAR[]  = "LAR";
constexpr char TEXT_LARK[] = "LARK";
constexpr char TEXT_LARP[] = "LARP";
// constexpr char TEXT_LDP[]  = "LDP";
constexpr char TEXT_LDPK[] = "LDPK";
// constexpr char TEXT_LST[]  = "LST";
constexpr char TEXT_LT[]   = "LT";
constexpr char TEXT_LTA[]  = "LTA";
constexpr char TEXT_LTD[]  = "LTD";
constexpr char TEXT_MAR[]  = "MAR";
// constexpr char TEXT_MPY[]  = "MPY";
constexpr char TEXT_MPYK[] = "MPYK";
// constexpr char TEXT_NOP[]  = "NOP";
// constexpr char TEXT_OR[]   = "OR";
// constexpr char TEXT_OUT[]  = "OUT";
constexpr char TEXT_PAC[]  = "PAC";
// constexpr char TEXT_POP[]  = "POP";
// constexpr char TEXT_PUSH[] = "PUSH";
// constexpr char TEXT_RET[]  = "RET";
constexpr char TEXT_ROVM[] = "ROVM";
constexpr char TEXT_SACH[] = "SACH";
constexpr char TEXT_SACL[] = "SACL";
// constexpr char TEXT_SAR[]  = "SAR";
constexpr char TEXT_SOVM[] = "SOVM";
constexpr char TEXT_SPAC[] = "SPAC";
constexpr char TEXT_SST[]  = "SST";
// constexpr char TEXT_SUB[]  = "SUB";
// constexpr char TEXT_SUBC[] = "SUBC";
constexpr char TEXT_SUBH[] = "SUBH";
// constexpr char TEXT_SUBS[] = "SUBS";
constexpr char TEXT_TBLR[] = "TBLR";
constexpr char TEXT_TBLW[] = "TBLW";
// constexpr char TEXT_XOR[]  = "XOR";
constexpr char TEXT_ZAC[]  = "ZAC";
constexpr char TEXT_ZALH[] = "ZALH";
constexpr char TEXT_ZALS[] = "ZALS";

// TMS32020
constexpr char TEXT_ADDT[] = "ADDT";
constexpr char TEXT_ADLK[] = "ADLK";
constexpr char TEXT_ANDK[] = "ANDK";
constexpr char TEXT_BACC[] = "BACC";
constexpr char TEXT_BBNZ[] = "BBNZ";
constexpr char TEXT_BBZ[]  = "BBZ";
// constexpr char TEXT_BIT[]  = "BIT";
constexpr char TEXT_BITT[] = "BITT";
constexpr char TEXT_BLKD[] = "BLKD";
constexpr char TEXT_BLKP[] = "BLKP";
// constexpr char TEXT_BNV[]  = "BNV";
// constexpr char TEXT_CMPL[] = "CMPL";
// constexpr char TEXT_CMPR[] = "CMPR";
constexpr char TEXT_CNFD[] = "CNFD";
constexpr char TEXT_CNFP[] = "CNFP";
constexpr char TEXT_FORT[] = "FORT";
// constexpr char TEXT_IDLE[] = "IDLE";
constexpr char TEXT_LACT[] = "LACT";
constexpr char TEXT_LALK[] = "LALK";
constexpr char TEXT_LPH[]  = "LPH";
constexpr char TEXT_LRLK[] = "LRLK";
constexpr char TEXT_LST1[] = "LST1";
constexpr char TEXT_LTP[]  = "LTP";
constexpr char TEXT_LTS[]  = "LTS";
// constexpr char TEXT_MAC[]  = "MAC";
constexpr char TEXT_MACD[] = "MACD";
// constexpr char TEXT_NEG[]  = "NEG";
// constexpr char TEXT_NORM[] = "NORM";
constexpr char TEXT_ORK[]  = "ORK";
constexpr char TEXT_POPD[] = "POPD";
// constexpr char TEXT_PSHD[] = "PSHD";
constexpr char TEXT_RPT[]  = "RPT";
constexpr char TEXT_RPTK[] = "RPTK";
constexpr char TEXT_RSXM[] = "RSXM";
constexpr char TEXT_RTXM[] = "RTXM";
constexpr char TEXT_RXF[]  = "RXF";
constexpr char TEXT_SBLK[] = "SBLK";
constexpr char TEXT_SFL[]  = "SFL";
constexpr char TEXT_SFR[]  = "SFR";
constexpr char TEXT_SPM[]  = "SPM";
constexpr char TEXT_SQRA[] = "SQRA";
constexpr char TEXT_SQRS[] = "SQRS";
constexpr char TEXT_SST1[] = "SST1";
constexpr char TEXT_SSXM[] = "SSXM";
constexpr char TEXT_STXM[] = "STXM";
constexpr char TEXT_SUBT[] = "SUBT";
constexpr char TEXT_SXF[]  = "SXF";
// constexpr char TEXT_TRAP[] = "TRAP";
constexpr char TEXT_XORK[] = "XORK";

// TMS320C25
// constexpr char TEXT_ADDC[] = "ADDC";
constexpr char TEXT_ADDK[] = "ADDK";
constexpr char TEXT_ADRK[] = "ADRK";
// constexpr char TEXT_BC[]   = "BC";
// constexpr char TEXT_BNC[]  = "BNC";
constexpr char TEXT_MPYA[] = "MPYA";
// constexpr char TEXT_MPYS[] = "MPYS";
constexpr char TEXT_MPYU[] = "MPYU";
// constexpr char TEXT_RC[]   = "RC";
constexpr char TEXT_RFSM[] = "RFSM";
constexpr char TEXT_RHM[]  = "RHM";
// constexpr char TEXT_ROL[]  = "ROL";
// constexpr char TEXT_ROR[]  = "ROR";
// constexpr char TEXT_RTC[]  = "RTC";
constexpr char TEXT_SBRK[] = "SBRK";
// constexpr char TEXT_SC[]   = "SC";
constexpr char TEXT_SFSM[] = "SFSM";
constexpr char TEXT_SHM[]  = "SHM";
constexpr char TEXT_SPH[]  = "SPH";
// constexpr char TEXT_SPL[]  = "SPL";
// constexpr char TEXT_STC[]  = "STC";
// constexpr char TEXT_SUBB[] = "SUBB";
constexpr char TEXT_SUBK[] = "SUBK";
constexpr char TEXT_ZALR[] = "ZALR";

// TMS320C26
constexpr char TEXT_CONF[] = "CONF";

// TMS320C2X Enhanced instruction
constexpr char TEXT_BCND[] = "BCND";
constexpr char TEXT_BLDD[] = "BLDD";
constexpr char TEXT_BLPD[] = "BLPD";
// constexpr char TEXT_CLRC[] = "CLRC";
constexpr char TEXT_LACC[] = "LACC";
constexpr char TEXT_LACL[] = "LACL";
// constexpr char TEXT_SETC[] = "SETC";

// TMS320C20X
// constexpr char TEXT_CC[]   = "CC";
constexpr char TEXT_NMI[]  = "NMI";
constexpr char TEXT_INTR[] = "INTR";
constexpr char TEXT_SPLK[] = "SPLK";
// constexpr char TEXT_RETC[] = "RETC";

// TMS320C5X
// constexpr char TEXT_ADCB[]  = "ADCB";
// constexpr char TEXT_ADDB[]  = "ADDB";
// constexpr char TEXT_ANDB[]  = "ANDB";
constexpr char TEXT_APL[]   = "APL";
constexpr char TEXT_BACCD[] = "BACCD";
constexpr char TEXT_BANZD[] = "BANZD";
constexpr char TEXT_BCNDD[] = "BCNDD";
// constexpr char TEXT_BD[]    = "BD";
constexpr char TEXT_BLDP[]  = "BLDP";
constexpr char TEXT_BSAR[]  = "BSAR";
constexpr char TEXT_CALAD[] = "CALAD";
constexpr char TEXT_CALLD[] = "CALLD";
constexpr char TEXT_CCD[]   = "CCD";
// constexpr char TEXT_CPL[]   = "CPL";
constexpr char TEXT_CRGT[]  = "CRGT";
constexpr char TEXT_CRLT[]  = "CRLT";
constexpr char TEXT_EXAR[]  = "EXAR";
// constexpr char TEXT_IDLE2[] = "IDLE2";
constexpr char TEXT_LACB[]  = "LACB";
constexpr char TEXT_LAMM[]  = "LAMM";
constexpr char TEXT_LMMR[]  = "LMMR";
constexpr char TEXT_MADD[]  = "MADD";
constexpr char TEXT_MADS[]  = "MADS";
constexpr char TEXT_OPL[]   = "OPL";
// constexpr char TEXT_ORB[]   = "ORB";
constexpr char TEXT_RETCD[] = "RETCD";
// constexpr char TEXT_RETD[]  = "RETD";
// constexpr char TEXT_RETE[]  = "RETE";
// constexpr char TEXT_RETI[]  = "RETI";
// constexpr char TEXT_ROLB[]  = "ROLB";
// constexpr char TEXT_RORB[]  = "RORB";
// constexpr char TEXT_RPTB[]  = "RPTB";
constexpr char TEXT_RPTZ[]  = "RPTZ";
constexpr char TEXT_SACB[]  = "SACB";
constexpr char TEXT_SAMM[]  = "SAMM";
constexpr char TEXT_SATH[]  = "SATH";
constexpr char TEXT_SATL[]  = "SATL";
// constexpr char TEXT_SBB[]   = "SBB";
constexpr char TEXT_SBBB[]  = "SBBB";
constexpr char TEXT_SFLB[]  = "SFLB";
constexpr char TEXT_SFRB[]  = "SFRB";
constexpr char TEXT_SMMR[]  = "SMMR";
constexpr char TEXT_XC[]    = "XC";
// constexpr char TEXT_XORB[]  = "XORB";
constexpr char TEXT_XPL[]   = "XPL";
constexpr char TEXT_ZAP[]   = "ZAP";
constexpr char TEXT_ZPR[]   = "ZPR";

// constexpr char TEXT_REG_AR[] = "AR";
constexpr char TEXT_REG_PA[] = "PA";

// TMS320C2X Enhanced instruction
constexpr char TEXT_REG_CNF[]  = "CNF";
// constexpr char TEXT_REG_C[]    = "C";
constexpr char TEXT_REG_HM[]   = "HM";
constexpr char TEXT_REG_INTM[] = "INTM";
constexpr char TEXT_REG_OVM[]  = "OVM";
constexpr char TEXT_REG_SXM[]  = "SXM";
// constexpr char TEXT_REG_TC[]   = "TC";
constexpr char TEXT_REG_XF[]   = "XF";

// TMS320C5X
constexpr char TEXT_REG_BMAR[] = "BMAR";

constexpr char TEXT_CC_BIO[] = "BIO";
// constexpr char TEXT_CC_C[]   = "C";
// constexpr char TEXT_CC_EQ[]  = "EQ";
constexpr char TEXT_CC_GEQ[] = "GEQ";
// constexpr char TEXT_CC_GT[]  = "GT";
constexpr char TEXT_CC_LEQ[] = "LEQ";
// constexpr char TEXT_CC_LT[]  = "LT";
// constexpr char TEXT_CC_NC[]  = "NC";
constexpr char TEXT_CC_NEQ[] = "NEQ";
// constexpr char TEXT_CC_NOV[] = "NOV";
constexpr char TEXT_CC_NTC[] = "NTC";
// constexpr char TEXT_CC_OV[]  = "OV";
constexpr char TEXT_CC_TC[]  = "TC";
constexpr char TEXT_CC_UNC[] = "UNC";

// clang-format on
}  // namespace tms320
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
