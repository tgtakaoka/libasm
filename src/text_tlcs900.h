/*
 * Copyright 2025 Tadashi G. Takaoka
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

#ifndef __LIBASM_TEXT_TLCS900__
#define __LIBASM_TEXT_TLCS900__

#include "text_common.h"

namespace libasm {
namespace text {
namespace tlcs900 {

extern const char TEXT_TLCS900_LIST[] PROGMEM;
extern const char TEXT_CPU_TLCS900[]  PROGMEM;
extern const char TEXT_CPU_TLCS900H[] PROGMEM;
extern const char TEXT_CPU_TLCS900L[] PROGMEM;

// clang-format off
extern const char TEXT_ADC[]   PROGMEM;
extern const char TEXT_ADD[]   PROGMEM;
extern const char TEXT_ANDCF[] PROGMEM;
extern const char TEXT_AND[]   PROGMEM;
extern const char TEXT_BIT[]   PROGMEM;
extern const char TEXT_BS1B[]  PROGMEM;
extern const char TEXT_BS1F[]  PROGMEM;
extern const char TEXT_CALL[]  PROGMEM;
extern const char TEXT_CALR[]  PROGMEM;
extern const char TEXT_CCF[]   PROGMEM;
extern const char TEXT_CHG[]   PROGMEM;
extern const char TEXT_CPD[]   PROGMEM;
extern const char TEXT_CPDR[]  PROGMEM;
extern const char TEXT_CPI[]   PROGMEM;
extern const char TEXT_CPIR[]  PROGMEM;
extern const char TEXT_CPL[]   PROGMEM;
extern const char TEXT_CP[]    PROGMEM;
extern const char TEXT_DAA[]   PROGMEM;
extern const char TEXT_DECF[]  PROGMEM;
extern const char TEXT_DEC[]   PROGMEM;
extern const char TEXT_DI[]    PROGMEM;
extern const char TEXT_DIV[]   PROGMEM;
extern const char TEXT_DIVS[]  PROGMEM;
extern const char TEXT_DJNZ[]  PROGMEM;
extern const char TEXT_EI[]    PROGMEM;
extern const char TEXT_EX[]    PROGMEM;
extern const char TEXT_EXTS[]  PROGMEM;
extern const char TEXT_EXTZ[]  PROGMEM;
extern const char TEXT_HALT[]  PROGMEM;
extern const char TEXT_INCF[]  PROGMEM;
extern const char TEXT_INC[]   PROGMEM;
extern const char TEXT_JP[]    PROGMEM;
extern const char TEXT_JRL[]   PROGMEM;
extern const char TEXT_JR[]    PROGMEM;
extern const char TEXT_LDA[]   PROGMEM;
extern const char TEXT_LDAR[]  PROGMEM;
extern const char TEXT_LDCF[]  PROGMEM;
extern const char TEXT_LDC[]   PROGMEM;
extern const char TEXT_LDD[]   PROGMEM;
extern const char TEXT_LDDR[]  PROGMEM;
extern const char TEXT_LDF[]   PROGMEM;
extern const char TEXT_LDI[]   PROGMEM;
extern const char TEXT_LDIR[]  PROGMEM;
extern const char TEXT_LD[]    PROGMEM;
extern const char TEXT_LDX[]   PROGMEM;
extern const char TEXT_LINK[]  PROGMEM;
extern const char TEXT_MAX[]   PROGMEM;
extern const char TEXT_MDEC1[] PROGMEM;
extern const char TEXT_MDEC2[] PROGMEM;
extern const char TEXT_MDEC4[] PROGMEM;
extern const char TEXT_MINC1[] PROGMEM;
extern const char TEXT_MINC2[] PROGMEM;
extern const char TEXT_MINC4[] PROGMEM;
extern const char TEXT_MIN[]   PROGMEM;
extern const char TEXT_MIRR[]  PROGMEM;
extern const char TEXT_MULA[]  PROGMEM;
extern const char TEXT_MUL[]   PROGMEM;
extern const char TEXT_MULS[]  PROGMEM;
extern const char TEXT_NEG[]   PROGMEM;
extern const char TEXT_NOP[]   PROGMEM;
extern const char TEXT_NORMAL[] PROGMEM;
extern const char TEXT_ORCF[]  PROGMEM;
extern const char TEXT_OR[]    PROGMEM;
extern const char TEXT_PAA[]   PROGMEM;
extern const char TEXT_POP[]   PROGMEM;
extern const char TEXT_PUSH[]  PROGMEM;
extern const char TEXT_RCF[]   PROGMEM;
extern const char TEXT_RES[]   PROGMEM;
extern const char TEXT_RETD[]  PROGMEM;
extern const char TEXT_RETI[]  PROGMEM;
extern const char TEXT_RET[]   PROGMEM;
extern const char TEXT_RLC[]   PROGMEM;
extern const char TEXT_RLD[]   PROGMEM;
extern const char TEXT_RL[]    PROGMEM;
extern const char TEXT_RRC[]   PROGMEM;
extern const char TEXT_RRD[]   PROGMEM;
extern const char TEXT_RR[]    PROGMEM;
extern const char TEXT_SBC[]   PROGMEM;
extern const char TEXT_SCC[]   PROGMEM;
extern const char TEXT_SCF[]   PROGMEM;
extern const char TEXT_SET[]   PROGMEM;
extern const char TEXT_SLA[]   PROGMEM;
extern const char TEXT_SLL[]   PROGMEM;
extern const char TEXT_SRA[]   PROGMEM;
extern const char TEXT_SRL[]   PROGMEM;
extern const char TEXT_STCF[]  PROGMEM;
extern const char TEXT_SUB[]   PROGMEM;
extern const char TEXT_SWI[]   PROGMEM;
extern const char TEXT_TSET[]  PROGMEM;
extern const char TEXT_UNLK[]  PROGMEM;
extern const char TEXT_XORCF[] PROGMEM;
extern const char TEXT_XOR[]   PROGMEM;
extern const char TEXT_ZCF[]   PROGMEM;

extern const char TEXT_REG_A[]   PROGMEM;
extern const char TEXT_REG_BC[]  PROGMEM;
extern const char TEXT_REG_B[]   PROGMEM;
extern const char TEXT_REG_C[]   PROGMEM;
extern const char TEXT_REG_DE[]  PROGMEM;
extern const char TEXT_REG_D[]   PROGMEM;
extern const char TEXT_REG_E[]   PROGMEM;
extern const char TEXT_REG_F[]   PROGMEM;
extern const char TEXT_REG_HL[]  PROGMEM;
extern const char TEXT_REG_H[]   PROGMEM;
extern const char TEXT_REG_IX[]  PROGMEM;
extern const char TEXT_REG_IY[]  PROGMEM;
extern const char TEXT_REG_IZ[]  PROGMEM;
extern const char TEXT_REG_L[]   PROGMEM;
extern const char TEXT_REG_PC[]  PROGMEM;
extern const char TEXT_REG_SP[]  PROGMEM;
extern const char TEXT_REG_SR[]  PROGMEM;
extern const char TEXT_REG_WA[]  PROGMEM;
extern const char TEXT_REG_W[]   PROGMEM;
extern const char TEXT_REG_XBC[] PROGMEM;
extern const char TEXT_REG_XDE[] PROGMEM;
extern const char TEXT_REG_XHL[] PROGMEM;
extern const char TEXT_REG_XIX[] PROGMEM;
extern const char TEXT_REG_XIY[] PROGMEM;
extern const char TEXT_REG_XIZ[] PROGMEM;
extern const char TEXT_REG_XSP[] PROGMEM;
extern const char TEXT_REG_XWA[] PROGMEM;

extern const char TEXT_CC_C[]   PROGMEM;
extern const char TEXT_CC_EQ[]  PROGMEM;
extern const char TEXT_CC_F[]   PROGMEM;
extern const char TEXT_CC_GE[]  PROGMEM;
extern const char TEXT_CC_GT[]  PROGMEM;
extern const char TEXT_CC_LE[]  PROGMEM;
extern const char TEXT_CC_LT[]  PROGMEM;
extern const char TEXT_CC_MI[]  PROGMEM;
extern const char TEXT_CC_M[]   PROGMEM;
extern const char TEXT_CC_NC[]  PROGMEM;
extern const char TEXT_CC_NE[]  PROGMEM;
extern const char TEXT_CC_NOV[] PROGMEM;
extern const char TEXT_CC_NZ[]  PROGMEM;
extern const char TEXT_CC_OV[]  PROGMEM;
extern const char TEXT_CC_PE[]  PROGMEM;
extern const char TEXT_CC_PL[]  PROGMEM;
extern const char TEXT_CC_PO[]  PROGMEM;
extern const char TEXT_CC_P[]   PROGMEM;
extern const char TEXT_CC_UGE[] PROGMEM;
extern const char TEXT_CC_UGT[] PROGMEM;
extern const char TEXT_CC_ULE[] PROGMEM;
extern const char TEXT_CC_ULT[] PROGMEM;
extern const char TEXT_CC_Z[]   PROGMEM;
// clang-format on

}  // namespace tlcs900
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_TLCS900__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
