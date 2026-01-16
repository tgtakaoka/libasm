/*
 * Copyright 2025 Tadashi G. Takaoka
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

#ifndef __LIBASM_TEXT_MC68HC12__
#define __LIBASM_TEXT_MC68HC12__

#include "text_common.h"

namespace libasm {
namespace text {
namespace mc68hc12 {
// clang-format off

extern const char TEXT_CPU_MC68HC12[] PROGMEM;
extern const char TEXT_CPU_68HC12[]   PROGMEM;

// MC68HC12
using common::TEXT_ABA;
using common::TEXT_ABX;
using common::TEXT_ABY;
using common::TEXT_ADCA;
using common::TEXT_ADCB;
using common::TEXT_ADDA;
using common::TEXT_ADDB;
using common::TEXT_ADDD;
using common::TEXT_ANDA;
using common::TEXT_ANDB;
using common::TEXT_ANDCC;
using common::TEXT_ASL;
using common::TEXT_ASLA;
using common::TEXT_ASLB;
using common::TEXT_ASLD;
using common::TEXT_ASR;
using common::TEXT_ASRA;
using common::TEXT_ASRB;
using common::TEXT_BCC;
using common::TEXT_BCLR;
using common::TEXT_BCS;
using common::TEXT_BEQ;
using common::TEXT_BGE;
using common::TEXT_BGND;
using common::TEXT_BGT;
using common::TEXT_BHI;
using common::TEXT_BHS;
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
using common::TEXT_BRCLR;
using common::TEXT_BRN;
using common::TEXT_BRSET;
using common::TEXT_BSET;
using common::TEXT_BSR;
using common::TEXT_BVC;
using common::TEXT_BVS;
using common::TEXT_CALL;
using common::TEXT_CBA;
using common::TEXT_CLC;
using common::TEXT_CLI;
using common::TEXT_CLR;
using common::TEXT_CLRA;
using common::TEXT_CLRB;
using common::TEXT_CLV;
using common::TEXT_CMPA;
using common::TEXT_CMPB;
using common::TEXT_COM;
using common::TEXT_COMA;
using common::TEXT_COMB;
using common::TEXT_CPD;
using common::TEXT_CPS;
using common::TEXT_CPX;
using common::TEXT_CPY;
using common::TEXT_DAA;
using common::TEXT_DBEQ;
using common::TEXT_DBNE;
using common::TEXT_DEC;
using common::TEXT_DECA;
using common::TEXT_DECB;
using common::TEXT_DES;
using common::TEXT_DEX;
using common::TEXT_DEY;
using common::TEXT_EDIV;
using common::TEXT_EDIVS;
extern const char TEXT_EMACS[] PROGMEM;
extern const char TEXT_EMAXD[] PROGMEM;
extern const char TEXT_EMAXM[] PROGMEM;
extern const char TEXT_EMIND[] PROGMEM;
extern const char TEXT_EMINM[] PROGMEM;
using common::TEXT_EMUL;
using common::TEXT_EMULS;
using common::TEXT_EORA;
using common::TEXT_EORB;
extern const char TEXT_ETBL[]  PROGMEM;
using common::TEXT_EXG;
using common::TEXT_FDIV;
extern const char TEXT_IBEQ[]  PROGMEM;
extern const char TEXT_IBNE[]  PROGMEM;
using common::TEXT_IDIV;
extern const char TEXT_IDIVS[] PROGMEM;
using common::TEXT_INC;
using common::TEXT_INCA;
using common::TEXT_INCB;
using common::TEXT_INS;
using common::TEXT_INX;
using common::TEXT_INY;
using common::TEXT_JMP;
using common::TEXT_JSR;
using common::TEXT_LBCC;
using common::TEXT_LBCS;
using common::TEXT_LBEQ;
using common::TEXT_LBGE;
using common::TEXT_LBGT;
using common::TEXT_LBHI;
using common::TEXT_LBHS;
using common::TEXT_LBLE;
using common::TEXT_LBLO;
using common::TEXT_LBLS;
using common::TEXT_LBLT;
using common::TEXT_LBMI;
using common::TEXT_LBNE;
using common::TEXT_LBPL;
using common::TEXT_LBRA;
using common::TEXT_LBRN;
using common::TEXT_LBVC;
using common::TEXT_LBVS;
using common::TEXT_LDAA;
using common::TEXT_LDAB;
using common::TEXT_LDD;
using common::TEXT_LDS;
using common::TEXT_LDX;
using common::TEXT_LDY;
using common::TEXT_LEAS;
using common::TEXT_LEAX;
using common::TEXT_LEAY;
using common::TEXT_LSL;
using common::TEXT_LSLA;
using common::TEXT_LSLB;
using common::TEXT_LSLD;
using common::TEXT_LSR;
using common::TEXT_LSRA;
using common::TEXT_LSRB;
using common::TEXT_LSRD;
extern const char TEXT_MAXA[]  PROGMEM;
extern const char TEXT_MAXM[]  PROGMEM;
extern const char TEXT_MEM[]   PROGMEM;
extern const char TEXT_MINA[]  PROGMEM;
extern const char TEXT_MINM[]  PROGMEM;
using common::TEXT_MOVB;
using common::TEXT_MOVW;
using common::TEXT_MUL;
using common::TEXT_NEG;
using common::TEXT_NEGA;
using common::TEXT_NEGB;
using common::TEXT_NOP;
using common::TEXT_ORAA;
using common::TEXT_ORAB;
using common::TEXT_ORCC;
using common::TEXT_PSHA;
using common::TEXT_PSHB;
extern const char TEXT_PSHC[]  PROGMEM;
using common::TEXT_PSHD;
using common::TEXT_PSHX;
using common::TEXT_PSHY;
using common::TEXT_PULA;
using common::TEXT_PULB;
extern const char TEXT_PULC[]  PROGMEM;
using common::TEXT_PULD;
using common::TEXT_PULX;
using common::TEXT_PULY;
extern const char TEXT_REV[]   PROGMEM;
extern const char TEXT_REVW[]  PROGMEM;
using common::TEXT_ROL;
using common::TEXT_ROLA;
using common::TEXT_ROLB;
using common::TEXT_ROR;
using common::TEXT_RORA;
using common::TEXT_RORB;
using common::TEXT_RTC;
using common::TEXT_RTI;
using common::TEXT_RTS;
using common::TEXT_SBA;
using common::TEXT_SBCA;
using common::TEXT_SBCB;
using common::TEXT_SEC;
using common::TEXT_SEI;
using common::TEXT_SEV;
using common::TEXT_SEX;
using common::TEXT_STAA;
using common::TEXT_STAB;
using common::TEXT_STD;
using common::TEXT_STOP;
using common::TEXT_STS;
using common::TEXT_STX;
using common::TEXT_STY;
using common::TEXT_SUBA;
using common::TEXT_SUBB;
using common::TEXT_SUBD;
using common::TEXT_SWI;
using common::TEXT_TAB;
using common::TEXT_TAP;
using common::TEXT_TBA;
extern const char TEXT_TBEQ[];
extern const char TEXT_TBL[];
extern const char TEXT_TBNE[];
using common::TEXT_TFR;
using common::TEXT_TPA;
using common::TEXT_TRAP;
using common::TEXT_TST;
using common::TEXT_TSTA;
using common::TEXT_TSTB;
using common::TEXT_TSX;
using common::TEXT_TSY;
using common::TEXT_TXS;
using common::TEXT_TYS;
using common::TEXT_WAI;
extern const char TEXT_WAV[]   PROGMEM;
using common::TEXT_XGDX;
using common::TEXT_XGDY;

using common::TEXT_REG_A;
using common::TEXT_REG_B;
using common::TEXT_REG_CCR;
using common::TEXT_REG_D;
using common::TEXT_REG_PC;
using common::TEXT_REG_SP;
using common::TEXT_REG_X;
using common::TEXT_REG_Y;

// clang-format on
}  // namespace mc68hc12
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_MC68HC12__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
