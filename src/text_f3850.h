/*
 * Copyright 2023 Tadashi G. Takaoka
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

#ifndef __LIBASM_TEXT_F3850__
#define __LIBASM_TEXT_F3850__

#include "text_common.h"

namespace libasm {
namespace text {
namespace f3850 {
// clang-format off

extern const char TEXT_CPU_F3850[] PROGMEM;
extern const char TEXT_CPU_3850[]  PROGMEM;

using common::TEXT_null;

// F3850
using common::TEXT_ADC;
using common::TEXT_AI;
using common::TEXT_AM;
extern const char TEXT_AMD[]  PROGMEM;
extern const char TEXT_AS[]   PROGMEM;
extern const char TEXT_ASD[]  PROGMEM;
using common::TEXT_BC;
extern const char TEXT_BF[]   PROGMEM;
using common::TEXT_BM;
using common::TEXT_BNC;
extern const char TEXT_BNO[]  PROGMEM;
using common::TEXT_BNZ;
using common::TEXT_BP;
using common::TEXT_BR;
extern const char TEXT_BR7[]  PROGMEM;
using common::TEXT_BT;
using common::TEXT_BZ;
using common::TEXT_CI;
using common::TEXT_CLR;
using common::TEXT_CM;
using common::TEXT_COM;
extern const char TEXT_DCI[]  PROGMEM;
using common::TEXT_DI;
using common::TEXT_DS;
using common::TEXT_EI;
using common::TEXT_IN;
using common::TEXT_INC;
using common::TEXT_INS;
using common::TEXT_JMP;
using common::TEXT_LI;
extern const char TEXT_LIS[]  PROGMEM;
extern const char TEXT_LISL[] PROGMEM;
extern const char TEXT_LISU[] PROGMEM;
extern const char TEXT_LM[]   PROGMEM;
extern const char TEXT_LNK[]  PROGMEM;
using common::TEXT_LR;
extern const char TEXT_NI[]   PROGMEM;
extern const char TEXT_NM[]   PROGMEM;
using common::TEXT_NOP;
extern const char TEXT_NS[]   PROGMEM;
extern const char TEXT_OI[]   PROGMEM;
extern const char TEXT_OM[]   PROGMEM;
using common::TEXT_OUT;
using common::TEXT_OUTS;
extern const char TEXT_PI[]   PROGMEM;
extern const char TEXT_PK[]   PROGMEM;
using common::TEXT_POP;
using common::TEXT_SL;
using common::TEXT_SR;
using common::TEXT_ST;
extern const char TEXT_XDC[]  PROGMEM;
extern const char TEXT_XI[]   PROGMEM;
extern const char TEXT_XM[]   PROGMEM;
extern const char TEXT_XS[]   PROGMEM;

using common::TEXT_REG_A;
using common::TEXT_REG_D;
extern const char TEXT_REG_DC[] PROGMEM;
using common::TEXT_REG_H;
using common::TEXT_REG_HL;
extern const char TEXT_REG_HU[] PROGMEM;
using common::TEXT_REG_I;
extern const char TEXT_REG_IS[] PROGMEM;
extern const char TEXT_REG_J[]  PROGMEM;
extern const char TEXT_REG_KL[] PROGMEM;
using common::TEXT_REG_K;
extern const char TEXT_REG_KU[] PROGMEM;
using common::TEXT_REG_P;
using common::TEXT_REG_P0;
extern const char TEXT_REG_QL[] PROGMEM;
extern const char TEXT_REG_Q[]  PROGMEM;
extern const char TEXT_REG_QU[] PROGMEM;
using common::TEXT_REG_S;
using common::TEXT_REG_W;

// clang-format on
}  // namespace f3850
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_F3850__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
