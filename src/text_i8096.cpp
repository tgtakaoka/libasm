/*
 * Copyright 2022 Tadashi G. Takaoka
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

#include "text_i8096.h"

namespace libasm {
namespace text {
namespace i8096 {

// clang-format off
const char TEXT_CPU_I8096[] PROGMEM = "i8096";
const char TEXT_CPU_8096[]  PROGMEM = "8096";

// const char TEXT_ADD[]   PROGMEM = "ADD";
// const char TEXT_ADDB[]  PROGMEM = "ADDB";
// const char TEXT_ADDC[]  PROGMEM = "ADDC";
// const char TEXT_ADDCB[] PROGMEM = "ADDCB";
// const char TEXT_AND[]   PROGMEM = "AND";
// const char TEXT_ANDB[]  PROGMEM = "ANDB";
// const char TEXT_BR[]    PROGMEM = "BR";
// const char TEXT_CLR[]   PROGMEM = "CLR";
// const char TEXT_CLRB[]  PROGMEM = "CLRB";
// const char TEXT_CLRC[]  PROGMEM = "CLRC";
const char TEXT_CLRVT[] PROGMEM = "CLRVT";
// const char TEXT_CMP[]   PROGMEM = "CMP";
// const char TEXT_CMPB[]  PROGMEM = "CMPB";
// const char TEXT_CPU[]   PROGMEM = "CPU";
// const char TEXT_DEC[]   PROGMEM = "DEC";
// const char TEXT_DECB[]  PROGMEM = "DECB";
// const char TEXT_DI[]    PROGMEM = "DI";
// const char TEXT_DIV[]   PROGMEM = "DIV";
// const char TEXT_DIVB[]  PROGMEM = "DIVB";
// const char TEXT_DIVU[]  PROGMEM = "DIVU";
const char TEXT_DIVUB[] PROGMEM = "DIVUB";
// const char TEXT_DJNZ[]  PROGMEM = "DJNZ";
// const char TEXT_EI[]    PROGMEM = "EI";
// const char TEXT_EXT[]   PROGMEM = "EXT";
// const char TEXT_EXTB[]  PROGMEM = "EXTB";
// const char TEXT_INC[]   PROGMEM = "INC";
// const char TEXT_INCB[]  PROGMEM = "INCB";
// const char TEXT_JBC[]   PROGMEM = "JBC";
const char TEXT_JBS[]   PROGMEM = "JBS";
// const char TEXT_JC[]    PROGMEM = "JC";
// const char TEXT_JE[]    PROGMEM = "JE";
// const char TEXT_JGE[]   PROGMEM = "JGE";
// const char TEXT_JGT[]   PROGMEM = "JGT";
// const char TEXT_JH[]    PROGMEM = "JH";
// const char TEXT_JLE[]   PROGMEM = "JLE";
// const char TEXT_JLT[]   PROGMEM = "JLT";
// const char TEXT_JNC[]   PROGMEM = "JNC";
// const char TEXT_JNE[]   PROGMEM = "JNE";
const char TEXT_JNH[]   PROGMEM = "JNH";
const char TEXT_JNST[]  PROGMEM = "JNST";
const char TEXT_JNV[]   PROGMEM = "JNV";
const char TEXT_JNVT[]  PROGMEM = "JNVT";
const char TEXT_JST[]   PROGMEM = "JST";
const char TEXT_JV[]    PROGMEM = "JV";
const char TEXT_JVT[]   PROGMEM = "JVT";
// const char TEXT_LCALL[] PROGMEM = "LCALL";
// const char TEXT_LD[]    PROGMEM = "LD";
// const char TEXT_LDB[]   PROGMEM = "LDB";
const char TEXT_LDBSE[] PROGMEM = "LDBSE";
const char TEXT_LDBZE[] PROGMEM = "LDBZE";
// const char TEXT_LJMP[]  PROGMEM = "LJMP";
// const char TEXT_MUL[]   PROGMEM = "MUL";
// const char TEXT_MULB[]  PROGMEM = "MULB";
// const char TEXT_MULU[]  PROGMEM = "MULU";
const char TEXT_MULUB[] PROGMEM = "MULUB";
// const char TEXT_NEG[]   PROGMEM = "NEG";
// const char TEXT_NEGB[]  PROGMEM = "NEGB";
// const char TEXT_NOP[]   PROGMEM = "NOP";
const char TEXT_NORML[] PROGMEM = "NORML";
// const char TEXT_NOT[]   PROGMEM = "NOT";
// const char TEXT_NOTB[]  PROGMEM = "NOTB";
// const char TEXT_OR[]    PROGMEM = "OR";
// const char TEXT_ORB[]   PROGMEM = "ORB";
const char TEXT_ORG[]   PROGMEM = "ORG";
// const char TEXT_POP[]   PROGMEM = "POP";
// const char TEXT_POPF[]  PROGMEM = "POPF";
// const char TEXT_PUSH[]  PROGMEM = "PUSH";
// const char TEXT_PUSHF[] PROGMEM = "PUSHF";
// const char TEXT_RET[]   PROGMEM = "RET";
// const char TEXT_RST[]   PROGMEM = "RST";
const char TEXT_SCALL[] PROGMEM = "SCALL";
// const char TEXT_SETC[]  PROGMEM = "SETC";
// const char TEXT_SHL[]   PROGMEM = "SHL";
const char TEXT_SHLB[]  PROGMEM = "SHLB";
const char TEXT_SHLL[]  PROGMEM = "SHLL";
// const char TEXT_SHR[]   PROGMEM = "SHR";
const char TEXT_SHRA[]  PROGMEM = "SHRA";
const char TEXT_SHRAB[] PROGMEM = "SHRAB";
const char TEXT_SHRAL[] PROGMEM = "SHRAL";
const char TEXT_SHRB[]  PROGMEM = "SHRB";
const char TEXT_SHRL[]  PROGMEM = "SHRL";
// const char TEXT_SJMP[]  PROGMEM = "SJMP";
const char TEXT_SKIP[]  PROGMEM = "SKIP";
// const char TEXT_ST[]    PROGMEM = "ST";
// const char TEXT_STB[]   PROGMEM = "STB";
// const char TEXT_SUB[]   PROGMEM = "SUB";
// const char TEXT_SUBB[]  PROGMEM = "SUBB";
// const char TEXT_SUBC[]  PROGMEM = "SUBC";
// const char TEXT_SUBCB[] PROGMEM = "SUBCB";
// const char TEXT_TRAP[]  PROGMEM = "TRAP";
// const char TEXT_XOR[]   PROGMEM = "XOR";
// const char TEXT_XORB[]  PROGMEM = "XORB";
// clang-format on

}  // namespace i8096
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
