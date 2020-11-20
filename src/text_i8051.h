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

#ifndef __TEXT_I8051__
#define __TEXT_I8051__

namespace libasm {
namespace i8051 {

// clang-format off
static const char TEXT_CPU_I8051[] PROGMEM = "i8051";
static constexpr const char *TEXT_CPU_8051 = (TEXT_CPU_I8051 + 1);

static const char TEXT_ACALL[] PROGMEM = "ACALL";
static const char TEXT_ADD[]   PROGMEM = "ADD";
static const char TEXT_ADDC[]  PROGMEM = "ADDC";
static const char TEXT_AJMP[]  PROGMEM = "AJMP";
static const char TEXT_ANL[]   PROGMEM = "ANL";
static const char TEXT_CJNE[]  PROGMEM = "CJNE";
static const char TEXT_CLR[]   PROGMEM = "CLR";
static const char TEXT_CPL[]   PROGMEM = "CPL";
static const char TEXT_CPU[]   PROGMEM = "CPU";
static const char TEXT_DA[]    PROGMEM = "DA";
static const char TEXT_DEC[]   PROGMEM = "DEC";
static const char TEXT_DIV[]   PROGMEM = "DIV";
static const char TEXT_DJNZ[]  PROGMEM = "DJNZ";
static const char TEXT_END[]   PROGMEM = "END";
static const char TEXT_INC[]   PROGMEM = "INC";
static const char TEXT_JB[]    PROGMEM = "JB";
static const char TEXT_JBC[]   PROGMEM = "JBC";
static const char TEXT_JC[]    PROGMEM = "JC";
static const char TEXT_JMP[]   PROGMEM = "JMP";
static const char TEXT_JNB[]   PROGMEM = "JNB";
static const char TEXT_JNC[]   PROGMEM = "JNC";
static const char TEXT_JNZ[]   PROGMEM = "JNZ";
static const char TEXT_JZ[]    PROGMEM = "JZ";
static const char TEXT_LCALL[] PROGMEM = "LCALL";
static const char TEXT_LJMP[]  PROGMEM = "LJMP";
static const char TEXT_MOV[]   PROGMEM = "MOV";
static const char TEXT_MOVC[]  PROGMEM = "MOVC";
static const char TEXT_MOVX[]  PROGMEM = "MOVX";
static const char TEXT_MUL[]   PROGMEM = "MUL";
static const char TEXT_NOP[]   PROGMEM = "NOP";
static const char TEXT_ORL[]   PROGMEM = "ORL";
static const char TEXT_POP[]   PROGMEM = "POP";
static const char TEXT_PUSH[]  PROGMEM = "PUSH";
static const char TEXT_RET[]   PROGMEM = "RET";
static const char TEXT_RETI[]  PROGMEM = "RETI";
static const char TEXT_RL[]    PROGMEM = "RL";
static const char TEXT_RLC[]   PROGMEM = "RLC";
static const char TEXT_RR[]    PROGMEM = "RR";
static const char TEXT_RRC[]   PROGMEM = "RRC";
static const char TEXT_SETB[]  PROGMEM = "SETB";
static const char TEXT_SJMP[]  PROGMEM = "SJMP";
static const char TEXT_SUBB[]  PROGMEM = "SUBB";
static const char TEXT_SWAP[]  PROGMEM = "SWAP";
static const char TEXT_XCH[]   PROGMEM = "XCH";
static const char TEXT_XCHD[]  PROGMEM = "XCHD";
static const char TEXT_XRL[]   PROGMEM = "XRL";
// clang-format on

}  // namespace i8051
}  // namespace libasm

#endif  // __TEXT_I8051__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
