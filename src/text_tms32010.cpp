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

#include "text_tms32010.h"

namespace libasm {
namespace text {
namespace tms32010 {

// clang-format off
const char TEXT_CPU_LIST[] PROGMEM = "TMS32010, TMS32015";
const char TEXT_CPU_32010[] PROGMEM = "32010";
const char TEXT_CPU_32015[] PROGMEM = "32015";

// TMS32010
// const char TEXT_ABS[]  PROGMEM = "ABS";
// const char TEXT_ADD[]  PROGMEM = "ADD";
const char TEXT_ADDH[] PROGMEM = "ADDH";
const char TEXT_ADDS[] PROGMEM = "ADDS";
// const char TEXT_AND[]  PROGMEM = "AND";
const char TEXT_APAC[] PROGMEM = "APAC";
// const char TEXT_B[]    PROGMEM = "B";
const char TEXT_BANZ[] PROGMEM = "BANZ";
const char TEXT_BGEZ[] PROGMEM = "BGEZ";
const char TEXT_BGZ[]  PROGMEM = "BGZ";
const char TEXT_BIOZ[] PROGMEM = "BIOZ";
const char TEXT_BLEZ[] PROGMEM = "BLEZ";
const char TEXT_BLZ[]  PROGMEM = "BLZ";
// const char TEXT_BNZ[]  PROGMEM = "BNZ";
const char TEXT_BV[]   PROGMEM = "BV";
// const char TEXT_BZ[]   PROGMEM = "BZ";
const char TEXT_CALA[] PROGMEM = "CALA";
// const char TEXT_CALL[] PROGMEM = "CALL";
// const char TEXT_DINT[] PROGMEM = "DINT";
const char TEXT_DMOV[] PROGMEM = "DMOV";
// const char TEXT_EINT[] PROGMEM = "EINT";
// const char TEXT_IN[]   PROGMEM = "IN";
const char TEXT_LAC[]  PROGMEM = "LAC";
const char TEXT_LACK[] PROGMEM = "LACK";
const char TEXT_LAR[]  PROGMEM = "LAR";
const char TEXT_LARK[] PROGMEM = "LARK";
const char TEXT_LARP[] PROGMEM = "LARP";
const char TEXT_LDP[]  PROGMEM = "LDP";
const char TEXT_LDPK[] PROGMEM = "LDPK";
// const char TEXT_LST[]  PROGMEM = "LST";
const char TEXT_LT[]   PROGMEM = "LT";
const char TEXT_LTA[]  PROGMEM = "LTA";
const char TEXT_LTD[]  PROGMEM = "LTD";
const char TEXT_MAR[]  PROGMEM = "MAR";
// const char TEXT_MPY[]  PROGMEM = "MPY";
const char TEXT_MPYK[] PROGMEM = "MPYK";
// const char TEXT_NOP[]  PROGMEM = "NOP";
// const char TEXT_OR[]   PROGMEM = "OR";
// const char TEXT_OUT[]  PROGMEM = "OUT";
const char TEXT_PAC[]  PROGMEM = "PAC";
// const char TEXT_POP[]  PROGMEM = "POP";
// const char TEXT_PUSH[] PROGMEM = "PUSH";
// const char TEXT_RET[]  PROGMEM = "RET";
const char TEXT_ROVM[] PROGMEM = "ROVM";
const char TEXT_SACH[] PROGMEM = "SACH";
const char TEXT_SACL[] PROGMEM = "SACL";
// const char TEXT_SAR[]  PROGMEM = "SAR";
const char TEXT_SOVM[] PROGMEM = "SOVM";
const char TEXT_SPAC[] PROGMEM = "SPAC";
const char TEXT_SST[]  PROGMEM = "SST";
// const char TEXT_SUB[]  PROGMEM = "SUB";
// const char TEXT_SUBC[] PROGMEM = "SUBC";
const char TEXT_SUBH[] PROGMEM = "SUBH";
const char TEXT_SUBS[] PROGMEM = "SUBS";
const char TEXT_TBLR[] PROGMEM = "TBLR";
const char TEXT_TBLW[] PROGMEM = "TBLW";
// const char TEXT_XOR[]  PROGMEM = "XOR";
const char TEXT_ZAC[]  PROGMEM = "ZAC";
const char TEXT_ZALH[] PROGMEM = "ZALH";
const char TEXT_ZALS[] PROGMEM = "ZALS";
// clang-format on

}  // namespace tms32010
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
