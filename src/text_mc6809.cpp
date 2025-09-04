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

constexpr char TEXT_MC6809_LIST[] PROGMEM = "MC6809, HD6309";
constexpr char TEXT_CPU_6809[]   PROGMEM = "6809";
constexpr char TEXT_CPU_6309[]   PROGMEM = "6309";
constexpr char TEXT_CPU_HD6309[] PROGMEM = "HD6309";
constexpr char TEXT_CPU_MC6809[] PROGMEM = "MC6809";

// MC6809
// constexpr char TEXT_ABX[]   PROGMEM = "ABX";
// constexpr char TEXT_ADCA[]  PROGMEM = "ADCA";
// constexpr char TEXT_ADCB[]  PROGMEM = "ADCB";
// constexpr char TEXT_ADDA[]  PROGMEM = "ADDA";
// constexpr char TEXT_ADDB[]  PROGMEM = "ADDB";
// constexpr char TEXT_ADDD[]  PROGMEM = "ADDD";
// constexpr char TEXT_ANDA[]  PROGMEM = "ANDA";
// constexpr char TEXT_ANDB[]  PROGMEM = "ANDB";
// constexpr char TEXT_ANDCC[] PROGMEM = "ANDCC";
// constexpr char TEXT_ASL[]   PROGMEM = "ASL";
// constexpr char TEXT_ASLA[]  PROGMEM = "ASLA";
// constexpr char TEXT_ASLB[]  PROGMEM = "ASLB";
// constexpr char TEXT_ASR[]   PROGMEM = "ASR";
// constexpr char TEXT_ASRA[]  PROGMEM = "ASRA";
// constexpr char TEXT_ASRB[]  PROGMEM = "ASRB";
// constexpr char TEXT_BCC[]   PROGMEM = "BCC";
// constexpr char TEXT_BCS[]   PROGMEM = "BCS";
// constexpr char TEXT_BEQ[]   PROGMEM = "BEQ";
// constexpr char TEXT_BGE[]   PROGMEM = "BGE";
// constexpr char TEXT_BGT[]   PROGMEM = "BGT";
// constexpr char TEXT_BHI[]   PROGMEM = "BHI";
// constexpr char TEXT_BHS[]   PROGMEM = "BHS";
// constexpr char TEXT_BITA[]  PROGMEM = "BITA";
// constexpr char TEXT_BITB[]  PROGMEM = "BITB";
// constexpr char TEXT_BLE[]   PROGMEM = "BLE";
// constexpr char TEXT_BLO[]   PROGMEM = "BLO";
// constexpr char TEXT_BLS[]   PROGMEM = "BLS";
// constexpr char TEXT_BLT[]   PROGMEM = "BLT";
// constexpr char TEXT_BMI[]   PROGMEM = "BMI";
// constexpr char TEXT_BNE[]   PROGMEM = "BNE";
// constexpr char TEXT_BPL[]   PROGMEM = "BPL";
// constexpr char TEXT_BRA[]   PROGMEM = "BRA";
// constexpr char TEXT_BRN[]   PROGMEM = "BRN";
// constexpr char TEXT_BSR[]   PROGMEM = "BSR";
// constexpr char TEXT_BVC[]   PROGMEM = "BVC";
// constexpr char TEXT_BVS[]   PROGMEM = "BVS";
// constexpr char TEXT_CLR[]   PROGMEM = "CLR";
// constexpr char TEXT_CLRA[]  PROGMEM = "CLRA";
// constexpr char TEXT_CLRB[]  PROGMEM = "CLRB";
// constexpr char TEXT_CMPA[]  PROGMEM = "CMPA";
// constexpr char TEXT_CMPB[]  PROGMEM = "CMPB";
// constexpr char TEXT_CMPD[]  PROGMEM = "CMPD";
// constexpr char TEXT_CMPS[]  PROGMEM = "CMPS";
constexpr char TEXT_CMPU[] PROGMEM = "CMPU";
constexpr char TEXT_CMPX[] PROGMEM = "CMPX";
constexpr char TEXT_CMPY[] PROGMEM = "CMPY";
// constexpr char TEXT_COM[]   PROGMEM = "COM";
// constexpr char TEXT_COMA[]  PROGMEM = "COMA";
// constexpr char TEXT_COMB[]  PROGMEM = "COMB";
constexpr char TEXT_CWAI[] PROGMEM = "CWAI";
// constexpr char TEXT_DAA[]   PROGMEM = "DAA";
// constexpr char TEXT_DEC[]   PROGMEM = "DEC";
// constexpr char TEXT_DECA[]  PROGMEM = "DECA";
// constexpr char TEXT_DECB[]  PROGMEM = "DECB";
// constexpr char TEXT_EORA[]  PROGMEM = "EORA";
// constexpr char TEXT_EORB[]  PROGMEM = "EORB";
// constexpr char TEXT_EXG[]   PROGMEM = "EXG";
// constexpr char TEXT_INC[]   PROGMEM = "INC";
// constexpr char TEXT_INCA[]  PROGMEM = "INCA";
// constexpr char TEXT_INCB[]  PROGMEM = "INCB";
// constexpr char TEXT_JMP[]   PROGMEM = "JMP";
// constexpr char TEXT_JSR[]   PROGMEM = "JSR";
// constexpr char TEXT_LBCC[]  PROGMEM = "LBCC";
// constexpr char TEXT_LBCS[]  PROGMEM = "LBCS";
// constexpr char TEXT_LBEQ[]  PROGMEM = "LBEQ";
// constexpr char TEXT_LBGE[]  PROGMEM = "LBGE";
// constexpr char TEXT_LBGT[]  PROGMEM = "LBGT";
// constexpr char TEXT_LBHI[]  PROGMEM = "LBHI";
// constexpr char TEXT_LBHS[]  PROGMEM = "LBHS";
// constexpr char TEXT_LBLE[]  PROGMEM = "LBLE";
// constexpr char TEXT_LBLO[]  PROGMEM = "LBLO";
// constexpr char TEXT_LBLS[]  PROGMEM = "LBLS";
// constexpr char TEXT_LBLT[]  PROGMEM = "LBLT";
// constexpr char TEXT_LBMI[]  PROGMEM = "LBMI";
// constexpr char TEXT_LBNE[]  PROGMEM = "LBNE";
// constexpr char TEXT_LBPL[]  PROGMEM = "LBPL";
// constexpr char TEXT_LBRA[]  PROGMEM = "LBRA";
// constexpr char TEXT_LBRN[]  PROGMEM = "LBRN";
// constexpr char TEXT_LBSR[]  PROGMEM = "LBSR";
// constexpr char TEXT_LBVC[]  PROGMEM = "LBVC";
// constexpr char TEXT_LBVS[]  PROGMEM = "LBVS";
// constexpr char TEXT_LDA[]   PROGMEM = "LDA";
// constexpr char TEXT_LDB[]   PROGMEM = "LDB";
// constexpr char TEXT_LDD[]   PROGMEM = "LDD";
// constexpr char TEXT_LDS[]   PROGMEM = "LDS";
constexpr char TEXT_LDU[]  PROGMEM = "LDU";
// constexpr char TEXT_LDX[]   PROGMEM = "LDX";
// constexpr char TEXT_LDY[]   PROGMEM = "LDY";
// constexpr char TEXT_LEAS[]  PROGMEM = "LEAS";
constexpr char TEXT_LEAU[] PROGMEM = "LEAU";
// constexpr char TEXT_LEAX[]  PROGMEM = "LEAX";
// constexpr char TEXT_LEAY[]  PROGMEM = "LEAY";
// constexpr char TEXT_LSL[]   PROGMEM = "LSL";
// constexpr char TEXT_LSLA[]  PROGMEM = "LSLA";
// constexpr char TEXT_LSLB[]  PROGMEM = "LSLB";
// constexpr char TEXT_LSR[]   PROGMEM = "LSR";
// constexpr char TEXT_LSRA[]  PROGMEM = "LSRA";
// constexpr char TEXT_LSRB[]  PROGMEM = "LSRB";
// constexpr char TEXT_MUL[]   PROGMEM = "MUL";
// constexpr char TEXT_NEG[]   PROGMEM = "NEG";
// constexpr char TEXT_NEGA[]  PROGMEM = "NEGA";
// constexpr char TEXT_NEGB[]  PROGMEM = "NEGB";
// constexpr char TEXT_NOP[]   PROGMEM = "NOP";
// constexpr char TEXT_ORA[]   PROGMEM = "ORA";
// constexpr char TEXT_ORB[]   PROGMEM = "ORB";
// constexpr char TEXT_ORCC[]  PROGMEM = "ORCC";
constexpr char TEXT_PSHS[] PROGMEM = "PSHS";
constexpr char TEXT_PSHU[] PROGMEM = "PSHU";
constexpr char TEXT_PULS[] PROGMEM = "PULS";
constexpr char TEXT_PULU[] PROGMEM = "PULU";
// constexpr char TEXT_ROL[]   PROGMEM = "ROL";
// constexpr char TEXT_ROLA[]  PROGMEM = "ROLA";
// constexpr char TEXT_ROLB[]  PROGMEM = "ROLB";
// constexpr char TEXT_ROR[]   PROGMEM = "ROR";
// constexpr char TEXT_RORA[]  PROGMEM = "RORA";
// constexpr char TEXT_RORB[]  PROGMEM = "RORB";
// constexpr char TEXT_RTI[]   PROGMEM = "RTI";
// constexpr char TEXT_RTS[]   PROGMEM = "RTS";
// constexpr char TEXT_SBCA[]  PROGMEM = "SBCA";
// constexpr char TEXT_SBCB[]  PROGMEM = "SBCB";
// constexpr char TEXT_SEX[]   PROGMEM = "SEX";
// constexpr char TEXT_STA[]   PROGMEM = "STA";
// constexpr char TEXT_STB[]   PROGMEM = "STB";
// constexpr char TEXT_STD[]   PROGMEM = "STD";
// constexpr char TEXT_STS[]   PROGMEM = "STS";
constexpr char TEXT_STU[]  PROGMEM = "STU";
// constexpr char TEXT_STX[]   PROGMEM = "STX";
// constexpr char TEXT_STY[]   PROGMEM = "STY";
// constexpr char TEXT_SUBA[]  PROGMEM = "SUBA";
// constexpr char TEXT_SUBB[]  PROGMEM = "SUBB";
// constexpr char TEXT_SUBD[]  PROGMEM = "SUBD";
// constexpr char TEXT_SWI[]   PROGMEM = "SWI";
constexpr char TEXT_SWI2[] PROGMEM = "SWI2";
constexpr char TEXT_SWI3[] PROGMEM = "SWI3";
constexpr char TEXT_SYNC[] PROGMEM = "SYNC";
// constexpr char TEXT_TFR[]   PROGMEM = "TFR";
// constexpr char TEXT_TST[]   PROGMEM = "TST";
// constexpr char TEXT_TSTA[]  PROGMEM = "TSTA";
// constexpr char TEXT_TSTB[]  PROGMEM = "TSTB";

// HD6309
// constexpr char TEXT_ADCD[] PROGMEM = "ADCD";
constexpr char TEXT_ADCR[]  PROGMEM = "ADCR";
// constexpr char TEXT_ADDE[] PROGMEM = "ADDE";
// constexpr char TEXT_ADDF[] PROGMEM = "ADDF";
// constexpr char TEXT_ADDR[] PROGMEM = "ADDR";
// constexpr char TEXT_ADDW[] PROGMEM = "ADDW";
// constexpr char TEXT_AIM[]  PROGMEM = "AIM";
// constexpr char TEXT_ANDD[] PROGMEM = "ANDD";
// constexpr char TEXT_ANDR[] PROGMEM = "ANDR";
// constexpr char TEXT_ASLD[] PROGMEM = "ASLD";
// constexpr char TEXT_ASRD[] PROGMEM = "ASRD";
// constexpr char TEXT_BAND[] PROGMEM = "BAND";
constexpr char TEXT_BEOR[]  PROGMEM = "BEOR";
constexpr char TEXT_BIAND[] PROGMEM = "BIAND";
constexpr char TEXT_BIEOR[] PROGMEM = "BIEOR";
constexpr char TEXT_BIOR[]  PROGMEM = "BIOR";
constexpr char TEXT_BITD[]  PROGMEM = "BITD";
constexpr char TEXT_BITMD[] PROGMEM = "BITMD";
// constexpr char TEXT_BOR[]  PROGMEM = "BOR";
// constexpr char TEXT_CLRD[] PROGMEM = "CLRD";
// constexpr char TEXT_CLRE[] PROGMEM = "CLRE";
// constexpr char TEXT_CLRF[] PROGMEM = "CLRF";
// constexpr char TEXT_CLRW[] PROGMEM = "CLRW";
constexpr char TEXT_CMPE[]  PROGMEM = "CMPE";
// constexpr char TEXT_CMPF[] PROGMEM = "CMPF";
// constexpr char TEXT_CMPR[] PROGMEM = "CMPR";
// constexpr char TEXT_CMPW[] PROGMEM = "CMPW";
// constexpr char TEXT_COMD[] PROGMEM = "COMD";
// constexpr char TEXT_COME[] PROGMEM = "COME";
constexpr char TEXT_COMF[]  PROGMEM = "COMF";
// constexpr char TEXT_COMW[] PROGMEM = "COMW";
// constexpr char TEXT_DECD[] PROGMEM = "DECD";
constexpr char TEXT_DECE[]  PROGMEM = "DECE";
// constexpr char TEXT_DECF[]  PROGMEM = "DECF";
// constexpr char TEXT_DECW[]  PROGMEM = "DECW";
// constexpr char TEXT_DIVD[]  PROGMEM = "DIVD";
constexpr char TEXT_DIVQ[]  PROGMEM = "DIVQ";
// constexpr char TEXT_EIM[]  PROGMEM = "EIM";
// constexpr char TEXT_EORD[] PROGMEM = "EORD";
// constexpr char TEXT_EORR[] PROGMEM = "EORR";
constexpr char TEXT_INCD[]  PROGMEM = "INCD";
constexpr char TEXT_INCE[]  PROGMEM = "INCE";
// constexpr char TEXT_INCF[]  PROGMEM = "INCF";
// constexpr char TEXT_INCW[]  PROGMEM = "INCW";
constexpr char TEXT_LDBT[]  PROGMEM = "LDBT";
// constexpr char TEXT_LDE[]  PROGMEM = "LDE";
// constexpr char TEXT_LDF[]  PROGMEM = "LDF";
constexpr char TEXT_LDMD[]  PROGMEM = "LDMD";
constexpr char TEXT_LDQ[]   PROGMEM = "LDQ";
// constexpr char TEXT_LDW[]  PROGMEM = "LDW";
// constexpr char TEXT_LSLD[] PROGMEM = "LSLD";
// constexpr char TEXT_LSRD[] PROGMEM = "LSRD";
// constexpr char TEXT_LSRW[] PROGMEM = "LSRW";
// constexpr char TEXT_MULD[] PROGMEM = "MULD";
// constexpr char TEXT_NEGD[] PROGMEM = "NEGD";
// constexpr char TEXT_OIM[]  PROGMEM = "OIM";
// constexpr char TEXT_ORD[]  PROGMEM = "ORD";
// constexpr char TEXT_ORR[]  PROGMEM = "ORR";
constexpr char TEXT_PSHSW[] PROGMEM = "PSHSW";
constexpr char TEXT_PSHUW[] PROGMEM = "PSHUW";
constexpr char TEXT_PULSW[] PROGMEM = "PULSW";
constexpr char TEXT_PULUW[] PROGMEM = "PULUW";
// constexpr char TEXT_ROLD[] PROGMEM = "ROLD";
// constexpr char TEXT_ROLW[] PROGMEM = "ROLW";
// constexpr char TEXT_RORD[] PROGMEM = "RORD";
// constexpr char TEXT_RORW[] PROGMEM = "RORW";
// constexpr char TEXT_SBCD[] PROGMEM = "SBCD";
constexpr char TEXT_SBCR[]  PROGMEM = "SBCR";
constexpr char TEXT_SEXW[]  PROGMEM = "SEXW";
constexpr char TEXT_STBT[]  PROGMEM = "STBT";
// constexpr char TEXT_STE[]  PROGMEM = "STE";
// constexpr char TEXT_STF[]  PROGMEM = "STF";
constexpr char TEXT_STQ[]   PROGMEM = "STQ";
constexpr char TEXT_STW[]   PROGMEM = "STW";
// constexpr char TEXT_SUBE[] PROGMEM = "SUBE";
// constexpr char TEXT_SUBF[] PROGMEM = "SUBF";
// constexpr char TEXT_SUBR[] PROGMEM = "SUBR";
// constexpr char TEXT_SUBW[] PROGMEM = "SUBW";
constexpr char TEXT_TFM[]   PROGMEM = "TFM";
// constexpr char TEXT_TIM[]  PROGMEM = "TIM";
// constexpr char TEXT_TSTD[] PROGMEM = "TSTD";
// constexpr char TEXT_TSTE[] PROGMEM = "TSTE";
// constexpr char TEXT_TSTF[] PROGMEM = "TSTF";
// constexpr char TEXT_TSTW[] PROGMEM = "TSTW";

// MC6809
// constexpr char TEXT_REG_A[]  PROGMEM = "A";
// constexpr char TEXT_REG_B[]  PROGMEM = "B";
constexpr char TEXT_REG_CC[]  PROGMEM = "CC";
// constexpr char TEXT_REG_D[]  PROGMEM = "D";
// constexpr char TEXT_REG_DP[] PROGMEM = "DP";
// constexpr char TEXT_REG_PC[] PROGMEM = "PC";
constexpr char TEXT_REG_PCR[] PROGMEM = "PCR";
// constexpr char TEXT_REG_S[]  PROGMEM = "S";
constexpr char TEXT_REG_U[]   PROGMEM = "U";
// constexpr char TEXT_REG_X[]  PROGMEM = "X";
// constexpr char TEXT_REG_Y[]  PROGMEM = "Y";

// HD6309
constexpr char TEXT_REG_0[] PROGMEM = "0";
// constexpr char TEXT_REG_E[] PROGMEM = "E";
// constexpr char TEXT_REG_F[] PROGMEM = "F";
constexpr char TEXT_REG_V[] PROGMEM = "V";
// constexpr char TEXT_REG_W[] PROGMEM = "W";
// constexpr char TEXT_REG_Z[] PROGMEM = "Z";

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
