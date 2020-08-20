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

static const char TEXT_ABSB[]    = "ABSB";
static const char TEXT_ABSD[]    = "ABSD";
static const char TEXT_ABSF[]    = "ABSF";
static const char TEXT_ABSL[]    = "ABSL";
static const char TEXT_ABSW[]    = "ABSW";
static const char TEXT_ACBB[]    = "ACBB";
static const char TEXT_ACBD[]    = "ACBD";
static const char TEXT_ACBW[]    = "ACBW";
static const char TEXT_ADDB[]    = "ADDB";
static const char TEXT_ADDCB[]   = "ADDCB";
static const char TEXT_ADDCD[]   = "ADDCD";
static const char TEXT_ADDCW[]   = "ADDCW";
static const char TEXT_ADDD[]    = "ADDD";
static const char TEXT_ADDF[]    = "ADDF";
static const char TEXT_ADDL[]    = "ADDL";
static const char TEXT_ADDPB[]   = "ADDPB";
static const char TEXT_ADDPD[]   = "ADDPD";
static const char TEXT_ADDPW[]   = "ADDPW";
static const char TEXT_ADDQB[]   = "ADDQB";
static const char TEXT_ADDQD[]   = "ADDQD";
static const char TEXT_ADDQW[]   = "ADDQW";
static const char TEXT_ADDR[]    = "ADDR";
static const char TEXT_ADDW[]    = "ADDW";
static const char TEXT_ADJSPB[]  = "ADJSPB";
static const char TEXT_ADJSPD[]  = "ADJSPD";
static const char TEXT_ADJSPW[]  = "ADJSPW";
static const char TEXT_ANDB[]    = "ANDB";
static const char TEXT_ANDD[]    = "ANDD";
static const char TEXT_ANDW[]    = "ANDW";
static const char TEXT_ASHB[]    = "ASHB";
static const char TEXT_ASHD[]    = "ASHD";
static const char TEXT_ASHW[]    = "ASHW";
static const char TEXT_BCC[]     = "BCC";
static const char TEXT_BCS[]     = "BCS";
static const char TEXT_BEQ[]     = "BEQ";
static const char TEXT_BFC[]     = "BFC";
static const char TEXT_BFS[]     = "BFS";
static const char TEXT_BGE[]     = "BGE";
static const char TEXT_BGT[]     = "BGT";
static const char TEXT_BHI[]     = "BHI";
static const char TEXT_BHS[]     = "BHS";
static const char TEXT_BICB[]    = "BICB";
static const char TEXT_BICD[]    = "BICD";
static const char TEXT_BICPSRB[] = "BICPSRB";
static const char TEXT_BICPSRW[] = "BICPSRW";
static const char TEXT_BICW[]    = "BICW";
static const char TEXT_BISPSRB[] = "BISPSRB";
static const char TEXT_BISPSRW[] = "BISPSRW";
static const char TEXT_BLE[]     = "BLE";
static const char TEXT_BLO[]     = "BLO";
static const char TEXT_BLS[]     = "BLS";
static const char TEXT_BLT[]     = "BLT";
static const char TEXT_BNE[]     = "BNE";
static const char TEXT_BPT[]     = "BPT";
static const char TEXT_BR[]      = "BR";
static const char TEXT_BSR[]     = "BSR";
static const char TEXT_CASEB[]   = "CASEB";
static const char TEXT_CASED[]   = "CASED";
static const char TEXT_CASEW[]   = "CASEW";
static const char TEXT_CBITB[]   = "CBITB";
static const char TEXT_CBITD[]   = "CBITD";
static const char TEXT_CBITIB[]  = "CBITIB";
static const char TEXT_CBITID[]  = "CBITID";
static const char TEXT_CBITIW[]  = "CBITIW";
static const char TEXT_CBITW[]   = "CBITW";
static const char TEXT_CHECKB[]  = "CHECKB";
static const char TEXT_CHECKD[]  = "CHECKD";
static const char TEXT_CHECKW[]  = "CHECKW";
static const char TEXT_CINV[]    = "CINV";
static const char TEXT_CMPB[]    = "CMPB";
static const char TEXT_CMPD[]    = "CMPD";
static const char TEXT_CMPF[]    = "CMPF";
static const char TEXT_CMPL[]    = "CMPL";
static const char TEXT_CMPMB[]   = "CMPMB";
static const char TEXT_CMPMD[]   = "CMPMD";
static const char TEXT_CMPMW[]   = "CMPMW";
static const char TEXT_CMPQB[]   = "CMPQB";
static const char TEXT_CMPQD[]   = "CMPQD";
static const char TEXT_CMPQW[]   = "CMPQW";
static const char TEXT_CMPSB[]   = "CMPSB";
static const char TEXT_CMPSD[]   = "CMPSD";
static const char TEXT_CMPST[]   = "CMPST";
static const char TEXT_CMPSW[]   = "CMPSW";
static const char TEXT_CMPW[]    = "CMPW";
static const char TEXT_COMB[]    = "COMB";
static const char TEXT_COMD[]    = "COMD";
static const char TEXT_COMW[]    = "COMW";
static const char TEXT_CVTP[]    = "CVTP";
static const char TEXT_CXP[]     = "CXP";
static const char TEXT_CXPD[]    = "CXPD";
static const char TEXT_DEIB[]    = "DEIB";
static const char TEXT_DEID[]    = "DEID";
static const char TEXT_DEIW[]    = "DEIW";
static const char TEXT_DIA[]     = "DIA";
static const char TEXT_DIVB[]    = "DIVB";
static const char TEXT_DIVD[]    = "DIVD";
static const char TEXT_DIVF[]    = "DIVF";
static const char TEXT_DIVL[]    = "DIVL";
static const char TEXT_DIVW[]    = "DIVW";
static const char TEXT_ENTER[]   = "ENTER";
static const char TEXT_EXIT[]    = "EXIT";
static const char TEXT_EXTB[]    = "EXTB";
static const char TEXT_EXTD[]    = "EXTD";
static const char TEXT_EXTSB[]   = "EXTSB";
static const char TEXT_EXTSD[]   = "EXTSD";
static const char TEXT_EXTSW[]   = "EXTSW";
static const char TEXT_EXTW[]    = "EXTW";
static const char TEXT_FFSB[]    = "FFSB";
static const char TEXT_FFSD[]    = "FFSD";
static const char TEXT_FFSW[]    = "FFSW";
static const char TEXT_FLAG[]    = "FLAG";
static const char TEXT_FLOORFB[] = "FLOORFB";
static const char TEXT_FLOORFD[] = "FLOORFD";
static const char TEXT_FLOORFW[] = "FLOORFW";
static const char TEXT_FLOORLB[] = "FLOORLB";
static const char TEXT_FLOORLD[] = "FLOORLD";
static const char TEXT_FLOORLW[] = "FLOORLW";
static const char TEXT_IBITB[]   = "IBITB";
static const char TEXT_IBITD[]   = "IBITD";
static const char TEXT_IBITW[]   = "IBITW";
static const char TEXT_INDEXB[]  = "INDEXB";
static const char TEXT_INDEXD[]  = "INDEXD";
static const char TEXT_INDEXW[]  = "INDEXW";
static const char TEXT_INSB[]    = "INSB";
static const char TEXT_INSD[]    = "INSD";
static const char TEXT_INSSB[]   = "INSSB";
static const char TEXT_INSSD[]   = "INSSD";
static const char TEXT_INSSW[]   = "INSSW";
static const char TEXT_INSW[]    = "INSW";
static const char TEXT_JSR[]     = "JSR";
static const char TEXT_JUMP[]    = "JUMP";
static const char TEXT_LMR[]     = "LMR";
static const char TEXT_LPRB[]    = "LPRB";
static const char TEXT_LPRD[]    = "LPRD";
static const char TEXT_LPRW[]    = "LPRW";
static const char TEXT_LFSR[]    = "LFSR";
static const char TEXT_LSHB[]    = "LSHB";
static const char TEXT_LSHD[]    = "LSHD";
static const char TEXT_LSHW[]    = "LSHW";
static const char TEXT_MEIB[]    = "MEIB";
static const char TEXT_MEID[]    = "MEID";
static const char TEXT_MEIW[]    = "MEIW";
static const char TEXT_MODB[]    = "MODB";
static const char TEXT_MODD[]    = "MODD";
static const char TEXT_MODW[]    = "MODW";
static const char TEXT_MOVB[]    = "MOVB";
static const char TEXT_MOVBF[]   = "MOVBF";
static const char TEXT_MOVBL[]   = "MOVBL";
static const char TEXT_MOVD[]    = "MOVD";
static const char TEXT_MOVDF[]   = "MOVDF";
static const char TEXT_MOVDL[]   = "MOVDL";
static const char TEXT_MOVF[]    = "MOVF";
static const char TEXT_MOVFL[]   = "MOVFL";
static const char TEXT_MOVL[]    = "MOVL";
static const char TEXT_MOVLF[]   = "MOVLF";
static const char TEXT_MOVMB[]   = "MOVMB";
static const char TEXT_MOVMD[]   = "MOVMD";
static const char TEXT_MOVMW[]   = "MOVMW";
static const char TEXT_MOVQB[]   = "MOVQB";
static const char TEXT_MOVQD[]   = "MOVQD";
static const char TEXT_MOVQW[]   = "MOVQW";
static const char TEXT_MOVSB[]   = "MOVSB";
static const char TEXT_MOVSD[]   = "MOVSD";
static const char TEXT_MOVST[]   = "MOVST";
static const char TEXT_MOVSUB[]  = "MOVSUB";
static const char TEXT_MOVSUD[]  = "MOVSUD";
static const char TEXT_MOVSUW[]  = "MOVSUW";
static const char TEXT_MOVSW[]   = "MOVSW";
static const char TEXT_MOVUSB[]  = "MOVUSB";
static const char TEXT_MOVUSD[]  = "MOVUSD";
static const char TEXT_MOVUSW[]  = "MOVUSW";
static const char TEXT_MOVW[]    = "MOVW";
static const char TEXT_MOVWF[]   = "MOVWF";
static const char TEXT_MOVWL[]   = "MOVWL";
static const char TEXT_MOVXBD[]  = "MOVXBD";
static const char TEXT_MOVXBW[]  = "MOVXBW";
static const char TEXT_MOVXWD[]  = "MOVXWD";
static const char TEXT_MOVZBD[]  = "MOVZBD";
static const char TEXT_MOVZBW[]  = "MOVZBW";
static const char TEXT_MOVZWD[]  = "MOVZWD";
static const char TEXT_MULB[]    = "MULB";
static const char TEXT_MULD[]    = "MULD";
static const char TEXT_MULF[]    = "MULF";
static const char TEXT_MULL[]    = "MULL";
static const char TEXT_MULW[]    = "MULW";
static const char TEXT_NEGB[]    = "NEGB";
static const char TEXT_NEGD[]    = "NEGD";
static const char TEXT_NEGF[]    = "NEGF";
static const char TEXT_NEGL[]    = "NEGL";
static const char TEXT_NEGW[]    = "NEGW";
static const char TEXT_NOP[]     = "NOP";
static const char TEXT_NOTB[]    = "NOTB";
static const char TEXT_NOTD[]    = "NOTD";
static const char TEXT_NOTW[]    = "NOTW";
static const char TEXT_ORB[]     = "ORB";
static const char TEXT_ORD[]     = "ORD";
static const char TEXT_ORW[]     = "ORW";
static const char TEXT_QUOB[]    = "QUOB";
static const char TEXT_QUOD[]    = "QUOD";
static const char TEXT_QUOW[]    = "QUOW";
static const char TEXT_RDVAL[]   = "RDVAL";
static const char TEXT_REMB[]    = "REMB";
static const char TEXT_REMD[]    = "REMD";
static const char TEXT_REMW[]    = "REMW";
static const char TEXT_RESTORE[] = "RESTORE";
static const char TEXT_RET[]     = "RET";
static const char TEXT_RETI[]    = "RETI";
static const char TEXT_RETT[]    = "RETT";
static const char TEXT_ROTB[]    = "ROTB";
static const char TEXT_ROTD[]    = "ROTD";
static const char TEXT_ROTW[]    = "ROTW";
static const char TEXT_ROUNDFB[] = "ROUNDFB";
static const char TEXT_ROUNDFD[] = "ROUNDFD";
static const char TEXT_ROUNDFW[] = "ROUNDFW";
static const char TEXT_ROUNDLB[] = "ROUNDLB";
static const char TEXT_ROUNDLD[] = "ROUNDLD";
static const char TEXT_ROUNDLW[] = "ROUNDLW";
static const char TEXT_RXP[]     = "RXP";
static const char TEXT_SAVE[]    = "SAVE";
static const char TEXT_SBITB[]   = "SBITB";
static const char TEXT_SBITD[]   = "SBITD";
static const char TEXT_SBITIB[]  = "SBITIB";
static const char TEXT_SBITID[]  = "SBITID";
static const char TEXT_SBITIW[]  = "SBITIW";
static const char TEXT_SBITW[]   = "SBITW";
static const char TEXT_SCCB[]    = "SCCB";
static const char TEXT_SCCD[]    = "SCCD";
static const char TEXT_SCCW[]    = "SCCW";
static const char TEXT_SCSB[]    = "SCSB";
static const char TEXT_SCSD[]    = "SCSD";
static const char TEXT_SCSW[]    = "SCSW";
static const char TEXT_SEQB[]    = "SEQB";
static const char TEXT_SEQD[]    = "SEQD";
static const char TEXT_SEQW[]    = "SEQW";
static const char TEXT_SETCFG[]  = "SETCFG";
static const char TEXT_SFCB[]    = "SFCB";
static const char TEXT_SFCD[]    = "SFCD";
static const char TEXT_SFCW[]    = "SFCW";
static const char TEXT_SFSB[]    = "SFSB";
static const char TEXT_SFSD[]    = "SFSD";
static const char TEXT_SFSR[]    = "SFSR";
static const char TEXT_SFSW[]    = "SFSW";
static const char TEXT_SGEB[]    = "SGEB";
static const char TEXT_SGED[]    = "SGED";
static const char TEXT_SGEW[]    = "SGEW";
static const char TEXT_SGTB[]    = "SGTB";
static const char TEXT_SGTD[]    = "SGTD";
static const char TEXT_SGTW[]    = "SGTW";
static const char TEXT_SHIB[]    = "SHIB";
static const char TEXT_SHID[]    = "SHID";
static const char TEXT_SHIW[]    = "SHIW";
static const char TEXT_SHSB[]    = "SHSB";
static const char TEXT_SHSD[]    = "SHSD";
static const char TEXT_SHSW[]    = "SHSW";
static const char TEXT_SKPSB[]   = "SKPSB";
static const char TEXT_SKPSD[]   = "SKPSD";
static const char TEXT_SKPST[]   = "SKPST";
static const char TEXT_SKPSW[]   = "SKPSW";
static const char TEXT_SLEB[]    = "SLEB";
static const char TEXT_SLED[]    = "SLED";
static const char TEXT_SLEW[]    = "SLEW";
static const char TEXT_SLOB[]    = "SLOB";
static const char TEXT_SLOD[]    = "SLOD";
static const char TEXT_SLOW[]    = "SLOW";
static const char TEXT_SLSB[]    = "SLSB";
static const char TEXT_SLSD[]    = "SLSD";
static const char TEXT_SLSW[]    = "SLSW";
static const char TEXT_SLTB[]    = "SLTB";
static const char TEXT_SLTD[]    = "SLTD";
static const char TEXT_SLTW[]    = "SLTW";
static const char TEXT_SMR[]     = "SMR";
static const char TEXT_SNEB[]    = "SNEB";
static const char TEXT_SNED[]    = "SNED";
static const char TEXT_SNEW[]    = "SNEW";
static const char TEXT_SPRB[]    = "SPRB";
static const char TEXT_SPRD[]    = "SPRD";
static const char TEXT_SPRW[]    = "SPRW";
static const char TEXT_SUBB[]    = "SUBB";
static const char TEXT_SUBCB[]   = "SUBCB";
static const char TEXT_SUBCD[]   = "SUBCD";
static const char TEXT_SUBCW[]   = "SUBCW";
static const char TEXT_SUBD[]    = "SUBD";
static const char TEXT_SUBF[]    = "SUBF";
static const char TEXT_SUBL[]    = "SUBL";
static const char TEXT_SUBPB[]   = "SUBPB";
static const char TEXT_SUBPD[]   = "SUBPD";
static const char TEXT_SUBPW[]   = "SUBPW";
static const char TEXT_SUBW[]    = "SUBW";
static const char TEXT_SVC[]     = "SVC";
static const char TEXT_TBITB[]   = "TBITB";
static const char TEXT_TBITD[]   = "TBITD";
static const char TEXT_TBITW[]   = "TBITW";
static const char TEXT_TRUNCFB[] = "TRUNCFB";
static const char TEXT_TRUNCFD[] = "TRUNCFD";
static const char TEXT_TRUNCFW[] = "TRUNCFW";
static const char TEXT_TRUNCLB[] = "TRUNCLB";
static const char TEXT_TRUNCLD[] = "TRUNCLD";
static const char TEXT_TRUNCLW[] = "TRUNCLW";
static const char TEXT_WAIT[]    = "WAIT";
static const char TEXT_WRVAL[]   = "WRVAL";
static const char TEXT_XORB[]    = "XORB";
static const char TEXT_XORD[]    = "XORD";
static const char TEXT_XORW[]    = "XORW";

} // namespace ns32000
} // namespace libasm

#endif // __TEXT_NS32000__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
