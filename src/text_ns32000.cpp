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

#include "text_ns32000.h"

namespace libasm {
namespace text {
namespace ns32000 {
// clang-format off

constexpr char TEXT_CPU_NS32032[]  = "NS32032";
constexpr char TEXT_FPU_NS32081[]  = "NS32081";
constexpr char TEXT_PMMU_NS32082[] = "NS32082";
constexpr char TEXT_CPU_32032[]    = "32032";

// NS32032
constexpr char TEXT_ABSB[]    = "ABSB";
// constexpr char TEXT_ABSD[]  = "ABSD";
constexpr char TEXT_ABSW[]    = "ABSW";
constexpr char TEXT_ACBB[]    = "ACBB";
constexpr char TEXT_ACBD[]    = "ACBD";
constexpr char TEXT_ACBW[]    = "ACBW";
// constexpr char TEXT_ADDB[]  = "ADDB";
// constexpr char TEXT_ADDCB[] = "ADDCB";
constexpr char TEXT_ADDCD[]   = "ADDCD";
constexpr char TEXT_ADDCW[]   = "ADDCW";
// constexpr char TEXT_ADDD[]  = "ADDD";
constexpr char TEXT_ADDPB[]   = "ADDPB";
constexpr char TEXT_ADDPD[]   = "ADDPD";
constexpr char TEXT_ADDPW[]   = "ADDPW";
constexpr char TEXT_ADDQB[]   = "ADDQB";
constexpr char TEXT_ADDQD[]   = "ADDQD";
constexpr char TEXT_ADDQW[]   = "ADDQW";
// constexpr char TEXT_ADDR[]  = "ADDR";
// constexpr char TEXT_ADDW[]  = "ADDW";
constexpr char TEXT_ADJSPB[]  = "ADJSPB";
constexpr char TEXT_ADJSPD[]  = "ADJSPD";
constexpr char TEXT_ADJSPW[]  = "ADJSPW";
// constexpr char TEXT_ANDB[]  = "ANDB";
// constexpr char TEXT_ANDD[]  = "ANDD";
// constexpr char TEXT_ANDW[]  = "ANDW";
constexpr char TEXT_ASHB[]    = "ASHB";
constexpr char TEXT_ASHD[]    = "ASHD";
constexpr char TEXT_ASHW[]    = "ASHW";
// constexpr char TEXT_BCC[]   = "BCC";
// constexpr char TEXT_BCS[]   = "BCS";
// constexpr char TEXT_BEQ[]   = "BEQ";
constexpr char TEXT_BFC[]     = "BFC";
constexpr char TEXT_BFS[]     = "BFS";
// constexpr char TEXT_BGE[]   = "BGE";
// constexpr char TEXT_BGT[]   = "BGT";
// constexpr char TEXT_BHI[]   = "BHI";
// constexpr char TEXT_BHS[]   = "BHS";
// constexpr char TEXT_BICB[]  = "BICB";
constexpr char TEXT_BICD[]    = "BICD";
constexpr char TEXT_BICPSRB[] = "BICPSRB";
constexpr char TEXT_BICPSRW[] = "BICPSRW";
constexpr char TEXT_BICW[]    = "BICW";
constexpr char TEXT_BISPSRB[] = "BISPSRB";
constexpr char TEXT_BISPSRW[] = "BISPSRW";
// constexpr char TEXT_BLE[]   = "BLE";
// constexpr char TEXT_BLO[]   = "BLO";
// constexpr char TEXT_BLS[]   = "BLS";
// constexpr char TEXT_BLT[]   = "BLT";
// constexpr char TEXT_BNE[]   = "BNE";
// constexpr char TEXT_BPT[]   = "BPT";
// constexpr char TEXT_BR[]    = "BR";
// constexpr char TEXT_BSR[]   = "BSR";
constexpr char TEXT_CASEB[]   = "CASEB";
constexpr char TEXT_CASED[]   = "CASED";
constexpr char TEXT_CASEW[]   = "CASEW";
constexpr char TEXT_CBITB[]   = "CBITB";
constexpr char TEXT_CBITD[]   = "CBITD";
constexpr char TEXT_CBITIB[]  = "CBITIB";
constexpr char TEXT_CBITID[]  = "CBITID";
constexpr char TEXT_CBITIW[]  = "CBITIW";
constexpr char TEXT_CBITW[]   = "CBITW";
constexpr char TEXT_CHECKB[]  = "CHECKB";
constexpr char TEXT_CHECKD[]  = "CHECKD";
constexpr char TEXT_CHECKW[]  = "CHECKW";
constexpr char TEXT_CINV[]    = "CINV";
// constexpr char TEXT_CMPB[]  = "CMPB";
// constexpr char TEXT_CMPD[]  = "CMPD";
constexpr char TEXT_CMPMB[]   = "CMPMB";
constexpr char TEXT_CMPMD[]   = "CMPMD";
constexpr char TEXT_CMPMW[]   = "CMPMW";
constexpr char TEXT_CMPQB[]   = "CMPQB";
constexpr char TEXT_CMPQD[]   = "CMPQD";
constexpr char TEXT_CMPQW[]   = "CMPQW";
// constexpr char TEXT_CMPSB[] = "CMPSB";
// constexpr char TEXT_CMPSD[] = "CMPSD";
constexpr char TEXT_CMPST[]   = "CMPST";
// constexpr char TEXT_CMPSW[] = "CMPSW";
// constexpr char TEXT_CMPW[]  = "CMPW";
// constexpr char TEXT_COMB[]  = "COMB";
// constexpr char TEXT_COMD[]  = "COMD";
// constexpr char TEXT_COMW[]  = "COMW";
constexpr char TEXT_CVTP[]    = "CVTP";
constexpr char TEXT_CXP[]     = "CXP";
constexpr char TEXT_CXPD[]    = "CXPD";
constexpr char TEXT_DEIB[]    = "DEIB";
constexpr char TEXT_DEID[]    = "DEID";
constexpr char TEXT_DEIW[]    = "DEIW";
constexpr char TEXT_DIA[]     = "DIA";
// constexpr char TEXT_DIVB[]  = "DIVB";
// constexpr char TEXT_DIVD[]  = "DIVD";
// constexpr char TEXT_DIVW[]  = "DIVW";
// constexpr char TEXT_ENTER[] = "ENTER";
// constexpr char TEXT_EXIT[]  = "EXIT";
// constexpr char TEXT_EXTB[]  = "EXTB";
constexpr char TEXT_EXTD[]    = "EXTD";
// constexpr char TEXT_EXTSB[] = "EXTSB";
constexpr char TEXT_EXTSD[]   = "EXTSD";
// constexpr char TEXT_EXTSW[] = "EXTSW";
constexpr char TEXT_EXTW[]    = "EXTW";
constexpr char TEXT_FFSB[]    = "FFSB";
constexpr char TEXT_FFSD[]    = "FFSD";
constexpr char TEXT_FFSW[]    = "FFSW";
constexpr char TEXT_FLAG[]    = "FLAG";
constexpr char TEXT_IBITB[]   = "IBITB";
constexpr char TEXT_IBITD[]   = "IBITD";
constexpr char TEXT_IBITW[]   = "IBITW";
constexpr char TEXT_INDEXB[]  = "INDEXB";
constexpr char TEXT_INDEXD[]  = "INDEXD";
constexpr char TEXT_INDEXW[]  = "INDEXW";
// constexpr char TEXT_INSB[]  = "INSB";
// constexpr char TEXT_INSD[]  = "INSD";
constexpr char TEXT_INSSB[]   = "INSSB";
constexpr char TEXT_INSSD[]   = "INSSD";
constexpr char TEXT_INSSW[]   = "INSSW";
// constexpr char TEXT_INSW[]  = "INSW";
// constexpr char TEXT_JSR[]   = "JSR";
constexpr char TEXT_JUMP[]    = "JUMP";
constexpr char TEXT_LFSR[]    = "LFSR";
constexpr char TEXT_LPRB[]    = "LPRB";
constexpr char TEXT_LPRD[]    = "LPRD";
constexpr char TEXT_LPRW[]    = "LPRW";
constexpr char TEXT_LSHB[]    = "LSHB";
constexpr char TEXT_LSHD[]    = "LSHD";
constexpr char TEXT_LSHW[]    = "LSHW";
constexpr char TEXT_MEIB[]    = "MEIB";
constexpr char TEXT_MEID[]    = "MEID";
constexpr char TEXT_MEIW[]    = "MEIW";
constexpr char TEXT_MODB[]    = "MODB";
// constexpr char TEXT_MODD[]  = "MODD";
constexpr char TEXT_MODW[]    = "MODW";
// constexpr char TEXT_MOVB[]  = "MOVB";
// constexpr char TEXT_MOVD[]  = "MOVD";
constexpr char TEXT_MOVMB[]   = "MOVMB";
constexpr char TEXT_MOVMD[]   = "MOVMD";
constexpr char TEXT_MOVMW[]   = "MOVMW";
constexpr char TEXT_MOVQB[]   = "MOVQB";
constexpr char TEXT_MOVQD[]   = "MOVQD";
constexpr char TEXT_MOVQW[]   = "MOVQW";
// constexpr char TEXT_MOVSB[] = "MOVSB";
// constexpr char TEXT_MOVSD[] = "MOVSD";
constexpr char TEXT_MOVST[]   = "MOVST";
// constexpr char TEXT_MOVSW[] = "MOVSW";
// constexpr char TEXT_MOVW[]  = "MOVW";
constexpr char TEXT_MOVXBD[]  = "MOVXBD";
constexpr char TEXT_MOVXBW[]  = "MOVXBW";
constexpr char TEXT_MOVXWD[]  = "MOVXWD";
constexpr char TEXT_MOVZBD[]  = "MOVZBD";
constexpr char TEXT_MOVZBW[]  = "MOVZBW";
constexpr char TEXT_MOVZWD[]  = "MOVZWD";
// constexpr char TEXT_MULB[]  = "MULB";
// constexpr char TEXT_MULD[]  = "MULD";
constexpr char TEXT_MULW[]    = "MULW";
// constexpr char TEXT_NEGB[]  = "NEGB";
// constexpr char TEXT_NEGD[]  = "NEGD";
// constexpr char TEXT_NEGW[]  = "NEGW";
// constexpr char TEXT_NOP[]   = "NOP";
// constexpr char TEXT_NOTB[]  = "NOTB";
constexpr char TEXT_NOTD[]    = "NOTD";
constexpr char TEXT_NOTW[]    = "NOTW";
// constexpr char TEXT_ORB[]   = "ORB";
// constexpr char TEXT_ORD[]   = "ORD";
// constexpr char TEXT_ORW[]   = "ORW";
constexpr char TEXT_QUOB[]    = "QUOB";
constexpr char TEXT_QUOD[]    = "QUOD";
constexpr char TEXT_QUOW[]    = "QUOW";
constexpr char TEXT_REMB[]    = "REMB";
constexpr char TEXT_REMD[]    = "REMD";
constexpr char TEXT_REMW[]    = "REMW";
constexpr char TEXT_RESTORE[] = "RESTORE";
// constexpr char TEXT_RET[]   = "RET";
// constexpr char TEXT_RETI[]  = "RETI";
constexpr char TEXT_RETT[]    = "RETT";
constexpr char TEXT_ROTB[]    = "ROTB";
constexpr char TEXT_ROTD[]    = "ROTD";
constexpr char TEXT_ROTW[]    = "ROTW";
constexpr char TEXT_RXP[]     = "RXP";
constexpr char TEXT_SAVE[]    = "SAVE";
constexpr char TEXT_SBITB[]   = "SBITB";
constexpr char TEXT_SBITD[]   = "SBITD";
constexpr char TEXT_SBITIB[]  = "SBITIB";
constexpr char TEXT_SBITID[]  = "SBITID";
constexpr char TEXT_SBITIW[]  = "SBITIW";
constexpr char TEXT_SBITW[]   = "SBITW";
constexpr char TEXT_SCCB[]    = "SCCB";
constexpr char TEXT_SCCD[]    = "SCCD";
constexpr char TEXT_SCCW[]    = "SCCW";
constexpr char TEXT_SCSB[]    = "SCSB";
constexpr char TEXT_SCSD[]    = "SCSD";
constexpr char TEXT_SCSW[]    = "SCSW";
constexpr char TEXT_SEQB[]    = "SEQB";
constexpr char TEXT_SEQD[]    = "SEQD";
constexpr char TEXT_SEQW[]    = "SEQW";
constexpr char TEXT_SETCFG[]  = "SETCFG";
constexpr char TEXT_SFCB[]    = "SFCB";
constexpr char TEXT_SFCD[]    = "SFCD";
constexpr char TEXT_SFCW[]    = "SFCW";
constexpr char TEXT_SFSB[]    = "SFSB";
constexpr char TEXT_SFSD[]    = "SFSD";
constexpr char TEXT_SFSR[]    = "SFSR";
constexpr char TEXT_SFSW[]    = "SFSW";
constexpr char TEXT_SGEB[]    = "SGEB";
constexpr char TEXT_SGED[]    = "SGED";
constexpr char TEXT_SGEW[]    = "SGEW";
constexpr char TEXT_SGTB[]    = "SGTB";
constexpr char TEXT_SGTD[]    = "SGTD";
constexpr char TEXT_SGTW[]    = "SGTW";
constexpr char TEXT_SHIB[]    = "SHIB";
constexpr char TEXT_SHID[]    = "SHID";
constexpr char TEXT_SHIW[]    = "SHIW";
constexpr char TEXT_SHSB[]    = "SHSB";
constexpr char TEXT_SHSD[]    = "SHSD";
constexpr char TEXT_SHSW[]    = "SHSW";
constexpr char TEXT_SKPSB[]   = "SKPSB";
constexpr char TEXT_SKPSD[]   = "SKPSD";
constexpr char TEXT_SKPST[]   = "SKPST";
constexpr char TEXT_SKPSW[]   = "SKPSW";
constexpr char TEXT_SLEB[]    = "SLEB";
constexpr char TEXT_SLED[]    = "SLED";
constexpr char TEXT_SLEW[]    = "SLEW";
constexpr char TEXT_SLOB[]    = "SLOB";
constexpr char TEXT_SLOD[]    = "SLOD";
constexpr char TEXT_SLOW[]    = "SLOW";
constexpr char TEXT_SLSB[]    = "SLSB";
constexpr char TEXT_SLSD[]    = "SLSD";
constexpr char TEXT_SLSW[]    = "SLSW";
constexpr char TEXT_SLTB[]    = "SLTB";
constexpr char TEXT_SLTD[]    = "SLTD";
constexpr char TEXT_SLTW[]    = "SLTW";
constexpr char TEXT_SNEB[]    = "SNEB";
constexpr char TEXT_SNED[]    = "SNED";
constexpr char TEXT_SNEW[]    = "SNEW";
constexpr char TEXT_SPRB[]    = "SPRB";
constexpr char TEXT_SPRD[]    = "SPRD";
constexpr char TEXT_SPRW[]    = "SPRW";
// constexpr char TEXT_SUBB[]  = "SUBB";
// constexpr char TEXT_SUBCB[] = "SUBCB";
constexpr char TEXT_SUBCD[]   = "SUBCD";
constexpr char TEXT_SUBCW[]   = "SUBCW";
// constexpr char TEXT_SUBD[]  = "SUBD";
constexpr char TEXT_SUBPB[]   = "SUBPB";
constexpr char TEXT_SUBPD[]   = "SUBPD";
constexpr char TEXT_SUBPW[]   = "SUBPW";
// constexpr char TEXT_SUBW[]  = "SUBW";
// constexpr char TEXT_SVC[]   = "SVC";
constexpr char TEXT_TBITB[]   = "TBITB";
constexpr char TEXT_TBITD[]   = "TBITD";
constexpr char TEXT_TBITW[]   = "TBITW";
// constexpr char TEXT_WAIT[]  = "WAIT";
// constexpr char TEXT_XORB[]  = "XORB";
constexpr char TEXT_XORD[]    = "XORD";
// constexpr char TEXT_XORW[]  = "XORW";

// NS32081
// constexpr char TEXT_ABSF[] = "ABSF";
constexpr char TEXT_ABSL[]    = "ABSL";
// constexpr char TEXT_ADDF[] = "ADDF";
// constexpr char TEXT_ADDL[] = "ADDL";
// constexpr char TEXT_CMPF[] = "CMPF";
// constexpr char TEXT_CMPL[] = "CMPL";
// constexpr char TEXT_DIVF[] = "DIVF";
// constexpr char TEXT_DIVL[] = "DIVL";
constexpr char TEXT_FLOORFB[] = "FLOORFB";
constexpr char TEXT_FLOORFD[] = "FLOORFD";
constexpr char TEXT_FLOORFW[] = "FLOORFW";
constexpr char TEXT_FLOORLB[] = "FLOORLB";
constexpr char TEXT_FLOORLD[] = "FLOORLD";
constexpr char TEXT_FLOORLW[] = "FLOORLW";
constexpr char TEXT_MOVBF[]   = "MOVBF";
constexpr char TEXT_MOVBL[]   = "MOVBL";
constexpr char TEXT_MOVDF[]   = "MOVDF";
constexpr char TEXT_MOVDL[]   = "MOVDL";
constexpr char TEXT_MOVF[]    = "MOVF";
constexpr char TEXT_MOVFL[]   = "MOVFL";
constexpr char TEXT_MOVL[]    = "MOVL";
constexpr char TEXT_MOVLF[]   = "MOVLF";
constexpr char TEXT_MOVWF[]   = "MOVWF";
constexpr char TEXT_MOVWL[]   = "MOVWL";
// constexpr char TEXT_MULF[] = "MULF";
constexpr char TEXT_MULL[]    = "MULL";
// constexpr char TEXT_NEGF[] = "NEGF";
constexpr char TEXT_NEGL[]    = "NEGL";
constexpr char TEXT_ROUNDFB[] = "ROUNDFB";
constexpr char TEXT_ROUNDFD[] = "ROUNDFD";
constexpr char TEXT_ROUNDFW[] = "ROUNDFW";
constexpr char TEXT_ROUNDLB[] = "ROUNDLB";
constexpr char TEXT_ROUNDLD[] = "ROUNDLD";
constexpr char TEXT_ROUNDLW[] = "ROUNDLW";
// constexpr char TEXT_SUBF[] = "SUBF";
// constexpr char TEXT_SUBL[] = "SUBL";
constexpr char TEXT_TRUNCFB[] = "TRUNCFB";
constexpr char TEXT_TRUNCFD[] = "TRUNCFD";
constexpr char TEXT_TRUNCFW[] = "TRUNCFW";
constexpr char TEXT_TRUNCLB[] = "TRUNCLB";
constexpr char TEXT_TRUNCLD[] = "TRUNCLD";
constexpr char TEXT_TRUNCLW[] = "TRUNCLW";

// NS32082
constexpr char TEXT_LMR[]    = "LMR";
constexpr char TEXT_MOVSUB[] = "MOVSUB";
constexpr char TEXT_MOVSUD[] = "MOVSUD";
constexpr char TEXT_MOVSUW[] = "MOVSUW";
constexpr char TEXT_MOVUSB[] = "MOVUSB";
constexpr char TEXT_MOVUSD[] = "MOVUSD";
constexpr char TEXT_MOVUSW[] = "MOVUSW";
constexpr char TEXT_RDVAL[]  = "RDVAL";
constexpr char TEXT_SMR[]    = "SMR";
constexpr char TEXT_WRVAL[]  = "WRVAL";

// NS32032
constexpr char TEXT_REG_EXT[] = "EXT";
// constexpr char TEXT_REG_FP[]  = "FP";
// constexpr char TEXT_REG_PC[] = "PC";
// constexpr char TEXT_REG_R0[] = "R0";
// constexpr char TEXT_REG_R1[] = "R1";
// constexpr char TEXT_REG_R2[] = "R2";
// constexpr char TEXT_REG_R3[] = "R3";
// constexpr char TEXT_REG_R4[] = "R4";
// constexpr char TEXT_REG_R5[] = "R5";
// constexpr char TEXT_REG_R6[] = "R6";
// constexpr char TEXT_REG_R7[] = "R7";
constexpr char TEXT_REG_SB[]  = "SB";
// constexpr char TEXT_REG_SP[] = "SP";
constexpr char TEXT_REG_TOS[] = "TOS";

// NS32081
// constexpr char TEXT_REG_F0[]  = "F0";
// constexpr char TEXT_REG_F1[]  = "F1";
constexpr char TEXT_REG_F2[]      = "F2";
constexpr char TEXT_REG_F3[]      = "F3";
constexpr char TEXT_REG_F4[]      = "F4";
constexpr char TEXT_REG_F5[]      = "F5";
constexpr char TEXT_REG_F6[]      = "F6";
constexpr char TEXT_REG_F7[]      = "F7";
constexpr char TEXT_REG_INTBASE[] = "INTBASE";
// constexpr char TEXT_REG_MOD[]     = "MOD";
// constexpr char TEXT_REG_PSR[] = "PSR";
constexpr char TEXT_REG_UPSR[]    = "UPSR";
constexpr char TEXT_REG_US[]      = "US";

// NS32082
constexpr char TEXT_REG_BCNT[] = "BCNT";
constexpr char TEXT_REG_BPR0[] = "BPR0";
constexpr char TEXT_REG_BPR1[] = "BPR1";
constexpr char TEXT_REG_EIA[]  = "EIA";
constexpr char TEXT_REG_MSR[]  = "MSR";
constexpr char TEXT_REG_PTB0[] = "PTB0";
constexpr char TEXT_REG_PTB1[] = "PTB1";

constexpr char TEXT_CONFIG_C[] = "C";
constexpr char TEXT_CONFIG_F[] = "F";
constexpr char TEXT_CONFIG_I[] = "I";
constexpr char TEXT_CONFIG_M[] = "M";

// clang-format on
}  // namespace ns32000
}  // namespace text
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
