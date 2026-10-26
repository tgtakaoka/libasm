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

constexpr char TEXT_CPU_LIST[] = "i8096, i80196KB, i80196, i80196N, i80296";
constexpr char TEXT_CPU_80296[]   = "80296";
constexpr char TEXT_CPU_80196[]   = "80196";
constexpr char TEXT_CPU_80196N[]  = "80196N";
constexpr char TEXT_CPU_80196KB[] = "80196KB";
constexpr char TEXT_CPU_8096[]    = "8096";

// constexpr char TEXT_ADD[]   = "ADD";
// constexpr char TEXT_ADDB[]  = "ADDB";
// constexpr char TEXT_ADDC[]  = "ADDC";
// constexpr char TEXT_ADDCB[] = "ADDCB";
// constexpr char TEXT_AND[]   = "AND";
// constexpr char TEXT_ANDB[]  = "ANDB";
// constexpr char TEXT_BR[]    = "BR";
// constexpr char TEXT_CLR[]   = "CLR";
// constexpr char TEXT_CLRB[]  = "CLRB";
// constexpr char TEXT_CLRC[]  = "CLRC";
constexpr char TEXT_CLRVT[] = "CLRVT";
// constexpr char TEXT_CMP[]   = "CMP";
// constexpr char TEXT_CMPB[]  = "CMPB";
// constexpr char TEXT_CPU[]   = "CPU";
// constexpr char TEXT_DEC[]   = "DEC";
// constexpr char TEXT_DECB[]  = "DECB";
// constexpr char TEXT_DI[]    = "DI";
// constexpr char TEXT_DIV[]   = "DIV";
// constexpr char TEXT_DIVB[]  = "DIVB";
// constexpr char TEXT_DIVU[]  = "DIVU";
constexpr char TEXT_DIVUB[] = "DIVUB";
// constexpr char TEXT_DJNZ[]  = "DJNZ";
// constexpr char TEXT_EI[]    = "EI";
// constexpr char TEXT_EXT[]   = "EXT";
// constexpr char TEXT_EXTB[]  = "EXTB";
// constexpr char TEXT_INC[]   = "INC";
// constexpr char TEXT_INCB[]  = "INCB";
// constexpr char TEXT_JBC[]   = "JBC";
constexpr char TEXT_JBS[]   = "JBS";
// constexpr char TEXT_JC[]    = "JC";
// constexpr char TEXT_JE[]    = "JE";
// constexpr char TEXT_JGE[]   = "JGE";
// constexpr char TEXT_JGT[]   = "JGT";
// constexpr char TEXT_JH[]    = "JH";
// constexpr char TEXT_JLE[]   = "JLE";
// constexpr char TEXT_JLT[]   = "JLT";
// constexpr char TEXT_JNC[]   = "JNC";
// constexpr char TEXT_JNE[]   = "JNE";
constexpr char TEXT_JNH[]   = "JNH";
constexpr char TEXT_JNST[]  = "JNST";
// constexpr char TEXT_JNV[]   = "JNV";
constexpr char TEXT_JNVT[]  = "JNVT";
constexpr char TEXT_JST[]   = "JST";
// constexpr char TEXT_JV[]    = "JV";
constexpr char TEXT_JVT[]   = "JVT";
// constexpr char TEXT_LCALL[] = "LCALL";
// constexpr char TEXT_LD[]    = "LD";
// constexpr char TEXT_LDB[]   = "LDB";
constexpr char TEXT_LDBSE[] = "LDBSE";
constexpr char TEXT_LDBZE[] = "LDBZE";
// constexpr char TEXT_LJMP[]  = "LJMP";
// constexpr char TEXT_MUL[]   = "MUL";
// constexpr char TEXT_MULB[]  = "MULB";
// constexpr char TEXT_MULU[]  = "MULU";
constexpr char TEXT_MULUB[] = "MULUB";
// constexpr char TEXT_NEG[]   = "NEG";
// constexpr char TEXT_NEGB[]  = "NEGB";
// constexpr char TEXT_NOP[]   = "NOP";
constexpr char TEXT_NORML[] = "NORML";
// constexpr char TEXT_NOT[]   = "NOT";
// constexpr char TEXT_NOTB[]  = "NOTB";
// constexpr char TEXT_OR[]    = "OR";
// constexpr char TEXT_ORB[]   = "ORB";
// constexpr char TEXT_ORG[]   = "ORG";
// constexpr char TEXT_POP[]   = "POP";
// constexpr char TEXT_POPF[]  = "POPF";
// constexpr char TEXT_PUSH[]  = "PUSH";
// constexpr char TEXT_PUSHF[] = "PUSHF";
// constexpr char TEXT_RET[]   = "RET";
// constexpr char TEXT_RETI[]  = "RETI";
// constexpr char TEXT_RST[]   = "RST";
constexpr char TEXT_SCALL[] = "SCALL";
// constexpr char TEXT_SETC[]  = "SETC";
// constexpr char TEXT_SHL[]   = "SHL";
constexpr char TEXT_SHLB[]  = "SHLB";
// constexpr char TEXT_SHLL[]  = "SHLL";
// constexpr char TEXT_SHR[]   = "SHR";
constexpr char TEXT_SHRA[]  = "SHRA";
constexpr char TEXT_SHRAB[] = "SHRAB";
constexpr char TEXT_SHRAL[] = "SHRAL";
constexpr char TEXT_SHRB[]  = "SHRB";
constexpr char TEXT_SHRL[]  = "SHRL";
// constexpr char TEXT_SJMP[]  = "SJMP";
constexpr char TEXT_SKIP[]  = "SKIP";
// constexpr char TEXT_ST[]    = "ST";
// constexpr char TEXT_STB[]   = "STB";
// constexpr char TEXT_SUB[]   = "SUB";
// constexpr char TEXT_SUBB[]  = "SUBB";
// constexpr char TEXT_SUBC[]  = "SUBC";
// constexpr char TEXT_SUBCB[] = "SUBCB";
// constexpr char TEXT_TRAP[]  = "TRAP";
// constexpr char TEXT_XOR[]   = "XOR";
// constexpr char TEXT_XORB[]  = "XORB";

// i80C196KB
// constexpr char TEXT_BMOV[]  = "BMOV";
// constexpr char TEXT_CMPL[]  = "CMPL";
constexpr char TEXT_DJNZW[] = "DJNZW";
constexpr char TEXT_IDLPD[] = "IDLPD";
// constexpr char TEXT_POPA[]  = "POPA";
// constexpr char TEXT_PUSHA[] = "PUSHA";

// i80C196KC
constexpr char TEXT_BMOVI[] = "BMOVI";
constexpr char TEXT_DPTS[]  = "DPTS";
constexpr char TEXT_EBMOVI[] = "EBMOVI";
constexpr char TEXT_EBR[]   = "EBR";
constexpr char TEXT_ECALL[] = "ECALL";
constexpr char TEXT_EJMP[]  = "EJMP";
constexpr char TEXT_ELD[]   = "ELD";
constexpr char TEXT_ELDB[]  = "ELDB";
constexpr char TEXT_EST[]   = "EST";
constexpr char TEXT_ESTB[]  = "ESTB";
constexpr char TEXT_EPTS[]  = "EPTS";
constexpr char TEXT_TIJMP[] = "TIJMP";
// constexpr char TEXT_XCH[]  = "XCH";
// constexpr char TEXT_XCHB[] = "XCHB";

// i80296
// constexpr char TEXT_MAC[]  = "MAC";
constexpr char TEXT_MACR[]     = "MACR";
constexpr char TEXT_MACRZ[]    = "MACRZ";
constexpr char TEXT_MACZ[]     = "MACZ";
constexpr char TEXT_MSAC[]     = "MSAC";
constexpr char TEXT_MVAC[]     = "MVAC";
constexpr char TEXT_RPT[]      = "RPT";
constexpr char TEXT_RPTC[]     = "RPTC";
constexpr char TEXT_RPTE[]     = "RPTE";
constexpr char TEXT_RPTGE[]    = "RPTGE";
constexpr char TEXT_RPTGT[]    = "RPTGT";
constexpr char TEXT_RPTH[]     = "RPTH";
constexpr char TEXT_RPTI[]     = "RPTI";
constexpr char TEXT_RPTIC[]    = "RPTIC";
constexpr char TEXT_RPTIE[]    = "RPTIE";
constexpr char TEXT_RPTIGE[]   = "RPTIGE";
constexpr char TEXT_RPTIGT[]   = "RPTIGT";
constexpr char TEXT_RPTIH[]    = "RPTIH";
constexpr char TEXT_RPTILE[]   = "RPTILE";
constexpr char TEXT_RPTILT[]   = "RPTILT";
constexpr char TEXT_RPTINC[]   = "RPTINC";
constexpr char TEXT_RPTINE[]   = "RPTINE";
constexpr char TEXT_RPTINH[]   = "RPTINH";
constexpr char TEXT_RPTINST[]  = "RPTINST";
constexpr char TEXT_RPTINV[]   = "RPTINV";
constexpr char TEXT_RPTINVT[]  = "RPTINVT";
constexpr char TEXT_RPTIST[]   = "RPTIST";
constexpr char TEXT_RPTIV[]    = "RPTIV";
constexpr char TEXT_RPTIVT[]   = "RPTIVT";
constexpr char TEXT_RPTLE[]    = "RPTLE";
constexpr char TEXT_RPTLT[]    = "RPTLT";
constexpr char TEXT_RPTNC[]    = "RPTNC";
constexpr char TEXT_RPTNE[]    = "RPTNE";
constexpr char TEXT_RPTNH[]    = "RPTNH";
constexpr char TEXT_RPTNST[]   = "RPTNST";
constexpr char TEXT_RPTNV[]    = "RPTNV";
constexpr char TEXT_RPTNVT[]   = "RPTNVT";
constexpr char TEXT_RPTST[]    = "RPTST";
constexpr char TEXT_RPTV[]     = "RPTV";
constexpr char TEXT_RPTVT[]    = "RPTVT";
constexpr char TEXT_SMAC[]     = "SMAC";
constexpr char TEXT_SMACR[]    = "SMACR";
constexpr char TEXT_SMACRZ[]   = "SMACRZ";
constexpr char TEXT_SMACZ[]    = "SMACZ";

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
