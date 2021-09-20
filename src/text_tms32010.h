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

namespace libasm {
namespace tms32010 {

// clang-format off
static const char TEXT_CPU_LIST[] PROGMEM = "TMS32010, TMS32015";
static constexpr const char *TEXT_CPU_TMS   = TEXT_CPU_LIST;
static const char TEXT_CPU_32010[] PROGMEM = "32010";
static constexpr const char *TEXT_CPU_32015 = (TEXT_CPU_LIST + 13);

// TMS32010
static const char TEXT_ABS[]  PROGMEM = "ABS";
static const char TEXT_ADD[]  PROGMEM = "ADD";
static const char TEXT_ADDH[] PROGMEM = "ADDH";
static const char TEXT_ADDS[] PROGMEM = "ADDS";
static const char TEXT_AND[]  PROGMEM = "AND";
static const char TEXT_APAC[] PROGMEM = "APAC";
static const char TEXT_B[]    PROGMEM = "B";
static const char TEXT_BANZ[] PROGMEM = "BANZ";
static const char TEXT_BGEZ[] PROGMEM = "BGEZ";
static const char TEXT_BGZ[]  PROGMEM = "BGZ";
static const char TEXT_BIOZ[] PROGMEM = "BIOZ";
static const char TEXT_BLEZ[] PROGMEM = "BLEZ";
static const char TEXT_BLZ[]  PROGMEM = "BLZ";
static const char TEXT_BNZ[]  PROGMEM = "BNZ";
static const char TEXT_BV[]   PROGMEM = "BV";
static const char TEXT_BZ[]   PROGMEM = "BZ";
static const char TEXT_CALA[] PROGMEM = "CALA";
static const char TEXT_CALL[] PROGMEM = "CALL";
static const char TEXT_DINT[] PROGMEM = "DINT";
static const char TEXT_DMOV[] PROGMEM = "DMOV";
static const char TEXT_EINT[] PROGMEM = "EINT";
static const char TEXT_IN[]   PROGMEM = "IN";
static const char TEXT_LAC[]  PROGMEM = "LAC";
static const char TEXT_LACK[] PROGMEM = "LACK";
static const char TEXT_LAR[]  PROGMEM = "LAR";
static const char TEXT_LARK[] PROGMEM = "LARK";
static const char TEXT_LARP[] PROGMEM = "LARP";
static const char TEXT_LDP[]  PROGMEM = "LDP";
static const char TEXT_LDPK[] PROGMEM = "LDPK";
static const char TEXT_LST[]  PROGMEM = "LST";
static const char TEXT_LT[]   PROGMEM = "LT";
static const char TEXT_LTA[]  PROGMEM = "LTA";
static const char TEXT_LTD[]  PROGMEM = "LTD";
static const char TEXT_MAR[]  PROGMEM = "MAR";
static const char TEXT_MPY[]  PROGMEM = "MPY";
static const char TEXT_MPYK[] PROGMEM = "MPYK";
static const char TEXT_NOP[]  PROGMEM = "NOP";
static const char TEXT_OR[]   PROGMEM = "OR";
static const char TEXT_OUT[]  PROGMEM = "OUT";
static const char TEXT_PAC[]  PROGMEM = "PAC";
static const char TEXT_POP[]  PROGMEM = "POP";
static const char TEXT_PUSH[] PROGMEM = "PUSH";
static const char TEXT_RET[]  PROGMEM = "RET";
static const char TEXT_ROVM[] PROGMEM = "ROVM";
static const char TEXT_SACH[] PROGMEM = "SACH";
static const char TEXT_SACL[] PROGMEM = "SACL";
static const char TEXT_SAR[]  PROGMEM = "SAR";
static const char TEXT_SOVM[] PROGMEM = "SOVM";
static const char TEXT_SPAC[] PROGMEM = "SPAC";
static const char TEXT_SST[]  PROGMEM = "SST";
static const char TEXT_SUB[]  PROGMEM = "SUB";
static const char TEXT_SUBC[] PROGMEM = "SUBC";
static const char TEXT_SUBH[] PROGMEM = "SUBH";
static const char TEXT_SUBS[] PROGMEM = "SUBS";
static const char TEXT_TBLR[] PROGMEM = "TBLR";
static const char TEXT_TBLW[] PROGMEM = "TBLW";
static const char TEXT_XOR[]  PROGMEM = "XOR";
static const char TEXT_ZAC[]  PROGMEM = "ZAC";
static const char TEXT_ZALH[] PROGMEM = "ZALH";
static const char TEXT_ZALS[] PROGMEM = "ZALS";
// clang-format on

}  // namespace tms32010
}  // namespace libasm

#endif  // __TEXT_TMS32010__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
