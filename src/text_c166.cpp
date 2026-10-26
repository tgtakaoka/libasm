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

constexpr char TEXT_C166_LIST[] = "80C166, 80C167, XC16X";
constexpr char TEXT_CPU_80C166[]   = "80C166";
constexpr char TEXT_CPU_80C167[]   = "80C167";
constexpr char TEXT_CPU_80C167CS[] = "80C167CS";
constexpr char TEXT_CPU_XC16X[]    = "XC16X";

// constexpr char TEXT_ADDB[]  = "ADDB";
// constexpr char TEXT_ADDCB[] = "ADDCB";
// constexpr char TEXT_ADDC[]  = "ADDC";
// constexpr char TEXT_ADD[]   = "ADD";
// constexpr char TEXT_ANDB[]  = "ANDB";
// constexpr char TEXT_AND[]   = "AND";
constexpr char TEXT_ASHR[]       = "ASHR";
constexpr char TEXT_ATOMIC[]     = "ATOMIC";
// constexpr char TEXT_BAND[]  = "BAND";
// constexpr char TEXT_BCLR[]  = "BCLR";
constexpr char TEXT_BCMP[]       = "BCMP";
constexpr char TEXT_BFLDH[]      = "BFLDH";
constexpr char TEXT_BFLDL[]      = "BFLDL";
// constexpr char TEXT_BMOV[]  = "BMOV";
constexpr char TEXT_BMOVN[]      = "BMOVN";
// constexpr char TEXT_BOR[]   = "BOR";
// constexpr char TEXT_BSET[]  = "BSET";
// constexpr char TEXT_BXOR[]  = "BXOR";
constexpr char TEXT_CALLA[]      = "CALLA";
constexpr char TEXT_CALLA_M[]    = "CALLA-";
constexpr char TEXT_CALLA_P[]    = "CALLA+";
// constexpr char TEXT_CALLI[] = "CALLI";
// constexpr char TEXT_CALLR[] = "CALLR";
// constexpr char TEXT_CALLS[] = "CALLS";
// constexpr char TEXT_CMPB[]  = "CMPB";
// constexpr char TEXT_CMP[]   = "CMP";
constexpr char TEXT_CMPD1[]      = "CMPD1";
constexpr char TEXT_CMPD2[]      = "CMPD2";
constexpr char TEXT_CMPI1[]      = "CMPI1";
constexpr char TEXT_CMPI2[]      = "CMPI2";
constexpr char TEXT_COABS[]      = "COABS";
constexpr char TEXT_COADD2[]     = "COADD2";
constexpr char TEXT_COADD[]      = "COADD";
constexpr char TEXT_COASHR[]     = "COASHR";
constexpr char TEXT_COCMP[]      = "COCMP";
constexpr char TEXT_COLOAD2[]    = "COLOAD2";
constexpr char TEXT_COLOAD2_M[]  = "COLOAD2-";
constexpr char TEXT_COLOAD[]     = "COLOAD";
constexpr char TEXT_COLOAD_M[]   = "COLOAD-";
constexpr char TEXT_COMAC[]      = "COMAC";
constexpr char TEXT_COMAC_M[]    = "COMAC-";
constexpr char TEXT_COMACM[]     = "COMACM";
constexpr char TEXT_COMACM_M[]   = "COMACM-";
constexpr char TEXT_COMACMR[]    = "COMACMR";
constexpr char TEXT_COMACMRSU[]  = "COMACMRSU";
constexpr char TEXT_COMACMRU[]   = "COMACMRU";
constexpr char TEXT_COMACMRUS[]  = "COMACMRUS";
constexpr char TEXT_COMACMSU[]   = "COMACMSU";
constexpr char TEXT_COMACMSU_M[] = "COMACMSU-";
constexpr char TEXT_COMACMU[]    = "COMACMU";
constexpr char TEXT_COMACMU_M[]  = "COMACMU-";
constexpr char TEXT_COMACMUS[]   = "COMACMUS";
constexpr char TEXT_COMACMUS_M[] = "COMACMUS-";
constexpr char TEXT_COMACR[]     = "COMACR";
constexpr char TEXT_COMACRSU[]   = "COMACRSU";
constexpr char TEXT_COMACRU[]    = "COMACRU";
constexpr char TEXT_COMACRUS[]   = "COMACRUS";
constexpr char TEXT_COMACSU[]    = "COMACSU";
constexpr char TEXT_COMACSU_M[]  = "COMACSU-";
constexpr char TEXT_COMACU[]     = "COMACU";
constexpr char TEXT_COMACU_M[]   = "COMACU-";
constexpr char TEXT_COMACUS[]    = "COMACUS";
constexpr char TEXT_COMACUS_M[]  = "COMACUS-";
constexpr char TEXT_COMAX[]      = "COMAX";
constexpr char TEXT_COMIN[]      = "COMIN";
constexpr char TEXT_COMOV[]      = "COMOV";
constexpr char TEXT_COMUL[]      = "COMUL";
constexpr char TEXT_COMUL_M[]    = "COMUL-";
constexpr char TEXT_COMULSU[]    = "COMULSU";
constexpr char TEXT_COMULSU_M[]  = "COMULSU-";
constexpr char TEXT_COMULU[]     = "COMULU";
constexpr char TEXT_COMULU_M[]   = "COMULU-";
constexpr char TEXT_COMULUS[]    = "COMULUS";
constexpr char TEXT_COMULUS_M[]  = "COMULUS-";
constexpr char TEXT_CONEG[]      = "CONEG";
constexpr char TEXT_CONOP[]      = "CONOP";
constexpr char TEXT_CORND[]      = "CORND";
constexpr char TEXT_COSHL[]      = "COSHL";
constexpr char TEXT_COSHR[]      = "COSHR";
constexpr char TEXT_COSTORE[]    = "COSTORE";
constexpr char TEXT_COSUB2[]     = "COSUB2";
constexpr char TEXT_COSUB2R[]    = "COSUB2R";
constexpr char TEXT_COSUB[]      = "COSUB";
constexpr char TEXT_COSUBR[]     = "COSUBR";
constexpr char TEXT_CPLB[]       = "CPLB";
// constexpr char TEXT_CPL[]   = "CPL";
constexpr char TEXT_DISWDT[]     = "DISWDT";
// constexpr char TEXT_DIV[]   = "DIV";
// constexpr char TEXT_DIVL[]  = "DIVL";
constexpr char TEXT_DIVLU[]      = "DIVLU";
// constexpr char TEXT_DIVU[]  = "DIVU";
constexpr char TEXT_EINIT[]      = "EINIT";
constexpr char TEXT_ENWDT[]      = "ENWDT";
constexpr char TEXT_EXTP[]       = "EXTP";
constexpr char TEXT_EXTPR[]      = "EXTPR";
constexpr char TEXT_EXTR[]       = "EXTR";
// constexpr char TEXT_EXTS[]  = "EXTS";
constexpr char TEXT_EXTSR[]      = "EXTSR";
// constexpr char TEXT_IDLE[]  = "IDLE";
// constexpr char TEXT_JBC[]   = "JBC";
// constexpr char TEXT_JB[]    = "JB";
constexpr char TEXT_JMPA[]       = "JMPA";
constexpr char TEXT_JMPA_M[]     = "JMPA-";
constexpr char TEXT_JMPA_P[]     = "JMPA+";
// constexpr char TEXT_JMPI[]  = "JMPI";
constexpr char TEXT_JMPR[]       = "JMPR";
constexpr char TEXT_JMPS[]       = "JMPS";
// constexpr char TEXT_JNB[]   = "JNB";
constexpr char TEXT_JNBS[]       = "JNBS";
// constexpr char TEXT_MOVB[]  = "MOVB";
constexpr char TEXT_MOVBS[]      = "MOVBS";
constexpr char TEXT_MOVBZ[]      = "MOVBZ";
// constexpr char TEXT_MOV[]   = "MOV";
// constexpr char TEXT_MUL[]   = "MUL";
// constexpr char TEXT_MULU[]  = "MULU";
// constexpr char TEXT_NEGB[]  = "NEGB";
// constexpr char TEXT_NEG[]   = "NEG";
// constexpr char TEXT_NOP[]   = "NOP";
// constexpr char TEXT_ORB[]   = "ORB";
// constexpr char TEXT_OR[]    = "OR";
constexpr char TEXT_PCALL[]      = "PCALL";
// constexpr char TEXT_POP[]   = "POP";
constexpr char TEXT_PRIOR[]      = "PRIOR";
// constexpr char TEXT_PUSH[]  = "PUSH";
constexpr char TEXT_PWRDN[]      = "PWRDN";
// constexpr char TEXT_RET[]   = "RET";
// constexpr char TEXT_RETI[]  = "RETI";
constexpr char TEXT_RETP[]       = "RETP";
// constexpr char TEXT_RETS[]  = "RETS";
constexpr char TEXT_RND[]        = "RND";
constexpr char TEXT_REPEAT[] = "- ";
// constexpr char TEXT_ROL[]   = "ROL";
// constexpr char TEXT_ROR[]   = "ROR";
constexpr char TEXT_SBRK[]       = "SBRK";
constexpr char TEXT_SCXT[]       = "SCXT";
// constexpr char TEXT_SHL[]   = "SHL";
// constexpr char TEXT_SHR[]   = "SHR";
constexpr char TEXT_SRST[]       = "SRST";
constexpr char TEXT_SRVWDT[]     = "SRVWDT";
// constexpr char TEXT_SUBB[]  = "SUBB";
// constexpr char TEXT_SUBCB[] = "SUBCB";
// constexpr char TEXT_SUBC[]  = "SUBC";
// constexpr char TEXT_SUB[]   = "SUB";
// constexpr char TEXT_TRAP[]  = "TRAP";
// constexpr char TEXT_XORB[]  = "XORB";
// constexpr char TEXT_XOR[]   = "XOR";

// constexpr char TEXT_CC_C[]   = "C";
constexpr char TEXT_CC_[]      = "CC_";
// constexpr char TEXT_CC_EQ[]  = "EQ";
// constexpr char TEXT_CC_NC[]  = "NC";
constexpr char TEXT_CC_N[]     = "N";
// constexpr char TEXT_CC_NE[]  = "NE";
constexpr char TEXT_CC_NET[]   = "NET";
constexpr char TEXT_CC_NN[]    = "NN";
constexpr char TEXT_CC_NUSR0[] = "NUSR0";
constexpr char TEXT_CC_NUSR1[] = "NUSR1";
// constexpr char TEXT_CC_NV[]  = "NV";
// constexpr char TEXT_CC_NZ[]  = "NZ";
constexpr char TEXT_CC_SGE[]   = "SGE";
constexpr char TEXT_CC_SGT[]   = "SGT";
constexpr char TEXT_CC_SLE[]   = "SLE";
constexpr char TEXT_CC_SLT[]   = "SLT";
constexpr char TEXT_CC_UC[]    = "UC";
// constexpr char TEXT_CC_UGE[] = "UGE";
// constexpr char TEXT_CC_UGT[] = "UGT";
// constexpr char TEXT_CC_ULE[] = "ULE";
// constexpr char TEXT_CC_ULT[] = "ULT";
constexpr char TEXT_CC_USR0[]  = "USR0";
constexpr char TEXT_CC_USR1[]  = "USR1";
// constexpr char TEXT_CC_V[]   = "V";
// constexpr char TEXT_CC_Z[]   = "Z";

constexpr char TEXT_REG_CP[]      = "CP";
constexpr char TEXT_REG_CPUCON1[] = "CPUCON1";
constexpr char TEXT_REG_CPUCON2[] = "CPUCON2";
constexpr char TEXT_REG_CSP[]     = "CSP";
constexpr char TEXT_REG_DPP0[]    = "DPP0";
constexpr char TEXT_REG_DPP1[]    = "DPP1";
constexpr char TEXT_REG_DPP2[]    = "DPP2";
constexpr char TEXT_REG_DPP3[]    = "DPP3";
constexpr char TEXT_REG_IDX0[]    = "IDX0";
constexpr char TEXT_REG_IDX1[]    = "IDX1";
constexpr char TEXT_REG_MAH[]     = "MAH";
constexpr char TEXT_REG_MAL[]     = "MAL";
constexpr char TEXT_REG_MAS[]     = "MAS";
constexpr char TEXT_REG_MCW[]     = "MCW";
constexpr char TEXT_REG_MDC[]     = "MDC";
constexpr char TEXT_REG_MDH[]     = "MDH";
constexpr char TEXT_REG_MDL[]     = "MDL";
constexpr char TEXT_REG_MRW[]     = "MRW";
constexpr char TEXT_REG_MSW[]     = "MSW";
constexpr char TEXT_REG_ONES[]    = "ONES";
constexpr char TEXT_REG_QR0[]     = "QR0";
constexpr char TEXT_REG_QR1[]     = "QR1";
constexpr char TEXT_REG_QX0[]     = "QX0";
constexpr char TEXT_REG_QX1[]     = "QX1";
constexpr char TEXT_REG_SPSEG[]   = "SPSEG";
constexpr char TEXT_REG_STKOV[]   = "STKOV";
constexpr char TEXT_REG_STKUN[]   = "STKUN";
constexpr char TEXT_REG_TFR[]     = "TFR";
constexpr char TEXT_REG_VECSEG[]  = "VECSEG";
constexpr char TEXT_REG_ZEROS[]   = "ZEROS";

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
