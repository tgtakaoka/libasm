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

#include "text_i8051.h"

namespace libasm {
namespace text {
namespace i8051 {

// clang-format off
constexpr char TEXT_CPU_I8051[] = "i8051";
constexpr char TEXT_CPU_8051[]  = "8051";

constexpr char TEXT_ACALL[] = "ACALL";
// constexpr char TEXT_ADD[]   = "ADD";
// constexpr char TEXT_ADDC[]  = "ADDC";
constexpr char TEXT_AJMP[]  = "AJMP";
// constexpr char TEXT_ANL[]   = "ANL";
constexpr char TEXT_CJNE[]  = "CJNE";
// constexpr char TEXT_CLR[]   = "CLR";
// constexpr char TEXT_CPL[]   = "CPL";
// constexpr char TEXT_CPU[]   = "CPU";
// constexpr char TEXT_DA[]    = "DA";
// constexpr char TEXT_DEC[]   = "DEC";
// constexpr char TEXT_DIV[]   = "DIV";
// constexpr char TEXT_DJNZ[]  = "DJNZ";
// constexpr char TEXT_INC[]   = "INC";
// constexpr char TEXT_JB[]    = "JB";
// constexpr char TEXT_JBC[]   = "JBC";
// constexpr char TEXT_JC[]    = "JC";
// constexpr char TEXT_JMP[]   = "JMP";
// constexpr char TEXT_JNB[]   = "JNB";
// constexpr char TEXT_JNC[]   = "JNC";
// constexpr char TEXT_JNZ[]   = "JNZ";
// constexpr char TEXT_JZ[]    = "JZ";
// constexpr char TEXT_LCALL[] = "LCALL";
// constexpr char TEXT_LJMP[]  = "LJMP";
// constexpr char TEXT_MOV[]   = "MOV";
constexpr char TEXT_MOVC[]  = "MOVC";
// constexpr char TEXT_MOVX[]  = "MOVX";
// constexpr char TEXT_MUL[]   = "MUL";
// constexpr char TEXT_NOP[]   = "NOP";
// constexpr char TEXT_ORL[]   = "ORL";
// constexpr char TEXT_POP[]   = "POP";
// constexpr char TEXT_PUSH[]  = "PUSH";
// constexpr char TEXT_RET[]   = "RET";
// constexpr char TEXT_RETI[]  = "RETI";
// constexpr char TEXT_RL[]    = "RL";
// constexpr char TEXT_RLC[]   = "RLC";
// constexpr char TEXT_RR[]    = "RR";
// constexpr char TEXT_RRC[]   = "RRC";
// constexpr char TEXT_SETB[]  = "SETB";
// constexpr char TEXT_SJMP[]  = "SJMP";
// constexpr char TEXT_SUBB[]  = "SUBB";
// constexpr char TEXT_SWAP[]  = "SWAP";
// constexpr char TEXT_XCH[]   = "XCH";
// constexpr char TEXT_XCHD[]  = "XCHD";
// constexpr char TEXT_XRL[]   = "XRL";

// constexpr char TEXT_REG_A[]  = "A";
constexpr char TEXT_REG_AB[]   = "AB";
// constexpr char TEXT_REG_C[]  = "C";
constexpr char TEXT_REG_DPTR[] = "DPTR";
// constexpr char TEXT_REG_PC[] = "PC";
// constexpr char TEXT_REG_R0[] = "R0";
// constexpr char TEXT_REG_R1[] = "R1";
// constexpr char TEXT_REG_R2[] = "R2";
// constexpr char TEXT_REG_R3[] = "R3";
// constexpr char TEXT_REG_R4[] = "R4";
// constexpr char TEXT_REG_R5[] = "R5";
// constexpr char TEXT_REG_R6[] = "R6";
// constexpr char TEXT_REG_R7[] = "R7";

// clang-format on
}  // namespace i8051
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
