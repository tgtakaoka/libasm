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

#ifndef __LIBASM_TEXT_TLCS90__
#define __LIBASM_TEXT_TLCS90__

#include "text_common.h"

namespace libasm {
namespace text {
namespace tlcs90 {
// clang-format off

extern const char TEXT_CPU_TLCS90[] PROGMEM;

using common::TEXT_ADC;
using common::TEXT_ADD;
using common::TEXT_AND;
using common::TEXT_BIT;
using common::TEXT_CALL;
using common::TEXT_CALR;
using common::TEXT_CCF;
using common::TEXT_CP;
using common::TEXT_CPD;
using common::TEXT_CPDR;
using common::TEXT_CPI;
using common::TEXT_CPIR;
using common::TEXT_CPL;
using common::TEXT_DAA;
using common::TEXT_DEC;
using common::TEXT_DECW;
using common::TEXT_DECX;
using common::TEXT_DI;
using common::TEXT_DIV;
using common::TEXT_DJNZ;
using common::TEXT_EI;
using common::TEXT_EX;
using common::TEXT_EXX;
using common::TEXT_HALT;
using common::TEXT_INC;
using common::TEXT_INCW;
using common::TEXT_INCX;
using common::TEXT_JP;
using common::TEXT_JR;
extern const char TEXT_JRL[]  PROGMEM;
using common::TEXT_LD;
using common::TEXT_LDA;
using common::TEXT_LDAR;
using common::TEXT_LDD;
using common::TEXT_LDDR;
using common::TEXT_LDI;
using common::TEXT_LDIR;
using common::TEXT_LDW;
using common::TEXT_MUL;
using common::TEXT_NEG;
using common::TEXT_NOP;
using common::TEXT_OR;
using common::TEXT_POP;
using common::TEXT_PUSH;
using common::TEXT_RCF;
using common::TEXT_RES;
using common::TEXT_RET;
using common::TEXT_RETI;
using common::TEXT_RL;
using common::TEXT_RLA;
using common::TEXT_RLC;
using common::TEXT_RLCA;
using common::TEXT_RLD;
using common::TEXT_RR;
using common::TEXT_RRA;
using common::TEXT_RRC;
using common::TEXT_RRCA;
using common::TEXT_RRD;
using common::TEXT_SBC;
using common::TEXT_SCF;
using common::TEXT_SET;
using common::TEXT_SLA;
extern const char TEXT_SLAA[] PROGMEM;
using common::TEXT_SLL;
extern const char TEXT_SLLA[] PROGMEM;
using common::TEXT_SRA;
extern const char TEXT_SRAA[] PROGMEM;
using common::TEXT_SRL;
extern const char TEXT_SRLA[] PROGMEM;
using common::TEXT_SUB;
using common::TEXT_SWI;
using common::TEXT_TSET;
using common::TEXT_XOR;

using common::TEXT_REG_A;
using common::TEXT_REG_AF;
using common::TEXT_REG_B;
using common::TEXT_REG_BC;
using common::TEXT_REG_C;
using common::TEXT_REG_D;
using common::TEXT_REG_DE;
using common::TEXT_REG_E;
using common::TEXT_REG_H;
using common::TEXT_REG_HL;
using common::TEXT_REG_IX;
using common::TEXT_REG_IY;
using common::TEXT_REG_L;
using common::TEXT_REG_SP;

using common::TEXT_CC_C;
using common::TEXT_CC_EQ;
using common::TEXT_CC_F;
using common::TEXT_CC_GE;
using common::TEXT_CC_GT;
using common::TEXT_CC_LE;
using common::TEXT_CC_LT;
using common::TEXT_CC_M;
using common::TEXT_CC_MI;
using common::TEXT_CC_NC;
using common::TEXT_CC_NE;
using common::TEXT_CC_NOV;
using common::TEXT_CC_NZ;
using common::TEXT_CC_OV;
using common::TEXT_CC_P;
using common::TEXT_CC_PE;
using common::TEXT_CC_PL;
using common::TEXT_CC_PO;
using common::TEXT_CC_UGE;
using common::TEXT_CC_UGT;
using common::TEXT_CC_ULE;
using common::TEXT_CC_ULT;
using common::TEXT_CC_Z;

// clang-format on
}  // namespace tlcs90
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_TLCS90__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
