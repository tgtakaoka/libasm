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

#ifndef __TEXT_MC68000_H__
#define __TEXT_MC68000_H__

#include "config_host.h"

namespace libasm {
namespace mc68000 {

// MC68000 instructions
static const char TEXT_ABCD[]  PROGMEM = "ABCD";
static const char TEXT_ADD[]   PROGMEM = "ADD";
static const char TEXT_ADDA[]  PROGMEM = "ADDA";
static const char TEXT_ADDI[]  PROGMEM = "ADDI";
static const char TEXT_ADDQ[]  PROGMEM = "ADDQ";
static const char TEXT_ADDX[]  PROGMEM = "ADDX";
static const char TEXT_AND[]   PROGMEM = "AND";
static const char TEXT_ANDI[]  PROGMEM = "ANDI";
static const char TEXT_ASL[]   PROGMEM = "ASL";
static const char TEXT_ASR[]   PROGMEM = "ASR";
static const char TEXT_BCC[]   PROGMEM = "BCC";
static const char TEXT_BCHG[]  PROGMEM = "BCHG";
static const char TEXT_BCLR[]  PROGMEM = "BCLR";
static const char TEXT_BCS[]   PROGMEM = "BCS";
static const char TEXT_BEQ[]   PROGMEM = "BEQ";
static const char TEXT_BGE[]   PROGMEM = "BGE";
static const char TEXT_BGT[]   PROGMEM = "BGT";
static const char TEXT_BHI[]   PROGMEM = "BHI";
static const char TEXT_BHS[]   PROGMEM = "BHS";
static const char TEXT_BLE[]   PROGMEM = "BLE";
static const char TEXT_BLO[]   PROGMEM = "BLO";
static const char TEXT_BLS[]   PROGMEM = "BLS";
static const char TEXT_BLT[]   PROGMEM = "BLT";
static const char TEXT_BMI[]   PROGMEM = "BMI";
static const char TEXT_BNE[]   PROGMEM = "BNE";
static const char TEXT_BPL[]   PROGMEM = "BPL";
static const char TEXT_BRA[]   PROGMEM = "BRA";
static const char TEXT_BSET[]  PROGMEM = "BSET";
static const char TEXT_BSR[]   PROGMEM = "BSR";
static const char TEXT_BT[]    PROGMEM = "BT";
static const char TEXT_BTST[]  PROGMEM = "BTST";
static const char TEXT_BVC[]   PROGMEM = "BVC";
static const char TEXT_BVS[]   PROGMEM = "BVS";
static const char TEXT_CHK[]   PROGMEM = "CHK";
static const char TEXT_CLR[]   PROGMEM = "CLR";
static const char TEXT_CMP[]   PROGMEM = "CMP";
static const char TEXT_CMPA[]  PROGMEM = "CMPA";
static const char TEXT_CMPI[]  PROGMEM = "CMPI";
static const char TEXT_CMPM[]  PROGMEM = "CMPM";
static const char TEXT_DBCC[]  PROGMEM = "DBCC";
static const char TEXT_DBCS[]  PROGMEM = "DBCS";
static const char TEXT_DBEQ[]  PROGMEM = "DBEQ";
static const char TEXT_DBF[]   PROGMEM = "DBF";
static const char TEXT_DBGE[]  PROGMEM = "DBGE";
static const char TEXT_DBGT[]  PROGMEM = "DBGT";
static const char TEXT_DBHI[]  PROGMEM = "DBHI";
static const char TEXT_DBHS[]  PROGMEM = "DBHS";
static const char TEXT_DBLS[]  PROGMEM = "DBLS";
static const char TEXT_DBLE[]  PROGMEM = "DBLE";
static const char TEXT_DBLO[]  PROGMEM = "DBLO";
static const char TEXT_DBLT[]  PROGMEM = "DBLT";
static const char TEXT_DBMI[]  PROGMEM = "DBMI";
static const char TEXT_DBNE[]  PROGMEM = "DBNE";
static const char TEXT_DBPL[]  PROGMEM = "DBPL";
static const char TEXT_DBRA[]  PROGMEM = "DBRA";
static const char TEXT_DBT[]   PROGMEM = "DBT";
static const char TEXT_DBVC[]  PROGMEM = "DBVC";
static const char TEXT_DBVS[]  PROGMEM = "DBVS";
static const char TEXT_DIVS[]  PROGMEM = "DIVS";
static const char TEXT_DIVU[]  PROGMEM = "DIVU";
static const char TEXT_EOR[]   PROGMEM = "EOR";
static const char TEXT_EORI[]  PROGMEM = "EORI";
static const char TEXT_EXG[]   PROGMEM = "EXG";
static const char TEXT_EXT[]   PROGMEM = "EXT";
static const char TEXT_ILLEGAL[] PROGMEM = "ILLEGAL";
static const char TEXT_JMP[]   PROGMEM = "JMP";
static const char TEXT_JSR[]   PROGMEM = "JSR";
static const char TEXT_LEA[]   PROGMEM = "LEA";
static const char TEXT_LINK[]  PROGMEM = "LINK";
static const char TEXT_LSL[]   PROGMEM = "LSL";
static const char TEXT_LSR[]   PROGMEM = "LSR";
static const char TEXT_MOVE[]  PROGMEM = "MOVE";
static const char TEXT_MOVEA[] PROGMEM = "MOVEA";
static const char TEXT_MOVEM[] PROGMEM = "MOVEM";
static const char TEXT_MOVEP[] PROGMEM = "MOVEP";
static const char TEXT_MOVEQ[] PROGMEM = "MOVEQ";
static const char TEXT_MULS[]  PROGMEM = "MULS";
static const char TEXT_MULU[]  PROGMEM = "MULU";
static const char TEXT_NBCD[]  PROGMEM = "NBCD";
static const char TEXT_NEG[]   PROGMEM = "NEG";
static const char TEXT_NEGX[]  PROGMEM = "NEGX";
static const char TEXT_NOP[]   PROGMEM = "NOP";
static const char TEXT_NOT[]   PROGMEM = "NOT";
static const char TEXT_OR[]    PROGMEM = "OR";
static const char TEXT_ORI[]   PROGMEM = "ORI";
static const char TEXT_PEA[]   PROGMEM = "PEA";
static const char TEXT_RESET[] PROGMEM = "RESET";
static const char TEXT_ROL[]   PROGMEM = "ROL";
static const char TEXT_ROR[]   PROGMEM = "ROR";
static const char TEXT_ROXL[]  PROGMEM = "ROXL";
static const char TEXT_ROXR[]  PROGMEM = "ROXR";
static const char TEXT_RTE[]   PROGMEM = "RTE";
static const char TEXT_RTR[]   PROGMEM = "RTR";
static const char TEXT_RTS[]   PROGMEM = "RTS";
static const char TEXT_SBCD[]  PROGMEM = "SBCD";
static const char TEXT_SCC[]   PROGMEM = "SCC";
static const char TEXT_SCS[]   PROGMEM = "SCS";
static const char TEXT_SEQ[]   PROGMEM = "SEQ";
static const char TEXT_SF[]    PROGMEM = "SF";
static const char TEXT_SGE[]   PROGMEM = "SGE";
static const char TEXT_SGT[]   PROGMEM = "SGT";
static const char TEXT_SHI[]   PROGMEM = "SHI";
static const char TEXT_SHS[]   PROGMEM = "SHS";
static const char TEXT_SLE[]   PROGMEM = "SLE";
static const char TEXT_SLO[]   PROGMEM = "SLO";
static const char TEXT_SLS[]   PROGMEM = "SLS";
static const char TEXT_SLT[]   PROGMEM = "SLT";
static const char TEXT_SMI[]   PROGMEM = "SMI";
static const char TEXT_SNE[]   PROGMEM = "SNE";
static const char TEXT_SPL[]   PROGMEM = "SPL";
static const char TEXT_ST[]    PROGMEM = "ST";
static const char TEXT_STOP[]  PROGMEM = "STOP";
static const char TEXT_SUB[]   PROGMEM = "SUB";
static const char TEXT_SUBA[]  PROGMEM = "SUBA";
static const char TEXT_SUBI[]  PROGMEM = "SUBI";
static const char TEXT_SUBQ[]  PROGMEM = "SUBQ";
static const char TEXT_SUBX[]  PROGMEM = "SUBX";
static const char TEXT_SVC[]   PROGMEM = "SVC";
static const char TEXT_SVS[]   PROGMEM = "SVS";
static const char TEXT_SWAP[]  PROGMEM = "SWAP";
static const char TEXT_TAS[]   PROGMEM = "TAS";
static const char TEXT_TRAP[]  PROGMEM = "TRAP";
static const char TEXT_TRAPV[] PROGMEM = "TRAPV";
static const char TEXT_TST[]   PROGMEM = "TST";
static const char TEXT_UNLK[]  PROGMEM = "UNLK";

} // namespace mc68000
} // namespace libasm

#endif // __TEXT_MC68000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
