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

constexpr char TEXT_CPU_TMS370[]  = "TMS370";
constexpr char TEXT_CPU_TMS370C[] = "TMS370C";

// constexpr char TEXT_ADC[]  = "ADC";
// constexpr char TEXT_ADD[]  = "ADD";
// constexpr char TEXT_AND[]  = "AND";
// constexpr char TEXT_BR[]   = "BR";
// constexpr char TEXT_BTJO[] = "BTJO";
// constexpr char TEXT_BTJZ[] = "BTJZ";
// constexpr char TEXT_CALL[] = "CALL";
// constexpr char TEXT_CALLR[] = "CALLR";
// constexpr char TEXT_CLR[]  = "CLR";
// constexpr char TEXT_CLRC[] = "CLRC";
// constexpr char TEXT_CMP[]  = "CMP";
constexpr char TEXT_COMPL[] = "COMPL";
// constexpr char TEXT_DAC[]  = "DAC";
// constexpr char TEXT_DEC[]  = "DEC";
// constexpr char TEXT_DINT[] = "DINT";
// constexpr char TEXT_DIV[]  = "DIV";
// constexpr char TEXT_DJNZ[] = "DJNZ";
// constexpr char TEXT_DSB[]  = "DSB";
// constexpr char TEXT_EINT[] = "EINT";
constexpr char TEXT_EINTH[] = "EINTH";
constexpr char TEXT_EINTL[] = "EINTL";
// constexpr char TEXT_IDLE[] = "IDLE";
// constexpr char TEXT_INC[]  = "INC";
// constexpr char TEXT_INCW[] = "INCW";
// constexpr char TEXT_INV[]  = "INV";
// constexpr char TEXT_JC[]   = "JC";;
// constexpr char TEXT_JEQ[]  = "JEQ";;
// constexpr char TEXT_JG[]   = "JG";
// constexpr char TEXT_JHS[]  = "JHS";;
// constexpr char TEXT_JL[]   = "JL";;
// constexpr char TEXT_JLE[]  = "JLE";
constexpr char TEXT_JLO[]   = "JLO";
// constexpr char TEXT_JMP[]  = "JMP";
// constexpr char TEXT_JMPL[] = "JMPL";
// constexpr char TEXT_JN[]   = "JN";;
// constexpr char TEXT_JNC[]  = "JNC";;
// constexpr char TEXT_JNE[]  = "JNE";;
// constexpr char TEXT_JNV[]  = "JNV";
// constexpr char TEXT_JNZ[]  = "JNZ";;
// constexpr char TEXT_JP[]   = "JP";;
// constexpr char TEXT_JPZ[]  = "JPZ";;
// constexpr char TEXT_JV[]   = "JV";
// constexpr char TEXT_JZ[]   = "JZ";;
// constexpr char TEXT_LDSP[] = "LDSP";
constexpr char TEXT_LDST[]  = "LDST";
// constexpr char TEXT_MOV[]  = "MOV";
// constexpr char TEXT_MOVW[] = "MOVW";
// constexpr char TEXT_MPY[]  = "MPY";
// constexpr char TEXT_NOP[]  = "NOP";
// constexpr char TEXT_OR[]   = "OR";
// constexpr char TEXT_POP[]  = "POP";
// constexpr char TEXT_PUSH[] = "PUSH";
// constexpr char TEXT_RL[]   = "RL";
// constexpr char TEXT_RLC[]  = "RLC";
// constexpr char TEXT_RR[]   = "RR";
// constexpr char TEXT_RRC[]  = "RRC";
// constexpr char TEXT_RTI[]  = "RTI";
// constexpr char TEXT_RTS[]  = "RTS";
// constexpr char TEXT_SBB[]  = "SBB";
// constexpr char TEXT_SETC[] = "SETC";
// constexpr char TEXT_STSP[] = "STSP";
// constexpr char TEXT_SUB[]  = "SUB";
// constexpr char TEXT_SWAP[] = "SWAP";
// constexpr char TEXT_TRAP[] = "TRAP";
// constexpr char TEXT_TST[]  = "TST";
// constexpr char TEXT_XCHB[] = "XCHB";
// constexpr char TEXT_XOR[]  = "XOR";

// constexpr char TEXT_REG_A[]  = "A";
// constexpr char TEXT_REG_B[]  = "B";
// constexpr char TEXT_REG_SP[] = "SP";
// constexpr char TEXT_REG_ST[] = "ST";

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
