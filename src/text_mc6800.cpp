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

constexpr char TEXT_MC6800_LIST[] = "MC6800, MB8861, MC6801, HD6301, MC68HC11";
constexpr char TEXT_CPU_6800[]   = "6800";
constexpr char TEXT_CPU_6801[]   = "6801";
constexpr char TEXT_CPU_6301[]   = "6301";
constexpr char TEXT_CPU_6811[]   = "6811";
constexpr char TEXT_CPU_68HC11[] = "68HC11";
constexpr char TEXT_CPU_HD6301[] = "HD6301";
constexpr char TEXT_CPU_MB8861[] = "MB8861";

// MC6800
// constexpr char TEXT_ABA[]  = "ABA";
// constexpr char TEXT_ADC[]  = "ADC";
// constexpr char TEXT_ADCA[] = "ADCA";
// constexpr char TEXT_ADCB[] = "ADCB";
// constexpr char TEXT_ADD[]  = "ADD";
// constexpr char TEXT_ADDA[] = "ADDA";
// constexpr char TEXT_ADDB[] = "ADDB";
// constexpr char TEXT_AND[]  = "AND";
// constexpr char TEXT_ANDA[] = "ANDA";
// constexpr char TEXT_ANDB[] = "ANDB";
// constexpr char TEXT_ASL[]  = "ASL";
// constexpr char TEXT_ASLA[] = "ASLA";
// constexpr char TEXT_ASLB[] = "ASLB";
// constexpr char TEXT_ASR[]  = "ASR";
// constexpr char TEXT_ASRA[] = "ASRA";
// constexpr char TEXT_ASRB[] = "ASRB";
// constexpr char TEXT_BCC[]  = "BCC";
// constexpr char TEXT_BCS[]  = "BCS";
// constexpr char TEXT_BEQ[]  = "BEQ";
// constexpr char TEXT_BGE[]  = "BGE";
// constexpr char TEXT_BGT[]  = "BGT";
// constexpr char TEXT_BHI[]  = "BHI";
// constexpr char TEXT_BHS[]  = "BHS";
// constexpr char TEXT_BIT[]  = "BIT";
// constexpr char TEXT_BITA[] = "BITA";
// constexpr char TEXT_BITB[] = "BITB";
// constexpr char TEXT_BLE[]  = "BLE";
// constexpr char TEXT_BLO[]  = "BLO";
// constexpr char TEXT_BLS[]  = "BLS";
// constexpr char TEXT_BLT[]  = "BLT";
// constexpr char TEXT_BMI[]  = "BMI";
// constexpr char TEXT_BNE[]  = "BNE";
// constexpr char TEXT_BPL[]  = "BPL";
// constexpr char TEXT_BRA[]  = "BRA";
// constexpr char TEXT_BSR[]  = "BSR";
// constexpr char TEXT_BVC[]  = "BVC";
// constexpr char TEXT_BVS[]  = "BVS";
// constexpr char TEXT_CBA[]  = "CBA";
// constexpr char TEXT_CLC[]  = "CLC";
// constexpr char TEXT_CLI[]  = "CLI";
// constexpr char TEXT_CLR[]  = "CLR";
// constexpr char TEXT_CLRA[] = "CLRA";
// constexpr char TEXT_CLRB[] = "CLRB";
// constexpr char TEXT_CLV[]  = "CLV";
// constexpr char TEXT_CMP[]  = "CMP";
// constexpr char TEXT_CMPA[] = "CMPA";
// constexpr char TEXT_CMPB[] = "CMPB";
// constexpr char TEXT_COM[]  = "COM";
// constexpr char TEXT_COMA[] = "COMA";
// constexpr char TEXT_COMB[] = "COMB";
// constexpr char TEXT_CPX[]  = "CPX";
// constexpr char TEXT_DAA[]  = "DAA";
// constexpr char TEXT_DEC[]  = "DEC";
// constexpr char TEXT_DECA[] = "DECA";
// constexpr char TEXT_DECB[] = "DECB";
// constexpr char TEXT_DES[]  = "DES";
// constexpr char TEXT_DEX[]  = "DEX";
// constexpr char TEXT_EOR[]  = "EOR";
// constexpr char TEXT_EORA[] = "EORA";
// constexpr char TEXT_EORB[] = "EORB";
// constexpr char TEXT_INC[]  = "INC";
// constexpr char TEXT_INCA[] = "INCA";
// constexpr char TEXT_INCB[] = "INCB";
// constexpr char TEXT_INS[]  = "INS";
// constexpr char TEXT_INX[]  = "INX";
// constexpr char TEXT_JMP[]  = "JMP";
// constexpr char TEXT_JSR[]  = "JSR";
// constexpr char TEXT_LDA[]  = "LDA";
// constexpr char TEXT_LDAA[] = "LDAA";
// constexpr char TEXT_LDAB[] = "LDAB";
// constexpr char TEXT_LDS[]  = "LDS";
// constexpr char TEXT_LDX[]  = "LDX";
// constexpr char TEXT_LSL[]  = "LSL";
// constexpr char TEXT_LSLA[] = "LSLA";
// constexpr char TEXT_LSLB[] = "LSLB";
// constexpr char TEXT_LSR[]  = "LSR";
// constexpr char TEXT_LSRA[] = "LSRA";
// constexpr char TEXT_LSRB[] = "LSRB";
// constexpr char TEXT_NEG[]  = "NEG";
// constexpr char TEXT_NEGA[] = "NEGA";
// constexpr char TEXT_NEGB[] = "NEGB";
// constexpr char TEXT_NOP[]  = "NOP";
// constexpr char TEXT_ORA[]  = "ORA";
// constexpr char TEXT_ORAA[] = "ORAA";
// constexpr char TEXT_ORAB[] = "ORAB";
constexpr char TEXT_PSH[] = "PSH";
// constexpr char TEXT_PSHA[] = "PSHA";
// constexpr char TEXT_PSHB[] = "PSHB";
constexpr char TEXT_PUL[] = "PUL";
// constexpr char TEXT_PULA[] = "PULA";
// constexpr char TEXT_PULB[] = "PULB";
// constexpr char TEXT_ROL[]  = "ROL";
// constexpr char TEXT_ROLA[] = "ROLA";
// constexpr char TEXT_ROLB[] = "ROLB";
// constexpr char TEXT_ROR[]  = "ROR";
// constexpr char TEXT_RORA[] = "RORA";
// constexpr char TEXT_RORB[] = "RORB";
// constexpr char TEXT_RTI[]  = "RTI";
// constexpr char TEXT_RTS[]  = "RTS";
// constexpr char TEXT_SBA[]  = "SBA";
// constexpr char TEXT_SBC[]  = "SBC";
// constexpr char TEXT_SBCA[] = "SBCA";
// constexpr char TEXT_SBCB[] = "SBCB";
// constexpr char TEXT_SEC[]  = "SEC";
// constexpr char TEXT_SEI[]  = "SEI";
// constexpr char TEXT_SEV[]  = "SEV";
// constexpr char TEXT_STA[]  = "STA";
// constexpr char TEXT_STAA[] = "STAA";
// constexpr char TEXT_STAB[] = "STAB";
// constexpr char TEXT_STS[]  = "STS";
// constexpr char TEXT_STX[]  = "STX";
// constexpr char TEXT_SUB[]  = "SUB";
// constexpr char TEXT_SUBA[] = "SUBA";
// constexpr char TEXT_SUBB[] = "SUBB";
// constexpr char TEXT_SWI[]  = "SWI";
// constexpr char TEXT_TAB[]  = "TAB";
// constexpr char TEXT_TAP[]  = "TAP";
// constexpr char TEXT_TBA[]  = "TBA";
// constexpr char TEXT_TPA[]  = "TPA";
// constexpr char TEXT_TST[]  = "TST";
// constexpr char TEXT_TSTA[] = "TSTA";
// constexpr char TEXT_TSTB[] = "TSTB";
// constexpr char TEXT_TSX[]  = "TSX";
// constexpr char TEXT_TXS[]  = "TXS";
// constexpr char TEXT_WAI[]  = "WAI";

// MB8861
// constexpr char TEXT_ADX[] = "ADX";
constexpr char TEXT_NIM[] = "NIM";
constexpr char TEXT_TMM[] = "TMM";
constexpr char TEXT_XIM[] = "XIM";

// MC6801
// constexpr char TEXT_ABX[]  = "ABX";
// constexpr char TEXT_ADDD[] = "ADDD";
// constexpr char TEXT_ASLD[] = "ASLD";
// constexpr char TEXT_BRN[]  = "BRN";
// constexpr char TEXT_LDD[]  = "LDD";
// constexpr char TEXT_LSLD[] = "LSLD";
// constexpr char TEXT_LSRD[] = "LSRD";
// constexpr char TEXT_MUL[]  = "MUL";
// constexpr char TEXT_PSHX[] = "PSHX";
// constexpr char TEXT_PULX[] = "PULX";
// constexpr char TEXT_STD[]  = "STD";
// constexpr char TEXT_SUBD[] = "SUBD";

// HD6301
// constexpr char TEXT_AIM[]  = "AIM";
// constexpr char TEXT_BCLR[] = "BCLR";
// constexpr char TEXT_BSET[] = "BSET";
constexpr char TEXT_BTGL[] = "BTGL";
// constexpr char TEXT_BTST[] = "BTST";
// constexpr char TEXT_EIM[]  = "EIM";
// constexpr char TEXT_OIM[]  = "OIM";
// constexpr char TEXT_SLP[]  = "SLP";
// constexpr char TEXT_TIM[]  = "TIM";
// constexpr char TEXT_XGDX[] = "XGDX";
 
// MC68HC11
// constexpr char TEXT_ABY[]   = "ABY";
// constexpr char TEXT_BRCLR[] = "BRCLR";
// constexpr char TEXT_BRSET[] = "BRSET";
// constexpr char TEXT_CPD[]   = "CPD";
// constexpr char TEXT_CPY[]   = "CPY";
// constexpr char TEXT_DEY[]   = "DEY";
// constexpr char TEXT_FDIV[]  = "FDIV";
// constexpr char TEXT_IDIV[]  = "IDIV";
// constexpr char TEXT_INY[]   = "INY";
// constexpr char TEXT_LDY[]   = "LDY";
// constexpr char TEXT_PSHY[]  = "PSHY";
// constexpr char TEXT_PULY[]  = "PULY";
// constexpr char TEXT_STOP[]  = "STOP";
// constexpr char TEXT_STY[]   = "STY";
// constexpr char TEXT_TSY[]   = "TSY";
// constexpr char TEXT_TYS[]   = "TYS";
// constexpr char TEXT_XGDY[]  = "XGDY";

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
