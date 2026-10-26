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

#ifndef __LIBASM_TEXT_MC68000_H__
#define __LIBASM_TEXT_MC68000_H__

#include "text_common.h"

namespace libasm {
namespace text {
namespace mc68000 {
// clang-format off

extern const char TEXT_MC68000_LIST[];
extern const char TEXT_CPU_MC68000[];
extern const char TEXT_CPU_MC68010[];
extern const char TEXT_CPU_MC68020[];
extern const char TEXT_CPU_MC68030[];
extern const char TEXT_CPU_MC68040[];
extern const char TEXT_FPU_MC68881[];
extern const char TEXT_PMMU_MC68851[];
extern const char TEXT_CPU_68000[];
extern const char TEXT_CPU_68010[];
extern const char TEXT_CPU_68020[];
extern const char TEXT_CPU_68030[];
extern const char TEXT_CPU_68040[];
extern const char TEXT_FPU_68881[];
extern const char TEXT_PMMU_68851[];
extern const char TEXT_PMMU_68EC030[];
extern const char TEXT_PMMU_68EC040[];

using common::TEXT_none;
using common::TEXT_FPU;
using common::TEXT_PMMU;

// MC68000 instructions
extern const char TEXT_ABCD[];
using common::TEXT_ADD;
using common::TEXT_ADDA;
using common::TEXT_ADDI;
extern const char TEXT_ADDQ[];
using common::TEXT_ADDX;
using common::TEXT_AND;
using common::TEXT_ANDI;
using common::TEXT_ASL;
using common::TEXT_ASR;
using common::TEXT_BCC;
extern const char TEXT_BCHG[];
using common::TEXT_BCLR;
using common::TEXT_BCS;
using common::TEXT_BEQ;
using common::TEXT_BGE;
using common::TEXT_BGT;
using common::TEXT_BHI;
using common::TEXT_BHS;
using common::TEXT_BLE;
using common::TEXT_BLO;
using common::TEXT_BLS;
using common::TEXT_BLT;
using common::TEXT_BMI;
using common::TEXT_BNE;
using common::TEXT_BPL;
using common::TEXT_BRA;
using common::TEXT_BSET;
using common::TEXT_BSR;
using common::TEXT_BT;
using common::TEXT_BTST;
using common::TEXT_BVC;
using common::TEXT_BVS;
extern const char TEXT_CHK[];
using common::TEXT_CLR;
using common::TEXT_CMP;
using common::TEXT_CMPA;
using common::TEXT_CMPI;
extern const char TEXT_CMPM[];
extern const char TEXT_DBCC[];
extern const char TEXT_DBCS[];
using common::TEXT_DBEQ;
extern const char TEXT_DBF[];
using common::TEXT_DBGE;
using common::TEXT_DBGT;
using common::TEXT_DBHI;
using common::TEXT_DBHS;
using common::TEXT_DBLE;
using common::TEXT_DBLO;
using common::TEXT_DBLS;
using common::TEXT_DBLT;
extern const char TEXT_DBMI[];
using common::TEXT_DBNE;
extern const char TEXT_DBPL[];
extern const char TEXT_DBRA[];
extern const char TEXT_DBT[];
extern const char TEXT_DBVC[];
extern const char TEXT_DBVS[];
using common::TEXT_DIVS;
using common::TEXT_DIVU;
using common::TEXT_EOR;
using common::TEXT_EORI;
using common::TEXT_EXG;
using common::TEXT_EXT;
extern const char TEXT_ILLEGAL[];
using common::TEXT_JMP;
using common::TEXT_JSR;
using common::TEXT_LEA;
using common::TEXT_LINK;
using common::TEXT_LSL;
using common::TEXT_LSR;
extern const char TEXT_MOVE[];
extern const char TEXT_MOVEA[];
extern const char TEXT_MOVEM[];
extern const char TEXT_MOVEP[];
extern const char TEXT_MOVEQ[];
using common::TEXT_MULS;
using common::TEXT_MULU;
extern const char TEXT_NBCD[];
using common::TEXT_NEG;
using common::TEXT_NEGX;
using common::TEXT_NOP;
using common::TEXT_NOT;
using common::TEXT_OR;
using common::TEXT_ORI;
using common::TEXT_PEA;
using common::TEXT_RESET;
using common::TEXT_ROL;
using common::TEXT_ROR;
extern const char TEXT_ROXL[];
extern const char TEXT_ROXR[];
using common::TEXT_RTE;
using common::TEXT_RTR;
using common::TEXT_RTS;
using common::TEXT_SBCD;
using common::TEXT_SCC;
extern const char TEXT_SCS[];
using common::TEXT_SEQ;
extern const char TEXT_SF[];
extern const char TEXT_SGE[];
using common::TEXT_SGT;
extern const char TEXT_SHI[];
extern const char TEXT_SHS[];
extern const char TEXT_SLE[];
extern const char TEXT_SLO[];
extern const char TEXT_SLS[];
extern const char TEXT_SLT[];
using common::TEXT_SMI;
extern const char TEXT_SNE[];
using common::TEXT_SPL;
using common::TEXT_ST;
using common::TEXT_STOP;
using common::TEXT_SUB;
using common::TEXT_SUBA;
using common::TEXT_SUBI;
extern const char TEXT_SUBQ[];
using common::TEXT_SUBX;
using common::TEXT_SVC;
extern const char TEXT_SVS[];
using common::TEXT_SWAP;
using common::TEXT_TAS;
using common::TEXT_TRAP;
using common::TEXT_TRAPV;
using common::TEXT_TST;
using common::TEXT_UNLK;

using common::TEXT_REG_A0;
extern const char TEXT_REG_A1[];
extern const char TEXT_REG_A2[];
extern const char TEXT_REG_A3[];
extern const char TEXT_REG_A4[];
extern const char TEXT_REG_A5[];
extern const char TEXT_REG_A6[];
extern const char TEXT_REG_A7[];
using common::TEXT_REG_CCR;
extern const char TEXT_REG_D0[];
extern const char TEXT_REG_D1[];
extern const char TEXT_REG_D2[];
extern const char TEXT_REG_D3[];
extern const char TEXT_REG_D4[];
extern const char TEXT_REG_D5[];
extern const char TEXT_REG_D6[];
extern const char TEXT_REG_D7[];
using common::TEXT_REG_PC;
using common::TEXT_REG_SR;
using common::TEXT_REG_USP;

// MC68010
extern const char TEXT_BKPT[];
extern const char TEXT_MOVEC[];
extern const char TEXT_MOVES[];
using common::TEXT_RTD;

extern const char TEXT_REG_DFC[];
extern const char TEXT_REG_SFC[];
using common::TEXT_REG_VBR;

// MC68020
extern const char TEXT_BFCHG[];
extern const char TEXT_BFCLR[];
extern const char TEXT_BFEXTS[];
extern const char TEXT_BFEXTU[];
extern const char TEXT_BFFFO[];
extern const char TEXT_BFINS[];
extern const char TEXT_BFSET[];
extern const char TEXT_BFTST[];
extern const char TEXT_CALLM[];
using common::TEXT_CAS;
extern const char TEXT_CAS2[];
extern const char TEXT_CHK2[];
extern const char TEXT_CMP2[];
extern const char TEXT_DIVSL[];
extern const char TEXT_DIVUL[];
extern const char TEXT_PACK[];
extern const char TEXT_RTM[];
extern const char TEXT_TRAPCC[];
extern const char TEXT_TRAPCS[];
using common::TEXT_TRAPEQ;
extern const char TEXT_TRAPF[];
using common::TEXT_TRAPGE;
using common::TEXT_TRAPGT;
using common::TEXT_TRAPHI;
using common::TEXT_TRAPHS;
using common::TEXT_TRAPLE;
using common::TEXT_TRAPLO;
using common::TEXT_TRAPLS;
using common::TEXT_TRAPLT;
extern const char TEXT_TRAPMI[];
using common::TEXT_TRAPNE;
extern const char TEXT_TRAPPL[];
extern const char TEXT_TRAPT[];
extern const char TEXT_TRAPVC[];
extern const char TEXT_TRAPVS[];
extern const char TEXT_UNPK[];

extern const char TEXT_REG_CAAR[];
extern const char TEXT_REG_CACR[];
extern const char TEXT_REG_ISP[];
extern const char TEXT_REG_MSP[];

// MC68030
extern const char TEXT_PMOVEFD[];

extern const char TEXT_REG_TT0[];
extern const char TEXT_REG_TT1[];

// MC68EC030
using common::TEXT_REG_AC0;
using common::TEXT_REG_AC1;
extern const char TEXT_REG_ACUSR[];

// MC68040
extern const char TEXT_CINVA[];
extern const char TEXT_CINVL[];
extern const char TEXT_CINVP[];
extern const char TEXT_CPUSHA[];
extern const char TEXT_CPUSHL[];
extern const char TEXT_CPUSHP[];
extern const char TEXT_FDABS[];
extern const char TEXT_FDADD[];
extern const char TEXT_FDDIV[];
extern const char TEXT_FDMOVE[];
extern const char TEXT_FDMUL[];
extern const char TEXT_FDNEG[];
extern const char TEXT_FDSQRT[];
extern const char TEXT_FDSUB[];
extern const char TEXT_FSABS[];
extern const char TEXT_FSADD[];
extern const char TEXT_FSDIV[];
extern const char TEXT_FSMOVE[];
extern const char TEXT_FSMUL[];
extern const char TEXT_FSNEG[];
extern const char TEXT_FSSQRT[];
extern const char TEXT_FSSUB[];
extern const char TEXT_MOVE16[];
extern const char TEXT_PFLUSHAN[];
extern const char TEXT_PFLUSHN[];

using common::TEXT_BC;
using common::TEXT_DC;
extern const char TEXT_IC[];
extern const char TEXT_NC[];

extern const char TEXT_REG_DTT0[];
extern const char TEXT_REG_DTT1[];
extern const char TEXT_REG_ITT0[];
extern const char TEXT_REG_ITT1[];
extern const char TEXT_REG_MMUSR[];
extern const char TEXT_REG_URP[];

// MC68EC040
extern const char TEXT_REG_DACR0[];
extern const char TEXT_REG_DACR1[];
extern const char TEXT_REG_IACR0[];
extern const char TEXT_REG_IACR1[];

// MC68881
using common::TEXT_FABS;
extern const char TEXT_FACOS[];
using common::TEXT_FADD;
extern const char TEXT_FASIN[];
extern const char TEXT_FATAN[];
extern const char TEXT_FATANH[];
extern const char TEXT_FBEQ[];
extern const char TEXT_FBF[];
extern const char TEXT_FBGE[];
extern const char TEXT_FBGL[];
extern const char TEXT_FBGLE[];
extern const char TEXT_FBGT[];
extern const char TEXT_FBLE[];
extern const char TEXT_FBLT[];
extern const char TEXT_FBNE[];
extern const char TEXT_FBNGE[];
extern const char TEXT_FBNGL[];
extern const char TEXT_FBNGLE[];
extern const char TEXT_FBNGT[];
extern const char TEXT_FBNLE[];
extern const char TEXT_FBNLT[];
extern const char TEXT_FBOGE[];
extern const char TEXT_FBOGL[];
extern const char TEXT_FBOGT[];
extern const char TEXT_FBOLE[];
extern const char TEXT_FBOLT[];
extern const char TEXT_FBOR[];
extern const char TEXT_FBSEQ[];
extern const char TEXT_FBSF[];
extern const char TEXT_FBSNE[];
extern const char TEXT_FBST[];
extern const char TEXT_FBT[];
extern const char TEXT_FBUEQ[];
extern const char TEXT_FBUGE[];
extern const char TEXT_FBUGT[];
extern const char TEXT_FBULE[];
extern const char TEXT_FBULT[];
extern const char TEXT_FBUN[];
extern const char TEXT_FCMP[];
using common::TEXT_FCOS;
extern const char TEXT_FCOSH[];
extern const char TEXT_FDBEQ[];
extern const char TEXT_FDBF[];
extern const char TEXT_FDBGE[];
extern const char TEXT_FDBGL[];
extern const char TEXT_FDBGLE[];
extern const char TEXT_FDBGT[];
extern const char TEXT_FDBLE[];
extern const char TEXT_FDBLT[];
extern const char TEXT_FDBNE[];
extern const char TEXT_FDBNGE[];
extern const char TEXT_FDBNGL[];
extern const char TEXT_FDBNGLE[];
extern const char TEXT_FDBNGT[];
extern const char TEXT_FDBNLE[];
extern const char TEXT_FDBNLT[];
extern const char TEXT_FDBOGE[];
extern const char TEXT_FDBOGL[];
extern const char TEXT_FDBOGT[];
extern const char TEXT_FDBOLE[];
extern const char TEXT_FDBOLT[];
extern const char TEXT_FDBOR[];
extern const char TEXT_FDBSEQ[];
extern const char TEXT_FDBSF[];
extern const char TEXT_FDBSNE[];
extern const char TEXT_FDBST[];
extern const char TEXT_FDBT[];
extern const char TEXT_FDBUEQ[];
extern const char TEXT_FDBUGE[];
extern const char TEXT_FDBUGT[];
extern const char TEXT_FDBULE[];
extern const char TEXT_FDBULT[];
extern const char TEXT_FDBUN[];
using common::TEXT_FDIV;
extern const char TEXT_FETOX[];
extern const char TEXT_FETOXM1[];
extern const char TEXT_FGETEXP[];
extern const char TEXT_FGETMAN[];
extern const char TEXT_FINT[];
extern const char TEXT_FINTRZ[];
extern const char TEXT_FLOG10[];
extern const char TEXT_FLOG2[];
extern const char TEXT_FLOGN[];
extern const char TEXT_FLOGNP1[];
extern const char TEXT_FMOD[];
extern const char TEXT_FMOVE[];
extern const char TEXT_FMOVECR[];
extern const char TEXT_FMOVEM[];
using common::TEXT_FMUL;
using common::TEXT_FNEG;
using common::TEXT_FNOP;
extern const char TEXT_FREM[];
extern const char TEXT_FRESTORE[];
using common::TEXT_FSAVE;
using common::TEXT_FSCALE;
extern const char TEXT_FSEQ[];
extern const char TEXT_FSF[];
extern const char TEXT_FSGE[];
extern const char TEXT_FSGL[];
extern const char TEXT_FSGLDIV[];
extern const char TEXT_FSGLE[];
extern const char TEXT_FSGLMUL[];
extern const char TEXT_FSGT[];
using common::TEXT_FSIN;
using common::TEXT_FSINCOS;
extern const char TEXT_FSINH[];
extern const char TEXT_FSLE[];
extern const char TEXT_FSLT[];
extern const char TEXT_FSNE[];
extern const char TEXT_FSNGE[];
extern const char TEXT_FSNGL[];
extern const char TEXT_FSNGLE[];
extern const char TEXT_FSNGT[];
extern const char TEXT_FSNLE[];
extern const char TEXT_FSNLT[];
extern const char TEXT_FSOGE[];
extern const char TEXT_FSOGL[];
extern const char TEXT_FSOGT[];
extern const char TEXT_FSOLE[];
extern const char TEXT_FSOLT[];
extern const char TEXT_FSOR[];
using common::TEXT_FSQRT;
extern const char TEXT_FSSEQ[];
extern const char TEXT_FSSF[];
extern const char TEXT_FSSNE[];
extern const char TEXT_FSST[];
using common::TEXT_FST;
using common::TEXT_FSUB;
extern const char TEXT_FSUEQ[];
extern const char TEXT_FSUGE[];
extern const char TEXT_FSUGT[];
extern const char TEXT_FSULE[];
extern const char TEXT_FSULT[];
extern const char TEXT_FSUN[];
extern const char TEXT_FTAN[];
extern const char TEXT_FTANH[];
extern const char TEXT_FTENTOX[];
extern const char TEXT_FTRAPEQ[];
extern const char TEXT_FTRAPF[];
extern const char TEXT_FTRAPGE[];
extern const char TEXT_FTRAPGL[];
extern const char TEXT_FTRAPGLE[];
extern const char TEXT_FTRAPGT[];
extern const char TEXT_FTRAPLE[];
extern const char TEXT_FTRAPLT[];
extern const char TEXT_FTRAPNE[];
extern const char TEXT_FTRAPNGE[];
extern const char TEXT_FTRAPNGL[];
extern const char TEXT_FTRAPNGLE[];
extern const char TEXT_FTRAPNGT[];
extern const char TEXT_FTRAPNLE[];
extern const char TEXT_FTRAPNLT[];
extern const char TEXT_FTRAPOGE[];
extern const char TEXT_FTRAPOGL[];
extern const char TEXT_FTRAPOGT[];
extern const char TEXT_FTRAPOLE[];
extern const char TEXT_FTRAPOLT[];
extern const char TEXT_FTRAPOR[];
extern const char TEXT_FTRAPSEQ[];
extern const char TEXT_FTRAPSF[];
extern const char TEXT_FTRAPSNE[];
extern const char TEXT_FTRAPST[];
extern const char TEXT_FTRAPT[];
extern const char TEXT_FTRAPUEQ[];
extern const char TEXT_FTRAPUGE[];
extern const char TEXT_FTRAPUGT[];
extern const char TEXT_FTRAPULE[];
extern const char TEXT_FTRAPULT[];
extern const char TEXT_FTRAPUN[];
using common::TEXT_FTST;
extern const char TEXT_FTWOTOX[];
extern const char TEXT_REG_FP0[];
extern const char TEXT_REG_FP1[];
extern const char TEXT_REG_FP2[];
extern const char TEXT_REG_FP3[];
extern const char TEXT_REG_FP4[];
extern const char TEXT_REG_FP5[];
extern const char TEXT_REG_FP6[];
extern const char TEXT_REG_FP7[];
extern const char TEXT_REG_FPCR[];
extern const char TEXT_REG_FPIAR[];
extern const char TEXT_REG_FPSR[];

// MC68851
extern const char TEXT_PBAC[];
extern const char TEXT_PBAS[];
extern const char TEXT_PBBC[];
extern const char TEXT_PBBS[];
extern const char TEXT_PBCC[];
extern const char TEXT_PBCS[];
extern const char TEXT_PBGC[];
extern const char TEXT_PBGS[];
extern const char TEXT_PBIC[];
extern const char TEXT_PBIS[];
extern const char TEXT_PBLC[];
extern const char TEXT_PBLS[];
extern const char TEXT_PBSC[];
extern const char TEXT_PBSS[];
extern const char TEXT_PBWC[];
extern const char TEXT_PBWS[];
extern const char TEXT_PDBAC[];
extern const char TEXT_PDBAS[];
extern const char TEXT_PDBBC[];
extern const char TEXT_PDBBS[];
extern const char TEXT_PDBCC[];
extern const char TEXT_PDBCS[];
extern const char TEXT_PDBGC[];
extern const char TEXT_PDBGS[];
extern const char TEXT_PDBIC[];
extern const char TEXT_PDBIS[];
extern const char TEXT_PDBLC[];
extern const char TEXT_PDBLS[];
extern const char TEXT_PDBSC[];
extern const char TEXT_PDBSS[];
extern const char TEXT_PDBWC[];
extern const char TEXT_PDBWS[];
extern const char TEXT_PFLUSH[];
extern const char TEXT_PFLUSHA[];
extern const char TEXT_PFLUSHR[];
extern const char TEXT_PFLUSHS[];
extern const char TEXT_PLOADR[];
extern const char TEXT_PLOADW[];
extern const char TEXT_PMOVE[];
extern const char TEXT_PRESTORE[];
extern const char TEXT_PSAC[];
extern const char TEXT_PSAS[];
extern const char TEXT_PSAVE[];
extern const char TEXT_PSBC[];
extern const char TEXT_PSBS[];
extern const char TEXT_PSCC[];
extern const char TEXT_PSCS[];
extern const char TEXT_PSGC[];
extern const char TEXT_PSGS[];
extern const char TEXT_PSIC[];
extern const char TEXT_PSIS[];
extern const char TEXT_PSLC[];
extern const char TEXT_PSLS[];
extern const char TEXT_PSSC[];
extern const char TEXT_PSSS[];
extern const char TEXT_PSWC[];
extern const char TEXT_PSWS[];
extern const char TEXT_PTESTR[];
extern const char TEXT_PTESTW[];
extern const char TEXT_PTRAPAC[];
extern const char TEXT_PTRAPAS[];
extern const char TEXT_PTRAPBC[];
extern const char TEXT_PTRAPBS[];
extern const char TEXT_PTRAPCC[];
extern const char TEXT_PTRAPCS[];
extern const char TEXT_PTRAPGC[];
extern const char TEXT_PTRAPGS[];
extern const char TEXT_PTRAPIC[];
extern const char TEXT_PTRAPIS[];
extern const char TEXT_PTRAPLC[];
extern const char TEXT_PTRAPLS[];
extern const char TEXT_PTRAPSC[];
extern const char TEXT_PTRAPSS[];
extern const char TEXT_PTRAPWC[];
extern const char TEXT_PTRAPWS[];
extern const char TEXT_PVALID[];

extern const char TEXT_REG_AC[];
extern const char TEXT_REG_BAC[];
extern const char TEXT_REG_BAD[];
extern const char TEXT_REG_CAL[];
extern const char TEXT_REG_CRP[];
extern const char TEXT_REG_DRP[];
extern const char TEXT_REG_PCSR[];
using common::TEXT_REG_PSR;
extern const char TEXT_REG_SCC[];
extern const char TEXT_REG_SRP[];
using common::TEXT_REG_TC;
extern const char TEXT_REG_VAL[];

// clang-format on
}  // namespace mc68000
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_MC68000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
