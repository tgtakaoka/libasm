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

#include "text_mc6809.h"

namespace libasm {
namespace text {
namespace mc6809 {
// clang-format off

constexpr char TEXT_MC6809_LIST[] = "MC6809, HD6309";
constexpr char TEXT_CPU_6809[]   = "6809";
constexpr char TEXT_CPU_6309[]   = "6309";
constexpr char TEXT_CPU_HD6309[] = "HD6309";
constexpr char TEXT_CPU_MC6809[] = "MC6809";

// MC6809
// constexpr char TEXT_ABX[]   = "ABX";
// constexpr char TEXT_ADCA[]  = "ADCA";
// constexpr char TEXT_ADCB[]  = "ADCB";
// constexpr char TEXT_ADDA[]  = "ADDA";
// constexpr char TEXT_ADDB[]  = "ADDB";
// constexpr char TEXT_ADDD[]  = "ADDD";
// constexpr char TEXT_ANDA[]  = "ANDA";
// constexpr char TEXT_ANDB[]  = "ANDB";
// constexpr char TEXT_ANDCC[] = "ANDCC";
// constexpr char TEXT_ASL[]   = "ASL";
// constexpr char TEXT_ASLA[]  = "ASLA";
// constexpr char TEXT_ASLB[]  = "ASLB";
// constexpr char TEXT_ASR[]   = "ASR";
// constexpr char TEXT_ASRA[]  = "ASRA";
// constexpr char TEXT_ASRB[]  = "ASRB";
// constexpr char TEXT_BCC[]   = "BCC";
// constexpr char TEXT_BCS[]   = "BCS";
// constexpr char TEXT_BEQ[]   = "BEQ";
// constexpr char TEXT_BGE[]   = "BGE";
// constexpr char TEXT_BGT[]   = "BGT";
// constexpr char TEXT_BHI[]   = "BHI";
// constexpr char TEXT_BHS[]   = "BHS";
// constexpr char TEXT_BITA[]  = "BITA";
// constexpr char TEXT_BITB[]  = "BITB";
// constexpr char TEXT_BLE[]   = "BLE";
// constexpr char TEXT_BLO[]   = "BLO";
// constexpr char TEXT_BLS[]   = "BLS";
// constexpr char TEXT_BLT[]   = "BLT";
// constexpr char TEXT_BMI[]   = "BMI";
// constexpr char TEXT_BNE[]   = "BNE";
// constexpr char TEXT_BPL[]   = "BPL";
// constexpr char TEXT_BRA[]   = "BRA";
// constexpr char TEXT_BRN[]   = "BRN";
// constexpr char TEXT_BSR[]   = "BSR";
// constexpr char TEXT_BVC[]   = "BVC";
// constexpr char TEXT_BVS[]   = "BVS";
// constexpr char TEXT_CLR[]   = "CLR";
// constexpr char TEXT_CLRA[]  = "CLRA";
// constexpr char TEXT_CLRB[]  = "CLRB";
// constexpr char TEXT_CMPA[]  = "CMPA";
// constexpr char TEXT_CMPB[]  = "CMPB";
// constexpr char TEXT_CMPD[]  = "CMPD";
// constexpr char TEXT_CMPS[]  = "CMPS";
constexpr char TEXT_CMPU[] = "CMPU";
constexpr char TEXT_CMPX[] = "CMPX";
constexpr char TEXT_CMPY[] = "CMPY";
// constexpr char TEXT_COM[]   = "COM";
// constexpr char TEXT_COMA[]  = "COMA";
// constexpr char TEXT_COMB[]  = "COMB";
constexpr char TEXT_CWAI[] = "CWAI";
// constexpr char TEXT_DAA[]   = "DAA";
// constexpr char TEXT_DEC[]   = "DEC";
// constexpr char TEXT_DECA[]  = "DECA";
// constexpr char TEXT_DECB[]  = "DECB";
// constexpr char TEXT_EORA[]  = "EORA";
// constexpr char TEXT_EORB[]  = "EORB";
// constexpr char TEXT_EXG[]   = "EXG";
// constexpr char TEXT_INC[]   = "INC";
// constexpr char TEXT_INCA[]  = "INCA";
// constexpr char TEXT_INCB[]  = "INCB";
// constexpr char TEXT_JMP[]   = "JMP";
// constexpr char TEXT_JSR[]   = "JSR";
// constexpr char TEXT_LBCC[]  = "LBCC";
// constexpr char TEXT_LBCS[]  = "LBCS";
// constexpr char TEXT_LBEQ[]  = "LBEQ";
// constexpr char TEXT_LBGE[]  = "LBGE";
// constexpr char TEXT_LBGT[]  = "LBGT";
// constexpr char TEXT_LBHI[]  = "LBHI";
// constexpr char TEXT_LBHS[]  = "LBHS";
// constexpr char TEXT_LBLE[]  = "LBLE";
// constexpr char TEXT_LBLO[]  = "LBLO";
// constexpr char TEXT_LBLS[]  = "LBLS";
// constexpr char TEXT_LBLT[]  = "LBLT";
// constexpr char TEXT_LBMI[]  = "LBMI";
// constexpr char TEXT_LBNE[]  = "LBNE";
// constexpr char TEXT_LBPL[]  = "LBPL";
// constexpr char TEXT_LBRA[]  = "LBRA";
// constexpr char TEXT_LBRN[]  = "LBRN";
// constexpr char TEXT_LBSR[]  = "LBSR";
// constexpr char TEXT_LBVC[]  = "LBVC";
// constexpr char TEXT_LBVS[]  = "LBVS";
// constexpr char TEXT_LDA[]   = "LDA";
// constexpr char TEXT_LDB[]   = "LDB";
// constexpr char TEXT_LDD[]   = "LDD";
// constexpr char TEXT_LDS[]   = "LDS";
constexpr char TEXT_LDU[]  = "LDU";
// constexpr char TEXT_LDX[]   = "LDX";
// constexpr char TEXT_LDY[]   = "LDY";
// constexpr char TEXT_LEAS[]  = "LEAS";
constexpr char TEXT_LEAU[] = "LEAU";
// constexpr char TEXT_LEAX[]  = "LEAX";
// constexpr char TEXT_LEAY[]  = "LEAY";
// constexpr char TEXT_LSL[]   = "LSL";
// constexpr char TEXT_LSLA[]  = "LSLA";
// constexpr char TEXT_LSLB[]  = "LSLB";
// constexpr char TEXT_LSR[]   = "LSR";
// constexpr char TEXT_LSRA[]  = "LSRA";
// constexpr char TEXT_LSRB[]  = "LSRB";
// constexpr char TEXT_MUL[]   = "MUL";
// constexpr char TEXT_NEG[]   = "NEG";
// constexpr char TEXT_NEGA[]  = "NEGA";
// constexpr char TEXT_NEGB[]  = "NEGB";
// constexpr char TEXT_NOP[]   = "NOP";
// constexpr char TEXT_ORA[]   = "ORA";
// constexpr char TEXT_ORB[]   = "ORB";
// constexpr char TEXT_ORCC[]  = "ORCC";
constexpr char TEXT_PSHS[] = "PSHS";
constexpr char TEXT_PSHU[] = "PSHU";
constexpr char TEXT_PULS[] = "PULS";
constexpr char TEXT_PULU[] = "PULU";
// constexpr char TEXT_ROL[]   = "ROL";
// constexpr char TEXT_ROLA[]  = "ROLA";
// constexpr char TEXT_ROLB[]  = "ROLB";
// constexpr char TEXT_ROR[]   = "ROR";
// constexpr char TEXT_RORA[]  = "RORA";
// constexpr char TEXT_RORB[]  = "RORB";
// constexpr char TEXT_RTI[]   = "RTI";
// constexpr char TEXT_RTS[]   = "RTS";
// constexpr char TEXT_SBCA[]  = "SBCA";
// constexpr char TEXT_SBCB[]  = "SBCB";
// constexpr char TEXT_SEX[]   = "SEX";
// constexpr char TEXT_STA[]   = "STA";
// constexpr char TEXT_STB[]   = "STB";
// constexpr char TEXT_STD[]   = "STD";
// constexpr char TEXT_STS[]   = "STS";
constexpr char TEXT_STU[]  = "STU";
// constexpr char TEXT_STX[]   = "STX";
// constexpr char TEXT_STY[]   = "STY";
// constexpr char TEXT_SUBA[]  = "SUBA";
// constexpr char TEXT_SUBB[]  = "SUBB";
// constexpr char TEXT_SUBD[]  = "SUBD";
// constexpr char TEXT_SWI[]   = "SWI";
constexpr char TEXT_SWI2[] = "SWI2";
constexpr char TEXT_SWI3[] = "SWI3";
constexpr char TEXT_SYNC[] = "SYNC";
// constexpr char TEXT_TFR[]   = "TFR";
// constexpr char TEXT_TST[]   = "TST";
// constexpr char TEXT_TSTA[]  = "TSTA";
// constexpr char TEXT_TSTB[]  = "TSTB";

// HD6309
// constexpr char TEXT_ADCD[]   = "ADCD";
// constexpr char TEXT_ADCR[]    = "ADCR";
// constexpr char TEXT_ADDE[]   = "ADDE";
// constexpr char TEXT_ADDF[]   = "ADDF";
// constexpr char TEXT_ADDR[]   = "ADDR";
// constexpr char TEXT_ADDW[]   = "ADDW";
// constexpr char TEXT_AIM[]    = "AIM";
// constexpr char TEXT_ANDD[]   = "ANDD";
// constexpr char TEXT_ANDR[]   = "ANDR";
// constexpr char TEXT_ASLD[]   = "ASLD";
// constexpr char TEXT_ASRD[]   = "ASRD";
// constexpr char TEXT_BAND[]   = "BAND";
constexpr char TEXT_BEOR[]    = "BEOR";
// constexpr char TEXT_BIAND[]  = "BIAND";
constexpr char TEXT_BIEOR[]   = "BIEOR";
// constexpr char TEXT_BIOR[]   = "BIOR";
constexpr char TEXT_BITD[]    = "BITD";
constexpr char TEXT_BITMD[]   = "BITMD";
// constexpr char TEXT_BOR[]    = "BOR";
// constexpr char TEXT_CLRD[]   = "CLRD";
// constexpr char TEXT_CLRE[]   = "CLRE";
// constexpr char TEXT_CLRF[]   = "CLRF";
// constexpr char TEXT_CLRW[]   = "CLRW";
constexpr char TEXT_CMPE[]    = "CMPE";
// constexpr char TEXT_CMPF[]   = "CMPF";
// constexpr char TEXT_CMPR[]   = "CMPR";
// constexpr char TEXT_CMPW[]   = "CMPW";
// constexpr char TEXT_COMD[]   = "COMD";
// constexpr char TEXT_COME[]   = "COME";
constexpr char TEXT_COMF[]    = "COMF";
// constexpr char TEXT_COMW[]   = "COMW";
// constexpr char TEXT_DECD[]   = "DECD";
constexpr char TEXT_DECE[]    = "DECE";
// constexpr char TEXT_DECF[]    = "DECF";
// constexpr char TEXT_DECW[]   = "DECW";
// constexpr char TEXT_DIVD[]   = "DIVD";
constexpr char TEXT_DIVQ[]    = "DIVQ";
// constexpr char TEXT_EIM[]    = "EIM";
// constexpr char TEXT_EORD[]   = "EORD";
// constexpr char TEXT_EORR[]   = "EORR";
constexpr char TEXT_INCD[]    = "INCD";
constexpr char TEXT_INCE[]    = "INCE";
// constexpr char TEXT_INCF[]    = "INCF";
// constexpr char TEXT_INCW[]   = "INCW";
constexpr char TEXT_LDBT[]    = "LDBT";
// constexpr char TEXT_LDE[]    = "LDE";
// constexpr char TEXT_LDF[]    = "LDF";
constexpr char TEXT_LDMD[]    = "LDMD";
constexpr char TEXT_LDQ[]     = "LDQ";
// constexpr char TEXT_LDW[]    = "LDW";
// constexpr char TEXT_LSLD[]   = "LSLD";
// constexpr char TEXT_LSRD[]   = "LSRD";
// constexpr char TEXT_LSRW[]   = "LSRW";
// constexpr char TEXT_MULD[]   = "MULD";
// constexpr char TEXT_NEGD[]   = "NEGD";
// constexpr char TEXT_OIM[]    = "OIM";
// constexpr char TEXT_ORD[]    = "ORD";
// constexpr char TEXT_ORR[]    = "ORR";
constexpr char TEXT_PSHSW[]   = "PSHSW";
constexpr char TEXT_PSHUW[]   = "PSHUW";
constexpr char TEXT_PULSW[]   = "PULSW";
constexpr char TEXT_PULUW[]   = "PULUW";
// constexpr char TEXT_REG_A[]  = "A";
// constexpr char TEXT_REG_B[]  = "B";
constexpr char TEXT_REG_CC[]  = "CC";
// constexpr char TEXT_REG_D[]  = "D";
// constexpr char TEXT_REG_DP[] = "DP";
// constexpr char TEXT_REG_PC[] = "PC";
constexpr char TEXT_REG_PCR[] = "PCR";
// constexpr char TEXT_REG_S[]  = "S";
constexpr char TEXT_REG_U[]   = "U";
// constexpr char TEXT_REG_X[]  = "X";
// constexpr char TEXT_REG_Y[]  = "Y";
// constexpr char TEXT_ROLD[]   = "ROLD";
// constexpr char TEXT_ROLW[]   = "ROLW";
// constexpr char TEXT_RORD[]   = "RORD";
// constexpr char TEXT_RORW[]   = "RORW";
// constexpr char TEXT_SBCD[]   = "SBCD";
constexpr char TEXT_SBCR[]    = "SBCR";
constexpr char TEXT_SEXW[]    = "SEXW";
constexpr char TEXT_STBT[]    = "STBT";
// constexpr char TEXT_STE[]    = "STE";
// constexpr char TEXT_STF[]    = "STF";
constexpr char TEXT_STQ[]     = "STQ";
constexpr char TEXT_STW[]     = "STW";
// constexpr char TEXT_SUBE[]   = "SUBE";
// constexpr char TEXT_SUBF[]   = "SUBF";
// constexpr char TEXT_SUBR[]   = "SUBR";
// constexpr char TEXT_SUBW[]   = "SUBW";
constexpr char TEXT_TFM[]     = "TFM";
// constexpr char TEXT_TIM[]    = "TIM";
// constexpr char TEXT_TSTD[]   = "TSTD";
// constexpr char TEXT_TSTE[]   = "TSTE";
// constexpr char TEXT_TSTF[]   = "TSTF";
// constexpr char TEXT_TSTW[]   = "TSTW";

// HD6309
constexpr char TEXT_REG_0[] = "0";
// constexpr char TEXT_REG_E[] = "E";
// constexpr char TEXT_REG_F[] = "F";
constexpr char TEXT_REG_V[] = "V";
// constexpr char TEXT_REG_W[] = "W";
// constexpr char TEXT_REG_Z[] = "Z";

// clang-format on
}  // namespace mc6809
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
