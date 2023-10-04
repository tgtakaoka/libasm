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
constexpr char TEXT_CPU_I8051[] PROGMEM = "i8051";
constexpr char TEXT_CPU_8051[]  PROGMEM = "8051";

constexpr char TEXT_ACALL[] PROGMEM = "ACALL";
// constexpr char TEXT_ADD[]   PROGMEM = "ADD";
// constexpr char TEXT_ADDC[]  PROGMEM = "ADDC";
constexpr char TEXT_AJMP[]  PROGMEM = "AJMP";
// constexpr char TEXT_ANL[]   PROGMEM = "ANL";
constexpr char TEXT_CJNE[]  PROGMEM = "CJNE";
// constexpr char TEXT_CLR[]   PROGMEM = "CLR";
// constexpr char TEXT_CPL[]   PROGMEM = "CPL";
// constexpr char TEXT_CPU[]   PROGMEM = "CPU";
// constexpr char TEXT_DA[]    PROGMEM = "DA";
// constexpr char TEXT_DEC[]   PROGMEM = "DEC";
// constexpr char TEXT_DIV[]   PROGMEM = "DIV";
// constexpr char TEXT_DJNZ[]  PROGMEM = "DJNZ";
// constexpr char TEXT_INC[]   PROGMEM = "INC";
// constexpr char TEXT_JB[]    PROGMEM = "JB";
// constexpr char TEXT_JBC[]   PROGMEM = "JBC";
// constexpr char TEXT_JC[]    PROGMEM = "JC";
// constexpr char TEXT_JMP[]   PROGMEM = "JMP";
// constexpr char TEXT_JNB[]   PROGMEM = "JNB";
// constexpr char TEXT_JNC[]   PROGMEM = "JNC";
// constexpr char TEXT_JNZ[]   PROGMEM = "JNZ";
// constexpr char TEXT_JZ[]    PROGMEM = "JZ";
// constexpr char TEXT_LCALL[] PROGMEM = "LCALL";
// constexpr char TEXT_LJMP[]  PROGMEM = "LJMP";
// constexpr char TEXT_MOV[]   PROGMEM = "MOV";
constexpr char TEXT_MOVC[]  PROGMEM = "MOVC";
// constexpr char TEXT_MOVX[]  PROGMEM = "MOVX";
// constexpr char TEXT_MUL[]   PROGMEM = "MUL";
// constexpr char TEXT_NOP[]   PROGMEM = "NOP";
// constexpr char TEXT_ORL[]   PROGMEM = "ORL";
// constexpr char TEXT_POP[]   PROGMEM = "POP";
// constexpr char TEXT_PUSH[]  PROGMEM = "PUSH";
// constexpr char TEXT_RET[]   PROGMEM = "RET";
// constexpr char TEXT_RETI[]  PROGMEM = "RETI";
// constexpr char TEXT_RL[]    PROGMEM = "RL";
// constexpr char TEXT_RLC[]   PROGMEM = "RLC";
// constexpr char TEXT_RR[]    PROGMEM = "RR";
// constexpr char TEXT_RRC[]   PROGMEM = "RRC";
// constexpr char TEXT_SETB[]  PROGMEM = "SETB";
// constexpr char TEXT_SJMP[]  PROGMEM = "SJMP";
// constexpr char TEXT_SUBB[]  PROGMEM = "SUBB";
// constexpr char TEXT_SWAP[]  PROGMEM = "SWAP";
// constexpr char TEXT_XCH[]   PROGMEM = "XCH";
// constexpr char TEXT_XCHD[]  PROGMEM = "XCHD";
// constexpr char TEXT_XRL[]   PROGMEM = "XRL";

// constexpr char TEXT_REG_A[]    PROGMEM = "A";
// constexpr char TEXT_REG_C[]    PROGMEM = "C";
constexpr char TEXT_REG_AB[]   PROGMEM = "AB";
// constexpr char TEXT_REG_R0[]   PROGMEM = "R0";
// constexpr char TEXT_REG_R1[]   PROGMEM = "R1";
// constexpr char TEXT_REG_R2[]   PROGMEM = "R2";
// constexpr char TEXT_REG_R3[]   PROGMEM = "R3";
// constexpr char TEXT_REG_R4[]   PROGMEM = "R4";
// constexpr char TEXT_REG_R5[]   PROGMEM = "R5";
// constexpr char TEXT_REG_R6[]   PROGMEM = "R6";
// constexpr char TEXT_REG_R7[]   PROGMEM = "R7";
// constexpr char TEXT_REG_PC[]   PROGMEM = "PC";
constexpr char TEXT_REG_DPTR[] PROGMEM = "DPTR";
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
