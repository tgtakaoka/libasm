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
const char TEXT_CPU_LIST[]   PROGMEM = "i8080, i8085, V30EMU";
// const char TEXT_CPU_8080[]   PROGMEM = "8080";
// const char TEXT_CPU_8085[]   PROGMEM = "8085";
// const char TEXT_CPU_V30EMU[] PROGMEM = "V30EMU";

const char TEXT_ACI[]  PROGMEM = "ACI";
// const char TEXT_ADC[]  PROGMEM = "ADC";
// const char TEXT_ADD[]  PROGMEM = "ADD";
// const char TEXT_ADI[]  PROGMEM = "ADI";
const char TEXT_ANA[]  PROGMEM = "ANA";
// const char TEXT_ANI[]  PROGMEM = "ANI";
// const char TEXT_CALL[] PROGMEM = "CALL";
const char TEXT_CC[]   PROGMEM = "CC";
// const char TEXT_CM[]   PROGMEM = "CM";
const char TEXT_CMA[]  PROGMEM = "CMA";
// const char TEXT_CMC[]  PROGMEM = "CMC";
// const char TEXT_CMP[]  PROGMEM = "CMP";
const char TEXT_CNC[]  PROGMEM = "CNC";
const char TEXT_CNZ[]  PROGMEM = "CNZ";
// const char TEXT_CP[]   PROGMEM = "CP";
const char TEXT_CPE[]  PROGMEM = "CPE";
// const char TEXT_CPI[]  PROGMEM = "CPI";
const char TEXT_CPO[]  PROGMEM = "CPO";
const char TEXT_CZ[]   PROGMEM = "CZ";
// const char TEXT_DAA[]  PROGMEM = "DAA";
// const char TEXT_DAD[]  PROGMEM = "DAD";
const char TEXT_DCR[]  PROGMEM = "DCR";
const char TEXT_DCX[]  PROGMEM = "DCX";
// const char TEXT_DI[]   PROGMEM = "DI";
// const char TEXT_EI[]   PROGMEM = "EI";
// const char TEXT_HLT[]  PROGMEM = "HLT";
// const char TEXT_IN[]   PROGMEM = "IN";
const char TEXT_INR[]  PROGMEM = "INR";
// const char TEXT_INX[]  PROGMEM = "INX";
// const char TEXT_JC[]   PROGMEM = "JC";
const char TEXT_JM[]   PROGMEM = "JM";
// const char TEXT_JMP[]  PROGMEM = "JMP";
// const char TEXT_JNC[]  PROGMEM = "JNC";
// const char TEXT_JNZ[]  PROGMEM = "JNZ";
// const char TEXT_JP[]   PROGMEM = "JP";
// const char TEXT_JPE[]  PROGMEM = "JPE";
// const char TEXT_JPO[]  PROGMEM = "JPO";
// const char TEXT_JZ[]   PROGMEM = "JZ";
// const char TEXT_LDA[]  PROGMEM = "LDA";
const char TEXT_LDAX[] PROGMEM = "LDAX";
const char TEXT_LHLD[] PROGMEM = "LHLD";
const char TEXT_LXI[]  PROGMEM = "LXI";
// const char TEXT_MOV[]  PROGMEM = "MOV";
// const char TEXT_MVI[]  PROGMEM = "MVI";
// const char TEXT_NOP[]  PROGMEM = "NOP";
// const char TEXT_ORA[]  PROGMEM = "ORA";
// const char TEXT_ORI[]  PROGMEM = "ORI";
// const char TEXT_OUT[]  PROGMEM = "OUT";
const char TEXT_PCHL[] PROGMEM = "PCHL";
// const char TEXT_POP[]  PROGMEM = "POP";
// const char TEXT_PUSH[] PROGMEM = "PUSH";
const char TEXT_RAL[]  PROGMEM = "RAL";
const char TEXT_RAR[]  PROGMEM = "RAR";
const char TEXT_RC[]   PROGMEM = "RC";
// const char TEXT_RET[]  PROGMEM = "RET";
// const char TEXT_RLC[]  PROGMEM = "RLC";
const char TEXT_RM[]   PROGMEM = "RM";
const char TEXT_RNC[]  PROGMEM = "RNC";
const char TEXT_RNZ[]  PROGMEM = "RNZ";
const char TEXT_RP[]   PROGMEM = "RP";
const char TEXT_RPE[]  PROGMEM = "RPE";
const char TEXT_RPO[]  PROGMEM = "RPO";
// const char TEXT_RRC[]  PROGMEM = "RRC";
// const char TEXT_RST[]  PROGMEM = "RST";
const char TEXT_RZ[]   PROGMEM = "RZ";
// const char TEXT_SBB[]  PROGMEM = "SBB";
const char TEXT_SBI[]  PROGMEM = "SBI";
const char TEXT_SHLD[] PROGMEM = "SHLD";
const char TEXT_SPHL[] PROGMEM = "SPHL";
// const char TEXT_STA[]  PROGMEM = "STA";
const char TEXT_STAX[] PROGMEM = "STAX";
// const char TEXT_STC[]  PROGMEM = "STC";
// const char TEXT_SUB[]  PROGMEM = "SUB";
const char TEXT_SUI[]  PROGMEM = "SUI";
// const char TEXT_XCHG[] PROGMEM = "XCHG";
const char TEXT_XRA[]  PROGMEM = "XRA";
// const char TEXT_XRI[]  PROGMEM = "XRI";
const char TEXT_XTHL[] PROGMEM = "XTHL";

// i8085
const char TEXT_RIM[]  PROGMEM = "RIM";
const char TEXT_SIM[]  PROGMEM = "SIM";

// V30EMU
// const char TEXT_CALLN[] PROGMEM = "CALLN";
// const char TEXT_RETEM[] PROGMEM = "RETEM";

// const char TEXT_REG_A[]   PROGMEM  = "A";
// const char TEXT_REG_B[]   PROGMEM  = "B";
// const char TEXT_REG_C[]   PROGMEM  = "C";
// const char TEXT_REG_D[]   PROGMEM  = "D";
// const char TEXT_REG_E[]   PROGMEM  = "E";
// const char TEXT_REG_H[]   PROGMEM  = "H";
// const char TEXT_REG_L[]   PROGMEM  = "L";
const char TEXT_REG_M[]   PROGMEM  = "M";
// const char TEXT_REG_SP[]  PROGMEM = "SP";
// const char TEXT_REG_PSW[] PROGMEM = "PSW";
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
