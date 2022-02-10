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

#ifndef __TEXT_I8096__
#define __TEXT_I8096__

#include "config_host.h"

namespace libasm {
namespace i8096 {

// clang-format off
static const char TEXT_CPU_I8096[] PROGMEM = "i8096";
static constexpr const char *TEXT_CPU_8096 = (TEXT_CPU_I8096 + 1);

static const char TEXT_ADD[]   PROGMEM = "ADD";
static const char TEXT_ADDB[]  PROGMEM = "ADDB";
static const char TEXT_ADDC[]  PROGMEM = "ADDC";
static const char TEXT_ADDCB[] PROGMEM = "ADDCB";
static const char TEXT_AND[]   PROGMEM = "AND";
static const char TEXT_ANDB[]  PROGMEM = "ANDB";
static const char TEXT_BR[]    PROGMEM = "BR";
static const char TEXT_CLR[]   PROGMEM = "CLR";
static const char TEXT_CLRB[]  PROGMEM = "CLRB";
static const char TEXT_CLRC[]  PROGMEM = "CLRC";
static const char TEXT_CLRVT[] PROGMEM = "CLRVT";
static const char TEXT_CMP[]   PROGMEM = "CMP";
static const char TEXT_CMPB[]  PROGMEM = "CMPB";
static const char TEXT_CPU[]   PROGMEM = "CPU";
static const char TEXT_DEC[]   PROGMEM = "DEC";
static const char TEXT_DECB[]  PROGMEM = "DECB";
static const char TEXT_DI[]    PROGMEM = "DI";
static const char TEXT_DIV[]   PROGMEM = "DIV";
static const char TEXT_DIVB[]  PROGMEM = "DIVB";
static const char TEXT_DIVU[]  PROGMEM = "DIVU";
static const char TEXT_DIVUB[] PROGMEM = "DIVUB";
static const char TEXT_DJNZ[]  PROGMEM = "DJNZ";
static const char TEXT_EI[]    PROGMEM = "EI";
static const char TEXT_EXT[]   PROGMEM = "EXT";
static const char TEXT_EXTB[]  PROGMEM = "EXTB";
static const char TEXT_INC[]   PROGMEM = "INC";
static const char TEXT_INCB[]  PROGMEM = "INCB";
static const char TEXT_JBC[]   PROGMEM = "JBC";
static const char TEXT_JBS[]   PROGMEM = "JBS";
static const char TEXT_JC[]    PROGMEM = "JC";
static const char TEXT_JE[]    PROGMEM = "JE";
static const char TEXT_JGE[]   PROGMEM = "JGE";
static const char TEXT_JGT[]   PROGMEM = "JGT";
static const char TEXT_JH[]    PROGMEM = "JH";
static const char TEXT_JLE[]   PROGMEM = "JLE";
static const char TEXT_JLT[]   PROGMEM = "JLT";
static const char TEXT_JNC[]   PROGMEM = "JNC";
static const char TEXT_JNE[]   PROGMEM = "JNE";
static const char TEXT_JNH[]   PROGMEM = "JNH";
static const char TEXT_JNST[]  PROGMEM = "JNST";
static const char TEXT_JNV[]   PROGMEM = "JNV";
static const char TEXT_JNVT[]  PROGMEM = "JNVT";
static const char TEXT_JST[]   PROGMEM = "JST";
static const char TEXT_JV[]    PROGMEM = "JV";
static const char TEXT_JVT[]   PROGMEM = "JVT";
static const char TEXT_LCALL[] PROGMEM = "LCALL";
static const char TEXT_LD[]    PROGMEM = "LD";
static const char TEXT_LDB[]   PROGMEM = "LDB";
static const char TEXT_LDBSE[] PROGMEM = "LDBSE";
static const char TEXT_LDBZE[] PROGMEM = "LDBZE";
static const char TEXT_LJMP[]  PROGMEM = "LJMP";
static const char TEXT_MUL[]   PROGMEM = "MUL";
static const char TEXT_MULB[]  PROGMEM = "MULB";
static const char TEXT_MULU[]  PROGMEM = "MULU";
static const char TEXT_MULUB[] PROGMEM = "MULUB";
static const char TEXT_NEG[]   PROGMEM = "NEG";
static const char TEXT_NEGB[]  PROGMEM = "NEGB";
static const char TEXT_NOP[]   PROGMEM = "NOP";
static const char TEXT_NORML[] PROGMEM = "NORML";
static const char TEXT_NOT[]   PROGMEM = "NOT";
static const char TEXT_NOTB[]  PROGMEM = "NOTB";
static const char TEXT_OR[]    PROGMEM = "OR";
static const char TEXT_ORB[]   PROGMEM = "ORB";
static const char TEXT_ORG[]   PROGMEM = "ORG";
static const char TEXT_POP[]   PROGMEM = "POP";
static const char TEXT_POPF[]  PROGMEM = "POPF";
static const char TEXT_PUSH[]  PROGMEM = "PUSH";
static const char TEXT_PUSHF[] PROGMEM = "PUSHF";
static const char TEXT_RET[]   PROGMEM = "RET";
static const char TEXT_RST[]   PROGMEM = "RST";
static const char TEXT_SCALL[] PROGMEM = "SCALL";
static const char TEXT_SETC[]  PROGMEM = "SETC";
static const char TEXT_SHL[]   PROGMEM = "SHL";
static const char TEXT_SHLB[]  PROGMEM = "SHLB";
static const char TEXT_SHLL[]  PROGMEM = "SHLL";
static const char TEXT_SHR[]   PROGMEM = "SHR";
static const char TEXT_SHRA[]  PROGMEM = "SHRA";
static const char TEXT_SHRAB[] PROGMEM = "SHRAB";
static const char TEXT_SHRAL[] PROGMEM = "SHRAL";
static const char TEXT_SHRB[]  PROGMEM = "SHRB";
static const char TEXT_SHRL[]  PROGMEM = "SHRL";
static const char TEXT_SJMP[]  PROGMEM = "SJMP";
static const char TEXT_SKIP[]  PROGMEM = "SKIP";
static const char TEXT_ST[]    PROGMEM = "ST";
static const char TEXT_STB[]   PROGMEM = "STB";
static const char TEXT_SUB[]   PROGMEM = "SUB";
static const char TEXT_SUBB[]  PROGMEM = "SUBB";
static const char TEXT_SUBC[]  PROGMEM = "SUBC";
static const char TEXT_SUBCB[] PROGMEM = "SUBCB";
static const char TEXT_TRAP[]  PROGMEM = "TRAP";
static const char TEXT_XOR[]   PROGMEM = "XOR";
static const char TEXT_XORB[]  PROGMEM = "XORB";
// clang-format on

}  // namespace i8096
}  // namespace libasm

#endif  // __TEXT_I8096__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
