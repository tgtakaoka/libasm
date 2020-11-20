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

#ifndef __TEXT_I8080__
#define __TEXT_I8080__

namespace libasm {
namespace i8080 {

// clang-format off
static const char TEXT_CPU_LIST[]  PROGMEM = "i8080, i8085";
static const char TEXT_CPU_I8080[] PROGMEM = "i8080";
static constexpr const char *TEXT_CPU_8080  = &TEXT_CPU_I8080[1];
static constexpr const char *TEXT_CPU_I8085 = &TEXT_CPU_LIST[7];
static constexpr const char *TEXT_CPU_8085  = &TEXT_CPU_LIST[8];

static const char TEXT_ACI[]  PROGMEM = "ACI";
static const char TEXT_ADC[]  PROGMEM = "ADC";
static const char TEXT_ADD[]  PROGMEM = "ADD";
static const char TEXT_ADI[]  PROGMEM = "ADI";
static const char TEXT_ANA[]  PROGMEM = "ANA";
static const char TEXT_ANI[]  PROGMEM = "ANI";
static const char TEXT_CALL[] PROGMEM = "CALL";
static const char TEXT_CC[]   PROGMEM = "CC";
static const char TEXT_CM[]   PROGMEM = "CM";
static const char TEXT_CMA[]  PROGMEM = "CMA";
static const char TEXT_CMC[]  PROGMEM = "CMC";
static const char TEXT_CMP[]  PROGMEM = "CMP";
static const char TEXT_CNC[]  PROGMEM = "CNC";
static const char TEXT_CNZ[]  PROGMEM = "CNZ";
static const char TEXT_CP[]   PROGMEM = "CP";
static const char TEXT_CPE[]  PROGMEM = "CPE";
static const char TEXT_CPI[]  PROGMEM = "CPI";
static const char TEXT_CPO[]  PROGMEM = "CPO";
static const char TEXT_CZ[]   PROGMEM = "CZ";
static const char TEXT_DAA[]  PROGMEM = "DAA";
static const char TEXT_DAD[]  PROGMEM = "DAD";
static const char TEXT_DCR[]  PROGMEM = "DCR";
static const char TEXT_DCX[]  PROGMEM = "DCX";
static const char TEXT_DI[]   PROGMEM = "DI";
static const char TEXT_EI[]   PROGMEM = "EI";
static const char TEXT_HLT[]  PROGMEM = "HLT";
static const char TEXT_IN[]   PROGMEM = "IN";
static const char TEXT_INR[]  PROGMEM = "INR";
static const char TEXT_INX[]  PROGMEM = "INX";
static const char TEXT_JC[]   PROGMEM = "JC";
static const char TEXT_JM[]   PROGMEM = "JM";
static const char TEXT_JMP[]  PROGMEM = "JMP";
static const char TEXT_JNC[]  PROGMEM = "JNC";
static const char TEXT_JNZ[]  PROGMEM = "JNZ";
static const char TEXT_JP[]   PROGMEM = "JP";
static const char TEXT_JPE[]  PROGMEM = "JPE";
static const char TEXT_JPO[]  PROGMEM = "JPO";
static const char TEXT_JZ[]   PROGMEM = "JZ";
static const char TEXT_LDA[]  PROGMEM = "LDA";
static const char TEXT_LDAX[] PROGMEM = "LDAX";
static const char TEXT_LHLD[] PROGMEM = "LHLD";
static const char TEXT_LXI[]  PROGMEM = "LXI";
static const char TEXT_MOV[]  PROGMEM = "MOV";
static const char TEXT_MVI[]  PROGMEM = "MVI";
static const char TEXT_NOP[]  PROGMEM = "NOP";
static const char TEXT_ORA[]  PROGMEM = "ORA";
static const char TEXT_ORI[]  PROGMEM = "ORI";
static const char TEXT_OUT[]  PROGMEM = "OUT";
static const char TEXT_PCHL[] PROGMEM = "PCHL";
static const char TEXT_POP[]  PROGMEM = "POP";
static const char TEXT_PUSH[] PROGMEM = "PUSH";
static const char TEXT_RAL[]  PROGMEM = "RAL";
static const char TEXT_RAR[]  PROGMEM = "RAR";
static const char TEXT_RC[]   PROGMEM = "RC";
static const char TEXT_RET[]  PROGMEM = "RET";
static const char TEXT_RLC[]  PROGMEM = "RLC";
static const char TEXT_RM[]   PROGMEM = "RM";
static const char TEXT_RNC[]  PROGMEM = "RNC";
static const char TEXT_RNZ[]  PROGMEM = "RNZ";
static const char TEXT_RP[]   PROGMEM = "RP";
static const char TEXT_RPE[]  PROGMEM = "RPE";
static const char TEXT_RPO[]  PROGMEM = "RPO";
static const char TEXT_RRC[]  PROGMEM = "RRC";
static const char TEXT_RST[]  PROGMEM = "RST";
static const char TEXT_RZ[]   PROGMEM = "RZ";
static const char TEXT_SBB[]  PROGMEM = "SBB";
static const char TEXT_SBI[]  PROGMEM = "SBI";
static const char TEXT_SHLD[] PROGMEM = "SHLD";
static const char TEXT_SPHL[] PROGMEM = "SPHL";
static const char TEXT_STA[]  PROGMEM = "STA";
static const char TEXT_STAX[] PROGMEM = "STAX";
static const char TEXT_STC[]  PROGMEM = "STC";
static const char TEXT_SUB[]  PROGMEM = "SUB";
static const char TEXT_SUI[]  PROGMEM = "SUI";
static const char TEXT_XCHG[] PROGMEM = "XCHG";
static const char TEXT_XRA[]  PROGMEM = "XRA";
static const char TEXT_XRI[]  PROGMEM = "XRI";
static const char TEXT_XTHL[] PROGMEM = "XTHL";

// i8085
static const char TEXT_RIM[]  PROGMEM = "RIM";
static const char TEXT_SIM[]  PROGMEM = "SIM";
// clang-format on

}  // namespace i8080
}  // namespace libasm

#endif  // __TEXT_I8080__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
