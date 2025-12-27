/*
 * Copyright 2025 Tadashi G. Takaoka
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

#ifndef __LIBASM_TEXT_TMS320F__
#define __LIBASM_TEXT_TMS320F__

#include "text_common.h"

namespace libasm {
namespace text {
namespace tms320f {

// clang-format off
extern const char TEXT_TMS320F_LIST[] PROGMEM;
extern const char TEXT_CPU_320C30[] PROGMEM;
extern const char TEXT_CPU_320C31[] PROGMEM;

// TMS320C3x
using common::TEXT_ABSF;
extern const char TEXT_ABSI[]      PROGMEM;
extern const char TEXT_ADDC3[]     PROGMEM;
using common::TEXT_ADDC;
extern const char TEXT_ADDF3[]     PROGMEM;
using common::TEXT_ADDF;
extern const char TEXT_ADDI3[]     PROGMEM;
using common::TEXT_ADDI;
using common::TEXT_ADD;
extern const char TEXT_AND3[]      PROGMEM;
extern const char TEXT_ANDN3[]     PROGMEM;
extern const char TEXT_ANDN[]      PROGMEM;
using common::TEXT_AND;
extern const char TEXT_ASH3[]      PROGMEM;
using common::TEXT_ASH;
using common::TEXT_B;
extern const char TEXT_BCD[]       PROGMEM;
using common::TEXT_BC;
using common::TEXT_BD;
extern const char TEXT_BEQD[]      PROGMEM;
using common::TEXT_BEQ;
extern const char TEXT_BGED[]      PROGMEM;
using common::TEXT_BGE;
extern const char TEXT_BGTD[]      PROGMEM;
using common::TEXT_BGT;
extern const char TEXT_BHID[]      PROGMEM;
using common::TEXT_BHI;
extern const char TEXT_BHSD[]      PROGMEM;
using common::TEXT_BHS;
extern const char TEXT_BLED[]      PROGMEM;
using common::TEXT_BLE;
extern const char TEXT_BLOD[]      PROGMEM;
using common::TEXT_BLO;
extern const char TEXT_BLSD[]      PROGMEM;
using common::TEXT_BLS;
extern const char TEXT_BLTD[]      PROGMEM;
using common::TEXT_BLT;
extern const char TEXT_BLUFD[]     PROGMEM;
extern const char TEXT_BLUF[]      PROGMEM;
extern const char TEXT_BLVD[]      PROGMEM;
extern const char TEXT_BLV[]       PROGMEM;
extern const char TEXT_BNCD[]      PROGMEM;
using common::TEXT_BNC;
using common::TEXT_BND;
extern const char TEXT_BNED[]      PROGMEM;
using common::TEXT_BNE;
extern const char TEXT_BNLUFD[]    PROGMEM;
extern const char TEXT_BNLUF[]     PROGMEM;
extern const char TEXT_BNLVD[]     PROGMEM;
extern const char TEXT_BNLV[]      PROGMEM;
extern const char TEXT_BNND[]      PROGMEM;
extern const char TEXT_BNN[]       PROGMEM;
extern const char TEXT_BNUFD[]     PROGMEM;
extern const char TEXT_BNUF[]      PROGMEM;
extern const char TEXT_BNVD[]      PROGMEM;
using common::TEXT_BNV;
extern const char TEXT_BNZD[]      PROGMEM;
using common::TEXT_BNZ;
extern const char TEXT_BN[]        PROGMEM;
extern const char TEXT_BPD[]       PROGMEM;
using common::TEXT_BP;
extern const char TEXT_BRD[]       PROGMEM;
using common::TEXT_BR;
extern const char TEXT_BUD[]       PROGMEM;
extern const char TEXT_BUFD[]      PROGMEM;
extern const char TEXT_BUF[]       PROGMEM;
extern const char TEXT_BU[]        PROGMEM;
extern const char TEXT_BVD[]       PROGMEM;
using common::TEXT_BV;
extern const char TEXT_BZD[]       PROGMEM;
extern const char TEXT_BZUFD[]     PROGMEM;
extern const char TEXT_BZUF[]      PROGMEM;
using common::TEXT_BZ;
extern const char TEXT_CALLCD[]    PROGMEM;
extern const char TEXT_CALLC[]     PROGMEM;
extern const char TEXT_CALLEQD[]   PROGMEM;
extern const char TEXT_CALLEQ[]    PROGMEM;
extern const char TEXT_CALLGED[]   PROGMEM;
extern const char TEXT_CALLGE[]    PROGMEM;
extern const char TEXT_CALLGTD[]   PROGMEM;
extern const char TEXT_CALLGT[]    PROGMEM;
extern const char TEXT_CALLHID[]   PROGMEM;
extern const char TEXT_CALLHI[]    PROGMEM;
extern const char TEXT_CALLHSD[]   PROGMEM;
extern const char TEXT_CALLHS[]    PROGMEM;
extern const char TEXT_CALLLED[]   PROGMEM;
extern const char TEXT_CALLLE[]    PROGMEM;
extern const char TEXT_CALLLOD[]   PROGMEM;
extern const char TEXT_CALLLO[]    PROGMEM;
extern const char TEXT_CALLLSD[]   PROGMEM;
extern const char TEXT_CALLLS[]    PROGMEM;
extern const char TEXT_CALLLTD[]   PROGMEM;
extern const char TEXT_CALLLT[]    PROGMEM;
extern const char TEXT_CALLLUFD[]  PROGMEM;
extern const char TEXT_CALLLUF[]   PROGMEM;
extern const char TEXT_CALLLVD[]   PROGMEM;
extern const char TEXT_CALLLV[]    PROGMEM;
extern const char TEXT_CALLNCD[]   PROGMEM;
extern const char TEXT_CALLNC[]    PROGMEM;
extern const char TEXT_CALLND[]    PROGMEM;
extern const char TEXT_CALLNED[]   PROGMEM;
extern const char TEXT_CALLNE[]    PROGMEM;
extern const char TEXT_CALLNLUFD[] PROGMEM;
extern const char TEXT_CALLNLUF[]  PROGMEM;
extern const char TEXT_CALLNLVD[]  PROGMEM;
extern const char TEXT_CALLNLV[]   PROGMEM;
extern const char TEXT_CALLNND[]   PROGMEM;
extern const char TEXT_CALLNN[]    PROGMEM;
extern const char TEXT_CALLNUFD[]  PROGMEM;
extern const char TEXT_CALLNUF[]   PROGMEM;
extern const char TEXT_CALLNVD[]   PROGMEM;
extern const char TEXT_CALLNV[]    PROGMEM;
extern const char TEXT_CALLNZD[]   PROGMEM;
extern const char TEXT_CALLNZ[]    PROGMEM;
using common::TEXT_CALLN;
extern const char TEXT_CALLPD[]    PROGMEM;
extern const char TEXT_CALLP[]     PROGMEM;
extern const char TEXT_CALLUD[]    PROGMEM;
extern const char TEXT_CALLUFD[]   PROGMEM;
extern const char TEXT_CALLUF[]    PROGMEM;
extern const char TEXT_CALLU[]     PROGMEM;
extern const char TEXT_CALLVD[]    PROGMEM;
extern const char TEXT_CALLV[]     PROGMEM;
extern const char TEXT_CALLZD[]    PROGMEM;
extern const char TEXT_CALLZUFD[]  PROGMEM;
extern const char TEXT_CALLZUF[]   PROGMEM;
extern const char TEXT_CALLZ[]     PROGMEM;
using common::TEXT_CALL;
extern const char TEXT_CMPF3[]     PROGMEM;
using common::TEXT_CMPF;
extern const char TEXT_CMPI3[]     PROGMEM;
using common::TEXT_CMPI;
using common::TEXT_DB;
extern const char TEXT_DBCD[]      PROGMEM;
extern const char TEXT_DBC[]       PROGMEM;
extern const char TEXT_DBD[]       PROGMEM;
extern const char TEXT_DBEQD[]     PROGMEM;
using common::TEXT_DBEQ;
extern const char TEXT_DBGED[]     PROGMEM;
using common::TEXT_DBGE;
extern const char TEXT_DBGTD[]     PROGMEM;
using common::TEXT_DBGT;
extern const char TEXT_DBHID[]     PROGMEM;
using common::TEXT_DBHI;
extern const char TEXT_DBHSD[]     PROGMEM;
using common::TEXT_DBHS;
extern const char TEXT_DBLED[]     PROGMEM;
using common::TEXT_DBLE;
extern const char TEXT_DBLOD[]     PROGMEM;
using common::TEXT_DBLO;
extern const char TEXT_DBLSD[]     PROGMEM;
using common::TEXT_DBLS;
extern const char TEXT_DBLTD[]     PROGMEM;
using common::TEXT_DBLT;
extern const char TEXT_DBLUFD[]    PROGMEM;
extern const char TEXT_DBLUF[]     PROGMEM;
extern const char TEXT_DBLVD[]     PROGMEM;
extern const char TEXT_DBLV[]      PROGMEM;
extern const char TEXT_DBNCD[]     PROGMEM;
extern const char TEXT_DBNC[]      PROGMEM;
extern const char TEXT_DBND[]      PROGMEM;
extern const char TEXT_DBNED[]     PROGMEM;
using common::TEXT_DBNE;
extern const char TEXT_DBNLUFD[]   PROGMEM;
extern const char TEXT_DBNLUF[]    PROGMEM;
extern const char TEXT_DBNLVD[]    PROGMEM;
extern const char TEXT_DBNLV[]     PROGMEM;
extern const char TEXT_DBNND[]     PROGMEM;
extern const char TEXT_DBNN[]      PROGMEM;
extern const char TEXT_DBNUFD[]    PROGMEM;
extern const char TEXT_DBNUF[]     PROGMEM;
extern const char TEXT_DBNVD[]     PROGMEM;
extern const char TEXT_DBNV[]      PROGMEM;
extern const char TEXT_DBNZD[]     PROGMEM;
using common::TEXT_DBNZ;
extern const char TEXT_DBN[]       PROGMEM;
extern const char TEXT_DBPD[]      PROGMEM;
extern const char TEXT_DBP[]       PROGMEM;
extern const char TEXT_DBUD[]      PROGMEM;
extern const char TEXT_DBUFD[]     PROGMEM;
extern const char TEXT_DBUF[]      PROGMEM;
extern const char TEXT_DBU[]       PROGMEM;
extern const char TEXT_DBVD[]      PROGMEM;
extern const char TEXT_DBV[]       PROGMEM;
extern const char TEXT_DBZD[]      PROGMEM;
extern const char TEXT_DBZUFD[]    PROGMEM;
extern const char TEXT_DBZUF[]     PROGMEM;
extern const char TEXT_DBZ[]       PROGMEM;
using common::TEXT_FIX;
extern const char TEXT_FLOAT[]     PROGMEM;
extern const char TEXT_IACK[]      PROGMEM;
using common::TEXT_IDLE2;
using common::TEXT_IDLE;
using common::TEXT_LDE;
extern const char TEXT_LDFC[]      PROGMEM;
extern const char TEXT_LDFEQ[]     PROGMEM;
extern const char TEXT_LDFGE[]     PROGMEM;
extern const char TEXT_LDFGT[]     PROGMEM;
extern const char TEXT_LDFHI[]     PROGMEM;
extern const char TEXT_LDFHS[]     PROGMEM;
extern const char TEXT_LDFI[]      PROGMEM;
extern const char TEXT_LDFLE[]     PROGMEM;
extern const char TEXT_LDFLO[]     PROGMEM;
extern const char TEXT_LDFLS[]     PROGMEM;
extern const char TEXT_LDFLT[]     PROGMEM;
extern const char TEXT_LDFLUF[]    PROGMEM;
extern const char TEXT_LDFLV[]     PROGMEM;
extern const char TEXT_LDFNC[]     PROGMEM;
extern const char TEXT_LDFNE[]     PROGMEM;
extern const char TEXT_LDFNLUF[]   PROGMEM;
extern const char TEXT_LDFNLV[]    PROGMEM;
extern const char TEXT_LDFNN[]     PROGMEM;
extern const char TEXT_LDFNUF[]    PROGMEM;
extern const char TEXT_LDFNV[]     PROGMEM;
extern const char TEXT_LDFNZ[]     PROGMEM;
extern const char TEXT_LDFN[]      PROGMEM;
extern const char TEXT_LDFP[]      PROGMEM;
extern const char TEXT_LDFUF[]     PROGMEM;
extern const char TEXT_LDFU[]      PROGMEM;
extern const char TEXT_LDFV[]      PROGMEM;
extern const char TEXT_LDFZUF[]    PROGMEM;
extern const char TEXT_LDFZ[]      PROGMEM;
using common::TEXT_LDF;
extern const char TEXT_LDIC[]      PROGMEM;
extern const char TEXT_LDIEQ[]     PROGMEM;
extern const char TEXT_LDIGE[]     PROGMEM;
extern const char TEXT_LDIGT[]     PROGMEM;
extern const char TEXT_LDIHI[]     PROGMEM;
extern const char TEXT_LDIHS[]     PROGMEM;
extern const char TEXT_LDII[]      PROGMEM;
extern const char TEXT_LDILE[]     PROGMEM;
extern const char TEXT_LDILO[]     PROGMEM;
extern const char TEXT_LDILS[]     PROGMEM;
extern const char TEXT_LDILT[]     PROGMEM;
extern const char TEXT_LDILUF[]    PROGMEM;
extern const char TEXT_LDILV[]     PROGMEM;
extern const char TEXT_LDINC[]     PROGMEM;
extern const char TEXT_LDINE[]     PROGMEM;
extern const char TEXT_LDINLUF[]   PROGMEM;
extern const char TEXT_LDINLV[]    PROGMEM;
extern const char TEXT_LDINN[]     PROGMEM;
extern const char TEXT_LDINUF[]    PROGMEM;
extern const char TEXT_LDINV[]     PROGMEM;
extern const char TEXT_LDINZ[]     PROGMEM;
extern const char TEXT_LDIN[]      PROGMEM;
extern const char TEXT_LDIP[]      PROGMEM;
extern const char TEXT_LDIUF[]     PROGMEM;
extern const char TEXT_LDIU[]      PROGMEM;
extern const char TEXT_LDIV[]      PROGMEM;
extern const char TEXT_LDIZUF[]    PROGMEM;
extern const char TEXT_LDIZ[]      PROGMEM;
using common::TEXT_LDI;
using common::TEXT_LDM;
using common::TEXT_LDP;
extern const char TEXT_LOPOWER[]   PROGMEM;
extern const char TEXT_LSH3[]      PROGMEM;
extern const char TEXT_LSH[]       PROGMEM;
extern const char TEXT_MAXSPEED[]  PROGMEM;
extern const char TEXT_MPYF3[]     PROGMEM;
extern const char TEXT_MPYF[]      PROGMEM;
extern const char TEXT_MPYI3[]     PROGMEM;
extern const char TEXT_MPYI[]      PROGMEM;
using common::TEXT_NEGB;
using common::TEXT_NEGF;
extern const char TEXT_NEGI[]      PROGMEM;
using common::TEXT_NOP;
using common::TEXT_NORM;
using common::TEXT_NOT;
extern const char TEXT_OR3[]       PROGMEM;
using common::TEXT_OR;
using common::TEXT_POPF;
using common::TEXT_POP;
using common::TEXT_PUSHF;
using common::TEXT_PUSH;
extern const char TEXT_RETIC[]     PROGMEM;
extern const char TEXT_RETIEQ[]    PROGMEM;
extern const char TEXT_RETIGE[]    PROGMEM;
extern const char TEXT_RETIGT[]    PROGMEM;
extern const char TEXT_RETIHI[]    PROGMEM;
extern const char TEXT_RETIHS[]    PROGMEM;
extern const char TEXT_RETILE[]    PROGMEM;
extern const char TEXT_RETILO[]    PROGMEM;
extern const char TEXT_RETILS[]    PROGMEM;
extern const char TEXT_RETILT[]    PROGMEM;
extern const char TEXT_RETILUF[]   PROGMEM;
extern const char TEXT_RETILV[]    PROGMEM;
extern const char TEXT_RETINC[]    PROGMEM;
extern const char TEXT_RETINE[]    PROGMEM;
extern const char TEXT_RETINLUF[]  PROGMEM;
extern const char TEXT_RETINLV[]   PROGMEM;
extern const char TEXT_RETINN[]    PROGMEM;
extern const char TEXT_RETINUF[]   PROGMEM;
extern const char TEXT_RETINV[]    PROGMEM;
extern const char TEXT_RETINZ[]    PROGMEM;
extern const char TEXT_RETIN[]     PROGMEM;
extern const char TEXT_RETIP[]     PROGMEM;
extern const char TEXT_RETIUF[]    PROGMEM;
extern const char TEXT_RETIU[]     PROGMEM;
extern const char TEXT_RETIV[]     PROGMEM;
extern const char TEXT_RETIZUF[]   PROGMEM;
extern const char TEXT_RETIZ[]     PROGMEM;
using common::TEXT_RETI;
extern const char TEXT_RETSC[]     PROGMEM;
extern const char TEXT_RETSEQ[]    PROGMEM;
extern const char TEXT_RETSGE[]    PROGMEM;
extern const char TEXT_RETSGT[]    PROGMEM;
extern const char TEXT_RETSHI[]    PROGMEM;
extern const char TEXT_RETSHS[]    PROGMEM;
extern const char TEXT_RETSLE[]    PROGMEM;
extern const char TEXT_RETSLO[]    PROGMEM;
extern const char TEXT_RETSLS[]    PROGMEM;
extern const char TEXT_RETSLT[]    PROGMEM;
extern const char TEXT_RETSLUF[]   PROGMEM;
extern const char TEXT_RETSLV[]    PROGMEM;
extern const char TEXT_RETSNC[]    PROGMEM;
extern const char TEXT_RETSNE[]    PROGMEM;
extern const char TEXT_RETSNLUF[]  PROGMEM;
extern const char TEXT_RETSNLV[]   PROGMEM;
extern const char TEXT_RETSNN[]    PROGMEM;
extern const char TEXT_RETSNUF[]   PROGMEM;
extern const char TEXT_RETSNV[]    PROGMEM;
extern const char TEXT_RETSNZ[]    PROGMEM;
extern const char TEXT_RETSN[]     PROGMEM;
extern const char TEXT_RETSP[]     PROGMEM;
extern const char TEXT_RETSUF[]    PROGMEM;
extern const char TEXT_RETSU[]     PROGMEM;
extern const char TEXT_RETSV[]     PROGMEM;
extern const char TEXT_RETSZUF[]   PROGMEM;
extern const char TEXT_RETSZ[]     PROGMEM;
using common::TEXT_RETS;
extern const char TEXT_RND[]       PROGMEM;
extern const char TEXT_ROLC[]      PROGMEM;
using common::TEXT_ROL;
extern const char TEXT_RORC[]      PROGMEM;
using common::TEXT_ROR;
using common::TEXT_RPTB;
extern const char TEXT_RPTS[]      PROGMEM;
extern const char TEXT_SIGI[]      PROGMEM;
extern const char TEXT_STFI[]      PROGMEM;
using common::TEXT_STF;
extern const char TEXT_STII[]      PROGMEM;
using common::TEXT_STI;
extern const char TEXT_SUB3[]      PROGMEM;
extern const char TEXT_SUBB3[]     PROGMEM;
using common::TEXT_SUBB;
using common::TEXT_SUBC;
extern const char TEXT_SUBF3[]     PROGMEM;
using common::TEXT_SUBF;
extern const char TEXT_SUBI3[]     PROGMEM;
using common::TEXT_SUBI;
extern const char TEXT_SUBRB[]     PROGMEM;
extern const char TEXT_SUBRF[]     PROGMEM;
extern const char TEXT_SUBRI[]     PROGMEM;
using common::TEXT_SUB;
using common::TEXT_SWI;
extern const char TEXT_TRAPC[]     PROGMEM;
using common::TEXT_TRAPEQ;
using common::TEXT_TRAPGE;
using common::TEXT_TRAPGT;
using common::TEXT_TRAPHI;
using common::TEXT_TRAPHS;
using common::TEXT_TRAPLE;
using common::TEXT_TRAPLO;
using common::TEXT_TRAPLS;
using common::TEXT_TRAPLT;
extern const char TEXT_TRAPLUF[]   PROGMEM;
extern const char TEXT_TRAPLV[]    PROGMEM;
extern const char TEXT_TRAPNC[]    PROGMEM;
using common::TEXT_TRAPNE;
extern const char TEXT_TRAPNLUF[]  PROGMEM;
extern const char TEXT_TRAPNLV[]   PROGMEM;
extern const char TEXT_TRAPNN[]    PROGMEM;
extern const char TEXT_TRAPNUF[]   PROGMEM;
extern const char TEXT_TRAPNV[]    PROGMEM;
extern const char TEXT_TRAPNZ[]    PROGMEM;
extern const char TEXT_TRAPN[]     PROGMEM;
extern const char TEXT_TRAPP[]     PROGMEM;
extern const char TEXT_TRAPUF[]    PROGMEM;
extern const char TEXT_TRAPU[]     PROGMEM;
using common::TEXT_TRAPV;
extern const char TEXT_TRAPZUF[]   PROGMEM;
extern const char TEXT_TRAPZ[]     PROGMEM;
using common::TEXT_TRAP;
extern const char TEXT_TSTB3[]     PROGMEM;
using common::TEXT_TSTB;
extern const char TEXT_XOR3[]      PROGMEM;
using common::TEXT_XOR;

using common::TEXT_REG_AR;
extern const char TEXT_REG_BK[]  PROGMEM;
using common::TEXT_REG_DP;
extern const char TEXT_REG_IE[]  PROGMEM;
extern const char TEXT_REG_IF[]  PROGMEM;
extern const char TEXT_REG_IOF[] PROGMEM;
extern const char TEXT_REG_IR0[] PROGMEM;
extern const char TEXT_REG_IR1[] PROGMEM;
extern const char TEXT_REG_RC[]  PROGMEM;
extern const char TEXT_REG_RE[]  PROGMEM;
extern const char TEXT_REG_RS[]  PROGMEM;
using common::TEXT_REG_R;
using common::TEXT_REG_SP;
using common::TEXT_REG_ST;

extern const char TEXT_dFLOAT[]   PROGMEM; // .float
extern const char TEXT_dHWORD[]   PROGMEM; // .hword
extern const char TEXT_dINT[]     PROGMEM; // .int
extern const char TEXT_dLDOUBLE[] PROGMEM; // .ldouble
extern const char TEXT_dSFLOAT[]  PROGMEM; // .sfloat
extern const char TEXT_dSPACE[]   PROGMEM; // .space

// clang-format on
}  // namespace tms320f
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_TMS320F__
// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
