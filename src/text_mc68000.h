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
extern const char TEXT_MC68000_LIST[] PROGMEM;
extern const char TEXT_CPU_MC68000[]  PROGMEM;
extern const char TEXT_CPU_MC68010[]  PROGMEM;
extern const char TEXT_CPU_MC68020[]  PROGMEM;
extern const char TEXT_CPU_MC68030[]  PROGMEM;
extern const char TEXT_CPU_MC68040[]  PROGMEM;
extern const char TEXT_FPU_MC68881[]  PROGMEM;
extern const char TEXT_PMMU_MC68851[] PROGMEM;
extern const char TEXT_CPU_68000[]    PROGMEM;
extern const char TEXT_CPU_68010[]    PROGMEM;
extern const char TEXT_CPU_68020[]    PROGMEM;
extern const char TEXT_CPU_68030[]    PROGMEM;
extern const char TEXT_CPU_68040[]    PROGMEM;
extern const char TEXT_FPU_68881[]    PROGMEM;
extern const char TEXT_PMMU_68851[]   PROGMEM;
extern const char TEXT_PMMU_68EC030[] PROGMEM;
extern const char TEXT_PMMU_68EC040[] PROGMEM;

using common::TEXT_none;

using common::TEXT_FPU;
using common::TEXT_PMMU;

// MC68000 instructions
extern const char TEXT_ABCD[]  PROGMEM;
using common::TEXT_ADD;
using common::TEXT_ADDA;
using common::TEXT_ADDI;
extern const char TEXT_ADDQ[]  PROGMEM;
extern const char TEXT_ADDX[]  PROGMEM;
using common::TEXT_AND;
using common::TEXT_ANDI;
using common::TEXT_ASL;
using common::TEXT_ASR;
using common::TEXT_BCC;
extern const char TEXT_BCHG[]  PROGMEM;
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
extern const char TEXT_CHK[]   PROGMEM;
using common::TEXT_CLR;
using common::TEXT_CMP;
using common::TEXT_CMPA;
extern const char TEXT_CMPI[]  PROGMEM;
extern const char TEXT_CMPM[]  PROGMEM;
extern const char TEXT_DBCC[]  PROGMEM;
extern const char TEXT_DBCS[]  PROGMEM;
using common::TEXT_DBEQ;
extern const char TEXT_DBF[]   PROGMEM;
extern const char TEXT_DBGE[]  PROGMEM;
extern const char TEXT_DBGT[]  PROGMEM;
extern const char TEXT_DBHI[]  PROGMEM;
extern const char TEXT_DBHS[]  PROGMEM;
extern const char TEXT_DBLS[]  PROGMEM;
extern const char TEXT_DBLE[]  PROGMEM;
extern const char TEXT_DBLO[]  PROGMEM;
extern const char TEXT_DBLT[]  PROGMEM;
extern const char TEXT_DBMI[]  PROGMEM;
using common::TEXT_DBNE;
extern const char TEXT_DBPL[]  PROGMEM;
extern const char TEXT_DBRA[]  PROGMEM;
extern const char TEXT_DBT[]   PROGMEM;
extern const char TEXT_DBVC[]  PROGMEM;
extern const char TEXT_DBVS[]  PROGMEM;
using common::TEXT_DIVS;
using common::TEXT_DIVU;
using common::TEXT_EOR;
using common::TEXT_EORI;
using common::TEXT_EXG;
using common::TEXT_EXT;
extern const char TEXT_ILLEGAL[] PROGMEM;
using common::TEXT_JMP;
using common::TEXT_JSR;
using common::TEXT_LEA;
extern const char TEXT_LINK[]  PROGMEM;
using common::TEXT_LSL;
using common::TEXT_LSR;
extern const char TEXT_MOVE[]  PROGMEM;
extern const char TEXT_MOVEA[] PROGMEM;
extern const char TEXT_MOVEM[] PROGMEM;
extern const char TEXT_MOVEP[] PROGMEM;
extern const char TEXT_MOVEQ[] PROGMEM;
extern const char TEXT_MULS[]  PROGMEM;
using common::TEXT_MULU;
extern const char TEXT_NBCD[]  PROGMEM;
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
extern const char TEXT_ROXL[]  PROGMEM;
extern const char TEXT_ROXR[]  PROGMEM;
extern const char TEXT_RTE[]   PROGMEM;
using common::TEXT_RTR;
using common::TEXT_RTS;
using common::TEXT_SBCD;
using common::TEXT_SCC;
extern const char TEXT_SCS[]   PROGMEM;
using common::TEXT_SEQ;
extern const char TEXT_SF[]    PROGMEM;
extern const char TEXT_SGE[]   PROGMEM;
using common::TEXT_SGT;
extern const char TEXT_SHI[]   PROGMEM;
extern const char TEXT_SHS[]   PROGMEM;
extern const char TEXT_SLE[]   PROGMEM;
extern const char TEXT_SLO[]   PROGMEM;
extern const char TEXT_SLS[]   PROGMEM;
extern const char TEXT_SLT[]   PROGMEM;
using common::TEXT_SMI;
extern const char TEXT_SNE[]   PROGMEM;
using common::TEXT_SPL;
using common::TEXT_ST;
using common::TEXT_STOP;
using common::TEXT_SUB;
using common::TEXT_SUBA;
using common::TEXT_SUBI;
extern const char TEXT_SUBQ[]  PROGMEM;
extern const char TEXT_SUBX[]  PROGMEM;
using common::TEXT_SVC;
extern const char TEXT_SVS[]   PROGMEM;
using common::TEXT_SWAP;
extern const char TEXT_TAS[]   PROGMEM;
using common::TEXT_TRAP;
extern const char TEXT_TRAPV[] PROGMEM;
using common::TEXT_TST;
extern const char TEXT_UNLK[]  PROGMEM;

extern const char TEXT_REG_D0[] PROGMEM;
extern const char TEXT_REG_D1[] PROGMEM;
extern const char TEXT_REG_D2[] PROGMEM;
extern const char TEXT_REG_D3[] PROGMEM;
extern const char TEXT_REG_D4[] PROGMEM;
extern const char TEXT_REG_D5[] PROGMEM;
extern const char TEXT_REG_D6[] PROGMEM;
extern const char TEXT_REG_D7[] PROGMEM;
extern const char TEXT_REG_A0[] PROGMEM;
extern const char TEXT_REG_A1[] PROGMEM;
extern const char TEXT_REG_A2[] PROGMEM;
extern const char TEXT_REG_A3[] PROGMEM;
extern const char TEXT_REG_A4[] PROGMEM;
extern const char TEXT_REG_A5[] PROGMEM;
extern const char TEXT_REG_A6[] PROGMEM;
extern const char TEXT_REG_A7[] PROGMEM;
using common::TEXT_REG_CCR;
using common::TEXT_REG_PC;
using common::TEXT_REG_SR;
using common::TEXT_REG_USP;

// MC68010
extern const char TEXT_BKPT[]  PROGMEM;
extern const char TEXT_MOVEC[] PROGMEM;
extern const char TEXT_MOVES[] PROGMEM;
extern const char TEXT_RTD[]   PROGMEM;

extern const char TEXT_REG_DFC[] PROGMEM;
extern const char TEXT_REG_SFC[] PROGMEM;
extern const char TEXT_REG_VBR[] PROGMEM;

// MC68020
extern const char TEXT_BFCHG[]  PROGMEM;
extern const char TEXT_BFCLR[]  PROGMEM;
extern const char TEXT_BFEXTS[] PROGMEM;
extern const char TEXT_BFEXTU[] PROGMEM;
extern const char TEXT_BFFFO[]  PROGMEM;
extern const char TEXT_BFINS[]  PROGMEM;
extern const char TEXT_BFSET[]  PROGMEM;
extern const char TEXT_BFTST[]  PROGMEM;
extern const char TEXT_CALLM[]  PROGMEM;
extern const char TEXT_CAS2[]   PROGMEM;
using common::TEXT_CAS;
extern const char TEXT_CHK2[]   PROGMEM;
extern const char TEXT_CMP2[]   PROGMEM;
extern const char TEXT_DIVSL[]  PROGMEM;
extern const char TEXT_DIVUL[]  PROGMEM;
extern const char TEXT_PACK[]   PROGMEM;
extern const char TEXT_RTM[]    PROGMEM;
extern const char TEXT_TRAPCC[] PROGMEM;
extern const char TEXT_TRAPCS[] PROGMEM;
extern const char TEXT_TRAPEQ[] PROGMEM;
extern const char TEXT_TRAPF[]  PROGMEM;
extern const char TEXT_TRAPGE[] PROGMEM;
extern const char TEXT_TRAPGT[] PROGMEM;
extern const char TEXT_TRAPHI[] PROGMEM;
extern const char TEXT_TRAPHS[] PROGMEM;
extern const char TEXT_TRAPLE[] PROGMEM;
extern const char TEXT_TRAPLO[] PROGMEM;
extern const char TEXT_TRAPLS[] PROGMEM;
extern const char TEXT_TRAPLT[] PROGMEM;
extern const char TEXT_TRAPMI[] PROGMEM;
extern const char TEXT_TRAPNE[] PROGMEM;
extern const char TEXT_TRAPPL[] PROGMEM;
extern const char TEXT_TRAPT[]  PROGMEM;
extern const char TEXT_TRAPVC[] PROGMEM;
extern const char TEXT_TRAPVS[] PROGMEM;
extern const char TEXT_UNPK[]   PROGMEM;

extern const char TEXT_REG_CAAR[] PROGMEM;
extern const char TEXT_REG_CACR[] PROGMEM;
extern const char TEXT_REG_ISP[]  PROGMEM;
extern const char TEXT_REG_MSP[]  PROGMEM;

// MC68030
extern const char TEXT_PMOVEFD[] PROGMEM;

extern const char TEXT_REG_TT0[] PROGMEM;
extern const char TEXT_REG_TT1[] PROGMEM;

// MC68EC030
extern const char TEXT_REG_ACUSR[] PROGMEM;
using common::TEXT_REG_AC0;
using common::TEXT_REG_AC1;

// MC68040
extern const char TEXT_CINVA[]  PROGMEM;
extern const char TEXT_CINVL[]  PROGMEM;
extern const char TEXT_CINVP[]  PROGMEM;
extern const char TEXT_CPUSHA[] PROGMEM;
extern const char TEXT_CPUSHL[] PROGMEM;
extern const char TEXT_CPUSHP[] PROGMEM;
extern const char TEXT_FDABS[]  PROGMEM;
extern const char TEXT_FDADD[]  PROGMEM;
extern const char TEXT_FDDIV[]  PROGMEM;
extern const char TEXT_FDMOVE[] PROGMEM;
extern const char TEXT_FDMUL[]  PROGMEM;
extern const char TEXT_FDNEG[]  PROGMEM;
extern const char TEXT_FDSQRT[] PROGMEM;
extern const char TEXT_FDSUB[]  PROGMEM;
extern const char TEXT_FSABS[]  PROGMEM;
extern const char TEXT_FSADD[]  PROGMEM;
extern const char TEXT_FSDIV[]  PROGMEM;
extern const char TEXT_FSMOVE[] PROGMEM;
extern const char TEXT_FSMUL[]  PROGMEM;
extern const char TEXT_FSNEG[]  PROGMEM;
extern const char TEXT_FSSQRT[] PROGMEM;
extern const char TEXT_FSSUB[]  PROGMEM;
extern const char TEXT_MOVE16[] PROGMEM;
extern const char TEXT_PFLUSHAN[] PROGMEM;
extern const char TEXT_PFLUSHN[]  PROGMEM;

extern const char TEXT_IC[] PROGMEM;
using common::TEXT_DC;
using common::TEXT_BC;
extern const char TEXT_NC[] PROGMEM;

extern const char TEXT_REG_DTT0[]  PROGMEM;
extern const char TEXT_REG_DTT1[]  PROGMEM;
extern const char TEXT_REG_ITT0[]  PROGMEM;
extern const char TEXT_REG_ITT1[]  PROGMEM;
extern const char TEXT_REG_MMUSR[] PROGMEM;
extern const char TEXT_REG_URP[]   PROGMEM;

// MC68EC040
extern const char TEXT_REG_DACR0[] PROGMEM;
extern const char TEXT_REG_DACR1[] PROGMEM;
extern const char TEXT_REG_IACR0[] PROGMEM;
extern const char TEXT_REG_IACR1[] PROGMEM;

// MC68881
using common::TEXT_FABS;
extern const char TEXT_FACOS[]     PROGMEM;
using common::TEXT_FADD;
extern const char TEXT_FASIN[]     PROGMEM;
extern const char TEXT_FATAN[]     PROGMEM;
extern const char TEXT_FATANH[]    PROGMEM;
extern const char TEXT_FBEQ[]      PROGMEM;
extern const char TEXT_FBF[]       PROGMEM;
extern const char TEXT_FBGE[]      PROGMEM;
extern const char TEXT_FBGL[]      PROGMEM;
extern const char TEXT_FBGLE[]     PROGMEM;
extern const char TEXT_FBGT[]      PROGMEM;
extern const char TEXT_FBLE[]      PROGMEM;
extern const char TEXT_FBLT[]      PROGMEM;
extern const char TEXT_FBNE[]      PROGMEM;
extern const char TEXT_FBNGE[]     PROGMEM;
extern const char TEXT_FBNGL[]     PROGMEM;
extern const char TEXT_FBNGLE[]    PROGMEM;
extern const char TEXT_FBNGT[]     PROGMEM;
extern const char TEXT_FBNLE[]     PROGMEM;
extern const char TEXT_FBNLT[]     PROGMEM;
extern const char TEXT_FBOGE[]     PROGMEM;
extern const char TEXT_FBOGL[]     PROGMEM;
extern const char TEXT_FBOGT[]     PROGMEM;
extern const char TEXT_FBOLE[]     PROGMEM;
extern const char TEXT_FBOLT[]     PROGMEM;
extern const char TEXT_FBOR[]      PROGMEM;
extern const char TEXT_FBSEQ[]     PROGMEM;
extern const char TEXT_FBSF[]      PROGMEM;
extern const char TEXT_FBSNE[]     PROGMEM;
extern const char TEXT_FBST[]      PROGMEM;
extern const char TEXT_FBT[]       PROGMEM;
extern const char TEXT_FBUEQ[]     PROGMEM;
extern const char TEXT_FBUGE[]     PROGMEM;
extern const char TEXT_FBUGT[]     PROGMEM;
extern const char TEXT_FBULE[]     PROGMEM;
extern const char TEXT_FBULT[]     PROGMEM;
extern const char TEXT_FBUN[]      PROGMEM;
extern const char TEXT_FCMP[]      PROGMEM;
using common::TEXT_FCOS;
extern const char TEXT_FCOSH[]     PROGMEM;
extern const char TEXT_FDBEQ[]     PROGMEM;
extern const char TEXT_FDBF[]      PROGMEM;
extern const char TEXT_FDBGE[]     PROGMEM;
extern const char TEXT_FDBGL[]     PROGMEM;
extern const char TEXT_FDBGLE[]    PROGMEM;
extern const char TEXT_FDBGT[]     PROGMEM;
extern const char TEXT_FDBLE[]     PROGMEM;
extern const char TEXT_FDBLT[]     PROGMEM;
extern const char TEXT_FDBNE[]     PROGMEM;
extern const char TEXT_FDBNGE[]    PROGMEM;
extern const char TEXT_FDBNGL[]    PROGMEM;
extern const char TEXT_FDBNGLE[]   PROGMEM;
extern const char TEXT_FDBNGT[]    PROGMEM;
extern const char TEXT_FDBNLE[]    PROGMEM;
extern const char TEXT_FDBNLT[]    PROGMEM;
extern const char TEXT_FDBOGE[]    PROGMEM;
extern const char TEXT_FDBOGL[]    PROGMEM;
extern const char TEXT_FDBOGT[]    PROGMEM;
extern const char TEXT_FDBOLE[]    PROGMEM;
extern const char TEXT_FDBOLT[]    PROGMEM;
extern const char TEXT_FDBOR[]     PROGMEM;
extern const char TEXT_FDBSEQ[]    PROGMEM;
extern const char TEXT_FDBSF[]     PROGMEM;
extern const char TEXT_FDBSNE[]    PROGMEM;
extern const char TEXT_FDBST[]     PROGMEM;
extern const char TEXT_FDBT[]      PROGMEM;
extern const char TEXT_FDBUEQ[]    PROGMEM;
extern const char TEXT_FDBUGE[]    PROGMEM;
extern const char TEXT_FDBUGT[]    PROGMEM;
extern const char TEXT_FDBULE[]    PROGMEM;
extern const char TEXT_FDBULT[]    PROGMEM;
extern const char TEXT_FDBUN[]     PROGMEM;
using common::TEXT_FDIV;
extern const char TEXT_FETOX[]     PROGMEM;
extern const char TEXT_FETOXM1[]   PROGMEM;
extern const char TEXT_FGETEXP[]   PROGMEM;
extern const char TEXT_FGETMAN[]   PROGMEM;
extern const char TEXT_FINT[]      PROGMEM;
extern const char TEXT_FINTRZ[]    PROGMEM;
extern const char TEXT_FLOG10[]    PROGMEM;
extern const char TEXT_FLOG2[]     PROGMEM;
extern const char TEXT_FLOGN[]     PROGMEM;
extern const char TEXT_FLOGNP1[]   PROGMEM;
extern const char TEXT_FMOD[]      PROGMEM;
extern const char TEXT_FMOVE[]     PROGMEM;
extern const char TEXT_FMOVECR[]   PROGMEM;
extern const char TEXT_FMOVEM[]    PROGMEM;
using common::TEXT_FMUL;
extern const char TEXT_FNEG[]      PROGMEM;
using common::TEXT_FNOP;
extern const char TEXT_FREM[]      PROGMEM;
extern const char TEXT_FRESTORE[]  PROGMEM;
using common::TEXT_FSAVE;
using common::TEXT_FSCALE;
extern const char TEXT_FSEQ[]      PROGMEM;
extern const char TEXT_FSF[]       PROGMEM;
extern const char TEXT_FSGE[]      PROGMEM;
extern const char TEXT_FSGL[]      PROGMEM;
extern const char TEXT_FSGLDIV[]   PROGMEM;
extern const char TEXT_FSGLE[]     PROGMEM;
extern const char TEXT_FSGLMUL[]   PROGMEM;
extern const char TEXT_FSGT[]      PROGMEM;
using common::TEXT_FSIN;
using common::TEXT_FSINCOS;
extern const char TEXT_FSINH[]     PROGMEM;
extern const char TEXT_FSLE[]      PROGMEM;
extern const char TEXT_FSLT[]      PROGMEM;
extern const char TEXT_FSNE[]      PROGMEM;
extern const char TEXT_FSNGE[]     PROGMEM;
extern const char TEXT_FSNGL[]     PROGMEM;
extern const char TEXT_FSNGLE[]    PROGMEM;
extern const char TEXT_FSNGT[]     PROGMEM;
extern const char TEXT_FSNLE[]     PROGMEM;
extern const char TEXT_FSNLT[]     PROGMEM;
extern const char TEXT_FSOGE[]     PROGMEM;
extern const char TEXT_FSOGL[]     PROGMEM;
extern const char TEXT_FSOGT[]     PROGMEM;
extern const char TEXT_FSOLE[]     PROGMEM;
extern const char TEXT_FSOLT[]     PROGMEM;
extern const char TEXT_FSOR[]      PROGMEM;
using common::TEXT_FSQRT;
extern const char TEXT_FSSEQ[]     PROGMEM;
extern const char TEXT_FSSF[]      PROGMEM;
extern const char TEXT_FSSNE[]     PROGMEM;
extern const char TEXT_FSST[]      PROGMEM;
using common::TEXT_FST;
using common::TEXT_FSUB;
extern const char TEXT_FSUEQ[]     PROGMEM;
extern const char TEXT_FSUGE[]     PROGMEM;
extern const char TEXT_FSUGT[]     PROGMEM;
extern const char TEXT_FSULE[]     PROGMEM;
extern const char TEXT_FSULT[]     PROGMEM;
extern const char TEXT_FSUN[]      PROGMEM;
extern const char TEXT_FTAN[]      PROGMEM;
extern const char TEXT_FTANH[]     PROGMEM;
extern const char TEXT_FTENTOX[]   PROGMEM;
extern const char TEXT_FTRAPEQ[]   PROGMEM;
extern const char TEXT_FTRAPF[]    PROGMEM;
extern const char TEXT_FTRAPGE[]   PROGMEM;
extern const char TEXT_FTRAPGL[]   PROGMEM;
extern const char TEXT_FTRAPGLE[]  PROGMEM;
extern const char TEXT_FTRAPGT[]   PROGMEM;
extern const char TEXT_FTRAPLE[]   PROGMEM;
extern const char TEXT_FTRAPLT[]   PROGMEM;
extern const char TEXT_FTRAPNE[]   PROGMEM;
extern const char TEXT_FTRAPNGE[]  PROGMEM;
extern const char TEXT_FTRAPNGL[]  PROGMEM;
extern const char TEXT_FTRAPNGLE[] PROGMEM;
extern const char TEXT_FTRAPNGT[]  PROGMEM;
extern const char TEXT_FTRAPNLE[]  PROGMEM;
extern const char TEXT_FTRAPNLT[]  PROGMEM;
extern const char TEXT_FTRAPOGE[]  PROGMEM;
extern const char TEXT_FTRAPOGL[]  PROGMEM;
extern const char TEXT_FTRAPOGT[]  PROGMEM;
extern const char TEXT_FTRAPOLE[]  PROGMEM;
extern const char TEXT_FTRAPOLT[]  PROGMEM;
extern const char TEXT_FTRAPOR[]   PROGMEM;
extern const char TEXT_FTRAPSEQ[]  PROGMEM;
extern const char TEXT_FTRAPSF[]   PROGMEM;
extern const char TEXT_FTRAPSNE[]  PROGMEM;
extern const char TEXT_FTRAPST[]   PROGMEM;
extern const char TEXT_FTRAPT[]    PROGMEM;
extern const char TEXT_FTRAPUEQ[]  PROGMEM;
extern const char TEXT_FTRAPUGE[]  PROGMEM;
extern const char TEXT_FTRAPUGT[]  PROGMEM;
extern const char TEXT_FTRAPULE[]  PROGMEM;
extern const char TEXT_FTRAPULT[]  PROGMEM;
extern const char TEXT_FTRAPUN[]   PROGMEM;
using common::TEXT_FTST;
extern const char TEXT_FTWOTOX[]   PROGMEM;

extern const char TEXT_REG_FP0[]   PROGMEM;
extern const char TEXT_REG_FP1[]   PROGMEM;
extern const char TEXT_REG_FP2[]   PROGMEM;
extern const char TEXT_REG_FP3[]   PROGMEM;
extern const char TEXT_REG_FP4[]   PROGMEM;
extern const char TEXT_REG_FP5[]   PROGMEM;
extern const char TEXT_REG_FP6[]   PROGMEM;
extern const char TEXT_REG_FP7[]   PROGMEM;
extern const char TEXT_REG_FPCR[]  PROGMEM;
extern const char TEXT_REG_FPIAR[] PROGMEM;
extern const char TEXT_REG_FPSR[]  PROGMEM;

// MC68851
extern const char TEXT_PBAC[] PROGMEM;
extern const char TEXT_PBAS[] PROGMEM;
extern const char TEXT_PBBC[] PROGMEM;
extern const char TEXT_PBBS[] PROGMEM;
extern const char TEXT_PBCC[] PROGMEM;
extern const char TEXT_PBCS[] PROGMEM;
extern const char TEXT_PBGC[] PROGMEM;
extern const char TEXT_PBGS[] PROGMEM;
extern const char TEXT_PBIC[] PROGMEM;
extern const char TEXT_PBIS[] PROGMEM;
extern const char TEXT_PBLC[] PROGMEM;
extern const char TEXT_PBLS[] PROGMEM;
extern const char TEXT_PBSC[] PROGMEM;
extern const char TEXT_PBSS[] PROGMEM;
extern const char TEXT_PBWC[] PROGMEM;
extern const char TEXT_PBWS[] PROGMEM;
extern const char TEXT_PDBAC[] PROGMEM;
extern const char TEXT_PDBAS[] PROGMEM;
extern const char TEXT_PDBBC[] PROGMEM;
extern const char TEXT_PDBBS[] PROGMEM;
extern const char TEXT_PDBCC[] PROGMEM;
extern const char TEXT_PDBCS[] PROGMEM;
extern const char TEXT_PDBGC[] PROGMEM;
extern const char TEXT_PDBGS[] PROGMEM;
extern const char TEXT_PDBIC[] PROGMEM;
extern const char TEXT_PDBIS[] PROGMEM;
extern const char TEXT_PDBLC[] PROGMEM;
extern const char TEXT_PDBLS[] PROGMEM;
extern const char TEXT_PDBSC[] PROGMEM;
extern const char TEXT_PDBSS[] PROGMEM;
extern const char TEXT_PDBWC[] PROGMEM;
extern const char TEXT_PDBWS[] PROGMEM;
extern const char TEXT_PFLUSHA[] PROGMEM;
extern const char TEXT_PFLUSH[]  PROGMEM;
extern const char TEXT_PFLUSHR[] PROGMEM;
extern const char TEXT_PFLUSHS[] PROGMEM;
extern const char TEXT_PLOADR[] PROGMEM;
extern const char TEXT_PLOADW[] PROGMEM;
extern const char TEXT_PMOVE[]  PROGMEM;
extern const char TEXT_PRESTORE[] PROGMEM;
extern const char TEXT_PSAC[]  PROGMEM;
extern const char TEXT_PSAS[]  PROGMEM;
extern const char TEXT_PSAVE[] PROGMEM;
extern const char TEXT_PSBC[]  PROGMEM;
extern const char TEXT_PSBS[]  PROGMEM;
extern const char TEXT_PSCC[]  PROGMEM;
extern const char TEXT_PSCS[]  PROGMEM;
extern const char TEXT_PSGC[]  PROGMEM;
extern const char TEXT_PSGS[]  PROGMEM;
extern const char TEXT_PSIC[]  PROGMEM;
extern const char TEXT_PSIS[]  PROGMEM;
extern const char TEXT_PSLC[]  PROGMEM;
extern const char TEXT_PSLS[]  PROGMEM;
extern const char TEXT_PSSC[]  PROGMEM;
extern const char TEXT_PSSS[]  PROGMEM;
extern const char TEXT_PSWC[]  PROGMEM;
extern const char TEXT_PSWS[]  PROGMEM;
extern const char TEXT_PTESTR[] PROGMEM;
extern const char TEXT_PTESTW[] PROGMEM;
extern const char TEXT_PTRAPAC[] PROGMEM;
extern const char TEXT_PTRAPAS[] PROGMEM;
extern const char TEXT_PTRAPBC[] PROGMEM;
extern const char TEXT_PTRAPBS[] PROGMEM;
extern const char TEXT_PTRAPCC[] PROGMEM;
extern const char TEXT_PTRAPCS[] PROGMEM;
extern const char TEXT_PTRAPGC[] PROGMEM;
extern const char TEXT_PTRAPGS[] PROGMEM;
extern const char TEXT_PTRAPIC[] PROGMEM;
extern const char TEXT_PTRAPIS[] PROGMEM;
extern const char TEXT_PTRAPLC[] PROGMEM;
extern const char TEXT_PTRAPLS[] PROGMEM;
extern const char TEXT_PTRAPSC[] PROGMEM;
extern const char TEXT_PTRAPSS[] PROGMEM;
extern const char TEXT_PTRAPWC[] PROGMEM;
extern const char TEXT_PTRAPWS[] PROGMEM;
extern const char TEXT_PVALID[]  PROGMEM;

extern const char TEXT_REG_AC[]   PROGMEM;
extern const char TEXT_REG_BAC[]  PROGMEM;
extern const char TEXT_REG_BAD[]  PROGMEM;
extern const char TEXT_REG_CAL[]  PROGMEM;
extern const char TEXT_REG_CRP[]  PROGMEM;
extern const char TEXT_REG_DRP[]  PROGMEM;
extern const char TEXT_REG_PCSR[] PROGMEM;
using common::TEXT_REG_PSR;
extern const char TEXT_REG_SCC[]  PROGMEM;
extern const char TEXT_REG_SRP[]  PROGMEM;
using common::TEXT_REG_TC;
extern const char TEXT_REG_VAL[]  PROGMEM;

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
