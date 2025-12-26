/*
 * Copyright 2024 Tadashi G. Takaoka
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

#ifndef __LIBASM_TEXT_PDP11_H__
#define __LIBASM_TEXT_PDP11_H__

#include "text_common.h"

namespace libasm {
namespace text {
namespace pdp11 {
// clang-format off

extern const char TEXT_PDP11_LIST[] PROGMEM;
extern const char TEXT_CPU_DCJ11[]  PROGMEM;
extern const char TEXT_CPU_DCT11[]  PROGMEM;
extern const char TEXT_CPU_J11[]    PROGMEM;
extern const char TEXT_CPU_T11[]    PROGMEM;

// PDP11 base instructions
using common::TEXT_ADC;
using common::TEXT_ADCB;
using common::TEXT_ADD;
using common::TEXT_ASL;
using common::TEXT_ASLB;
using common::TEXT_ASR;
using common::TEXT_ASRB;
using common::TEXT_BCC;
using common::TEXT_BCS;
using common::TEXT_BEQ;
using common::TEXT_BGE;
using common::TEXT_BGT;
using common::TEXT_BHI;
extern const char TEXT_BHIS[] PROGMEM;
extern const char TEXT_BIC[]  PROGMEM;
using common::TEXT_BICB;
extern const char TEXT_BIS[]  PROGMEM;
extern const char TEXT_BISB[] PROGMEM;
using common::TEXT_BIT;
using common::TEXT_BITB;
using common::TEXT_BLE;
using common::TEXT_BLO;
extern const char TEXT_BLOS[] PROGMEM;
using common::TEXT_BLT;
using common::TEXT_BMI;
using common::TEXT_BNE;
using common::TEXT_BPL;
using common::TEXT_BPT;
using common::TEXT_BR;
using common::TEXT_BVC;
using common::TEXT_BVS;
extern const char TEXT_CCC[]  PROGMEM;
using common::TEXT_CLC;
extern const char TEXT_CLN[]  PROGMEM;
using common::TEXT_CLR;
using common::TEXT_CLRB;
using common::TEXT_CLV;
extern const char TEXT_CLZ[]  PROGMEM;
using common::TEXT_CMP;
using common::TEXT_CMPB;
using common::TEXT_COM;
using common::TEXT_COMB;
using common::TEXT_DEC;
using common::TEXT_DECB;
extern const char TEXT_EMT[]  PROGMEM;
using common::TEXT_HALT;
using common::TEXT_INC;
using common::TEXT_INCB;
using common::TEXT_IOT;
using common::TEXT_JMP;
using common::TEXT_JSR;
extern const char TEXT_MFPS[] PROGMEM;
extern const char TEXT_MFPT[] PROGMEM;
using common::TEXT_MOV;
using common::TEXT_MOVB;
extern const char TEXT_MTPS[] PROGMEM;
using common::TEXT_NEG;
using common::TEXT_NEGB;
using common::TEXT_NOP;
extern const char TEXT_PDP[]  PROGMEM;
using common::TEXT_RESET;
using common::TEXT_ROL;
using common::TEXT_ROLB;
using common::TEXT_ROR;
using common::TEXT_RORB;
using common::TEXT_RTI;
using common::TEXT_RTS;
extern const char TEXT_RTT[]  PROGMEM;
using common::TEXT_SBC;
using common::TEXT_SBCB;
using common::TEXT_SCC;
using common::TEXT_SEC;
extern const char TEXT_SEN[]  PROGMEM;
using common::TEXT_SEV;
extern const char TEXT_SEZ[]  PROGMEM;
extern const char TEXT_SOB[]  PROGMEM;
using common::TEXT_SUB;
extern const char TEXT_SWAB[] PROGMEM;
using common::TEXT_SXT;
using common::TEXT_TRAP;
using common::TEXT_TST;
using common::TEXT_TSTB;
using common::TEXT_WAIT;
using common::TEXT_XOR;

// PDP-11 return from varargs subroutine
using common::TEXT_MARK;

// PDP-11 extended instruction set (EIS)
using common::TEXT_ASH;
extern const char TEXT_ASHC[]  PROGMEM;
using common::TEXT_DIV;
using common::TEXT_MUL;

// PDP-11 memory management
extern const char TEXT_MFPD[] PROGMEM;
extern const char TEXT_MFPI[] PROGMEM;
extern const char TEXT_MTPD[] PROGMEM;
extern const char TEXT_MTPI[] PROGMEM;

// DCJ11
extern const char TEXT_CSM[]    PROGMEM;
using common::TEXT_SPL;
extern const char TEXT_TSTSET[] PROGMEM;
extern const char TEXT_WRTLCK[] PROGMEM;

// PDP-11 floating point processor (FP11)
using common::TEXT_ABSD;
using common::TEXT_ABSF;
using common::TEXT_ADDD;
using common::TEXT_ADDF;
extern const char TEXT_CFCC[]  PROGMEM;
using common::TEXT_CLRD;
using common::TEXT_CLRF;
using common::TEXT_CMPD;
using common::TEXT_CMPF;
using common::TEXT_DIVD;
using common::TEXT_DIVF;
extern const char TEXT_LDCDF[] PROGMEM;
extern const char TEXT_LDCFD[] PROGMEM;
extern const char TEXT_LDCID[] PROGMEM;
extern const char TEXT_LDCIF[] PROGMEM;
extern const char TEXT_LDCLD[] PROGMEM;
extern const char TEXT_LDCLF[] PROGMEM;
using common::TEXT_LDD;
extern const char TEXT_LDEXP[] PROGMEM;
using common::TEXT_LDF;
extern const char TEXT_LDFPS[] PROGMEM;
using common::TEXT_MODD;
extern const char TEXT_MODF[]  PROGMEM;
using common::TEXT_MULD;
using common::TEXT_MULF;
using common::TEXT_NEGD;
using common::TEXT_NEGF;
extern const char TEXT_SETD[]  PROGMEM;
extern const char TEXT_SETF[]  PROGMEM;
extern const char TEXT_SETI[]  PROGMEM;
extern const char TEXT_SETL[]  PROGMEM;
extern const char TEXT_STCDF[] PROGMEM;
extern const char TEXT_STCDI[] PROGMEM;
extern const char TEXT_STCDL[] PROGMEM;
extern const char TEXT_STCFD[] PROGMEM;
extern const char TEXT_STCFI[] PROGMEM;
extern const char TEXT_STCFL[] PROGMEM;
using common::TEXT_STD;
extern const char TEXT_STEXP[] PROGMEM;
using common::TEXT_STF;
extern const char TEXT_STFPS[] PROGMEM;
using common::TEXT_STST;
using common::TEXT_SUBD;
using common::TEXT_SUBF;
using common::TEXT_TSTD;
using common::TEXT_TSTF;

using common::TEXT_REG_AC0;
using common::TEXT_REG_AC1;
extern const char TEXT_REG_AC2[] PROGMEM;
extern const char TEXT_REG_AC3[] PROGMEM;
extern const char TEXT_REG_AC4[] PROGMEM;
extern const char TEXT_REG_AC5[] PROGMEM;
using common::TEXT_REG_PC;
using common::TEXT_REG_R0;
using common::TEXT_REG_R1;
using common::TEXT_REG_R2;
using common::TEXT_REG_R3;
using common::TEXT_REG_R4;
using common::TEXT_REG_R5;
using common::TEXT_REG_R6;
using common::TEXT_REG_R7;
using common::TEXT_REG_SP;

// clang-format on
}  // namespace pdp11
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_PDP11_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
