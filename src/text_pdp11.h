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
extern const char TEXT_CPU_DCT11[] PROGMEM;
extern const char TEXT_CPU_DCJ11[] PROGMEM;
extern const char TEXT_CPU_T11[] PROGMEM;
extern const char TEXT_CPU_J11[] PROGMEM;

// PDP11 base instructions
extern const char TEXT_ADC[]   PROGMEM;
extern const char TEXT_ADCB[]  PROGMEM;
extern const char TEXT_ADD[]   PROGMEM;
extern const char TEXT_ASL[]   PROGMEM;
extern const char TEXT_ASLB[]  PROGMEM;
extern const char TEXT_ASR[]   PROGMEM;
extern const char TEXT_ASRB[]  PROGMEM;
extern const char TEXT_BCC[]   PROGMEM;
extern const char TEXT_BCS[]   PROGMEM;
extern const char TEXT_BEQ[]   PROGMEM;
extern const char TEXT_BGE[]   PROGMEM;
extern const char TEXT_BGT[]   PROGMEM;
extern const char TEXT_BHI[]   PROGMEM;
extern const char TEXT_BHIS[]  PROGMEM;
extern const char TEXT_BIC[]   PROGMEM;
extern const char TEXT_BICB[]  PROGMEM;
extern const char TEXT_BIS[]   PROGMEM;
extern const char TEXT_BISB[]  PROGMEM;
extern const char TEXT_BIT[]   PROGMEM;
extern const char TEXT_BITB[]  PROGMEM;
extern const char TEXT_BLE[]   PROGMEM;
extern const char TEXT_BLO[]   PROGMEM;
extern const char TEXT_BLOS[]  PROGMEM;
extern const char TEXT_BLT[]   PROGMEM;
extern const char TEXT_BMI[]   PROGMEM;
extern const char TEXT_BNE[]   PROGMEM;
extern const char TEXT_BPL[]   PROGMEM;
extern const char TEXT_BPT[]   PROGMEM;
extern const char TEXT_BR[]    PROGMEM;
extern const char TEXT_BVC[]   PROGMEM;
extern const char TEXT_BVS[]   PROGMEM;
extern const char TEXT_CCC[]   PROGMEM;
extern const char TEXT_CLC[]   PROGMEM;
extern const char TEXT_CLN[]   PROGMEM;
extern const char TEXT_CLR[]   PROGMEM;
extern const char TEXT_CLRB[]  PROGMEM;
extern const char TEXT_CLV[]   PROGMEM;
extern const char TEXT_CLZ[]   PROGMEM;
extern const char TEXT_CMP[]   PROGMEM;
extern const char TEXT_CMPB[]  PROGMEM;
extern const char TEXT_COM[]   PROGMEM;
extern const char TEXT_COMB[]  PROGMEM;
extern const char TEXT_DEC[]   PROGMEM;
extern const char TEXT_DECB[]  PROGMEM;
extern const char TEXT_EMT[]   PROGMEM;
extern const char TEXT_HALT[]  PROGMEM;
extern const char TEXT_INC[]   PROGMEM;
extern const char TEXT_INCB[]  PROGMEM;
extern const char TEXT_IOT[]   PROGMEM;
extern const char TEXT_JMP[]   PROGMEM;
extern const char TEXT_JSR[]   PROGMEM;
extern const char TEXT_MFPS[]  PROGMEM;
extern const char TEXT_MFPT[]  PROGMEM;
extern const char TEXT_MOV[]   PROGMEM;
extern const char TEXT_MOVB[]  PROGMEM;
extern const char TEXT_MTPS[]  PROGMEM;
extern const char TEXT_NEG[]   PROGMEM;
extern const char TEXT_NEGB[]  PROGMEM;
extern const char TEXT_NOP[]   PROGMEM;
extern const char TEXT_RESET[] PROGMEM;
extern const char TEXT_ROL[]   PROGMEM;
extern const char TEXT_ROLB[]  PROGMEM;
extern const char TEXT_ROR[]   PROGMEM;
extern const char TEXT_RORB[]  PROGMEM;
extern const char TEXT_RTI[]   PROGMEM;
extern const char TEXT_RTS[]   PROGMEM;
extern const char TEXT_RTT[]   PROGMEM;
extern const char TEXT_SBC[]   PROGMEM;
extern const char TEXT_SBCB[]  PROGMEM;
extern const char TEXT_SCC[]   PROGMEM;
extern const char TEXT_SEC[]   PROGMEM;
extern const char TEXT_SEN[]   PROGMEM;
extern const char TEXT_SEV[]   PROGMEM;
extern const char TEXT_SEZ[]   PROGMEM;
extern const char TEXT_SOB[]   PROGMEM;
extern const char TEXT_SUB[]   PROGMEM;
extern const char TEXT_SWAB[]  PROGMEM;
extern const char TEXT_SXT[]   PROGMEM;
extern const char TEXT_TRAP[]  PROGMEM;
extern const char TEXT_TST[]   PROGMEM;
extern const char TEXT_TSTB[]  PROGMEM;
extern const char TEXT_WAIT[]  PROGMEM;
extern const char TEXT_XOR[]   PROGMEM;
extern const char TEXT_PDP[]   PROGMEM;
// PDP-11 return from varargs subroutine
extern const char TEXT_MARK[]  PROGMEM;
// PDP-11 extended instruction set (EIS)
extern const char TEXT_ASH[]   PROGMEM;
extern const char TEXT_ASHC[]  PROGMEM;
extern const char TEXT_DIV[]   PROGMEM;
extern const char TEXT_MUL[]   PROGMEM;
// PDP-11 memory management
extern const char TEXT_MFPD[]  PROGMEM;
extern const char TEXT_MFPI[]  PROGMEM;
extern const char TEXT_MTPD[]  PROGMEM;
extern const char TEXT_MTPI[]  PROGMEM;
// DCJ11
extern const char TEXT_CSM[]   PROGMEM;
extern const char TEXT_SPL[]   PROGMEM;
extern const char TEXT_TSTSET[] PROGMEM;
extern const char TEXT_WRTLCK[] PROGMEM;

// PDP-11 floating point processor (FP11)
extern const char TEXT_ABSD[]  PROGMEM;
extern const char TEXT_ABSF[]  PROGMEM;
extern const char TEXT_ADDD[]  PROGMEM;
extern const char TEXT_ADDF[]  PROGMEM;
extern const char TEXT_CFCC[]  PROGMEM;
extern const char TEXT_CLRD[]  PROGMEM;
extern const char TEXT_CLRF[]  PROGMEM;
extern const char TEXT_CMPD[]  PROGMEM;
extern const char TEXT_CMPF[]  PROGMEM;
extern const char TEXT_DIVD[]  PROGMEM;
extern const char TEXT_DIVF[]  PROGMEM;
extern const char TEXT_LDEXP[] PROGMEM;
extern const char TEXT_LDCDF[] PROGMEM;
extern const char TEXT_LDCFD[] PROGMEM;
extern const char TEXT_LDCID[] PROGMEM;
extern const char TEXT_LDCIF[] PROGMEM;
extern const char TEXT_LDCLD[] PROGMEM;
extern const char TEXT_LDCLF[] PROGMEM;
extern const char TEXT_LDD[]   PROGMEM;
extern const char TEXT_LDF[]   PROGMEM;
extern const char TEXT_LDFPS[] PROGMEM;
extern const char TEXT_MODD[]  PROGMEM;
extern const char TEXT_MODF[]  PROGMEM;
extern const char TEXT_MULD[]  PROGMEM;
extern const char TEXT_MULF[]  PROGMEM;
extern const char TEXT_NEGD[]  PROGMEM;
extern const char TEXT_NEGF[]  PROGMEM;
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
extern const char TEXT_STD[]   PROGMEM;
extern const char TEXT_STEXP[] PROGMEM;
extern const char TEXT_STF[]   PROGMEM;
extern const char TEXT_STFPS[] PROGMEM;
extern const char TEXT_STST[]  PROGMEM;
extern const char TEXT_SUBD[]  PROGMEM;
extern const char TEXT_SUBF[]  PROGMEM;
extern const char TEXT_TSTD[]  PROGMEM;
extern const char TEXT_TSTF[]  PROGMEM;

extern const char TEXT_REG_AC0[] PROGMEM;
extern const char TEXT_REG_AC1[] PROGMEM;
extern const char TEXT_REG_AC2[] PROGMEM;
extern const char TEXT_REG_AC3[] PROGMEM;
extern const char TEXT_REG_AC4[] PROGMEM;
extern const char TEXT_REG_AC5[] PROGMEM;
extern const char TEXT_REG_PC[]  PROGMEM;
extern const char TEXT_REG_R0[]  PROGMEM;
extern const char TEXT_REG_R1[]  PROGMEM;
extern const char TEXT_REG_R2[]  PROGMEM;
extern const char TEXT_REG_R3[]  PROGMEM;
extern const char TEXT_REG_R4[]  PROGMEM;
extern const char TEXT_REG_R5[]  PROGMEM;
extern const char TEXT_REG_R6[]  PROGMEM;
extern const char TEXT_REG_R7[]  PROGMEM;
extern const char TEXT_REG_SP[]  PROGMEM;
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
