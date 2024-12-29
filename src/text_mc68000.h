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
extern const char TEXT_CPU_MC68000[] PROGMEM;
extern const char TEXT_CPU_MC68010[] PROGMEM;
extern const char TEXT_FPU_MC68881[] PROGMEM;
extern const char TEXT_CPU_68000[]   PROGMEM;
extern const char TEXT_CPU_68010[]   PROGMEM;
extern const char TEXT_FPU_68881[]   PROGMEM;
using common::TEXT_none;

using common::TEXT_FPU;

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
extern const char TEXT_DBEQ[]  PROGMEM;
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
extern const char TEXT_DBNE[]  PROGMEM;
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
using common::TEXT_REG_PC;
extern const char TEXT_REG_SR[] PROGMEM;
extern const char TEXT_REG_CCR[] PROGMEM;
extern const char TEXT_REG_USP[] PROGMEM;

// MC68010
extern const char TEXT_MOVEC[] PROGMEM;
extern const char TEXT_MOVES[] PROGMEM;
extern const char TEXT_RTD[]   PROGMEM;

extern const char TEXT_REG_DFC[] PROGMEM;
extern const char TEXT_REG_SFC[] PROGMEM;
extern const char TEXT_REG_VBR[] PROGMEM;

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
extern const char TEXT_FCOS[]      PROGMEM;
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
extern const char TEXT_FSIN[]      PROGMEM;
extern const char TEXT_FSINCOS[]   PROGMEM;
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
