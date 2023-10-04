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
constexpr char TEXT_CPU_MC68000[] PROGMEM = "MC68000";
constexpr char TEXT_CPU_68000[]   PROGMEM = "68000";

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
constexpr char TEXT_DBEQ[]  PROGMEM = "DBEQ";
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
constexpr char TEXT_DBNE[]  PROGMEM = "DBNE";
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
constexpr char TEXT_ILLEG[] PROGMEM = "ILLEGAL";
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
constexpr char TEXT_RESET[] PROGMEM = "RESET";
// constexpr char TEXT_ROL[]   PROGMEM = "ROL";
// constexpr char TEXT_ROR[]   PROGMEM = "ROR";
constexpr char TEXT_ROXL[]  PROGMEM = "ROXL";
constexpr char TEXT_ROXR[]  PROGMEM = "ROXR";
constexpr char TEXT_RTE[]   PROGMEM = "RTE";
constexpr char TEXT_RTR[]   PROGMEM = "RTR";
// constexpr char TEXT_RTS[]   PROGMEM = "RTS";
// constexpr char TEXT_SBCD[]  PROGMEM = "SBCD";
constexpr char TEXT_SCC[]   PROGMEM = "SCC";
constexpr char TEXT_SCS[]   PROGMEM = "SCS";
// constexpr char TEXT_SEQ[]   PROGMEM = "SEQ";
constexpr char TEXT_SF[]    PROGMEM = "SF";
constexpr char TEXT_SGE[]   PROGMEM = "SGE";
constexpr char TEXT_SGT[]   PROGMEM = "SGT";
constexpr char TEXT_SHI[]   PROGMEM = "SHI";
constexpr char TEXT_SHS[]   PROGMEM = "SHS";
constexpr char TEXT_SLE[]   PROGMEM = "SLE";
constexpr char TEXT_SLO[]   PROGMEM = "SLO";
constexpr char TEXT_SLS[]   PROGMEM = "SLS";
constexpr char TEXT_SLT[]   PROGMEM = "SLT";
// constexpr char TEXT_SMI[]   PROGMEM = "SMI";
constexpr char TEXT_SNE[]   PROGMEM = "SNE";
constexpr char TEXT_SPL[]   PROGMEM = "SPL";
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
constexpr char TEXT_REG_CCR[] PROGMEM = "CCR";
constexpr char TEXT_REG_USP[] PROGMEM = "USP";
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
