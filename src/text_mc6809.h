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

#ifndef __LIBASM_TEXT_MC6809_H__
#define __LIBASM_TEXT_MC6809_H__

#include "text_common.h"

namespace libasm {
namespace text {
namespace mc6809 {
// clang-format off

extern const char TEXT_MC6809_LIST[] PROGMEM;
extern const char TEXT_CPU_6809[]    PROGMEM;
extern const char TEXT_CPU_6309[]    PROGMEM;
extern const char TEXT_CPU_HD6309[]  PROGMEM;
extern const char TEXT_CPU_MC6809[]  PROGMEM;

using common::TEXT_ABX;
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
using common::TEXT_BRN;
using common::TEXT_BSR;
using common::TEXT_BVC;
using common::TEXT_BVS;
using common::TEXT_CLR;
using common::TEXT_CLRA;
using common::TEXT_CLRB;
using common::TEXT_CMPA;
using common::TEXT_CMPB;
using common::TEXT_CMPD;
using common::TEXT_CMPS;
extern const char TEXT_CMPU[] PROGMEM;
extern const char TEXT_CMPX[] PROGMEM;
extern const char TEXT_CMPY[] PROGMEM;
using common::TEXT_COM;
using common::TEXT_COMA;
using common::TEXT_COMB;
extern const char TEXT_CWAI[] PROGMEM;
using common::TEXT_DAA;
using common::TEXT_DEC;
using common::TEXT_DECA;
using common::TEXT_DECB;
using common::TEXT_EORA;
using common::TEXT_EORB;
using common::TEXT_EXG;
using common::TEXT_INC;
using common::TEXT_INCA;
using common::TEXT_INCB;
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
using common::TEXT_LBSR;
using common::TEXT_LBVC;
using common::TEXT_LBVS;
using common::TEXT_LDA;
using common::TEXT_LDB;
using common::TEXT_LDD;
using common::TEXT_LDS;
extern const char TEXT_LDU[]  PROGMEM;
using common::TEXT_LDX;
using common::TEXT_LDY;
using common::TEXT_LEAS;
extern const char TEXT_LEAU[] PROGMEM;
using common::TEXT_LEAX;
using common::TEXT_LEAY;
using common::TEXT_LSL;
using common::TEXT_LSLA;
using common::TEXT_LSLB;
using common::TEXT_LSR;
using common::TEXT_LSRA;
using common::TEXT_LSRB;
using common::TEXT_MUL;
using common::TEXT_NEG;
using common::TEXT_NEGA;
using common::TEXT_NEGB;
using common::TEXT_NOP;
using common::TEXT_ORA;
using common::TEXT_ORB;
using common::TEXT_ORCC;
extern const char TEXT_PSHS[] PROGMEM;
extern const char TEXT_PSHU[] PROGMEM;
extern const char TEXT_PULS[] PROGMEM;
extern const char TEXT_PULU[] PROGMEM;
using common::TEXT_ROL;
using common::TEXT_ROLA;
using common::TEXT_ROLB;
using common::TEXT_ROR;
using common::TEXT_RORA;
using common::TEXT_RORB;
using common::TEXT_RTI;
using common::TEXT_RTS;
using common::TEXT_SBCA;
using common::TEXT_SBCB;
using common::TEXT_SEX;
using common::TEXT_STA;
using common::TEXT_STB;
using common::TEXT_STD;
using common::TEXT_STS;
extern const char TEXT_STU[]  PROGMEM;
using common::TEXT_STX;
using common::TEXT_STY;
using common::TEXT_SUBA;
using common::TEXT_SUBB;
using common::TEXT_SUBD;
using common::TEXT_SWI;
extern const char TEXT_SWI2[] PROGMEM;
extern const char TEXT_SWI3[] PROGMEM;
extern const char TEXT_SYNC[] PROGMEM;
using common::TEXT_TFR;
using common::TEXT_TST;
using common::TEXT_TSTA;
using common::TEXT_TSTB;

// HD6309
using common::TEXT_ADCD;
extern const char TEXT_ADCR[]  PROGMEM;
using common::TEXT_ADDE;
using common::TEXT_ADDF;
using common::TEXT_ADDR;
using common::TEXT_ADDW;
using common::TEXT_AIM;
using common::TEXT_ANDD;
using common::TEXT_ANDR;
using common::TEXT_ASLD;
using common::TEXT_ASRD;
using common::TEXT_BAND;
extern const char TEXT_BEOR[]  PROGMEM;
extern const char TEXT_BIAND[] PROGMEM;
extern const char TEXT_BIEOR[] PROGMEM;
extern const char TEXT_BIOR[]  PROGMEM;
extern const char TEXT_BITD[]  PROGMEM;
extern const char TEXT_BITMD[] PROGMEM;
using common::TEXT_BOR;
using common::TEXT_CLRD;
using common::TEXT_CLRE;
using common::TEXT_CLRF;
using common::TEXT_CLRW;
extern const char TEXT_CMPE[]  PROGMEM;
using common::TEXT_CMPF;
using common::TEXT_CMPR;
using common::TEXT_CMPW;
using common::TEXT_COMD;
using common::TEXT_COME;
extern const char TEXT_COMF[]  PROGMEM;
using common::TEXT_COMW;
using common::TEXT_DECD;
extern const char TEXT_DECE[]  PROGMEM;
extern const char TEXT_DECF[]  PROGMEM;
using common::TEXT_DECW;
using common::TEXT_DIVD;
extern const char TEXT_DIVQ[]  PROGMEM;
using common::TEXT_EIM;
using common::TEXT_EORD;
using common::TEXT_EORR;
extern const char TEXT_INCD[]  PROGMEM;
extern const char TEXT_INCE[]  PROGMEM;
extern const char TEXT_INCF[]  PROGMEM;
using common::TEXT_INCW;
extern const char TEXT_LDBT[]  PROGMEM;
using common::TEXT_LDE;
using common::TEXT_LDF;
extern const char TEXT_LDMD[]  PROGMEM;
extern const char TEXT_LDQ[]   PROGMEM;
using common::TEXT_LDW;
using common::TEXT_LSLD;
using common::TEXT_LSRD;
using common::TEXT_LSRW;
using common::TEXT_MULD;
using common::TEXT_NEGD;
using common::TEXT_OIM;
using common::TEXT_ORD;
using common::TEXT_ORR;
extern const char TEXT_PSHSW[] PROGMEM;
extern const char TEXT_PSHUW[] PROGMEM;
extern const char TEXT_PULSW[] PROGMEM;
extern const char TEXT_PULUW[] PROGMEM;
using common::TEXT_ROLD;
using common::TEXT_ROLW;
using common::TEXT_RORD;
using common::TEXT_RORW;
using common::TEXT_SBCD;
extern const char TEXT_SBCR[]  PROGMEM;
extern const char TEXT_SEXW[]  PROGMEM;
extern const char TEXT_STBT[]  PROGMEM;
using common::TEXT_STE;
using common::TEXT_STF;
extern const char TEXT_STQ[]   PROGMEM;
extern const char TEXT_STW[]   PROGMEM;
using common::TEXT_SUBE;
using common::TEXT_SUBF;
using common::TEXT_SUBR;
using common::TEXT_SUBW;
extern const char TEXT_TFM[]   PROGMEM;
using common::TEXT_TIM;
using common::TEXT_TSTD;
using common::TEXT_TSTE;
using common::TEXT_TSTF;
using common::TEXT_TSTW;

using common::TEXT_REG_A;
using common::TEXT_REG_B;
extern const char TEXT_REG_CC[]  PROGMEM;
using common::TEXT_REG_D;
extern const char TEXT_REG_DP[]  PROGMEM;
using common::TEXT_REG_PC;
extern const char TEXT_REG_PCR[] PROGMEM;
using common::TEXT_REG_S;
extern const char TEXT_REG_U[]   PROGMEM;
using common::TEXT_REG_X;
using common::TEXT_REG_Y;

// HD6309
extern const char TEXT_REG_0[] PROGMEM;
using common::TEXT_REG_E;
extern const char TEXT_REG_F[] PROGMEM;
extern const char TEXT_REG_V[] PROGMEM;
using common::TEXT_REG_W;
using common::TEXT_REG_Z;

// clang-format on
}  // namespace mc6809
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_MC6809_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
