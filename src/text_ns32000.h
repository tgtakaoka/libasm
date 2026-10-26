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

#ifndef __LIBASM_TEXT_NS32000__
#define __LIBASM_TEXT_NS32000__

#include "text_common.h"

namespace libasm {
namespace text {
namespace ns32000 {
// clang-format off

extern const char TEXT_CPU_NS32032[];
extern const char TEXT_FPU_NS32081[];
extern const char TEXT_PMMU_NS32082[];
extern const char TEXT_CPU_32032[];
using common::TEXT_none;

using common::TEXT_FPU;
using common::TEXT_PMMU;

// Standard instructions
extern const char TEXT_ABSB[];
using common::TEXT_ABSD;
extern const char TEXT_ABSW[];
extern const char TEXT_ACBB[];
extern const char TEXT_ACBD[];
extern const char TEXT_ACBW[];
using common::TEXT_ADDB;
using common::TEXT_ADDCB;
extern const char TEXT_ADDCD[];
extern const char TEXT_ADDCW[];
using common::TEXT_ADDD;
extern const char TEXT_ADDPB[];
extern const char TEXT_ADDPD[];
extern const char TEXT_ADDPW[];
extern const char TEXT_ADDQB[];
extern const char TEXT_ADDQD[];
extern const char TEXT_ADDQW[];
using common::TEXT_ADDR;
using common::TEXT_ADDW;
extern const char TEXT_ADJSPB[];
extern const char TEXT_ADJSPD[];
extern const char TEXT_ADJSPW[];
using common::TEXT_ANDB;
using common::TEXT_ANDD;
using common::TEXT_ANDW;
extern const char TEXT_ASHB[];
extern const char TEXT_ASHD[];
extern const char TEXT_ASHW[];
using common::TEXT_BCC;
using common::TEXT_BCS;
using common::TEXT_BEQ;
extern const char TEXT_BFC[];
extern const char TEXT_BFS[];
using common::TEXT_BGE;
using common::TEXT_BGT;
using common::TEXT_BHI;
using common::TEXT_BHS;
using common::TEXT_BICB;
extern const char TEXT_BICD[];
extern const char TEXT_BICPSRB[];
extern const char TEXT_BICPSRW[];
extern const char TEXT_BICW[];
extern const char TEXT_BISPSRB[];
extern const char TEXT_BISPSRW[];
using common::TEXT_BLE;
using common::TEXT_BLO;
using common::TEXT_BLS;
using common::TEXT_BLT;
using common::TEXT_BNE;
using common::TEXT_BPT;
using common::TEXT_BR;
using common::TEXT_BSR;
extern const char TEXT_CASEB[];
extern const char TEXT_CASED[];
extern const char TEXT_CASEW[];
extern const char TEXT_CBITB[];
extern const char TEXT_CBITD[];
extern const char TEXT_CBITIB[];
extern const char TEXT_CBITID[];
extern const char TEXT_CBITIW[];
extern const char TEXT_CBITW[];
extern const char TEXT_CHECKB[];
extern const char TEXT_CHECKD[];
extern const char TEXT_CHECKW[];
extern const char TEXT_CINV[];
using common::TEXT_CMPB;
using common::TEXT_CMPD;
extern const char TEXT_CMPMB[];
extern const char TEXT_CMPMD[];
extern const char TEXT_CMPMW[];
extern const char TEXT_CMPQB[];
extern const char TEXT_CMPQD[];
extern const char TEXT_CMPQW[];
using common::TEXT_CMPSB;
using common::TEXT_CMPSD;
extern const char TEXT_CMPST[];
using common::TEXT_CMPSW;
using common::TEXT_CMPW;
using common::TEXT_COMB;
using common::TEXT_COMD;
using common::TEXT_COMW;
extern const char TEXT_CVTP[];
extern const char TEXT_CXP[];
extern const char TEXT_CXPD[];
extern const char TEXT_DEIB[];
extern const char TEXT_DEID[];
extern const char TEXT_DEIW[];
extern const char TEXT_DIA[];
using common::TEXT_DIVB;
using common::TEXT_DIVD;
using common::TEXT_DIVW;
using common::TEXT_ENTER;
using common::TEXT_EXIT;
using common::TEXT_EXTB;
extern const char TEXT_EXTD[];
using common::TEXT_EXTSB;
extern const char TEXT_EXTSD[];
using common::TEXT_EXTSW;
extern const char TEXT_EXTW[];
extern const char TEXT_FFSB[];
extern const char TEXT_FFSD[];
extern const char TEXT_FFSW[];
extern const char TEXT_FLAG[];
extern const char TEXT_IBITB[];
extern const char TEXT_IBITD[];
extern const char TEXT_IBITW[];
extern const char TEXT_INDEXB[];
extern const char TEXT_INDEXD[];
extern const char TEXT_INDEXW[];
using common::TEXT_INSB;
using common::TEXT_INSD;
extern const char TEXT_INSSB[];
extern const char TEXT_INSSD[];
extern const char TEXT_INSSW[];
using common::TEXT_INSW;
using common::TEXT_JSR;
extern const char TEXT_JUMP[];
extern const char TEXT_LFSR[];
extern const char TEXT_LPRB[];
extern const char TEXT_LPRD[];
extern const char TEXT_LPRW[];
extern const char TEXT_LSHB[];
extern const char TEXT_LSHD[];
extern const char TEXT_LSHW[];
extern const char TEXT_MEIB[];
extern const char TEXT_MEID[];
extern const char TEXT_MEIW[];
extern const char TEXT_MODB[];
using common::TEXT_MODD;
extern const char TEXT_MODW[];
using common::TEXT_MOVB;
using common::TEXT_MOVD;
extern const char TEXT_MOVMB[];
extern const char TEXT_MOVMD[];
extern const char TEXT_MOVMW[];
extern const char TEXT_MOVQB[];
extern const char TEXT_MOVQD[];
extern const char TEXT_MOVQW[];
using common::TEXT_MOVSB;
using common::TEXT_MOVSD;
extern const char TEXT_MOVST[];
using common::TEXT_MOVSW;
using common::TEXT_MOVW;
extern const char TEXT_MOVXBD[];
extern const char TEXT_MOVXBW[];
extern const char TEXT_MOVXWD[];
extern const char TEXT_MOVZBD[];
extern const char TEXT_MOVZBW[];
extern const char TEXT_MOVZWD[];
using common::TEXT_MULB;
using common::TEXT_MULD;
extern const char TEXT_MULW[];
using common::TEXT_NEGB;
using common::TEXT_NEGD;
using common::TEXT_NEGW;
using common::TEXT_NOP;
using common::TEXT_NOTB;
extern const char TEXT_NOTD[];
extern const char TEXT_NOTW[];
using common::TEXT_ORB;
using common::TEXT_ORD;
using common::TEXT_ORW;
extern const char TEXT_QUOB[];
extern const char TEXT_QUOD[];
extern const char TEXT_QUOW[];
extern const char TEXT_REMB[];
extern const char TEXT_REMD[];
extern const char TEXT_REMW[];
extern const char TEXT_RESTORE[];
using common::TEXT_RET;
using common::TEXT_RETI;
extern const char TEXT_RETT[];
extern const char TEXT_ROTB[];
extern const char TEXT_ROTD[];
extern const char TEXT_ROTW[];
extern const char TEXT_RXP[];
extern const char TEXT_SAVE[];
extern const char TEXT_SBITB[];
extern const char TEXT_SBITD[];
extern const char TEXT_SBITIB[];
extern const char TEXT_SBITID[];
extern const char TEXT_SBITIW[];
extern const char TEXT_SBITW[];
extern const char TEXT_SCCB[];
extern const char TEXT_SCCD[];
extern const char TEXT_SCCW[];
extern const char TEXT_SCSB[];
extern const char TEXT_SCSD[];
extern const char TEXT_SCSW[];
extern const char TEXT_SEQB[];
extern const char TEXT_SEQD[];
extern const char TEXT_SEQW[];
extern const char TEXT_SETCFG[];
extern const char TEXT_SFCB[];
extern const char TEXT_SFCD[];
extern const char TEXT_SFCW[];
extern const char TEXT_SFSB[];
extern const char TEXT_SFSD[];
extern const char TEXT_SFSR[];
extern const char TEXT_SFSW[];
extern const char TEXT_SGEB[];
extern const char TEXT_SGED[];
extern const char TEXT_SGEW[];
extern const char TEXT_SGTB[];
extern const char TEXT_SGTD[];
extern const char TEXT_SGTW[];
extern const char TEXT_SHIB[];
extern const char TEXT_SHID[];
extern const char TEXT_SHIW[];
extern const char TEXT_SHSB[];
extern const char TEXT_SHSD[];
extern const char TEXT_SHSW[];
extern const char TEXT_SKPSB[];
extern const char TEXT_SKPSD[];
extern const char TEXT_SKPST[];
extern const char TEXT_SKPSW[];
extern const char TEXT_SLEB[];
extern const char TEXT_SLED[];
extern const char TEXT_SLEW[];
extern const char TEXT_SLOB[];
extern const char TEXT_SLOD[];
extern const char TEXT_SLOW[];
extern const char TEXT_SLSB[];
extern const char TEXT_SLSD[];
extern const char TEXT_SLSW[];
extern const char TEXT_SLTB[];
extern const char TEXT_SLTD[];
extern const char TEXT_SLTW[];
extern const char TEXT_SNEB[];
extern const char TEXT_SNED[];
extern const char TEXT_SNEW[];
extern const char TEXT_SPRB[];
extern const char TEXT_SPRD[];
extern const char TEXT_SPRW[];
using common::TEXT_SUBB;
using common::TEXT_SUBCB;
extern const char TEXT_SUBCD[];
extern const char TEXT_SUBCW[];
using common::TEXT_SUBD;
extern const char TEXT_SUBPB[];
extern const char TEXT_SUBPD[];
extern const char TEXT_SUBPW[];
using common::TEXT_SUBW;
using common::TEXT_SVC;
extern const char TEXT_TBITB[];
extern const char TEXT_TBITD[];
extern const char TEXT_TBITW[];
using common::TEXT_WAIT;
using common::TEXT_XORB;
extern const char TEXT_XORD[];
using common::TEXT_XORW;

// NS32081
using common::TEXT_ABSF;
extern const char TEXT_ABSL[];
using common::TEXT_ADDF;
using common::TEXT_ADDL;
using common::TEXT_CMPF;
using common::TEXT_CMPL;
using common::TEXT_DIVF;
using common::TEXT_DIVL;
extern const char TEXT_FLOORFB[];
extern const char TEXT_FLOORFD[];
extern const char TEXT_FLOORFW[];
extern const char TEXT_FLOORLB[];
extern const char TEXT_FLOORLD[];
extern const char TEXT_FLOORLW[];
extern const char TEXT_MOVBF[];
extern const char TEXT_MOVBL[];
extern const char TEXT_MOVDF[];
extern const char TEXT_MOVDL[];
extern const char TEXT_MOVF[];
extern const char TEXT_MOVFL[];
extern const char TEXT_MOVL[];
extern const char TEXT_MOVLF[];
extern const char TEXT_MOVWF[];
extern const char TEXT_MOVWL[];
using common::TEXT_MULF;
extern const char TEXT_MULL[];
using common::TEXT_NEGF;
extern const char TEXT_NEGL[];
extern const char TEXT_ROUNDFB[];
extern const char TEXT_ROUNDFD[];
extern const char TEXT_ROUNDFW[];
extern const char TEXT_ROUNDLB[];
extern const char TEXT_ROUNDLD[];
extern const char TEXT_ROUNDLW[];
using common::TEXT_SUBF;
using common::TEXT_SUBL;
extern const char TEXT_TRUNCFB[];
extern const char TEXT_TRUNCFD[];
extern const char TEXT_TRUNCFW[];
extern const char TEXT_TRUNCLB[];
extern const char TEXT_TRUNCLD[];
extern const char TEXT_TRUNCLW[];

// NS32082
extern const char TEXT_LMR[];
extern const char TEXT_MOVSUB[];
extern const char TEXT_MOVSUD[];
extern const char TEXT_MOVSUW[];
extern const char TEXT_MOVUSB[];
extern const char TEXT_MOVUSD[];
extern const char TEXT_MOVUSW[];
extern const char TEXT_RDVAL[];
extern const char TEXT_SMR[];
extern const char TEXT_WRVAL[];

extern const char TEXT_REG_EXT[];
using common::TEXT_REG_FP;
using common::TEXT_REG_PC;
using common::TEXT_REG_R0;
using common::TEXT_REG_R1;
using common::TEXT_REG_R2;
using common::TEXT_REG_R3;
using common::TEXT_REG_R4;
using common::TEXT_REG_R5;
using common::TEXT_REG_R6;
using common::TEXT_REG_R7;
extern const char TEXT_REG_SB[];
using common::TEXT_REG_SP;
extern const char TEXT_REG_TOS[];

// NS32081
using common::TEXT_REG_F0;
using common::TEXT_REG_F1;
extern const char TEXT_REG_F2[];
extern const char TEXT_REG_F3[];
extern const char TEXT_REG_F4[];
extern const char TEXT_REG_F5[];
extern const char TEXT_REG_F6[];
extern const char TEXT_REG_F7[];
extern const char TEXT_REG_INTBASE[];
using common::TEXT_REG_MOD;
using common::TEXT_REG_PSR;
extern const char TEXT_REG_UPSR[];
extern const char TEXT_REG_US[];

// NS32082
extern const char TEXT_REG_BCNT[];
extern const char TEXT_REG_BPR0[];
extern const char TEXT_REG_BPR1[];
extern const char TEXT_REG_EIA[];
extern const char TEXT_REG_MSR[];
extern const char TEXT_REG_PTB0[];
extern const char TEXT_REG_PTB1[];

extern const char TEXT_CONFIG_C[];
extern const char TEXT_CONFIG_F[];
extern const char TEXT_CONFIG_I[];
extern const char TEXT_CONFIG_M[];

// clang-format on
}  // namespace ns32000
}  // namespace text
}  // namespace libasm

#endif  // __LIBASM_TEXT_NS32000__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
