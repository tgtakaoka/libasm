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

#ifndef __TEXT_MC6805__
#define __TEXT_MC6805__

#include "config_host.h"

namespace libasm {
namespace mc6805 {

// clang-format off
static const char TEXT_CPU_LIST_6805[] PROGMEM = "MC6805, MC146805, MC68HC05";
static const char TEXT_CPU_6805[] PROGMEM = "6805";
static const char TEXT_CPU_146805[] PROGMEM = "146805";
static constexpr const char *TEXT_CPU_68HC05 = (TEXT_CPU_LIST_6805 + 20);
static constexpr const char *TEXT_CPU_6805_MC   = TEXT_CPU_LIST_6805;

static const char TEXT_ASRX[] PROGMEM = "ASRX";
static const char TEXT_ASLX[] PROGMEM = "ASLX";
static const char TEXT_BHCC[] PROGMEM = "BHCC";
static const char TEXT_BHCS[] PROGMEM = "BHCS";
static const char TEXT_BIH[]  PROGMEM = "BIH";
static const char TEXT_BIL[]  PROGMEM = "BIL";
static const char TEXT_BMC[]  PROGMEM = "BMC";
static const char TEXT_BMS[]  PROGMEM = "BMS";
static const char TEXT_CLRX[] PROGMEM = "CLRX";
static const char TEXT_COMX[] PROGMEM = "COMX";
static const char TEXT_DECX[] PROGMEM = "DECX";
static const char TEXT_INCX[] PROGMEM = "INCX";
static const char TEXT_LSLX[] PROGMEM = "LSLX";
static const char TEXT_LSRX[] PROGMEM = "LSRX";
static const char TEXT_NEGX[] PROGMEM = "NEGX";
static const char TEXT_ROLX[] PROGMEM = "ROLX";
static const char TEXT_RORX[] PROGMEM = "RORX";
static const char TEXT_RSP[]  PROGMEM = "RSP";
static const char TEXT_TAX[]  PROGMEM = "TAX";
static const char TEXT_TSTX[] PROGMEM = "TSTX";
static const char TEXT_TXA[]  PROGMEM = "TXA";
static const char TEXT_WAIT[] PROGMEM = "WAIT";
// clang-format on

}  // namespace mc6805
}  // namespace libasm

#endif  // __TEXT_MC6805__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
