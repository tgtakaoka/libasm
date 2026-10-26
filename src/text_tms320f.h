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
extern const char TEXT_TMS320F_LIST[];
extern const char TEXT_CPU_320C30[];
extern const char TEXT_CPU_320C31[];
extern const char TEXT_CPU_320C32[];

extern const char TEXT_SILICON[];
extern const char OPT_INT_SILICON[];
extern const char OPT_DESC_SILICON[];

// TMS320C3x
using common::TEXT_ABSF;
extern const char TEXT_ABSI[];
extern const char TEXT_ADDC3[];
using common::TEXT_ADDC;
extern const char TEXT_ADDF3[];
using common::TEXT_ADDF;
extern const char TEXT_ADDI3[];
using common::TEXT_ADDI;
using common::TEXT_ADD;
extern const char TEXT_AND3[];
extern const char TEXT_ANDN3[];
extern const char TEXT_ANDN[];
using common::TEXT_AND;
extern const char TEXT_ASH3[];
using common::TEXT_ASH;
using common::TEXT_B;
extern const char TEXT_BCD[];
using common::TEXT_BC;
using common::TEXT_BD;
extern const char TEXT_BEQD[];
using common::TEXT_BEQ;
extern const char TEXT_BGED[];
using common::TEXT_BGE;
extern const char TEXT_BGTD[];
using common::TEXT_BGT;
extern const char TEXT_BHID[];
using common::TEXT_BHI;
extern const char TEXT_BHSD[];
using common::TEXT_BHS;
extern const char TEXT_BLED[];
using common::TEXT_BLE;
extern const char TEXT_BLOD[];
using common::TEXT_BLO;
extern const char TEXT_BLSD[];
using common::TEXT_BLS;
extern const char TEXT_BLTD[];
using common::TEXT_BLT;
extern const char TEXT_BLUFD[];
extern const char TEXT_BLUF[];
extern const char TEXT_BLVD[];
extern const char TEXT_BLV[];
extern const char TEXT_BNCD[];
using common::TEXT_BNC;
using common::TEXT_BND;
extern const char TEXT_BNED[];
using common::TEXT_BNE;
extern const char TEXT_BNLUFD[];
extern const char TEXT_BNLUF[];
extern const char TEXT_BNLVD[];
extern const char TEXT_BNLV[];
extern const char TEXT_BNND[];
extern const char TEXT_BNN[];
extern const char TEXT_BNUFD[];
extern const char TEXT_BNUF[];
extern const char TEXT_BNVD[];
using common::TEXT_BNV;
extern const char TEXT_BNZD[];
using common::TEXT_BNZ;
extern const char TEXT_BN[];
extern const char TEXT_BPD[];
using common::TEXT_BP;
extern const char TEXT_BRD[];
using common::TEXT_BR;
extern const char TEXT_BUD[];
extern const char TEXT_BUFD[];
extern const char TEXT_BUF[];
extern const char TEXT_BU[];
extern const char TEXT_BVD[];
using common::TEXT_BV;
extern const char TEXT_BZD[];
extern const char TEXT_BZUFD[];
extern const char TEXT_BZUF[];
using common::TEXT_BZ;
extern const char TEXT_CALLC[];
extern const char TEXT_CALLEQ[];
extern const char TEXT_CALLGE[];
extern const char TEXT_CALLGT[];
extern const char TEXT_CALLHI[];
extern const char TEXT_CALLHS[];
extern const char TEXT_CALLLE[];
extern const char TEXT_CALLLO[];
extern const char TEXT_CALLLS[];
extern const char TEXT_CALLLT[];
extern const char TEXT_CALLLUF[];
extern const char TEXT_CALLLV[];
extern const char TEXT_CALLNC[];
extern const char TEXT_CALLNE[];
extern const char TEXT_CALLNLUF[];
extern const char TEXT_CALLNLV[];
extern const char TEXT_CALLNN[];
extern const char TEXT_CALLNUF[];
extern const char TEXT_CALLNV[];
extern const char TEXT_CALLNZ[];
using common::TEXT_CALLN;
extern const char TEXT_CALLP[];
extern const char TEXT_CALLUF[];
extern const char TEXT_CALLU[];
extern const char TEXT_CALLV[];
extern const char TEXT_CALLZUF[];
extern const char TEXT_CALLZ[];
using common::TEXT_CALL;
extern const char TEXT_CMPF3[];
using common::TEXT_CMPF;
extern const char TEXT_CMPI3[];
using common::TEXT_CMPI;
using common::TEXT_DB;
extern const char TEXT_DBCD[];
extern const char TEXT_DBC[];
extern const char TEXT_DBD[];
extern const char TEXT_DBEQD[];
using common::TEXT_DBEQ;
extern const char TEXT_DBGED[];
using common::TEXT_DBGE;
extern const char TEXT_DBGTD[];
using common::TEXT_DBGT;
extern const char TEXT_DBHID[];
using common::TEXT_DBHI;
extern const char TEXT_DBHSD[];
using common::TEXT_DBHS;
extern const char TEXT_DBLED[];
using common::TEXT_DBLE;
extern const char TEXT_DBLOD[];
using common::TEXT_DBLO;
extern const char TEXT_DBLSD[];
using common::TEXT_DBLS;
extern const char TEXT_DBLTD[];
using common::TEXT_DBLT;
extern const char TEXT_DBLUFD[];
extern const char TEXT_DBLUF[];
extern const char TEXT_DBLVD[];
extern const char TEXT_DBLV[];
extern const char TEXT_DBNCD[];
extern const char TEXT_DBNC[];
extern const char TEXT_DBND[];
extern const char TEXT_DBNED[];
using common::TEXT_DBNE;
extern const char TEXT_DBNLUFD[];
extern const char TEXT_DBNLUF[];
extern const char TEXT_DBNLVD[];
extern const char TEXT_DBNLV[];
extern const char TEXT_DBNND[];
extern const char TEXT_DBNN[];
extern const char TEXT_DBNUFD[];
extern const char TEXT_DBNUF[];
extern const char TEXT_DBNVD[];
extern const char TEXT_DBNV[];
extern const char TEXT_DBNZD[];
using common::TEXT_DBNZ;
extern const char TEXT_DBN[];
extern const char TEXT_DBPD[];
extern const char TEXT_DBP[];
extern const char TEXT_DBUD[];
extern const char TEXT_DBUFD[];
extern const char TEXT_DBUF[];
extern const char TEXT_DBU[];
extern const char TEXT_DBVD[];
extern const char TEXT_DBV[];
extern const char TEXT_DBZD[];
extern const char TEXT_DBZUFD[];
extern const char TEXT_DBZUF[];
extern const char TEXT_DBZ[];
using common::TEXT_FIX;
using common::TEXT_FLOAT;
extern const char TEXT_IACK[];
using common::TEXT_IDLE2;
using common::TEXT_IDLE;
using common::TEXT_LDE;
extern const char TEXT_LDFC[];
extern const char TEXT_LDFEQ[];
extern const char TEXT_LDFGE[];
extern const char TEXT_LDFGT[];
extern const char TEXT_LDFHI[];
extern const char TEXT_LDFHS[];
extern const char TEXT_LDFI[];
extern const char TEXT_LDFLE[];
extern const char TEXT_LDFLO[];
extern const char TEXT_LDFLS[];
extern const char TEXT_LDFLT[];
extern const char TEXT_LDFLUF[];
extern const char TEXT_LDFLV[];
extern const char TEXT_LDFNC[];
extern const char TEXT_LDFNE[];
extern const char TEXT_LDFNLUF[];
extern const char TEXT_LDFNLV[];
extern const char TEXT_LDFNN[];
extern const char TEXT_LDFNUF[];
extern const char TEXT_LDFNV[];
extern const char TEXT_LDFNZ[];
extern const char TEXT_LDFN[];
extern const char TEXT_LDFP[];
extern const char TEXT_LDFUF[];
extern const char TEXT_LDFU[];
extern const char TEXT_LDFV[];
extern const char TEXT_LDFZUF[];
extern const char TEXT_LDFZ[];
using common::TEXT_LDF;
extern const char TEXT_LDIC[];
extern const char TEXT_LDIEQ[];
extern const char TEXT_LDIGE[];
extern const char TEXT_LDIGT[];
extern const char TEXT_LDIHI[];
extern const char TEXT_LDIHS[];
extern const char TEXT_LDII[];
extern const char TEXT_LDILE[];
extern const char TEXT_LDILO[];
extern const char TEXT_LDILS[];
extern const char TEXT_LDILT[];
extern const char TEXT_LDILUF[];
extern const char TEXT_LDILV[];
extern const char TEXT_LDINC[];
extern const char TEXT_LDINE[];
extern const char TEXT_LDINLUF[];
extern const char TEXT_LDINLV[];
extern const char TEXT_LDINN[];
extern const char TEXT_LDINUF[];
extern const char TEXT_LDINV[];
extern const char TEXT_LDINZ[];
extern const char TEXT_LDIN[];
extern const char TEXT_LDIP[];
extern const char TEXT_LDIUF[];
extern const char TEXT_LDIU[];
extern const char TEXT_LDIV[];
extern const char TEXT_LDIZUF[];
extern const char TEXT_LDIZ[];
using common::TEXT_LDI;
using common::TEXT_LDM;
using common::TEXT_LDP;
extern const char TEXT_LOPOWER[];
extern const char TEXT_LSH3[];
extern const char TEXT_LSH[];
extern const char TEXT_MAXSPEED[];
extern const char TEXT_MPYF3[];
extern const char TEXT_MPYF[];
extern const char TEXT_MPYI3[];
extern const char TEXT_MPYI[];
using common::TEXT_NEGB;
using common::TEXT_NEGF;
extern const char TEXT_NEGI[];
using common::TEXT_NOP;
using common::TEXT_NORM;
using common::TEXT_NOT;
extern const char TEXT_OR3[];
using common::TEXT_OR;
using common::TEXT_POPF;
using common::TEXT_POP;
using common::TEXT_PUSHF;
using common::TEXT_PUSH;
extern const char TEXT_RETIC[];
extern const char TEXT_RETIEQ[];
extern const char TEXT_RETIGE[];
extern const char TEXT_RETIGT[];
extern const char TEXT_RETIHI[];
extern const char TEXT_RETIHS[];
extern const char TEXT_RETILE[];
extern const char TEXT_RETILO[];
extern const char TEXT_RETILS[];
extern const char TEXT_RETILT[];
extern const char TEXT_RETILUF[];
extern const char TEXT_RETILV[];
extern const char TEXT_RETINC[];
extern const char TEXT_RETINE[];
extern const char TEXT_RETINLUF[];
extern const char TEXT_RETINLV[];
extern const char TEXT_RETINN[];
extern const char TEXT_RETINUF[];
extern const char TEXT_RETINV[];
extern const char TEXT_RETINZ[];
extern const char TEXT_RETIN[];
extern const char TEXT_RETIP[];
extern const char TEXT_RETIUF[];
extern const char TEXT_RETIU[];
extern const char TEXT_RETIV[];
extern const char TEXT_RETIZUF[];
extern const char TEXT_RETIZ[];
using common::TEXT_RETI;
extern const char TEXT_RETSC[];
extern const char TEXT_RETSEQ[];
extern const char TEXT_RETSGE[];
extern const char TEXT_RETSGT[];
extern const char TEXT_RETSHI[];
extern const char TEXT_RETSHS[];
extern const char TEXT_RETSLE[];
extern const char TEXT_RETSLO[];
extern const char TEXT_RETSLS[];
extern const char TEXT_RETSLT[];
extern const char TEXT_RETSLUF[];
extern const char TEXT_RETSLV[];
extern const char TEXT_RETSNC[];
extern const char TEXT_RETSNE[];
extern const char TEXT_RETSNLUF[];
extern const char TEXT_RETSNLV[];
extern const char TEXT_RETSNN[];
extern const char TEXT_RETSNUF[];
extern const char TEXT_RETSNV[];
extern const char TEXT_RETSNZ[];
extern const char TEXT_RETSN[];
extern const char TEXT_RETSP[];
extern const char TEXT_RETSUF[];
extern const char TEXT_RETSU[];
extern const char TEXT_RETSV[];
extern const char TEXT_RETSZUF[];
extern const char TEXT_RETSZ[];
using common::TEXT_RETS;
extern const char TEXT_RND[];
extern const char TEXT_ROLC[];
using common::TEXT_ROL;
extern const char TEXT_RORC[];
using common::TEXT_ROR;
using common::TEXT_RPTB;
extern const char TEXT_RPTS[];
extern const char TEXT_SIGI[];
extern const char TEXT_STFI[];
using common::TEXT_STF;
extern const char TEXT_STII[];
using common::TEXT_STI;
extern const char TEXT_SUBB3[];
using common::TEXT_SUBB;
using common::TEXT_SUBC;
extern const char TEXT_SUBF3[];
using common::TEXT_SUBF;
extern const char TEXT_SUBI3[];
using common::TEXT_SUBI;
extern const char TEXT_SUBRB[];
extern const char TEXT_SUBRF[];
extern const char TEXT_SUBRI[];
using common::TEXT_SUB;
using common::TEXT_SWI;
extern const char TEXT_TRAPC[];
using common::TEXT_TRAPEQ;
using common::TEXT_TRAPGE;
using common::TEXT_TRAPGT;
using common::TEXT_TRAPHI;
using common::TEXT_TRAPHS;
using common::TEXT_TRAPLE;
using common::TEXT_TRAPLO;
using common::TEXT_TRAPLS;
using common::TEXT_TRAPLT;
extern const char TEXT_TRAPLUF[];
extern const char TEXT_TRAPLV[];
extern const char TEXT_TRAPNC[];
using common::TEXT_TRAPNE;
extern const char TEXT_TRAPNLUF[];
extern const char TEXT_TRAPNLV[];
extern const char TEXT_TRAPNN[];
extern const char TEXT_TRAPNUF[];
extern const char TEXT_TRAPNV[];
extern const char TEXT_TRAPNZ[];
extern const char TEXT_TRAPN[];
extern const char TEXT_TRAPP[];
extern const char TEXT_TRAPUF[];
extern const char TEXT_TRAPU[];
using common::TEXT_TRAPV;
extern const char TEXT_TRAPZUF[];
extern const char TEXT_TRAPZ[];
using common::TEXT_TRAP;
extern const char TEXT_TSTB3[];
using common::TEXT_TSTB;
extern const char TEXT_XOR3[];
using common::TEXT_XOR;

using common::TEXT_REG_AR;
extern const char TEXT_REG_BK[];
using common::TEXT_REG_DP;
extern const char TEXT_REG_IE[];
extern const char TEXT_REG_IF[];
extern const char TEXT_REG_IOF[];
extern const char TEXT_REG_IR0[];
extern const char TEXT_REG_IR1[];
using common::TEXT_REG_RC;
using common::TEXT_REG_RE;
using common::TEXT_REG_RS;
using common::TEXT_REG_R;
using common::TEXT_REG_SP;
using common::TEXT_REG_ST;

extern const char TEXT_dFLOAT[];   // .float
extern const char TEXT_dHWORD[];   // .hword
extern const char TEXT_dINT[];     // .int
extern const char TEXT_dLDOUBLE[]; // .ldouble
extern const char TEXT_dSFLOAT[];  // .sfloat
extern const char TEXT_dSPACE[];   // .space
extern const char TEXT_PARALLEL[]; // ||

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
