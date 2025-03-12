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
constexpr char TEXT_CPU_NS32032[] PROGMEM = "NS32032";
constexpr char TEXT_FPU_NS32081[] PROGMEM = "NS32081";
constexpr char TEXT_MMU_NS32082[] PROGMEM = "NS32082";
constexpr char TEXT_CPU_32032[]   PROGMEM = "32032";

constexpr char TEXT_PMMU[] PROGMEM = "PMMU";

// Standard instructions
constexpr char TEXT_ABSB[]    PROGMEM = "ABSB";
// constexpr char TEXT_ABSD[]    PROGMEM = "ABSD";
constexpr char TEXT_ABSW[]    PROGMEM = "ABSW";
constexpr char TEXT_ACBB[]    PROGMEM = "ACBB";
constexpr char TEXT_ACBD[]    PROGMEM = "ACBD";
constexpr char TEXT_ACBW[]    PROGMEM = "ACBW";
// constexpr char TEXT_ADDB[]    PROGMEM = "ADDB";
// constexpr char TEXT_ADDCB[]   PROGMEM = "ADDCB";
constexpr char TEXT_ADDCD[]   PROGMEM = "ADDCD";
constexpr char TEXT_ADDCW[]   PROGMEM = "ADDCW";
// constexpr char TEXT_ADDD[]    PROGMEM = "ADDD";
constexpr char TEXT_ADDPB[]   PROGMEM = "ADDPB";
constexpr char TEXT_ADDPD[]   PROGMEM = "ADDPD";
constexpr char TEXT_ADDPW[]   PROGMEM = "ADDPW";
constexpr char TEXT_ADDQB[]   PROGMEM = "ADDQB";
constexpr char TEXT_ADDQD[]   PROGMEM = "ADDQD";
constexpr char TEXT_ADDQW[]   PROGMEM = "ADDQW";
// constexpr char TEXT_ADDR[]    PROGMEM = "ADDR";
// constexpr char TEXT_ADDW[]    PROGMEM = "ADDW";
constexpr char TEXT_ADJSPB[]  PROGMEM = "ADJSPB";
constexpr char TEXT_ADJSPD[]  PROGMEM = "ADJSPD";
constexpr char TEXT_ADJSPW[]  PROGMEM = "ADJSPW";
// constexpr char TEXT_ANDB[]    PROGMEM = "ANDB";
// constexpr char TEXT_ANDD[]    PROGMEM = "ANDD";
constexpr char TEXT_ANDW[]    PROGMEM = "ANDW";
constexpr char TEXT_ASHB[]    PROGMEM = "ASHB";
constexpr char TEXT_ASHD[]    PROGMEM = "ASHD";
constexpr char TEXT_ASHW[]    PROGMEM = "ASHW";
// constexpr char TEXT_BCC[]     PROGMEM = "BCC";
// constexpr char TEXT_BCS[]     PROGMEM = "BCS";
// constexpr char TEXT_BEQ[]     PROGMEM = "BEQ";
constexpr char TEXT_BFC[]     PROGMEM = "BFC";
constexpr char TEXT_BFS[]     PROGMEM = "BFS";
// constexpr char TEXT_BGE[]     PROGMEM = "BGE";
// constexpr char TEXT_BGT[]     PROGMEM = "BGT";
// constexpr char TEXT_BHI[]     PROGMEM = "BHI";
// constexpr char TEXT_BHS[]     PROGMEM = "BHS";
// constexpr char TEXT_BICB[]    PROGMEM = "BICB";
constexpr char TEXT_BICD[]    PROGMEM = "BICD";
constexpr char TEXT_BICPSRB[] PROGMEM = "BICPSRB";
constexpr char TEXT_BICPSRW[] PROGMEM = "BICPSRW";
constexpr char TEXT_BICW[]    PROGMEM = "BICW";
constexpr char TEXT_BISPSRB[] PROGMEM = "BISPSRB";
constexpr char TEXT_BISPSRW[] PROGMEM = "BISPSRW";
// constexpr char TEXT_BLE[]     PROGMEM = "BLE";
// constexpr char TEXT_BLO[]     PROGMEM = "BLO";
// constexpr char TEXT_BLS[]     PROGMEM = "BLS";
// constexpr char TEXT_BLT[]     PROGMEM = "BLT";
// constexpr char TEXT_BNE[]     PROGMEM = "BNE";
// constexpr char TEXT_BPT[]     PROGMEM = "BPT";
// constexpr char TEXT_BR[]      PROGMEM = "BR";
// constexpr char TEXT_BSR[]     PROGMEM = "BSR";
constexpr char TEXT_CASEB[]   PROGMEM = "CASEB";
constexpr char TEXT_CASED[]   PROGMEM = "CASED";
constexpr char TEXT_CASEW[]   PROGMEM = "CASEW";
constexpr char TEXT_CBITB[]   PROGMEM = "CBITB";
constexpr char TEXT_CBITD[]   PROGMEM = "CBITD";
constexpr char TEXT_CBITIB[]  PROGMEM = "CBITIB";
constexpr char TEXT_CBITID[]  PROGMEM = "CBITID";
constexpr char TEXT_CBITIW[]  PROGMEM = "CBITIW";
constexpr char TEXT_CBITW[]   PROGMEM = "CBITW";
constexpr char TEXT_CHECKB[]  PROGMEM = "CHECKB";
constexpr char TEXT_CHECKD[]  PROGMEM = "CHECKD";
constexpr char TEXT_CHECKW[]  PROGMEM = "CHECKW";
constexpr char TEXT_CINV[]    PROGMEM = "CINV";
// constexpr char TEXT_CMPB[]    PROGMEM = "CMPB";
// constexpr char TEXT_CMPD[]    PROGMEM = "CMPD";
constexpr char TEXT_CMPMB[]   PROGMEM = "CMPMB";
constexpr char TEXT_CMPMD[]   PROGMEM = "CMPMD";
constexpr char TEXT_CMPMW[]   PROGMEM = "CMPMW";
constexpr char TEXT_CMPQB[]   PROGMEM = "CMPQB";
constexpr char TEXT_CMPQD[]   PROGMEM = "CMPQD";
constexpr char TEXT_CMPQW[]   PROGMEM = "CMPQW";
// constexpr char TEXT_CMPSB[]   PROGMEM = "CMPSB";
constexpr char TEXT_CMPSD[]   PROGMEM = "CMPSD";
constexpr char TEXT_CMPST[]   PROGMEM = "CMPST";
// constexpr char TEXT_CMPSW[]   PROGMEM = "CMPSW";
// constexpr char TEXT_CMPW[]    PROGMEM = "CMPW";
// constexpr char TEXT_COMB[]    PROGMEM = "COMB";
// constexpr char TEXT_COMD[]    PROGMEM = "COMD";
// constexpr char TEXT_COMW[]    PROGMEM = "COMW";
constexpr char TEXT_CVTP[]    PROGMEM = "CVTP";
constexpr char TEXT_CXP[]     PROGMEM = "CXP";
constexpr char TEXT_CXPD[]    PROGMEM = "CXPD";
constexpr char TEXT_DEIB[]    PROGMEM = "DEIB";
constexpr char TEXT_DEID[]    PROGMEM = "DEID";
constexpr char TEXT_DEIW[]    PROGMEM = "DEIW";
constexpr char TEXT_DIA[]     PROGMEM = "DIA";
// constexpr char TEXT_DIVB[]    PROGMEM = "DIVB";
// constexpr char TEXT_DIVD[]    PROGMEM = "DIVD";
// constexpr char TEXT_DIVW[]    PROGMEM = "DIVW";
// constexpr char TEXT_ENTER[]   PROGMEM = "ENTER";
// constexpr char TEXT_EXIT[]    PROGMEM = "EXIT";
// constexpr char TEXT_EXTB[]    PROGMEM = "EXTB";
constexpr char TEXT_EXTD[]    PROGMEM = "EXTD";
// constexpr char TEXT_EXTSB[]   PROGMEM = "EXTSB";
constexpr char TEXT_EXTSD[]   PROGMEM = "EXTSD";
constexpr char TEXT_EXTSW[]   PROGMEM = "EXTSW";
constexpr char TEXT_EXTW[]    PROGMEM = "EXTW";
constexpr char TEXT_FFSB[]    PROGMEM = "FFSB";
constexpr char TEXT_FFSD[]    PROGMEM = "FFSD";
constexpr char TEXT_FFSW[]    PROGMEM = "FFSW";
constexpr char TEXT_FLAG[]    PROGMEM = "FLAG";
constexpr char TEXT_IBITB[]   PROGMEM = "IBITB";
constexpr char TEXT_IBITD[]   PROGMEM = "IBITD";
constexpr char TEXT_IBITW[]   PROGMEM = "IBITW";
constexpr char TEXT_INDEXB[]  PROGMEM = "INDEXB";
constexpr char TEXT_INDEXD[]  PROGMEM = "INDEXD";
constexpr char TEXT_INDEXW[]  PROGMEM = "INDEXW";
// constexpr char TEXT_INSB[]    PROGMEM = "INSB";
constexpr char TEXT_INSD[]    PROGMEM = "INSD";
constexpr char TEXT_INSSB[]   PROGMEM = "INSSB";
constexpr char TEXT_INSSD[]   PROGMEM = "INSSD";
constexpr char TEXT_INSSW[]   PROGMEM = "INSSW";
// constexpr char TEXT_INSW[]    PROGMEM = "INSW";
// constexpr char TEXT_JSR[]     PROGMEM = "JSR";
constexpr char TEXT_JUMP[]    PROGMEM = "JUMP";
constexpr char TEXT_LPRB[]    PROGMEM = "LPRB";
constexpr char TEXT_LPRD[]    PROGMEM = "LPRD";
constexpr char TEXT_LPRW[]    PROGMEM = "LPRW";
constexpr char TEXT_LFSR[]    PROGMEM = "LFSR";
constexpr char TEXT_LSHB[]    PROGMEM = "LSHB";
constexpr char TEXT_LSHD[]    PROGMEM = "LSHD";
constexpr char TEXT_LSHW[]    PROGMEM = "LSHW";
constexpr char TEXT_MEIB[]    PROGMEM = "MEIB";
constexpr char TEXT_MEID[]    PROGMEM = "MEID";
constexpr char TEXT_MEIW[]    PROGMEM = "MEIW";
constexpr char TEXT_MODB[]    PROGMEM = "MODB";
// constexpr char TEXT_MODD[]    PROGMEM = "MODD";
constexpr char TEXT_MODW[]    PROGMEM = "MODW";
// constexpr char TEXT_MOVB[]    PROGMEM = "MOVB";
// constexpr char TEXT_MOVD[]    PROGMEM = "MOVD";
constexpr char TEXT_MOVMB[]   PROGMEM = "MOVMB";
constexpr char TEXT_MOVMD[]   PROGMEM = "MOVMD";
constexpr char TEXT_MOVMW[]   PROGMEM = "MOVMW";
constexpr char TEXT_MOVQB[]   PROGMEM = "MOVQB";
constexpr char TEXT_MOVQD[]   PROGMEM = "MOVQD";
constexpr char TEXT_MOVQW[]   PROGMEM = "MOVQW";
// constexpr char TEXT_MOVSB[]   PROGMEM = "MOVSB";
constexpr char TEXT_MOVSD[]   PROGMEM = "MOVSD";
constexpr char TEXT_MOVST[]   PROGMEM = "MOVST";
// constexpr char TEXT_MOVSW[]   PROGMEM = "MOVSW";
// constexpr char TEXT_MOVW[]    PROGMEM = "MOVW";
constexpr char TEXT_MOVXBD[]  PROGMEM = "MOVXBD";
constexpr char TEXT_MOVXBW[]  PROGMEM = "MOVXBW";
constexpr char TEXT_MOVXWD[]  PROGMEM = "MOVXWD";
constexpr char TEXT_MOVZBD[]  PROGMEM = "MOVZBD";
constexpr char TEXT_MOVZBW[]  PROGMEM = "MOVZBW";
constexpr char TEXT_MOVZWD[]  PROGMEM = "MOVZWD";
// constexpr char TEXT_MULB[]    PROGMEM = "MULB";
// constexpr char TEXT_MULD[]    PROGMEM = "MULD";
constexpr char TEXT_MULW[]    PROGMEM = "MULW";
// constexpr char TEXT_NEGB[]    PROGMEM = "NEGB";
// constexpr char TEXT_NEGD[]    PROGMEM = "NEGD";
constexpr char TEXT_NEGW[]    PROGMEM = "NEGW";
// constexpr char TEXT_NOP[]     PROGMEM = "NOP";
// constexpr char TEXT_NOTB[]    PROGMEM = "NOTB";
constexpr char TEXT_NOTD[]    PROGMEM = "NOTD";
constexpr char TEXT_NOTW[]    PROGMEM = "NOTW";
// constexpr char TEXT_ORB[]     PROGMEM = "ORB";
// constexpr char TEXT_ORD[]     PROGMEM = "ORD";
constexpr char TEXT_ORW[]     PROGMEM = "ORW";
constexpr char TEXT_QUOB[]    PROGMEM = "QUOB";
constexpr char TEXT_QUOD[]    PROGMEM = "QUOD";
constexpr char TEXT_QUOW[]    PROGMEM = "QUOW";
constexpr char TEXT_REMB[]    PROGMEM = "REMB";
constexpr char TEXT_REMD[]    PROGMEM = "REMD";
constexpr char TEXT_REMW[]    PROGMEM = "REMW";
constexpr char TEXT_RESTORE[] PROGMEM = "RESTORE";
// constexpr char TEXT_RET[]     PROGMEM = "RET";
// constexpr char TEXT_RETI[]    PROGMEM = "RETI";
constexpr char TEXT_RETT[]    PROGMEM = "RETT";
constexpr char TEXT_ROTB[]    PROGMEM = "ROTB";
constexpr char TEXT_ROTD[]    PROGMEM = "ROTD";
constexpr char TEXT_ROTW[]    PROGMEM = "ROTW";
constexpr char TEXT_RXP[]     PROGMEM = "RXP";
constexpr char TEXT_SAVE[]    PROGMEM = "SAVE";
constexpr char TEXT_SBITB[]   PROGMEM = "SBITB";
constexpr char TEXT_SBITD[]   PROGMEM = "SBITD";
constexpr char TEXT_SBITIB[]  PROGMEM = "SBITIB";
constexpr char TEXT_SBITID[]  PROGMEM = "SBITID";
constexpr char TEXT_SBITIW[]  PROGMEM = "SBITIW";
constexpr char TEXT_SBITW[]   PROGMEM = "SBITW";
constexpr char TEXT_SCCB[]    PROGMEM = "SCCB";
constexpr char TEXT_SCCD[]    PROGMEM = "SCCD";
constexpr char TEXT_SCCW[]    PROGMEM = "SCCW";
constexpr char TEXT_SCSB[]    PROGMEM = "SCSB";
constexpr char TEXT_SCSD[]    PROGMEM = "SCSD";
constexpr char TEXT_SCSW[]    PROGMEM = "SCSW";
constexpr char TEXT_SEQB[]    PROGMEM = "SEQB";
constexpr char TEXT_SEQD[]    PROGMEM = "SEQD";
constexpr char TEXT_SEQW[]    PROGMEM = "SEQW";
constexpr char TEXT_SETCFG[]  PROGMEM = "SETCFG";
constexpr char TEXT_SFCB[]    PROGMEM = "SFCB";
constexpr char TEXT_SFCD[]    PROGMEM = "SFCD";
constexpr char TEXT_SFCW[]    PROGMEM = "SFCW";
constexpr char TEXT_SFSB[]    PROGMEM = "SFSB";
constexpr char TEXT_SFSD[]    PROGMEM = "SFSD";
constexpr char TEXT_SFSR[]    PROGMEM = "SFSR";
constexpr char TEXT_SFSW[]    PROGMEM = "SFSW";
constexpr char TEXT_SGEB[]    PROGMEM = "SGEB";
constexpr char TEXT_SGED[]    PROGMEM = "SGED";
constexpr char TEXT_SGEW[]    PROGMEM = "SGEW";
constexpr char TEXT_SGTB[]    PROGMEM = "SGTB";
constexpr char TEXT_SGTD[]    PROGMEM = "SGTD";
constexpr char TEXT_SGTW[]    PROGMEM = "SGTW";
constexpr char TEXT_SHIB[]    PROGMEM = "SHIB";
constexpr char TEXT_SHID[]    PROGMEM = "SHID";
constexpr char TEXT_SHIW[]    PROGMEM = "SHIW";
constexpr char TEXT_SHSB[]    PROGMEM = "SHSB";
constexpr char TEXT_SHSD[]    PROGMEM = "SHSD";
constexpr char TEXT_SHSW[]    PROGMEM = "SHSW";
constexpr char TEXT_SKPSB[]   PROGMEM = "SKPSB";
constexpr char TEXT_SKPSD[]   PROGMEM = "SKPSD";
constexpr char TEXT_SKPST[]   PROGMEM = "SKPST";
constexpr char TEXT_SKPSW[]   PROGMEM = "SKPSW";
constexpr char TEXT_SLEB[]    PROGMEM = "SLEB";
constexpr char TEXT_SLED[]    PROGMEM = "SLED";
constexpr char TEXT_SLEW[]    PROGMEM = "SLEW";
constexpr char TEXT_SLOB[]    PROGMEM = "SLOB";
constexpr char TEXT_SLOD[]    PROGMEM = "SLOD";
constexpr char TEXT_SLOW[]    PROGMEM = "SLOW";
constexpr char TEXT_SLSB[]    PROGMEM = "SLSB";
constexpr char TEXT_SLSD[]    PROGMEM = "SLSD";
constexpr char TEXT_SLSW[]    PROGMEM = "SLSW";
constexpr char TEXT_SLTB[]    PROGMEM = "SLTB";
constexpr char TEXT_SLTD[]    PROGMEM = "SLTD";
constexpr char TEXT_SLTW[]    PROGMEM = "SLTW";
constexpr char TEXT_SNEB[]    PROGMEM = "SNEB";
constexpr char TEXT_SNED[]    PROGMEM = "SNED";
constexpr char TEXT_SNEW[]    PROGMEM = "SNEW";
constexpr char TEXT_SPRB[]    PROGMEM = "SPRB";
constexpr char TEXT_SPRD[]    PROGMEM = "SPRD";
constexpr char TEXT_SPRW[]    PROGMEM = "SPRW";
// constexpr char TEXT_SUBB[]    PROGMEM = "SUBB";
// constexpr char TEXT_SUBCB[]   PROGMEM = "SUBCB";
constexpr char TEXT_SUBCD[]   PROGMEM = "SUBCD";
constexpr char TEXT_SUBCW[]   PROGMEM = "SUBCW";
// constexpr char TEXT_SUBD[]    PROGMEM = "SUBD";
constexpr char TEXT_SUBPB[]   PROGMEM = "SUBPB";
constexpr char TEXT_SUBPD[]   PROGMEM = "SUBPD";
constexpr char TEXT_SUBPW[]   PROGMEM = "SUBPW";
// constexpr char TEXT_SUBW[]    PROGMEM = "SUBW";
// constexpr char TEXT_SVC[]     PROGMEM = "SVC";
constexpr char TEXT_TBITB[]   PROGMEM = "TBITB";
constexpr char TEXT_TBITD[]   PROGMEM = "TBITD";
constexpr char TEXT_TBITW[]   PROGMEM = "TBITW";
// constexpr char TEXT_WAIT[]    PROGMEM = "WAIT";
// constexpr char TEXT_XORB[]    PROGMEM = "XORB";
constexpr char TEXT_XORD[]    PROGMEM = "XORD";
constexpr char TEXT_XORW[]    PROGMEM = "XORW";
// Floating point instructions
// constexpr char TEXT_ABSF[]    PROGMEM = "ABSF";
constexpr char TEXT_ABSL[]    PROGMEM = "ABSL";
// constexpr char TEXT_ADDF[]    PROGMEM = "ADDF";
// constexpr char TEXT_ADDL[]    PROGMEM = "ADDL";
// constexpr char TEXT_CMPF[]    PROGMEM = "CMPF";
// constexpr char TEXT_CMPL[]    PROGMEM = "CMPL";
// constexpr char TEXT_DIVF[]    PROGMEM = "DIVF";
// constexpr char TEXT_DIVL[]    PROGMEM = "DIVL";
constexpr char TEXT_FLOORFB[] PROGMEM = "FLOORFB";
constexpr char TEXT_FLOORFD[] PROGMEM = "FLOORFD";
constexpr char TEXT_FLOORFW[] PROGMEM = "FLOORFW";
constexpr char TEXT_FLOORLB[] PROGMEM = "FLOORLB";
constexpr char TEXT_FLOORLD[] PROGMEM = "FLOORLD";
constexpr char TEXT_FLOORLW[] PROGMEM = "FLOORLW";
constexpr char TEXT_MOVBF[]   PROGMEM = "MOVBF";
constexpr char TEXT_MOVBL[]   PROGMEM = "MOVBL";
constexpr char TEXT_MOVDF[]   PROGMEM = "MOVDF";
constexpr char TEXT_MOVDL[]   PROGMEM = "MOVDL";
constexpr char TEXT_MOVF[]    PROGMEM = "MOVF";
constexpr char TEXT_MOVFL[]   PROGMEM = "MOVFL";
constexpr char TEXT_MOVL[]    PROGMEM = "MOVL";
constexpr char TEXT_MOVLF[]   PROGMEM = "MOVLF";
constexpr char TEXT_MOVWF[]   PROGMEM = "MOVWF";
constexpr char TEXT_MOVWL[]   PROGMEM = "MOVWL";
// constexpr char TEXT_MULF[]    PROGMEM = "MULF";
constexpr char TEXT_MULL[]    PROGMEM = "MULL";
// constexpr char TEXT_NEGF[]    PROGMEM = "NEGF";
constexpr char TEXT_NEGL[]    PROGMEM = "NEGL";
constexpr char TEXT_ROUNDFB[] PROGMEM = "ROUNDFB";
constexpr char TEXT_ROUNDFD[] PROGMEM = "ROUNDFD";
constexpr char TEXT_ROUNDFW[] PROGMEM = "ROUNDFW";
constexpr char TEXT_ROUNDLB[] PROGMEM = "ROUNDLB";
constexpr char TEXT_ROUNDLD[] PROGMEM = "ROUNDLD";
constexpr char TEXT_ROUNDLW[] PROGMEM = "ROUNDLW";
// constexpr char TEXT_SUBF[]    PROGMEM = "SUBF";
// constexpr char TEXT_SUBL[]    PROGMEM = "SUBL";
constexpr char TEXT_TRUNCFB[] PROGMEM = "TRUNCFB";
constexpr char TEXT_TRUNCFD[] PROGMEM = "TRUNCFD";
constexpr char TEXT_TRUNCFW[] PROGMEM = "TRUNCFW";
constexpr char TEXT_TRUNCLB[] PROGMEM = "TRUNCLB";
constexpr char TEXT_TRUNCLD[] PROGMEM = "TRUNCLD";
constexpr char TEXT_TRUNCLW[] PROGMEM = "TRUNCLW";
// Memory management instructions
constexpr char TEXT_LMR[]     PROGMEM = "LMR";
constexpr char TEXT_MOVSUB[]  PROGMEM = "MOVSUB";
constexpr char TEXT_MOVSUD[]  PROGMEM = "MOVSUD";
constexpr char TEXT_MOVSUW[]  PROGMEM = "MOVSUW";
constexpr char TEXT_MOVUSB[]  PROGMEM = "MOVUSB";
constexpr char TEXT_MOVUSD[]  PROGMEM = "MOVUSD";
constexpr char TEXT_MOVUSW[]  PROGMEM = "MOVUSW";
constexpr char TEXT_RDVAL[]   PROGMEM = "RDVAL";
constexpr char TEXT_SMR[]     PROGMEM = "SMR";
constexpr char TEXT_WRVAL[]   PROGMEM = "WRVAL";

// constexpr char TEXT_REG_R0[] PROGMEM = "R0";
// constexpr char TEXT_REG_R1[] PROGMEM = "R1";
// constexpr char TEXT_REG_R2[] PROGMEM = "R2";
// constexpr char TEXT_REG_R3[] PROGMEM = "R3";
// constexpr char TEXT_REG_R4[] PROGMEM = "R4";
// constexpr char TEXT_REG_R5[] PROGMEM = "R5";
// constexpr char TEXT_REG_R6[] PROGMEM = "R6";
// constexpr char TEXT_REG_R7[] PROGMEM = "R7";

constexpr char TEXT_REG_FP[]  PROGMEM = "FP";
// constexpr char TEXT_REG_SP[]  PROGMEM = "SP";
constexpr char TEXT_REG_SB[]  PROGMEM = "SB";
// constexpr char TEXT_REG_PC[]  PROGMEM = "PC";
constexpr char TEXT_REG_TOS[] PROGMEM = "TOS";
constexpr char TEXT_REG_EXT[] PROGMEM = "EXT";

// NS32081/FPU Registers
// constexpr char TEXT_REG_F0[] PROGMEM = "F0";
// constexpr char TEXT_REG_F1[] PROGMEM = "F1";
constexpr char TEXT_REG_F2[] PROGMEM = "F2";
constexpr char TEXT_REG_F3[] PROGMEM = "F3";
constexpr char TEXT_REG_F4[] PROGMEM = "F4";
constexpr char TEXT_REG_F5[] PROGMEM = "F5";
constexpr char TEXT_REG_F6[] PROGMEM = "F6";
constexpr char TEXT_REG_F7[] PROGMEM = "F7";

constexpr char TEXT_REG_UPSR[]    PROGMEM = "UPSR";
constexpr char TEXT_REG_US[]      PROGMEM = "US";
constexpr char TEXT_REG_PSR[]     PROGMEM = "PSR";
constexpr char TEXT_REG_INTBASE[] PROGMEM = "INTBASE";
constexpr char TEXT_REG_MOD[]     PROGMEM = "MOD";

// NS32082/MMU registers.
constexpr char TEXT_REG_BPR0[] PROGMEM = "BPR0";
constexpr char TEXT_REG_BPR1[] PROGMEM = "BPR1";
constexpr char TEXT_REG_MSR[]  PROGMEM = "MSR";
constexpr char TEXT_REG_BCNT[] PROGMEM = "BCNT";
constexpr char TEXT_REG_PTB0[] PROGMEM = "PTB0";
constexpr char TEXT_REG_PTB1[] PROGMEM = "PTB1";
constexpr char TEXT_REG_EIA[]  PROGMEM = "EIA";

constexpr char TEXT_CONFIG_I[] PROGMEM = "I";
constexpr char TEXT_CONFIG_F[] PROGMEM = "F";
constexpr char TEXT_CONFIG_M[] PROGMEM = "M";
constexpr char TEXT_CONFIG_C[] PROGMEM = "C";
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
