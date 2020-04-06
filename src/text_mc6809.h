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

#ifndef __TEXT_MC6809_H__
#define __TEXT_MC6809_H__

#include "config_host.h"

namespace libasm {
namespace mc6809 {

static const char TEXT_ABX[]   PROGMEM = "ABX";
static const char TEXT_ADCA[]  PROGMEM = "ADCA";
static const char TEXT_ADCB[]  PROGMEM = "ADCB";
static const char TEXT_ADDA[]  PROGMEM = "ADDA";
static const char TEXT_ADDB[]  PROGMEM = "ADDB";
static const char TEXT_ADDD[]  PROGMEM = "ADDD";
static const char TEXT_ANDA[]  PROGMEM = "ANDA";
static const char TEXT_ANDB[]  PROGMEM = "ANDB";
static const char TEXT_ANDCC[] PROGMEM = "ANDCC";
static const char TEXT_ASLA[]  PROGMEM = "ASLA";
static const char TEXT_ASLB[]  PROGMEM = "ASLB";
static const char TEXT_ASL[]   PROGMEM = "ASL";
static const char TEXT_ASRA[]  PROGMEM = "ASRA";
static const char TEXT_ASRB[]  PROGMEM = "ASRB";
static const char TEXT_ASR[]   PROGMEM = "ASR";
static const char TEXT_BCC[]   PROGMEM = "BCC";
static const char TEXT_BCS[]   PROGMEM = "BCS";
static const char TEXT_BEQ[]   PROGMEM = "BEQ";
static const char TEXT_BGE[]   PROGMEM = "BGE";
static const char TEXT_BGT[]   PROGMEM = "BGT";
static const char TEXT_BHI[]   PROGMEM = "BHI";
static const char TEXT_BHS[]   PROGMEM = "BHS";
static const char TEXT_BITA[]  PROGMEM = "BITA";
static const char TEXT_BITB[]  PROGMEM = "BITB";
static const char TEXT_BLE[]   PROGMEM = "BLE";
static const char TEXT_BLO[]   PROGMEM = "BLO";
static const char TEXT_BLS[]   PROGMEM = "BLS";
static const char TEXT_BLT[]   PROGMEM = "BLT";
static const char TEXT_BMI[]   PROGMEM = "BMI";
static const char TEXT_BNE[]   PROGMEM = "BNE";
static const char TEXT_BPL[]   PROGMEM = "BPL";
static const char TEXT_BRA[]   PROGMEM = "BRA";
static const char TEXT_BRN[]   PROGMEM = "BRN";
static const char TEXT_BSR[]   PROGMEM = "BSR";
static const char TEXT_BVC[]   PROGMEM = "BVC";
static const char TEXT_BVS[]   PROGMEM = "BVS";
static const char TEXT_CLRA[]  PROGMEM = "CLRA";
static const char TEXT_CLRB[]  PROGMEM = "CLRB";
static const char TEXT_CLR[]   PROGMEM = "CLR";
static const char TEXT_CMPA[]  PROGMEM = "CMPA";
static const char TEXT_CMPB[]  PROGMEM = "CMPB";
static const char TEXT_CMPD[]  PROGMEM = "CMPD";
static const char TEXT_CMPS[]  PROGMEM = "CMPS";
static const char TEXT_CMPU[]  PROGMEM = "CMPU";
static const char TEXT_CMPX[]  PROGMEM = "CMPX";
static const char TEXT_CMPY[]  PROGMEM = "CMPY";
static const char TEXT_COMA[]  PROGMEM = "COMA";
static const char TEXT_COMB[]  PROGMEM = "COMB";
static const char TEXT_COM[]   PROGMEM = "COM";
static const char TEXT_CWAI[]  PROGMEM = "CWAI";
static const char TEXT_DAA[]   PROGMEM = "DAA";
static const char TEXT_DECA[]  PROGMEM = "DECA";
static const char TEXT_DECB[]  PROGMEM = "DECB";
static const char TEXT_DEC[]   PROGMEM = "DEC";
static const char TEXT_EORA[]  PROGMEM = "EORA";
static const char TEXT_EORB[]  PROGMEM = "EORB";
static const char TEXT_EXG[]   PROGMEM = "EXG";
static const char TEXT_INCA[]  PROGMEM = "INCA";
static const char TEXT_INCB[]  PROGMEM = "INCB";
static const char TEXT_INC[]   PROGMEM = "INC";
static const char TEXT_JMP[]   PROGMEM = "JMP";
static const char TEXT_JSR[]   PROGMEM = "JSR";
static const char TEXT_LBCC[]  PROGMEM = "LBCC";
static const char TEXT_LBCS[]  PROGMEM = "LBCS";
static const char TEXT_LBEQ[]  PROGMEM = "LBEQ";
static const char TEXT_LBGE[]  PROGMEM = "LBGE";
static const char TEXT_LBGT[]  PROGMEM = "LBGT";
static const char TEXT_LBHI[]  PROGMEM = "LBHI";
static const char TEXT_LBHS[]  PROGMEM = "LBHS";
static const char TEXT_LBLE[]  PROGMEM = "LBLE";
static const char TEXT_LBLO[]  PROGMEM = "LBLO";
static const char TEXT_LBLS[]  PROGMEM = "LBLS";
static const char TEXT_LBLT[]  PROGMEM = "LBLT";
static const char TEXT_LBMI[]  PROGMEM = "LBMI";
static const char TEXT_LBNE[]  PROGMEM = "LBNE";
static const char TEXT_LBPL[]  PROGMEM = "LBPL";
static const char TEXT_LBRA[]  PROGMEM = "LBRA";
static const char TEXT_LBRN[]  PROGMEM = "LBRN";
static const char TEXT_LBSR[]  PROGMEM = "LBSR";
static const char TEXT_LBVC[]  PROGMEM = "LBVC";
static const char TEXT_LBVS[]  PROGMEM = "LBVS";
static const char TEXT_LDA[]   PROGMEM = "LDA";
static const char TEXT_LDB[]   PROGMEM = "LDB";
static const char TEXT_LDD[]   PROGMEM = "LDD";
static const char TEXT_LDS[]   PROGMEM = "LDS";
static const char TEXT_LDU[]   PROGMEM = "LDU";
static const char TEXT_LDX[]   PROGMEM = "LDX";
static const char TEXT_LDY[]   PROGMEM = "LDY";
static const char TEXT_LEAS[]  PROGMEM = "LEAS";
static const char TEXT_LEAU[]  PROGMEM = "LEAU";
static const char TEXT_LEAX[]  PROGMEM = "LEAX";
static const char TEXT_LEAY[]  PROGMEM = "LEAY";
static const char TEXT_LSLA[]  PROGMEM = "LSLA";
static const char TEXT_LSLB[]  PROGMEM = "LSLB";
static const char TEXT_LSL[]   PROGMEM = "LSL";
static const char TEXT_LSRA[]  PROGMEM = "LSRA";
static const char TEXT_LSRB[]  PROGMEM = "LSRB";
static const char TEXT_LSR[]   PROGMEM = "LSR";
static const char TEXT_MUL[]   PROGMEM = "MUL";
static const char TEXT_NEGA[]  PROGMEM = "NEGA";
static const char TEXT_NEGB[]  PROGMEM = "NEGB";
static const char TEXT_NEG[]   PROGMEM = "NEG";
static const char TEXT_NOP[]   PROGMEM = "NOP";
static const char TEXT_ORA[]   PROGMEM = "ORA";
static const char TEXT_ORB[]   PROGMEM = "ORB";
static const char TEXT_ORCC[]  PROGMEM = "ORCC";
static const char TEXT_PSHS[]  PROGMEM = "PSHS";
static const char TEXT_PSHU[]  PROGMEM = "PSHU";
static const char TEXT_PULS[]  PROGMEM = "PULS";
static const char TEXT_PULU[]  PROGMEM = "PULU";
static const char TEXT_ROLA[]  PROGMEM = "ROLA";
static const char TEXT_ROLB[]  PROGMEM = "ROLB";
static const char TEXT_ROL[]   PROGMEM = "ROL";
static const char TEXT_RORA[]  PROGMEM = "RORA";
static const char TEXT_RORB[]  PROGMEM = "RORB";
static const char TEXT_ROR[]   PROGMEM = "ROR";
static const char TEXT_RTI[]   PROGMEM = "RTI";
static const char TEXT_RTS[]   PROGMEM = "RTS";
static const char TEXT_SBCA[]  PROGMEM = "SBCA";
static const char TEXT_SBCB[]  PROGMEM = "SBCB";
static const char TEXT_SEX[]   PROGMEM = "SEX";
static const char TEXT_STA[]   PROGMEM = "STA";
static const char TEXT_STB[]   PROGMEM = "STB";
static const char TEXT_STD[]   PROGMEM = "STD";
static const char TEXT_STS[]   PROGMEM = "STS";
static const char TEXT_STU[]   PROGMEM = "STU";
static const char TEXT_STX[]   PROGMEM = "STX";
static const char TEXT_STY[]   PROGMEM = "STY";
static const char TEXT_SUBA[]  PROGMEM = "SUBA";
static const char TEXT_SUBB[]  PROGMEM = "SUBB";
static const char TEXT_SUBD[]  PROGMEM = "SUBD";
static const char TEXT_SWI2[]  PROGMEM = "SWI2";
static const char TEXT_SWI3[]  PROGMEM = "SWI3";
static const char TEXT_SWI[]   PROGMEM = "SWI";
static const char TEXT_SYNC[]  PROGMEM = "SYNC";
static const char TEXT_TFR[]   PROGMEM = "TFR";
static const char TEXT_TSTA[]  PROGMEM = "TSTA";
static const char TEXT_TSTB[]  PROGMEM = "TSTB";
static const char TEXT_TST[]   PROGMEM = "TST";

// HD6309

static const char TEXT_ADCD[]  PROGMEM = "ADCD";
static const char TEXT_ADCR[]  PROGMEM = "ADCR";
static const char TEXT_ADDE[]  PROGMEM = "ADDE";
static const char TEXT_ADDF[]  PROGMEM = "ADDF";
static const char TEXT_ADDR[]  PROGMEM = "ADDR";
static const char TEXT_ADDW[]  PROGMEM = "ADDW";
static const char TEXT_AIM[]   PROGMEM = "AIM";
static const char TEXT_ANDD[]  PROGMEM = "ANDD";
static const char TEXT_ANDR[]  PROGMEM = "ANDR";
static const char TEXT_ASLD[]  PROGMEM = "ASLD";
static const char TEXT_ASRD[]  PROGMEM = "ASRD";
static const char TEXT_BAND[]  PROGMEM = "BAND";
static const char TEXT_BEOR[]  PROGMEM = "BEOR";
static const char TEXT_BIAND[] PROGMEM = "BIAND";
static const char TEXT_BIEOR[] PROGMEM = "BIEOR";
static const char TEXT_BIOR[]  PROGMEM = "BIOR";
static const char TEXT_BITD[]  PROGMEM = "BITD";
static const char TEXT_BITMD[] PROGMEM = "BITMD";
static const char TEXT_BOR[]   PROGMEM = "BOR";
static const char TEXT_CLRD[]  PROGMEM = "CLRD";
static const char TEXT_CLRE[]  PROGMEM = "CLRE";
static const char TEXT_CLRF[]  PROGMEM = "CLRF";
static const char TEXT_CLRW[]  PROGMEM = "CLRW";
static const char TEXT_CMPE[]  PROGMEM = "CMPE";
static const char TEXT_CMPF[]  PROGMEM = "CMPF";
static const char TEXT_CMPR[]  PROGMEM = "CMPR";
static const char TEXT_CMPW[]  PROGMEM = "CMPW";
static const char TEXT_COMD[]  PROGMEM = "COMD";
static const char TEXT_COME[]  PROGMEM = "COME";
static const char TEXT_COMF[]  PROGMEM = "COMF";
static const char TEXT_COMW[]  PROGMEM = "COMW";
static const char TEXT_DECD[]  PROGMEM = "DECD";
static const char TEXT_DECE[]  PROGMEM = "DECE";
static const char TEXT_DECF[]  PROGMEM = "DECF";
static const char TEXT_DECW[]  PROGMEM = "DECW";
static const char TEXT_DIVD[]  PROGMEM = "DIVD";
static const char TEXT_DIVQ[]  PROGMEM = "DIVQ";
static const char TEXT_EIM[]   PROGMEM = "EIM";
static const char TEXT_EORD[]  PROGMEM = "EORD";
static const char TEXT_EORR[]  PROGMEM = "EORR";
static const char TEXT_INCD[]  PROGMEM = "INCD";
static const char TEXT_INCE[]  PROGMEM = "INCE";
static const char TEXT_INCF[]  PROGMEM = "INCF";
static const char TEXT_INCW[]  PROGMEM = "INCW";
static const char TEXT_LDBT[]  PROGMEM = "LDBT";
static const char TEXT_LDE[]   PROGMEM = "LDE";
static const char TEXT_LDF[]   PROGMEM = "LDF";
static const char TEXT_LDMD[]  PROGMEM = "LDMD";
static const char TEXT_LDQ[]   PROGMEM = "LDQ";
static const char TEXT_LDW[]   PROGMEM = "LDW";
static const char TEXT_LSLD[]  PROGMEM = "LSLD";
static const char TEXT_LSRD[]  PROGMEM = "LSRD";
static const char TEXT_LSRW[]  PROGMEM = "LSRW";
static const char TEXT_MULD[]  PROGMEM = "MULD";
static const char TEXT_NEGD[]  PROGMEM = "NEGD";
static const char TEXT_OIM[]   PROGMEM = "OIM";
static const char TEXT_ORD[]   PROGMEM = "ORD";
static const char TEXT_ORR[]   PROGMEM = "ORR";
static const char TEXT_PSHSW[] PROGMEM = "PSHSW";
static const char TEXT_PSHUW[] PROGMEM = "PSHUW";
static const char TEXT_PULSW[] PROGMEM = "PULSW";
static const char TEXT_PULUW[] PROGMEM = "PULUW";
static const char TEXT_ROLD[]  PROGMEM = "ROLD";
static const char TEXT_ROLW[]  PROGMEM = "ROLW";
static const char TEXT_RORD[]  PROGMEM = "RORD";
static const char TEXT_RORW[]  PROGMEM = "RORW";
static const char TEXT_SBCD[]  PROGMEM = "SBCD";
static const char TEXT_SBCR[]  PROGMEM = "SBCR";
static const char TEXT_SEXW[]  PROGMEM = "SEXW";
static const char TEXT_STBT[]  PROGMEM = "STBT";
static const char TEXT_STE[]   PROGMEM = "STE";
static const char TEXT_STF[]   PROGMEM = "STF";
static const char TEXT_STQ[]   PROGMEM = "STQ";
static const char TEXT_STW[]   PROGMEM = "STW";
static const char TEXT_SUBE[]  PROGMEM = "SUBE";
static const char TEXT_SUBF[]  PROGMEM = "SUBF";
static const char TEXT_SUBR[]  PROGMEM = "SUBR";
static const char TEXT_SUBW[]  PROGMEM = "SUBW";
static const char TEXT_TFM[]   PROGMEM = "TFM";
static const char TEXT_TIM[]   PROGMEM = "TIM";
static const char TEXT_TSTD[]  PROGMEM = "TSTD";
static const char TEXT_TSTE[]  PROGMEM = "TSTE";
static const char TEXT_TSTF[]  PROGMEM = "TSTF";
static const char TEXT_TSTW[]  PROGMEM = "TSTW";

// Pseudo

static const char TEXT_SETDP[] PROGMEM  = "SETDP";
static const char TEXT_ASSUME[] PROGMEM = "ASSUME";

} // namespace mc6809
} // namespace libasm

#endif // __TEXT_MC6809_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
