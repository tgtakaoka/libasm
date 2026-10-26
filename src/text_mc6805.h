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

extern const char TEXT_MC6805_LIST[];
extern const char TEXT_CPU_6805[];
extern const char TEXT_CPU_146805[];
extern const char TEXT_CPU_68HC05[];
extern const char TEXT_CPU_68HC08[];
extern const char TEXT_CPU_68HCS08[];

using common::TEXT_ADC;
using common::TEXT_ADD;
using common::TEXT_AND;
using common::TEXT_ASL;
using common::TEXT_ASLA;
using common::TEXT_ASLX;
using common::TEXT_ASR;
using common::TEXT_ASRA;
using common::TEXT_ASRX;
using common::TEXT_BCC;
using common::TEXT_BCLR;
using common::TEXT_BCS;
using common::TEXT_BEQ;
extern const char TEXT_BHCC[];
extern const char TEXT_BHCS[];
using common::TEXT_BHI;
using common::TEXT_BHS;
extern const char TEXT_BIH[];
extern const char TEXT_BIL[];
using common::TEXT_BIT;
using common::TEXT_BLO;
using common::TEXT_BLS;
extern const char TEXT_BMC[];
using common::TEXT_BMI;
extern const char TEXT_BMS[];
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
using common::TEXT_CLRX;
using common::TEXT_CMP;
using common::TEXT_COM;
using common::TEXT_COMA;
using common::TEXT_COMX;
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
using common::TEXT_LSLX;
using common::TEXT_LSR;
using common::TEXT_LSRA;
using common::TEXT_LSRX;
using common::TEXT_MUL;
using common::TEXT_NEG;
using common::TEXT_NEGA;
using common::TEXT_NEGX;
using common::TEXT_NOP;
using common::TEXT_ORA;
using common::TEXT_ROL;
using common::TEXT_ROLA;
using common::TEXT_ROLX;
using common::TEXT_ROR;
using common::TEXT_RORA;
using common::TEXT_RORX;
extern const char TEXT_RSP[];
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
using common::TEXT_TSTX;
using common::TEXT_TXA;
using common::TEXT_WAIT;

// MC68HC08
using common::TEXT_AIS;
using common::TEXT_AIX;
using common::TEXT_BGE;
using common::TEXT_BGT;
using common::TEXT_BLE;
using common::TEXT_BLT;
extern const char TEXT_CBEQ[];
extern const char TEXT_CBEQA[];
extern const char TEXT_CBEQX[];
extern const char TEXT_CLRH[];
extern const char TEXT_CPHX[];
using common::TEXT_DAA;
using common::TEXT_DBNZ;
extern const char TEXT_DBNZA[];
extern const char TEXT_DBNZX[];
using common::TEXT_DIV;
extern const char TEXT_LDHX[];
using common::TEXT_MOV;
extern const char TEXT_NSA[];
using common::TEXT_PSHA;
extern const char TEXT_PSHH[];
using common::TEXT_PSHX;
using common::TEXT_PULA;
extern const char TEXT_PULH[];
using common::TEXT_PULX;
extern const char TEXT_STHX[];
using common::TEXT_TAP;
using common::TEXT_TPA;
using common::TEXT_TSX;
using common::TEXT_TXS;

// MC68HCS08
using common::TEXT_BGND;
    
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
