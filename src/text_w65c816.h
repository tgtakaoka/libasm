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

#ifndef __TEXT_W65C816_H__
#define __TEXT_W65C816_H__

#include "config_host.h"

namespace libasm {
namespace w65c816 {

// MOS6502 instructions

static const char TEXT_ADC[] PROGMEM = "ADC";
static const char TEXT_AND[] PROGMEM = "AND";
static const char TEXT_CMP[] PROGMEM = "CMP";
static const char TEXT_EOR[] PROGMEM = "EOR";
static const char TEXT_JMP[] PROGMEM = "JMP";
static const char TEXT_JSR[] PROGMEM = "JSR";
static const char TEXT_LDA[] PROGMEM = "LDA";
static const char TEXT_ORA[] PROGMEM = "ORA";
static const char TEXT_SBC[] PROGMEM = "SBC";
static const char TEXT_STA[] PROGMEM = "STA";

// W65C02S instructions

static const char TEXT_STP[] PROGMEM  = "STP";
static const char TEXT_WAI[] PROGMEM  = "WAI";

// W65C816 instructions

static const char TEXT_BRL[] PROGMEM  = "BRL";
static const char TEXT_COP[] PROGMEM  = "COP";
static const char TEXT_JML[] PROGMEM  = "JML";
static const char TEXT_JSL[] PROGMEM  = "JSL";
static const char TEXT_MVN[] PROGMEM  = "MVN";
static const char TEXT_MVP[] PROGMEM  = "MVP";
static const char TEXT_PEA[] PROGMEM  = "PEA";
static const char TEXT_PEI[] PROGMEM  = "PEI";
static const char TEXT_PER[] PROGMEM  = "PER";
static const char TEXT_PHB[] PROGMEM  = "PHB";
static const char TEXT_PHD[] PROGMEM  = "PHD";
static const char TEXT_PHK[] PROGMEM  = "PHK";
static const char TEXT_PLB[] PROGMEM  = "PLB";
static const char TEXT_PLD[] PROGMEM  = "PLD";
static const char TEXT_PSD[] PROGMEM  = "PSD";
static const char TEXT_REP[] PROGMEM  = "REP";
static const char TEXT_RTL[] PROGMEM  = "RTL";
static const char TEXT_SEP[] PROGMEM  = "SEP";
static const char TEXT_TCD[] PROGMEM  = "TCD";
static const char TEXT_TCS[] PROGMEM  = "TCS";
static const char TEXT_TDC[] PROGMEM  = "TDC";
static const char TEXT_TSC[] PROGMEM  = "TSC";
static const char TEXT_TXY[] PROGMEM  = "TXY";
static const char TEXT_TYX[] PROGMEM  = "TYX";
static const char TEXT_WDM[] PROGMEM  = "WDM";
static const char TEXT_XBA[] PROGMEM  = "XBA";
static const char TEXT_XCE[] PROGMEM  = "XCE";

static const char TEXT_JMPL[] PROGMEM = "JMPL";
static const char TEXT_ADCL[] PROGMEM = "ADCL";
static const char TEXT_ANDL[] PROGMEM = "ANDL";
static const char TEXT_CMPL[] PROGMEM = "CMPL";
static const char TEXT_EORL[] PROGMEM = "EORL";
static const char TEXT_LDAL[] PROGMEM = "LDAL";
static const char TEXT_ORAL[] PROGMEM = "ORAL";
static const char TEXT_SBCL[] PROGMEM = "SBCL";
static const char TEXT_STAL[] PROGMEM = "STAL";

// Pseudo

static const char TEXT_LONGA[] PROGMEM  = "LONGA";
static const char TEXT_LONGI[] PROGMEM  = "LONGI";
static const char TEXT_ASSUME[] PROGMEM = "ASSUME";

} // namespace w65c816
} // namespace libasm

#endif // __TEXT_W65C816_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
