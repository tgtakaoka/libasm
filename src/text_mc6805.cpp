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

constexpr char TEXT_MC6805_LIST[] = "MC6805, MC146805, MC68HC05, MC68HC08, MC68HCS08";
constexpr char TEXT_CPU_6805[]    = "6805";
constexpr char TEXT_CPU_146805[]  = "146805";
constexpr char TEXT_CPU_68HC05[]  = "68HC05";
constexpr char TEXT_CPU_68HC08[]  = "68HC08";
constexpr char TEXT_CPU_68HCS08[] = "68HCS08";

// MC6805
// constexpr char TEXT_ADC[]   = "ADC";
// constexpr char TEXT_ADD[]   = "ADD";
// constexpr char TEXT_AND[]   = "AND";
// constexpr char TEXT_ASL[]   = "ASL";
// constexpr char TEXT_ASLA[]  = "ASLA";
// constexpr char TEXT_ASLX[] = "ASLX";
// constexpr char TEXT_ASR[]   = "ASR";
// constexpr char TEXT_ASRA[]  = "ASRA";
// constexpr char TEXT_ASRX[] = "ASRX";
// constexpr char TEXT_BCC[]   = "BCC";
// constexpr char TEXT_BCLR[]  = "BCLR";
// constexpr char TEXT_BCS[]   = "BCS";
// constexpr char TEXT_BEQ[]   = "BEQ";
constexpr char TEXT_BHCC[] = "BHCC";
constexpr char TEXT_BHCS[] = "BHCS";
// constexpr char TEXT_BHI[]   = "BHI";
// constexpr char TEXT_BHS[]   = "BHS";
constexpr char TEXT_BIH[]  = "BIH";
constexpr char TEXT_BIL[]  = "BIL";
// constexpr char TEXT_BIT[]   = "BIT";
// constexpr char TEXT_BLO[]   = "BLO";
// constexpr char TEXT_BLS[]   = "BLS";
constexpr char TEXT_BMC[]  = "BMC";
// constexpr char TEXT_BMI[]   = "BMI";
constexpr char TEXT_BMS[]  = "BMS";
// constexpr char TEXT_BNE[]   = "BNE";
// constexpr char TEXT_BPL[]   = "BPL";
// constexpr char TEXT_BRA[]   = "BRA";
// constexpr char TEXT_BRCLR[] = "BRCLR";
// constexpr char TEXT_BRN[]   = "BRN";
// constexpr char TEXT_BRSET[] = "BRSET";
// constexpr char TEXT_BSET[]  = "BSET";
// constexpr char TEXT_BSR[]   = "BSR";
// constexpr char TEXT_CLC[]   = "CLC";
// constexpr char TEXT_CLI[]   = "CLI";
// constexpr char TEXT_CLR[]   = "CLR";
// constexpr char TEXT_CLRA[]  = "CLRA";
// constexpr char TEXT_CLRX[] = "CLRX";
// constexpr char TEXT_CMP[]   = "CMP";
// constexpr char TEXT_COM[]   = "COM";
// constexpr char TEXT_COMA[]  = "COMA";
// constexpr char TEXT_COMX[] = "COMX";
// constexpr char TEXT_CPX[]   = "CPX";
// constexpr char TEXT_DEC[]   = "DEC";
// constexpr char TEXT_DECA[]  = "DECA";
// constexpr char TEXT_DECX[]  = "DECX";
// constexpr char TEXT_EOR[]   = "EOR";
// constexpr char TEXT_INC[]   = "INC";
// constexpr char TEXT_INCA[]  = "INCA";
// constexpr char TEXT_INCX[]  = "INCX";
// constexpr char TEXT_JMP[]   = "JMP";
// constexpr char TEXT_JSR[]   = "JSR";
// constexpr char TEXT_LDA[]   = "LDA";
// constexpr char TEXT_LDX[]   = "LDX";
// constexpr char TEXT_LSL[]   = "LSL";
// constexpr char TEXT_LSLA[]  = "LSLA";
// constexpr char TEXT_LSLX[] = "LSLX";
// constexpr char TEXT_LSR[]   = "LSR";
// constexpr char TEXT_LSRA[]  = "LSRA";
// constexpr char TEXT_LSRX[] = "LSRX";
// constexpr char TEXT_MUL[]   = "MUL";
// constexpr char TEXT_NEG[]   = "NEG";
// constexpr char TEXT_NEGA[]  = "NEGA";
// constexpr char TEXT_NEGX[]  = "NEGX";
// constexpr char TEXT_NOP[]   = "NOP";
// constexpr char TEXT_ORA[]   = "ORA";
// constexpr char TEXT_ROL[]   = "ROL";
// constexpr char TEXT_ROLA[]  = "ROLA";
// constexpr char TEXT_ROLX[] = "ROLX";
// constexpr char TEXT_ROR[]   = "ROR";
// constexpr char TEXT_RORA[]  = "RORA";
// constexpr char TEXT_RORX[] = "RORX";
constexpr char TEXT_RSP[]  = "RSP";
// constexpr char TEXT_RTI[]   = "RTI";
// constexpr char TEXT_RTS[]   = "RTS";
// constexpr char TEXT_SBC[]   = "SBC";
// constexpr char TEXT_SEC[]   = "SEC";
// constexpr char TEXT_SEI[]   = "SEI";
// constexpr char TEXT_STA[]   = "STA";
// constexpr char TEXT_STOP[]  = "STOP";
// constexpr char TEXT_STX[]   = "STX";
// constexpr char TEXT_SUB[]   = "SUB";
// constexpr char TEXT_SWI[]   = "SWI";
// constexpr char TEXT_TAX[]   = "TAX";
// constexpr char TEXT_TST[]   = "TST";
// constexpr char TEXT_TSTA[]  = "TSTA";
// constexpr char TEXT_TSTX[] = "TSTX";
// constexpr char TEXT_TXA[]   = "TXA";
// constexpr char TEXT_WAIT[]  = "WAIT";

// MC68HC08
// constexpr char TEXT_AIS[]  = "AIS";
// constexpr char TEXT_AIX[]  = "AIX";
// constexpr char TEXT_BGE[]  = "BGE";
// constexpr char TEXT_BGT[]  = "BGT";
// constexpr char TEXT_BLE[]  = "BLE";
// constexpr char TEXT_BLT[]  = "BLT";
constexpr char TEXT_CBEQ[]  = "CBEQ";
constexpr char TEXT_CBEQA[] = "CBEQA";
constexpr char TEXT_CBEQX[] = "CBEQX";
constexpr char TEXT_CLRH[]  = "CLRH";
constexpr char TEXT_CPHX[]  = "CPHX";
// constexpr char TEXT_DAA[]  = "DAA";
// constexpr char TEXT_DBNZ[] = "DBNZ";
constexpr char TEXT_DBNZA[] = "DBNZA";
constexpr char TEXT_DBNZX[] = "DBNZX";
// constexpr char TEXT_DIV[]  = "DIV";
constexpr char TEXT_LDHX[]  = "LDHX";
// constexpr char TEXT_MOV[]  = "MOV";
constexpr char TEXT_NSA[]   = "NSA";
// constexpr char TEXT_PSHA[] = "PSHA";
constexpr char TEXT_PSHH[]  = "PSHH";
// constexpr char TEXT_PSHX[] = "PSHX";
// constexpr char TEXT_PULA[] = "PULA";
constexpr char TEXT_PULH[]  = "PULH";
// constexpr char TEXT_PULX[] = "PULX";
constexpr char TEXT_STHX[]  = "STHX";
// constexpr char TEXT_TAP[]  = "TAP";
// constexpr char TEXT_TPA[]  = "TPA";
// constexpr char TEXT_TSX[]  = "TSX";
// constexpr char TEXT_TXS[]  = "TXS";

// MC68HCS08
// constexpr char TEXT_BGND[]  = "BGND";

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
