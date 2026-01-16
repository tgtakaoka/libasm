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

constexpr char TEXT_CPU_LIST[] PROGMEM = "i8096, i80196";
constexpr char TEXT_CPU_80196[]   PROGMEM = "80196";
constexpr char TEXT_CPU_80196KB[] PROGMEM = "80196KB";
constexpr char TEXT_CPU_8096[]    PROGMEM = "8096";

// constexpr char TEXT_ADD[]   PROGMEM = "ADD";
// constexpr char TEXT_ADDB[]  PROGMEM = "ADDB";
// constexpr char TEXT_ADDC[]  PROGMEM = "ADDC";
// constexpr char TEXT_ADDCB[] PROGMEM = "ADDCB";
// constexpr char TEXT_AND[]   PROGMEM = "AND";
// constexpr char TEXT_ANDB[]  PROGMEM = "ANDB";
// constexpr char TEXT_BR[]    PROGMEM = "BR";
// constexpr char TEXT_CLR[]   PROGMEM = "CLR";
// constexpr char TEXT_CLRB[]  PROGMEM = "CLRB";
// constexpr char TEXT_CLRC[]  PROGMEM = "CLRC";
constexpr char TEXT_CLRVT[] PROGMEM = "CLRVT";
// constexpr char TEXT_CMP[]   PROGMEM = "CMP";
// constexpr char TEXT_CMPB[]  PROGMEM = "CMPB";
// constexpr char TEXT_CPU[]   PROGMEM = "CPU";
// constexpr char TEXT_DEC[]   PROGMEM = "DEC";
// constexpr char TEXT_DECB[]  PROGMEM = "DECB";
// constexpr char TEXT_DI[]    PROGMEM = "DI";
// constexpr char TEXT_DIV[]   PROGMEM = "DIV";
// constexpr char TEXT_DIVB[]  PROGMEM = "DIVB";
// constexpr char TEXT_DIVU[]  PROGMEM = "DIVU";
constexpr char TEXT_DIVUB[] PROGMEM = "DIVUB";
// constexpr char TEXT_DJNZ[]  PROGMEM = "DJNZ";
// constexpr char TEXT_EI[]    PROGMEM = "EI";
// constexpr char TEXT_EXT[]   PROGMEM = "EXT";
// constexpr char TEXT_EXTB[]  PROGMEM = "EXTB";
// constexpr char TEXT_INC[]   PROGMEM = "INC";
// constexpr char TEXT_INCB[]  PROGMEM = "INCB";
// constexpr char TEXT_JBC[]   PROGMEM = "JBC";
constexpr char TEXT_JBS[]   PROGMEM = "JBS";
// constexpr char TEXT_JC[]    PROGMEM = "JC";
// constexpr char TEXT_JE[]    PROGMEM = "JE";
// constexpr char TEXT_JGE[]   PROGMEM = "JGE";
// constexpr char TEXT_JGT[]   PROGMEM = "JGT";
// constexpr char TEXT_JH[]    PROGMEM = "JH";
// constexpr char TEXT_JLE[]   PROGMEM = "JLE";
// constexpr char TEXT_JLT[]   PROGMEM = "JLT";
// constexpr char TEXT_JNC[]   PROGMEM = "JNC";
// constexpr char TEXT_JNE[]   PROGMEM = "JNE";
constexpr char TEXT_JNH[]   PROGMEM = "JNH";
constexpr char TEXT_JNST[]  PROGMEM = "JNST";
// constexpr char TEXT_JNV[]   PROGMEM = "JNV";
constexpr char TEXT_JNVT[]  PROGMEM = "JNVT";
constexpr char TEXT_JST[]   PROGMEM = "JST";
// constexpr char TEXT_JV[]    PROGMEM = "JV";
constexpr char TEXT_JVT[]   PROGMEM = "JVT";
// constexpr char TEXT_LCALL[] PROGMEM = "LCALL";
// constexpr char TEXT_LD[]    PROGMEM = "LD";
// constexpr char TEXT_LDB[]   PROGMEM = "LDB";
constexpr char TEXT_LDBSE[] PROGMEM = "LDBSE";
constexpr char TEXT_LDBZE[] PROGMEM = "LDBZE";
// constexpr char TEXT_LJMP[]  PROGMEM = "LJMP";
// constexpr char TEXT_MUL[]   PROGMEM = "MUL";
// constexpr char TEXT_MULB[]  PROGMEM = "MULB";
// constexpr char TEXT_MULU[]  PROGMEM = "MULU";
constexpr char TEXT_MULUB[] PROGMEM = "MULUB";
// constexpr char TEXT_NEG[]   PROGMEM = "NEG";
// constexpr char TEXT_NEGB[]  PROGMEM = "NEGB";
// constexpr char TEXT_NOP[]   PROGMEM = "NOP";
constexpr char TEXT_NORML[] PROGMEM = "NORML";
// constexpr char TEXT_NOT[]   PROGMEM = "NOT";
// constexpr char TEXT_NOTB[]  PROGMEM = "NOTB";
// constexpr char TEXT_OR[]    PROGMEM = "OR";
// constexpr char TEXT_ORB[]   PROGMEM = "ORB";
// constexpr char TEXT_ORG[]   PROGMEM = "ORG";
// constexpr char TEXT_POP[]   PROGMEM = "POP";
// constexpr char TEXT_POPF[]  PROGMEM = "POPF";
// constexpr char TEXT_PUSH[]  PROGMEM = "PUSH";
// constexpr char TEXT_PUSHF[] PROGMEM = "PUSHF";
// constexpr char TEXT_RET[]   PROGMEM = "RET";
// constexpr char TEXT_RST[]   PROGMEM = "RST";
constexpr char TEXT_SCALL[] PROGMEM = "SCALL";
// constexpr char TEXT_SETC[]  PROGMEM = "SETC";
// constexpr char TEXT_SHL[]   PROGMEM = "SHL";
constexpr char TEXT_SHLB[]  PROGMEM = "SHLB";
constexpr char TEXT_SHLL[]  PROGMEM = "SHLL";
// constexpr char TEXT_SHR[]   PROGMEM = "SHR";
constexpr char TEXT_SHRA[]  PROGMEM = "SHRA";
constexpr char TEXT_SHRAB[] PROGMEM = "SHRAB";
constexpr char TEXT_SHRAL[] PROGMEM = "SHRAL";
constexpr char TEXT_SHRB[]  PROGMEM = "SHRB";
constexpr char TEXT_SHRL[]  PROGMEM = "SHRL";
// constexpr char TEXT_SJMP[]  PROGMEM = "SJMP";
constexpr char TEXT_SKIP[]  PROGMEM = "SKIP";
// constexpr char TEXT_ST[]    PROGMEM = "ST";
// constexpr char TEXT_STB[]   PROGMEM = "STB";
// constexpr char TEXT_SUB[]   PROGMEM = "SUB";
// constexpr char TEXT_SUBB[]  PROGMEM = "SUBB";
// constexpr char TEXT_SUBC[]  PROGMEM = "SUBC";
// constexpr char TEXT_SUBCB[] PROGMEM = "SUBCB";
// constexpr char TEXT_TRAP[]  PROGMEM = "TRAP";
// constexpr char TEXT_XOR[]   PROGMEM = "XOR";
// constexpr char TEXT_XORB[]  PROGMEM = "XORB";

// i80C196KB
constexpr char TEXT_BMOV[]  PROGMEM = "BMOV";
// constexpr char TEXT_CMPL[]  PROGMEM = "CMPL";
constexpr char TEXT_DJNZW[] PROGMEM = "DJNZW";
constexpr char TEXT_IDLPD[] PROGMEM = "IDLPD";
// constexpr char TEXT_POPA[]  PROGMEM = "POPA";
// constexpr char TEXT_PUSHA[] PROGMEM = "PUSHA";

// i80C196KC
constexpr char TEXT_BMOVI[] PROGMEM = "BMOVI";
constexpr char TEXT_DPTS[]  PROGMEM = "DPTS";
constexpr char TEXT_EPTS[]  PROGMEM = "EPTS";
constexpr char TEXT_TIJMP[] PROGMEM = "TIJMP";
// constexpr char TEXT_XCH[]  PROGMEM = "XCH";
// constexpr char TEXT_XCHB[] PROGMEM = "XCHB";

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
