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

#ifndef __LIBASM_TEXT_TMS320__
#define __LIBASM_TEXT_TMS320__

#include "text_common.h"

namespace libasm {
namespace text {
namespace tms320 {
// clang-format off

extern const char TEXT_TMS320_LIST[];
extern const char TEXT_CPU_32010[];
extern const char TEXT_CPU_32015[];
extern const char TEXT_CPU_32020[];
extern const char TEXT_CPU_320C20X[];
extern const char TEXT_CPU_320C25[];
extern const char TEXT_CPU_320C26[];
extern const char TEXT_CPU_320C5X[];

// TMS320
using common::TEXT_ABS;
using common::TEXT_ADD;
extern const char TEXT_ADDH[];
using common::TEXT_ADDS;
using common::TEXT_AND;
extern const char TEXT_APAC[];
using common::TEXT_B;
extern const char TEXT_BANZ[];
extern const char TEXT_BGEZ[];
extern const char TEXT_BGZ[];
extern const char TEXT_BIOZ[];
extern const char TEXT_BLEZ[];
extern const char TEXT_BLZ[];
using common::TEXT_BNZ;
using common::TEXT_BV;
using common::TEXT_BZ;
extern const char TEXT_CALA[];
using common::TEXT_CALL;
using common::TEXT_DINT;
extern const char TEXT_DMOV[];
using common::TEXT_EINT;
using common::TEXT_IN;
extern const char TEXT_LAC[];
extern const char TEXT_LACK[];
using common::TEXT_LAR;
extern const char TEXT_LARK[];
extern const char TEXT_LARP[];
using common::TEXT_LDP;
extern const char TEXT_LDPK[];
using common::TEXT_LST;
extern const char TEXT_LT[];
extern const char TEXT_LTA[];
extern const char TEXT_LTD[];
using common::TEXT_MPY;
extern const char TEXT_MPYK[];
using common::TEXT_NOP;
using common::TEXT_OR;
using common::TEXT_OUT;
extern const char TEXT_PAC[];
using common::TEXT_POP;
using common::TEXT_PUSH;
using common::TEXT_RET;
extern const char TEXT_ROVM[];
extern const char TEXT_SACH[];
extern const char TEXT_SACL[];
using common::TEXT_SAR;
extern const char TEXT_SOVM[];
extern const char TEXT_SPAC[];
extern const char TEXT_SST[];
using common::TEXT_SUB;
using common::TEXT_SUBC;
extern const char TEXT_SUBH[];
using common::TEXT_SUBS;
extern const char TEXT_TBLR[];
extern const char TEXT_TBLW[];
using common::TEXT_XOR;
extern const char TEXT_ZAC[];
extern const char TEXT_ZALH[];
extern const char TEXT_ZALS[];
extern const char TEXT_MAR[];

// TMS32020
extern const char TEXT_ADDT[];
extern const char TEXT_ADLK[];
extern const char TEXT_ANDK[];
extern const char TEXT_BACC[];
extern const char TEXT_BBNZ[];
extern const char TEXT_BBZ[];
using common::TEXT_BIT;
extern const char TEXT_BITT[];
extern const char TEXT_BLKD[];
extern const char TEXT_BLKP[];
using common::TEXT_BNV;
using common::TEXT_CMPL;
using common::TEXT_CMPR;
extern const char TEXT_CNFD[];
extern const char TEXT_CNFP[];
extern const char TEXT_FORT[];
using common::TEXT_IDLE;
extern const char TEXT_LACT[];
extern const char TEXT_LALK[];
extern const char TEXT_LPH[];
extern const char TEXT_LRLK[];
extern const char TEXT_LST1[];
extern const char TEXT_LTP[];
extern const char TEXT_LTS[];
using common::TEXT_MAC;
extern const char TEXT_MACD[];
using common::TEXT_NEG;
using common::TEXT_NORM;
extern const char TEXT_ORK[];
extern const char TEXT_POPD[];
using common::TEXT_PSHD;
extern const char TEXT_RPT[];
extern const char TEXT_RPTK[];
extern const char TEXT_RSXM[];
extern const char TEXT_RTXM[];
extern const char TEXT_RXF[];
extern const char TEXT_SBLK[];
extern const char TEXT_SFL[];
extern const char TEXT_SFR[];
extern const char TEXT_SPM[];
extern const char TEXT_SQRA[];
extern const char TEXT_SQRS[];
extern const char TEXT_SST1[];
extern const char TEXT_SSXM[];
extern const char TEXT_STXM[];
extern const char TEXT_SUBT[];
extern const char TEXT_SXF[];
using common::TEXT_TRAP;
extern const char TEXT_XORK[];

// TMS320C25
using common::TEXT_ADDC;
extern const char TEXT_ADDK[];
extern const char TEXT_ADRK[];
using common::TEXT_BC;
using common::TEXT_BNC;
extern const char TEXT_MPYA[];
using common::TEXT_MPYS;
extern const char TEXT_MPYU[];
using common::TEXT_RC;
extern const char TEXT_RFSM[];
extern const char TEXT_RHM[];
using common::TEXT_ROL;
using common::TEXT_ROR;
using common::TEXT_RTC;
extern const char TEXT_SBRK[];
using common::TEXT_SC;
extern const char TEXT_SFSM[];
extern const char TEXT_SHM[];
extern const char TEXT_SPH[];
using common::TEXT_SPL;
using common::TEXT_STC;
using common::TEXT_SUBB;
extern const char TEXT_SUBK[];
extern const char TEXT_ZALR[];

// TMS320C26
extern const char TEXT_CONF[];

// TMS320C2X Enhanced instruction
extern const char TEXT_BCND[];
extern const char TEXT_BLDD[];
extern const char TEXT_BLPD[];
using common::TEXT_CLRC;
extern const char TEXT_LACC[];
extern const char TEXT_LACL[];
using common::TEXT_SETC;

// TMS320C20X
using common::TEXT_CC;
extern const char TEXT_INTR[];
extern const char TEXT_NMI[];
using common::TEXT_RETC;
extern const char TEXT_SPLK[];

using common::TEXT_REG_AR;
extern const char TEXT_REG_PA[];

// TMS320C5X
using common::TEXT_ADCB;
using common::TEXT_ADDB;
using common::TEXT_ANDB;
extern const char TEXT_APL[];
extern const char TEXT_BACCD[];
extern const char TEXT_BANZD[];
extern const char TEXT_BCNDD[];
using common::TEXT_BD;
extern const char TEXT_BLDP[];
extern const char TEXT_BSAR[];
extern const char TEXT_CALAD[];
extern const char TEXT_CALLD[];
extern const char TEXT_CCD[];
using common::TEXT_CPL;
extern const char TEXT_CRGT[];
extern const char TEXT_CRLT[];
extern const char TEXT_EXAR[];
using common::TEXT_IDLE2;
extern const char TEXT_LACB[];
extern const char TEXT_LAMM[];
extern const char TEXT_LMMR[];
extern const char TEXT_MADD[];
extern const char TEXT_MADS[];
extern const char TEXT_OPL[];
using common::TEXT_ORB;
extern const char TEXT_RETCD[];
using common::TEXT_RETD;
using common::TEXT_RETE;
using common::TEXT_RETI;
using common::TEXT_ROLB;
using common::TEXT_RORB;
using common::TEXT_RPTB;
extern const char TEXT_RPTZ[];
extern const char TEXT_SACB[];
extern const char TEXT_SAMM[];
extern const char TEXT_SATH[];
extern const char TEXT_SATL[];
using common::TEXT_SBB;
extern const char TEXT_SBBB[];
extern const char TEXT_SFLB[];
extern const char TEXT_SFRB[];
extern const char TEXT_SMMR[];
extern const char TEXT_XC[];
using common::TEXT_XORB;
extern const char TEXT_XPL[];
extern const char TEXT_ZAP[];
extern const char TEXT_ZPR[];

// TMS320C2X Enhanced instruction
using common::TEXT_REG_C;
extern const char TEXT_REG_CNF[];
extern const char TEXT_REG_HM[];
extern const char TEXT_REG_INTM[];
extern const char TEXT_REG_OVM[];
extern const char TEXT_REG_SXM[];
using common::TEXT_REG_TC;
extern const char TEXT_REG_XF[];

// TMS320C5X
extern const char TEXT_REG_BMAR[];

extern const char TEXT_CC_BIO[];
using common::TEXT_CC_C;
using common::TEXT_CC_EQ;
extern const char TEXT_CC_GEQ[];
using common::TEXT_CC_GT;
extern const char TEXT_CC_LEQ[];
using common::TEXT_CC_LT;
using common::TEXT_CC_NC;
extern const char TEXT_CC_NEQ[];
using common::TEXT_CC_NOV;
extern const char TEXT_CC_NTC[];
using common::TEXT_CC_OV;
extern const char TEXT_CC_TC[];
extern const char TEXT_CC_UNC[];

// clang-format on
}  // namespace tms320
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_TMS320__
// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
