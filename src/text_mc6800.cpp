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
constexpr char TEXT_MC6800_LIST[] PROGMEM = "MC6800, MB8861, MC6801, HD6301, MC68HC11";
constexpr char TEXT_CPU_6800[] PROGMEM = "6800";
constexpr char TEXT_CPU_6801[] PROGMEM = "6801";
constexpr char TEXT_CPU_6301[] PROGMEM = "6301";
constexpr char TEXT_CPU_6811[] PROGMEM = "6811";
constexpr char TEXT_CPU_68HC11[] PROGMEM = "68HC11";
constexpr char TEXT_CPU_HD6301[] PROGMEM = "HD6301";
constexpr char TEXT_CPU_MB8861[] PROGMEM = "MB8861";

// MC6800
// constexpr char TEXT_ABA[]  PROGMEM = "ABA";
// constexpr char TEXT_ADC[]  PROGMEM = "ADC";
// constexpr char TEXT_ADCA[] PROGMEM = "ADCA";
// constexpr char TEXT_ADCB[] PROGMEM = "ADCB";
// constexpr char TEXT_ADD[]  PROGMEM = "ADD";
// constexpr char TEXT_ADDA[] PROGMEM = "ADDA";
// constexpr char TEXT_ADDB[] PROGMEM = "ADDB";
// constexpr char TEXT_AND[]  PROGMEM = "AND";
// constexpr char TEXT_ANDA[] PROGMEM = "ANDA";
// constexpr char TEXT_ANDB[] PROGMEM = "ANDB";
// constexpr char TEXT_ASL[]  PROGMEM = "ASL";
// constexpr char TEXT_ASLA[] PROGMEM = "ASLA";
// constexpr char TEXT_ASLB[] PROGMEM = "ASLB";
// constexpr char TEXT_ASR[]  PROGMEM = "ASR";
// constexpr char TEXT_ASRA[] PROGMEM = "ASRA";
// constexpr char TEXT_ASRB[] PROGMEM = "ASRB";
// constexpr char TEXT_BCC[]  PROGMEM = "BCC";
// constexpr char TEXT_BCS[]  PROGMEM = "BCS";
// constexpr char TEXT_BEQ[]  PROGMEM = "BEQ";
// constexpr char TEXT_BGE[]  PROGMEM = "BGE";
// constexpr char TEXT_BGT[]  PROGMEM = "BGT";
// constexpr char TEXT_BHI[]  PROGMEM = "BHI";
// constexpr char TEXT_BHS[]  PROGMEM = "BHS";
// constexpr char TEXT_BIT[]  PROGMEM = "BIT";
// constexpr char TEXT_BITA[] PROGMEM = "BITA";
// constexpr char TEXT_BITB[] PROGMEM = "BITB";
// constexpr char TEXT_BLE[]  PROGMEM = "BLE";
// constexpr char TEXT_BLO[]  PROGMEM = "BLO";
// constexpr char TEXT_BLS[]  PROGMEM = "BLS";
// constexpr char TEXT_BLT[]  PROGMEM = "BLT";
// constexpr char TEXT_BMI[]  PROGMEM = "BMI";
// constexpr char TEXT_BNE[]  PROGMEM = "BNE";
// constexpr char TEXT_BPL[]  PROGMEM = "BPL";
// constexpr char TEXT_BRA[]  PROGMEM = "BRA";
// constexpr char TEXT_BSR[]  PROGMEM = "BSR";
// constexpr char TEXT_BVC[]  PROGMEM = "BVC";
// constexpr char TEXT_BVS[]  PROGMEM = "BVS";
// constexpr char TEXT_CBA[]  PROGMEM = "CBA";
// constexpr char TEXT_CLC[]  PROGMEM = "CLC";
// constexpr char TEXT_CLI[]  PROGMEM = "CLI";
// constexpr char TEXT_CLR[]  PROGMEM = "CLR";
// constexpr char TEXT_CLRA[] PROGMEM = "CLRA";
// constexpr char TEXT_CLRB[] PROGMEM = "CLRB";
// constexpr char TEXT_CLV[]  PROGMEM = "CLV";
// constexpr char TEXT_CMP[]  PROGMEM = "CMP";
// constexpr char TEXT_CMPA[] PROGMEM = "CMPA";
// constexpr char TEXT_CMPB[] PROGMEM = "CMPB";
// constexpr char TEXT_COM[]  PROGMEM = "COM";
// constexpr char TEXT_COMA[] PROGMEM = "COMA";
// constexpr char TEXT_COMB[] PROGMEM = "COMB";
// constexpr char TEXT_CPX[]  PROGMEM = "CPX";
// constexpr char TEXT_DAA[]  PROGMEM = "DAA";
// constexpr char TEXT_DEC[]  PROGMEM = "DEC";
// constexpr char TEXT_DECA[] PROGMEM = "DECA";
// constexpr char TEXT_DECB[] PROGMEM = "DECB";
// constexpr char TEXT_DES[]  PROGMEM = "DES";
// constexpr char TEXT_DEX[]  PROGMEM = "DEX";
// constexpr char TEXT_EOR[]  PROGMEM = "EOR";
// constexpr char TEXT_EORA[] PROGMEM = "EORA";
// constexpr char TEXT_EORB[] PROGMEM = "EORB";
// constexpr char TEXT_INC[]  PROGMEM = "INC";
// constexpr char TEXT_INCA[] PROGMEM = "INCA";
// constexpr char TEXT_INCB[] PROGMEM = "INCB";
// constexpr char TEXT_INS[]  PROGMEM = "INS";
// constexpr char TEXT_INX[]  PROGMEM = "INX";
// constexpr char TEXT_JMP[]  PROGMEM = "JMP";
// constexpr char TEXT_JSR[]  PROGMEM = "JSR";
// constexpr char TEXT_LDA[]  PROGMEM = "LDA";
// constexpr char TEXT_LDAA[] PROGMEM = "LDAA";
// constexpr char TEXT_LDAB[] PROGMEM = "LDAB";
// constexpr char TEXT_LDS[]  PROGMEM = "LDS";
// constexpr char TEXT_LDX[]  PROGMEM = "LDX";
// constexpr char TEXT_LSL[]  PROGMEM = "LSL";
// constexpr char TEXT_LSLA[] PROGMEM = "LSLA";
// constexpr char TEXT_LSLB[] PROGMEM = "LSLB";
// constexpr char TEXT_LSR[]  PROGMEM = "LSR";
// constexpr char TEXT_LSRA[] PROGMEM = "LSRA";
// constexpr char TEXT_LSRB[] PROGMEM = "LSRB";
// constexpr char TEXT_NEG[]  PROGMEM = "NEG";
// constexpr char TEXT_NEGA[] PROGMEM = "NEGA";
// constexpr char TEXT_NEGB[] PROGMEM = "NEGB";
// constexpr char TEXT_NOP[]  PROGMEM = "NOP";
// constexpr char TEXT_ORA[]  PROGMEM = "ORA";
// constexpr char TEXT_ORAA[] PROGMEM = "ORAA";
// constexpr char TEXT_ORAB[] PROGMEM = "ORAB";
constexpr char TEXT_PSH[]  PROGMEM = "PSH";
// constexpr char TEXT_PSHA[] PROGMEM = "PSHA";
// constexpr char TEXT_PSHB[] PROGMEM = "PSHB";
constexpr char TEXT_PUL[]  PROGMEM = "PUL";
// constexpr char TEXT_PULA[] PROGMEM = "PULA";
// constexpr char TEXT_PULB[] PROGMEM = "PULB";
// constexpr char TEXT_ROL[]  PROGMEM = "ROL";
// constexpr char TEXT_ROLA[] PROGMEM = "ROLA";
// constexpr char TEXT_ROLB[] PROGMEM = "ROLB";
// constexpr char TEXT_ROR[]  PROGMEM = "ROR";
// constexpr char TEXT_RORA[] PROGMEM = "RORA";
// constexpr char TEXT_RORB[] PROGMEM = "RORB";
// constexpr char TEXT_RTI[]  PROGMEM = "RTI";
// constexpr char TEXT_RTS[]  PROGMEM = "RTS";
// constexpr char TEXT_SBA[]  PROGMEM = "SBA";
// constexpr char TEXT_SBC[]  PROGMEM = "SBC";
// constexpr char TEXT_SBCA[] PROGMEM = "SBCA";
// constexpr char TEXT_SBCB[] PROGMEM = "SBCB";
// constexpr char TEXT_SEC[]  PROGMEM = "SEC";
// constexpr char TEXT_SEI[]  PROGMEM = "SEI";
// constexpr char TEXT_SEV[]  PROGMEM = "SEV";
// constexpr char TEXT_STA[]  PROGMEM = "STA";
// constexpr char TEXT_STAA[] PROGMEM = "STAA";
// constexpr char TEXT_STAB[] PROGMEM = "STAB";
// constexpr char TEXT_STS[]  PROGMEM = "STS";
// constexpr char TEXT_STX[]  PROGMEM = "STX";
// constexpr char TEXT_SUB[]  PROGMEM = "SUB";
// constexpr char TEXT_SUBA[] PROGMEM = "SUBA";
// constexpr char TEXT_SUBB[] PROGMEM = "SUBB";
// constexpr char TEXT_SWI[]  PROGMEM = "SWI";
// constexpr char TEXT_TAB[]  PROGMEM = "TAB";
// constexpr char TEXT_TAP[]  PROGMEM = "TAP";
// constexpr char TEXT_TBA[]  PROGMEM = "TBA";
// constexpr char TEXT_TPA[]  PROGMEM = "TPA";
// constexpr char TEXT_TST[]  PROGMEM = "TST";
// constexpr char TEXT_TSTA[] PROGMEM = "TSTA";
// constexpr char TEXT_TSTB[] PROGMEM = "TSTB";
// constexpr char TEXT_TSX[]  PROGMEM = "TSX";
// constexpr char TEXT_TXS[]  PROGMEM = "TXS";
// constexpr char TEXT_WAI[]  PROGMEM = "WAI";

// MB8861
constexpr char TEXT_ADX[] PROGMEM = "ADX";
constexpr char TEXT_NIM[] PROGMEM = "NIM";
constexpr char TEXT_TMM[] PROGMEM = "TMM";
constexpr char TEXT_XIM[] PROGMEM = "XIM";

// MC6801
// constexpr char TEXT_ABX[]  PROGMEM = "ABX";
// constexpr char TEXT_ADDD[] PROGMEM = "ADDD";
// constexpr char TEXT_ASLD[] PROGMEM = "ASLD";
// constexpr char TEXT_BRN[]  PROGMEM = "BRN";
// constexpr char TEXT_LDD[]  PROGMEM = "LDD";
// constexpr char TEXT_LSLD[] PROGMEM = "LSLD";
// constexpr char TEXT_LSRD[] PROGMEM = "LSRD";
// constexpr char TEXT_MUL[]  PROGMEM = "MUL";
// constexpr char TEXT_STD[]  PROGMEM = "STD";
// constexpr char TEXT_SUBD[] PROGMEM = "SUBD";
// constexpr char TEXT_PSHX[] PROGMEM = "PSHX";
// constexpr char TEXT_PULX[] PROGMEM = "PULX";

// HD6301
// constexpr char TEXT_AIM[]  PROGMEM = "AIM";
// constexpr char TEXT_BCLR[] PROGMEM = "BCLR";
// constexpr char TEXT_BSET[] PROGMEM = "BSET";
constexpr char TEXT_BTGL[] PROGMEM = "BTGL";
// constexpr char TEXT_BTST[] PROGMEM = "BTST";
// constexpr char TEXT_EIM[]  PROGMEM = "EIM";
// constexpr char TEXT_OIM[]  PROGMEM = "OIM";
// constexpr char TEXT_SLP[]  PROGMEM = "SLP";
// constexpr char TEXT_TIM[]  PROGMEM = "TIM";
// constexpr char TEXT_XGDX[] PROGMEM = "XGDX";
 
// // MC68HC11
// constexpr char TEXT_ABY[]   PROGMEM = "ABY";
// constexpr char TEXT_BRCLR[] PROGMEM = "BRCLR";
// constexpr char TEXT_BRSET[] PROGMEM = "BRSET";
// constexpr char TEXT_CPD[]   PROGMEM = "CPD";
// constexpr char TEXT_CPY[]   PROGMEM = "CPY";
// constexpr char TEXT_DEY[]   PROGMEM = "DEY";
// constexpr char TEXT_FDIV[]  PROGMEM = "FDIV";
// constexpr char TEXT_IDIV[]  PROGMEM = "IDIV";
// constexpr char TEXT_INY[]   PROGMEM = "INY";
// constexpr char TEXT_LDY[]   PROGMEM = "LDY";
// constexpr char TEXT_PSHY[]  PROGMEM = "PSHY";
// constexpr char TEXT_PULY[]  PROGMEM = "PULY";
// constexpr char TEXT_STOP[]  PROGMEM = "STOP";
// constexpr char TEXT_STY[]   PROGMEM = "STY";
// constexpr char TEXT_TSY[]   PROGMEM = "TSY";
// constexpr char TEXT_TYS[]   PROGMEM = "TYS";
// constexpr char TEXT_XGDY[]  PROGMEM = "XGDY";
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
