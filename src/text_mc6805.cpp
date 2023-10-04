/*
 * Copyright 2021 Tadashi G. Takaoka
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

#include "text_mc6805.h"

namespace libasm {
namespace text {
namespace mc6805 {

// clang-format off
constexpr char TEXT_MC6805_LIST[] PROGMEM = "MC6805, MC146805, MC68HC05";
constexpr char TEXT_CPU_6805[] PROGMEM = "6805";
constexpr char TEXT_CPU_146805[] PROGMEM = "146805";
constexpr char TEXT_CPU_68HC05[] PROGMEM = "68HC05";

// constexpr char TEXT_ADC[]  PROGMEM = "ADC";
// constexpr char TEXT_ADD[]  PROGMEM = "ADD";
// constexpr char TEXT_AND[]  PROGMEM = "AND";
// constexpr char TEXT_ASR[]  PROGMEM = "ASR";
// constexpr char TEXT_ASRA[] PROGMEM = "ASRA";
constexpr char TEXT_ASRX[] PROGMEM = "ASRX";
// constexpr char TEXT_ASL[]  PROGMEM = "ASL";
// constexpr char TEXT_ASLA[] PROGMEM = "ASLA";
constexpr char TEXT_ASLX[] PROGMEM = "ASLX";
// constexpr char TEXT_BCC[]  PROGMEM = "BCC";
// constexpr char TEXT_BCLR[] PROGMEM = "BCLR";
// constexpr char TEXT_BCS[]  PROGMEM = "BCS";
// constexpr char TEXT_BEQ[]  PROGMEM = "BEQ";
constexpr char TEXT_BHCC[] PROGMEM = "BHCC";
constexpr char TEXT_BHCS[] PROGMEM = "BHCS";
// constexpr char TEXT_BHI[]  PROGMEM = "BHI";
// constexpr char TEXT_BHS[]  PROGMEM = "BHS";
constexpr char TEXT_BIH[]  PROGMEM = "BIH";
constexpr char TEXT_BIL[]  PROGMEM = "BIL";
// constexpr char TEXT_BIT[]  PROGMEM = "BIT";
// constexpr char TEXT_BLO[]  PROGMEM = "BLO";
// constexpr char TEXT_BLS[]  PROGMEM = "BLS";
constexpr char TEXT_BMC[]  PROGMEM = "BMC";
// constexpr char TEXT_BMI[]  PROGMEM = "BMI";
constexpr char TEXT_BMS[]  PROGMEM = "BMS";
// constexpr char TEXT_BNE[]  PROGMEM = "BNE";
// constexpr char TEXT_BPL[]  PROGMEM = "BPL";
// constexpr char TEXT_BRA[]  PROGMEM = "BRA";
// constexpr char TEXT_BRCLR[] PROGMEM = "BRCLR";
// constexpr char TEXT_BRN[]  PROGMEM = "BRN";
// constexpr char TEXT_BRSET[] PROGMEM = "BRSET";
// constexpr char TEXT_BSET[] PROGMEM = "BSET";
// constexpr char TEXT_BSR[]  PROGMEM = "BSR";
// constexpr char TEXT_CLC[]  PROGMEM = "CLC";
// constexpr char TEXT_CLI[]  PROGMEM = "CLI";
// constexpr char TEXT_CLR[]  PROGMEM = "CLR";
// constexpr char TEXT_CLRA[] PROGMEM = "CLRA";
constexpr char TEXT_CLRX[] PROGMEM = "CLRX";
// constexpr char TEXT_CMP[]  PROGMEM = "CMP";
// constexpr char TEXT_COM[]  PROGMEM = "COM";
// constexpr char TEXT_COMA[] PROGMEM = "COMA";
constexpr char TEXT_COMX[] PROGMEM = "COMX";
// constexpr char TEXT_CPX[]  PROGMEM = "CPX";
// constexpr char TEXT_DEC[]  PROGMEM = "DEC";
// constexpr char TEXT_DECA[] PROGMEM = "DECA";
// constexpr char TEXT_DECX[] PROGMEM = "DECX";
// constexpr char TEXT_EOR[]  PROGMEM = "EOR";
// constexpr char TEXT_INC[]  PROGMEM = "INC";
// constexpr char TEXT_INCA[] PROGMEM = "INCA";
// constexpr char TEXT_INCX[] PROGMEM = "INCX";
// constexpr char TEXT_JMP[]  PROGMEM = "JMP";
// constexpr char TEXT_JSR[]  PROGMEM = "JSR";
// constexpr char TEXT_LDA[]  PROGMEM = "LDA";
// constexpr char TEXT_LDX[]  PROGMEM = "LDX";
// constexpr char TEXT_LSL[]  PROGMEM = "LSL";
// constexpr char TEXT_LSLA[] PROGMEM = "LSLA";
constexpr char TEXT_LSLX[] PROGMEM = "LSLX";
// constexpr char TEXT_LSR[]  PROGMEM = "LSR";
// constexpr char TEXT_LSRA[] PROGMEM = "LSRA";
constexpr char TEXT_LSRX[] PROGMEM = "LSRX";
// constexpr char TEXT_MUL[]  PROGMEM = "MUL";
// constexpr char TEXT_NEG[]  PROGMEM = "NEG";
// constexpr char TEXT_NEGA[] PROGMEM = "NEGA";
// constexpr char TEXT_NEGX[] PROGMEM = "NEGX";
// constexpr char TEXT_NOP[]  PROGMEM = "NOP";
// constexpr char TEXT_ORA[]  PROGMEM = "ORA";
// constexpr char TEXT_ROL[]  PROGMEM = "ROL";
// constexpr char TEXT_ROLA[] PROGMEM = "ROLA";
constexpr char TEXT_ROLX[] PROGMEM = "ROLX";
// constexpr char TEXT_ROR[]  PROGMEM = "ROR";
// constexpr char TEXT_RORA[] PROGMEM = "RORA";
constexpr char TEXT_RORX[] PROGMEM = "RORX";
constexpr char TEXT_RSP[]  PROGMEM = "RSP";
// constexpr char TEXT_RTI[]  PROGMEM = "RTI";
// constexpr char TEXT_RTS[]  PROGMEM = "RTS";
// constexpr char TEXT_SBC[]  PROGMEM = "SBC";
// constexpr char TEXT_SEC[]  PROGMEM = "SEC";
// constexpr char TEXT_SEI[]  PROGMEM = "SEI";
// constexpr char TEXT_STA[]  PROGMEM = "STA";
// constexpr char TEXT_STOP[] PROGMEM = "STOP";
// constexpr char TEXT_STX[]  PROGMEM = "STX";
// constexpr char TEXT_SUB[]  PROGMEM = "SUB";
// constexpr char TEXT_SWI[]  PROGMEM = "SWI";
// constexpr char TEXT_TAX[]  PROGMEM = "TAX";
// constexpr char TEXT_TST[]  PROGMEM = "TST";
// constexpr char TEXT_TSTA[] PROGMEM = "TSTA";
constexpr char TEXT_TSTX[] PROGMEM = "TSTX";
// constexpr char TEXT_TXA[]  PROGMEM = "TXA";
// constexpr char TEXT_WAIT[] PROGMEM = "WAIT";
// clang-format on

}  // namespace mc6805
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
