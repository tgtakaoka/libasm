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

constexpr char TEXT_I8080_LIST[] = "i8080, i8085, V30EMU";
constexpr char TEXT_CPU_8080[]   = "8080";
constexpr char TEXT_CPU_8085[]   = "8085";
constexpr char TEXT_CPU_V30EMU[] = "V30EMU";

// i8080
constexpr char TEXT_ACI[]  = "ACI";
// constexpr char TEXT_ADC[]  = "ADC";
// constexpr char TEXT_ADD[]  = "ADD";
// constexpr char TEXT_ADI[]  = "ADI";
constexpr char TEXT_ANA[]  = "ANA";
// constexpr char TEXT_ANI[]  = "ANI";
// constexpr char TEXT_CALL[] = "CALL";
// constexpr char TEXT_CC[]   = "CC";
// constexpr char TEXT_CM[]   = "CM";
// constexpr char TEXT_CMA[]  = "CMA";
// constexpr char TEXT_CMC[]  = "CMC";
// constexpr char TEXT_CMP[]  = "CMP";
constexpr char TEXT_CNC[]  = "CNC";
constexpr char TEXT_CNZ[]  = "CNZ";
// constexpr char TEXT_CP[]   = "CP";
// constexpr char TEXT_CPE[]  = "CPE";
// constexpr char TEXT_CPI[]  = "CPI";
constexpr char TEXT_CPO[]  = "CPO";
constexpr char TEXT_CZ[]   = "CZ";
// constexpr char TEXT_DAA[]  = "DAA";
// constexpr char TEXT_DAD[]  = "DAD";
constexpr char TEXT_DCR[]  = "DCR";
constexpr char TEXT_DCX[]  = "DCX";
// constexpr char TEXT_DI[]   = "DI";
// constexpr char TEXT_EI[]   = "EI";
// constexpr char TEXT_HLT[]  = "HLT";
// constexpr char TEXT_IN[]   = "IN";
constexpr char TEXT_INR[]  = "INR";
// constexpr char TEXT_INX[]  = "INX";
// constexpr char TEXT_JC[]   = "JC";
constexpr char TEXT_JM[]   = "JM";
// constexpr char TEXT_JMP[]  = "JMP";
// constexpr char TEXT_JNC[]  = "JNC";
// constexpr char TEXT_JNZ[]  = "JNZ";
// constexpr char TEXT_JP[]   = "JP";
// constexpr char TEXT_JPE[]  = "JPE";
// constexpr char TEXT_JPO[]  = "JPO";
// constexpr char TEXT_JZ[]   = "JZ";
// constexpr char TEXT_LDA[]  = "LDA";
constexpr char TEXT_LDAX[] = "LDAX";
constexpr char TEXT_LHLD[] = "LHLD";
constexpr char TEXT_LXI[]  = "LXI";
// constexpr char TEXT_MOV[]  = "MOV";
// constexpr char TEXT_MVI[]  = "MVI";
// constexpr char TEXT_NOP[]  = "NOP";
// constexpr char TEXT_ORA[]  = "ORA";
// constexpr char TEXT_ORI[]  = "ORI";
// constexpr char TEXT_OUT[]  = "OUT";
constexpr char TEXT_PCHL[] = "PCHL";
// constexpr char TEXT_POP[]  = "POP";
// constexpr char TEXT_PUSH[] = "PUSH";
// constexpr char TEXT_RAL[]  = "RAL";
// constexpr char TEXT_RAR[]  = "RAR";
// constexpr char TEXT_RC[]   = "RC";
// constexpr char TEXT_RET[]  = "RET";
// constexpr char TEXT_RLC[]  = "RLC";
constexpr char TEXT_RM[]   = "RM";
constexpr char TEXT_RNC[]  = "RNC";
constexpr char TEXT_RNZ[]  = "RNZ";
constexpr char TEXT_RP[]   = "RP";
constexpr char TEXT_RPE[]  = "RPE";
constexpr char TEXT_RPO[]  = "RPO";
// constexpr char TEXT_RRC[]  = "RRC";
// constexpr char TEXT_RST[]  = "RST";
constexpr char TEXT_RZ[]   = "RZ";
// constexpr char TEXT_SBB[]  = "SBB";
constexpr char TEXT_SBI[]  = "SBI";
// constexpr char TEXT_SHLD[] = "SHLD";
constexpr char TEXT_SPHL[] = "SPHL";
// constexpr char TEXT_STA[]  = "STA";
constexpr char TEXT_STAX[] = "STAX";
// constexpr char TEXT_STC[]  = "STC";
// constexpr char TEXT_SUB[]  = "SUB";
constexpr char TEXT_SUI[]  = "SUI";
// constexpr char TEXT_XCHG[] = "XCHG";
constexpr char TEXT_XRA[]  = "XRA";
// constexpr char TEXT_XRI[]  = "XRI";
constexpr char TEXT_XTHL[] = "XTHL";

// i8085
constexpr char TEXT_RIM[] = "RIM";
constexpr char TEXT_SIM[] = "SIM";

// V30EMU
// constexpr char TEXT_CALLN[] = "CALLN";
// constexpr char TEXT_RETEM[] = "RETEM";

// constexpr char TEXT_REG_A[]   = "A";
// constexpr char TEXT_REG_B[]   = "B";
// constexpr char TEXT_REG_C[]   = "C";
// constexpr char TEXT_REG_D[]   = "D";
// constexpr char TEXT_REG_E[]   = "E";
// constexpr char TEXT_REG_H[]   = "H";
// constexpr char TEXT_REG_L[]   = "L";
constexpr char TEXT_REG_M[] = "M";
// constexpr char TEXT_REG_PSW[] = "PSW";
// constexpr char TEXT_REG_SP[]  = "SP";

// Zilog syntax
// constexpr char TEXT_CC_C[]   = "C";
// constexpr char TEXT_CC_M[]   = "M";
// constexpr char TEXT_CC_NC[]  = "NC";
// constexpr char TEXT_CC_NZ[]  = "NZ";
// constexpr char TEXT_CC_P[]   = "P";
// constexpr char TEXT_CC_PE[]  = "PE";
// constexpr char TEXT_CC_PO[]  = "PO";
// constexpr char TEXT_CC_Z[]   = "Z";
// constexpr char TEXT_REG_AF[] = "AF";
// constexpr char TEXT_REG_BC[] = "BC";
// constexpr char TEXT_REG_DE[] = "DE";
// constexpr char TEXT_REG_HL[] = "HL";
constexpr char TEXT_REG_IM[] = "IM";

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
