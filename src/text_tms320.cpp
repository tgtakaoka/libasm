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
constexpr char TEXT_TMS320_LIST[] PROGMEM =
    "TMS32010, TMS32015, TMS32020, TMS320C25, TMS320C26, TMS320C20X, TMS320C5X";
constexpr char TEXT_CPU_32010[] PROGMEM = "32010";
constexpr char TEXT_CPU_32015[] PROGMEM = "32015";
constexpr char TEXT_CPU_32020[] PROGMEM = "32020";
constexpr char TEXT_CPU_320C25[] PROGMEM = "320C25";
constexpr char TEXT_CPU_320C26[] PROGMEM = "320C26";
constexpr char TEXT_CPU_320C20X[] PROGMEM = "320C20X";
constexpr char TEXT_CPU_320C5X[] PROGMEM = "320C5X";

// TMS320
// constexpr char TEXT_ABS[]  PROGMEM = "ABS";
// constexpr char TEXT_ADD[]  PROGMEM = "ADD";
constexpr char TEXT_ADDH[] PROGMEM = "ADDH";
constexpr char TEXT_ADDS[] PROGMEM = "ADDS";
// constexpr char TEXT_AND[]  PROGMEM = "AND";
constexpr char TEXT_APAC[] PROGMEM = "APAC";
// constexpr char TEXT_B[]    PROGMEM = "B";
constexpr char TEXT_BANZ[] PROGMEM = "BANZ";
constexpr char TEXT_BGEZ[] PROGMEM = "BGEZ";
constexpr char TEXT_BGZ[]  PROGMEM = "BGZ";
constexpr char TEXT_BIOZ[] PROGMEM = "BIOZ";
constexpr char TEXT_BLEZ[] PROGMEM = "BLEZ";
constexpr char TEXT_BLZ[]  PROGMEM = "BLZ";
// constexpr char TEXT_BNZ[]  PROGMEM = "BNZ";
constexpr char TEXT_BV[]   PROGMEM = "BV";
// constexpr char TEXT_BZ[]   PROGMEM = "BZ";
constexpr char TEXT_CALA[] PROGMEM = "CALA";
// constexpr char TEXT_CALL[] PROGMEM = "CALL";
// constexpr char TEXT_DINT[] PROGMEM = "DINT";
constexpr char TEXT_DMOV[] PROGMEM = "DMOV";
// constexpr char TEXT_EINT[] PROGMEM = "EINT";
// constexpr char TEXT_IN[]   PROGMEM = "IN";
constexpr char TEXT_LAC[]  PROGMEM = "LAC";
constexpr char TEXT_LACK[] PROGMEM = "LACK";
// constexpr char TEXT_LAR[]  PROGMEM = "LAR";
constexpr char TEXT_LARK[] PROGMEM = "LARK";
constexpr char TEXT_LARP[] PROGMEM = "LARP";
constexpr char TEXT_LDP[]  PROGMEM = "LDP";
constexpr char TEXT_LDPK[] PROGMEM = "LDPK";
// constexpr char TEXT_LST[]  PROGMEM = "LST";
constexpr char TEXT_LT[]   PROGMEM = "LT";
constexpr char TEXT_LTA[]  PROGMEM = "LTA";
constexpr char TEXT_LTD[]  PROGMEM = "LTD";
constexpr char TEXT_MAR[]  PROGMEM = "MAR";
// constexpr char TEXT_MPY[]  PROGMEM = "MPY";
constexpr char TEXT_MPYK[] PROGMEM = "MPYK";
// constexpr char TEXT_NOP[]  PROGMEM = "NOP";
// constexpr char TEXT_OR[]   PROGMEM = "OR";
// constexpr char TEXT_OUT[]  PROGMEM = "OUT";
constexpr char TEXT_PAC[]  PROGMEM = "PAC";
// constexpr char TEXT_POP[]  PROGMEM = "POP";
// constexpr char TEXT_PUSH[] PROGMEM = "PUSH";
// constexpr char TEXT_RET[]  PROGMEM = "RET";
constexpr char TEXT_ROVM[] PROGMEM = "ROVM";
constexpr char TEXT_SACH[] PROGMEM = "SACH";
constexpr char TEXT_SACL[] PROGMEM = "SACL";
// constexpr char TEXT_SAR[]  PROGMEM = "SAR";
constexpr char TEXT_SOVM[] PROGMEM = "SOVM";
constexpr char TEXT_SPAC[] PROGMEM = "SPAC";
constexpr char TEXT_SST[]  PROGMEM = "SST";
// constexpr char TEXT_SUB[]  PROGMEM = "SUB";
// constexpr char TEXT_SUBC[] PROGMEM = "SUBC";
constexpr char TEXT_SUBH[] PROGMEM = "SUBH";
constexpr char TEXT_SUBS[] PROGMEM = "SUBS";
constexpr char TEXT_TBLR[] PROGMEM = "TBLR";
constexpr char TEXT_TBLW[] PROGMEM = "TBLW";
// constexpr char TEXT_XOR[]  PROGMEM = "XOR";
constexpr char TEXT_ZAC[]  PROGMEM = "ZAC";
constexpr char TEXT_ZALH[] PROGMEM = "ZALH";
constexpr char TEXT_ZALS[] PROGMEM = "ZALS";

// TMS32020
constexpr char TEXT_ADDT[] PROGMEM = "ADDT";
constexpr char TEXT_ADLK[] PROGMEM = "ADLK";
constexpr char TEXT_ANDK[] PROGMEM = "ANDK";
constexpr char TEXT_BACC[] PROGMEM = "BACC";
constexpr char TEXT_BBNZ[] PROGMEM = "BBNZ";
constexpr char TEXT_BBZ[]  PROGMEM = "BBZ";
// constexpr char TEXT_BIT[]  PROGMEM = "BIT";
constexpr char TEXT_BITT[] PROGMEM = "BITT";
constexpr char TEXT_BLKD[] PROGMEM = "BLKD";
constexpr char TEXT_BLKP[] PROGMEM = "BLKP";
constexpr char TEXT_BNV[]  PROGMEM = "BNV";
// constexpr char TEXT_CMPL[] PROGMEM = "CMPL";
// constexpr char TEXT_CMPR[] PROGMEM = "CMPR";
constexpr char TEXT_CNFD[] PROGMEM = "CNFD";
constexpr char TEXT_CNFP[] PROGMEM = "CNFP";
constexpr char TEXT_FORT[] PROGMEM = "FORT";
// constexpr char TEXT_IDLE[] PROGMEM = "IDLE";
constexpr char TEXT_LACT[] PROGMEM = "LACT";
constexpr char TEXT_LALK[] PROGMEM = "LALK";
constexpr char TEXT_LPH[]  PROGMEM = "LPH";
constexpr char TEXT_LRLK[] PROGMEM = "LRLK";
constexpr char TEXT_LST1[] PROGMEM = "LST1";
constexpr char TEXT_LTP[]  PROGMEM = "LTP";
constexpr char TEXT_LTS[]  PROGMEM = "LTS";
// constexpr char TEXT_MAC[]  PROGMEM = "MAC";
constexpr char TEXT_MACD[] PROGMEM = "MACD";
// constexpr char TEXT_NEG[]  PROGMEM = "NEG";
constexpr char TEXT_NORM[] PROGMEM = "NORM";
constexpr char TEXT_ORK[]  PROGMEM = "ORK";
constexpr char TEXT_POPD[] PROGMEM = "POPD";
// constexpr char TEXT_PSHD[] PROGMEM = "PSHD";
constexpr char TEXT_RPT[]  PROGMEM = "RPT";
constexpr char TEXT_RPTK[] PROGMEM = "RPTK";
constexpr char TEXT_RSXM[] PROGMEM = "RSXM";
constexpr char TEXT_RTXM[] PROGMEM = "RTXM";
constexpr char TEXT_RXF[]  PROGMEM = "RXF";
constexpr char TEXT_SBLK[] PROGMEM = "SBLK";
constexpr char TEXT_SFL[]  PROGMEM = "SFL";
constexpr char TEXT_SFR[]  PROGMEM = "SFR";
constexpr char TEXT_SPM[]  PROGMEM = "SPM";
constexpr char TEXT_SQRA[] PROGMEM = "SQRA";
constexpr char TEXT_SQRS[] PROGMEM = "SQRS";
constexpr char TEXT_SST1[] PROGMEM = "SST1";
constexpr char TEXT_SSXM[] PROGMEM = "SSXM";
constexpr char TEXT_SUBT[] PROGMEM = "SUBT";
constexpr char TEXT_STXM[] PROGMEM = "STXM";
constexpr char TEXT_SXF[]  PROGMEM = "SXF";
// constexpr char TEXT_TRAP[] PROGMEM = "TRAP";
constexpr char TEXT_XORK[] PROGMEM = "XORK";

// TMS320C25
// constexpr char TEXT_ADDC[] PROGMEM = "ADDC";
constexpr char TEXT_ADDK[] PROGMEM = "ADDK";
constexpr char TEXT_ADRK[] PROGMEM = "ADRK";
// constexpr char TEXT_BC[]   PROGMEM = "BC";
// constexpr char TEXT_BNC[]  PROGMEM = "BNC";
constexpr char TEXT_MPYA[] PROGMEM = "MPYA";
// constexpr char TEXT_MPYS[] PROGMEM = "MPYS";
constexpr char TEXT_MPYU[] PROGMEM = "MPYU";
// constexpr char TEXT_RC[]   PROGMEM = "RC";
constexpr char TEXT_RFSM[] PROGMEM = "RFSM";
constexpr char TEXT_RHM[]  PROGMEM = "RHM";
// constexpr char TEXT_ROL[]  PROGMEM = "ROL";
// constexpr char TEXT_ROR[]  PROGMEM = "ROR";
// constexpr char TEXT_RTC[]  PROGMEM = "RTC";
constexpr char TEXT_SBRK[] PROGMEM = "SBRK";
// constexpr char TEXT_SC[]   PROGMEM = "SC";
constexpr char TEXT_SFSM[] PROGMEM = "SFSM";
constexpr char TEXT_SHM[]  PROGMEM = "SHM";
constexpr char TEXT_SPH[]  PROGMEM = "SPH";
// constexpr char TEXT_SPL[]  PROGMEM = "SPL";
// constexpr char TEXT_STC[]  PROGMEM = "STC";
// constexpr char TEXT_SUBB[] PROGMEM = "SUBB";
constexpr char TEXT_SUBK[] PROGMEM = "SUBK";
constexpr char TEXT_ZALR[] PROGMEM = "ZALR";

// TMS320C26
constexpr char TEXT_CONF[] PROGMEM = "CONF";

// TMS320C2X Enhanced instruction
constexpr char TEXT_BCND[] PROGMEM = "BCND";
constexpr char TEXT_BLDD[] PROGMEM = "BLDD";
constexpr char TEXT_BLPD[] PROGMEM = "BLPD";
// constexpr char TEXT_CLRC[] PROGMEM = "CLRC";
constexpr char TEXT_LACC[] PROGMEM = "LACC";
constexpr char TEXT_LACL[] PROGMEM = "LACL";
// constexpr char TEXT_SETC[] PROGMEM = "SETC";

// TMS320C20X
// constexpr char TEXT_CC[]   PROGMEM = "CC";
constexpr char TEXT_NMI[]  PROGMEM = "NMI";
constexpr char TEXT_INTR[] PROGMEM = "INTR";
constexpr char TEXT_SPLK[] PROGMEM = "SPLK";
// constexpr char TEXT_RETC[] PROGMEM = "RETC";

// TMS320C5X
// constexpr char TEXT_ADCB[]  PROGMEM = "ADCB";
// constexpr char TEXT_ADDB[]  PROGMEM = "ADDB";
// constexpr char TEXT_ANDB[]  PROGMEM = "ANDB";
constexpr char TEXT_APL[]   PROGMEM = "APL";
constexpr char TEXT_BACCD[] PROGMEM = "BACCD";
constexpr char TEXT_BANZD[] PROGMEM = "BANZD";
constexpr char TEXT_BCNDD[] PROGMEM = "BCNDD";
// constexpr char TEXT_BD[]    PROGMEM = "BD";
constexpr char TEXT_BLDP[]  PROGMEM = "BLDP";
constexpr char TEXT_BSAR[]  PROGMEM = "BSAR";
constexpr char TEXT_CALAD[] PROGMEM = "CALAD";
constexpr char TEXT_CALLD[] PROGMEM = "CALLD";
constexpr char TEXT_CCD[]   PROGMEM = "CCD";
// constexpr char TEXT_CPL[]   PROGMEM = "CPL";
constexpr char TEXT_CRGT[]  PROGMEM = "CRGT";
constexpr char TEXT_CRLT[]  PROGMEM = "CRLT";
constexpr char TEXT_EXAR[]  PROGMEM = "EXAR";
constexpr char TEXT_IDLE2[] PROGMEM = "IDLE2";
constexpr char TEXT_LACB[]  PROGMEM = "LACB";
constexpr char TEXT_LAMM[]  PROGMEM = "LAMM";
constexpr char TEXT_LMMR[]  PROGMEM = "LMMR";
constexpr char TEXT_MADD[]  PROGMEM = "MADD";
constexpr char TEXT_MADS[]  PROGMEM = "MADS";
constexpr char TEXT_OPL[]   PROGMEM = "OPL";
// constexpr char TEXT_ORB[]   PROGMEM = "ORB";
constexpr char TEXT_RETCD[] PROGMEM = "RETCD";
constexpr char TEXT_RETD[]  PROGMEM = "RETD";
// constexpr char TEXT_RETE[]  PROGMEM = "RETE";
// constexpr char TEXT_RETI[]  PROGMEM = "RETI";
// constexpr char TEXT_ROLB[]  PROGMEM = "ROLB";
// constexpr char TEXT_RORB[]  PROGMEM = "RORB";
constexpr char TEXT_RPTB[]  PROGMEM = "RPTB";
constexpr char TEXT_RPTZ[]  PROGMEM = "RPTZ";
constexpr char TEXT_SACB[]  PROGMEM = "SACB";
constexpr char TEXT_SAMM[]  PROGMEM = "SAMM";
constexpr char TEXT_SATH[]  PROGMEM = "SATH";
constexpr char TEXT_SATL[]  PROGMEM = "SATL";
// constexpr char TEXT_SBB[]   PROGMEM = "SBB";
constexpr char TEXT_SBBB[]  PROGMEM = "SBBB";
constexpr char TEXT_SFLB[]  PROGMEM = "SFLB";
constexpr char TEXT_SFRB[]  PROGMEM = "SFRB";
constexpr char TEXT_SMMR[]  PROGMEM = "SMMR";
constexpr char TEXT_XC[]    PROGMEM = "XC";
// constexpr char TEXT_XORB[]  PROGMEM = "XORB";
constexpr char TEXT_XPL[]   PROGMEM = "XPL";
constexpr char TEXT_ZAP[]   PROGMEM = "ZAP";
constexpr char TEXT_ZPR[]   PROGMEM = "ZPR";

constexpr char TEXT_REG_AR[] PROGMEM = "AR";
constexpr char TEXT_REG_PA[] PROGMEM = "PA";

// TMS320C2X Enhanced instruction
constexpr char TEXT_REG_CNF[]  PROGMEM = "CNF";
// constexpr char TEXT_REG_C[]    PROGMEM = "C";
constexpr char TEXT_REG_HM[]   PROGMEM = "HM";
constexpr char TEXT_REG_INTM[] PROGMEM = "INTM";
constexpr char TEXT_REG_OVM[]  PROGMEM = "OVM";
constexpr char TEXT_REG_SXM[]  PROGMEM = "SXM";
// constexpr char TEXT_REG_TC[]   PROGMEM = "TC";
constexpr char TEXT_REG_XF[]   PROGMEM = "XF";

// TMS320C5X
constexpr char TEXT_REG_BMAR[] PROGMEM = "BMAR";

constexpr char TEXT_CC_BIO[] PROGMEM = "BIO";
// constexpr char TEXT_CC_C[]   PROGMEM = "C";
// constexpr char TEXT_CC_EQ[]  PROGMEM = "EQ";
constexpr char TEXT_CC_GEQ[] PROGMEM = "GEQ";
// constexpr char TEXT_CC_GT[]  PROGMEM = "GT";
constexpr char TEXT_CC_LEQ[] PROGMEM = "LEQ";
// constexpr char TEXT_CC_LT[]  PROGMEM = "LT";
// constexpr char TEXT_CC_NC[]  PROGMEM = "NC";
constexpr char TEXT_CC_NEQ[] PROGMEM = "NEQ";
// constexpr char TEXT_CC_NOV[] PROGMEM = "NOV";
constexpr char TEXT_CC_NTC[] PROGMEM = "NTC";
// constexpr char TEXT_CC_OV[]  PROGMEM = "OV";
constexpr char TEXT_CC_TC[]  PROGMEM = "TC";
constexpr char TEXT_CC_UNC[] PROGMEM = "UNC";

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
