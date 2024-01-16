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

#ifndef __LIBASM_TEXT_I8080__
#define __LIBASM_TEXT_I8080__

#include "text_common.h"

namespace libasm {
namespace text {
namespace i8080 {

// clang-format off
extern const char TEXT_I8080_LIST[] PROGMEM;
using common::TEXT_CPU_8080;
using common::TEXT_CPU_8085;
using common::TEXT_CPU_V30EMU;

extern const char TEXT_ACI[]  PROGMEM;
using common::TEXT_ADC;
using common::TEXT_ADD;
using common::TEXT_ADI;
extern const char TEXT_ANA[]  PROGMEM;
using common::TEXT_ANI;
using common::TEXT_CALL;
extern const char TEXT_CC[]   PROGMEM;
using common::TEXT_CM;
using common::TEXT_CMA;
using common::TEXT_CMC;
using common::TEXT_CMP;
extern const char TEXT_CNC[]  PROGMEM;
extern const char TEXT_CNZ[]  PROGMEM;
using common::TEXT_CP;
extern const char TEXT_CPE[]  PROGMEM;
using common::TEXT_CPI;
extern const char TEXT_CPO[]  PROGMEM;
extern const char TEXT_CZ[]   PROGMEM;
using common::TEXT_DAA;
using common::TEXT_DAD;
extern const char TEXT_DCR[]  PROGMEM;
extern const char TEXT_DCX[]  PROGMEM;
using common::TEXT_DI;
using common::TEXT_EI;
using common::TEXT_HLT;
using common::TEXT_IN;
extern const char TEXT_INR[]  PROGMEM;
using common::TEXT_INX;
using common::TEXT_JC;
extern const char TEXT_JM[]   PROGMEM;
using common::TEXT_JMP;
using common::TEXT_JNC;
using common::TEXT_JNZ;
using common::TEXT_JP;
using common::TEXT_JPE;
using common::TEXT_JPO;
using common::TEXT_JZ;
using common::TEXT_LDA;
extern const char TEXT_LDAX[] PROGMEM;
extern const char TEXT_LHLD[] PROGMEM;
extern const char TEXT_LXI[]  PROGMEM;
using common::TEXT_MOV;
using common::TEXT_MVI;
using common::TEXT_NOP;
using common::TEXT_ORA;
using common::TEXT_ORI;
using common::TEXT_OUT;
extern const char TEXT_PCHL[] PROGMEM;
using common::TEXT_POP;
using common::TEXT_PUSH;
using common::TEXT_RAL;
using common::TEXT_RAR;
extern const char TEXT_RC[]   PROGMEM;
using common::TEXT_RET;
using common::TEXT_RLC;
extern const char TEXT_RM[]   PROGMEM;
extern const char TEXT_RNC[]  PROGMEM;
extern const char TEXT_RNZ[]  PROGMEM;
extern const char TEXT_RP[]   PROGMEM;
extern const char TEXT_RPE[]  PROGMEM;
extern const char TEXT_RPO[]  PROGMEM;
using common::TEXT_RRC;
using common::TEXT_RST;
extern const char TEXT_RZ[]   PROGMEM;
using common::TEXT_SBB;
extern const char TEXT_SBI[]  PROGMEM;
extern const char TEXT_SHLD[] PROGMEM;
extern const char TEXT_SPHL[] PROGMEM;
using common::TEXT_STA;
extern const char TEXT_STAX[] PROGMEM;
using common::TEXT_STC;
using common::TEXT_SUB;
extern const char TEXT_SUI[]  PROGMEM;
using common::TEXT_XCHG;
extern const char TEXT_XRA[]  PROGMEM;
using common::TEXT_XRI;
extern const char TEXT_XTHL[] PROGMEM;

// i8085
extern const char TEXT_RIM[]  PROGMEM;
extern const char TEXT_SIM[]  PROGMEM;

// V30EMU
using common::TEXT_CALLN;
using common::TEXT_RETEM;

using common::TEXT_REG_A;
using common::TEXT_REG_B;
using common::TEXT_REG_C;
using common::TEXT_REG_D;
using common::TEXT_REG_E;
using common::TEXT_REG_H;
using common::TEXT_REG_L;
extern const char TEXT_REG_M[]   PROGMEM;
using common::TEXT_REG_SP;
using common::TEXT_REG_PSW;
// clang-format on

}  // namespace i8080
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_I8080__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
