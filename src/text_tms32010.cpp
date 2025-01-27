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
constexpr char TEXT_TMS32010_LIST[] PROGMEM = "TMS32010, TMS32015";
constexpr char TEXT_CPU_32010[] PROGMEM = "32010";
constexpr char TEXT_CPU_32015[] PROGMEM = "32015";

// TMS32010
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
constexpr char TEXT_LAR[]  PROGMEM = "LAR";
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

constexpr char TEXT_REG_AR[] PROGMEM = "AR";
constexpr char TEXT_REG_PA[] PROGMEM = "PA";
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
