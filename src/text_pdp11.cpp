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

#include "text_pdp11.h"

namespace libasm {
namespace text {
namespace pdp11 {
// clang-format off

constexpr char TEXT_PDP11_LIST[] = "J11, T11";
constexpr char TEXT_CPU_DCJ11[] = "DCJ11";
constexpr char TEXT_CPU_DCT11[] = "DCT11";
constexpr char TEXT_CPU_J11[]   = "J11";
constexpr char TEXT_CPU_T11[]   = "T11";

// PDP11 base instructions
// constexpr char TEXT_ADC[]   = "ADC";
// constexpr char TEXT_ADCB[]  = "ADCB";
// constexpr char TEXT_ADD[]   = "ADD";
// constexpr char TEXT_ASL[]   = "ASL";
// constexpr char TEXT_ASLB[]  = "ASLB";
// constexpr char TEXT_ASR[]   = "ASR";
// constexpr char TEXT_ASRB[]  = "ASRB";
// constexpr char TEXT_BCC[]   = "BCC";
// constexpr char TEXT_BCS[]   = "BCS";
// constexpr char TEXT_BEQ[]   = "BEQ";
// constexpr char TEXT_BGE[]   = "BGE";
// constexpr char TEXT_BGT[]   = "BGT";
// constexpr char TEXT_BHI[]   = "BHI";
constexpr char TEXT_BHIS[] = "BHIS";
constexpr char TEXT_BIC[]  = "BIC";
// constexpr char TEXT_BICB[]  = "BICB";
constexpr char TEXT_BIS[]  = "BIS";
constexpr char TEXT_BISB[] = "BISB";
// constexpr char TEXT_BIT[]   = "BIT";
// constexpr char TEXT_BITB[]  = "BITB";
// constexpr char TEXT_BLE[]   = "BLE";
// constexpr char TEXT_BLO[]   = "BLO";
constexpr char TEXT_BLOS[] = "BLOS";
// constexpr char TEXT_BLT[]   = "BLT";
// constexpr char TEXT_BMI[]   = "BMI";
// constexpr char TEXT_BNE[]   = "BNE";
// constexpr char TEXT_BPL[]   = "BPL";
// constexpr char TEXT_BPT[]   = "BPT";
// constexpr char TEXT_BR[]    = "BR";
// constexpr char TEXT_BVC[]   = "BVC";
// constexpr char TEXT_BVS[]   = "BVS";
constexpr char TEXT_CCC[]  = "CCC";
// constexpr char TEXT_CLC[]   = "CLC";
constexpr char TEXT_CLN[]  = "CLN";
// constexpr char TEXT_CLR[]   = "CLR";
// constexpr char TEXT_CLRB[]  = "CLRB";
// constexpr char TEXT_CLV[]   = "CLV";
constexpr char TEXT_CLZ[]  = "CLZ";
// constexpr char TEXT_CMP[]   = "CMP";
// constexpr char TEXT_CMPB[]  = "CMPB";
// constexpr char TEXT_COM[]   = "COM";
// constexpr char TEXT_COMB[]  = "COMB";
// constexpr char TEXT_DEC[]   = "DEC";
// constexpr char TEXT_DECB[]  = "DECB";
constexpr char TEXT_EMT[]  = "EMT";
// constexpr char TEXT_HALT[]  = "HALT";
// constexpr char TEXT_INC[]   = "INC";
// constexpr char TEXT_INCB[]  = "INCB";
// constexpr char TEXT_IOT[]   = "IOT";
// constexpr char TEXT_JMP[]   = "JMP";
// constexpr char TEXT_JSR[]   = "JSR";
constexpr char TEXT_MFPS[] = "MFPS";
constexpr char TEXT_MFPT[] = "MFPT";
// constexpr char TEXT_MOV[]   = "MOV";
// constexpr char TEXT_MOVB[]  = "MOVB";
constexpr char TEXT_MTPS[] = "MTPS";
// constexpr char TEXT_NEG[]   = "NEG";
// constexpr char TEXT_NEGB[]  = "NEGB";
// constexpr char TEXT_NOP[]   = "NOP";
constexpr char TEXT_PDP[]  = "PDP";
// constexpr char TEXT_RESET[] = "RESET";
// constexpr char TEXT_ROL[]   = "ROL";
// constexpr char TEXT_ROLB[]  = "ROLB";
// constexpr char TEXT_ROR[]   = "ROR";
// constexpr char TEXT_RORB[]  = "RORB";
// constexpr char TEXT_RTI[]   = "RTI";
// constexpr char TEXT_RTS[]   = "RTS";
constexpr char TEXT_RTT[]  = "RTT";
// constexpr char TEXT_SBC[]   = "SBC";
// constexpr char TEXT_SBCB[]  = "SBCB";
// constexpr char TEXT_SCC[]   = "SCC";
// constexpr char TEXT_SEC[]   = "SEC";
constexpr char TEXT_SEN[]  = "SEN";
// constexpr char TEXT_SEV[]   = "SEV";
constexpr char TEXT_SEZ[]  = "SEZ";
constexpr char TEXT_SOB[]  = "SOB";
// constexpr char TEXT_SUB[]   = "SUB";
constexpr char TEXT_SWAB[] = "SWAB";
// constexpr char TEXT_SXT[]   = "SXT";
// constexpr char TEXT_TRAP[]  = "TRAP";
// constexpr char TEXT_TST[]   = "TST";
// constexpr char TEXT_TSTB[]  = "TSTB";
// constexpr char TEXT_WAIT[]  = "WAIT";
// constexpr char TEXT_XOR[]   = "XOR";

// PDP-11 return from varargs subroutine
// constexpr char TEXT_MARK[]  = "MARK";

// PDP-11 extended instruction set (EIS)
// constexpr char TEXT_ASH[]   = "ASH";
constexpr char TEXT_ASHC[]  = "ASHC";
// constexpr char TEXT_DIV[]   = "DIV";
// constexpr char TEXT_MUL[]   = "MUL";

// PDP-11 memory management
constexpr char TEXT_MFPD[] = "MFPD";
constexpr char TEXT_MFPI[] = "MFPI";
constexpr char TEXT_MTPD[] = "MTPD";
constexpr char TEXT_MTPI[] = "MTPI";

// DCJ11
constexpr char TEXT_CSM[]    = "CSM";
// constexpr char TEXT_SPL[] = "SPL";
constexpr char TEXT_TSTSET[] = "TSTSET";
constexpr char TEXT_WRTLCK[] = "WRTLCK";

// PDP-11 floating point processor (FP11)
// constexpr char TEXT_ABSD[] = "ABSD";
// constexpr char TEXT_ABSF[] = "ABSF";
// constexpr char TEXT_ADDD[] = "ADDD";
// constexpr char TEXT_ADDF[] = "ADDF";
constexpr   char TEXT_CFCC[]  = "CFCC";
// constexpr char TEXT_CLRD[] = "CLRD";
// constexpr char TEXT_CLRF[] = "CLRF";
// constexpr char TEXT_CMPD[] = "CMPD";
// constexpr char TEXT_CMPF[] = "CMPF";
// constexpr char TEXT_DIVD[] = "DIVD";
// constexpr char TEXT_DIVF[] = "DIVF";
constexpr   char TEXT_LDCDF[] = "LDCDF";
constexpr   char TEXT_LDCFD[] = "LDCFD";
constexpr   char TEXT_LDCID[] = "LDCID";
constexpr   char TEXT_LDCIF[] = "LDCIF";
constexpr   char TEXT_LDCLD[] = "LDCLD";
constexpr   char TEXT_LDCLF[] = "LDCLF";
// constexpr char TEXT_LDD[]  = "LDD";
constexpr   char TEXT_LDEXP[] = "LDEXP";
//constexpr char TEXT_LDF[]   = "LDF";
constexpr   char TEXT_LDFPS[] = "LDFPS";
// constexpr char TEXT_MODD[] = "MODD";
constexpr   char TEXT_MODF[]  = "MODF";
// constexpr char TEXT_MULD[] = "MULD";
// constexpr char TEXT_MULF[] = "MULF";
// constexpr char TEXT_NEGD[] = "NEGD";
// constexpr char TEXT_NEGF[] = "NEGF";
constexpr   char TEXT_SETD[]  = "SETD";
constexpr   char TEXT_SETF[]  = "SETF";
constexpr   char TEXT_SETI[]  = "SETI";
// constexpr   char TEXT_SETL[]  = "SETL";
constexpr   char TEXT_STCDF[] = "STCDF";
constexpr   char TEXT_STCDI[] = "STCDI";
constexpr   char TEXT_STCDL[] = "STCDL";
constexpr   char TEXT_STCFD[] = "STCFD";
constexpr   char TEXT_STCFI[] = "STCFI";
constexpr   char TEXT_STCFL[] = "STCFL";
// constexpr char TEXT_STD[]  = "STD";
constexpr   char TEXT_STEXP[] = "STEXP";
// constexpr char TEXT_STF[]  = "STF";
constexpr   char TEXT_STFPS[] = "STFPS";
// constexpr char TEXT_STST[] = "STST";
// constexpr char TEXT_SUBD[] = "SUBD";
// constexpr char TEXT_SUBF[] = "SUBF";
// constexpr char TEXT_TSTD[] = "TSTD";
// constexpr char TEXT_TSTF[] = "TSTF";

// constexpr char TEXT_REG_AC0[] = "AC0";
// constexpr char TEXT_REG_AC1[] = "AC1";
constexpr char TEXT_REG_AC2[] = "AC2";
constexpr char TEXT_REG_AC3[] = "AC3";
constexpr char TEXT_REG_AC4[] = "AC4";
constexpr char TEXT_REG_AC5[] = "AC5";
// constexpr char TEXT_REG_PC[]  = "PC";
// constexpr char TEXT_REG_R0[]  = "R0";
// constexpr char TEXT_REG_R1[]  = "R1";
// constexpr char TEXT_REG_R2[]  = "R2";
// constexpr char TEXT_REG_R3[]  = "R3";
// constexpr char TEXT_REG_R4[]  = "R4";
// constexpr char TEXT_REG_R5[]  = "R5";
// constexpr char TEXT_REG_R6[]  = "R6";
// constexpr char TEXT_REG_R7[]  = "R7";
// constexpr char TEXT_REG_SP[]  = "SP";

// clang-format on
}  // namespace pdp11
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
