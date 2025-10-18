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

#ifndef __LIBASM_TEXT_Z280__
#define __LIBASM_TEXT_Z280__

#include "config_z280.h"
#include "text_common.h"

namespace libasm {
namespace text {
namespace z280 {

// clang-format off
extern const char TEXT_Z280_LIST[] PROGMEM;
using common::TEXT_CPU_Z80;
extern const char TEXT_CPU_Z280[] PROGMEM;

using common::TEXT_ADC;
using common::TEXT_ADD;
using common::TEXT_AND;
using common::TEXT_BIT;
using common::TEXT_CALL;
using common::TEXT_CCF;
using common::TEXT_CP;
using common::TEXT_CPD;
using common::TEXT_CPDR;
using common::TEXT_CPI;
using common::TEXT_CPIR;
using common::TEXT_CPL;
using common::TEXT_DAA;
using common::TEXT_DEC;
using common::TEXT_DI;
using common::TEXT_DJNZ;
using common::TEXT_EI;
using common::TEXT_EX;
using common::TEXT_EXX;
using common::TEXT_HALT;
using common::TEXT_IM;
using common::TEXT_IN;
using common::TEXT_INC;
using common::TEXT_IND;
using common::TEXT_INDR;
using common::TEXT_INI;
using common::TEXT_INIR;
using common::TEXT_JP;
using common::TEXT_JR;
using common::TEXT_LD;
using common::TEXT_LDD;
using common::TEXT_LDDR;
using common::TEXT_LDI;
using common::TEXT_LDIR;
using common::TEXT_NEG;
using common::TEXT_NOP;
using common::TEXT_OR;
using common::TEXT_OTDR;
using common::TEXT_OTIR;
using common::TEXT_OUT;
using common::TEXT_OUTD;
using common::TEXT_OUTI;
using common::TEXT_POP;
using common::TEXT_PUSH;
using common::TEXT_RER;
using common::TEXT_RES;
using common::TEXT_RET;
using common::TEXT_RETI;
using common::TEXT_RETN;
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
using common::TEXT_RST;
using common::TEXT_SBC;
using common::TEXT_SCF;
using common::TEXT_SET;
using common::TEXT_SLA;
using common::TEXT_SRA;
using common::TEXT_SRL;
using common::TEXT_SUB;
using common::TEXT_XOR;

// Z280
using common::TEXT_ADDW;
using common::TEXT_CPW;
using common::TEXT_DECW;
using common::TEXT_DIV;
using common::TEXT_DIVU;
using common::TEXT_DIVUW;
using common::TEXT_DIVW;
#if defined(LIBASM_Z280_EPU)
extern const char TEXT_EPUF[]   PROGMEM;
extern const char TEXT_EPUI[]   PROGMEM;
extern const char TEXT_EPUM[]   PROGMEM;
#endif
using common::TEXT_EXTS;
using common::TEXT_INCW;
using common::TEXT_INDRW;
using common::TEXT_INDW;
using common::TEXT_INIRW;
using common::TEXT_INIW;
using common::TEXT_INW;
extern const char TEXT_JAF[]    PROGMEM;
extern const char TEXT_JAR[]    PROGMEM;
using common::TEXT_LDA;
using common::TEXT_LDCTL;
extern const char TEXT_LDUD[]   PROGMEM;
extern const char TEXT_LDUP[]   PROGMEM;
using common::TEXT_LDW;
#if defined(LIBASM_Z280_EPU)
extern const char TEXT_MEPU[]   PROGMEM;
#endif
using common::TEXT_MULT;
extern const char TEXT_MULTU[]  PROGMEM;
using common::TEXT_MULTUW;
using common::TEXT_MULTW;
using common::TEXT_OTDRW;
using common::TEXT_OTIRW;
using common::TEXT_OUTDW;
using common::TEXT_OUTIW;
using common::TEXT_OUTW;
extern const char TEXT_PCACHE[] PROGMEM;
extern const char TEXT_RETIL[]  PROGMEM;
using common::TEXT_SC;
using common::TEXT_SUBW;
using common::TEXT_TSET;
extern const char TEXT_TSTI[]   PROGMEM;

// Z80
using common::TEXT_REG_BC;
using common::TEXT_REG_DE;
using common::TEXT_REG_HL;
using common::TEXT_REG_IX;
using common::TEXT_REG_IY;
using common::TEXT_REG_SP;
using common::TEXT_REG_AF;
using common::TEXT_REG_A;
using common::TEXT_REG_B;
using common::TEXT_REG_C;
using common::TEXT_REG_D;
using common::TEXT_REG_E;
using common::TEXT_REG_H;
using common::TEXT_REG_L;
using common::TEXT_REG_R;
using common::TEXT_REG_I;

// Z280
extern const char TEXT_REG_DEHL[] PROGMEM;
extern const char TEXT_REG_IXH[]  PROGMEM;
using common::TEXT_REG_IXL;
extern const char TEXT_REG_IYH[]  PROGMEM;
using common::TEXT_REG_IYL;
using common::TEXT_REG_USP;

// Z80
using common::TEXT_CC_NZ;
using common::TEXT_CC_Z;
using common::TEXT_CC_NC;
using common::TEXT_CC_C;
using common::TEXT_CC_PO;
using common::TEXT_CC_PE;
using common::TEXT_CC_P;
using common::TEXT_CC_M;

// Z280
using common::TEXT_CC_NS;
using common::TEXT_CC_NV;
using common::TEXT_CC_S;
using common::TEXT_CC_V;

// clang-format on

}  // namespace z280
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_Z280__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
