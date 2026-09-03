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

extern const char TEXT_AM29000_LIST[] PROGMEM;
extern const char TEXT_CPU_AM29030[]  PROGMEM;
extern const char TEXT_CPU_AM29040[]  PROGMEM;
extern const char TEXT_CPU_AM29050[]  PROGMEM;
extern const char TEXT_CPU_AM29200[]  PROGMEM;
extern const char TEXT_CPU_AM29240[]  PROGMEM;
extern const char TEXT_CPU_AM29245[]  PROGMEM;

// Am29000
using common::TEXT_ADD;
using common::TEXT_ADDC;
extern const char TEXT_ADDCS[]   PROGMEM;
extern const char TEXT_ADDCU[]   PROGMEM;
using common::TEXT_ADDS;
extern const char TEXT_ADDU[]    PROGMEM;
using common::TEXT_AND;
extern const char TEXT_ANDN[]    PROGMEM;
extern const char TEXT_ASEQ[]    PROGMEM;
extern const char TEXT_ASGE[]    PROGMEM;
extern const char TEXT_ASGEU[]   PROGMEM;
extern const char TEXT_ASGT[]    PROGMEM;
extern const char TEXT_ASGTU[]   PROGMEM;
extern const char TEXT_ASLE[]    PROGMEM;
extern const char TEXT_ASLEU[]   PROGMEM;
extern const char TEXT_ASLT[]    PROGMEM;
extern const char TEXT_ASLTU[]   PROGMEM;
extern const char TEXT_ASNEQ[]   PROGMEM;
using common::TEXT_CALL;
extern const char TEXT_CALLI[]   PROGMEM;
extern const char TEXT_CLASS[]   PROGMEM;
extern const char TEXT_CLZ[]     PROGMEM;
extern const char TEXT_CONST[]   PROGMEM;
extern const char TEXT_CONSTH[]  PROGMEM;
extern const char TEXT_CONSTHZ[] PROGMEM;
extern const char TEXT_CONSTN[]  PROGMEM;
extern const char TEXT_CONVERT[] PROGMEM;
extern const char TEXT_CPBYTE[]  PROGMEM;
extern const char TEXT_CPEQ[]    PROGMEM;
extern const char TEXT_CPGE[]    PROGMEM;
extern const char TEXT_CPGEU[]   PROGMEM;
extern const char TEXT_CPGT[]    PROGMEM;
extern const char TEXT_CPGTU[]   PROGMEM;
extern const char TEXT_CPLE[]    PROGMEM;
extern const char TEXT_CPLEU[]   PROGMEM;
extern const char TEXT_CPLT[]    PROGMEM;
extern const char TEXT_CPLTU[]   PROGMEM;
extern const char TEXT_CPNEQ[]   PROGMEM;
using common::TEXT_DADD;
extern const char TEXT_DDIV[]    PROGMEM;
extern const char TEXT_DEQ[]     PROGMEM;
extern const char TEXT_DGE[]     PROGMEM;
extern const char TEXT_DGT[]     PROGMEM;
extern const char TEXT_DMAC[]    PROGMEM;
extern const char TEXT_DMSM[]    PROGMEM;
extern const char TEXT_DMUL[]    PROGMEM;
extern const char TEXT_DSUB[]    PROGMEM;
using common::TEXT_DIV;
extern const char TEXT_DIV0[]    PROGMEM;
extern const char TEXT_DIVIDE[]  PROGMEM;
extern const char TEXT_DIVIDU[]  PROGMEM;
using common::TEXT_DIVL;
extern const char TEXT_DIVREM[]  PROGMEM;
extern const char TEXT_EMULATE[] PROGMEM;
extern const char TEXT_EXBYTE[]  PROGMEM;
extern const char TEXT_EXHW[]    PROGMEM;
extern const char TEXT_EXHWS[]   PROGMEM;
extern const char TEXT_EXTRACT[] PROGMEM;
using common::TEXT_FADD;
using common::TEXT_FDIV;
extern const char TEXT_FDMUL[]   PROGMEM;
extern const char TEXT_FEQ[]     PROGMEM;
extern const char TEXT_FGE[]     PROGMEM;
extern const char TEXT_FGT[]     PROGMEM;
extern const char TEXT_FMAC[]    PROGMEM;
extern const char TEXT_FMSM[]    PROGMEM;
using common::TEXT_FMUL;
using common::TEXT_FSUB;
using common::TEXT_HALT;
extern const char TEXT_INBYTE[]  PROGMEM;
extern const char TEXT_INHW[]    PROGMEM;
using common::TEXT_INV;
using common::TEXT_IRET;
extern const char TEXT_IRETINV[] PROGMEM;
using common::TEXT_JMP;
extern const char TEXT_JMPF[]    PROGMEM;
extern const char TEXT_JMPFDEC[] PROGMEM;
extern const char TEXT_JMPFI[]   PROGMEM;
extern const char TEXT_JMPI[]    PROGMEM;
extern const char TEXT_JMPT[]    PROGMEM;
extern const char TEXT_JMPTI[]   PROGMEM;
extern const char TEXT_LOAD[]    PROGMEM;
extern const char TEXT_LOADL[]   PROGMEM;
extern const char TEXT_LOADM[]   PROGMEM;
extern const char TEXT_LOADSET[] PROGMEM;
extern const char TEXT_MFACC[]   PROGMEM;
extern const char TEXT_MFSR[]    PROGMEM;
extern const char TEXT_MFTLB[]   PROGMEM;
extern const char TEXT_MTACC[]   PROGMEM;
extern const char TEXT_MTSR[]    PROGMEM;
extern const char TEXT_MTSRIM[]  PROGMEM;
extern const char TEXT_MTTLB[]   PROGMEM;
using common::TEXT_MUL;
extern const char TEXT_MULL[]    PROGMEM;
extern const char TEXT_MULTIPLU[] PROGMEM;
extern const char TEXT_MULTIPLY[] PROGMEM;
extern const char TEXT_MULTM[]   PROGMEM;
extern const char TEXT_MULTMU[]  PROGMEM;
using common::TEXT_MULU;
extern const char TEXT_NAND[]    PROGMEM;
extern const char TEXT_NOR[]     PROGMEM;
using common::TEXT_OR;
extern const char TEXT_ORN[]     PROGMEM;
extern const char TEXT_SETIP[]   PROGMEM;
using common::TEXT_SLL;
extern const char TEXT_SQRT[]    PROGMEM;
using common::TEXT_SRA;
using common::TEXT_SRL;
extern const char TEXT_STORE[]   PROGMEM;
extern const char TEXT_STOREL[]  PROGMEM;
extern const char TEXT_STOREM[]  PROGMEM;
using common::TEXT_SUB;
using common::TEXT_SUBC;
extern const char TEXT_SUBCS[]   PROGMEM;
extern const char TEXT_SUBCU[]   PROGMEM;
using common::TEXT_SUBR;
extern const char TEXT_SUBRC[]   PROGMEM;
extern const char TEXT_SUBRCS[]  PROGMEM;
extern const char TEXT_SUBRCU[]  PROGMEM;
extern const char TEXT_SUBRS[]   PROGMEM;
extern const char TEXT_SUBRU[]   PROGMEM;
using common::TEXT_SUBS;
extern const char TEXT_SUBU[]    PROGMEM;
using common::TEXT_XOR;
extern const char TEXT_XNOR[]    PROGMEM;

// Options
extern const char OPT_BOOL_FPU[]     PROGMEM;
extern const char OPT_DESC_FPU[]     PROGMEM;
extern const char OPT_BOOL_INTMULT[] PROGMEM;
extern const char OPT_DESC_INTMULT[] PROGMEM;

// Special-purpose registers
extern const char TEXT_REG_VAB[]  PROGMEM;
extern const char TEXT_REG_OPS[]  PROGMEM;
extern const char TEXT_REG_CPS[]  PROGMEM;
extern const char TEXT_REG_CFG[]  PROGMEM;
extern const char TEXT_REG_CHA[]  PROGMEM;
extern const char TEXT_REG_CHD[]  PROGMEM;
extern const char TEXT_REG_CHC[]  PROGMEM;
extern const char TEXT_REG_RBP[]  PROGMEM;
extern const char TEXT_REG_TMC[]  PROGMEM;
extern const char TEXT_REG_TMR[]  PROGMEM;
extern const char TEXT_REG_PC0[]  PROGMEM;
extern const char TEXT_REG_PC1[]  PROGMEM;
extern const char TEXT_REG_PC2[]  PROGMEM;
extern const char TEXT_REG_MMU[]  PROGMEM;
extern const char TEXT_REG_LRU[]  PROGMEM;
extern const char TEXT_REG_RSN[]  PROGMEM;
extern const char TEXT_REG_RMA0[] PROGMEM;
extern const char TEXT_REG_RMC0[] PROGMEM;
extern const char TEXT_REG_RMA1[] PROGMEM;
extern const char TEXT_REG_RMC1[] PROGMEM;
extern const char TEXT_REG_SPC0[] PROGMEM;
extern const char TEXT_REG_SPC1[] PROGMEM;
extern const char TEXT_REG_SPC2[] PROGMEM;
extern const char TEXT_REG_IBA0[] PROGMEM;
extern const char TEXT_REG_IBC0[] PROGMEM;
extern const char TEXT_REG_IBA1[] PROGMEM;
extern const char TEXT_REG_IBC1[] PROGMEM;
extern const char TEXT_REG_EXOP[] PROGMEM;
extern const char TEXT_REG_CIR[]  PROGMEM;
extern const char TEXT_REG_CDR[]  PROGMEM;
extern const char TEXT_REG_IPC[]  PROGMEM;
extern const char TEXT_REG_IPA[]  PROGMEM;
extern const char TEXT_REG_IPB[]  PROGMEM;
extern const char TEXT_REG_Q[]    PROGMEM;
extern const char TEXT_REG_ALU[]  PROGMEM;
extern const char TEXT_REG_BP[]   PROGMEM;
extern const char TEXT_REG_FC[]   PROGMEM;
extern const char TEXT_REG_CR[]   PROGMEM;
extern const char TEXT_REG_FPE[]  PROGMEM;
extern const char TEXT_REG_INTE[] PROGMEM;
extern const char TEXT_REG_FPS[]  PROGMEM;

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
