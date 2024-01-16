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

#ifndef __LIBASM_TEXT_MC6805__
#define __LIBASM_TEXT_MC6805__

#include "text_common.h"

namespace libasm {
namespace text {
namespace mc6805 {

// clang-format off
extern const char TEXT_MC6805_LIST[] PROGMEM;
extern const char TEXT_CPU_6805[] PROGMEM;
extern const char TEXT_CPU_146805[] PROGMEM;
extern const char TEXT_CPU_68HC05[] PROGMEM;

using common::TEXT_ADC;
using common::TEXT_ADD;
using common::TEXT_AND;
using common::TEXT_ASR;
using common::TEXT_ASRA;
extern const char TEXT_ASRX[] PROGMEM;
using common::TEXT_ASL;
using common::TEXT_ASLA;
extern const char TEXT_ASLX[] PROGMEM;
using common::TEXT_BCC;
using common::TEXT_BCLR;
using common::TEXT_BCS;
using common::TEXT_BEQ;
extern const char TEXT_BHCC[] PROGMEM;
extern const char TEXT_BHCS[] PROGMEM;
using common::TEXT_BHI;
using common::TEXT_BHS;
extern const char TEXT_BIH[]  PROGMEM;
extern const char TEXT_BIL[]  PROGMEM;
using common::TEXT_BIT;
using common::TEXT_BLO;
using common::TEXT_BLS;
extern const char TEXT_BMC[]  PROGMEM;
using common::TEXT_BMI;
extern const char TEXT_BMS[]  PROGMEM;
using common::TEXT_BNE;
using common::TEXT_BPL;
using common::TEXT_BRA;
using common::TEXT_BRCLR;
using common::TEXT_BRN;
using common::TEXT_BRSET;
using common::TEXT_BSET;
using common::TEXT_BSR;
using common::TEXT_CLC;
using common::TEXT_CLI;
using common::TEXT_CLR;
using common::TEXT_CLRA;
extern const char TEXT_CLRX[] PROGMEM;
using common::TEXT_CMP;
using common::TEXT_COM;
using common::TEXT_COMA;
extern const char TEXT_COMX[] PROGMEM;
using common::TEXT_CPX;
using common::TEXT_DEC;
using common::TEXT_DECA;
using common::TEXT_DECX;
using common::TEXT_EOR;
using common::TEXT_INC;
using common::TEXT_INCA;
using common::TEXT_INCX;
using common::TEXT_JMP;
using common::TEXT_JSR;
using common::TEXT_LDA;
using common::TEXT_LDX;
using common::TEXT_LSL;
using common::TEXT_LSLA;
extern const char TEXT_LSLX[] PROGMEM;
using common::TEXT_LSR;
using common::TEXT_LSRA;
extern const char TEXT_LSRX[] PROGMEM;
using common::TEXT_MUL;
using common::TEXT_NEG;
using common::TEXT_NEGA;
using common::TEXT_NEGX;
using common::TEXT_NOP;
using common::TEXT_ORA;
using common::TEXT_ROL;
using common::TEXT_ROLA;
extern const char TEXT_ROLX[] PROGMEM;
using common::TEXT_ROR;
using common::TEXT_RORA;
extern const char TEXT_RORX[] PROGMEM;
extern const char TEXT_RSP[]  PROGMEM;
using common::TEXT_RTI;
using common::TEXT_RTS;
using common::TEXT_SBC;
using common::TEXT_SEC;
using common::TEXT_SEI;
using common::TEXT_STA;
using common::TEXT_STOP;
using common::TEXT_STX;
using common::TEXT_SUB;
using common::TEXT_SWI;
using common::TEXT_TAX;
using common::TEXT_TST;
using common::TEXT_TSTA;
extern const char TEXT_TSTX[] PROGMEM;
using common::TEXT_TXA;
using common::TEXT_WAIT;
// clang-format on

}  // namespace mc6805
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_MC6805__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
