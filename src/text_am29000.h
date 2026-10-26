/*
 * Copyright 2026 Tadashi G. Takaoka
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

#ifndef __LIBASM_TEXT_AM29000__
#define __LIBASM_TEXT_AM29000__

#include "text_common.h"

namespace libasm {
namespace text {
namespace am29000 {
// clang-format off

extern const char TEXT_AM29000_LIST[];
extern const char TEXT_CPU_AM29030[];
extern const char TEXT_CPU_AM29040[];
extern const char TEXT_CPU_AM29050[];
extern const char TEXT_CPU_AM29200[];
extern const char TEXT_CPU_AM29240[];
extern const char TEXT_CPU_AM29245[];

// Am29000
using common::TEXT_ADD;
using common::TEXT_ADDC;
extern const char TEXT_ADDCS[];
extern const char TEXT_ADDCU[];
using common::TEXT_ADDS;
extern const char TEXT_ADDU[];
using common::TEXT_AND;
extern const char TEXT_ANDN[];
extern const char TEXT_ASEQ[];
extern const char TEXT_ASGE[];
extern const char TEXT_ASGEU[];
extern const char TEXT_ASGT[];
extern const char TEXT_ASGTU[];
extern const char TEXT_ASLE[];
extern const char TEXT_ASLEU[];
extern const char TEXT_ASLT[];
extern const char TEXT_ASLTU[];
extern const char TEXT_ASNEQ[];
using common::TEXT_CALL;
using common::TEXT_CALLI;
extern const char TEXT_CLASS[];
extern const char TEXT_CLZ[];
extern const char TEXT_CONST[];
extern const char TEXT_CONSTH[];
extern const char TEXT_CONSTHZ[];
extern const char TEXT_CONSTN[];
extern const char TEXT_CONVERT[];
extern const char TEXT_CPBYTE[];
extern const char TEXT_CPEQ[];
extern const char TEXT_CPGE[];
extern const char TEXT_CPGEU[];
extern const char TEXT_CPGT[];
extern const char TEXT_CPGTU[];
extern const char TEXT_CPLE[];
extern const char TEXT_CPLEU[];
extern const char TEXT_CPLT[];
extern const char TEXT_CPLTU[];
extern const char TEXT_CPNEQ[];
using common::TEXT_DADD;
extern const char TEXT_DDIV[];
extern const char TEXT_DEQ[];
extern const char TEXT_DGE[];
extern const char TEXT_DGT[];
extern const char TEXT_DMAC[];
extern const char TEXT_DMSM[];
extern const char TEXT_DMUL[];
extern const char TEXT_DSUB[];
using common::TEXT_DIV;
extern const char TEXT_DIV0[];
extern const char TEXT_DIVIDE[];
extern const char TEXT_DIVIDU[];
using common::TEXT_DIVL;
extern const char TEXT_DIVREM[];
extern const char TEXT_EMULATE[];
extern const char TEXT_EXBYTE[];
extern const char TEXT_EXHW[];
extern const char TEXT_EXHWS[];
extern const char TEXT_EXTRACT[];
using common::TEXT_FADD;
using common::TEXT_FDIV;
extern const char TEXT_FDMUL[];
extern const char TEXT_FEQ[];
extern const char TEXT_FGE[];
extern const char TEXT_FGT[];
extern const char TEXT_FMAC[];
extern const char TEXT_FMSM[];
using common::TEXT_FMUL;
using common::TEXT_FSUB;
using common::TEXT_HALT;
extern const char TEXT_INBYTE[];
extern const char TEXT_INHW[];
using common::TEXT_INV;
using common::TEXT_IRET;
extern const char TEXT_IRETINV[];
using common::TEXT_JMP;
extern const char TEXT_JMPF[];
extern const char TEXT_JMPFDEC[];
extern const char TEXT_JMPFI[];
using common::TEXT_JMPI;
extern const char TEXT_JMPT[];
extern const char TEXT_JMPTI[];
extern const char TEXT_LOAD[];
extern const char TEXT_LOADL[];
extern const char TEXT_LOADM[];
extern const char TEXT_LOADSET[];
extern const char TEXT_MFACC[];
extern const char TEXT_MFSR[];
extern const char TEXT_MFTLB[];
extern const char TEXT_MTACC[];
extern const char TEXT_MTSR[];
extern const char TEXT_MTSRIM[];
extern const char TEXT_MTTLB[];
using common::TEXT_MUL;
extern const char TEXT_MULL[];
extern const char TEXT_MULTIPLU[];
extern const char TEXT_MULTIPLY[];
extern const char TEXT_MULTM[];
extern const char TEXT_MULTMU[];
using common::TEXT_MULU;
extern const char TEXT_NAND[];
extern const char TEXT_NOR[];
using common::TEXT_OR;
extern const char TEXT_ORN[];
extern const char TEXT_SETIP[];
using common::TEXT_SLL;
extern const char TEXT_SQRT[];
using common::TEXT_SRA;
using common::TEXT_SRL;
extern const char TEXT_STORE[];
extern const char TEXT_STOREL[];
extern const char TEXT_STOREM[];
using common::TEXT_SUB;
using common::TEXT_SUBC;
extern const char TEXT_SUBCS[];
extern const char TEXT_SUBCU[];
using common::TEXT_SUBR;
extern const char TEXT_SUBRC[];
extern const char TEXT_SUBRCS[];
extern const char TEXT_SUBRCU[];
extern const char TEXT_SUBRS[];
extern const char TEXT_SUBRU[];
using common::TEXT_SUBS;
extern const char TEXT_SUBU[];
using common::TEXT_XOR;
extern const char TEXT_XNOR[];

// Options
extern const char OPT_BOOL_FPU[];
extern const char OPT_DESC_FPU[];
extern const char OPT_BOOL_INTMULT[];
extern const char OPT_DESC_INTMULT[];

// Special-purpose registers
extern const char TEXT_REG_VAB[];
extern const char TEXT_REG_OPS[];
extern const char TEXT_REG_CPS[];
extern const char TEXT_REG_CFG[];
extern const char TEXT_REG_CHA[];
extern const char TEXT_REG_CHD[];
extern const char TEXT_REG_CHC[];
extern const char TEXT_REG_RBP[];
extern const char TEXT_REG_TMC[];
extern const char TEXT_REG_TMR[];
extern const char TEXT_REG_PC0[];
extern const char TEXT_REG_PC1[];
extern const char TEXT_REG_PC2[];
extern const char TEXT_REG_MMU[];
extern const char TEXT_REG_LRU[];
extern const char TEXT_REG_RSN[];
extern const char TEXT_REG_RMA0[];
extern const char TEXT_REG_RMC0[];
extern const char TEXT_REG_RMA1[];
extern const char TEXT_REG_RMC1[];
extern const char TEXT_REG_SPC0[];
extern const char TEXT_REG_SPC1[];
extern const char TEXT_REG_SPC2[];
extern const char TEXT_REG_IBA0[];
extern const char TEXT_REG_IBC0[];
extern const char TEXT_REG_IBA1[];
extern const char TEXT_REG_IBC1[];
extern const char TEXT_REG_EXOP[];
extern const char TEXT_REG_CIR[];
extern const char TEXT_REG_CDR[];
extern const char TEXT_REG_IPC[];
extern const char TEXT_REG_IPA[];
extern const char TEXT_REG_IPB[];
extern const char TEXT_REG_Q[];
extern const char TEXT_REG_ALU[];
extern const char TEXT_REG_BP[];
extern const char TEXT_REG_FC[];
extern const char TEXT_REG_CR[];
extern const char TEXT_REG_FPE[];
extern const char TEXT_REG_INTE[];
extern const char TEXT_REG_FPS[];

// clang-format on
}  // namespace am29000
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_AM29000__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
