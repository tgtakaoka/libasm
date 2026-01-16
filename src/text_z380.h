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

#ifndef __LIBASM_TEXT_Z380__
#define __LIBASM_TEXT_Z380__

#include "text_common.h"

namespace libasm {
namespace text {
namespace z380 {
// clang-format off

extern const char TEXT_Z380_LIST[] PROGMEM;
using common::TEXT_CPU_Z180;
extern const char TEXT_CPU_Z380[]  PROGMEM;
using common::TEXT_CPU_Z80;

// Z80
using common::TEXT_ADC;
using common::TEXT_ADD;
using common::TEXT_AND;
using common::TEXT_BIT;
using common::TEXT_CALL;
using common::TEXT_CCF;
using common::TEXT_CP;
using common::TEXT_CPD;
using common::TEXT_CPDR;
using common::TEXT_CPI;
using common::TEXT_CPIR;
using common::TEXT_CPL;
using common::TEXT_DAA;
using common::TEXT_DEC;
using common::TEXT_DI;
using common::TEXT_DJNZ;
using common::TEXT_EI;
using common::TEXT_EX;
using common::TEXT_EXX;
using common::TEXT_HALT;
using common::TEXT_IM;
using common::TEXT_IN;
using common::TEXT_INC;
using common::TEXT_IND;
using common::TEXT_INDR;
using common::TEXT_INI;
using common::TEXT_INIR;
using common::TEXT_JP;
using common::TEXT_JR;
using common::TEXT_LD;
using common::TEXT_LDD;
using common::TEXT_LDDR;
using common::TEXT_LDI;
using common::TEXT_LDIR;
using common::TEXT_NEG;
using common::TEXT_NOP;
using common::TEXT_OR;
using common::TEXT_OTDR;
using common::TEXT_OTIR;
using common::TEXT_OUT;
using common::TEXT_OUTD;
using common::TEXT_OUTI;
using common::TEXT_POP;
using common::TEXT_PUSH;
using common::TEXT_RER;
using common::TEXT_RES;
using common::TEXT_RET;
using common::TEXT_RETI;
using common::TEXT_RETN;
using common::TEXT_RL;
using common::TEXT_RLA;
using common::TEXT_RLC;
using common::TEXT_RLCA;
using common::TEXT_RLD;
using common::TEXT_RR;
using common::TEXT_RRA;
using common::TEXT_RRC;
using common::TEXT_RRCA;
using common::TEXT_RRD;
using common::TEXT_RST;
using common::TEXT_SBC;
using common::TEXT_SCF;
using common::TEXT_SET;
using common::TEXT_SLA;
using common::TEXT_SRA;
using common::TEXT_SRL;
using common::TEXT_SUB;
using common::TEXT_XOR;

// Z180
using common::TEXT_IN0;
using common::TEXT_MLT;
using common::TEXT_OTDM;
using common::TEXT_OTDMR;
using common::TEXT_OTIM;
using common::TEXT_OTIMR;
using common::TEXT_OUT0;
using common::TEXT_SLP;
using common::TEXT_TST;
using common::TEXT_TSTIO;

// Z380
extern const char TEXT_ADCW[]  PROGMEM;
using common::TEXT_ADDW;
using common::TEXT_ANDW;
extern const char TEXT_BTEST[] PROGMEM;
using common::TEXT_CALR;
extern const char TEXT_CPLW[]  PROGMEM;
using common::TEXT_CPW;
extern const char TEXT_DDIR[]  PROGMEM;
using common::TEXT_DECW;
using common::TEXT_DIVUW;
using common::TEXT_DIVW;
extern const char TEXT_EXALL[] PROGMEM;
using common::TEXT_EXTS;
using common::TEXT_EXTSW;
extern const char TEXT_EXXX[]  PROGMEM;
extern const char TEXT_EXXY[]  PROGMEM;
extern const char TEXT_INA[]   PROGMEM;
extern const char TEXT_INAW[]  PROGMEM;
using common::TEXT_INCW;
using common::TEXT_INDRW;
using common::TEXT_INDW;
using common::TEXT_INIRW;
using common::TEXT_INIW;
using common::TEXT_INW;
using common::TEXT_LDCTL;
extern const char TEXT_LDDRW[] PROGMEM;
extern const char TEXT_LDDW[]  PROGMEM;
extern const char TEXT_LDIRW[] PROGMEM;
extern const char TEXT_LDIW[]  PROGMEM;
using common::TEXT_LDW;
extern const char TEXT_MTEST[] PROGMEM;
using common::TEXT_MULTUW;
using common::TEXT_MULTW;
using common::TEXT_NEGW;
using common::TEXT_ORW;
using common::TEXT_OTDRW;
using common::TEXT_OTIRW;
extern const char TEXT_OUTA[]  PROGMEM;
extern const char TEXT_OUTAW[] PROGMEM;
using common::TEXT_OUTDW;
using common::TEXT_OUTIW;
using common::TEXT_OUTW;
extern const char TEXT_RESC[]  PROGMEM;
extern const char TEXT_RETB[]  PROGMEM;
extern const char TEXT_RLCW[]  PROGMEM;
extern const char TEXT_RLW[]   PROGMEM;
extern const char TEXT_RRCW[]  PROGMEM;
extern const char TEXT_RRW[]   PROGMEM;
extern const char TEXT_SBCW[]  PROGMEM;
using common::TEXT_SETC;
extern const char TEXT_SLAW[]  PROGMEM;
extern const char TEXT_SRAW[]  PROGMEM;
extern const char TEXT_SRLW[]  PROGMEM;
using common::TEXT_SUBW;
using common::TEXT_SWAP;
using common::TEXT_XORW;

// Z80, Z180
using common::TEXT_REG_A;
using common::TEXT_REG_AF;
using common::TEXT_REG_B;
using common::TEXT_REG_BC;
using common::TEXT_REG_C;
using common::TEXT_REG_D;
using common::TEXT_REG_DE;
using common::TEXT_REG_E;
using common::TEXT_REG_H;
using common::TEXT_REG_HL;
using common::TEXT_REG_I;
using common::TEXT_REG_IX;
using common::TEXT_REG_IY;
using common::TEXT_REG_L;
using common::TEXT_REG_R;
using common::TEXT_REG_SP;

// Z380
extern const char TEXT_REG_DSR[] PROGMEM;
using common::TEXT_REG_IXL;
extern const char TEXT_REG_IXU[] PROGMEM;
using common::TEXT_REG_IYL;
extern const char TEXT_REG_IYU[] PROGMEM;
using common::TEXT_REG_SR;
extern const char TEXT_REG_XSR[] PROGMEM;
extern const char TEXT_REG_YSR[] PROGMEM;

// Z80, Z180
using common::TEXT_CC_C;
using common::TEXT_CC_M;
using common::TEXT_CC_NC;
using common::TEXT_CC_NZ;
using common::TEXT_CC_P;
using common::TEXT_CC_PE;
using common::TEXT_CC_PO;
using common::TEXT_CC_Z;

// Z380
using common::TEXT_CC_NS;
using common::TEXT_CC_NV;
using common::TEXT_CC_S;
using common::TEXT_CC_V;

extern const char PSTR_IB[]  PROGMEM;
extern const char PSTR_IW[]  PROGMEM;
extern const char PSTR_LCK[] PROGMEM;
extern const char PSTR_LW[]  PROGMEM;
extern const char PSTR_W[]   PROGMEM;
extern const char PSTR_XM[]  PROGMEM;

// clang-format on
}  // namespace z380
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_Z380__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
