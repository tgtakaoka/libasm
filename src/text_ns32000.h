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

#include "config_host.h"

namespace libasm {
namespace ns32000 {

static const char TEXT_CPU_NS32032[] PROGMEM = "NS32032";
static constexpr const char *TEXT_CPU_LIST  = TEXT_CPU_NS32032;
static constexpr const char *TEXT_CPU_NS    = TEXT_CPU_NS32032;
static constexpr const char *TEXT_CPU_32032 = (TEXT_CPU_NS32032 + 2);
static const char TEXT_CPU_32016[] PROGMEM = "32016";
static const char TEXT_CPU_32008[] PROGMEM = "32008";

static const char TEXT_ABSB[]    PROGMEM = "ABSB";
static const char TEXT_ABSD[]    PROGMEM = "ABSD";
static const char TEXT_ABSW[]    PROGMEM = "ABSW";
static const char TEXT_ACBB[]    PROGMEM = "ACBB";
static const char TEXT_ACBD[]    PROGMEM = "ACBD";
static const char TEXT_ACBW[]    PROGMEM = "ACBW";
static const char TEXT_ADDB[]    PROGMEM = "ADDB";
static const char TEXT_ADDCB[]   PROGMEM = "ADDCB";
static const char TEXT_ADDCD[]   PROGMEM = "ADDCD";
static const char TEXT_ADDCW[]   PROGMEM = "ADDCW";
static const char TEXT_ADDD[]    PROGMEM = "ADDD";
static const char TEXT_ADDPB[]   PROGMEM = "ADDPB";
static const char TEXT_ADDPD[]   PROGMEM = "ADDPD";
static const char TEXT_ADDPW[]   PROGMEM = "ADDPW";
static const char TEXT_ADDQB[]   PROGMEM = "ADDQB";
static const char TEXT_ADDQD[]   PROGMEM = "ADDQD";
static const char TEXT_ADDQW[]   PROGMEM = "ADDQW";
static const char TEXT_ADDR[]    PROGMEM = "ADDR";
static const char TEXT_ADDW[]    PROGMEM = "ADDW";
static const char TEXT_ADJSPB[]  PROGMEM = "ADJSPB";
static const char TEXT_ADJSPD[]  PROGMEM = "ADJSPD";
static const char TEXT_ADJSPW[]  PROGMEM = "ADJSPW";
static const char TEXT_ANDB[]    PROGMEM = "ANDB";
static const char TEXT_ANDD[]    PROGMEM = "ANDD";
static const char TEXT_ANDW[]    PROGMEM = "ANDW";
static const char TEXT_ASHB[]    PROGMEM = "ASHB";
static const char TEXT_ASHD[]    PROGMEM = "ASHD";
static const char TEXT_ASHW[]    PROGMEM = "ASHW";
static const char TEXT_BCC[]     PROGMEM = "BCC";
static const char TEXT_BCS[]     PROGMEM = "BCS";
static const char TEXT_BEQ[]     PROGMEM = "BEQ";
static const char TEXT_BFC[]     PROGMEM = "BFC";
static const char TEXT_BFS[]     PROGMEM = "BFS";
static const char TEXT_BGE[]     PROGMEM = "BGE";
static const char TEXT_BGT[]     PROGMEM = "BGT";
static const char TEXT_BHI[]     PROGMEM = "BHI";
static const char TEXT_BHS[]     PROGMEM = "BHS";
static const char TEXT_BICB[]    PROGMEM = "BICB";
static const char TEXT_BICD[]    PROGMEM = "BICD";
static const char TEXT_BICPSRB[] PROGMEM = "BICPSRB";
static const char TEXT_BICPSRW[] PROGMEM = "BICPSRW";
static const char TEXT_BICW[]    PROGMEM = "BICW";
static const char TEXT_BISPSRB[] PROGMEM = "BISPSRB";
static const char TEXT_BISPSRW[] PROGMEM = "BISPSRW";
static const char TEXT_BLE[]     PROGMEM = "BLE";
static const char TEXT_BLO[]     PROGMEM = "BLO";
static const char TEXT_BLS[]     PROGMEM = "BLS";
static const char TEXT_BLT[]     PROGMEM = "BLT";
static const char TEXT_BNE[]     PROGMEM = "BNE";
static const char TEXT_BPT[]     PROGMEM = "BPT";
static const char TEXT_BR[]      PROGMEM = "BR";
static const char TEXT_BSR[]     PROGMEM = "BSR";
static const char TEXT_CASEB[]   PROGMEM = "CASEB";
static const char TEXT_CASED[]   PROGMEM = "CASED";
static const char TEXT_CASEW[]   PROGMEM = "CASEW";
static const char TEXT_CBITB[]   PROGMEM = "CBITB";
static const char TEXT_CBITD[]   PROGMEM = "CBITD";
static const char TEXT_CBITIB[]  PROGMEM = "CBITIB";
static const char TEXT_CBITID[]  PROGMEM = "CBITID";
static const char TEXT_CBITIW[]  PROGMEM = "CBITIW";
static const char TEXT_CBITW[]   PROGMEM = "CBITW";
static const char TEXT_CHECKB[]  PROGMEM = "CHECKB";
static const char TEXT_CHECKD[]  PROGMEM = "CHECKD";
static const char TEXT_CHECKW[]  PROGMEM = "CHECKW";
static const char TEXT_CINV[]    PROGMEM = "CINV";
static const char TEXT_CMPB[]    PROGMEM = "CMPB";
static const char TEXT_CMPD[]    PROGMEM = "CMPD";
static const char TEXT_CMPMB[]   PROGMEM = "CMPMB";
static const char TEXT_CMPMD[]   PROGMEM = "CMPMD";
static const char TEXT_CMPMW[]   PROGMEM = "CMPMW";
static const char TEXT_CMPQB[]   PROGMEM = "CMPQB";
static const char TEXT_CMPQD[]   PROGMEM = "CMPQD";
static const char TEXT_CMPQW[]   PROGMEM = "CMPQW";
static const char TEXT_CMPSB[]   PROGMEM = "CMPSB";
static const char TEXT_CMPSD[]   PROGMEM = "CMPSD";
static const char TEXT_CMPST[]   PROGMEM = "CMPST";
static const char TEXT_CMPSW[]   PROGMEM = "CMPSW";
static const char TEXT_CMPW[]    PROGMEM = "CMPW";
static const char TEXT_COMB[]    PROGMEM = "COMB";
static const char TEXT_COMD[]    PROGMEM = "COMD";
static const char TEXT_COMW[]    PROGMEM = "COMW";
static const char TEXT_CVTP[]    PROGMEM = "CVTP";
static const char TEXT_CXP[]     PROGMEM = "CXP";
static const char TEXT_CXPD[]    PROGMEM = "CXPD";
static const char TEXT_DEIB[]    PROGMEM = "DEIB";
static const char TEXT_DEID[]    PROGMEM = "DEID";
static const char TEXT_DEIW[]    PROGMEM = "DEIW";
static const char TEXT_DIA[]     PROGMEM = "DIA";
static const char TEXT_DIVB[]    PROGMEM = "DIVB";
static const char TEXT_DIVD[]    PROGMEM = "DIVD";
static const char TEXT_DIVW[]    PROGMEM = "DIVW";
static const char TEXT_ENTER[]   PROGMEM = "ENTER";
static const char TEXT_EXIT[]    PROGMEM = "EXIT";
static const char TEXT_EXTB[]    PROGMEM = "EXTB";
static const char TEXT_EXTD[]    PROGMEM = "EXTD";
static const char TEXT_EXTSB[]   PROGMEM = "EXTSB";
static const char TEXT_EXTSD[]   PROGMEM = "EXTSD";
static const char TEXT_EXTSW[]   PROGMEM = "EXTSW";
static const char TEXT_EXTW[]    PROGMEM = "EXTW";
static const char TEXT_FFSB[]    PROGMEM = "FFSB";
static const char TEXT_FFSD[]    PROGMEM = "FFSD";
static const char TEXT_FFSW[]    PROGMEM = "FFSW";
static const char TEXT_FLAG[]    PROGMEM = "FLAG";
static const char TEXT_IBITB[]   PROGMEM = "IBITB";
static const char TEXT_IBITD[]   PROGMEM = "IBITD";
static const char TEXT_IBITW[]   PROGMEM = "IBITW";
static const char TEXT_INDEXB[]  PROGMEM = "INDEXB";
static const char TEXT_INDEXD[]  PROGMEM = "INDEXD";
static const char TEXT_INDEXW[]  PROGMEM = "INDEXW";
static const char TEXT_INSB[]    PROGMEM = "INSB";
static const char TEXT_INSD[]    PROGMEM = "INSD";
static const char TEXT_INSSB[]   PROGMEM = "INSSB";
static const char TEXT_INSSD[]   PROGMEM = "INSSD";
static const char TEXT_INSSW[]   PROGMEM = "INSSW";
static const char TEXT_INSW[]    PROGMEM = "INSW";
static const char TEXT_JSR[]     PROGMEM = "JSR";
static const char TEXT_JUMP[]    PROGMEM = "JUMP";
static const char TEXT_LPRB[]    PROGMEM = "LPRB";
static const char TEXT_LPRD[]    PROGMEM = "LPRD";
static const char TEXT_LPRW[]    PROGMEM = "LPRW";
static const char TEXT_LFSR[]    PROGMEM = "LFSR";
static const char TEXT_LSHB[]    PROGMEM = "LSHB";
static const char TEXT_LSHD[]    PROGMEM = "LSHD";
static const char TEXT_LSHW[]    PROGMEM = "LSHW";
static const char TEXT_MEIB[]    PROGMEM = "MEIB";
static const char TEXT_MEID[]    PROGMEM = "MEID";
static const char TEXT_MEIW[]    PROGMEM = "MEIW";
static const char TEXT_MODB[]    PROGMEM = "MODB";
static const char TEXT_MODD[]    PROGMEM = "MODD";
static const char TEXT_MODW[]    PROGMEM = "MODW";
static const char TEXT_MOVB[]    PROGMEM = "MOVB";
static const char TEXT_MOVD[]    PROGMEM = "MOVD";
static const char TEXT_MOVMB[]   PROGMEM = "MOVMB";
static const char TEXT_MOVMD[]   PROGMEM = "MOVMD";
static const char TEXT_MOVMW[]   PROGMEM = "MOVMW";
static const char TEXT_MOVQB[]   PROGMEM = "MOVQB";
static const char TEXT_MOVQD[]   PROGMEM = "MOVQD";
static const char TEXT_MOVQW[]   PROGMEM = "MOVQW";
static const char TEXT_MOVSB[]   PROGMEM = "MOVSB";
static const char TEXT_MOVSD[]   PROGMEM = "MOVSD";
static const char TEXT_MOVST[]   PROGMEM = "MOVST";
static const char TEXT_MOVSUB[]  PROGMEM = "MOVSUB";
static const char TEXT_MOVSUD[]  PROGMEM = "MOVSUD";
static const char TEXT_MOVSUW[]  PROGMEM = "MOVSUW";
static const char TEXT_MOVSW[]   PROGMEM = "MOVSW";
static const char TEXT_MOVUSB[]  PROGMEM = "MOVUSB";
static const char TEXT_MOVUSD[]  PROGMEM = "MOVUSD";
static const char TEXT_MOVUSW[]  PROGMEM = "MOVUSW";
static const char TEXT_MOVW[]    PROGMEM = "MOVW";
static const char TEXT_MOVXBD[]  PROGMEM = "MOVXBD";
static const char TEXT_MOVXBW[]  PROGMEM = "MOVXBW";
static const char TEXT_MOVXWD[]  PROGMEM = "MOVXWD";
static const char TEXT_MOVZBD[]  PROGMEM = "MOVZBD";
static const char TEXT_MOVZBW[]  PROGMEM = "MOVZBW";
static const char TEXT_MOVZWD[]  PROGMEM = "MOVZWD";
static const char TEXT_MULB[]    PROGMEM = "MULB";
static const char TEXT_MULD[]    PROGMEM = "MULD";
static const char TEXT_MULW[]    PROGMEM = "MULW";
static const char TEXT_NEGB[]    PROGMEM = "NEGB";
static const char TEXT_NEGD[]    PROGMEM = "NEGD";
static const char TEXT_NEGW[]    PROGMEM = "NEGW";
static const char TEXT_NOP[]     PROGMEM = "NOP";
static const char TEXT_NOTB[]    PROGMEM = "NOTB";
static const char TEXT_NOTD[]    PROGMEM = "NOTD";
static const char TEXT_NOTW[]    PROGMEM = "NOTW";
static const char TEXT_ORB[]     PROGMEM = "ORB";
static const char TEXT_ORD[]     PROGMEM = "ORD";
static const char TEXT_ORW[]     PROGMEM = "ORW";
static const char TEXT_QUOB[]    PROGMEM = "QUOB";
static const char TEXT_QUOD[]    PROGMEM = "QUOD";
static const char TEXT_QUOW[]    PROGMEM = "QUOW";
static const char TEXT_REMB[]    PROGMEM = "REMB";
static const char TEXT_REMD[]    PROGMEM = "REMD";
static const char TEXT_REMW[]    PROGMEM = "REMW";
static const char TEXT_RESTORE[] PROGMEM = "RESTORE";
static const char TEXT_RET[]     PROGMEM = "RET";
static const char TEXT_RETI[]    PROGMEM = "RETI";
static const char TEXT_RETT[]    PROGMEM = "RETT";
static const char TEXT_ROTB[]    PROGMEM = "ROTB";
static const char TEXT_ROTD[]    PROGMEM = "ROTD";
static const char TEXT_ROTW[]    PROGMEM = "ROTW";
static const char TEXT_RXP[]     PROGMEM = "RXP";
static const char TEXT_SAVE[]    PROGMEM = "SAVE";
static const char TEXT_SBITB[]   PROGMEM = "SBITB";
static const char TEXT_SBITD[]   PROGMEM = "SBITD";
static const char TEXT_SBITIB[]  PROGMEM = "SBITIB";
static const char TEXT_SBITID[]  PROGMEM = "SBITID";
static const char TEXT_SBITIW[]  PROGMEM = "SBITIW";
static const char TEXT_SBITW[]   PROGMEM = "SBITW";
static const char TEXT_SCCB[]    PROGMEM = "SCCB";
static const char TEXT_SCCD[]    PROGMEM = "SCCD";
static const char TEXT_SCCW[]    PROGMEM = "SCCW";
static const char TEXT_SCSB[]    PROGMEM = "SCSB";
static const char TEXT_SCSD[]    PROGMEM = "SCSD";
static const char TEXT_SCSW[]    PROGMEM = "SCSW";
static const char TEXT_SEQB[]    PROGMEM = "SEQB";
static const char TEXT_SEQD[]    PROGMEM = "SEQD";
static const char TEXT_SEQW[]    PROGMEM = "SEQW";
static const char TEXT_SETCFG[]  PROGMEM = "SETCFG";
static const char TEXT_SFCB[]    PROGMEM = "SFCB";
static const char TEXT_SFCD[]    PROGMEM = "SFCD";
static const char TEXT_SFCW[]    PROGMEM = "SFCW";
static const char TEXT_SFSB[]    PROGMEM = "SFSB";
static const char TEXT_SFSD[]    PROGMEM = "SFSD";
static const char TEXT_SFSR[]    PROGMEM = "SFSR";
static const char TEXT_SFSW[]    PROGMEM = "SFSW";
static const char TEXT_SGEB[]    PROGMEM = "SGEB";
static const char TEXT_SGED[]    PROGMEM = "SGED";
static const char TEXT_SGEW[]    PROGMEM = "SGEW";
static const char TEXT_SGTB[]    PROGMEM = "SGTB";
static const char TEXT_SGTD[]    PROGMEM = "SGTD";
static const char TEXT_SGTW[]    PROGMEM = "SGTW";
static const char TEXT_SHIB[]    PROGMEM = "SHIB";
static const char TEXT_SHID[]    PROGMEM = "SHID";
static const char TEXT_SHIW[]    PROGMEM = "SHIW";
static const char TEXT_SHSB[]    PROGMEM = "SHSB";
static const char TEXT_SHSD[]    PROGMEM = "SHSD";
static const char TEXT_SHSW[]    PROGMEM = "SHSW";
static const char TEXT_SKPSB[]   PROGMEM = "SKPSB";
static const char TEXT_SKPSD[]   PROGMEM = "SKPSD";
static const char TEXT_SKPST[]   PROGMEM = "SKPST";
static const char TEXT_SKPSW[]   PROGMEM = "SKPSW";
static const char TEXT_SLEB[]    PROGMEM = "SLEB";
static const char TEXT_SLED[]    PROGMEM = "SLED";
static const char TEXT_SLEW[]    PROGMEM = "SLEW";
static const char TEXT_SLOB[]    PROGMEM = "SLOB";
static const char TEXT_SLOD[]    PROGMEM = "SLOD";
static const char TEXT_SLOW[]    PROGMEM = "SLOW";
static const char TEXT_SLSB[]    PROGMEM = "SLSB";
static const char TEXT_SLSD[]    PROGMEM = "SLSD";
static const char TEXT_SLSW[]    PROGMEM = "SLSW";
static const char TEXT_SLTB[]    PROGMEM = "SLTB";
static const char TEXT_SLTD[]    PROGMEM = "SLTD";
static const char TEXT_SLTW[]    PROGMEM = "SLTW";
static const char TEXT_SNEB[]    PROGMEM = "SNEB";
static const char TEXT_SNED[]    PROGMEM = "SNED";
static const char TEXT_SNEW[]    PROGMEM = "SNEW";
static const char TEXT_SPRB[]    PROGMEM = "SPRB";
static const char TEXT_SPRD[]    PROGMEM = "SPRD";
static const char TEXT_SPRW[]    PROGMEM = "SPRW";
static const char TEXT_SUBB[]    PROGMEM = "SUBB";
static const char TEXT_SUBCB[]   PROGMEM = "SUBCB";
static const char TEXT_SUBCD[]   PROGMEM = "SUBCD";
static const char TEXT_SUBCW[]   PROGMEM = "SUBCW";
static const char TEXT_SUBD[]    PROGMEM = "SUBD";
static const char TEXT_SUBPB[]   PROGMEM = "SUBPB";
static const char TEXT_SUBPD[]   PROGMEM = "SUBPD";
static const char TEXT_SUBPW[]   PROGMEM = "SUBPW";
static const char TEXT_SUBW[]    PROGMEM = "SUBW";
static const char TEXT_SVC[]     PROGMEM = "SVC";
static const char TEXT_TBITB[]   PROGMEM = "TBITB";
static const char TEXT_TBITD[]   PROGMEM = "TBITD";
static const char TEXT_TBITW[]   PROGMEM = "TBITW";
static const char TEXT_WAIT[]    PROGMEM = "WAIT";
static const char TEXT_XORB[]    PROGMEM = "XORB";
static const char TEXT_XORD[]    PROGMEM = "XORD";
static const char TEXT_XORW[]    PROGMEM = "XORW";
#ifdef ENABLE_FLOAT
static const char TEXT_ABSF[]    PROGMEM = "ABSF";
static const char TEXT_ABSL[]    PROGMEM = "ABSL";
static const char TEXT_ADDF[]    PROGMEM = "ADDF";
static const char TEXT_ADDL[]    PROGMEM = "ADDL";
static const char TEXT_CMPF[]    PROGMEM = "CMPF";
static const char TEXT_CMPL[]    PROGMEM = "CMPL";
static const char TEXT_DIVF[]    PROGMEM = "DIVF";
static const char TEXT_DIVL[]    PROGMEM = "DIVL";
static const char TEXT_FLOORFB[] PROGMEM = "FLOORFB";
static const char TEXT_FLOORFD[] PROGMEM = "FLOORFD";
static const char TEXT_FLOORFW[] PROGMEM = "FLOORFW";
static const char TEXT_FLOORLB[] PROGMEM = "FLOORLB";
static const char TEXT_FLOORLD[] PROGMEM = "FLOORLD";
static const char TEXT_FLOORLW[] PROGMEM = "FLOORLW";
static const char TEXT_MOVBF[]   PROGMEM = "MOVBF";
static const char TEXT_MOVBL[]   PROGMEM = "MOVBL";
static const char TEXT_MOVDF[]   PROGMEM = "MOVDF";
static const char TEXT_MOVDL[]   PROGMEM = "MOVDL";
static const char TEXT_MOVF[]    PROGMEM = "MOVF";
static const char TEXT_MOVFL[]   PROGMEM = "MOVFL";
static const char TEXT_MOVL[]    PROGMEM = "MOVL";
static const char TEXT_MOVLF[]   PROGMEM = "MOVLF";
static const char TEXT_MOVWF[]   PROGMEM = "MOVWF";
static const char TEXT_MOVWL[]   PROGMEM = "MOVWL";
static const char TEXT_MULF[]    PROGMEM = "MULF";
static const char TEXT_MULL[]    PROGMEM = "MULL";
static const char TEXT_NEGF[]    PROGMEM = "NEGF";
static const char TEXT_NEGL[]    PROGMEM = "NEGL";
static const char TEXT_ROUNDFB[] PROGMEM = "ROUNDFB";
static const char TEXT_ROUNDFD[] PROGMEM = "ROUNDFD";
static const char TEXT_ROUNDFW[] PROGMEM = "ROUNDFW";
static const char TEXT_ROUNDLB[] PROGMEM = "ROUNDLB";
static const char TEXT_ROUNDLD[] PROGMEM = "ROUNDLD";
static const char TEXT_ROUNDLW[] PROGMEM = "ROUNDLW";
static const char TEXT_SUBF[]    PROGMEM = "SUBF";
static const char TEXT_SUBL[]    PROGMEM = "SUBL";
static const char TEXT_TRUNCFB[] PROGMEM = "TRUNCFB";
static const char TEXT_TRUNCFD[] PROGMEM = "TRUNCFD";
static const char TEXT_TRUNCFW[] PROGMEM = "TRUNCFW";
static const char TEXT_TRUNCLB[] PROGMEM = "TRUNCLB";
static const char TEXT_TRUNCLD[] PROGMEM = "TRUNCLD";
static const char TEXT_TRUNCLW[] PROGMEM = "TRUNCLW";
#endif
#ifdef ENABLE_MMU
static const char TEXT_LMR[]     PROGMEM = "LMR";
static const char TEXT_RDVAL[]   PROGMEM = "RDVAL";
static const char TEXT_SMR[]     PROGMEM = "SMR";
static const char TEXT_WRVAL[]   PROGMEM = "WRVAL";
#endif

} // namespace ns32000
} // namespace libasm

#endif // __TEXT_NS32000__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
