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

#ifndef __TEXT_TMS32010__
#define __TEXT_TMS32010__

#include "text_common.h"

namespace libasm {
namespace text {
namespace tms32010 {

// clang-format off
extern const char TEXT_CPU_LIST[]  PROGMEM;
extern const char TEXT_CPU_32010[] PROGMEM;
extern const char TEXT_CPU_32015[] PROGMEM;

// TMS32010
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
extern const char TEXT_LAR[]  PROGMEM;
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
// clang-format on

}  // namespace tms32010
}  // namespace text
}  // namespace libasm

#endif  // __TEXT_TMS32010__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
