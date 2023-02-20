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

#ifndef __TEXT_F3850__
#define __TEXT_F3850__

#include "config_host.h"

namespace libasm {
namespace f3850 {

// clang-format off
static const char TEXT_CPU_LIST[] PROGMEM = "F3850";
static constexpr const char *TEXT_CPU_F3850 = TEXT_CPU_LIST;
static constexpr const char *TEXT_CPU_3850 = TEXT_CPU_LIST + 1;
static constexpr const char *TEXT_null = TEXT_CPU_3850 + 4;

// F3850
static const char TEXT_ADC[]  PROGMEM = "ADC";
static const char TEXT_AI[]   PROGMEM = "AI";
static const char TEXT_AM[]   PROGMEM = "AM";
static const char TEXT_AMD[]  PROGMEM = "AMD";
static const char TEXT_AS[]   PROGMEM = "AS";
static const char TEXT_ASD[]  PROGMEM = "ASD";
static const char TEXT_BC[]   PROGMEM = "BC";
static const char TEXT_BF[]   PROGMEM = "BF";
static const char TEXT_BM[]   PROGMEM = "BM";
static const char TEXT_BNC[]  PROGMEM = "BNC";
static const char TEXT_BNO[]  PROGMEM = "BNO";
static const char TEXT_BNZ[]  PROGMEM = "BNZ";
static const char TEXT_BP[]   PROGMEM = "BP";
static const char TEXT_BR[]   PROGMEM = "BR";
static const char TEXT_BR7[]  PROGMEM = "BR7";
static const char TEXT_BT[]   PROGMEM = "BT";
static const char TEXT_BZ[]   PROGMEM = "BZ";
static const char TEXT_CI[]   PROGMEM = "CI";
static const char TEXT_CLR[]  PROGMEM = "CLR";
static const char TEXT_CM[]   PROGMEM = "CM";
static const char TEXT_COM[]  PROGMEM = "COM";
static const char TEXT_DCI[]  PROGMEM = "DCI";
static const char TEXT_DI[]   PROGMEM = "DI";
static const char TEXT_DS[]   PROGMEM = "DS";
static const char TEXT_EI[]   PROGMEM = "EI";
static const char TEXT_IN[]   PROGMEM = "IN";
static const char TEXT_INC[]  PROGMEM = "INC";
static const char TEXT_INS[]  PROGMEM = "INS";
static const char TEXT_JMP[]  PROGMEM = "JMP";
static const char TEXT_LI[]   PROGMEM = "LI";
static const char TEXT_LIS[]  PROGMEM = "LIS";
static const char TEXT_LISL[] PROGMEM = "LISL";
static const char TEXT_LISU[] PROGMEM = "LISU";
static const char TEXT_LM[]   PROGMEM = "LM";
static const char TEXT_LNK[]  PROGMEM = "LNK";
static const char TEXT_LR[]   PROGMEM = "LR";
static const char TEXT_NI[]   PROGMEM = "NI";
static const char TEXT_NM[]   PROGMEM = "NM";
static const char TEXT_NOP[]  PROGMEM = "NOP";
static const char TEXT_NS[]   PROGMEM = "NS";
static const char TEXT_OI[]   PROGMEM = "OI";
static const char TEXT_OM[]   PROGMEM = "OM";
static const char TEXT_OUT[]  PROGMEM = "OUT";
static const char TEXT_OUTS[] PROGMEM = "OUTS";
static const char TEXT_PI[]   PROGMEM = "PI";
static const char TEXT_PK[]   PROGMEM = "PK";
static const char TEXT_POP[]  PROGMEM = "POP";
static const char TEXT_SL[]   PROGMEM = "SL";
static const char TEXT_SR[]   PROGMEM = "SR";
static const char TEXT_ST[]   PROGMEM = "ST";
static const char TEXT_XDC[]  PROGMEM = "XDC";
static const char TEXT_XI[]   PROGMEM = "XI";
static const char TEXT_XM[]   PROGMEM = "XM";
static const char TEXT_XS[]   PROGMEM = "XS";

// clang-format off

} // namespace f3850
} // namespace libasm

#endif // __TEXT_F3850__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
