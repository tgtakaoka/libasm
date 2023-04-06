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
const char TEXT_CPU_LIST[] PROGMEM = "i8051";
const char TEXT_CPU_8051[] PROGMEM = "8051";

const char TEXT_ACALL[] PROGMEM = "ACALL";
// const char TEXT_ADD[]   PROGMEM = "ADD";
// const char TEXT_ADDC[]  PROGMEM = "ADDC";
const char TEXT_AJMP[]  PROGMEM = "AJMP";
// const char TEXT_ANL[]   PROGMEM = "ANL";
const char TEXT_CJNE[]  PROGMEM = "CJNE";
// const char TEXT_CLR[]   PROGMEM = "CLR";
// const char TEXT_CPL[]   PROGMEM = "CPL";
// const char TEXT_CPU[]   PROGMEM = "CPU";
// const char TEXT_DA[]    PROGMEM = "DA";
// const char TEXT_DEC[]   PROGMEM = "DEC";
// const char TEXT_DIV[]   PROGMEM = "DIV";
// const char TEXT_DJNZ[]  PROGMEM = "DJNZ";
// const char TEXT_INC[]   PROGMEM = "INC";
// const char TEXT_JB[]    PROGMEM = "JB";
// const char TEXT_JBC[]   PROGMEM = "JBC";
// const char TEXT_JC[]    PROGMEM = "JC";
// const char TEXT_JMP[]   PROGMEM = "JMP";
// const char TEXT_JNB[]   PROGMEM = "JNB";
// const char TEXT_JNC[]   PROGMEM = "JNC";
// const char TEXT_JNZ[]   PROGMEM = "JNZ";
// const char TEXT_JZ[]    PROGMEM = "JZ";
// const char TEXT_LCALL[] PROGMEM = "LCALL";
// const char TEXT_LJMP[]  PROGMEM = "LJMP";
// const char TEXT_MOV[]   PROGMEM = "MOV";
const char TEXT_MOVC[]  PROGMEM = "MOVC";
// const char TEXT_MOVX[]  PROGMEM = "MOVX";
// const char TEXT_MUL[]   PROGMEM = "MUL";
// const char TEXT_NOP[]   PROGMEM = "NOP";
// const char TEXT_ORL[]   PROGMEM = "ORL";
// const char TEXT_POP[]   PROGMEM = "POP";
// const char TEXT_PUSH[]  PROGMEM = "PUSH";
// const char TEXT_RET[]   PROGMEM = "RET";
// const char TEXT_RETI[]  PROGMEM = "RETI";
// const char TEXT_RL[]    PROGMEM = "RL";
// const char TEXT_RLC[]   PROGMEM = "RLC";
// const char TEXT_RR[]    PROGMEM = "RR";
// const char TEXT_RRC[]   PROGMEM = "RRC";
// const char TEXT_SETB[]  PROGMEM = "SETB";
// const char TEXT_SJMP[]  PROGMEM = "SJMP";
// const char TEXT_SUBB[]  PROGMEM = "SUBB";
// const char TEXT_SWAP[]  PROGMEM = "SWAP";
// const char TEXT_XCH[]   PROGMEM = "XCH";
// const char TEXT_XCHD[]  PROGMEM = "XCHD";
// const char TEXT_XRL[]   PROGMEM = "XRL";

// const char TEXT_REG_A[]    PROGMEM = "A";
// const char TEXT_REG_C[]    PROGMEM = "C";
const char TEXT_REG_AB[]   PROGMEM = "AB";
// const char TEXT_REG_R0[]   PROGMEM = "R0";
// const char TEXT_REG_R1[]   PROGMEM = "R1";
// const char TEXT_REG_R2[]   PROGMEM = "R2";
// const char TEXT_REG_R3[]   PROGMEM = "R3";
// const char TEXT_REG_R4[]   PROGMEM = "R4";
// const char TEXT_REG_R5[]   PROGMEM = "R5";
// const char TEXT_REG_R6[]   PROGMEM = "R6";
// const char TEXT_REG_R7[]   PROGMEM = "R7";
// const char TEXT_REG_PC[]   PROGMEM = "PC";
const char TEXT_REG_DPTR[] PROGMEM = "DPTR";
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
