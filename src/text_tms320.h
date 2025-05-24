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
extern const char TEXT_TMS320_LIST[]  PROGMEM;
extern const char TEXT_CPU_32010[] PROGMEM;
extern const char TEXT_CPU_32015[] PROGMEM;
extern const char TEXT_CPU_32020[]  PROGMEM;
extern const char TEXT_CPU_320C25[] PROGMEM;
extern const char TEXT_CPU_320C26[] PROGMEM;

// TMS320
using common::TEXT_ABS;
using common::TEXT_ADD;
extern const char TEXT_ADDH[] PROGMEM;
extern const char TEXT_ADDS[] PROGMEM;
using common::TEXT_AND;
extern const char TEXT_APAC[] PROGMEM;
using common::TEXT_B;
extern const char TEXT_BANZ[] PROGMEM;
extern const char TEXT_BGEZ[] PROGMEM;
extern const char TEXT_BGZ[]  PROGMEM;
extern const char TEXT_BIOZ[] PROGMEM;
extern const char TEXT_BLEZ[] PROGMEM;
extern const char TEXT_BLZ[]  PROGMEM;
using common::TEXT_BNZ;
extern const char TEXT_BV[]   PROGMEM;
using common::TEXT_BZ;
extern const char TEXT_CALA[] PROGMEM;
using common::TEXT_CALL;
using common::TEXT_DINT;
extern const char TEXT_DMOV[] PROGMEM;
using common::TEXT_EINT;
using common::TEXT_IN;
extern const char TEXT_LAC[]  PROGMEM;
extern const char TEXT_LACK[] PROGMEM;
using common::TEXT_LAR;
extern const char TEXT_LARK[] PROGMEM;
extern const char TEXT_LARP[] PROGMEM;
extern const char TEXT_LDP[]  PROGMEM;
extern const char TEXT_LDPK[] PROGMEM;
using common::TEXT_LST;
extern const char TEXT_LT[]   PROGMEM;
extern const char TEXT_LTA[]  PROGMEM;
extern const char TEXT_LTD[]  PROGMEM;
    extern const char TEXT_MAR[]  PROGMEM;
using common::TEXT_MPY;
extern const char TEXT_MPYK[] PROGMEM;
using common::TEXT_NOP;
using common::TEXT_OR;
using common::TEXT_OUT;
extern const char TEXT_PAC[]  PROGMEM;
using common::TEXT_POP;
using common::TEXT_PUSH;
using common::TEXT_RET;
extern const char TEXT_ROVM[] PROGMEM;
extern const char TEXT_SACH[] PROGMEM;
extern const char TEXT_SACL[] PROGMEM;
using common::TEXT_SAR;
extern const char TEXT_SOVM[] PROGMEM;
extern const char TEXT_SPAC[] PROGMEM;
extern const char TEXT_SST[]  PROGMEM;
using common::TEXT_SUB;
using common::TEXT_SUBC;
extern const char TEXT_SUBH[] PROGMEM;
extern const char TEXT_SUBS[] PROGMEM;
extern const char TEXT_TBLR[] PROGMEM;
extern const char TEXT_TBLW[] PROGMEM;
using common::TEXT_XOR;
extern const char TEXT_ZAC[]  PROGMEM;
extern const char TEXT_ZALH[] PROGMEM;
extern const char TEXT_ZALS[] PROGMEM;

// TMS32020
extern const char TEXT_ADDT[] PROGMEM;
extern const char TEXT_ADLK[] PROGMEM;
extern const char TEXT_ANDK[] PROGMEM;
extern const char TEXT_BACC[] PROGMEM;
extern const char TEXT_BBNZ[] PROGMEM;
extern const char TEXT_BBZ[]  PROGMEM;
using common::TEXT_BIT;
extern const char TEXT_BITT[] PROGMEM;
extern const char TEXT_BLKD[] PROGMEM;
extern const char TEXT_BLKP[] PROGMEM;
extern const char TEXT_BNV[]  PROGMEM;
using common::TEXT_CMPL;
using common::TEXT_CMPR;
extern const char TEXT_CNFD[] PROGMEM;
extern const char TEXT_CNFP[] PROGMEM;
extern const char TEXT_FORT[] PROGMEM;
using common::TEXT_IDLE;
extern const char TEXT_LACT[] PROGMEM;
extern const char TEXT_LALK[] PROGMEM;
extern const char TEXT_LPH[]  PROGMEM;
extern const char TEXT_LRLK[] PROGMEM;
extern const char TEXT_LST1[] PROGMEM;
extern const char TEXT_LTP[]  PROGMEM;
extern const char TEXT_LTS[]  PROGMEM;
using common::TEXT_MAC;
extern const char TEXT_MACD[] PROGMEM;
using common::TEXT_NEG;
extern const char TEXT_NORM[] PROGMEM;
extern const char TEXT_ORK[]  PROGMEM;
extern const char TEXT_POPD[] PROGMEM;
using common::TEXT_PSHD;
extern const char TEXT_RPT[]  PROGMEM;
extern const char TEXT_RPTK[] PROGMEM;
extern const char TEXT_RSXM[] PROGMEM;
extern const char TEXT_RTXM[] PROGMEM;
extern const char TEXT_RXF[]  PROGMEM;
extern const char TEXT_SBLK[] PROGMEM;
extern const char TEXT_SFL[]  PROGMEM;
extern const char TEXT_SFR[]  PROGMEM;
extern const char TEXT_SPM[]  PROGMEM;
extern const char TEXT_SQRA[] PROGMEM;
extern const char TEXT_SQRS[] PROGMEM;
extern const char TEXT_SST1[] PROGMEM;
extern const char TEXT_SSXM[] PROGMEM;
extern const char TEXT_SUBT[] PROGMEM;
extern const char TEXT_STXM[] PROGMEM;
extern const char TEXT_SXF[]  PROGMEM;
using common::TEXT_TRAP;
extern const char TEXT_XORK[] PROGMEM;

// TMS320C25
using common::TEXT_ADDC;
extern const char TEXT_ADDK[] PROGMEM;
extern const char TEXT_ADRK[] PROGMEM;
using common::TEXT_BC;
using common::TEXT_BNC;
extern const char TEXT_MPYA[] PROGMEM;
using common::TEXT_MPYS;
extern const char TEXT_MPYU[] PROGMEM;
using common::TEXT_RC;
extern const char TEXT_RFSM[] PROGMEM;
extern const char TEXT_RHM[]  PROGMEM;
using common::TEXT_ROL;
using common::TEXT_ROR;
using common::TEXT_RTC;
extern const char TEXT_SBRK[] PROGMEM;
using common::TEXT_SC;
extern const char TEXT_SFSM[] PROGMEM;
extern const char TEXT_SHM[]  PROGMEM;
extern const char TEXT_SPH[]  PROGMEM;
using common::TEXT_SPL;
using common::TEXT_STC;
using common::TEXT_SUBB;
extern const char TEXT_SUBK[] PROGMEM;
extern const char TEXT_ZALR[] PROGMEM;

// TMS320C26
extern const char TEXT_CONF[] PROGMEM;

extern const char TEXT_REG_AR[] PROGMEM;
extern const char TEXT_REG_PA[] PROGMEM;

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
