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
const char TEXT_CPU_LIST[] PROGMEM = "MC6805, MC146805, MC68HC05";
const char TEXT_CPU_6805[] PROGMEM = "6805";
const char TEXT_CPU_146805[] PROGMEM = "146805";
const char TEXT_CPU_68HC05[] PROGMEM = "68HC05";

// const char TEXT_ADC[]  PROGMEM = "ADC";
// const char TEXT_ADD[]  PROGMEM = "ADD";
// const char TEXT_AND[]  PROGMEM = "AND";
// const char TEXT_ASR[]  PROGMEM = "ASR";
// const char TEXT_ASRA[] PROGMEM = "ASRA";
const char TEXT_ASRX[] PROGMEM = "ASRX";
// const char TEXT_ASL[]  PROGMEM = "ASL";
// const char TEXT_ASLA[] PROGMEM = "ASLA";
const char TEXT_ASLX[] PROGMEM = "ASLX";
// const char TEXT_BCC[]  PROGMEM = "BCC";
// const char TEXT_BCLR[] PROGMEM = "BCLR";
// const char TEXT_BCS[]  PROGMEM = "BCS";
// const char TEXT_BEQ[]  PROGMEM = "BEQ";
const char TEXT_BHCC[] PROGMEM = "BHCC";
const char TEXT_BHCS[] PROGMEM = "BHCS";
// const char TEXT_BHI[]  PROGMEM = "BHI";
// const char TEXT_BHS[]  PROGMEM = "BHS";
const char TEXT_BIH[]  PROGMEM = "BIH";
const char TEXT_BIL[]  PROGMEM = "BIL";
// const char TEXT_BIT[]  PROGMEM = "BIT";
// const char TEXT_BLO[]  PROGMEM = "BLO";
// const char TEXT_BLS[]  PROGMEM = "BLS";
const char TEXT_BMC[]  PROGMEM = "BMC";
// const char TEXT_BMI[]  PROGMEM = "BMI";
const char TEXT_BMS[]  PROGMEM = "BMS";
// const char TEXT_BNE[]  PROGMEM = "BNE";
// const char TEXT_BPL[]  PROGMEM = "BPL";
// const char TEXT_BRA[]  PROGMEM = "BRA";
// const char TEXT_BRCLR[] PROGMEM = "BRCLR";
// const char TEXT_BRN[]  PROGMEM = "BRN";
// const char TEXT_BRSET[] PROGMEM = "BRSET";
// const char TEXT_BSET[] PROGMEM = "BSET";
// const char TEXT_BSR[]  PROGMEM = "BSR";
// const char TEXT_CLC[]  PROGMEM = "CLC";
// const char TEXT_CLI[]  PROGMEM = "CLI";
// const char TEXT_CLR[]  PROGMEM = "CLR";
// const char TEXT_CLRA[] PROGMEM = "CLRA";
const char TEXT_CLRX[] PROGMEM = "CLRX";
// const char TEXT_CMP[]  PROGMEM = "CMP";
// const char TEXT_COM[]  PROGMEM = "COM";
// const char TEXT_COMA[] PROGMEM = "COMA";
const char TEXT_COMX[] PROGMEM = "COMX";
// const char TEXT_CPX[]  PROGMEM = "CPX";
// const char TEXT_DEC[]  PROGMEM = "DEC";
// const char TEXT_DECA[] PROGMEM = "DECA";
// const char TEXT_DECX[] PROGMEM = "DECX";
// const char TEXT_EOR[]  PROGMEM = "EOR";
// const char TEXT_INC[]  PROGMEM = "INC";
// const char TEXT_INCA[] PROGMEM = "INCA";
// const char TEXT_INCX[] PROGMEM = "INCX";
// const char TEXT_JMP[]  PROGMEM = "JMP";
// const char TEXT_JSR[]  PROGMEM = "JSR";
// const char TEXT_LDA[]  PROGMEM = "LDA";
// const char TEXT_LDX[]  PROGMEM = "LDX";
// const char TEXT_LSL[]  PROGMEM = "LSL";
// const char TEXT_LSLA[] PROGMEM = "LSLA";
const char TEXT_LSLX[] PROGMEM = "LSLX";
// const char TEXT_LSR[]  PROGMEM = "LSR";
// const char TEXT_LSRA[] PROGMEM = "LSRA";
const char TEXT_LSRX[] PROGMEM = "LSRX";
// const char TEXT_MUL[]  PROGMEM = "MUL";
// const char TEXT_NEG[]  PROGMEM = "NEG";
// const char TEXT_NEGA[] PROGMEM = "NEGA";
// const char TEXT_NEGX[] PROGMEM = "NEGX";
// const char TEXT_NOP[]  PROGMEM = "NOP";
// const char TEXT_ORA[]  PROGMEM = "ORA";
// const char TEXT_ROL[]  PROGMEM = "ROL";
// const char TEXT_ROLA[] PROGMEM = "ROLA";
const char TEXT_ROLX[] PROGMEM = "ROLX";
// const char TEXT_ROR[]  PROGMEM = "ROR";
// const char TEXT_RORA[] PROGMEM = "RORA";
const char TEXT_RORX[] PROGMEM = "RORX";
const char TEXT_RSP[]  PROGMEM = "RSP";
// const char TEXT_RTI[]  PROGMEM = "RTI";
// const char TEXT_RTS[]  PROGMEM = "RTS";
// const char TEXT_SBC[]  PROGMEM = "SBC";
// const char TEXT_SEC[]  PROGMEM = "SEC";
// const char TEXT_SEI[]  PROGMEM = "SEI";
// const char TEXT_STA[]  PROGMEM = "STA";
// const char TEXT_STOP[] PROGMEM = "STOP";
// const char TEXT_STX[]  PROGMEM = "STX";
// const char TEXT_SUB[]  PROGMEM = "SUB";
// const char TEXT_SWI[]  PROGMEM = "SWI";
// const char TEXT_TAX[]  PROGMEM = "TAX";
// const char TEXT_TST[]  PROGMEM = "TST";
// const char TEXT_TSTA[] PROGMEM = "TSTA";
const char TEXT_TSTX[] PROGMEM = "TSTX";
// const char TEXT_TXA[]  PROGMEM = "TXA";
// const char TEXT_WAIT[] PROGMEM = "WAIT";
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
