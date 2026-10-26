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

#ifndef __LIBASM_TEXT_MC68HC16__
#define __LIBASM_TEXT_MC68HC16__

#include "text_common.h"

namespace libasm {
namespace text {
namespace mc68hc16 {
// clang-format off

extern const char TEXT_CPU_MC68HC16[];
extern const char TEXT_CPU_68HC16[];

// MC68HC16
using common::TEXT_ABA;
using common::TEXT_ABX;
using common::TEXT_ABY;
extern const char TEXT_ABZ[];
extern const char TEXT_ACE[];
extern const char TEXT_ACED[];
using common::TEXT_ADCA;
using common::TEXT_ADCB;
using common::TEXT_ADCD;
extern const char TEXT_ADCE[];
using common::TEXT_ADDA;
using common::TEXT_ADDB;
using common::TEXT_ADDD;
using common::TEXT_ADDE;
using common::TEXT_ADE;
using common::TEXT_ADX;
extern const char TEXT_ADY[];
extern const char TEXT_ADZ[];
extern const char TEXT_AEX[];
extern const char TEXT_AEY[];
extern const char TEXT_AEZ[];
using common::TEXT_AIS;
using common::TEXT_AIX;
extern const char TEXT_AIY[];
extern const char TEXT_AIZ[];
using common::TEXT_ANDA;
using common::TEXT_ANDB;
using common::TEXT_ANDD;
extern const char TEXT_ANDE[];
using common::TEXT_ANDP;
using common::TEXT_ASL;
using common::TEXT_ASLA;
using common::TEXT_ASLB;
using common::TEXT_ASLD;
extern const char TEXT_ASLE[];
extern const char TEXT_ASLM[];
using common::TEXT_ASLW;
using common::TEXT_ASR;
using common::TEXT_ASRA;
using common::TEXT_ASRB;
using common::TEXT_ASRD;
extern const char TEXT_ASRE[];
extern const char TEXT_ASRM[];
using common::TEXT_ASRW;
using common::TEXT_BCC;
using common::TEXT_BCLR;
extern const char TEXT_BCLRW[];
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
extern const char TEXT_BSETW[];
using common::TEXT_BSR;
using common::TEXT_BVC;
using common::TEXT_BVS;
using common::TEXT_CBA;
using common::TEXT_CLC;
using common::TEXT_CLI;
using common::TEXT_CLR;
using common::TEXT_CLRA;
using common::TEXT_CLRB;
using common::TEXT_CLRD;
using common::TEXT_CLRE;
extern const char TEXT_CLRM[];
using common::TEXT_CLRW;
using common::TEXT_CLV;
using common::TEXT_CMPA;
using common::TEXT_CMPB;
using common::TEXT_COM;
using common::TEXT_COMA;
using common::TEXT_COMB;
using common::TEXT_COMD;
using common::TEXT_COME;
using common::TEXT_COMW;
using common::TEXT_CPD;
using common::TEXT_CPE;
using common::TEXT_CPS;
using common::TEXT_CPX;
using common::TEXT_CPY;
extern const char TEXT_CPZ[];
using common::TEXT_DAA;
using common::TEXT_DEC;
using common::TEXT_DECA;
using common::TEXT_DECB;
using common::TEXT_DECW;
using common::TEXT_DES;
using common::TEXT_DEX;
using common::TEXT_DEY;
using common::TEXT_EDIV;
using common::TEXT_EDIVS;
using common::TEXT_EMUL;
using common::TEXT_EMULS;
using common::TEXT_EORA;
using common::TEXT_EORB;
using common::TEXT_EORD;
extern const char TEXT_EORE[];
using common::TEXT_FDIV;
extern const char TEXT_FMULS[];
using common::TEXT_IDIV;
using common::TEXT_INC;
using common::TEXT_INCA;
using common::TEXT_INCB;
using common::TEXT_INCW;
using common::TEXT_INS;
using common::TEXT_INX;
using common::TEXT_INY;
using common::TEXT_JMP;
using common::TEXT_JSR;
using common::TEXT_LBCC;
using common::TEXT_LBCS;
using common::TEXT_LBEQ;
extern const char TEXT_LBEV[];
using common::TEXT_LBGE;
using common::TEXT_LBGT;
using common::TEXT_LBHI;
using common::TEXT_LBHS;
using common::TEXT_LBLE;
using common::TEXT_LBLO;
using common::TEXT_LBLS;
using common::TEXT_LBLT;
using common::TEXT_LBMI;
extern const char TEXT_LBMV[];
using common::TEXT_LBNE;
using common::TEXT_LBPL;
using common::TEXT_LBRA;
using common::TEXT_LBRN;
using common::TEXT_LBSR;
using common::TEXT_LBVC;
using common::TEXT_LBVS;
using common::TEXT_LDAA;
using common::TEXT_LDAB;
using common::TEXT_LDD;
using common::TEXT_LDE;
using common::TEXT_LDED;
extern const char TEXT_LDHI[];
using common::TEXT_LDS;
using common::TEXT_LDX;
using common::TEXT_LDY;
extern const char TEXT_LDZ[];
extern const char TEXT_LPSTOP[];
using common::TEXT_LSL;
using common::TEXT_LSLA;
using common::TEXT_LSLB;
using common::TEXT_LSLD;
extern const char TEXT_LSLE[];
using common::TEXT_LSLW;
using common::TEXT_LSR;
using common::TEXT_LSRA;
using common::TEXT_LSRB;
using common::TEXT_LSRD;
extern const char TEXT_LSRE[];
using common::TEXT_LSRW;
using common::TEXT_MAC;
using common::TEXT_MOVB;
using common::TEXT_MOVW;
using common::TEXT_MUL;
using common::TEXT_NEG;
using common::TEXT_NEGA;
using common::TEXT_NEGB;
using common::TEXT_NEGD;
extern const char TEXT_NEGE[];
using common::TEXT_NEGW;
using common::TEXT_NOP;
using common::TEXT_ORAA;
using common::TEXT_ORAB;
using common::TEXT_ORD;
using common::TEXT_ORE;
using common::TEXT_ORP;
using common::TEXT_PSHA;
using common::TEXT_PSHB;
using common::TEXT_PSHD;
using common::TEXT_PSHM;
extern const char TEXT_PSHMAC[];
using common::TEXT_PSHX;
using common::TEXT_PSHY;
using common::TEXT_PULA;
using common::TEXT_PULB;
using common::TEXT_PULD;
extern const char TEXT_PULM[];
extern const char TEXT_PULMAC[];
using common::TEXT_PULX;
using common::TEXT_PULY;
extern const char TEXT_RMAC[];
using common::TEXT_ROL;
using common::TEXT_ROLA;
using common::TEXT_ROLB;
using common::TEXT_ROLD;
extern const char TEXT_ROLE[];
using common::TEXT_ROLW;
using common::TEXT_ROR;
using common::TEXT_RORA;
using common::TEXT_RORB;
using common::TEXT_RORD;
extern const char TEXT_RORE[];
using common::TEXT_RORW;
using common::TEXT_RTI;
using common::TEXT_RTS;
using common::TEXT_SBA;
using common::TEXT_SBCA;
using common::TEXT_SBCB;
using common::TEXT_SBCD;
extern const char TEXT_SBCE[];
extern const char TEXT_SDE[];
using common::TEXT_SEC;
using common::TEXT_SEI;
using common::TEXT_SEV;
using common::TEXT_STAA;
using common::TEXT_STAB;
using common::TEXT_STD;
using common::TEXT_STE;
extern const char TEXT_STED[];
using common::TEXT_STS;
using common::TEXT_STX;
using common::TEXT_STY;
using common::TEXT_STZ;
using common::TEXT_SUBA;
using common::TEXT_SUBB;
using common::TEXT_SUBD;
using common::TEXT_SUBE;
using common::TEXT_SWI;
using common::TEXT_SXT;
using common::TEXT_TAB;
using common::TEXT_TAP;
using common::TEXT_TBA;
extern const char TEXT_TBEK[];
extern const char TEXT_TBSK[];
extern const char TEXT_TBXK[];
extern const char TEXT_TBYK[];
extern const char TEXT_TBZK[];
extern const char TEXT_TDE[];
extern const char TEXT_TDMSK[];
extern const char TEXT_TDP[];
extern const char TEXT_TED[];
extern const char TEXT_TEDM[];
extern const char TEXT_TEKB[];
extern const char TEXT_TEM[];
extern const char TEXT_TMER[];
extern const char TEXT_TMET[];
extern const char TEXT_TMXED[];
using common::TEXT_TPA;
extern const char TEXT_TPD[];
extern const char TEXT_TSKB[];
using common::TEXT_TST;
using common::TEXT_TSTA;
using common::TEXT_TSTB;
using common::TEXT_TSTD;
using common::TEXT_TSTE;
using common::TEXT_TSTW;
using common::TEXT_TSX;
using common::TEXT_TSY;
extern const char TEXT_TSZ[];
extern const char TEXT_TXKB[];
using common::TEXT_TXS;
using common::TEXT_TXY;
extern const char TEXT_TXZ[];
extern const char TEXT_TYKB[];
using common::TEXT_TYS;
using common::TEXT_TYX;
extern const char TEXT_TYZ[];
extern const char TEXT_TZKB[];
extern const char TEXT_TZS[];
extern const char TEXT_TZX[];
extern const char TEXT_TZY[];
using common::TEXT_WAI;
extern const char TEXT_XGAB[];
extern const char TEXT_XGDE[];
using common::TEXT_XGDX;
using common::TEXT_XGDY;
extern const char TEXT_XGDZ[];
extern const char TEXT_XGEX[];
extern const char TEXT_XGEY[];
extern const char TEXT_XGEZ[];

using common::TEXT_REG_CCR;
using common::TEXT_REG_D;
using common::TEXT_REG_E;
using common::TEXT_REG_K;
using common::TEXT_REG_X;
using common::TEXT_REG_Y;
using common::TEXT_REG_Z;

// clang-format on
}  // namespace mc68hc16
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_MC68HC16__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
