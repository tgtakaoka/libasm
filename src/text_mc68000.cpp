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
constexpr char TEXT_MC68000_LIST[] PROGMEM = "MC68000, MC68010";
constexpr char TEXT_CPU_MC68000[] PROGMEM = "MC68000";
constexpr char TEXT_CPU_MC68010[] PROGMEM = "MC68010";
constexpr char TEXT_FPU_MC68881[] PROGMEM = "MC68881";
constexpr char TEXT_CPU_68000[]   PROGMEM = "68000";
constexpr char TEXT_CPU_68010[]   PROGMEM = "68010";
constexpr char TEXT_FPU_68881[]   PROGMEM = "68881";

// MC68000 instructions
constexpr char TEXT_ABCD[]  PROGMEM = "ABCD";
// constexpr char TEXT_ADD[]   PROGMEM = "ADD";
// constexpr char TEXT_ADDA[]  PROGMEM = "ADDA";
// constexpr char TEXT_ADDI[]  PROGMEM = "ADDI";
constexpr char TEXT_ADDQ[]  PROGMEM = "ADDQ";
constexpr char TEXT_ADDX[]  PROGMEM = "ADDX";
// constexpr char TEXT_AND[]   PROGMEM = "AND";
// constexpr char TEXT_ANDI[]  PROGMEM = "ANDI";
// constexpr char TEXT_ASL[]   PROGMEM = "ASL";
// constexpr char TEXT_ASR[]   PROGMEM = "ASR";
// constexpr char TEXT_BCC[]   PROGMEM = "BCC";
constexpr char TEXT_BCHG[]  PROGMEM = "BCHG";
// constexpr char TEXT_BCLR[]  PROGMEM = "BCLR";
// constexpr char TEXT_BCS[]   PROGMEM = "BCS";
// constexpr char TEXT_BEQ[]   PROGMEM = "BEQ";
// constexpr char TEXT_BGE[]   PROGMEM = "BGE";
// constexpr char TEXT_BGT[]   PROGMEM = "BGT";
// constexpr char TEXT_BHI[]   PROGMEM = "BHI";
// constexpr char TEXT_BHS[]   PROGMEM = "BHS";
// constexpr char TEXT_BLE[]   PROGMEM = "BLE";
// constexpr char TEXT_BLO[]   PROGMEM = "BLO";
// constexpr char TEXT_BLS[]   PROGMEM = "BLS";
// constexpr char TEXT_BLT[]   PROGMEM = "BLT";
// constexpr char TEXT_BMI[]   PROGMEM = "BMI";
// constexpr char TEXT_BNE[]   PROGMEM = "BNE";
// constexpr char TEXT_BPL[]   PROGMEM = "BPL";
// constexpr char TEXT_BRA[]   PROGMEM = "BRA";
// constexpr char TEXT_BSET[]  PROGMEM = "BSET";
// constexpr char TEXT_BSR[]   PROGMEM = "BSR";
// constexpr char TEXT_BT[]    PROGMEM = "BT";
// constexpr char TEXT_BTST[]  PROGMEM = "BTST";
// constexpr char TEXT_BVC[]   PROGMEM = "BVC";
// constexpr char TEXT_BVS[]   PROGMEM = "BVS";
constexpr char TEXT_CHK[]   PROGMEM = "CHK";
// constexpr char TEXT_CLR[]   PROGMEM = "CLR";
// constexpr char TEXT_CMP[]   PROGMEM = "CMP";
// constexpr char TEXT_CMPA[]  PROGMEM = "CMPA";
constexpr char TEXT_CMPI[]  PROGMEM = "CMPI";
constexpr char TEXT_CMPM[]  PROGMEM = "CMPM";
constexpr char TEXT_DBCC[]  PROGMEM = "DBCC";
constexpr char TEXT_DBCS[]  PROGMEM = "DBCS";
// constexpr char TEXT_DBEQ[]  PROGMEM = "DBEQ";
constexpr char TEXT_DBF[]   PROGMEM = "DBF";
constexpr char TEXT_DBGE[]  PROGMEM = "DBGE";
constexpr char TEXT_DBGT[]  PROGMEM = "DBGT";
constexpr char TEXT_DBHI[]  PROGMEM = "DBHI";
constexpr char TEXT_DBHS[]  PROGMEM = "DBHS";
constexpr char TEXT_DBLS[]  PROGMEM = "DBLS";
constexpr char TEXT_DBLE[]  PROGMEM = "DBLE";
constexpr char TEXT_DBLO[]  PROGMEM = "DBLO";
constexpr char TEXT_DBLT[]  PROGMEM = "DBLT";
constexpr char TEXT_DBMI[]  PROGMEM = "DBMI";
// constexpr char TEXT_DBNE[]  PROGMEM = "DBNE";
constexpr char TEXT_DBPL[]  PROGMEM = "DBPL";
constexpr char TEXT_DBRA[]  PROGMEM = "DBRA";
constexpr char TEXT_DBT[]   PROGMEM = "DBT";
constexpr char TEXT_DBVC[]  PROGMEM = "DBVC";
constexpr char TEXT_DBVS[]  PROGMEM = "DBVS";
// constexpr char TEXT_DIVS[]  PROGMEM = "DIVS";
// constexpr char TEXT_DIVU[]  PROGMEM = "DIVU";
// constexpr char TEXT_EOR[]   PROGMEM = "EOR";
// constexpr char TEXT_EORI[]  PROGMEM = "EORI";
// constexpr char TEXT_EXG[]   PROGMEM = "EXG";
// constexpr char TEXT_EXT[]   PROGMEM = "EXT";
constexpr char TEXT_ILLEGAL[] PROGMEM = "ILLEGAL";
// constexpr char TEXT_JMP[]   PROGMEM = "JMP";
// constexpr char TEXT_JSR[]   PROGMEM = "JSR";
// constexpr char TEXT_LEA[]   PROGMEM = "LEA";
constexpr char TEXT_LINK[]  PROGMEM = "LINK";
// constexpr char TEXT_LSL[]   PROGMEM = "LSL";
// constexpr char TEXT_LSR[]   PROGMEM = "LSR";
constexpr char TEXT_MOVE[]  PROGMEM = "MOVE";
constexpr char TEXT_MOVEA[] PROGMEM = "MOVEA";
constexpr char TEXT_MOVEM[] PROGMEM = "MOVEM";
constexpr char TEXT_MOVEP[] PROGMEM = "MOVEP";
constexpr char TEXT_MOVEQ[] PROGMEM = "MOVEQ";
constexpr char TEXT_MULS[]  PROGMEM = "MULS";
// constexpr char TEXT_MULU[]  PROGMEM = "MULU";
constexpr char TEXT_NBCD[]  PROGMEM = "NBCD";
// constexpr char TEXT_NEG[]   PROGMEM = "NEG";
// constexpr char TEXT_NEGX[]  PROGMEM = "NEGX";
// constexpr char TEXT_NOP[]   PROGMEM = "NOP";
// constexpr char TEXT_NOT[]   PROGMEM = "NOT";
// constexpr char TEXT_OR[]    PROGMEM = "OR";
// constexpr char TEXT_ORI[]   PROGMEM = "ORI";
// constexpr char TEXT_PEA[]   PROGMEM = "PEA";
// constexpr char TEXT_RESET[] PROGMEM = "RESET";
// constexpr char TEXT_ROL[]   PROGMEM = "ROL";
// constexpr char TEXT_ROR[]   PROGMEM = "ROR";
constexpr char TEXT_ROXL[]  PROGMEM = "ROXL";
constexpr char TEXT_ROXR[]  PROGMEM = "ROXR";
constexpr char TEXT_RTE[]   PROGMEM = "RTE";
// constexpr char TEXT_RTR[]   PROGMEM = "RTR";
// constexpr char TEXT_RTS[]   PROGMEM = "RTS";
// constexpr char TEXT_SBCD[]  PROGMEM = "SBCD";
// constexpr char TEXT_SCC[]   PROGMEM = "SCC";
constexpr char TEXT_SCS[]   PROGMEM = "SCS";
// constexpr char TEXT_SEQ[]   PROGMEM = "SEQ";
constexpr char TEXT_SF[]    PROGMEM = "SF";
constexpr char TEXT_SGE[]   PROGMEM = "SGE";
// constexpr char TEXT_SGT[]   PROGMEM = "SGT";
constexpr char TEXT_SHI[]   PROGMEM = "SHI";
constexpr char TEXT_SHS[]   PROGMEM = "SHS";
constexpr char TEXT_SLE[]   PROGMEM = "SLE";
constexpr char TEXT_SLO[]   PROGMEM = "SLO";
constexpr char TEXT_SLS[]   PROGMEM = "SLS";
constexpr char TEXT_SLT[]   PROGMEM = "SLT";
// constexpr char TEXT_SMI[]   PROGMEM = "SMI";
constexpr char TEXT_SNE[]   PROGMEM = "SNE";
// constexpr char TEXT_SPL[]   PROGMEM = "SPL";
// constexpr char TEXT_ST[]    PROGMEM = "ST";
// constexpr char TEXT_STOP[]  PROGMEM = "STOP";
// constexpr char TEXT_SUB[]   PROGMEM = "SUB";
// constexpr char TEXT_SUBA[]  PROGMEM = "SUBA";
// constexpr char TEXT_SUBI[]  PROGMEM = "SUBI";
constexpr char TEXT_SUBQ[]  PROGMEM = "SUBQ";
constexpr char TEXT_SUBX[]  PROGMEM = "SUBX";
// constexpr char TEXT_SVC[]   PROGMEM = "SVC";
constexpr char TEXT_SVS[]   PROGMEM = "SVS";
// constexpr char TEXT_SWAP[]  PROGMEM = "SWAP";
constexpr char TEXT_TAS[]   PROGMEM = "TAS";
// constexpr char TEXT_TRAP[]  PROGMEM = "TRAP";
constexpr char TEXT_TRAPV[] PROGMEM = "TRAPV";
// constexpr char TEXT_TST[]   PROGMEM = "TST";
constexpr char TEXT_UNLK[]  PROGMEM = "UNLK";

constexpr char TEXT_REG_D0[] PROGMEM = "D0";
constexpr char TEXT_REG_D1[] PROGMEM = "D1";
constexpr char TEXT_REG_D2[] PROGMEM = "D2";
constexpr char TEXT_REG_D3[] PROGMEM = "D3";
constexpr char TEXT_REG_D4[] PROGMEM = "D4";
constexpr char TEXT_REG_D5[] PROGMEM = "D5";
constexpr char TEXT_REG_D6[] PROGMEM = "D6";
constexpr char TEXT_REG_D7[] PROGMEM = "D7";
constexpr char TEXT_REG_A0[] PROGMEM = "A0";
constexpr char TEXT_REG_A1[] PROGMEM = "A1";
constexpr char TEXT_REG_A2[] PROGMEM = "A2";
constexpr char TEXT_REG_A3[] PROGMEM = "A3";
constexpr char TEXT_REG_A4[] PROGMEM = "A4";
constexpr char TEXT_REG_A5[] PROGMEM = "A5";
constexpr char TEXT_REG_A6[] PROGMEM = "A6";
constexpr char TEXT_REG_A7[] PROGMEM = "A7";
// constexpr char TEXT_REG_PC[] PROGMEM = "PC";
constexpr char TEXT_REG_SR[] PROGMEM = "SR";
// constexpr char TEXT_REG_CCR[] PROGMEM = "CCR";
// constexpr char TEXT_REG_USP[] PROGMEM = "USP";

// MC68010
constexpr char TEXT_MOVEC[] PROGMEM = "MOVEC";
constexpr char TEXT_MOVES[] PROGMEM = "MOVES";
constexpr char TEXT_RTD[]   PROGMEM = "RTD";

constexpr char TEXT_REG_DFC[] PROGMEM = "DFC";
constexpr char TEXT_REG_SFC[] PROGMEM = "SFC";
constexpr char TEXT_REG_VBR[] PROGMEM = "VBR";

// MC68881
// constexpr char TEXT_FABS[]      PROGMEM = "FABS";
constexpr char TEXT_FACOS[]     PROGMEM = "FACOS";
// constexpr char TEXT_FADD[]      PROGMEM = "FADD";
constexpr char TEXT_FASIN[]     PROGMEM = "FASIN";
constexpr char TEXT_FATAN[]     PROGMEM = "FATAN";
constexpr char TEXT_FATANH[]    PROGMEM = "FATANH";
constexpr char TEXT_FBEQ[]      PROGMEM = "FBEQ";
constexpr char TEXT_FBF[]       PROGMEM = "FBF";
constexpr char TEXT_FBGE[]      PROGMEM = "FBGE";
constexpr char TEXT_FBGL[]      PROGMEM = "FBGL";
constexpr char TEXT_FBGLE[]     PROGMEM = "FBGLE";
constexpr char TEXT_FBGT[]      PROGMEM = "FBGT";
constexpr char TEXT_FBLE[]      PROGMEM = "FBLE";
constexpr char TEXT_FBLT[]      PROGMEM = "FBLT";
constexpr char TEXT_FBNE[]      PROGMEM = "FBNE";
constexpr char TEXT_FBNGE[]     PROGMEM = "FBNGE";
constexpr char TEXT_FBNGL[]     PROGMEM = "FBNGL";
constexpr char TEXT_FBNGLE[]    PROGMEM = "FBNGLE";
constexpr char TEXT_FBNGT[]     PROGMEM = "FBNGT";
constexpr char TEXT_FBNLE[]     PROGMEM = "FBNLE";
constexpr char TEXT_FBNLT[]     PROGMEM = "FBNLT";
constexpr char TEXT_FBOGE[]     PROGMEM = "FBOGE";
constexpr char TEXT_FBOGL[]     PROGMEM = "FBOGL";
constexpr char TEXT_FBOGT[]     PROGMEM = "FBOGT";
constexpr char TEXT_FBOLE[]     PROGMEM = "FBOLE";
constexpr char TEXT_FBOLT[]     PROGMEM = "FBOLT";
constexpr char TEXT_FBOR[]      PROGMEM = "FBOR";
constexpr char TEXT_FBSEQ[]     PROGMEM = "FBSEQ";
constexpr char TEXT_FBSF[]      PROGMEM = "FBSF";
constexpr char TEXT_FBSNE[]     PROGMEM = "FBSNE";
constexpr char TEXT_FBST[]      PROGMEM = "FBST";
constexpr char TEXT_FBT[]       PROGMEM = "FBT";
constexpr char TEXT_FBUEQ[]     PROGMEM = "FBUEQ";
constexpr char TEXT_FBUGE[]     PROGMEM = "FBUGE";
constexpr char TEXT_FBUGT[]     PROGMEM = "FBUGT";
constexpr char TEXT_FBULE[]     PROGMEM = "FBULE";
constexpr char TEXT_FBULT[]     PROGMEM = "FBULT";
constexpr char TEXT_FBUN[]      PROGMEM = "FBUN";
constexpr char TEXT_FCMP[]      PROGMEM = "FCMP";
constexpr char TEXT_FCOS[]      PROGMEM = "FCOS";
constexpr char TEXT_FCOSH[]     PROGMEM = "FCOSH";
constexpr char TEXT_FDBEQ[]     PROGMEM = "FDBEQ";
constexpr char TEXT_FDBF[]      PROGMEM = "FDBF";
constexpr char TEXT_FDBGE[]     PROGMEM = "FDBGE";
constexpr char TEXT_FDBGL[]     PROGMEM = "FDBGL";
constexpr char TEXT_FDBGLE[]    PROGMEM = "FDBGLE";
constexpr char TEXT_FDBGT[]     PROGMEM = "FDBGT";
constexpr char TEXT_FDBLE[]     PROGMEM = "FDBLE";
constexpr char TEXT_FDBLT[]     PROGMEM = "FDBLT";
constexpr char TEXT_FDBNE[]     PROGMEM = "FDBNE";
constexpr char TEXT_FDBNGE[]    PROGMEM = "FDBNGE";
constexpr char TEXT_FDBNGL[]    PROGMEM = "FDBNGL";
constexpr char TEXT_FDBNGLE[]   PROGMEM = "FDBNGLE";
constexpr char TEXT_FDBNGT[]    PROGMEM = "FDBNGT";
constexpr char TEXT_FDBNLE[]    PROGMEM = "FDBNLE";
constexpr char TEXT_FDBNLT[]    PROGMEM = "FDBNLT";
constexpr char TEXT_FDBOGE[]    PROGMEM = "FDBOGE";
constexpr char TEXT_FDBOGL[]    PROGMEM = "FDBOGL";
constexpr char TEXT_FDBOGT[]    PROGMEM = "FDBOGT";
constexpr char TEXT_FDBOLE[]    PROGMEM = "FDBOLE";
constexpr char TEXT_FDBOLT[]    PROGMEM = "FDBOLT";
constexpr char TEXT_FDBOR[]     PROGMEM = "FDBOR";
constexpr char TEXT_FDBSEQ[]    PROGMEM = "FDBSEQ";
constexpr char TEXT_FDBSF[]     PROGMEM = "FDBSF";
constexpr char TEXT_FDBSNE[]    PROGMEM = "FDBSNE";
constexpr char TEXT_FDBST[]     PROGMEM = "FDBST";
constexpr char TEXT_FDBT[]      PROGMEM = "FDBT";
constexpr char TEXT_FDBUEQ[]    PROGMEM = "FDBUEQ";
constexpr char TEXT_FDBUGE[]    PROGMEM = "FDBUGE";
constexpr char TEXT_FDBUGT[]    PROGMEM = "FDBUGT";
constexpr char TEXT_FDBULE[]    PROGMEM = "FDBULE";
constexpr char TEXT_FDBULT[]    PROGMEM = "FDBULT";
constexpr char TEXT_FDBUN[]     PROGMEM = "FDBUN";
// constexpr char TEXT_FDIV[]      PROGMEM = "FDIV";
constexpr char TEXT_FETOX[]     PROGMEM = "FETOX";
constexpr char TEXT_FETOXM1[]   PROGMEM = "FETOXM1";
constexpr char TEXT_FGETEXP[]   PROGMEM = "FGETEXP";
constexpr char TEXT_FGETMAN[]   PROGMEM = "FGETMAN";
constexpr char TEXT_FINT[]      PROGMEM = "FINT";
constexpr char TEXT_FINTRZ[]    PROGMEM = "FINTRZ";
constexpr char TEXT_FLOG10[]    PROGMEM = "FLOG10";
constexpr char TEXT_FLOG2[]     PROGMEM = "FLOG2";
constexpr char TEXT_FLOGN[]     PROGMEM = "FLOGN";
constexpr char TEXT_FLOGNP1[]   PROGMEM = "FLOGNP1";
constexpr char TEXT_FMOD[]      PROGMEM = "FMOD";
constexpr char TEXT_FMOVE[]     PROGMEM = "FMOVE";
constexpr char TEXT_FMOVECR[]   PROGMEM = "FMOVECR";
constexpr char TEXT_FMOVEM[]    PROGMEM = "FMOVEM";
// constexpr char TEXT_FMUL[]      PROGMEM = "FMUL";
constexpr char TEXT_FNEG[]      PROGMEM = "FNEG";
// constexpr char TEXT_FNOP[]      PROGMEM = "FNOP";
constexpr char TEXT_FREM[]      PROGMEM = "FREM";
constexpr char TEXT_FRESTORE[]  PROGMEM = "FRESTORE";
// constexpr char TEXT_FSAVE[]     PROGMEM = "FSAVE";
// constexpr char TEXT_FSCALE[]    PROGMEM = "FSCALE";
constexpr char TEXT_FSEQ[]      PROGMEM = "FSEQ";
constexpr char TEXT_FSF[]       PROGMEM = "FSF";
constexpr char TEXT_FSGE[]      PROGMEM = "FSGE";
constexpr char TEXT_FSGL[]      PROGMEM = "FSGL";
constexpr char TEXT_FSGLDIV[]   PROGMEM = "FSGLDIV";
constexpr char TEXT_FSGLE[]     PROGMEM = "FSGLE";
constexpr char TEXT_FSGLMUL[]   PROGMEM = "FSGLMUL";
constexpr char TEXT_FSGT[]      PROGMEM = "FSGT";
constexpr char TEXT_FSIN[]      PROGMEM = "FSIN";
constexpr char TEXT_FSINCOS[]   PROGMEM = "FSINCOS";
constexpr char TEXT_FSINH[]     PROGMEM = "FSINH";
constexpr char TEXT_FSLE[]      PROGMEM = "FSLE";
constexpr char TEXT_FSLT[]      PROGMEM = "FSLT";
constexpr char TEXT_FSNE[]      PROGMEM = "FSNE";
constexpr char TEXT_FSNGE[]     PROGMEM = "FSNGE";
constexpr char TEXT_FSNGL[]     PROGMEM = "FSNGL";
constexpr char TEXT_FSNGLE[]    PROGMEM = "FSNGLE";
constexpr char TEXT_FSNGT[]     PROGMEM = "FSNGT";
constexpr char TEXT_FSNLE[]     PROGMEM = "FSNLE";
constexpr char TEXT_FSNLT[]     PROGMEM = "FSNLT";
constexpr char TEXT_FSOGE[]     PROGMEM = "FSOGE";
constexpr char TEXT_FSOGL[]     PROGMEM = "FSOGL";
constexpr char TEXT_FSOGT[]     PROGMEM = "FSOGT";
constexpr char TEXT_FSOLE[]     PROGMEM = "FSOLE";
constexpr char TEXT_FSOLT[]     PROGMEM = "FSOLT";
constexpr char TEXT_FSOR[]      PROGMEM = "FSOR";
// constexpr char TEXT_FSQRT[]     PROGMEM = "FSQRT";
constexpr char TEXT_FSSEQ[]     PROGMEM = "FSSEQ";
constexpr char TEXT_FSSF[]      PROGMEM = "FSSF";
constexpr char TEXT_FSSNE[]     PROGMEM = "FSSNE";
constexpr char TEXT_FSST[]      PROGMEM = "FSST";
// constexpr char TEXT_FST[]       PROGMEM = "FST";
// constexpr char TEXT_FSUB[]      PROGMEM = "FSUB";
constexpr char TEXT_FSUEQ[]     PROGMEM = "FSUEQ";
constexpr char TEXT_FSUGE[]     PROGMEM = "FSUGE";
constexpr char TEXT_FSUGT[]     PROGMEM = "FSUGT";
constexpr char TEXT_FSULE[]     PROGMEM = "FSULE";
constexpr char TEXT_FSULT[]     PROGMEM = "FSULT";
constexpr char TEXT_FSUN[]      PROGMEM = "FSUN";
constexpr char TEXT_FTAN[]      PROGMEM = "FTAN";
constexpr char TEXT_FTANH[]     PROGMEM = "FTANH";
constexpr char TEXT_FTENTOX[]   PROGMEM = "FTENTOX";
constexpr char TEXT_FTRAPEQ[]   PROGMEM = "FTRAPEQ";
constexpr char TEXT_FTRAPF[]    PROGMEM = "FTRAPF";
constexpr char TEXT_FTRAPGE[]   PROGMEM = "FTRAPGE";
constexpr char TEXT_FTRAPGL[]   PROGMEM = "FTRAPGL";
constexpr char TEXT_FTRAPGLE[]  PROGMEM = "FTRAPGLE";
constexpr char TEXT_FTRAPGT[]   PROGMEM = "FTRAPGT";
constexpr char TEXT_FTRAPLE[]   PROGMEM = "FTRAPLE";
constexpr char TEXT_FTRAPLT[]   PROGMEM = "FTRAPLT";
constexpr char TEXT_FTRAPNE[]   PROGMEM = "FTRAPNE";
constexpr char TEXT_FTRAPNGE[]  PROGMEM = "FTRAPNGE";
constexpr char TEXT_FTRAPNGL[]  PROGMEM = "FTRAPNGL";
constexpr char TEXT_FTRAPNGLE[] PROGMEM = "FTRAPNGLE";
constexpr char TEXT_FTRAPNGT[]  PROGMEM = "FTRAPNGT";
constexpr char TEXT_FTRAPNLE[]  PROGMEM = "FTRAPNLE";
constexpr char TEXT_FTRAPNLT[]  PROGMEM = "FTRAPNLT";
constexpr char TEXT_FTRAPOGE[]  PROGMEM = "FTRAPOGE";
constexpr char TEXT_FTRAPOGL[]  PROGMEM = "FTRAPOGL";
constexpr char TEXT_FTRAPOGT[]  PROGMEM = "FTRAPOGT";
constexpr char TEXT_FTRAPOLE[]  PROGMEM = "FTRAPOLE";
constexpr char TEXT_FTRAPOLT[]  PROGMEM = "FTRAPOLT";
constexpr char TEXT_FTRAPOR[]   PROGMEM = "FTRAPOR";
constexpr char TEXT_FTRAPSEQ[]  PROGMEM = "FTRAPSEQ";
constexpr char TEXT_FTRAPSF[]   PROGMEM = "FTRAPSF";
constexpr char TEXT_FTRAPSNE[]  PROGMEM = "FTRAPSNE";
constexpr char TEXT_FTRAPST[]   PROGMEM = "FTRAPST";
constexpr char TEXT_FTRAPT[]    PROGMEM = "FTRAPT";
constexpr char TEXT_FTRAPUEQ[]  PROGMEM = "FTRAPUEQ";
constexpr char TEXT_FTRAPUGE[]  PROGMEM = "FTRAPUGE";
constexpr char TEXT_FTRAPUGT[]  PROGMEM = "FTRAPUGT";
constexpr char TEXT_FTRAPULE[]  PROGMEM = "FTRAPULE";
constexpr char TEXT_FTRAPULT[]  PROGMEM = "FTRAPULT";
constexpr char TEXT_FTRAPUN[]   PROGMEM = "FTRAPUN";
// constexpr char TEXT_FTST[]      PROGMEM = "FTST";
constexpr char TEXT_FTWOTOX[]   PROGMEM = "FTWOTOX";

constexpr char TEXT_REG_FP0[]   PROGMEM = "FP0";
constexpr char TEXT_REG_FP1[]   PROGMEM = "FP1";
constexpr char TEXT_REG_FP2[]   PROGMEM = "FP2";
constexpr char TEXT_REG_FP3[]   PROGMEM = "FP3";
constexpr char TEXT_REG_FP4[]   PROGMEM = "FP4";
constexpr char TEXT_REG_FP5[]   PROGMEM = "FP5";
constexpr char TEXT_REG_FP6[]   PROGMEM = "FP6";
constexpr char TEXT_REG_FP7[]   PROGMEM = "FP7";
constexpr char TEXT_REG_FPCR[]  PROGMEM = "FPCR";
constexpr char TEXT_REG_FPIAR[] PROGMEM = "FPIAR";
constexpr char TEXT_REG_FPSR[]  PROGMEM = "FPSR";
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
