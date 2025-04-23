/*
 * Copyright 2025 Tadashi G. Takaoka
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

#include "text_tms370.h"

namespace libasm {
namespace text {
namespace tms370 {

// clang-format off
constexpr char TEXT_CPU_TMS370[]  PROGMEM = "TMS370";
constexpr char TEXT_CPU_TMS370C[] PROGMEM = "TMS370C";

// constexpr char TEXT_ADC[]   PROGMEM = "ADC";
// constexpr char TEXT_ADD[]   PROGMEM = "ADD";
// constexpr char TEXT_AND[]   PROGMEM = "AND";
constexpr char TEXT_BTJO[]  PROGMEM = "BTJO";
constexpr char TEXT_BTJZ[]  PROGMEM = "BTJZ";
// constexpr char TEXT_BR[]    PROGMEM = "BR";
// constexpr char TEXT_CALL[]  PROGMEM = "CALL";
constexpr char TEXT_CALLR[] PROGMEM = "CALLR";
// constexpr char TEXT_CLR[]   PROGMEM = "CLR";
// constexpr char TEXT_CLRC[]  PROGMEM = "CLRC";
// constexpr char TEXT_CMP[]   PROGMEM = "CMP";
constexpr char TEXT_COMPL[] PROGMEM = "COMPL";
constexpr char TEXT_DAC[]   PROGMEM = "DAC";
// constexpr char TEXT_DEC[]   PROGMEM = "DEC";
// constexpr char TEXT_DINT[]  PROGMEM = "DINT";
// constexpr char TEXT_DIV[]   PROGMEM = "DIV";
// constexpr char TEXT_DJNZ[]  PROGMEM = "DJNZ";
// constexpr char TEXT_DSB[]   PROGMEM = "DSB";
// constexpr char TEXT_EINT[]  PROGMEM = "EINT";
constexpr char TEXT_EINTH[] PROGMEM = "EINTH";
constexpr char TEXT_EINTL[] PROGMEM = "EINTL";
// constexpr char TEXT_IDLE[]  PROGMEM = "IDLE";
// constexpr char TEXT_INC[]   PROGMEM = "INC";
// constexpr char TEXT_INCW[]  PROGMEM = "INCW";
// constexpr char TEXT_INV[]   PROGMEM = "INV";
// constexpr char TEXT_JC[]    PROGMEM = "JC";;
// constexpr char TEXT_JEQ[]   PROGMEM = "JEQ";;
// constexpr char TEXT_JG[]    PROGMEM = "JG";
constexpr char TEXT_JHS[]   PROGMEM = "JHS";;
// constexpr char TEXT_JL[]    PROGMEM = "JL";;
// constexpr char TEXT_JLE[]   PROGMEM = "JLE";
constexpr char TEXT_JLO[]   PROGMEM = "JLO";
// constexpr char TEXT_JMP[]   PROGMEM = "JMP";
// constexpr char TEXT_JMPL[]  PROGMEM = "JMPL";
// constexpr char TEXT_JNC[]   PROGMEM = "JNC";;
// constexpr char TEXT_JNE[]   PROGMEM = "JNE";;
constexpr char TEXT_JN[]    PROGMEM = "JN";;
// constexpr char TEXT_JNV[]   PROGMEM = "JNV";
// constexpr char TEXT_JNZ[]   PROGMEM = "JNZ";;
// constexpr char TEXT_JP[]    PROGMEM = "JP";;
constexpr char TEXT_JPZ[]   PROGMEM = "JPZ";;
// constexpr char TEXT_JV[]    PROGMEM = "JV";
// constexpr char TEXT_JZ[]    PROGMEM = "JZ";;
constexpr char TEXT_LDSP[]  PROGMEM = "LDSP";
constexpr char TEXT_LDST[]  PROGMEM = "LDST";
// constexpr char TEXT_MOV[]   PROGMEM = "MOV";
// constexpr char TEXT_MOVW[]  PROGMEM = "MOVW";
// constexpr char TEXT_MPY[]   PROGMEM = "MPY";
// constexpr char TEXT_NOP[]   PROGMEM = "NOP";
// constexpr char TEXT_OR[]    PROGMEM = "OR";
// constexpr char TEXT_POP[]   PROGMEM = "POP";
// constexpr char TEXT_PUSH[]  PROGMEM = "PUSH";
// constexpr char TEXT_RL[]    PROGMEM = "RL";
// constexpr char TEXT_RLC[]   PROGMEM = "RLC";
// constexpr char TEXT_RR[]    PROGMEM = "RR";
// constexpr char TEXT_RRC[]   PROGMEM = "RRC";
// constexpr char TEXT_RTI[]   PROGMEM = "RTI";
// constexpr char TEXT_RTS[]   PROGMEM = "RTS";
// constexpr char TEXT_SBB[]   PROGMEM = "SBB";
// constexpr char TEXT_SETC[]  PROGMEM = "SETC";
constexpr char TEXT_STSP[] PROGMEM = "STSP";
// constexpr char TEXT_SUB[]   PROGMEM = "SUB";
// constexpr char TEXT_SWAP[]  PROGMEM = "SWAP";
// constexpr char TEXT_TRAP[]  PROGMEM = "TRAP";
// constexpr char TEXT_TST[]   PROGMEM = "TST";
constexpr char TEXT_XCHB[] PROGMEM = "XCHB";
// constexpr char TEXT_XOR[]   PROGMEM = "XOR";

// constexpr char TEXT_REG_A[]  PROGMEM = "A";
// constexpr char TEXT_REG_B[]  PROGMEM = "B";
// constexpr char TEXT_REG_SP[] PROGMEM = "SP";
// constexpr char TEXT_REG_ST[] PROGMEM = "ST";

// clang-format on

}  // namespace tms370
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
