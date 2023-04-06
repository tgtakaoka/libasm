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

#ifndef __TEXT_NS32000__
#define __TEXT_NS32000__

#include "text_common.h"

namespace libasm {
namespace text {
namespace ns32000 {

// clang-format off
extern const char TEXT_CPU_NS32032[] PROGMEM;
extern const char TEXT_FPU_NS32081[] PROGMEM;
extern const char TEXT_MMU_NS32082[] PROGMEM;
extern const char TEXT_CPU_32032[]   PROGMEM;
extern const char TEXT_none[] PROGMEM;

extern const char TEXT_FPU[]  PROGMEM;
extern const char TEXT_PMMU[] PROGMEM;

// Standard instructions
extern const char TEXT_ABSB[]    PROGMEM;
extern const char TEXT_ABSD[]    PROGMEM;
extern const char TEXT_ABSW[]    PROGMEM;
extern const char TEXT_ACBB[]    PROGMEM;
extern const char TEXT_ACBD[]    PROGMEM;
extern const char TEXT_ACBW[]    PROGMEM;
using common::TEXT_ADDB;
using common::TEXT_ADDCB;
extern const char TEXT_ADDCD[]   PROGMEM;
extern const char TEXT_ADDCW[]   PROGMEM;
using common::TEXT_ADDD;
extern const char TEXT_ADDPB[]   PROGMEM;
extern const char TEXT_ADDPD[]   PROGMEM;
extern const char TEXT_ADDPW[]   PROGMEM;
extern const char TEXT_ADDQB[]   PROGMEM;
extern const char TEXT_ADDQD[]   PROGMEM;
extern const char TEXT_ADDQW[]   PROGMEM;
using common::TEXT_ADDR;
using common::TEXT_ADDW;
extern const char TEXT_ADJSPB[]  PROGMEM;
extern const char TEXT_ADJSPD[]  PROGMEM;
extern const char TEXT_ADJSPW[]  PROGMEM;
using common::TEXT_ANDB;
using common::TEXT_ANDD;
extern const char TEXT_ANDW[]    PROGMEM;
extern const char TEXT_ASHB[]    PROGMEM;
extern const char TEXT_ASHD[]    PROGMEM;
extern const char TEXT_ASHW[]    PROGMEM;
using common::TEXT_BCC;
using common::TEXT_BCS;
using common::TEXT_BEQ;
extern const char TEXT_BFC[]     PROGMEM;
extern const char TEXT_BFS[]     PROGMEM;
using common::TEXT_BGE;
using common::TEXT_BGT;
using common::TEXT_BHI;
using common::TEXT_BHS;
extern const char TEXT_BICB[]    PROGMEM;
extern const char TEXT_BICD[]    PROGMEM;
extern const char TEXT_BICPSRB[] PROGMEM;
extern const char TEXT_BICPSRW[] PROGMEM;
extern const char TEXT_BICW[]    PROGMEM;
extern const char TEXT_BISPSRB[] PROGMEM;
extern const char TEXT_BISPSRW[] PROGMEM;
using common::TEXT_BLE;
using common::TEXT_BLO;
using common::TEXT_BLS;
using common::TEXT_BLT;
using common::TEXT_BNE;
extern const char TEXT_BPT[]     PROGMEM;
using common::TEXT_BR;
using common::TEXT_BSR;
extern const char TEXT_CASEB[]   PROGMEM;
extern const char TEXT_CASED[]   PROGMEM;
extern const char TEXT_CASEW[]   PROGMEM;
extern const char TEXT_CBITB[]   PROGMEM;
extern const char TEXT_CBITD[]   PROGMEM;
extern const char TEXT_CBITIB[]  PROGMEM;
extern const char TEXT_CBITID[]  PROGMEM;
extern const char TEXT_CBITIW[]  PROGMEM;
extern const char TEXT_CBITW[]   PROGMEM;
extern const char TEXT_CHECKB[]  PROGMEM;
extern const char TEXT_CHECKD[]  PROGMEM;
extern const char TEXT_CHECKW[]  PROGMEM;
extern const char TEXT_CINV[]    PROGMEM;
using common::TEXT_CMPB;
using common::TEXT_CMPD;
extern const char TEXT_CMPMB[]   PROGMEM;
extern const char TEXT_CMPMD[]   PROGMEM;
extern const char TEXT_CMPMW[]   PROGMEM;
extern const char TEXT_CMPQB[]   PROGMEM;
extern const char TEXT_CMPQD[]   PROGMEM;
extern const char TEXT_CMPQW[]   PROGMEM;
using common::TEXT_CMPSB;
extern const char TEXT_CMPSD[]   PROGMEM;
extern const char TEXT_CMPST[]   PROGMEM;
using common::TEXT_CMPSW;
using common::TEXT_CMPW;
using common::TEXT_COMB;
using common::TEXT_COMD;
using common::TEXT_COMW;
extern const char TEXT_CVTP[]    PROGMEM;
extern const char TEXT_CXP[]     PROGMEM;
extern const char TEXT_CXPD[]    PROGMEM;
extern const char TEXT_DEIB[]    PROGMEM;
extern const char TEXT_DEID[]    PROGMEM;
extern const char TEXT_DEIW[]    PROGMEM;
extern const char TEXT_DIA[]     PROGMEM;
using common::TEXT_DIVB;
using common::TEXT_DIVD;
extern const char TEXT_DIVW[]    PROGMEM;
using common::TEXT_ENTER;
using common::TEXT_EXIT;
using common::TEXT_EXTB;
extern const char TEXT_EXTD[]    PROGMEM;
using common::TEXT_EXTSB;
extern const char TEXT_EXTSD[]   PROGMEM;
extern const char TEXT_EXTSW[]   PROGMEM;
extern const char TEXT_EXTW[]    PROGMEM;
extern const char TEXT_FFSB[]    PROGMEM;
extern const char TEXT_FFSD[]    PROGMEM;
extern const char TEXT_FFSW[]    PROGMEM;
extern const char TEXT_FLAG[]    PROGMEM;
extern const char TEXT_IBITB[]   PROGMEM;
extern const char TEXT_IBITD[]   PROGMEM;
extern const char TEXT_IBITW[]   PROGMEM;
extern const char TEXT_INDEXB[]  PROGMEM;
extern const char TEXT_INDEXD[]  PROGMEM;
extern const char TEXT_INDEXW[]  PROGMEM;
using common::TEXT_INSB;
extern const char TEXT_INSD[]    PROGMEM;
extern const char TEXT_INSSB[]   PROGMEM;
extern const char TEXT_INSSD[]   PROGMEM;
extern const char TEXT_INSSW[]   PROGMEM;
using common::TEXT_INSW;
using common::TEXT_JSR;
extern const char TEXT_JUMP[]    PROGMEM;
extern const char TEXT_LPRB[]    PROGMEM;
extern const char TEXT_LPRD[]    PROGMEM;
extern const char TEXT_LPRW[]    PROGMEM;
extern const char TEXT_LFSR[]    PROGMEM;
extern const char TEXT_LSHB[]    PROGMEM;
extern const char TEXT_LSHD[]    PROGMEM;
extern const char TEXT_LSHW[]    PROGMEM;
extern const char TEXT_MEIB[]    PROGMEM;
extern const char TEXT_MEID[]    PROGMEM;
extern const char TEXT_MEIW[]    PROGMEM;
extern const char TEXT_MODB[]    PROGMEM;
extern const char TEXT_MODD[]    PROGMEM;
extern const char TEXT_MODW[]    PROGMEM;
using common::TEXT_MOVB;
using common::TEXT_MOVD;
extern const char TEXT_MOVMB[]   PROGMEM;
extern const char TEXT_MOVMD[]   PROGMEM;
extern const char TEXT_MOVMW[]   PROGMEM;
extern const char TEXT_MOVQB[]   PROGMEM;
extern const char TEXT_MOVQD[]   PROGMEM;
extern const char TEXT_MOVQW[]   PROGMEM;
using common::TEXT_MOVSB;
extern const char TEXT_MOVSD[]   PROGMEM;
extern const char TEXT_MOVST[]   PROGMEM;
using common::TEXT_MOVSW;
extern const char TEXT_MOVW[]    PROGMEM;
extern const char TEXT_MOVXBD[]  PROGMEM;
extern const char TEXT_MOVXBW[]  PROGMEM;
extern const char TEXT_MOVXWD[]  PROGMEM;
extern const char TEXT_MOVZBD[]  PROGMEM;
extern const char TEXT_MOVZBW[]  PROGMEM;
extern const char TEXT_MOVZWD[]  PROGMEM;
using common::TEXT_MULB;
using common::TEXT_MULD;
extern const char TEXT_MULW[]    PROGMEM;
using common::TEXT_NEGB;
using common::TEXT_NEGD;
extern const char TEXT_NEGW[]    PROGMEM;
using common::TEXT_NOP;
using common::TEXT_NOTB;
extern const char TEXT_NOTD[]    PROGMEM;
extern const char TEXT_NOTW[]    PROGMEM;
using common::TEXT_ORB;
using common::TEXT_ORD;
extern const char TEXT_ORW[]     PROGMEM;
extern const char TEXT_QUOB[]    PROGMEM;
extern const char TEXT_QUOD[]    PROGMEM;
extern const char TEXT_QUOW[]    PROGMEM;
extern const char TEXT_REMB[]    PROGMEM;
extern const char TEXT_REMD[]    PROGMEM;
extern const char TEXT_REMW[]    PROGMEM;
extern const char TEXT_RESTORE[] PROGMEM;
using common::TEXT_RET;
using common::TEXT_RETI;
extern const char TEXT_RETT[]    PROGMEM;
extern const char TEXT_ROTB[]    PROGMEM;
extern const char TEXT_ROTD[]    PROGMEM;
extern const char TEXT_ROTW[]    PROGMEM;
extern const char TEXT_RXP[]     PROGMEM;
extern const char TEXT_SAVE[]    PROGMEM;
extern const char TEXT_SBITB[]   PROGMEM;
extern const char TEXT_SBITD[]   PROGMEM;
extern const char TEXT_SBITIB[]  PROGMEM;
extern const char TEXT_SBITID[]  PROGMEM;
extern const char TEXT_SBITIW[]  PROGMEM;
extern const char TEXT_SBITW[]   PROGMEM;
extern const char TEXT_SCCB[]    PROGMEM;
extern const char TEXT_SCCD[]    PROGMEM;
extern const char TEXT_SCCW[]    PROGMEM;
extern const char TEXT_SCSB[]    PROGMEM;
extern const char TEXT_SCSD[]    PROGMEM;
extern const char TEXT_SCSW[]    PROGMEM;
extern const char TEXT_SEQB[]    PROGMEM;
extern const char TEXT_SEQD[]    PROGMEM;
extern const char TEXT_SEQW[]    PROGMEM;
extern const char TEXT_SETCFG[]  PROGMEM;
extern const char TEXT_SFCB[]    PROGMEM;
extern const char TEXT_SFCD[]    PROGMEM;
extern const char TEXT_SFCW[]    PROGMEM;
extern const char TEXT_SFSB[]    PROGMEM;
extern const char TEXT_SFSD[]    PROGMEM;
extern const char TEXT_SFSR[]    PROGMEM;
extern const char TEXT_SFSW[]    PROGMEM;
extern const char TEXT_SGEB[]    PROGMEM;
extern const char TEXT_SGED[]    PROGMEM;
extern const char TEXT_SGEW[]    PROGMEM;
extern const char TEXT_SGTB[]    PROGMEM;
extern const char TEXT_SGTD[]    PROGMEM;
extern const char TEXT_SGTW[]    PROGMEM;
extern const char TEXT_SHIB[]    PROGMEM;
extern const char TEXT_SHID[]    PROGMEM;
extern const char TEXT_SHIW[]    PROGMEM;
extern const char TEXT_SHSB[]    PROGMEM;
extern const char TEXT_SHSD[]    PROGMEM;
extern const char TEXT_SHSW[]    PROGMEM;
extern const char TEXT_SKPSB[]   PROGMEM;
extern const char TEXT_SKPSD[]   PROGMEM;
extern const char TEXT_SKPST[]   PROGMEM;
extern const char TEXT_SKPSW[]   PROGMEM;
extern const char TEXT_SLEB[]    PROGMEM;
extern const char TEXT_SLED[]    PROGMEM;
extern const char TEXT_SLEW[]    PROGMEM;
extern const char TEXT_SLOB[]    PROGMEM;
extern const char TEXT_SLOD[]    PROGMEM;
extern const char TEXT_SLOW[]    PROGMEM;
extern const char TEXT_SLSB[]    PROGMEM;
extern const char TEXT_SLSD[]    PROGMEM;
extern const char TEXT_SLSW[]    PROGMEM;
extern const char TEXT_SLTB[]    PROGMEM;
extern const char TEXT_SLTD[]    PROGMEM;
extern const char TEXT_SLTW[]    PROGMEM;
extern const char TEXT_SNEB[]    PROGMEM;
extern const char TEXT_SNED[]    PROGMEM;
extern const char TEXT_SNEW[]    PROGMEM;
extern const char TEXT_SPRB[]    PROGMEM;
extern const char TEXT_SPRD[]    PROGMEM;
extern const char TEXT_SPRW[]    PROGMEM;
using common::TEXT_SUBB;
using common::TEXT_SUBCB;
extern const char TEXT_SUBCD[]   PROGMEM;
extern const char TEXT_SUBCW[]   PROGMEM;
using common::TEXT_SUBD;
extern const char TEXT_SUBPB[]   PROGMEM;
extern const char TEXT_SUBPD[]   PROGMEM;
extern const char TEXT_SUBPW[]   PROGMEM;
using common::TEXT_SUBW;
using common::TEXT_SVC;
extern const char TEXT_TBITB[]   PROGMEM;
extern const char TEXT_TBITD[]   PROGMEM;
extern const char TEXT_TBITW[]   PROGMEM;
using common::TEXT_WAIT;
using common::TEXT_XORB;
extern const char TEXT_XORD[]    PROGMEM;
extern const char TEXT_XORW[]    PROGMEM;
// Floating point instructions
extern const char TEXT_ABSF[]    PROGMEM;
extern const char TEXT_ABSL[]    PROGMEM;
using common::TEXT_ADDF;
using common::TEXT_ADDL;
using common::TEXT_CMPF;
using common::TEXT_CMPL;
extern const char TEXT_DIVF[]    PROGMEM;
using common::TEXT_DIVL;
extern const char TEXT_FLOORFB[] PROGMEM;
extern const char TEXT_FLOORFD[] PROGMEM;
extern const char TEXT_FLOORFW[] PROGMEM;
extern const char TEXT_FLOORLB[] PROGMEM;
extern const char TEXT_FLOORLD[] PROGMEM;
extern const char TEXT_FLOORLW[] PROGMEM;
extern const char TEXT_MOVBF[]   PROGMEM;
extern const char TEXT_MOVBL[]   PROGMEM;
extern const char TEXT_MOVDF[]   PROGMEM;
extern const char TEXT_MOVDL[]   PROGMEM;
extern const char TEXT_MOVF[]    PROGMEM;
extern const char TEXT_MOVFL[]   PROGMEM;
extern const char TEXT_MOVL[]    PROGMEM;
extern const char TEXT_MOVLF[]   PROGMEM;
extern const char TEXT_MOVWF[]   PROGMEM;
extern const char TEXT_MOVWL[]   PROGMEM;
extern const char TEXT_MULF[]    PROGMEM;
extern const char TEXT_MULL[]    PROGMEM;
extern const char TEXT_NEGF[]    PROGMEM;
extern const char TEXT_NEGL[]    PROGMEM;
extern const char TEXT_ROUNDFB[] PROGMEM;
extern const char TEXT_ROUNDFD[] PROGMEM;
extern const char TEXT_ROUNDFW[] PROGMEM;
extern const char TEXT_ROUNDLB[] PROGMEM;
extern const char TEXT_ROUNDLD[] PROGMEM;
extern const char TEXT_ROUNDLW[] PROGMEM;
using common::TEXT_SUBF;
using common::TEXT_SUBL;
extern const char TEXT_TRUNCFB[] PROGMEM;
extern const char TEXT_TRUNCFD[] PROGMEM;
extern const char TEXT_TRUNCFW[] PROGMEM;
extern const char TEXT_TRUNCLB[] PROGMEM;
extern const char TEXT_TRUNCLD[] PROGMEM;
extern const char TEXT_TRUNCLW[] PROGMEM;
// Memory management instructions
extern const char TEXT_LMR[]     PROGMEM;
extern const char TEXT_MOVSUB[]  PROGMEM;
extern const char TEXT_MOVSUD[]  PROGMEM;
extern const char TEXT_MOVSUW[]  PROGMEM;
extern const char TEXT_MOVUSB[]  PROGMEM;
extern const char TEXT_MOVUSD[]  PROGMEM;
extern const char TEXT_MOVUSW[]  PROGMEM;
extern const char TEXT_RDVAL[]   PROGMEM;
extern const char TEXT_SMR[]     PROGMEM;
extern const char TEXT_WRVAL[]   PROGMEM;

using common::TEXT_REG_R0;
using common::TEXT_REG_R1;
using common::TEXT_REG_R2;
using common::TEXT_REG_R3;
using common::TEXT_REG_R4;
using common::TEXT_REG_R5;
using common::TEXT_REG_R6;
using common::TEXT_REG_R7;

extern const char TEXT_REG_FP[]  PROGMEM;
using common::TEXT_REG_SP;
extern const char TEXT_REG_SB[]  PROGMEM;
using common::TEXT_REG_PC;
extern const char TEXT_REG_TOS[] PROGMEM;
extern const char TEXT_REG_EXT[] PROGMEM;

// NS32081/FPU Registers
using common::TEXT_REG_F0;
using common::TEXT_REG_F1;
extern const char TEXT_REG_F2[] PROGMEM;
extern const char TEXT_REG_F3[] PROGMEM;
extern const char TEXT_REG_F4[] PROGMEM;
extern const char TEXT_REG_F5[] PROGMEM;
extern const char TEXT_REG_F6[] PROGMEM;
extern const char TEXT_REG_F7[] PROGMEM;

extern const char TEXT_PREG_UPSR[]    PROGMEM;
extern const char TEXT_PREG_US[]      PROGMEM;
extern const char TEXT_PREG_PSR[]     PROGMEM;
extern const char TEXT_PREG_INTBASE[] PROGMEM;
extern const char TEXT_PREG_MOD[]     PROGMEM;

// NS32082/MMU registers.
extern const char TEXT_MREG_BPR0[] PROGMEM;
extern const char TEXT_MREG_BPR1[] PROGMEM;
extern const char TEXT_MREG_MSR[]  PROGMEM;
extern const char TEXT_MREG_BCNT[] PROGMEM;
extern const char TEXT_MREG_PTB0[] PROGMEM;
extern const char TEXT_MREG_PTB1[] PROGMEM;
extern const char TEXT_MREG_EIA[]  PROGMEM;

extern const char TEXT_CONFIG_I[] PROGMEM;
extern const char TEXT_CONFIG_F[] PROGMEM;
extern const char TEXT_CONFIG_M[] PROGMEM;
extern const char TEXT_CONFIG_C[] PROGMEM;

extern const char TEXT_STROPT_B[] PROGMEM;
extern const char TEXT_STROPT_W[] PROGMEM;
extern const char TEXT_STROPT_U[] PROGMEM;
// clang-format on

}  // namespace ns32000
}  // namespace text
}  // namespace libasm

#endif  // __TEXT_NS32000__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
