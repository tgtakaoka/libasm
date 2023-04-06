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
const char TEXT_CPU_NS32032[] PROGMEM = "NS32032";
const char TEXT_FPU_NS32081[] PROGMEM = "NS32081";
const char TEXT_MMU_NS32082[] PROGMEM = "NS32082";
const char TEXT_CPU_32032[]   PROGMEM = "32032";
const char TEXT_none[] PROGMEM = "none";

const char TEXT_FPU[]  PROGMEM = "FPU";
const char TEXT_PMMU[] PROGMEM = "PMMU";

// Standard instructions
const char TEXT_ABSB[]    PROGMEM = "ABSB";
const char TEXT_ABSD[]    PROGMEM = "ABSD";
const char TEXT_ABSW[]    PROGMEM = "ABSW";
const char TEXT_ACBB[]    PROGMEM = "ACBB";
const char TEXT_ACBD[]    PROGMEM = "ACBD";
const char TEXT_ACBW[]    PROGMEM = "ACBW";
// const char TEXT_ADDB[]    PROGMEM = "ADDB";
// const char TEXT_ADDCB[]   PROGMEM = "ADDCB";
const char TEXT_ADDCD[]   PROGMEM = "ADDCD";
const char TEXT_ADDCW[]   PROGMEM = "ADDCW";
// const char TEXT_ADDD[]    PROGMEM = "ADDD";
const char TEXT_ADDPB[]   PROGMEM = "ADDPB";
const char TEXT_ADDPD[]   PROGMEM = "ADDPD";
const char TEXT_ADDPW[]   PROGMEM = "ADDPW";
const char TEXT_ADDQB[]   PROGMEM = "ADDQB";
const char TEXT_ADDQD[]   PROGMEM = "ADDQD";
const char TEXT_ADDQW[]   PROGMEM = "ADDQW";
// const char TEXT_ADDR[]    PROGMEM = "ADDR";
// const char TEXT_ADDW[]    PROGMEM = "ADDW";
const char TEXT_ADJSPB[]  PROGMEM = "ADJSPB";
const char TEXT_ADJSPD[]  PROGMEM = "ADJSPD";
const char TEXT_ADJSPW[]  PROGMEM = "ADJSPW";
// const char TEXT_ANDB[]    PROGMEM = "ANDB";
// const char TEXT_ANDD[]    PROGMEM = "ANDD";
const char TEXT_ANDW[]    PROGMEM = "ANDW";
const char TEXT_ASHB[]    PROGMEM = "ASHB";
const char TEXT_ASHD[]    PROGMEM = "ASHD";
const char TEXT_ASHW[]    PROGMEM = "ASHW";
// const char TEXT_BCC[]     PROGMEM = "BCC";
// const char TEXT_BCS[]     PROGMEM = "BCS";
// const char TEXT_BEQ[]     PROGMEM = "BEQ";
const char TEXT_BFC[]     PROGMEM = "BFC";
const char TEXT_BFS[]     PROGMEM = "BFS";
// const char TEXT_BGE[]     PROGMEM = "BGE";
// const char TEXT_BGT[]     PROGMEM = "BGT";
// const char TEXT_BHI[]     PROGMEM = "BHI";
// const char TEXT_BHS[]     PROGMEM = "BHS";
const char TEXT_BICB[]    PROGMEM = "BICB";
const char TEXT_BICD[]    PROGMEM = "BICD";
const char TEXT_BICPSRB[] PROGMEM = "BICPSRB";
const char TEXT_BICPSRW[] PROGMEM = "BICPSRW";
const char TEXT_BICW[]    PROGMEM = "BICW";
const char TEXT_BISPSRB[] PROGMEM = "BISPSRB";
const char TEXT_BISPSRW[] PROGMEM = "BISPSRW";
// const char TEXT_BLE[]     PROGMEM = "BLE";
// const char TEXT_BLO[]     PROGMEM = "BLO";
// const char TEXT_BLS[]     PROGMEM = "BLS";
// const char TEXT_BLT[]     PROGMEM = "BLT";
// const char TEXT_BNE[]     PROGMEM = "BNE";
const char TEXT_BPT[]     PROGMEM = "BPT";
// const char TEXT_BR[]      PROGMEM = "BR";
// const char TEXT_BSR[]     PROGMEM = "BSR";
const char TEXT_CASEB[]   PROGMEM = "CASEB";
const char TEXT_CASED[]   PROGMEM = "CASED";
const char TEXT_CASEW[]   PROGMEM = "CASEW";
const char TEXT_CBITB[]   PROGMEM = "CBITB";
const char TEXT_CBITD[]   PROGMEM = "CBITD";
const char TEXT_CBITIB[]  PROGMEM = "CBITIB";
const char TEXT_CBITID[]  PROGMEM = "CBITID";
const char TEXT_CBITIW[]  PROGMEM = "CBITIW";
const char TEXT_CBITW[]   PROGMEM = "CBITW";
const char TEXT_CHECKB[]  PROGMEM = "CHECKB";
const char TEXT_CHECKD[]  PROGMEM = "CHECKD";
const char TEXT_CHECKW[]  PROGMEM = "CHECKW";
const char TEXT_CINV[]    PROGMEM = "CINV";
// const char TEXT_CMPB[]    PROGMEM = "CMPB";
// const char TEXT_CMPD[]    PROGMEM = "CMPD";
const char TEXT_CMPMB[]   PROGMEM = "CMPMB";
const char TEXT_CMPMD[]   PROGMEM = "CMPMD";
const char TEXT_CMPMW[]   PROGMEM = "CMPMW";
const char TEXT_CMPQB[]   PROGMEM = "CMPQB";
const char TEXT_CMPQD[]   PROGMEM = "CMPQD";
const char TEXT_CMPQW[]   PROGMEM = "CMPQW";
// const char TEXT_CMPSB[]   PROGMEM = "CMPSB";
const char TEXT_CMPSD[]   PROGMEM = "CMPSD";
const char TEXT_CMPST[]   PROGMEM = "CMPST";
// const char TEXT_CMPSW[]   PROGMEM = "CMPSW";
// const char TEXT_CMPW[]    PROGMEM = "CMPW";
// const char TEXT_COMB[]    PROGMEM = "COMB";
// const char TEXT_COMD[]    PROGMEM = "COMD";
// const char TEXT_COMW[]    PROGMEM = "COMW";
const char TEXT_CVTP[]    PROGMEM = "CVTP";
const char TEXT_CXP[]     PROGMEM = "CXP";
const char TEXT_CXPD[]    PROGMEM = "CXPD";
const char TEXT_DEIB[]    PROGMEM = "DEIB";
const char TEXT_DEID[]    PROGMEM = "DEID";
const char TEXT_DEIW[]    PROGMEM = "DEIW";
const char TEXT_DIA[]     PROGMEM = "DIA";
// const char TEXT_DIVB[]    PROGMEM = "DIVB";
// const char TEXT_DIVD[]    PROGMEM = "DIVD";
const char TEXT_DIVW[]    PROGMEM = "DIVW";
// const char TEXT_ENTER[]   PROGMEM = "ENTER";
// const char TEXT_EXIT[]    PROGMEM = "EXIT";
// const char TEXT_EXTB[]    PROGMEM = "EXTB";
const char TEXT_EXTD[]    PROGMEM = "EXTD";
// const char TEXT_EXTSB[]   PROGMEM = "EXTSB";
const char TEXT_EXTSD[]   PROGMEM = "EXTSD";
const char TEXT_EXTSW[]   PROGMEM = "EXTSW";
const char TEXT_EXTW[]    PROGMEM = "EXTW";
const char TEXT_FFSB[]    PROGMEM = "FFSB";
const char TEXT_FFSD[]    PROGMEM = "FFSD";
const char TEXT_FFSW[]    PROGMEM = "FFSW";
const char TEXT_FLAG[]    PROGMEM = "FLAG";
const char TEXT_IBITB[]   PROGMEM = "IBITB";
const char TEXT_IBITD[]   PROGMEM = "IBITD";
const char TEXT_IBITW[]   PROGMEM = "IBITW";
const char TEXT_INDEXB[]  PROGMEM = "INDEXB";
const char TEXT_INDEXD[]  PROGMEM = "INDEXD";
const char TEXT_INDEXW[]  PROGMEM = "INDEXW";
// const char TEXT_INSB[]    PROGMEM = "INSB";
const char TEXT_INSD[]    PROGMEM = "INSD";
const char TEXT_INSSB[]   PROGMEM = "INSSB";
const char TEXT_INSSD[]   PROGMEM = "INSSD";
const char TEXT_INSSW[]   PROGMEM = "INSSW";
// const char TEXT_INSW[]    PROGMEM = "INSW";
// const char TEXT_JSR[]     PROGMEM = "JSR";
const char TEXT_JUMP[]    PROGMEM = "JUMP";
const char TEXT_LPRB[]    PROGMEM = "LPRB";
const char TEXT_LPRD[]    PROGMEM = "LPRD";
const char TEXT_LPRW[]    PROGMEM = "LPRW";
const char TEXT_LFSR[]    PROGMEM = "LFSR";
const char TEXT_LSHB[]    PROGMEM = "LSHB";
const char TEXT_LSHD[]    PROGMEM = "LSHD";
const char TEXT_LSHW[]    PROGMEM = "LSHW";
const char TEXT_MEIB[]    PROGMEM = "MEIB";
const char TEXT_MEID[]    PROGMEM = "MEID";
const char TEXT_MEIW[]    PROGMEM = "MEIW";
const char TEXT_MODB[]    PROGMEM = "MODB";
const char TEXT_MODD[]    PROGMEM = "MODD";
const char TEXT_MODW[]    PROGMEM = "MODW";
// const char TEXT_MOVB[]    PROGMEM = "MOVB";
// const char TEXT_MOVD[]    PROGMEM = "MOVD";
const char TEXT_MOVMB[]   PROGMEM = "MOVMB";
const char TEXT_MOVMD[]   PROGMEM = "MOVMD";
const char TEXT_MOVMW[]   PROGMEM = "MOVMW";
const char TEXT_MOVQB[]   PROGMEM = "MOVQB";
const char TEXT_MOVQD[]   PROGMEM = "MOVQD";
const char TEXT_MOVQW[]   PROGMEM = "MOVQW";
// const char TEXT_MOVSB[]   PROGMEM = "MOVSB";
const char TEXT_MOVSD[]   PROGMEM = "MOVSD";
const char TEXT_MOVST[]   PROGMEM = "MOVST";
// const char TEXT_MOVSW[]   PROGMEM = "MOVSW";
const char TEXT_MOVW[]    PROGMEM = "MOVW";
const char TEXT_MOVXBD[]  PROGMEM = "MOVXBD";
const char TEXT_MOVXBW[]  PROGMEM = "MOVXBW";
const char TEXT_MOVXWD[]  PROGMEM = "MOVXWD";
const char TEXT_MOVZBD[]  PROGMEM = "MOVZBD";
const char TEXT_MOVZBW[]  PROGMEM = "MOVZBW";
const char TEXT_MOVZWD[]  PROGMEM = "MOVZWD";
// const char TEXT_MULB[]    PROGMEM = "MULB";
// const char TEXT_MULD[]    PROGMEM = "MULD";
const char TEXT_MULW[]    PROGMEM = "MULW";
// const char TEXT_NEGB[]    PROGMEM = "NEGB";
// const char TEXT_NEGD[]    PROGMEM = "NEGD";
const char TEXT_NEGW[]    PROGMEM = "NEGW";
// const char TEXT_NOP[]     PROGMEM = "NOP";
// const char TEXT_NOTB[]    PROGMEM = "NOTB";
const char TEXT_NOTD[]    PROGMEM = "NOTD";
const char TEXT_NOTW[]    PROGMEM = "NOTW";
// const char TEXT_ORB[]     PROGMEM = "ORB";
// const char TEXT_ORD[]     PROGMEM = "ORD";
const char TEXT_ORW[]     PROGMEM = "ORW";
const char TEXT_QUOB[]    PROGMEM = "QUOB";
const char TEXT_QUOD[]    PROGMEM = "QUOD";
const char TEXT_QUOW[]    PROGMEM = "QUOW";
const char TEXT_REMB[]    PROGMEM = "REMB";
const char TEXT_REMD[]    PROGMEM = "REMD";
const char TEXT_REMW[]    PROGMEM = "REMW";
const char TEXT_RESTORE[] PROGMEM = "RESTORE";
// const char TEXT_RET[]     PROGMEM = "RET";
// const char TEXT_RETI[]    PROGMEM = "RETI";
const char TEXT_RETT[]    PROGMEM = "RETT";
const char TEXT_ROTB[]    PROGMEM = "ROTB";
const char TEXT_ROTD[]    PROGMEM = "ROTD";
const char TEXT_ROTW[]    PROGMEM = "ROTW";
const char TEXT_RXP[]     PROGMEM = "RXP";
const char TEXT_SAVE[]    PROGMEM = "SAVE";
const char TEXT_SBITB[]   PROGMEM = "SBITB";
const char TEXT_SBITD[]   PROGMEM = "SBITD";
const char TEXT_SBITIB[]  PROGMEM = "SBITIB";
const char TEXT_SBITID[]  PROGMEM = "SBITID";
const char TEXT_SBITIW[]  PROGMEM = "SBITIW";
const char TEXT_SBITW[]   PROGMEM = "SBITW";
const char TEXT_SCCB[]    PROGMEM = "SCCB";
const char TEXT_SCCD[]    PROGMEM = "SCCD";
const char TEXT_SCCW[]    PROGMEM = "SCCW";
const char TEXT_SCSB[]    PROGMEM = "SCSB";
const char TEXT_SCSD[]    PROGMEM = "SCSD";
const char TEXT_SCSW[]    PROGMEM = "SCSW";
const char TEXT_SEQB[]    PROGMEM = "SEQB";
const char TEXT_SEQD[]    PROGMEM = "SEQD";
const char TEXT_SEQW[]    PROGMEM = "SEQW";
const char TEXT_SETCFG[]  PROGMEM = "SETCFG";
const char TEXT_SFCB[]    PROGMEM = "SFCB";
const char TEXT_SFCD[]    PROGMEM = "SFCD";
const char TEXT_SFCW[]    PROGMEM = "SFCW";
const char TEXT_SFSB[]    PROGMEM = "SFSB";
const char TEXT_SFSD[]    PROGMEM = "SFSD";
const char TEXT_SFSR[]    PROGMEM = "SFSR";
const char TEXT_SFSW[]    PROGMEM = "SFSW";
const char TEXT_SGEB[]    PROGMEM = "SGEB";
const char TEXT_SGED[]    PROGMEM = "SGED";
const char TEXT_SGEW[]    PROGMEM = "SGEW";
const char TEXT_SGTB[]    PROGMEM = "SGTB";
const char TEXT_SGTD[]    PROGMEM = "SGTD";
const char TEXT_SGTW[]    PROGMEM = "SGTW";
const char TEXT_SHIB[]    PROGMEM = "SHIB";
const char TEXT_SHID[]    PROGMEM = "SHID";
const char TEXT_SHIW[]    PROGMEM = "SHIW";
const char TEXT_SHSB[]    PROGMEM = "SHSB";
const char TEXT_SHSD[]    PROGMEM = "SHSD";
const char TEXT_SHSW[]    PROGMEM = "SHSW";
const char TEXT_SKPSB[]   PROGMEM = "SKPSB";
const char TEXT_SKPSD[]   PROGMEM = "SKPSD";
const char TEXT_SKPST[]   PROGMEM = "SKPST";
const char TEXT_SKPSW[]   PROGMEM = "SKPSW";
const char TEXT_SLEB[]    PROGMEM = "SLEB";
const char TEXT_SLED[]    PROGMEM = "SLED";
const char TEXT_SLEW[]    PROGMEM = "SLEW";
const char TEXT_SLOB[]    PROGMEM = "SLOB";
const char TEXT_SLOD[]    PROGMEM = "SLOD";
const char TEXT_SLOW[]    PROGMEM = "SLOW";
const char TEXT_SLSB[]    PROGMEM = "SLSB";
const char TEXT_SLSD[]    PROGMEM = "SLSD";
const char TEXT_SLSW[]    PROGMEM = "SLSW";
const char TEXT_SLTB[]    PROGMEM = "SLTB";
const char TEXT_SLTD[]    PROGMEM = "SLTD";
const char TEXT_SLTW[]    PROGMEM = "SLTW";
const char TEXT_SNEB[]    PROGMEM = "SNEB";
const char TEXT_SNED[]    PROGMEM = "SNED";
const char TEXT_SNEW[]    PROGMEM = "SNEW";
const char TEXT_SPRB[]    PROGMEM = "SPRB";
const char TEXT_SPRD[]    PROGMEM = "SPRD";
const char TEXT_SPRW[]    PROGMEM = "SPRW";
// const char TEXT_SUBB[]    PROGMEM = "SUBB";
// const char TEXT_SUBCB[]   PROGMEM = "SUBCB";
const char TEXT_SUBCD[]   PROGMEM = "SUBCD";
const char TEXT_SUBCW[]   PROGMEM = "SUBCW";
// const char TEXT_SUBD[]    PROGMEM = "SUBD";
const char TEXT_SUBPB[]   PROGMEM = "SUBPB";
const char TEXT_SUBPD[]   PROGMEM = "SUBPD";
const char TEXT_SUBPW[]   PROGMEM = "SUBPW";
// const char TEXT_SUBW[]    PROGMEM = "SUBW";
// const char TEXT_SVC[]     PROGMEM = "SVC";
const char TEXT_TBITB[]   PROGMEM = "TBITB";
const char TEXT_TBITD[]   PROGMEM = "TBITD";
const char TEXT_TBITW[]   PROGMEM = "TBITW";
// const char TEXT_WAIT[]    PROGMEM = "WAIT";
// const char TEXT_XORB[]    PROGMEM = "XORB";
const char TEXT_XORD[]    PROGMEM = "XORD";
const char TEXT_XORW[]    PROGMEM = "XORW";
// Floating point instructions
const char TEXT_ABSF[]    PROGMEM = "ABSF";
const char TEXT_ABSL[]    PROGMEM = "ABSL";
// const char TEXT_ADDF[]    PROGMEM = "ADDF";
// const char TEXT_ADDL[]    PROGMEM = "ADDL";
// const char TEXT_CMPF[]    PROGMEM = "CMPF";
// const char TEXT_CMPL[]    PROGMEM = "CMPL";
const char TEXT_DIVF[]    PROGMEM = "DIVF";
// const char TEXT_DIVL[]    PROGMEM = "DIVL";
const char TEXT_FLOORFB[] PROGMEM = "FLOORFB";
const char TEXT_FLOORFD[] PROGMEM = "FLOORFD";
const char TEXT_FLOORFW[] PROGMEM = "FLOORFW";
const char TEXT_FLOORLB[] PROGMEM = "FLOORLB";
const char TEXT_FLOORLD[] PROGMEM = "FLOORLD";
const char TEXT_FLOORLW[] PROGMEM = "FLOORLW";
const char TEXT_MOVBF[]   PROGMEM = "MOVBF";
const char TEXT_MOVBL[]   PROGMEM = "MOVBL";
const char TEXT_MOVDF[]   PROGMEM = "MOVDF";
const char TEXT_MOVDL[]   PROGMEM = "MOVDL";
const char TEXT_MOVF[]    PROGMEM = "MOVF";
const char TEXT_MOVFL[]   PROGMEM = "MOVFL";
const char TEXT_MOVL[]    PROGMEM = "MOVL";
const char TEXT_MOVLF[]   PROGMEM = "MOVLF";
const char TEXT_MOVWF[]   PROGMEM = "MOVWF";
const char TEXT_MOVWL[]   PROGMEM = "MOVWL";
const char TEXT_MULF[]    PROGMEM = "MULF";
const char TEXT_MULL[]    PROGMEM = "MULL";
const char TEXT_NEGF[]    PROGMEM = "NEGF";
const char TEXT_NEGL[]    PROGMEM = "NEGL";
const char TEXT_ROUNDFB[] PROGMEM = "ROUNDFB";
const char TEXT_ROUNDFD[] PROGMEM = "ROUNDFD";
const char TEXT_ROUNDFW[] PROGMEM = "ROUNDFW";
const char TEXT_ROUNDLB[] PROGMEM = "ROUNDLB";
const char TEXT_ROUNDLD[] PROGMEM = "ROUNDLD";
const char TEXT_ROUNDLW[] PROGMEM = "ROUNDLW";
// const char TEXT_SUBF[]    PROGMEM = "SUBF";
// const char TEXT_SUBL[]    PROGMEM = "SUBL";
const char TEXT_TRUNCFB[] PROGMEM = "TRUNCFB";
const char TEXT_TRUNCFD[] PROGMEM = "TRUNCFD";
const char TEXT_TRUNCFW[] PROGMEM = "TRUNCFW";
const char TEXT_TRUNCLB[] PROGMEM = "TRUNCLB";
const char TEXT_TRUNCLD[] PROGMEM = "TRUNCLD";
const char TEXT_TRUNCLW[] PROGMEM = "TRUNCLW";
// Memory management instructions
const char TEXT_LMR[]     PROGMEM = "LMR";
const char TEXT_MOVSUB[]  PROGMEM = "MOVSUB";
const char TEXT_MOVSUD[]  PROGMEM = "MOVSUD";
const char TEXT_MOVSUW[]  PROGMEM = "MOVSUW";
const char TEXT_MOVUSB[]  PROGMEM = "MOVUSB";
const char TEXT_MOVUSD[]  PROGMEM = "MOVUSD";
const char TEXT_MOVUSW[]  PROGMEM = "MOVUSW";
const char TEXT_RDVAL[]   PROGMEM = "RDVAL";
const char TEXT_SMR[]     PROGMEM = "SMR";
const char TEXT_WRVAL[]   PROGMEM = "WRVAL";

// const char TEXT_REG_R0[] PROGMEM = "R0";
// const char TEXT_REG_R1[] PROGMEM = "R1";
// const char TEXT_REG_R2[] PROGMEM = "R2";
// const char TEXT_REG_R3[] PROGMEM = "R3";
// const char TEXT_REG_R4[] PROGMEM = "R4";
// const char TEXT_REG_R5[] PROGMEM = "R5";
// const char TEXT_REG_R6[] PROGMEM = "R6";
// const char TEXT_REG_R7[] PROGMEM = "R7";

const char TEXT_REG_FP[]  PROGMEM = "FP";
// const char TEXT_REG_SP[]  PROGMEM = "SP";
const char TEXT_REG_SB[]  PROGMEM = "SB";
// const char TEXT_REG_PC[]  PROGMEM = "PC";
const char TEXT_REG_TOS[] PROGMEM = "TOS";
const char TEXT_REG_EXT[] PROGMEM = "EXT";

// NS32081/FPU Registers
// const char TEXT_REG_F0[] PROGMEM = "F0";
// const char TEXT_REG_F1[] PROGMEM = "F1";
const char TEXT_REG_F2[] PROGMEM = "F2";
const char TEXT_REG_F3[] PROGMEM = "F3";
const char TEXT_REG_F4[] PROGMEM = "F4";
const char TEXT_REG_F5[] PROGMEM = "F5";
const char TEXT_REG_F6[] PROGMEM = "F6";
const char TEXT_REG_F7[] PROGMEM = "F7";

const char TEXT_PREG_UPSR[]    PROGMEM = "UPSR";
const char TEXT_PREG_US[]      PROGMEM = "US";
const char TEXT_PREG_PSR[]     PROGMEM = "PSR";
const char TEXT_PREG_INTBASE[] PROGMEM = "INTBASE";
const char TEXT_PREG_MOD[]     PROGMEM = "MOD";

// NS32082/MMU registers.
const char TEXT_MREG_BPR0[] PROGMEM = "BPR0";
const char TEXT_MREG_BPR1[] PROGMEM = "BPR1";
const char TEXT_MREG_MSR[]  PROGMEM = "MSR";
const char TEXT_MREG_BCNT[] PROGMEM = "BCNT";
const char TEXT_MREG_PTB0[] PROGMEM = "PTB0";
const char TEXT_MREG_PTB1[] PROGMEM = "PTB1";
const char TEXT_MREG_EIA[]  PROGMEM = "EIA";

const char TEXT_CONFIG_I[] PROGMEM = "I";
const char TEXT_CONFIG_F[] PROGMEM = "F";
const char TEXT_CONFIG_M[] PROGMEM = "M";
const char TEXT_CONFIG_C[] PROGMEM = "C";

const char TEXT_STROPT_B[] PROGMEM = "B";
const char TEXT_STROPT_W[] PROGMEM = "W";
const char TEXT_STROPT_U[] PROGMEM = "U";
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
