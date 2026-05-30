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

constexpr char TEXT_PDP11_LIST[] PROGMEM = "J11, T11";
constexpr char TEXT_CPU_DCJ11[] PROGMEM = "DCJ11";
constexpr char TEXT_CPU_DCT11[] PROGMEM = "DCT11";
constexpr char TEXT_CPU_J11[]   PROGMEM = "J11";
constexpr char TEXT_CPU_T11[]   PROGMEM = "T11";

// PDP11 base instructions
// constexpr char TEXT_ADC[]   PROGMEM = "ADC";
// constexpr char TEXT_ADCB[]  PROGMEM = "ADCB";
// constexpr char TEXT_ADD[]   PROGMEM = "ADD";
// constexpr char TEXT_ASL[]   PROGMEM = "ASL";
// constexpr char TEXT_ASLB[]  PROGMEM = "ASLB";
// constexpr char TEXT_ASR[]   PROGMEM = "ASR";
// constexpr char TEXT_ASRB[]  PROGMEM = "ASRB";
// constexpr char TEXT_BCC[]   PROGMEM = "BCC";
// constexpr char TEXT_BCS[]   PROGMEM = "BCS";
// constexpr char TEXT_BEQ[]   PROGMEM = "BEQ";
// constexpr char TEXT_BGE[]   PROGMEM = "BGE";
// constexpr char TEXT_BGT[]   PROGMEM = "BGT";
// constexpr char TEXT_BHI[]   PROGMEM = "BHI";
constexpr char TEXT_BHIS[] PROGMEM = "BHIS";
constexpr char TEXT_BIC[]  PROGMEM = "BIC";
// constexpr char TEXT_BICB[]  PROGMEM = "BICB";
constexpr char TEXT_BIS[]  PROGMEM = "BIS";
constexpr char TEXT_BISB[] PROGMEM = "BISB";
// constexpr char TEXT_BIT[]   PROGMEM = "BIT";
// constexpr char TEXT_BITB[]  PROGMEM = "BITB";
// constexpr char TEXT_BLE[]   PROGMEM = "BLE";
// constexpr char TEXT_BLO[]   PROGMEM = "BLO";
constexpr char TEXT_BLOS[] PROGMEM = "BLOS";
// constexpr char TEXT_BLT[]   PROGMEM = "BLT";
// constexpr char TEXT_BMI[]   PROGMEM = "BMI";
// constexpr char TEXT_BNE[]   PROGMEM = "BNE";
// constexpr char TEXT_BPL[]   PROGMEM = "BPL";
// constexpr char TEXT_BPT[]   PROGMEM = "BPT";
// constexpr char TEXT_BR[]    PROGMEM = "BR";
// constexpr char TEXT_BVC[]   PROGMEM = "BVC";
// constexpr char TEXT_BVS[]   PROGMEM = "BVS";
constexpr char TEXT_CCC[]  PROGMEM = "CCC";
// constexpr char TEXT_CLC[]   PROGMEM = "CLC";
constexpr char TEXT_CLN[]  PROGMEM = "CLN";
// constexpr char TEXT_CLR[]   PROGMEM = "CLR";
// constexpr char TEXT_CLRB[]  PROGMEM = "CLRB";
// constexpr char TEXT_CLV[]   PROGMEM = "CLV";
constexpr char TEXT_CLZ[]  PROGMEM = "CLZ";
// constexpr char TEXT_CMP[]   PROGMEM = "CMP";
// constexpr char TEXT_CMPB[]  PROGMEM = "CMPB";
// constexpr char TEXT_COM[]   PROGMEM = "COM";
// constexpr char TEXT_COMB[]  PROGMEM = "COMB";
// constexpr char TEXT_DEC[]   PROGMEM = "DEC";
// constexpr char TEXT_DECB[]  PROGMEM = "DECB";
constexpr char TEXT_EMT[]  PROGMEM = "EMT";
// constexpr char TEXT_HALT[]  PROGMEM = "HALT";
// constexpr char TEXT_INC[]   PROGMEM = "INC";
// constexpr char TEXT_INCB[]  PROGMEM = "INCB";
// constexpr char TEXT_IOT[]   PROGMEM = "IOT";
// constexpr char TEXT_JMP[]   PROGMEM = "JMP";
// constexpr char TEXT_JSR[]   PROGMEM = "JSR";
constexpr char TEXT_MFPS[] PROGMEM = "MFPS";
constexpr char TEXT_MFPT[] PROGMEM = "MFPT";
// constexpr char TEXT_MOV[]   PROGMEM = "MOV";
// constexpr char TEXT_MOVB[]  PROGMEM = "MOVB";
constexpr char TEXT_MTPS[] PROGMEM = "MTPS";
// constexpr char TEXT_NEG[]   PROGMEM = "NEG";
// constexpr char TEXT_NEGB[]  PROGMEM = "NEGB";
// constexpr char TEXT_NOP[]   PROGMEM = "NOP";
constexpr char TEXT_PDP[]  PROGMEM = "PDP";
// constexpr char TEXT_RESET[] PROGMEM = "RESET";
// constexpr char TEXT_ROL[]   PROGMEM = "ROL";
// constexpr char TEXT_ROLB[]  PROGMEM = "ROLB";
// constexpr char TEXT_ROR[]   PROGMEM = "ROR";
// constexpr char TEXT_RORB[]  PROGMEM = "RORB";
// constexpr char TEXT_RTI[]   PROGMEM = "RTI";
// constexpr char TEXT_RTS[]   PROGMEM = "RTS";
constexpr char TEXT_RTT[]  PROGMEM = "RTT";
// constexpr char TEXT_SBC[]   PROGMEM = "SBC";
// constexpr char TEXT_SBCB[]  PROGMEM = "SBCB";
// constexpr char TEXT_SCC[]   PROGMEM = "SCC";
// constexpr char TEXT_SEC[]   PROGMEM = "SEC";
constexpr char TEXT_SEN[]  PROGMEM = "SEN";
// constexpr char TEXT_SEV[]   PROGMEM = "SEV";
constexpr char TEXT_SEZ[]  PROGMEM = "SEZ";
constexpr char TEXT_SOB[]  PROGMEM = "SOB";
// constexpr char TEXT_SUB[]   PROGMEM = "SUB";
constexpr char TEXT_SWAB[] PROGMEM = "SWAB";
// constexpr char TEXT_SXT[]   PROGMEM = "SXT";
// constexpr char TEXT_TRAP[]  PROGMEM = "TRAP";
// constexpr char TEXT_TST[]   PROGMEM = "TST";
// constexpr char TEXT_TSTB[]  PROGMEM = "TSTB";
// constexpr char TEXT_WAIT[]  PROGMEM = "WAIT";
// constexpr char TEXT_XOR[]   PROGMEM = "XOR";

// PDP-11 return from varargs subroutine
// constexpr char TEXT_MARK[]  PROGMEM = "MARK";

// PDP-11 extended instruction set (EIS)
// constexpr char TEXT_ASH[]   PROGMEM = "ASH";
constexpr char TEXT_ASHC[]  PROGMEM = "ASHC";
// constexpr char TEXT_DIV[]   PROGMEM = "DIV";
// constexpr char TEXT_MUL[]   PROGMEM = "MUL";

// PDP-11 memory management
constexpr char TEXT_MFPD[] PROGMEM = "MFPD";
constexpr char TEXT_MFPI[] PROGMEM = "MFPI";
constexpr char TEXT_MTPD[] PROGMEM = "MTPD";
constexpr char TEXT_MTPI[] PROGMEM = "MTPI";

// DCJ11
constexpr char TEXT_CSM[]    PROGMEM = "CSM";
// constexpr char TEXT_SPL[] PROGMEM = "SPL";
constexpr char TEXT_TSTSET[] PROGMEM = "TSTSET";
constexpr char TEXT_WRTLCK[] PROGMEM = "WRTLCK";

// PDP-11 floating point processor (FP11)
// constexpr char TEXT_ABSD[] PROGMEM = "ABSD";
// constexpr char TEXT_ABSF[] PROGMEM = "ABSF";
// constexpr char TEXT_ADDD[] PROGMEM = "ADDD";
// constexpr char TEXT_ADDF[] PROGMEM = "ADDF";
constexpr   char TEXT_CFCC[]  PROGMEM = "CFCC";
// constexpr char TEXT_CLRD[] PROGMEM = "CLRD";
// constexpr char TEXT_CLRF[] PROGMEM = "CLRF";
// constexpr char TEXT_CMPD[] PROGMEM = "CMPD";
// constexpr char TEXT_CMPF[] PROGMEM = "CMPF";
// constexpr char TEXT_DIVD[] PROGMEM = "DIVD";
// constexpr char TEXT_DIVF[] PROGMEM = "DIVF";
constexpr   char TEXT_LDCDF[] PROGMEM = "LDCDF";
constexpr   char TEXT_LDCFD[] PROGMEM = "LDCFD";
constexpr   char TEXT_LDCID[] PROGMEM = "LDCID";
constexpr   char TEXT_LDCIF[] PROGMEM = "LDCIF";
constexpr   char TEXT_LDCLD[] PROGMEM = "LDCLD";
constexpr   char TEXT_LDCLF[] PROGMEM = "LDCLF";
// constexpr char TEXT_LDD[]  PROGMEM = "LDD";
constexpr   char TEXT_LDEXP[] PROGMEM = "LDEXP";
//constexpr char TEXT_LDF[]   PROGMEM = "LDF";
constexpr   char TEXT_LDFPS[] PROGMEM = "LDFPS";
// constexpr char TEXT_MODD[] PROGMEM = "MODD";
constexpr   char TEXT_MODF[]  PROGMEM = "MODF";
// constexpr char TEXT_MULD[] PROGMEM = "MULD";
// constexpr char TEXT_MULF[] PROGMEM = "MULF";
// constexpr char TEXT_NEGD[] PROGMEM = "NEGD";
// constexpr char TEXT_NEGF[] PROGMEM = "NEGF";
constexpr   char TEXT_SETD[]  PROGMEM = "SETD";
constexpr   char TEXT_SETF[]  PROGMEM = "SETF";
constexpr   char TEXT_SETI[]  PROGMEM = "SETI";
// constexpr   char TEXT_SETL[]  PROGMEM = "SETL";
constexpr   char TEXT_STCDF[] PROGMEM = "STCDF";
constexpr   char TEXT_STCDI[] PROGMEM = "STCDI";
constexpr   char TEXT_STCDL[] PROGMEM = "STCDL";
constexpr   char TEXT_STCFD[] PROGMEM = "STCFD";
constexpr   char TEXT_STCFI[] PROGMEM = "STCFI";
constexpr   char TEXT_STCFL[] PROGMEM = "STCFL";
// constexpr char TEXT_STD[]  PROGMEM = "STD";
constexpr   char TEXT_STEXP[] PROGMEM = "STEXP";
// constexpr char TEXT_STF[]  PROGMEM = "STF";
constexpr   char TEXT_STFPS[] PROGMEM = "STFPS";
// constexpr char TEXT_STST[] PROGMEM = "STST";
// constexpr char TEXT_SUBD[] PROGMEM = "SUBD";
// constexpr char TEXT_SUBF[] PROGMEM = "SUBF";
// constexpr char TEXT_TSTD[] PROGMEM = "TSTD";
// constexpr char TEXT_TSTF[] PROGMEM = "TSTF";

// constexpr char TEXT_REG_AC0[] PROGMEM = "AC0";
// constexpr char TEXT_REG_AC1[] PROGMEM = "AC1";
constexpr char TEXT_REG_AC2[] PROGMEM = "AC2";
constexpr char TEXT_REG_AC3[] PROGMEM = "AC3";
constexpr char TEXT_REG_AC4[] PROGMEM = "AC4";
constexpr char TEXT_REG_AC5[] PROGMEM = "AC5";
// constexpr char TEXT_REG_PC[]  PROGMEM = "PC";
// constexpr char TEXT_REG_R0[]  PROGMEM = "R0";
// constexpr char TEXT_REG_R1[]  PROGMEM = "R1";
// constexpr char TEXT_REG_R2[]  PROGMEM = "R2";
// constexpr char TEXT_REG_R3[]  PROGMEM = "R3";
// constexpr char TEXT_REG_R4[]  PROGMEM = "R4";
// constexpr char TEXT_REG_R5[]  PROGMEM = "R5";
// constexpr char TEXT_REG_R6[]  PROGMEM = "R6";
// constexpr char TEXT_REG_R7[]  PROGMEM = "R7";
// constexpr char TEXT_REG_SP[]  PROGMEM = "SP";

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
