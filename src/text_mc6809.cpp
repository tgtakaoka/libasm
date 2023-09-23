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
const char TEXT_MC6809_LIST[] PROGMEM = "MC6809, HD6309";
const char TEXT_CPU_MC6809[] PROGMEM = "MC6809";
const char TEXT_CPU_HD6309[] PROGMEM = "HD6309";
const char TEXT_CPU_6809[] PROGMEM = "6809";
const char TEXT_CPU_6309[] PROGMEM = "6309";

// const char TEXT_ABX[]   PROGMEM = "ABX";
// const char TEXT_ADCA[]  PROGMEM = "ADCA";
// const char TEXT_ADCB[]  PROGMEM = "ADCB";
// const char TEXT_ADDA[]  PROGMEM = "ADDA";
// const char TEXT_ADDB[]  PROGMEM = "ADDB";
// const char TEXT_ADDD[]  PROGMEM = "ADDD";
// const char TEXT_ANDA[]  PROGMEM = "ANDA";
// const char TEXT_ANDB[]  PROGMEM = "ANDB";
const char TEXT_ANDCC[] PROGMEM = "ANDCC";
// const char TEXT_ASLA[]  PROGMEM = "ASLA";
// const char TEXT_ASLB[]  PROGMEM = "ASLB";
// const char TEXT_ASL[]   PROGMEM = "ASL";
// const char TEXT_ASRA[]  PROGMEM = "ASRA";
// const char TEXT_ASRB[]  PROGMEM = "ASRB";
// const char TEXT_ASR[]   PROGMEM = "ASR";
// const char TEXT_BCC[]   PROGMEM = "BCC";
// const char TEXT_BCS[]   PROGMEM = "BCS";
// const char TEXT_BEQ[]   PROGMEM = "BEQ";
// const char TEXT_BGE[]   PROGMEM = "BGE";
// const char TEXT_BGT[]   PROGMEM = "BGT";
// const char TEXT_BHI[]   PROGMEM = "BHI";
// const char TEXT_BHS[]   PROGMEM = "BHS";
// const char TEXT_BITA[]  PROGMEM = "BITA";
// const char TEXT_BITB[]  PROGMEM = "BITB";
// const char TEXT_BLE[]   PROGMEM = "BLE";
// const char TEXT_BLO[]   PROGMEM = "BLO";
// const char TEXT_BLS[]   PROGMEM = "BLS";
// const char TEXT_BLT[]   PROGMEM = "BLT";
// const char TEXT_BMI[]   PROGMEM = "BMI";
// const char TEXT_BNE[]   PROGMEM = "BNE";
// const char TEXT_BPL[]   PROGMEM = "BPL";
// const char TEXT_BRA[]   PROGMEM = "BRA";
// const char TEXT_BRN[]   PROGMEM = "BRN";
// const char TEXT_BSR[]   PROGMEM = "BSR";
// const char TEXT_BVC[]   PROGMEM = "BVC";
// const char TEXT_BVS[]   PROGMEM = "BVS";
// const char TEXT_CLRA[]  PROGMEM = "CLRA";
// const char TEXT_CLRB[]  PROGMEM = "CLRB";
// const char TEXT_CLR[]   PROGMEM = "CLR";
// const char TEXT_CMPA[]  PROGMEM = "CMPA";
// const char TEXT_CMPB[]  PROGMEM = "CMPB";
// const char TEXT_CMPD[]  PROGMEM = "CMPD";
// const char TEXT_CMPS[]  PROGMEM = "CMPS";
const char TEXT_CMPU[]  PROGMEM = "CMPU";
const char TEXT_CMPX[]  PROGMEM = "CMPX";
const char TEXT_CMPY[]  PROGMEM = "CMPY";
// const char TEXT_COMA[]  PROGMEM = "COMA";
// const char TEXT_COMB[]  PROGMEM = "COMB";
// const char TEXT_COM[]   PROGMEM = "COM";
const char TEXT_CWAI[]  PROGMEM = "CWAI";
// const char TEXT_DAA[]   PROGMEM = "DAA";
// const char TEXT_DECA[]  PROGMEM = "DECA";
// const char TEXT_DECB[]  PROGMEM = "DECB";
// const char TEXT_DEC[]   PROGMEM = "DEC";
// const char TEXT_EORA[]  PROGMEM = "EORA";
// const char TEXT_EORB[]  PROGMEM = "EORB";
// const char TEXT_EXG[]   PROGMEM = "EXG";
// const char TEXT_INCA[]  PROGMEM = "INCA";
// const char TEXT_INCB[]  PROGMEM = "INCB";
// const char TEXT_INC[]   PROGMEM = "INC";
// const char TEXT_JMP[]   PROGMEM = "JMP";
// const char TEXT_JSR[]   PROGMEM = "JSR";
const char TEXT_LBCC[]  PROGMEM = "LBCC";
const char TEXT_LBCS[]  PROGMEM = "LBCS";
const char TEXT_LBEQ[]  PROGMEM = "LBEQ";
    // const char TEXT_LBGE[]  PROGMEM = "LBGE";
const char TEXT_LBGT[]  PROGMEM = "LBGT";
const char TEXT_LBHI[]  PROGMEM = "LBHI";
const char TEXT_LBHS[]  PROGMEM = "LBHS";
const char TEXT_LBLE[]  PROGMEM = "LBLE";
const char TEXT_LBLO[]  PROGMEM = "LBLO";
const char TEXT_LBLS[]  PROGMEM = "LBLS";
const char TEXT_LBLT[]  PROGMEM = "LBLT";
const char TEXT_LBMI[]  PROGMEM = "LBMI";
const char TEXT_LBNE[]  PROGMEM = "LBNE";
const char TEXT_LBPL[]  PROGMEM = "LBPL";
const char TEXT_LBRA[]  PROGMEM = "LBRA";
const char TEXT_LBRN[]  PROGMEM = "LBRN";
const char TEXT_LBSR[]  PROGMEM = "LBSR";
const char TEXT_LBVC[]  PROGMEM = "LBVC";
const char TEXT_LBVS[]  PROGMEM = "LBVS";
// const char TEXT_LDA[]   PROGMEM = "LDA";
// const char TEXT_LDB[]   PROGMEM = "LDB";
// const char TEXT_LDD[]   PROGMEM = "LDD";
// const char TEXT_LDS[]   PROGMEM = "LDS";
const char TEXT_LDU[]   PROGMEM = "LDU";
// const char TEXT_LDX[]   PROGMEM = "LDX";
// const char TEXT_LDY[]   PROGMEM = "LDY";
const char TEXT_LEAS[]  PROGMEM = "LEAS";
const char TEXT_LEAU[]  PROGMEM = "LEAU";
const char TEXT_LEAX[]  PROGMEM = "LEAX";
const char TEXT_LEAY[]  PROGMEM = "LEAY";
// const char TEXT_LSLA[]  PROGMEM = "LSLA";
// const char TEXT_LSLB[]  PROGMEM = "LSLB";
// const char TEXT_LSL[]   PROGMEM = "LSL";
// const char TEXT_LSRA[]  PROGMEM = "LSRA";
// const char TEXT_LSRB[]  PROGMEM = "LSRB";
// const char TEXT_LSR[]   PROGMEM = "LSR";
// const char TEXT_MUL[]   PROGMEM = "MUL";
// const char TEXT_NEGA[]  PROGMEM = "NEGA";
// const char TEXT_NEGB[]  PROGMEM = "NEGB";
// const char TEXT_NEG[]   PROGMEM = "NEG";
// const char TEXT_NOP[]   PROGMEM = "NOP";
// const char TEXT_ORA[]   PROGMEM = "ORA";
// const char TEXT_ORB[]   PROGMEM = "ORB";
const char TEXT_ORCC[]  PROGMEM = "ORCC";
const char TEXT_PSHS[]  PROGMEM = "PSHS";
const char TEXT_PSHU[]  PROGMEM = "PSHU";
const char TEXT_PULS[]  PROGMEM = "PULS";
const char TEXT_PULU[]  PROGMEM = "PULU";
// const char TEXT_ROLA[]  PROGMEM = "ROLA";
// const char TEXT_ROLB[]  PROGMEM = "ROLB";
// const char TEXT_ROL[]   PROGMEM = "ROL";
// const char TEXT_RORA[]  PROGMEM = "RORA";
// const char TEXT_RORB[]  PROGMEM = "RORB";
// const char TEXT_ROR[]   PROGMEM = "ROR";
// const char TEXT_RTI[]   PROGMEM = "RTI";
// const char TEXT_RTS[]   PROGMEM = "RTS";
// const char TEXT_SBCA[]  PROGMEM = "SBCA";
// const char TEXT_SBCB[]  PROGMEM = "SBCB";
// const char TEXT_SEX[]   PROGMEM = "SEX";
// const char TEXT_STA[]   PROGMEM = "STA";
// const char TEXT_STB[]   PROGMEM = "STB";
// const char TEXT_STD[]   PROGMEM = "STD";
// const char TEXT_STS[]   PROGMEM = "STS";
const char TEXT_STU[]   PROGMEM = "STU";
// const char TEXT_STX[]   PROGMEM = "STX";
// const char TEXT_STY[]   PROGMEM = "STY";
// const char TEXT_SUBA[]  PROGMEM = "SUBA";
// const char TEXT_SUBB[]  PROGMEM = "SUBB";
// const char TEXT_SUBD[]  PROGMEM = "SUBD";
const char TEXT_SWI2[]  PROGMEM = "SWI2";
const char TEXT_SWI3[]  PROGMEM = "SWI3";
// const char TEXT_SWI[]   PROGMEM = "SWI";
const char TEXT_SYNC[]  PROGMEM = "SYNC";
const char TEXT_TFR[]   PROGMEM = "TFR";
// const char TEXT_TSTA[]  PROGMEM = "TSTA";
// const char TEXT_TSTB[]  PROGMEM = "TSTB";
// const char TEXT_TST[]   PROGMEM = "TST";

// HD6309

const char TEXT_ADCD[]  PROGMEM = "ADCD";
const char TEXT_ADCR[]  PROGMEM = "ADCR";
const char TEXT_ADDE[]  PROGMEM = "ADDE";
// const char TEXT_ADDF[]  PROGMEM = "ADDF";
// const char TEXT_ADDR[]  PROGMEM = "ADDR";
// const char TEXT_ADDW[]  PROGMEM = "ADDW";
// const char TEXT_AIM[]   PROGMEM = "AIM";
// const char TEXT_ANDD[]  PROGMEM = "ANDD";
// const char TEXT_ANDR[]  PROGMEM = "ANDR";
// const char TEXT_ASLD[]  PROGMEM = "ASLD";
const char TEXT_ASRD[]  PROGMEM = "ASRD";
// const char TEXT_BAND[]  PROGMEM = "BAND";
const char TEXT_BEOR[]  PROGMEM = "BEOR";
const char TEXT_BIAND[] PROGMEM = "BIAND";
const char TEXT_BIEOR[] PROGMEM = "BIEOR";
const char TEXT_BIOR[]  PROGMEM = "BIOR";
const char TEXT_BITD[]  PROGMEM = "BITD";
const char TEXT_BITMD[] PROGMEM = "BITMD";
// const char TEXT_BOR[]   PROGMEM = "BOR";
const char TEXT_CLRD[]  PROGMEM = "CLRD";
const char TEXT_CLRE[]  PROGMEM = "CLRE";
const char TEXT_CLRF[]  PROGMEM = "CLRF";
const char TEXT_CLRW[]  PROGMEM = "CLRW";
const char TEXT_CMPE[]  PROGMEM = "CMPE";
// const char TEXT_CMPF[]  PROGMEM = "CMPF";
const char TEXT_CMPR[]  PROGMEM = "CMPR";
// const char TEXT_CMPW[]  PROGMEM = "CMPW";
// const char TEXT_COMD[]  PROGMEM = "COMD";
const char TEXT_COME[]  PROGMEM = "COME";
const char TEXT_COMF[]  PROGMEM = "COMF";
// const char TEXT_COMW[]  PROGMEM = "COMW";
// const char TEXT_DECD[]  PROGMEM = "DECD";
const char TEXT_DECE[]  PROGMEM = "DECE";
const char TEXT_DECF[]  PROGMEM = "DECF";
// const char TEXT_DECW[]  PROGMEM = "DECW";
// const char TEXT_DIVD[]  PROGMEM = "DIVD";
const char TEXT_DIVQ[]  PROGMEM = "DIVQ";
// const char TEXT_EIM[]   PROGMEM = "EIM";
const char TEXT_EORD[]  PROGMEM = "EORD";
// const char TEXT_EORR[]  PROGMEM = "EORR";
const char TEXT_INCD[]  PROGMEM = "INCD";
const char TEXT_INCE[]  PROGMEM = "INCE";
const char TEXT_INCF[]  PROGMEM = "INCF";
// const char TEXT_INCW[]  PROGMEM = "INCW";
const char TEXT_LDBT[]  PROGMEM = "LDBT";
// const char TEXT_LDE[]   PROGMEM = "LDE";
const char TEXT_LDF[]   PROGMEM = "LDF";
const char TEXT_LDMD[]  PROGMEM = "LDMD";
const char TEXT_LDQ[]   PROGMEM = "LDQ";
// const char TEXT_LDW[]   PROGMEM = "LDW";
// const char TEXT_LSLD[]  PROGMEM = "LSLD";
// const char TEXT_LSRD[]  PROGMEM = "LSRD";
const char TEXT_LSRW[]  PROGMEM = "LSRW";
// const char TEXT_MULD[]  PROGMEM = "MULD";
// const char TEXT_NEGD[]  PROGMEM = "NEGD";
// const char TEXT_OIM[]   PROGMEM = "OIM";
// const char TEXT_ORD[]   PROGMEM = "ORD";
// const char TEXT_ORR[]   PROGMEM = "ORR";
const char TEXT_PSHSW[] PROGMEM = "PSHSW";
const char TEXT_PSHUW[] PROGMEM = "PSHUW";
const char TEXT_PULSW[] PROGMEM = "PULSW";
const char TEXT_PULUW[] PROGMEM = "PULUW";
const char TEXT_ROLD[]  PROGMEM = "ROLD";
const char TEXT_ROLW[]  PROGMEM = "ROLW";
const char TEXT_RORD[]  PROGMEM = "RORD";
const char TEXT_RORW[]  PROGMEM = "RORW";
// const char TEXT_SBCD[]  PROGMEM = "SBCD";
const char TEXT_SBCR[]  PROGMEM = "SBCR";
const char TEXT_SEXW[]  PROGMEM = "SEXW";
const char TEXT_STBT[]  PROGMEM = "STBT";
const char TEXT_STE[]   PROGMEM = "STE";
const char TEXT_STF[]   PROGMEM = "STF";
const char TEXT_STQ[]   PROGMEM = "STQ";
const char TEXT_STW[]   PROGMEM = "STW";
const char TEXT_SUBE[]  PROGMEM = "SUBE";
// const char TEXT_SUBF[]  PROGMEM = "SUBF";
// const char TEXT_SUBR[]  PROGMEM = "SUBR";
// const char TEXT_SUBW[]  PROGMEM = "SUBW";
const char TEXT_TFM[]   PROGMEM = "TFM";
// const char TEXT_TIM[]   PROGMEM = "TIM";
const char TEXT_TSTD[]  PROGMEM = "TSTD";
const char TEXT_TSTE[]  PROGMEM = "TSTE";
const char TEXT_TSTF[]  PROGMEM = "TSTF";
const char TEXT_TSTW[]  PROGMEM = "TSTW";

// clang-format off
// const char TEXT_REG_A[] PROGMEM   = "A";
// const char TEXT_REG_B[] PROGMEM   = "B";
// const char TEXT_REG_D[] PROGMEM   = "D";
const char TEXT_REG_X[] PROGMEM   = "X";
const char TEXT_REG_Y[] PROGMEM   = "Y";
const char TEXT_REG_U[] PROGMEM   = "U";
// const char TEXT_REG_S[] PROGMEM   = "S";
// const char TEXT_REG_PC[] PROGMEM  = "PC";
const char TEXT_REG_CC[] PROGMEM  = "CC";
const char TEXT_REG_DP[] PROGMEM  = "DP";
const char TEXT_REG_PCR[] PROGMEM = "PCR";
// const char TEXT_REG_W[] PROGMEM   = "W";
// const char TEXT_REG_E[] PROGMEM   = "E";
const char TEXT_REG_F[] PROGMEM   = "F";
const char TEXT_REG_V[] PROGMEM   = "V";
const char TEXT_REG_Z[] PROGMEM   = "Z";
const char TEXT_REG_0[] PROGMEM   = "0";
// clang-format on
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
