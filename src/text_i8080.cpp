/*
 * Copyright 2020 Tadashi G. Takaoka
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

#include "text_i8080.h"

namespace libasm {
namespace text {
namespace i8080 {

// clang-format off
constexpr char TEXT_I8080_LIST[] PROGMEM = "i8080, i8085, V30EMU";
constexpr char TEXT_CPU_8080[]   PROGMEM = "8080";
constexpr char TEXT_CPU_8085[]   PROGMEM = "8085";
constexpr char TEXT_CPU_V30EMU[] PROGMEM = "V30EMU";

constexpr char TEXT_ACI[]  PROGMEM = "ACI";
// constexpr char TEXT_ADC[]  PROGMEM = "ADC";
// constexpr char TEXT_ADD[]  PROGMEM = "ADD";
// constexpr char TEXT_ADI[]  PROGMEM = "ADI";
constexpr char TEXT_ANA[]  PROGMEM = "ANA";
// constexpr char TEXT_ANI[]  PROGMEM = "ANI";
// constexpr char TEXT_CALL[] PROGMEM = "CALL";
constexpr char TEXT_CC[]   PROGMEM = "CC";
// constexpr char TEXT_CM[]   PROGMEM = "CM";
// constexpr char TEXT_CMA[]  PROGMEM = "CMA";
// constexpr char TEXT_CMC[]  PROGMEM = "CMC";
// constexpr char TEXT_CMP[]  PROGMEM = "CMP";
constexpr char TEXT_CNC[]  PROGMEM = "CNC";
constexpr char TEXT_CNZ[]  PROGMEM = "CNZ";
// constexpr char TEXT_CP[]   PROGMEM = "CP";
// constexpr char TEXT_CPE[]  PROGMEM = "CPE";
// constexpr char TEXT_CPI[]  PROGMEM = "CPI";
constexpr char TEXT_CPO[]  PROGMEM = "CPO";
constexpr char TEXT_CZ[]   PROGMEM = "CZ";
// constexpr char TEXT_DAA[]  PROGMEM = "DAA";
// constexpr char TEXT_DAD[]  PROGMEM = "DAD";
constexpr char TEXT_DCR[]  PROGMEM = "DCR";
constexpr char TEXT_DCX[]  PROGMEM = "DCX";
// constexpr char TEXT_DI[]   PROGMEM = "DI";
// constexpr char TEXT_EI[]   PROGMEM = "EI";
// constexpr char TEXT_HLT[]  PROGMEM = "HLT";
// constexpr char TEXT_IN[]   PROGMEM = "IN";
constexpr char TEXT_INR[]  PROGMEM = "INR";
// constexpr char TEXT_INX[]  PROGMEM = "INX";
// constexpr char TEXT_JC[]   PROGMEM = "JC";
constexpr char TEXT_JM[]   PROGMEM = "JM";
// constexpr char TEXT_JMP[]  PROGMEM = "JMP";
// constexpr char TEXT_JNC[]  PROGMEM = "JNC";
// constexpr char TEXT_JNZ[]  PROGMEM = "JNZ";
// constexpr char TEXT_JP[]   PROGMEM = "JP";
// constexpr char TEXT_JPE[]  PROGMEM = "JPE";
// constexpr char TEXT_JPO[]  PROGMEM = "JPO";
// constexpr char TEXT_JZ[]   PROGMEM = "JZ";
// constexpr char TEXT_LDA[]  PROGMEM = "LDA";
constexpr char TEXT_LDAX[] PROGMEM = "LDAX";
constexpr char TEXT_LHLD[] PROGMEM = "LHLD";
constexpr char TEXT_LXI[]  PROGMEM = "LXI";
// constexpr char TEXT_MOV[]  PROGMEM = "MOV";
// constexpr char TEXT_MVI[]  PROGMEM = "MVI";
// constexpr char TEXT_NOP[]  PROGMEM = "NOP";
// constexpr char TEXT_ORA[]  PROGMEM = "ORA";
// constexpr char TEXT_ORI[]  PROGMEM = "ORI";
// constexpr char TEXT_OUT[]  PROGMEM = "OUT";
constexpr char TEXT_PCHL[] PROGMEM = "PCHL";
// constexpr char TEXT_POP[]  PROGMEM = "POP";
// constexpr char TEXT_PUSH[] PROGMEM = "PUSH";
// constexpr char TEXT_RAL[]  PROGMEM = "RAL";
// constexpr char TEXT_RAR[]  PROGMEM = "RAR";
// constexpr char TEXT_RC[]   PROGMEM = "RC";
// constexpr char TEXT_RET[]  PROGMEM = "RET";
// constexpr char TEXT_RLC[]  PROGMEM = "RLC";
constexpr char TEXT_RM[]   PROGMEM = "RM";
constexpr char TEXT_RNC[]  PROGMEM = "RNC";
constexpr char TEXT_RNZ[]  PROGMEM = "RNZ";
constexpr char TEXT_RP[]   PROGMEM = "RP";
constexpr char TEXT_RPE[]  PROGMEM = "RPE";
constexpr char TEXT_RPO[]  PROGMEM = "RPO";
// constexpr char TEXT_RRC[]  PROGMEM = "RRC";
// constexpr char TEXT_RST[]  PROGMEM = "RST";
constexpr char TEXT_RZ[]   PROGMEM = "RZ";
// constexpr char TEXT_SBB[]  PROGMEM = "SBB";
constexpr char TEXT_SBI[]  PROGMEM = "SBI";
constexpr char TEXT_SHLD[] PROGMEM = "SHLD";
constexpr char TEXT_SPHL[] PROGMEM = "SPHL";
// constexpr char TEXT_STA[]  PROGMEM = "STA";
constexpr char TEXT_STAX[] PROGMEM = "STAX";
// constexpr char TEXT_STC[]  PROGMEM = "STC";
// constexpr char TEXT_SUB[]  PROGMEM = "SUB";
constexpr char TEXT_SUI[]  PROGMEM = "SUI";
// constexpr char TEXT_XCHG[] PROGMEM = "XCHG";
constexpr char TEXT_XRA[]  PROGMEM = "XRA";
// constexpr char TEXT_XRI[]  PROGMEM = "XRI";
constexpr char TEXT_XTHL[] PROGMEM = "XTHL";

// i8085
constexpr char TEXT_RIM[]  PROGMEM = "RIM";
constexpr char TEXT_SIM[]  PROGMEM = "SIM";

// V30EMU
// constexpr char TEXT_CALLN[] PROGMEM = "CALLN";
// constexpr char TEXT_RETEM[] PROGMEM = "RETEM";

// constexpr char TEXT_REG_A[]   PROGMEM  = "A";
// constexpr char TEXT_REG_B[]   PROGMEM  = "B";
// constexpr char TEXT_REG_C[]   PROGMEM  = "C";
// constexpr char TEXT_REG_D[]   PROGMEM  = "D";
// constexpr char TEXT_REG_E[]   PROGMEM  = "E";
// constexpr char TEXT_REG_H[]   PROGMEM  = "H";
// constexpr char TEXT_REG_L[]   PROGMEM  = "L";
constexpr char TEXT_REG_M[]   PROGMEM  = "M";
// constexpr char TEXT_REG_SP[]  PROGMEM = "SP";
// constexpr char TEXT_REG_PSW[] PROGMEM = "PSW";

// Zilog syntax
// constexpr char TEXT_REG_AF[]  PROGMEM = "AF";
// constexpr char TEXT_REG_BC[]  PROGMEM = "BC";
// constexpr char TEXT_REG_DE[]  PROGMEM = "DE";
// constexpr char TEXT_REG_HL[]  PROGMEM = "HL";
constexpr char TEXT_REG_IM[] PROGMEM = "IM";
// constexpr char TEXT_CC_NZ[] PROGMEM = "NZ";
// constexpr char TEXT_CC_Z[]  PROGMEM = "Z";
// constexpr char TEXT_CC_NC[] PROGMEM = "NC";
// constexpr char TEXT_CC_C[]  PROGMEM = "C";
// constexpr char TEXT_CC_PO[] PROGMEM = "PO";
// constexpr char TEXT_CC_PE[] PROGMEM = "PE";
// constexpr char TEXT_CC_P[]  PROGMEM = "P";
// constexpr char TEXT_CC_M[]  PROGMEM = "M";

// clang-format on
}  // namespace i8080
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
