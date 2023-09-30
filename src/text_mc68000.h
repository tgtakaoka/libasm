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

#include "text_common.h"

namespace libasm {
namespace text {
namespace mc68000 {

// clang-format off
extern const char TEXT_CPU_MC68000[] PROGMEM;
extern const char TEXT_CPU_68000[]   PROGMEM;

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
extern const char TEXT_ILLEG[] PROGMEM;
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
extern const char TEXT_RESET[] PROGMEM;
using common::TEXT_ROL;
using common::TEXT_ROR;
extern const char TEXT_ROXL[]  PROGMEM;
extern const char TEXT_ROXR[]  PROGMEM;
extern const char TEXT_RTE[]   PROGMEM;
using common::TEXT_RTR;
using common::TEXT_RTS;
using common::TEXT_SBCD;
extern const char TEXT_SCC[]   PROGMEM;
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
extern const char TEXT_SPL[]   PROGMEM;
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
// clang-format on

}  // namespace mc68000
}  // namespace text
}  // namespace libasm

#endif  // __TEXT_MC68000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
