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

#include "text_mc6800.h"

namespace libasm {
namespace text {
namespace mc6800 {

// clang-format off
const char TEXT_MC6800_LIST[] PROGMEM = "MC6800, MB8861, MC6801, HD6301, MC68HC11";
const char TEXT_CPU_6800[] PROGMEM = "6800";
const char TEXT_CPU_6801[] PROGMEM = "6801";
const char TEXT_CPU_6301[] PROGMEM = "6301";
const char TEXT_CPU_6811[] PROGMEM = "6811";
const char TEXT_CPU_68HC11[] PROGMEM = "68HC11";
const char TEXT_CPU_HD6301[] PROGMEM = "HD6301";
const char TEXT_CPU_MB8861[] PROGMEM = "MB8861";

// MC6800
const char TEXT_ABA[]  PROGMEM = "ABA";
// const char TEXT_ADC[]  PROGMEM = "ADC";
// const char TEXT_ADCA[] PROGMEM = "ADCA";
// const char TEXT_ADCB[] PROGMEM = "ADCB";
// const char TEXT_ADD[]  PROGMEM = "ADD";
// const char TEXT_ADDA[] PROGMEM = "ADDA";
// const char TEXT_ADDB[] PROGMEM = "ADDB";
// const char TEXT_AND[]  PROGMEM = "AND";
// const char TEXT_ANDA[] PROGMEM = "ANDA";
// const char TEXT_ANDB[] PROGMEM = "ANDB";
// const char TEXT_ASL[]  PROGMEM = "ASL";
// const char TEXT_ASLA[] PROGMEM = "ASLA";
// const char TEXT_ASLB[] PROGMEM = "ASLB";
// const char TEXT_ASR[]  PROGMEM = "ASR";
// const char TEXT_ASRA[] PROGMEM = "ASRA";
// const char TEXT_ASRB[] PROGMEM = "ASRB";
// const char TEXT_BCC[]  PROGMEM = "BCC";
// const char TEXT_BCS[]  PROGMEM = "BCS";
// const char TEXT_BEQ[]  PROGMEM = "BEQ";
// const char TEXT_BGE[]  PROGMEM = "BGE";
// const char TEXT_BGT[]  PROGMEM = "BGT";
// const char TEXT_BHI[]  PROGMEM = "BHI";
// const char TEXT_BHS[]  PROGMEM = "BHS";
// const char TEXT_BIT[]  PROGMEM = "BIT";
// const char TEXT_BITA[] PROGMEM = "BITA";
// const char TEXT_BITB[] PROGMEM = "BITB";
// const char TEXT_BLE[]  PROGMEM = "BLE";
// const char TEXT_BLO[]  PROGMEM = "BLO";
// const char TEXT_BLS[]  PROGMEM = "BLS";
// const char TEXT_BLT[]  PROGMEM = "BLT";
// const char TEXT_BMI[]  PROGMEM = "BMI";
// const char TEXT_BNE[]  PROGMEM = "BNE";
// const char TEXT_BPL[]  PROGMEM = "BPL";
// const char TEXT_BRA[]  PROGMEM = "BRA";
// const char TEXT_BSR[]  PROGMEM = "BSR";
// const char TEXT_BVC[]  PROGMEM = "BVC";
// const char TEXT_BVS[]  PROGMEM = "BVS";
const char TEXT_CBA[]  PROGMEM = "CBA";
// const char TEXT_CLC[]  PROGMEM = "CLC";
// const char TEXT_CLI[]  PROGMEM = "CLI";
// const char TEXT_CLR[]  PROGMEM = "CLR";
// const char TEXT_CLRA[] PROGMEM = "CLRA";
// const char TEXT_CLRB[] PROGMEM = "CLRB";
// const char TEXT_CLV[]  PROGMEM = "CLV";
// const char TEXT_CMP[]  PROGMEM = "CMP";
// const char TEXT_CMPA[] PROGMEM = "CMPA";
// const char TEXT_CMPB[] PROGMEM = "CMPB";
// const char TEXT_COM[]  PROGMEM = "COM";
// const char TEXT_COMA[] PROGMEM = "COMA";
// const char TEXT_COMB[] PROGMEM = "COMB";
// const char TEXT_CPX[]  PROGMEM = "CPX";
// const char TEXT_DAA[]  PROGMEM = "DAA";
// const char TEXT_DEC[]  PROGMEM = "DEC";
// const char TEXT_DECA[] PROGMEM = "DECA";
// const char TEXT_DECB[] PROGMEM = "DECB";
const char TEXT_DES[]  PROGMEM = "DES";
// const char TEXT_DEX[]  PROGMEM = "DEX";
// const char TEXT_EOR[]  PROGMEM = "EOR";
// const char TEXT_EORA[] PROGMEM = "EORA";
// const char TEXT_EORB[] PROGMEM = "EORB";
// const char TEXT_INC[]  PROGMEM = "INC";
// const char TEXT_INCA[] PROGMEM = "INCA";
// const char TEXT_INCB[] PROGMEM = "INCB";
// const char TEXT_INS[]  PROGMEM = "INS";
// const char TEXT_INX[]  PROGMEM = "INX";
// const char TEXT_JMP[]  PROGMEM = "JMP";
// const char TEXT_JSR[]  PROGMEM = "JSR";
// const char TEXT_LDA[]  PROGMEM = "LDA";
const char TEXT_LDAA[] PROGMEM = "LDAA";
const char TEXT_LDAB[] PROGMEM = "LDAB";
// const char TEXT_LDS[]  PROGMEM = "LDS";
// const char TEXT_LDX[]  PROGMEM = "LDX";
// const char TEXT_LSL[]  PROGMEM = "LSL";
// const char TEXT_LSLA[] PROGMEM = "LSLA";
// const char TEXT_LSLB[] PROGMEM = "LSLB";
// const char TEXT_LSR[]  PROGMEM = "LSR";
// const char TEXT_LSRA[] PROGMEM = "LSRA";
// const char TEXT_LSRB[] PROGMEM = "LSRB";
// const char TEXT_NEG[]  PROGMEM = "NEG";
// const char TEXT_NEGA[] PROGMEM = "NEGA";
// const char TEXT_NEGB[] PROGMEM = "NEGB";
// const char TEXT_NOP[]  PROGMEM = "NOP";
// const char TEXT_ORA[]  PROGMEM = "ORA";
const char TEXT_ORAA[] PROGMEM = "ORAA";
const char TEXT_ORAB[] PROGMEM = "ORAB";
const char TEXT_PSH[]  PROGMEM = "PSH";
const char TEXT_PSHA[] PROGMEM = "PSHA";
const char TEXT_PSHB[] PROGMEM = "PSHB";
const char TEXT_PUL[]  PROGMEM = "PUL";
const char TEXT_PULA[] PROGMEM = "PULA";
const char TEXT_PULB[] PROGMEM = "PULB";
// const char TEXT_ROL[]  PROGMEM = "ROL";
// const char TEXT_ROLA[] PROGMEM = "ROLA";
// const char TEXT_ROLB[] PROGMEM = "ROLB";
// const char TEXT_ROR[]  PROGMEM = "ROR";
// const char TEXT_RORA[] PROGMEM = "RORA";
// const char TEXT_RORB[] PROGMEM = "RORB";
// const char TEXT_RTI[]  PROGMEM = "RTI";
// const char TEXT_RTS[]  PROGMEM = "RTS";
const char TEXT_SBA[]  PROGMEM = "SBA";
// const char TEXT_SBC[]  PROGMEM = "SBC";
// const char TEXT_SBCA[] PROGMEM = "SBCA";
// const char TEXT_SBCB[] PROGMEM = "SBCB";
// const char TEXT_SEC[]  PROGMEM = "SEC";
// const char TEXT_SEI[]  PROGMEM = "SEI";
const char TEXT_SEV[]  PROGMEM = "SEV";
// const char TEXT_STA[]  PROGMEM = "STA";
const char TEXT_STAA[] PROGMEM = "STAA";
const char TEXT_STAB[] PROGMEM = "STAB";
// const char TEXT_STS[]  PROGMEM = "STS";
// const char TEXT_STX[]  PROGMEM = "STX";
// const char TEXT_SUB[]  PROGMEM = "SUB";
// const char TEXT_SUBA[] PROGMEM = "SUBA";
// const char TEXT_SUBB[] PROGMEM = "SUBB";
// const char TEXT_SWI[]  PROGMEM = "SWI";
const char TEXT_TAB[]  PROGMEM = "TAB";
const char TEXT_TAP[]  PROGMEM = "TAP";
const char TEXT_TBA[]  PROGMEM = "TBA";
const char TEXT_TPA[]  PROGMEM = "TPA";
// const char TEXT_TST[]  PROGMEM = "TST";
// const char TEXT_TSTA[] PROGMEM = "TSTA";
// const char TEXT_TSTB[] PROGMEM = "TSTB";
// const char TEXT_TSX[]  PROGMEM = "TSX";
// const char TEXT_TXS[]  PROGMEM = "TXS";
// const char TEXT_WAI[]  PROGMEM = "WAI";

// MB8861
const char TEXT_ADX[] PROGMEM = "ADX";
const char TEXT_NIM[] PROGMEM = "NIM";
const char TEXT_TMM[] PROGMEM = "TMM";
const char TEXT_XIM[] PROGMEM = "XIM";

// MC6801
// const char TEXT_ABX[]  PROGMEM = "ABX";
// const char TEXT_ADDD[] PROGMEM = "ADDD";
// const char TEXT_ASLD[] PROGMEM = "ASLD";
// const char TEXT_BRN[]  PROGMEM = "BRN";
// const char TEXT_LDD[]  PROGMEM = "LDD";
// const char TEXT_LSLD[] PROGMEM = "LSLD";
// const char TEXT_LSRD[] PROGMEM = "LSRD";
// const char TEXT_MUL[]  PROGMEM = "MUL";
// const char TEXT_STD[]  PROGMEM = "STD";
// const char TEXT_SUBD[] PROGMEM = "SUBD";
const char TEXT_PSHX[] PROGMEM = "PSHX";
const char TEXT_PULX[] PROGMEM = "PULX";

// HD6301
// const char TEXT_AIM[]  PROGMEM = "AIM";
// const char TEXT_BCLR[] PROGMEM = "BCLR";
// const char TEXT_BSET[] PROGMEM = "BSET";
const char TEXT_BTGL[] PROGMEM = "BTGL";
// const char TEXT_BTST[] PROGMEM = "BTST";
// const char TEXT_EIM[]  PROGMEM = "EIM";
// const char TEXT_OIM[]  PROGMEM = "OIM";
const char TEXT_SLP[]  PROGMEM = "SLP";
// const char TEXT_TIM[]  PROGMEM = "TIM";
const char TEXT_XGDX[] PROGMEM = "XGDX";
 
// // MC68HC11
const char TEXT_ABY[]   PROGMEM = "ABY";
// const char TEXT_BRCLR[] PROGMEM = "BRCLR";
// const char TEXT_BRSET[] PROGMEM = "BRSET";
// const char TEXT_CPD[]   PROGMEM = "CPD";
// const char TEXT_CPY[]   PROGMEM = "CPY";
// const char TEXT_DEY[]   PROGMEM = "DEY";
const char TEXT_FDIV[]  PROGMEM = "FDIV";
// const char TEXT_IDIV[]  PROGMEM = "IDIV";
// const char TEXT_INY[]   PROGMEM = "INY";
// const char TEXT_LDY[]   PROGMEM = "LDY";
const char TEXT_PSHY[]  PROGMEM = "PSHY";
const char TEXT_PULY[]  PROGMEM = "PULY";
// const char TEXT_STOP[]  PROGMEM = "STOP";
// const char TEXT_STY[]   PROGMEM = "STY";
const char TEXT_TSY[]   PROGMEM = "TSY";
const char TEXT_TYS[]   PROGMEM = "TYS";
const char TEXT_XGDY[]  PROGMEM = "XGDY";
// clang-format on

}  // namespace mc6800
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
