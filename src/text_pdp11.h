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

extern const char TEXT_PDP11_LIST[];
extern const char TEXT_CPU_DCJ11[];
extern const char TEXT_CPU_DCT11[];
extern const char TEXT_CPU_J11[];
extern const char TEXT_CPU_T11[];

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
extern const char TEXT_BHIS[];
extern const char TEXT_BIC[];
using common::TEXT_BICB;
extern const char TEXT_BIS[];
extern const char TEXT_BISB[];
using common::TEXT_BIT;
using common::TEXT_BITB;
using common::TEXT_BLE;
using common::TEXT_BLO;
extern const char TEXT_BLOS[];
using common::TEXT_BLT;
using common::TEXT_BMI;
using common::TEXT_BNE;
using common::TEXT_BPL;
using common::TEXT_BPT;
using common::TEXT_BR;
using common::TEXT_BVC;
using common::TEXT_BVS;
extern const char TEXT_CCC[];
using common::TEXT_CLC;
extern const char TEXT_CLN[];
using common::TEXT_CLR;
using common::TEXT_CLRB;
using common::TEXT_CLV;
extern const char TEXT_CLZ[];
using common::TEXT_CMP;
using common::TEXT_CMPB;
using common::TEXT_COM;
using common::TEXT_COMB;
using common::TEXT_DEC;
using common::TEXT_DECB;
extern const char TEXT_EMT[];
using common::TEXT_HALT;
using common::TEXT_INC;
using common::TEXT_INCB;
using common::TEXT_IOT;
using common::TEXT_JMP;
using common::TEXT_JSR;
extern const char TEXT_MFPS[];
extern const char TEXT_MFPT[];
using common::TEXT_MOV;
using common::TEXT_MOVB;
extern const char TEXT_MTPS[];
using common::TEXT_NEG;
using common::TEXT_NEGB;
using common::TEXT_NOP;
extern const char TEXT_PDP[];
using common::TEXT_RESET;
using common::TEXT_ROL;
using common::TEXT_ROLB;
using common::TEXT_ROR;
using common::TEXT_RORB;
using common::TEXT_RTI;
using common::TEXT_RTS;
extern const char TEXT_RTT[];
using common::TEXT_SBC;
using common::TEXT_SBCB;
using common::TEXT_SCC;
using common::TEXT_SEC;
extern const char TEXT_SEN[];
using common::TEXT_SEV;
extern const char TEXT_SEZ[];
extern const char TEXT_SOB[];
using common::TEXT_SUB;
extern const char TEXT_SWAB[];
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
extern const char TEXT_ASHC[];
using common::TEXT_DIV;
using common::TEXT_MUL;

// PDP-11 memory management
extern const char TEXT_MFPD[];
extern const char TEXT_MFPI[];
extern const char TEXT_MTPD[];
extern const char TEXT_MTPI[];

// DCJ11
extern const char TEXT_CSM[];
using common::TEXT_SPL;
extern const char TEXT_TSTSET[];
extern const char TEXT_WRTLCK[];

// PDP-11 floating point processor (FP11)
using common::TEXT_ABSD;
using common::TEXT_ABSF;
using common::TEXT_ADDD;
using common::TEXT_ADDF;
extern const char TEXT_CFCC[];
using common::TEXT_CLRD;
using common::TEXT_CLRF;
using common::TEXT_CMPD;
using common::TEXT_CMPF;
using common::TEXT_DIVD;
using common::TEXT_DIVF;
extern const char TEXT_LDCDF[];
extern const char TEXT_LDCFD[];
extern const char TEXT_LDCID[];
extern const char TEXT_LDCIF[];
extern const char TEXT_LDCLD[];
extern const char TEXT_LDCLF[];
using common::TEXT_LDD;
extern const char TEXT_LDEXP[];
using common::TEXT_LDF;
extern const char TEXT_LDFPS[];
using common::TEXT_MODD;
extern const char TEXT_MODF[];
using common::TEXT_MULD;
using common::TEXT_MULF;
using common::TEXT_NEGD;
using common::TEXT_NEGF;
extern const char TEXT_SETD[];
extern const char TEXT_SETF[];
extern const char TEXT_SETI[];
using common::TEXT_SETL;
extern const char TEXT_STCDF[];
extern const char TEXT_STCDI[];
extern const char TEXT_STCDL[];
extern const char TEXT_STCFD[];
extern const char TEXT_STCFI[];
extern const char TEXT_STCFL[];
using common::TEXT_STD;
extern const char TEXT_STEXP[];
using common::TEXT_STF;
extern const char TEXT_STFPS[];
using common::TEXT_STST;
using common::TEXT_SUBD;
using common::TEXT_SUBF;
using common::TEXT_TSTD;
using common::TEXT_TSTF;

using common::TEXT_REG_AC0;
using common::TEXT_REG_AC1;
extern const char TEXT_REG_AC2[];
extern const char TEXT_REG_AC3[];
extern const char TEXT_REG_AC4[];
extern const char TEXT_REG_AC5[];
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
