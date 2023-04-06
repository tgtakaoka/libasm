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
const char TEXT_CPU_MC68000[] PROGMEM = "MC68000";
const char TEXT_CPU_68000[]   PROGMEM = "68000";

// MC68000 instructions
const char TEXT_ABCD[]  PROGMEM = "ABCD";
// const char TEXT_ADD[]   PROGMEM = "ADD";
// const char TEXT_ADDA[]  PROGMEM = "ADDA";
// const char TEXT_ADDI[]  PROGMEM = "ADDI";
const char TEXT_ADDQ[]  PROGMEM = "ADDQ";
const char TEXT_ADDX[]  PROGMEM = "ADDX";
// const char TEXT_AND[]   PROGMEM = "AND";
// const char TEXT_ANDI[]  PROGMEM = "ANDI";
// const char TEXT_ASL[]   PROGMEM = "ASL";
// const char TEXT_ASR[]   PROGMEM = "ASR";
// const char TEXT_BCC[]   PROGMEM = "BCC";
const char TEXT_BCHG[]  PROGMEM = "BCHG";
// const char TEXT_BCLR[]  PROGMEM = "BCLR";
// const char TEXT_BCS[]   PROGMEM = "BCS";
// const char TEXT_BEQ[]   PROGMEM = "BEQ";
// const char TEXT_BGE[]   PROGMEM = "BGE";
// const char TEXT_BGT[]   PROGMEM = "BGT";
// const char TEXT_BHI[]   PROGMEM = "BHI";
// const char TEXT_BHS[]   PROGMEM = "BHS";
// const char TEXT_BLE[]   PROGMEM = "BLE";
// const char TEXT_BLO[]   PROGMEM = "BLO";
// const char TEXT_BLS[]   PROGMEM = "BLS";
// const char TEXT_BLT[]   PROGMEM = "BLT";
// const char TEXT_BMI[]   PROGMEM = "BMI";
// const char TEXT_BNE[]   PROGMEM = "BNE";
// const char TEXT_BPL[]   PROGMEM = "BPL";
// const char TEXT_BRA[]   PROGMEM = "BRA";
// const char TEXT_BSET[]  PROGMEM = "BSET";
// const char TEXT_BSR[]   PROGMEM = "BSR";
// const char TEXT_BT[]    PROGMEM = "BT";
// const char TEXT_BTST[]  PROGMEM = "BTST";
// const char TEXT_BVC[]   PROGMEM = "BVC";
// const char TEXT_BVS[]   PROGMEM = "BVS";
const char TEXT_CHK[]   PROGMEM = "CHK";
// const char TEXT_CLR[]   PROGMEM = "CLR";
// const char TEXT_CMP[]   PROGMEM = "CMP";
// const char TEXT_CMPA[]  PROGMEM = "CMPA";
const char TEXT_CMPI[]  PROGMEM = "CMPI";
const char TEXT_CMPM[]  PROGMEM = "CMPM";
const char TEXT_DBCC[]  PROGMEM = "DBCC";
const char TEXT_DBCS[]  PROGMEM = "DBCS";
const char TEXT_DBEQ[]  PROGMEM = "DBEQ";
const char TEXT_DBF[]   PROGMEM = "DBF";
const char TEXT_DBGE[]  PROGMEM = "DBGE";
const char TEXT_DBGT[]  PROGMEM = "DBGT";
const char TEXT_DBHI[]  PROGMEM = "DBHI";
const char TEXT_DBHS[]  PROGMEM = "DBHS";
const char TEXT_DBLS[]  PROGMEM = "DBLS";
const char TEXT_DBLE[]  PROGMEM = "DBLE";
const char TEXT_DBLO[]  PROGMEM = "DBLO";
const char TEXT_DBLT[]  PROGMEM = "DBLT";
const char TEXT_DBMI[]  PROGMEM = "DBMI";
const char TEXT_DBNE[]  PROGMEM = "DBNE";
const char TEXT_DBPL[]  PROGMEM = "DBPL";
const char TEXT_DBRA[]  PROGMEM = "DBRA";
const char TEXT_DBT[]   PROGMEM = "DBT";
const char TEXT_DBVC[]  PROGMEM = "DBVC";
const char TEXT_DBVS[]  PROGMEM = "DBVS";
// const char TEXT_DIVS[]  PROGMEM = "DIVS";
// const char TEXT_DIVU[]  PROGMEM = "DIVU";
// const char TEXT_EOR[]   PROGMEM = "EOR";
// const char TEXT_EORI[]  PROGMEM = "EORI";
// const char TEXT_EXG[]   PROGMEM = "EXG";
// const char TEXT_EXT[]   PROGMEM = "EXT";
const char TEXT_ILLEG[] PROGMEM = "ILLEGAL";
// const char TEXT_JMP[]   PROGMEM = "JMP";
// const char TEXT_JSR[]   PROGMEM = "JSR";
// const char TEXT_LEA[]   PROGMEM = "LEA";
const char TEXT_LINK[]  PROGMEM = "LINK";
// const char TEXT_LSL[]   PROGMEM = "LSL";
// const char TEXT_LSR[]   PROGMEM = "LSR";
const char TEXT_MOVE[]  PROGMEM = "MOVE";
const char TEXT_MOVEA[] PROGMEM = "MOVEA";
const char TEXT_MOVEM[] PROGMEM = "MOVEM";
const char TEXT_MOVEP[] PROGMEM = "MOVEP";
const char TEXT_MOVEQ[] PROGMEM = "MOVEQ";
const char TEXT_MULS[]  PROGMEM = "MULS";
// const char TEXT_MULU[]  PROGMEM = "MULU";
const char TEXT_NBCD[]  PROGMEM = "NBCD";
// const char TEXT_NEG[]   PROGMEM = "NEG";
// const char TEXT_NEGX[]  PROGMEM = "NEGX";
// const char TEXT_NOP[]   PROGMEM = "NOP";
// const char TEXT_NOT[]   PROGMEM = "NOT";
// const char TEXT_OR[]    PROGMEM = "OR";
// const char TEXT_ORI[]   PROGMEM = "ORI";
// const char TEXT_PEA[]   PROGMEM = "PEA";
const char TEXT_RESET[] PROGMEM = "RESET";
// const char TEXT_ROL[]   PROGMEM = "ROL";
// const char TEXT_ROR[]   PROGMEM = "ROR";
const char TEXT_ROXL[]  PROGMEM = "ROXL";
const char TEXT_ROXR[]  PROGMEM = "ROXR";
const char TEXT_RTE[]   PROGMEM = "RTE";
const char TEXT_RTR[]   PROGMEM = "RTR";
// const char TEXT_RTS[]   PROGMEM = "RTS";
// const char TEXT_SBCD[]  PROGMEM = "SBCD";
const char TEXT_SCC[]   PROGMEM = "SCC";
const char TEXT_SCS[]   PROGMEM = "SCS";
// const char TEXT_SEQ[]   PROGMEM = "SEQ";
const char TEXT_SF[]    PROGMEM = "SF";
const char TEXT_SGE[]   PROGMEM = "SGE";
const char TEXT_SGT[]   PROGMEM = "SGT";
const char TEXT_SHI[]   PROGMEM = "SHI";
const char TEXT_SHS[]   PROGMEM = "SHS";
const char TEXT_SLE[]   PROGMEM = "SLE";
const char TEXT_SLO[]   PROGMEM = "SLO";
const char TEXT_SLS[]   PROGMEM = "SLS";
const char TEXT_SLT[]   PROGMEM = "SLT";
// const char TEXT_SMI[]   PROGMEM = "SMI";
const char TEXT_SNE[]   PROGMEM = "SNE";
const char TEXT_SPL[]   PROGMEM = "SPL";
// const char TEXT_ST[]    PROGMEM = "ST";
// const char TEXT_STOP[]  PROGMEM = "STOP";
// const char TEXT_SUB[]   PROGMEM = "SUB";
// const char TEXT_SUBA[]  PROGMEM = "SUBA";
// const char TEXT_SUBI[]  PROGMEM = "SUBI";
const char TEXT_SUBQ[]  PROGMEM = "SUBQ";
const char TEXT_SUBX[]  PROGMEM = "SUBX";
// const char TEXT_SVC[]   PROGMEM = "SVC";
const char TEXT_SVS[]   PROGMEM = "SVS";
// const char TEXT_SWAP[]  PROGMEM = "SWAP";
const char TEXT_TAS[]   PROGMEM = "TAS";
// const char TEXT_TRAP[]  PROGMEM = "TRAP";
const char TEXT_TRAPV[] PROGMEM = "TRAPV";
// const char TEXT_TST[]   PROGMEM = "TST";
const char TEXT_UNLK[]  PROGMEM = "UNLK";

const char TEXT_REG_D0[] PROGMEM = "D0";
const char TEXT_REG_D1[] PROGMEM = "D1";
const char TEXT_REG_D2[] PROGMEM = "D2";
const char TEXT_REG_D3[] PROGMEM = "D3";
const char TEXT_REG_D4[] PROGMEM = "D4";
const char TEXT_REG_D5[] PROGMEM = "D5";
const char TEXT_REG_D6[] PROGMEM = "D6";
const char TEXT_REG_D7[] PROGMEM = "D7";
const char TEXT_REG_A0[] PROGMEM = "A0";
const char TEXT_REG_A1[] PROGMEM = "A1";
const char TEXT_REG_A2[] PROGMEM = "A2";
const char TEXT_REG_A3[] PROGMEM = "A3";
const char TEXT_REG_A4[] PROGMEM = "A4";
const char TEXT_REG_A5[] PROGMEM = "A5";
const char TEXT_REG_A6[] PROGMEM = "A6";
const char TEXT_REG_A7[] PROGMEM = "A7";
// const char TEXT_REG_PC[] PROGMEM = "PC";
const char TEXT_REG_SR[] PROGMEM = "SR";
const char TEXT_REG_CCR[] PROGMEM = "CCR";
const char TEXT_REG_USP[] PROGMEM = "USP";
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
