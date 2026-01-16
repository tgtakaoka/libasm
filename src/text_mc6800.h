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

#ifndef __LIBASM_TEXT_MC6800__
#define __LIBASM_TEXT_MC6800__

#include "text_common.h"

namespace libasm {
namespace text {
namespace mc6800 {
// clang-format off

extern const char TEXT_MC6800_LIST[] PROGMEM;
extern const char TEXT_CPU_6800[]    PROGMEM;
extern const char TEXT_CPU_6801[]    PROGMEM;
extern const char TEXT_CPU_6301[]    PROGMEM;
extern const char TEXT_CPU_6811[]    PROGMEM;
extern const char TEXT_CPU_68HC11[]  PROGMEM;
extern const char TEXT_CPU_HD6301[]  PROGMEM;
extern const char TEXT_CPU_MB8861[]  PROGMEM;

// MC6800
using common::TEXT_ABA;
using common::TEXT_ADC;
using common::TEXT_ADCA;
using common::TEXT_ADCB;
using common::TEXT_ADD;
using common::TEXT_ADDA;
using common::TEXT_ADDB;
using common::TEXT_AND;
using common::TEXT_ANDA;
using common::TEXT_ANDB;
using common::TEXT_ASL;
using common::TEXT_ASLA;
using common::TEXT_ASLB;
using common::TEXT_ASR;
using common::TEXT_ASRA;
using common::TEXT_ASRB;
using common::TEXT_BCC;
using common::TEXT_BCS;
using common::TEXT_BEQ;
using common::TEXT_BGE;
using common::TEXT_BGT;
using common::TEXT_BHI;
using common::TEXT_BHS;
using common::TEXT_BIT;
using common::TEXT_BITA;
using common::TEXT_BITB;
using common::TEXT_BLE;
using common::TEXT_BLO;
using common::TEXT_BLS;
using common::TEXT_BLT;
using common::TEXT_BMI;
using common::TEXT_BNE;
using common::TEXT_BPL;
using common::TEXT_BRA;
using common::TEXT_BSR;
using common::TEXT_BVC;
using common::TEXT_BVS;
using common::TEXT_CBA;
using common::TEXT_CLC;
using common::TEXT_CLI;
using common::TEXT_CLR;
using common::TEXT_CLRA;
using common::TEXT_CLRB;
using common::TEXT_CLV;
using common::TEXT_CMP;
using common::TEXT_CMPA;
using common::TEXT_CMPB;
using common::TEXT_COM;
using common::TEXT_COMA;
using common::TEXT_COMB;
using common::TEXT_CPX;
using common::TEXT_DAA;
using common::TEXT_DEC;
using common::TEXT_DECA;
using common::TEXT_DECB;
using common::TEXT_DES;
using common::TEXT_DEX;
using common::TEXT_EOR;
using common::TEXT_EORA;
using common::TEXT_EORB;
using common::TEXT_INC;
using common::TEXT_INCA;
using common::TEXT_INCB;
using common::TEXT_INS;
using common::TEXT_INX;
using common::TEXT_JMP;
using common::TEXT_JSR;
using common::TEXT_LDA;
using common::TEXT_LDAA;
using common::TEXT_LDAB;
using common::TEXT_LDS;
using common::TEXT_LDX;
using common::TEXT_LSL;
using common::TEXT_LSLA;
using common::TEXT_LSLB;
using common::TEXT_LSR;
using common::TEXT_LSRA;
using common::TEXT_LSRB;
using common::TEXT_NEG;
using common::TEXT_NEGA;
using common::TEXT_NEGB;
using common::TEXT_NOP;
using common::TEXT_ORA;
using common::TEXT_ORAA;
using common::TEXT_ORAB;
extern const char TEXT_PSH[] PROGMEM;
using common::TEXT_PSHA;
using common::TEXT_PSHB;
extern const char TEXT_PUL[] PROGMEM;
using common::TEXT_PULA;
using common::TEXT_PULB;
using common::TEXT_ROL;
using common::TEXT_ROLA;
using common::TEXT_ROLB;
using common::TEXT_ROR;
using common::TEXT_RORA;
using common::TEXT_RORB;
using common::TEXT_RTI;
using common::TEXT_RTS;
using common::TEXT_SBA;
using common::TEXT_SBC;
using common::TEXT_SBCA;
using common::TEXT_SBCB;
using common::TEXT_SEC;
using common::TEXT_SEI;
using common::TEXT_SEV;
using common::TEXT_STA;
using common::TEXT_STAA;
using common::TEXT_STAB;
using common::TEXT_STS;
using common::TEXT_STX;
using common::TEXT_SUB;
using common::TEXT_SUBA;
using common::TEXT_SUBB;
using common::TEXT_SWI;
using common::TEXT_TAB;
using common::TEXT_TAP;
using common::TEXT_TBA;
using common::TEXT_TPA;
using common::TEXT_TST;
using common::TEXT_TSTA;
using common::TEXT_TSTB;
using common::TEXT_TSX;
using common::TEXT_TXS;
using common::TEXT_WAI;

// MB8861
using common::TEXT_ADX;
extern const char TEXT_NIM[] PROGMEM;
extern const char TEXT_TMM[] PROGMEM;
extern const char TEXT_XIM[] PROGMEM;

// MC6801
using common::TEXT_ABX;
using common::TEXT_ADDD;
using common::TEXT_ASLD;
using common::TEXT_BRN;
using common::TEXT_LDD;
using common::TEXT_LSLD;
using common::TEXT_LSRD;
using common::TEXT_MUL;
using common::TEXT_PSHX;
using common::TEXT_PULX;
using common::TEXT_STD;
using common::TEXT_SUBD;

// HD6301
using common::TEXT_AIM;
using common::TEXT_BCLR;
using common::TEXT_BSET;
extern const char TEXT_BTGL[] PROGMEM;
using common::TEXT_BTST;
using common::TEXT_EIM;
using common::TEXT_OIM;
using common::TEXT_SLP;
using common::TEXT_TIM;
using common::TEXT_XGDX;

// MC68HC11
using common::TEXT_ABY;
using common::TEXT_BRCLR;
using common::TEXT_BRSET;
using common::TEXT_CPD;
using common::TEXT_CPY;
using common::TEXT_DEY;
using common::TEXT_FDIV;
using common::TEXT_IDIV;
using common::TEXT_INY;
using common::TEXT_LDY;
using common::TEXT_PSHY;
using common::TEXT_PULY;
using common::TEXT_STOP;
using common::TEXT_STY;
using common::TEXT_TSY;
using common::TEXT_TYS;
using common::TEXT_XGDY;

using common::TEXT_REG_X;
using common::TEXT_REG_Y;

// clang-format on
}  // namespace mc6800
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_MC6800__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
