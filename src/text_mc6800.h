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

#ifndef __TEXT_MC6800__
#define __TEXT_MC6800__

#include "config_host.h"

namespace libasm {
namespace mc6800 {

// clang-format off
static const char TEXT_CPU_LIST[] PROGMEM = "MC6800, MC6801, HD6301, MC68HC11";
static const char TEXT_CPU_6800[] PROGMEM = "6800";
static const char TEXT_CPU_6801[] PROGMEM = "6801";
static const char TEXT_CPU_6301[] PROGMEM = "6301";
static const char TEXT_CPU_6811[] PROGMEM = "6811";
static constexpr const char *TEXT_CPU_MC   = TEXT_CPU_LIST;
static constexpr const char *TEXT_CPU_HD   = (TEXT_CPU_LIST + 16);
static constexpr const char *TEXT_CPU_68HC11 = (TEXT_CPU_LIST + 26);

// MC6800
static const char TEXT_ABA[]  PROGMEM = "ABA";
static const char TEXT_ADC[]  PROGMEM = "ADC";
static const char TEXT_ADCA[] PROGMEM = "ADCA";
static const char TEXT_ADCB[] PROGMEM = "ADCB";
static const char TEXT_ADD[]  PROGMEM = "ADD";
static const char TEXT_ADDA[] PROGMEM = "ADDA";
static const char TEXT_ADDB[] PROGMEM = "ADDB";
static const char TEXT_AND[]  PROGMEM = "AND";
static const char TEXT_ANDA[] PROGMEM = "ANDA";
static const char TEXT_ANDB[] PROGMEM = "ANDB";
static const char TEXT_ASL[]  PROGMEM = "ASL";
static const char TEXT_ASLA[] PROGMEM = "ASLA";
static const char TEXT_ASLB[] PROGMEM = "ASLB";
static const char TEXT_ASR[]  PROGMEM = "ASR";
static const char TEXT_ASRA[] PROGMEM = "ASRA";
static const char TEXT_ASRB[] PROGMEM = "ASRB";
static const char TEXT_BCC[]  PROGMEM = "BCC";
static const char TEXT_BCS[]  PROGMEM = "BCS";
static const char TEXT_BEQ[]  PROGMEM = "BEQ";
static const char TEXT_BGE[]  PROGMEM = "BGE";
static const char TEXT_BGT[]  PROGMEM = "BGT";
static const char TEXT_BHI[]  PROGMEM = "BHI";
static const char TEXT_BHS[]  PROGMEM = "BHS";
static const char TEXT_BIT[]  PROGMEM = "BIT";
static const char TEXT_BITA[] PROGMEM = "BITA";
static const char TEXT_BITB[] PROGMEM = "BITB";
static const char TEXT_BLE[]  PROGMEM = "BLE";
static const char TEXT_BLO[]  PROGMEM = "BLO";
static const char TEXT_BLS[]  PROGMEM = "BLS";
static const char TEXT_BLT[]  PROGMEM = "BLT";
static const char TEXT_BMI[]  PROGMEM = "BMI";
static const char TEXT_BNE[]  PROGMEM = "BNE";
static const char TEXT_BPL[]  PROGMEM = "BPL";
static const char TEXT_BRA[]  PROGMEM = "BRA";
static const char TEXT_BSR[]  PROGMEM = "BSR";
static const char TEXT_BVC[]  PROGMEM = "BVC";
static const char TEXT_BVS[]  PROGMEM = "BVS";
static const char TEXT_CBA[]  PROGMEM = "CBA";
static const char TEXT_CLC[]  PROGMEM = "CLC";
static const char TEXT_CLI[]  PROGMEM = "CLI";
static const char TEXT_CLR[]  PROGMEM = "CLR";
static const char TEXT_CLRA[] PROGMEM = "CLRA";
static const char TEXT_CLRB[] PROGMEM = "CLRB";
static const char TEXT_CLV[]  PROGMEM = "CLV";
static const char TEXT_CMP[]  PROGMEM = "CMP";
static const char TEXT_CMPA[] PROGMEM = "CMPA";
static const char TEXT_CMPB[] PROGMEM = "CMPB";
static const char TEXT_COM[]  PROGMEM = "COM";
static const char TEXT_COMA[] PROGMEM = "COMA";
static const char TEXT_COMB[] PROGMEM = "COMB";
static const char TEXT_CPX[]  PROGMEM = "CPX";
static const char TEXT_DAA[]  PROGMEM = "DAA";
static const char TEXT_DEC[]  PROGMEM = "DEC";
static const char TEXT_DECA[] PROGMEM = "DECA";
static const char TEXT_DECB[] PROGMEM = "DECB";
static const char TEXT_DES[]  PROGMEM = "DES";
static const char TEXT_DEX[]  PROGMEM = "DEX";
static const char TEXT_EOR[]  PROGMEM = "EOR";
static const char TEXT_EORA[] PROGMEM = "EORA";
static const char TEXT_EORB[] PROGMEM = "EORB";
static const char TEXT_INC[]  PROGMEM = "INC";
static const char TEXT_INCA[] PROGMEM = "INCA";
static const char TEXT_INCB[] PROGMEM = "INCB";
static const char TEXT_INS[]  PROGMEM = "INS";
static const char TEXT_INX[]  PROGMEM = "INX";
static const char TEXT_JMP[]  PROGMEM = "JMP";
static const char TEXT_JSR[]  PROGMEM = "JSR";
static const char TEXT_LDA[]  PROGMEM = "LDA";
static const char TEXT_LDAA[] PROGMEM = "LDAA";
static const char TEXT_LDAB[] PROGMEM = "LDAB";
static const char TEXT_LDS[]  PROGMEM = "LDS";
static const char TEXT_LDX[]  PROGMEM = "LDX";
static const char TEXT_LSL[]  PROGMEM = "LSL";
static const char TEXT_LSLA[] PROGMEM = "LSLA";
static const char TEXT_LSLB[] PROGMEM = "LSLB";
static const char TEXT_LSR[]  PROGMEM = "LSR";
static const char TEXT_LSRA[] PROGMEM = "LSRA";
static const char TEXT_LSRB[] PROGMEM = "LSRB";
static const char TEXT_NEG[]  PROGMEM = "NEG";
static const char TEXT_NEGA[] PROGMEM = "NEGA";
static const char TEXT_NEGB[] PROGMEM = "NEGB";
static const char TEXT_NOP[]  PROGMEM = "NOP";
static const char TEXT_ORA[]  PROGMEM = "ORA";
static const char TEXT_ORAA[] PROGMEM = "ORAA";
static const char TEXT_ORAB[] PROGMEM = "ORAB";
static const char TEXT_PSH[]  PROGMEM = "PSH";
static const char TEXT_PSHA[] PROGMEM = "PSHA";
static const char TEXT_PSHB[] PROGMEM = "PSHB";
static const char TEXT_PUL[]  PROGMEM = "PUL";
static const char TEXT_PULA[] PROGMEM = "PULA";
static const char TEXT_PULB[] PROGMEM = "PULB";
static const char TEXT_ROL[]  PROGMEM = "ROL";
static const char TEXT_ROLA[] PROGMEM = "ROLA";
static const char TEXT_ROLB[] PROGMEM = "ROLB";
static const char TEXT_ROR[]  PROGMEM = "ROR";
static const char TEXT_RORA[] PROGMEM = "RORA";
static const char TEXT_RORB[] PROGMEM = "RORB";
static const char TEXT_RTI[]  PROGMEM = "RTI";
static const char TEXT_RTS[]  PROGMEM = "RTS";
static const char TEXT_SBA[]  PROGMEM = "SBA";
static const char TEXT_SBC[]  PROGMEM = "SBC";
static const char TEXT_SBCA[] PROGMEM = "SBCA";
static const char TEXT_SBCB[] PROGMEM = "SBCB";
static const char TEXT_SEC[]  PROGMEM = "SEC";
static const char TEXT_SEI[]  PROGMEM = "SEI";
static const char TEXT_SEV[]  PROGMEM = "SEV";
static const char TEXT_STA[]  PROGMEM = "STA";
static const char TEXT_STAA[] PROGMEM = "STAA";
static const char TEXT_STAB[] PROGMEM = "STAB";
static const char TEXT_STS[]  PROGMEM = "STS";
static const char TEXT_STX[]  PROGMEM = "STX";
static const char TEXT_SUB[]  PROGMEM = "SUB";
static const char TEXT_SUBA[] PROGMEM = "SUBA";
static const char TEXT_SUBB[] PROGMEM = "SUBB";
static const char TEXT_SWI[]  PROGMEM = "SWI";
static const char TEXT_TAB[]  PROGMEM = "TAB";
static const char TEXT_TAP[]  PROGMEM = "TAP";
static const char TEXT_TBA[]  PROGMEM = "TBA";
static const char TEXT_TPA[]  PROGMEM = "TPA";
static const char TEXT_TST[]  PROGMEM = "TST";
static const char TEXT_TSTA[] PROGMEM = "TSTA";
static const char TEXT_TSTB[] PROGMEM = "TSTB";
static const char TEXT_TSX[]  PROGMEM = "TSX";
static const char TEXT_TXS[]  PROGMEM = "TXS";
static const char TEXT_WAI[]  PROGMEM = "WAI";

// MC6801
static const char TEXT_ABX[]  PROGMEM = "ABX";
static const char TEXT_ADDD[] PROGMEM = "ADDD";
static const char TEXT_ASLD[] PROGMEM = "ASLD";
static const char TEXT_BRN[]  PROGMEM = "BRN";
static const char TEXT_LDD[]  PROGMEM = "LDD";
static const char TEXT_LSLD[] PROGMEM = "LSLD";
static const char TEXT_LSRD[] PROGMEM = "LSRD";
static const char TEXT_MUL[]  PROGMEM = "MUL";
static const char TEXT_STD[]  PROGMEM = "STD";
static const char TEXT_SUBD[] PROGMEM = "SUBD";
static const char TEXT_PSHX[] PROGMEM = "PSHX";
static const char TEXT_PULX[] PROGMEM = "PULX";

// HD6301
static const char TEXT_AIM[]  PROGMEM = "AIM";
static const char TEXT_BCLR[] PROGMEM = "BCLR";
static const char TEXT_BSET[] PROGMEM = "BSET";
static const char TEXT_BTGL[] PROGMEM = "BTGL";
static const char TEXT_BTST[] PROGMEM = "BTST";
static const char TEXT_EIM[]  PROGMEM = "EIM";
static const char TEXT_OIM[]  PROGMEM = "OIM";
static const char TEXT_SLP[]  PROGMEM = "SLP";
static const char TEXT_TIM[]  PROGMEM = "TIM";
static const char TEXT_XGDX[] PROGMEM = "XGDX";

// MC68HC11
static const char TEXT_ABY[]   PROGMEM = "ABY";
static const char TEXT_BRCLR[] PROGMEM = "BRCLR";
static const char TEXT_BRSET[] PROGMEM = "BRSET";
static const char TEXT_CPD[]   PROGMEM = "CPD";
static const char TEXT_CPY[]   PROGMEM = "CPY";
static const char TEXT_DEY[]   PROGMEM = "DEY";
static const char TEXT_FDIV[]  PROGMEM = "FDIV";
static const char TEXT_IDIV[]  PROGMEM = "IDIV";
static const char TEXT_INY[]   PROGMEM = "INY";
static const char TEXT_LDY[]   PROGMEM = "LDY";
static const char TEXT_PSHY[]  PROGMEM = "PSHY";
static const char TEXT_PULY[]  PROGMEM = "PULY";
static const char TEXT_STOP[]  PROGMEM = "STOP";
static const char TEXT_STY[]   PROGMEM = "STY";
static const char TEXT_TSY[]   PROGMEM = "TSY";
static const char TEXT_TYS[]   PROGMEM = "TYS";
static const char TEXT_XGDY[]  PROGMEM = "XGDY";
// clang-format on

}  // namespace mc6800
}  // namespace libasm

#endif  // __TEXT_MC6800__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
