/*
 * Copyright 2026 Tadashi G. Takaoka
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

#include "text_c166.h"

namespace libasm {
namespace text {
namespace c166 {

// clang-format off
constexpr char TEXT_C166_LIST[]    PROGMEM = "80C166, 80C167";
constexpr char TEXT_CPU_80C166[]   PROGMEM = "80C166";
constexpr char TEXT_CPU_80C167[]   PROGMEM = "80C167";
constexpr char TEXT_CPU_80C167CS[] PROGMEM = "80C167CS";

// constexpr char TEXT_ADD[]   PROGMEM = "ADD";
// constexpr char TEXT_ADDB[]  PROGMEM = "ADDB";
// constexpr char TEXT_ADDC[]  PROGMEM = "ADDC";
// constexpr char TEXT_ADDCB[] PROGMEM = "ADDCB";
// constexpr char TEXT_AND[]   PROGMEM = "AND";
// constexpr char TEXT_ANDB[]  PROGMEM = "ANDB";
constexpr char TEXT_ASHR[]  PROGMEM = "ASHR";
constexpr char TEXT_ATOMIC[] PROGMEM = "ATOMIC";
// constexpr char TEXT_BAND[]  PROGMEM = "BAND";
// constexpr char TEXT_BCLR[]  PROGMEM = "BCLR";
constexpr char TEXT_BCMP[]  PROGMEM = "BCMP";
constexpr char TEXT_BFLDH[] PROGMEM = "BFLDH";
constexpr char TEXT_BFLDL[] PROGMEM = "BFLDL";
// constexpr char TEXT_BMOV[]  PROGMEM = "BMOV";
constexpr char TEXT_BMOVN[] PROGMEM = "BMOVN";
// constexpr char TEXT_BOR[]   PROGMEM = "BOR";
// constexpr char TEXT_BSET[]  PROGMEM = "BSET";
// constexpr char TEXT_BXOR[]  PROGMEM = "BXOR";
constexpr char TEXT_CALLA[] PROGMEM = "CALLA";
// constexpr char TEXT_CALLI[] PROGMEM = "CALLI";
// constexpr char TEXT_CALLR[] PROGMEM = "CALLR";
// constexpr char TEXT_CALLS[] PROGMEM = "CALLS";
// constexpr char TEXT_CMP[]   PROGMEM = "CMP";
// constexpr char TEXT_CMPB[]  PROGMEM = "CMPB";
constexpr char TEXT_CMPD1[] PROGMEM = "CMPD1";
constexpr char TEXT_CMPD2[] PROGMEM = "CMPD2";
constexpr char TEXT_CMPI1[] PROGMEM = "CMPI1";
constexpr char TEXT_CMPI2[] PROGMEM = "CMPI2";
// constexpr char TEXT_CPL[]   PROGMEM = "CPL";
constexpr char TEXT_CPLB[]  PROGMEM = "CPLB";
constexpr char TEXT_DISWDT[] PROGMEM = "DISWDT";
// constexpr char TEXT_DIV[]   PROGMEM = "DIV";
// constexpr char TEXT_DIVL[]  PROGMEM = "DIVL";
constexpr char TEXT_DIVLU[] PROGMEM = "DIVLU";
// constexpr char TEXT_DIVU[]  PROGMEM = "DIVU";
constexpr char TEXT_EINIT[] PROGMEM = "EINIT";
constexpr char TEXT_EXTP[]  PROGMEM = "EXTP";
constexpr char TEXT_EXTPR[] PROGMEM = "EXTPR";
constexpr char TEXT_EXTR[]  PROGMEM = "EXTR";
// constexpr char TEXT_EXTS[]  PROGMEM = "EXTS";
constexpr char TEXT_EXTSR[] PROGMEM = "EXTSR";
// constexpr char TEXT_IDLE[]  PROGMEM = "IDLE";
// constexpr char TEXT_JB[]    PROGMEM = "JB";
// constexpr char TEXT_JBC[]   PROGMEM = "JBC";
constexpr char TEXT_JMPA[]  PROGMEM = "JMPA";
// constexpr char TEXT_JMPI[]  PROGMEM = "JMPI";
constexpr char TEXT_JMPR[]  PROGMEM = "JMPR";
constexpr char TEXT_JMPS[]  PROGMEM = "JMPS";
// constexpr char TEXT_JNB[]   PROGMEM = "JNB";
constexpr char TEXT_JNBS[]  PROGMEM = "JNBS";
// constexpr char TEXT_MOV[]   PROGMEM = "MOV";
// constexpr char TEXT_MOVB[]  PROGMEM = "MOVB";
constexpr char TEXT_MOVBS[] PROGMEM = "MOVBS";
constexpr char TEXT_MOVBZ[] PROGMEM = "MOVBZ";
// constexpr char TEXT_MUL[]   PROGMEM = "MUL";
// constexpr char TEXT_MULU[]  PROGMEM = "MULU";
// constexpr char TEXT_NEG[]   PROGMEM = "NEG";
// constexpr char TEXT_NEGB[]  PROGMEM = "NEGB";
// constexpr char TEXT_NOP[]   PROGMEM = "NOP";
// constexpr char TEXT_OR[]    PROGMEM = "OR";
// constexpr char TEXT_ORB[]   PROGMEM = "ORB";
constexpr char TEXT_PCALL[] PROGMEM = "PCALL";
// constexpr char TEXT_POP[]   PROGMEM = "POP";
constexpr char TEXT_PRIOR[] PROGMEM = "PRIOR";
// constexpr char TEXT_PUSH[]  PROGMEM = "PUSH";
constexpr char TEXT_PWRDN[] PROGMEM = "PWRDN";
// constexpr char TEXT_RET[]   PROGMEM = "RET";
// constexpr char TEXT_RETI[]  PROGMEM = "RETI";
constexpr char TEXT_RETP[]  PROGMEM = "RETP";
// constexpr char TEXT_RETS[]  PROGMEM = "RETS";
// constexpr char TEXT_ROL[]   PROGMEM = "ROL";
// constexpr char TEXT_ROR[]   PROGMEM = "ROR";
constexpr char TEXT_SCXT[]  PROGMEM = "SCXT";
// constexpr char TEXT_SHL[]   PROGMEM = "SHL";
// constexpr char TEXT_SHR[]   PROGMEM = "SHR";
constexpr char TEXT_SRST[]  PROGMEM = "SRST";
constexpr char TEXT_SRVWDT[] PROGMEM = "SRVWDT";
// constexpr char TEXT_SUB[]   PROGMEM = "SUB";
// constexpr char TEXT_SUBB[]  PROGMEM = "SUBB";
// constexpr char TEXT_SUBC[]  PROGMEM = "SUBC";
// constexpr char TEXT_SUBCB[] PROGMEM = "SUBCB";
// constexpr char TEXT_TRAP[]  PROGMEM = "TRAP";
// constexpr char TEXT_XOR[]   PROGMEM = "XOR";
// constexpr char TEXT_XORB[]  PROGMEM = "XORB";

constexpr char TEXT_CC_[]   PROGMEM = "CC_";
// constexpr char TEXT_CC_C[]  PROGMEM = "C";
// constexpr char TEXT_CC_EQ[] PROGMEM = "EQ";
constexpr char TEXT_CC_N[]  PROGMEM = "N";
// constexpr char TEXT_CC_NC[] PROGMEM = "NC";
// constexpr char TEXT_CC_NE[] PROGMEM = "NE";
constexpr char TEXT_CC_NET[] PROGMEM = "NET";
constexpr char TEXT_CC_NN[] PROGMEM = "NN";
// constexpr char TEXT_CC_NV[] PROGMEM = "NV";
// constexpr char TEXT_CC_NZ[] PROGMEM = "NZ";
constexpr char TEXT_CC_SGE[] PROGMEM = "SGE";
constexpr char TEXT_CC_SGT[] PROGMEM = "SGT";
constexpr char TEXT_CC_SLE[] PROGMEM = "SLE";
constexpr char TEXT_CC_SLT[] PROGMEM = "SLT";
constexpr char TEXT_CC_UC[] PROGMEM = "UC";
// constexpr char TEXT_CC_UGE[] PROGMEM = "UGE";
// constexpr char TEXT_CC_UGT[] PROGMEM = "UGT";
// constexpr char TEXT_CC_ULE[] PROGMEM = "ULE";
// constexpr char TEXT_CC_ULT[] PROGMEM = "ULT";
// constexpr char TEXT_CC_V[]  PROGMEM = "V";
// constexpr char TEXT_CC_Z[]  PROGMEM = "Z";

constexpr char TEXT_REG_CP[]    PROGMEM = "CP";
constexpr char TEXT_REG_CSP[]   PROGMEM = "CSP";
constexpr char TEXT_REG_DPP0[]  PROGMEM = "DPP0";
constexpr char TEXT_REG_DPP1[]  PROGMEM = "DPP1";
constexpr char TEXT_REG_DPP2[]  PROGMEM = "DPP2";
constexpr char TEXT_REG_DPP3[]  PROGMEM = "DPP3";
constexpr char TEXT_REG_MDC[]   PROGMEM = "MDC";
constexpr char TEXT_REG_MDH[]   PROGMEM = "MDH";
constexpr char TEXT_REG_MDL[]   PROGMEM = "MDL";
constexpr char TEXT_REG_ONES[]  PROGMEM = "ONES";
constexpr char TEXT_REG_STKOV[] PROGMEM = "STKOV";
constexpr char TEXT_REG_STKUN[] PROGMEM = "STKUN";
constexpr char TEXT_REG_TFR[]   PROGMEM = "TFR";
constexpr char TEXT_REG_ZEROS[] PROGMEM = "ZEROS";

// clang-format on
}  // namespace c166
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
