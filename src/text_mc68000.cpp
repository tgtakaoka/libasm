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

#include "text_mc68000.h"

namespace libasm {
namespace text {
namespace mc68000 {
// clang-format off

constexpr char TEXT_MC68000_LIST[] = "MC68000, MC68010, MC68020, MC68030, MC68040";
constexpr char TEXT_CPU_MC68000[]  = "MC68000";
constexpr char TEXT_CPU_MC68010[]  = "MC68010";
constexpr char TEXT_CPU_MC68020[]  = "MC68020";
constexpr char TEXT_CPU_MC68030[]  = "MC68030";
constexpr char TEXT_CPU_MC68040[]  = "MC68040";
constexpr char TEXT_FPU_MC68881[]  = "MC68881";
constexpr char TEXT_PMMU_MC68851[] = "MC68851";
constexpr char TEXT_CPU_68000[]    = "68000";
constexpr char TEXT_CPU_68010[]    = "68010";
constexpr char TEXT_CPU_68020[]    = "68020";
constexpr char TEXT_CPU_68030[]    = "68030";
constexpr char TEXT_CPU_68040[]    = "68040";
constexpr char TEXT_FPU_68881[]    = "68881";
constexpr char TEXT_PMMU_68851[]   = "68851";
constexpr char TEXT_PMMU_68EC030[] = "68EC030";
constexpr char TEXT_PMMU_68EC040[] = "68EC040";

// MC68000 instructions
constexpr char TEXT_ABCD[]    = "ABCD";
// constexpr char TEXT_ADD[]   = "ADD";
// constexpr char TEXT_ADDA[]  = "ADDA";
// constexpr char TEXT_ADDI[]  = "ADDI";
constexpr char TEXT_ADDQ[]    = "ADDQ";
// constexpr char TEXT_ADDX[]    = "ADDX";
// constexpr char TEXT_AND[]   = "AND";
// constexpr char TEXT_ANDI[]  = "ANDI";
// constexpr char TEXT_ASL[]   = "ASL";
// constexpr char TEXT_ASR[]   = "ASR";
// constexpr char TEXT_BCC[]   = "BCC";
constexpr char TEXT_BCHG[]    = "BCHG";
// constexpr char TEXT_BCLR[]  = "BCLR";
// constexpr char TEXT_BCS[]   = "BCS";
// constexpr char TEXT_BEQ[]   = "BEQ";
// constexpr char TEXT_BGE[]   = "BGE";
// constexpr char TEXT_BGT[]   = "BGT";
// constexpr char TEXT_BHI[]   = "BHI";
// constexpr char TEXT_BHS[]   = "BHS";
// constexpr char TEXT_BLE[]   = "BLE";
// constexpr char TEXT_BLO[]   = "BLO";
// constexpr char TEXT_BLS[]   = "BLS";
// constexpr char TEXT_BLT[]   = "BLT";
// constexpr char TEXT_BMI[]   = "BMI";
// constexpr char TEXT_BNE[]   = "BNE";
// constexpr char TEXT_BPL[]   = "BPL";
// constexpr char TEXT_BRA[]   = "BRA";
// constexpr char TEXT_BSET[]  = "BSET";
// constexpr char TEXT_BSR[]   = "BSR";
// constexpr char TEXT_BT[]    = "BT";
// constexpr char TEXT_BTST[]  = "BTST";
// constexpr char TEXT_BVC[]   = "BVC";
// constexpr char TEXT_BVS[]   = "BVS";
constexpr char TEXT_CHK[]     = "CHK";
// constexpr char TEXT_CLR[]   = "CLR";
// constexpr char TEXT_CMP[]   = "CMP";
// constexpr char TEXT_CMPA[]  = "CMPA";
// constexpr char TEXT_CMPI[]  = "CMPI";
constexpr char TEXT_CMPM[]    = "CMPM";
constexpr char TEXT_DBCC[]    = "DBCC";
constexpr char TEXT_DBCS[]    = "DBCS";
// constexpr char TEXT_DBEQ[]  = "DBEQ";
constexpr char TEXT_DBF[]     = "DBF";
// constexpr char TEXT_DBGE[]  = "DBGE";
// constexpr char TEXT_DBGT[]  = "DBGT";
// constexpr char TEXT_DBHI[]  = "DBHI";
// constexpr char TEXT_DBHS[]  = "DBHS";
// constexpr char TEXT_DBLE[]  = "DBLE";
// constexpr char TEXT_DBLO[]  = "DBLO";
// constexpr char TEXT_DBLS[]  = "DBLS";
// constexpr char TEXT_DBLT[]  = "DBLT";
constexpr char TEXT_DBMI[]    = "DBMI";
// constexpr char TEXT_DBNE[]  = "DBNE";
constexpr char TEXT_DBPL[]    = "DBPL";
constexpr char TEXT_DBRA[]    = "DBRA";
constexpr char TEXT_DBT[]     = "DBT";
constexpr char TEXT_DBVC[]    = "DBVC";
constexpr char TEXT_DBVS[]    = "DBVS";
// constexpr char TEXT_DIVS[]  = "DIVS";
// constexpr char TEXT_DIVU[]  = "DIVU";
// constexpr char TEXT_EOR[]   = "EOR";
// constexpr char TEXT_EORI[]  = "EORI";
// constexpr char TEXT_EXG[]   = "EXG";
// constexpr char TEXT_EXT[]   = "EXT";
constexpr char TEXT_ILLEGAL[] = "ILLEGAL";
// constexpr char TEXT_JMP[]   = "JMP";
// constexpr char TEXT_JSR[]   = "JSR";
// constexpr char TEXT_LEA[]   = "LEA";
// constexpr char TEXT_LINK[]    = "LINK";
// constexpr char TEXT_LSL[]   = "LSL";
// constexpr char TEXT_LSR[]   = "LSR";
constexpr char TEXT_MOVE[]    = "MOVE";
constexpr char TEXT_MOVEA[]   = "MOVEA";
constexpr char TEXT_MOVEM[]   = "MOVEM";
constexpr char TEXT_MOVEP[]   = "MOVEP";
constexpr char TEXT_MOVEQ[]   = "MOVEQ";
// constexpr char TEXT_MULS[]    = "MULS";
// constexpr char TEXT_MULU[]  = "MULU";
constexpr char TEXT_NBCD[]    = "NBCD";
// constexpr char TEXT_NEG[]   = "NEG";
// constexpr char TEXT_NEGX[]  = "NEGX";
// constexpr char TEXT_NOP[]   = "NOP";
// constexpr char TEXT_NOT[]   = "NOT";
// constexpr char TEXT_OR[]    = "OR";
// constexpr char TEXT_ORI[]   = "ORI";
// constexpr char TEXT_PEA[]   = "PEA";
// constexpr char TEXT_RESET[] = "RESET";
// constexpr char TEXT_ROL[]   = "ROL";
// constexpr char TEXT_ROR[]   = "ROR";
constexpr char TEXT_ROXL[]    = "ROXL";
constexpr char TEXT_ROXR[]    = "ROXR";
// constexpr char TEXT_RTE[]     = "RTE";
// constexpr char TEXT_RTR[]   = "RTR";
// constexpr char TEXT_RTS[]   = "RTS";
// constexpr char TEXT_SBCD[]  = "SBCD";
// constexpr char TEXT_SCC[]   = "SCC";
constexpr char TEXT_SCS[]     = "SCS";
// constexpr char TEXT_SEQ[]   = "SEQ";
constexpr char TEXT_SF[]      = "SF";
constexpr char TEXT_SGE[]     = "SGE";
// constexpr char TEXT_SGT[]   = "SGT";
constexpr char TEXT_SHI[]     = "SHI";
constexpr char TEXT_SHS[]     = "SHS";
constexpr char TEXT_SLE[]     = "SLE";
constexpr char TEXT_SLO[]     = "SLO";
constexpr char TEXT_SLS[]     = "SLS";
constexpr char TEXT_SLT[]     = "SLT";
// constexpr char TEXT_SMI[]   = "SMI";
constexpr char TEXT_SNE[]     = "SNE";
// constexpr char TEXT_SPL[]   = "SPL";
// constexpr char TEXT_ST[]    = "ST";
// constexpr char TEXT_STOP[]  = "STOP";
// constexpr char TEXT_SUB[]   = "SUB";
// constexpr char TEXT_SUBA[]  = "SUBA";
// constexpr char TEXT_SUBI[]  = "SUBI";
constexpr char TEXT_SUBQ[]    = "SUBQ";
// constexpr char TEXT_SUBX[]    = "SUBX";
// constexpr char TEXT_SVC[]   = "SVC";
constexpr char TEXT_SVS[]     = "SVS";
// constexpr char TEXT_SWAP[]  = "SWAP";
// constexpr char TEXT_TAS[]     = "TAS";
// constexpr char TEXT_TRAP[]  = "TRAP";
// constexpr char TEXT_TRAPV[] = "TRAPV";
// constexpr char TEXT_TST[]   = "TST";
// constexpr char TEXT_UNLK[]    = "UNLK";

// constexpr char TEXT_REG_A0[] = "A0";
constexpr char TEXT_REG_A1[] = "A1";
constexpr char TEXT_REG_A2[] = "A2";
constexpr char TEXT_REG_A3[] = "A3";
constexpr char TEXT_REG_A4[] = "A4";
constexpr char TEXT_REG_A5[] = "A5";
constexpr char TEXT_REG_A6[] = "A6";
constexpr char TEXT_REG_A7[] = "A7";
// constexpr char TEXT_REG_CCR[] = "CCR";
constexpr char TEXT_REG_D0[] = "D0";
constexpr char TEXT_REG_D1[] = "D1";
constexpr char TEXT_REG_D2[] = "D2";
constexpr char TEXT_REG_D3[] = "D3";
constexpr char TEXT_REG_D4[] = "D4";
constexpr char TEXT_REG_D5[] = "D5";
constexpr char TEXT_REG_D6[] = "D6";
constexpr char TEXT_REG_D7[] = "D7";
// constexpr char TEXT_REG_PC[]  = "PC";
// constexpr char TEXT_REG_SR[]  = "SR";
// constexpr char TEXT_REG_USP[] = "USP";

// MC68010
constexpr char TEXT_BKPT[]  = "BKPT";
constexpr char TEXT_MOVEC[] = "MOVEC";
constexpr char TEXT_MOVES[] = "MOVES";
// constexpr char TEXT_RTD[]   = "RTD";

constexpr char TEXT_REG_DFC[] = "DFC";
constexpr char TEXT_REG_SFC[] = "SFC";
// constexpr char TEXT_REG_VBR[] = "VBR";

// MC68020
constexpr char TEXT_BFCHG[]  = "BFCHG";
constexpr char TEXT_BFCLR[]  = "BFCLR";
constexpr char TEXT_BFEXTS[] = "BFEXTS";
constexpr char TEXT_BFEXTU[] = "BFEXTU";
constexpr char TEXT_BFFFO[]  = "BFFFO";
constexpr char TEXT_BFINS[]  = "BFINS";
constexpr char TEXT_BFSET[]  = "BFSET";
constexpr char TEXT_BFTST[]  = "BFTST";
constexpr char TEXT_CALLM[]  = "CALLM";
// constexpr char TEXT_CAS[] = "CAS";
constexpr char TEXT_CAS2[]   = "CAS2";
constexpr char TEXT_CHK2[]   = "CHK2";
constexpr char TEXT_CMP2[]   = "CMP2";
constexpr char TEXT_DIVSL[]  = "DIVSL";
constexpr char TEXT_DIVUL[]  = "DIVUL";
constexpr char TEXT_PACK[]   = "PACK";
constexpr char TEXT_RTM[]    = "RTM";
constexpr char TEXT_TRAPCC[] = "TRAPCC";
constexpr char TEXT_TRAPCS[] = "TRAPCS";
// constexpr char TEXT_TRAPEQ[] = "TRAPEQ";
constexpr char TEXT_TRAPF[]  = "TRAPF";
// constexpr char TEXT_TRAPGE[] = "TRAPGE";
// constexpr char TEXT_TRAPGT[] = "TRAPGT";
// constexpr char TEXT_TRAPHI[] = "TRAPHI";
// constexpr char TEXT_TRAPHS[] = "TRAPHS";
// constexpr char TEXT_TRAPLE[] = "TRAPLE";
// constexpr char TEXT_TRAPLO[] = "TRAPLO";
// constexpr char TEXT_TRAPLS[] = "TRAPLS";
// constexpr char TEXT_TRAPLT[] = "TRAPLT";
constexpr char TEXT_TRAPMI[] = "TRAPMI";
// constexpr char TEXT_TRAPNE[] = "TRAPNE";
constexpr char TEXT_TRAPPL[] = "TRAPPL";
constexpr char TEXT_TRAPT[]  = "TRAPT";
constexpr char TEXT_TRAPVC[] = "TRAPVC";
constexpr char TEXT_TRAPVS[] = "TRAPVS";
constexpr char TEXT_UNPK[]   = "UNPK";

constexpr char TEXT_REG_CAAR[] = "CAAR";
constexpr char TEXT_REG_CACR[] = "CACR";
constexpr char TEXT_REG_ISP[]  = "ISP";
constexpr char TEXT_REG_MSP[]  = "MSP";

// MC68030
constexpr char TEXT_PMOVEFD[] = "PMOVEFD";

constexpr char TEXT_REG_TT0[] = "TT0";
constexpr char TEXT_REG_TT1[] = "TT1";

// MC68EC030
// constexpr char TEXT_REG_AC0[] = "AC0";
// constexpr char TEXT_REG_AC1[] = "AC1";
constexpr char TEXT_REG_ACUSR[] = "ACUSR";

// MC68040
constexpr char TEXT_CINVA[]    = "CINVA";
constexpr char TEXT_CINVL[]    = "CINVL";
constexpr char TEXT_CINVP[]    = "CINVP";
constexpr char TEXT_CPUSHA[]   = "CPUSHA";
constexpr char TEXT_CPUSHL[]   = "CPUSHL";
constexpr char TEXT_CPUSHP[]   = "CPUSHP";
constexpr char TEXT_FDABS[]    = "FDABS";
constexpr char TEXT_FDADD[]    = "FDADD";
constexpr char TEXT_FDDIV[]    = "FDDIV";
constexpr char TEXT_FDMOVE[]   = "FDMOVE";
constexpr char TEXT_FDMUL[]    = "FDMUL";
constexpr char TEXT_FDNEG[]    = "FDNEG";
constexpr char TEXT_FDSQRT[]   = "FDSQRT";
constexpr char TEXT_FDSUB[]    = "FDSUB";
constexpr char TEXT_FSABS[]    = "FSABS";
constexpr char TEXT_FSADD[]    = "FSADD";
constexpr char TEXT_FSDIV[]    = "FSDIV";
constexpr char TEXT_FSMOVE[]   = "FSMOVE";
constexpr char TEXT_FSMUL[]    = "FSMUL";
constexpr char TEXT_FSNEG[]    = "FSNEG";
constexpr char TEXT_FSSQRT[]   = "FSSQRT";
constexpr char TEXT_FSSUB[]    = "FSSUB";
constexpr char TEXT_MOVE16[]   = "MOVE16";
constexpr char TEXT_PFLUSHAN[] = "PFLUSHAN";
constexpr char TEXT_PFLUSHN[]  = "PFLUSHN";

// constexpr char TEXT_BC[] = "BC";
// constexpr char TEXT_DC[] = "DC";
constexpr char TEXT_IC[] = "IC";
constexpr char TEXT_NC[] = "NC";

constexpr char TEXT_REG_DTT0[]  = "DTT0";
constexpr char TEXT_REG_DTT1[]  = "DTT1";
constexpr char TEXT_REG_ITT0[]  = "ITT0";
constexpr char TEXT_REG_ITT1[]  = "ITT1";
constexpr char TEXT_REG_MMUSR[] = "MMUSR";
constexpr char TEXT_REG_URP[]   = "URP";

// MC68EC040
constexpr char TEXT_REG_DACR0[] = "DACR0";
constexpr char TEXT_REG_DACR1[] = "DACR1";
constexpr char TEXT_REG_IACR0[] = "IACR0";
constexpr char TEXT_REG_IACR1[] = "IACR1";

// MC68881
// constexpr char TEXT_FABS[]    = "FABS";
constexpr char TEXT_FACOS[]     = "FACOS";
// constexpr char TEXT_FADD[]    = "FADD";
constexpr char TEXT_FASIN[]     = "FASIN";
constexpr char TEXT_FATAN[]     = "FATAN";
constexpr char TEXT_FATANH[]    = "FATANH";
constexpr char TEXT_FBEQ[]      = "FBEQ";
constexpr char TEXT_FBF[]       = "FBF";
constexpr char TEXT_FBGE[]      = "FBGE";
constexpr char TEXT_FBGL[]      = "FBGL";
constexpr char TEXT_FBGLE[]     = "FBGLE";
constexpr char TEXT_FBGT[]      = "FBGT";
constexpr char TEXT_FBLE[]      = "FBLE";
constexpr char TEXT_FBLT[]      = "FBLT";
constexpr char TEXT_FBNE[]      = "FBNE";
constexpr char TEXT_FBNGE[]     = "FBNGE";
constexpr char TEXT_FBNGL[]     = "FBNGL";
constexpr char TEXT_FBNGLE[]    = "FBNGLE";
constexpr char TEXT_FBNGT[]     = "FBNGT";
constexpr char TEXT_FBNLE[]     = "FBNLE";
constexpr char TEXT_FBNLT[]     = "FBNLT";
constexpr char TEXT_FBOGE[]     = "FBOGE";
constexpr char TEXT_FBOGL[]     = "FBOGL";
constexpr char TEXT_FBOGT[]     = "FBOGT";
constexpr char TEXT_FBOLE[]     = "FBOLE";
constexpr char TEXT_FBOLT[]     = "FBOLT";
constexpr char TEXT_FBOR[]      = "FBOR";
constexpr char TEXT_FBSEQ[]     = "FBSEQ";
constexpr char TEXT_FBSF[]      = "FBSF";
constexpr char TEXT_FBSNE[]     = "FBSNE";
constexpr char TEXT_FBST[]      = "FBST";
constexpr char TEXT_FBT[]       = "FBT";
constexpr char TEXT_FBUEQ[]     = "FBUEQ";
constexpr char TEXT_FBUGE[]     = "FBUGE";
constexpr char TEXT_FBUGT[]     = "FBUGT";
constexpr char TEXT_FBULE[]     = "FBULE";
constexpr char TEXT_FBULT[]     = "FBULT";
constexpr char TEXT_FBUN[]      = "FBUN";
constexpr char TEXT_FCMP[]      = "FCMP";
// constexpr char TEXT_FCOS[]    = "FCOS";
constexpr char TEXT_FCOSH[]     = "FCOSH";
constexpr char TEXT_FDBEQ[]     = "FDBEQ";
constexpr char TEXT_FDBF[]      = "FDBF";
constexpr char TEXT_FDBGE[]     = "FDBGE";
constexpr char TEXT_FDBGL[]     = "FDBGL";
constexpr char TEXT_FDBGLE[]    = "FDBGLE";
constexpr char TEXT_FDBGT[]     = "FDBGT";
constexpr char TEXT_FDBLE[]     = "FDBLE";
constexpr char TEXT_FDBLT[]     = "FDBLT";
constexpr char TEXT_FDBNE[]     = "FDBNE";
constexpr char TEXT_FDBNGE[]    = "FDBNGE";
constexpr char TEXT_FDBNGL[]    = "FDBNGL";
constexpr char TEXT_FDBNGLE[]   = "FDBNGLE";
constexpr char TEXT_FDBNGT[]    = "FDBNGT";
constexpr char TEXT_FDBNLE[]    = "FDBNLE";
constexpr char TEXT_FDBNLT[]    = "FDBNLT";
constexpr char TEXT_FDBOGE[]    = "FDBOGE";
constexpr char TEXT_FDBOGL[]    = "FDBOGL";
constexpr char TEXT_FDBOGT[]    = "FDBOGT";
constexpr char TEXT_FDBOLE[]    = "FDBOLE";
constexpr char TEXT_FDBOLT[]    = "FDBOLT";
constexpr char TEXT_FDBOR[]     = "FDBOR";
constexpr char TEXT_FDBSEQ[]    = "FDBSEQ";
constexpr char TEXT_FDBSF[]     = "FDBSF";
constexpr char TEXT_FDBSNE[]    = "FDBSNE";
constexpr char TEXT_FDBST[]     = "FDBST";
constexpr char TEXT_FDBT[]      = "FDBT";
constexpr char TEXT_FDBUEQ[]    = "FDBUEQ";
constexpr char TEXT_FDBUGE[]    = "FDBUGE";
constexpr char TEXT_FDBUGT[]    = "FDBUGT";
constexpr char TEXT_FDBULE[]    = "FDBULE";
constexpr char TEXT_FDBULT[]    = "FDBULT";
constexpr char TEXT_FDBUN[]     = "FDBUN";
// constexpr char TEXT_FDIV[]    = "FDIV";
constexpr char TEXT_FETOX[]     = "FETOX";
constexpr char TEXT_FETOXM1[]   = "FETOXM1";
constexpr char TEXT_FGETEXP[]   = "FGETEXP";
constexpr char TEXT_FGETMAN[]   = "FGETMAN";
constexpr char TEXT_FINT[]      = "FINT";
constexpr char TEXT_FINTRZ[]    = "FINTRZ";
constexpr char TEXT_FLOG10[]    = "FLOG10";
constexpr char TEXT_FLOG2[]     = "FLOG2";
constexpr char TEXT_FLOGN[]     = "FLOGN";
constexpr char TEXT_FLOGNP1[]   = "FLOGNP1";
constexpr char TEXT_FMOD[]      = "FMOD";
constexpr char TEXT_FMOVE[]     = "FMOVE";
constexpr char TEXT_FMOVECR[]   = "FMOVECR";
constexpr char TEXT_FMOVEM[]    = "FMOVEM";
// constexpr char TEXT_FMUL[]    = "FMUL";
// constexpr char TEXT_FNEG[]      = "FNEG";
// constexpr char TEXT_FNOP[]    = "FNOP";
constexpr char TEXT_FREM[]      = "FREM";
constexpr char TEXT_FRESTORE[]  = "FRESTORE";
// constexpr char TEXT_FSAVE[]   = "FSAVE";
// constexpr char TEXT_FSCALE[]  = "FSCALE";
constexpr char TEXT_FSEQ[]      = "FSEQ";
constexpr char TEXT_FSF[]       = "FSF";
constexpr char TEXT_FSGE[]      = "FSGE";
constexpr char TEXT_FSGL[]      = "FSGL";
constexpr char TEXT_FSGLDIV[]   = "FSGLDIV";
constexpr char TEXT_FSGLE[]     = "FSGLE";
constexpr char TEXT_FSGLMUL[]   = "FSGLMUL";
constexpr char TEXT_FSGT[]      = "FSGT";
// constexpr char TEXT_FSIN[]    = "FSIN";
// constexpr char TEXT_FSINCOS[] = "FSINCOS";
constexpr char TEXT_FSINH[]     = "FSINH";
constexpr char TEXT_FSLE[]      = "FSLE";
constexpr char TEXT_FSLT[]      = "FSLT";
constexpr char TEXT_FSNE[]      = "FSNE";
constexpr char TEXT_FSNGE[]     = "FSNGE";
constexpr char TEXT_FSNGL[]     = "FSNGL";
constexpr char TEXT_FSNGLE[]    = "FSNGLE";
constexpr char TEXT_FSNGT[]     = "FSNGT";
constexpr char TEXT_FSNLE[]     = "FSNLE";
constexpr char TEXT_FSNLT[]     = "FSNLT";
constexpr char TEXT_FSOGE[]     = "FSOGE";
constexpr char TEXT_FSOGL[]     = "FSOGL";
constexpr char TEXT_FSOGT[]     = "FSOGT";
constexpr char TEXT_FSOLE[]     = "FSOLE";
constexpr char TEXT_FSOLT[]     = "FSOLT";
constexpr char TEXT_FSOR[]      = "FSOR";
// constexpr char TEXT_FSQRT[]   = "FSQRT";
constexpr char TEXT_FSSEQ[]     = "FSSEQ";
constexpr char TEXT_FSSF[]      = "FSSF";
constexpr char TEXT_FSSNE[]     = "FSSNE";
constexpr char TEXT_FSST[]      = "FSST";
// constexpr char TEXT_FST[]     = "FST";
// constexpr char TEXT_FSUB[]    = "FSUB";
constexpr char TEXT_FSUEQ[]     = "FSUEQ";
constexpr char TEXT_FSUGE[]     = "FSUGE";
constexpr char TEXT_FSUGT[]     = "FSUGT";
constexpr char TEXT_FSULE[]     = "FSULE";
constexpr char TEXT_FSULT[]     = "FSULT";
constexpr char TEXT_FSUN[]      = "FSUN";
constexpr char TEXT_FTAN[]      = "FTAN";
constexpr char TEXT_FTANH[]     = "FTANH";
constexpr char TEXT_FTENTOX[]   = "FTENTOX";
constexpr char TEXT_FTRAPEQ[]   = "FTRAPEQ";
constexpr char TEXT_FTRAPF[]    = "FTRAPF";
constexpr char TEXT_FTRAPGE[]   = "FTRAPGE";
constexpr char TEXT_FTRAPGL[]   = "FTRAPGL";
constexpr char TEXT_FTRAPGLE[]  = "FTRAPGLE";
constexpr char TEXT_FTRAPGT[]   = "FTRAPGT";
constexpr char TEXT_FTRAPLE[]   = "FTRAPLE";
constexpr char TEXT_FTRAPLT[]   = "FTRAPLT";
constexpr char TEXT_FTRAPNE[]   = "FTRAPNE";
constexpr char TEXT_FTRAPNGE[]  = "FTRAPNGE";
constexpr char TEXT_FTRAPNGL[]  = "FTRAPNGL";
constexpr char TEXT_FTRAPNGLE[] = "FTRAPNGLE";
constexpr char TEXT_FTRAPNGT[]  = "FTRAPNGT";
constexpr char TEXT_FTRAPNLE[]  = "FTRAPNLE";
constexpr char TEXT_FTRAPNLT[]  = "FTRAPNLT";
constexpr char TEXT_FTRAPOGE[]  = "FTRAPOGE";
constexpr char TEXT_FTRAPOGL[]  = "FTRAPOGL";
constexpr char TEXT_FTRAPOGT[]  = "FTRAPOGT";
constexpr char TEXT_FTRAPOLE[]  = "FTRAPOLE";
constexpr char TEXT_FTRAPOLT[]  = "FTRAPOLT";
constexpr char TEXT_FTRAPOR[]   = "FTRAPOR";
constexpr char TEXT_FTRAPSEQ[]  = "FTRAPSEQ";
constexpr char TEXT_FTRAPSF[]   = "FTRAPSF";
constexpr char TEXT_FTRAPSNE[]  = "FTRAPSNE";
constexpr char TEXT_FTRAPST[]   = "FTRAPST";
constexpr char TEXT_FTRAPT[]    = "FTRAPT";
constexpr char TEXT_FTRAPUEQ[]  = "FTRAPUEQ";
constexpr char TEXT_FTRAPUGE[]  = "FTRAPUGE";
constexpr char TEXT_FTRAPUGT[]  = "FTRAPUGT";
constexpr char TEXT_FTRAPULE[]  = "FTRAPULE";
constexpr char TEXT_FTRAPULT[]  = "FTRAPULT";
constexpr char TEXT_FTRAPUN[]   = "FTRAPUN";
// constexpr char TEXT_FTST[]    = "FTST";
constexpr char TEXT_FTWOTOX[]   = "FTWOTOX";

constexpr char TEXT_REG_FP0[]   = "FP0";
constexpr char TEXT_REG_FP1[]   = "FP1";
constexpr char TEXT_REG_FP2[]   = "FP2";
constexpr char TEXT_REG_FP3[]   = "FP3";
constexpr char TEXT_REG_FP4[]   = "FP4";
constexpr char TEXT_REG_FP5[]   = "FP5";
constexpr char TEXT_REG_FP6[]   = "FP6";
constexpr char TEXT_REG_FP7[]   = "FP7";
constexpr char TEXT_REG_FPCR[]  = "FPCR";
constexpr char TEXT_REG_FPIAR[] = "FPIAR";
constexpr char TEXT_REG_FPSR[]  = "FPSR";

// MC68851
constexpr char TEXT_PBAC[]     = "PBAC";
constexpr char TEXT_PBAS[]     = "PBAS";
constexpr char TEXT_PBBC[]     = "PBBC";
constexpr char TEXT_PBBS[]     = "PBBS";
constexpr char TEXT_PBCC[]     = "PBCC";
constexpr char TEXT_PBCS[]     = "PBCS";
constexpr char TEXT_PBGC[]     = "PBGC";
constexpr char TEXT_PBGS[]     = "PBGS";
constexpr char TEXT_PBIC[]     = "PBIC";
constexpr char TEXT_PBIS[]     = "PBIS";
constexpr char TEXT_PBLC[]     = "PBLC";
constexpr char TEXT_PBLS[]     = "PBLS";
constexpr char TEXT_PBSC[]     = "PBSC";
constexpr char TEXT_PBSS[]     = "PBSS";
constexpr char TEXT_PBWC[]     = "PBWC";
constexpr char TEXT_PBWS[]     = "PBWS";
constexpr char TEXT_PDBAC[]    = "PDBAC";
constexpr char TEXT_PDBAS[]    = "PDBAS";
constexpr char TEXT_PDBBC[]    = "PDBBC";
constexpr char TEXT_PDBBS[]    = "PDBBS";
constexpr char TEXT_PDBCC[]    = "PDBCC";
constexpr char TEXT_PDBCS[]    = "PDBCS";
constexpr char TEXT_PDBGC[]    = "PDBGC";
constexpr char TEXT_PDBGS[]    = "PDBGS";
constexpr char TEXT_PDBIC[]    = "PDBIC";
constexpr char TEXT_PDBIS[]    = "PDBIS";
constexpr char TEXT_PDBLC[]    = "PDBLC";
constexpr char TEXT_PDBLS[]    = "PDBLS";
constexpr char TEXT_PDBSC[]    = "PDBSC";
constexpr char TEXT_PDBSS[]    = "PDBSS";
constexpr char TEXT_PDBWC[]    = "PDBWC";
constexpr char TEXT_PDBWS[]    = "PDBWS";
constexpr char TEXT_PFLUSH[]   = "PFLUSH";
constexpr char TEXT_PFLUSHA[]  = "PFLUSHA";
constexpr char TEXT_PFLUSHR[]  = "PFLUSHR";
constexpr char TEXT_PFLUSHS[]  = "PFLUSHS";
constexpr char TEXT_PLOADR[]   = "PLOADR";
constexpr char TEXT_PLOADW[]   = "PLOADW";
constexpr char TEXT_PMOVE[]    = "PMOVE";
constexpr char TEXT_PRESTORE[] = "PRESTORE";
constexpr char TEXT_PSAC[]     = "PSAC";
constexpr char TEXT_PSAS[]     = "PSAS";
constexpr char TEXT_PSAVE[]    = "PSAVE";
constexpr char TEXT_PSBC[]     = "PSBC";
constexpr char TEXT_PSBS[]     = "PSBS";
constexpr char TEXT_PSCC[]     = "PSCC";
constexpr char TEXT_PSCS[]     = "PSCS";
constexpr char TEXT_PSGC[]     = "PSGC";
constexpr char TEXT_PSGS[]     = "PSGS";
constexpr char TEXT_PSIC[]     = "PSIC";
constexpr char TEXT_PSIS[]     = "PSIS";
constexpr char TEXT_PSLC[]     = "PSLC";
constexpr char TEXT_PSLS[]     = "PSLS";
constexpr char TEXT_PSSC[]     = "PSSC";
constexpr char TEXT_PSSS[]     = "PSSS";
constexpr char TEXT_PSWC[]     = "PSWC";
constexpr char TEXT_PSWS[]     = "PSWS";
constexpr char TEXT_PTESTR[]   = "PTESTR";
constexpr char TEXT_PTESTW[]   = "PTESTW";
constexpr char TEXT_PTRAPAC[]  = "PTRAPAC";
constexpr char TEXT_PTRAPAS[]  = "PTRAPAS";
constexpr char TEXT_PTRAPBC[]  = "PTRAPBC";
constexpr char TEXT_PTRAPBS[]  = "PTRAPBS";
constexpr char TEXT_PTRAPCC[]  = "PTRAPCC";
constexpr char TEXT_PTRAPCS[]  = "PTRAPCS";
constexpr char TEXT_PTRAPGC[]  = "PTRAPGC";
constexpr char TEXT_PTRAPGS[]  = "PTRAPGS";
constexpr char TEXT_PTRAPIC[]  = "PTRAPIC";
constexpr char TEXT_PTRAPIS[]  = "PTRAPIS";
constexpr char TEXT_PTRAPLC[]  = "PTRAPLC";
constexpr char TEXT_PTRAPLS[]  = "PTRAPLS";
constexpr char TEXT_PTRAPSC[]  = "PTRAPSC";
constexpr char TEXT_PTRAPSS[]  = "PTRAPSS";
constexpr char TEXT_PTRAPWC[]  = "PTRAPWC";
constexpr char TEXT_PTRAPWS[]  = "PTRAPWS";
constexpr char TEXT_PVALID[]   = "PVALID";

constexpr char TEXT_REG_AC[]   = "AC";
constexpr char TEXT_REG_BAC[]  = "BAC";
constexpr char TEXT_REG_BAD[]  = "BAD";
constexpr char TEXT_REG_CAL[]  = "CAL";
constexpr char TEXT_REG_CRP[]  = "CRP";
constexpr char TEXT_REG_DRP[]  = "DRP";
constexpr char TEXT_REG_PCSR[] = "PCSR";
// constexpr char TEXT_REG_PSR[] = "PSR";
constexpr char TEXT_REG_SCC[]  = "SCC";
constexpr char TEXT_REG_SRP[]  = "SRP";
// constexpr char TEXT_REG_TC[]  = "TC";
constexpr char TEXT_REG_VAL[]  = "VAL";

// clang-format on
}  // namespace mc68000
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
