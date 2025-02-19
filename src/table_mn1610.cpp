/*
 * Copyright 2021 Tadashi G. Takaoka
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

#include "table_mn1610.h"
#include "entry_mn1610.h"
#include "entry_table.h"
#include "text_mn1610.h"

using namespace libasm::text::mn1610;

namespace libasm {
namespace mn1610 {

#define E4(_opc, _name, _opr1, _opr2, _opr3, _opr4) \
    {_opc, Entry::Flags::create(_opr1, _opr2, _opr3, _opr4), _name}
#define E3(_opc, _name, _opr1, _opr2, _opr3) E4(_opc, _name, _opr1, _opr2, _opr3, M_NONE)
#define E2(_opc, _name, _opr1, _opr2) E3(_opc, _name, _opr1, _opr2, M_NONE)
#define E1(_opc, _name, _opr1) E2(_opc, _name, _opr1, M_NONE)
#define E0(_opc, _name) E1(_opc, _name, M_NONE)

// clang-format off
constexpr Entry TABLE_COMMON[] PROGMEM = {
        E0(0x2000, TEXT_H),
        E0(0x2003, TEXT_RET),
        E1(0x2001, TEXT_PUSH, M_RD),
        E1(0x2002, TEXT_POP,  M_RD),
        E1(0x2004, TEXT_LPSW, M_ILVL),
};

constexpr uint8_t INDEX_COMMON[] PROGMEM = {
      0,  // TEXT_H
      4,  // TEXT_LPSW
      3,  // TEXT_POP
      2,  // TEXT_PUSH
      1,  // TEXT_RET
};

constexpr Entry TABLE_MN1610[] PROGMEM = {
        E2(0x0800, TEXT_MVI,  M_RD,   M_IM8),
        E2(0x1000, TEXT_WT,   M_RD,   M_IOA),
        E2(0x1800, TEXT_RD,   M_RD,   M_IOA),
        E3(0x2008, TEXT_SR,   M_RD,   M_EOP, M_SKIP),
        E2(0x2008, TEXT_SR,   M_RD,   M_SKIP),
        E3(0x200C, TEXT_SL,   M_RD,   M_EOP, M_SKIP),
        E2(0x200C, TEXT_SL,   M_RD,   M_SKIP),
        E3(0x2800, TEXT_TBIT, M_RD,   M_BIT, M_SKIP),
        E3(0x3000, TEXT_RBIT, M_RD,   M_BIT, M_SKIP),
        E3(0x3800, TEXT_SBIT, M_RD,   M_BIT, M_SKIP),
        E3(0x4000, TEXT_SI,   M_RD,   M_IM4, M_SKIP),
        E3(0x4800, TEXT_AI,   M_RD,   M_IM4, M_SKIP),
        E3(0x5000, TEXT_CB,   M_RD,   M_RS,  M_SKIP),
        E3(0x5008, TEXT_C,    M_RD,   M_RS,  M_SKIP),
        E3(0x5800, TEXT_S,    M_RD,   M_RS,  M_SKIP),
        E3(0x5808, TEXT_A,    M_RD,   M_RS,  M_SKIP),
        E3(0x6000, TEXT_EOR,  M_RD,   M_RS,  M_SKIP),
        E3(0x6008, TEXT_OR,   M_RD,   M_RS,  M_SKIP),
        E3(0x6800, TEXT_LAD,  M_RD,   M_RS,  M_SKIP),
        E3(0x6808, TEXT_AND,  M_RD,   M_RS,  M_SKIP),
        E3(0x7000, TEXT_DSWP, M_RD,   M_RS,  M_SKIP),
        E3(0x7008, TEXT_BSWP, M_RD,   M_RS,  M_SKIP),
        E3(0x7800, TEXT_MVB,  M_RD,   M_RS,  M_SKIP),
        E3(0x7808, TEXT_MV,   M_RD,   M_RS,  M_SKIP),
        E1(0x8600, TEXT_DMS,  M_GEN),
        E1(0x8700, TEXT_BAL,  M_GEN),
        E2(0x8000, TEXT_ST,   M_RDG,  M_GEN),
        E1(0xC600, TEXT_IMS,  M_GEN),
        E1(0xC700, TEXT_B,    M_GEN),
        E2(0xC000, TEXT_L,    M_RDG,  M_GEN),
        E2(0x6000, TEXT_CLR,  M_RDS,  M_SKIP),
        E2(0x6008, TEXT_TST,  M_RDS,  M_SKIP),
};

constexpr uint8_t INDEX_MN1610[] PROGMEM = {
     15,  // TEXT_A
     11,  // TEXT_AI
     19,  // TEXT_AND
     28,  // TEXT_B
     25,  // TEXT_BAL
     21,  // TEXT_BSWP
     13,  // TEXT_C
     12,  // TEXT_CB
     30,  // TEXT_CLR
     24,  // TEXT_DMS
     20,  // TEXT_DSWP
     16,  // TEXT_EOR
     27,  // TEXT_IMS
     29,  // TEXT_L
     18,  // TEXT_LAD
     23,  // TEXT_MV
     22,  // TEXT_MVB
      0,  // TEXT_MVI
     17,  // TEXT_OR
      8,  // TEXT_RBIT
      2,  // TEXT_RD
     14,  // TEXT_S
      9,  // TEXT_SBIT
     10,  // TEXT_SI
      5,  // TEXT_SL
      6,  // TEXT_SL
      3,  // TEXT_SR
      4,  // TEXT_SR
     26,  // TEXT_ST
      7,  // TEXT_TBIT
     31,  // TEXT_TST
      1,  // TEXT_WT
};

constexpr Entry TABLE_MN1613[] PROGMEM = {
        E2(0x0F07, TEXT_LB,   M_RBW,  M_ABS),
        E2(0x0F00, TEXT_SETB, M_RS,   M_RBW),
        E2(0x0F0F, TEXT_LS,   M_RP,   M_ABS),
        E2(0x0F08, TEXT_SETS, M_RS,   M_RP),
        E2(0x0F87, TEXT_STB,  M_RB,   M_ABS),
        E2(0x0F80, TEXT_CPYB, M_RS,   M_RB),
        E2(0x0F8F, TEXT_STS,  M_RP,   M_ABS),
        E2(0x0F88, TEXT_CPYS, M_RS,   M_RP),
        E0(0x1707, TEXT_POPM),
        E3(0x1700, TEXT_TRST, M_RS,   M_ABS,  M_SKIP),
        E0(0x170F, TEXT_PSHM),
        E3(0x1708, TEXT_TSET, M_RS,   M_ABS,  M_SKIP),
        E3(0x1F07, TEXT_FLT,  M_DR0,  M_R0,   M_SKIP),
        E3(0x1F0F, TEXT_FIX,  M_R0,   M_DR0,  M_SKIP),
        E3(0x1F00, TEXT_NEG,  M_RS,   M_COP,  M_SKIP),
        E2(0x1F00, TEXT_NEG,  M_RS,   M_SKIP),
        E1(0x2607, TEXT_BD,   M_ABS),
        E1(0x2617, TEXT_BALD, M_ABS),
        E1(0x270F, TEXT_BL,   M_IABS),
        E1(0x2704, TEXT_BR,   M_RI),
        E1(0x271F, TEXT_BALL, M_IABS),
        E1(0x2714, TEXT_BALR, M_RI),
        E3(0x2708, TEXT_LD,   M_RSG,  M_SB,   M_ABS),
        E2(0x2708, TEXT_LD,   M_RSG,  M_ABS),
        E3(0x2748, TEXT_STD,  M_RSG,  M_SB,   M_ABS),
        E2(0x2748, TEXT_STD,  M_RSG,  M_ABS),
        E2(0x2010, TEXT_WTR,  M_RDG,  M_RI),
        E2(0x2014, TEXT_RDR,  M_RDG,  M_RI),
        E3(0x2000, TEXT_LR,   M_RDG,  M_SB,   M_RIAU),
        E2(0x2000, TEXT_LR,   M_RDG,  M_RIAU),
        E3(0x2004, TEXT_STR,  M_RDG,  M_SB,   M_RIAU),
        E2(0x2004, TEXT_STR,  M_RDG,  M_RIAU),
        E0(0x3F07, TEXT_RETL),
        E3(0x3F17, TEXT_BLK,  M_RI2,  M_RI1,  M_R0),
        E2(0x3F70, TEXT_SRBT, M_R0,   M_RS),
        E2(0x3F00, TEXT_SETH, M_RS,   M_RHW),
        E2(0x3FF0, TEXT_DEBP, M_RS,   M_R0),
        E2(0x3F80, TEXT_CPYH, M_RS,   M_RHR),
        E4(0x4704, TEXT_SD,   M_DR0,  M_RI,   M_COP,  M_SKIP),
        E3(0x4704, TEXT_SD,   M_DR0,  M_RI,   M_SKIP),
        E4(0x4F04, TEXT_AD,   M_DR0,  M_RI,   M_COP,  M_SKIP),
        E3(0x4F04, TEXT_AD,   M_DR0,  M_RI,   M_SKIP),
        E4(0x5704, TEXT_DAS,  M_R0,   M_RI,   M_COP,  M_SKIP),
        E3(0x5704, TEXT_DAS,  M_R0,   M_RI,   M_SKIP),
        E3(0x5700, TEXT_CBR,  M_R0,   M_RI,   M_SKIP),
        E3(0x5708, TEXT_CWR,  M_R0,   M_RI,   M_SKIP),
        E3(0x5007, TEXT_CBI,  M_RD,   M_IM8W, M_SKIP),
        E3(0x500F, TEXT_CWI,  M_RD,   M_IM16, M_SKIP),
        E4(0x5F04, TEXT_DAA,  M_R0,   M_RI,   M_COP,  M_SKIP),
        E3(0x5F04, TEXT_DAA,  M_R0,   M_RI,   M_SKIP),
        E3(0x5F00, TEXT_SWR,  M_R0,   M_RI,   M_SKIP),
        E3(0x5F08, TEXT_AWR,  M_R0,   M_RI,   M_SKIP),
        E3(0x5807, TEXT_SWI,  M_RD,   M_IM16, M_SKIP),
        E3(0x580F, TEXT_AWI,  M_RD,   M_IM16, M_SKIP),
        E3(0x6704, TEXT_FD,   M_DR0,  M_RI,   M_SKIP),
        E3(0x670C, TEXT_FM,   M_DR0,  M_RI,   M_SKIP),
        E3(0x6700, TEXT_EORR, M_R0,   M_RI,   M_SKIP),
        E3(0x6708, TEXT_ORR,  M_R0,   M_RI,   M_SKIP),
        E3(0x6007, TEXT_EORI, M_RD,   M_IM16, M_SKIP),
        E3(0x600F, TEXT_ORI,  M_RD,   M_IM16, M_SKIP),
        E3(0x6F08, TEXT_ANDR, M_R0,   M_RI,   M_SKIP),
        E3(0x6F00, TEXT_LADR, M_R0,   M_RI,   M_SKIP),
        E3(0x6F04, TEXT_FS,   M_DR0,  M_RI,   M_SKIP),
        E3(0x6F0C, TEXT_FA,   M_DR0,  M_RI,   M_SKIP),
        E3(0x6807, TEXT_LADI, M_RD,   M_IM16, M_SKIP),
        E3(0x680F, TEXT_ANDI, M_RD,   M_IM16, M_SKIP),
        E3(0x7700, TEXT_DSWR, M_R0,   M_RI,   M_SKIP),
        E3(0x7708, TEXT_BSWR, M_R0,   M_RI,   M_SKIP),
        E3(0x770C, TEXT_D,    M_DR0,  M_RI,   M_SKIP),
        E3(0x7F00, TEXT_MVBR, M_R0,   M_RI,   M_SKIP),
        E3(0x7F08, TEXT_MVWR, M_R0,   M_RI,   M_SKIP),
        E3(0x7F0C, TEXT_M,    M_DR0,  M_RI,   M_SKIP),
        E3(0x780F, TEXT_MVWI, M_RD,   M_IM16, M_SKIP),
};

constexpr uint8_t INDEX_MN1613[] PROGMEM = {
     40,  // TEXT_AD
     41,  // TEXT_AD
     65,  // TEXT_ANDI
     60,  // TEXT_ANDR
     53,  // TEXT_AWI
     51,  // TEXT_AWR
     17,  // TEXT_BALD
     20,  // TEXT_BALL
     21,  // TEXT_BALR
     16,  // TEXT_BD
     18,  // TEXT_BL
     33,  // TEXT_BLK
     19,  // TEXT_BR
     67,  // TEXT_BSWR
     46,  // TEXT_CBI
     44,  // TEXT_CBR
      5,  // TEXT_CPYB
     37,  // TEXT_CPYH
      7,  // TEXT_CPYS
     47,  // TEXT_CWI
     45,  // TEXT_CWR
     68,  // TEXT_D
     48,  // TEXT_DAA
     49,  // TEXT_DAA
     42,  // TEXT_DAS
     43,  // TEXT_DAS
     36,  // TEXT_DEBP
     66,  // TEXT_DSWR
     58,  // TEXT_EORI
     56,  // TEXT_EORR
     63,  // TEXT_FA
     54,  // TEXT_FD
     13,  // TEXT_FIX
     12,  // TEXT_FLT
     55,  // TEXT_FM
     62,  // TEXT_FS
     64,  // TEXT_LADI
     61,  // TEXT_LADR
      0,  // TEXT_LB
     22,  // TEXT_LD
     23,  // TEXT_LD
     28,  // TEXT_LR
     29,  // TEXT_LR
      2,  // TEXT_LS
     71,  // TEXT_M
     69,  // TEXT_MVBR
     72,  // TEXT_MVWI
     70,  // TEXT_MVWR
     14,  // TEXT_NEG
     15,  // TEXT_NEG
     59,  // TEXT_ORI
     57,  // TEXT_ORR
      8,  // TEXT_POPM
     10,  // TEXT_PSHM
     27,  // TEXT_RDR
     32,  // TEXT_RETL
     38,  // TEXT_SD
     39,  // TEXT_SD
      1,  // TEXT_SETB
     35,  // TEXT_SETH
      3,  // TEXT_SETS
     34,  // TEXT_SRBT
      4,  // TEXT_STB
     24,  // TEXT_STD
     25,  // TEXT_STD
     30,  // TEXT_STR
     31,  // TEXT_STR
      6,  // TEXT_STS
     52,  // TEXT_SWI
     50,  // TEXT_SWR
      9,  // TEXT_TRST
     11,  // TEXT_TSET
     26,  // TEXT_WTR
};
// clang-format on

using EntryPage = entry::TableBase<Entry>;

constexpr EntryPage MN1610_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_COMMON), ARRAY_RANGE(INDEX_COMMON)},
        {ARRAY_RANGE(TABLE_MN1610), ARRAY_RANGE(INDEX_MN1610)},
};

constexpr EntryPage MN1613_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_COMMON), ARRAY_RANGE(INDEX_COMMON)},
        {ARRAY_RANGE(TABLE_MN1613), ARRAY_RANGE(INDEX_MN1613)},
        {ARRAY_RANGE(TABLE_MN1610), ARRAY_RANGE(INDEX_MN1610)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {MN1610, TEXT_CPU_MN1610, ARRAY_RANGE(MN1610_PAGES)},
        {MN1613, TEXT_CPU_MN1613, ARRAY_RANGE(MN1613_PAGES)},
        {MN1613A, TEXT_CPU_MN1613A, ARRAY_RANGE(MN1613_PAGES)},
};

const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

bool hasOperand(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, Cpu::acceptAll<AsmInsn, Entry>);
    return insn.isOK() && insn.mode1() != M_NONE;
}

bool acceptMode(AddrMode opr, AddrMode table) {
    if (table == opr)
        return true;
    switch (table) {
    case M_SKIP:
        return opr == M_NONE;
    case M_RD:
        return opr == M_R0 || opr == M_RDG;
    case M_RDG:
        return opr == M_R0;
    case M_RDS:
        return opr == M_R0 || opr == M_RD || opr == M_RDG;
    case M_RS:
        return opr == M_R0 || opr == M_RDG || opr == M_RD;
    case M_RSG:
        return opr == M_R0 || opr == M_RDG;
    case M_RI:
        return opr == M_RI1 || opr == M_RI2;
    case M_RIAU:
        return opr == M_RI1 || opr == M_RI2 || opr == M_RI;
    case M_COP:
    case M_ILVL:
    case M_BIT:
    case M_IM4:
    case M_IM8:
    case M_IM8W:
    case M_IM16:
    case M_IOA:
    case M_ABS:
        return opr == M_IM8 || opr == M_IM16 || opr == M_ABS;
    case M_RB:
    case M_RBW:
        return opr == M_SB || opr == M_RB;
    case M_RHW:
        return opr == M_RHR;
    case M_GEN:
        return opr == M_IM8 || opr == M_IM16 || opr == M_ABS || opr == M_IABS || opr == M_INDX ||
               opr == M_IXID || opr == M_IDIX;
    default:
        return false;
    }
}

bool acceptModes(AsmInsn &insn, const Entry *entry) {
    const auto table = entry->readFlags();
    return acceptMode(insn.op1.mode, table.mode1()) && acceptMode(insn.op2.mode, table.mode2()) &&
           acceptMode(insn.op3.mode, table.mode3()) && acceptMode(insn.op4.mode, table.mode4());
}

Error searchName(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, acceptModes);
    return insn.getError();
}

bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *) {
    auto opc = insn.opCode();
    const auto flags = entry->readFlags();
    const auto mode1 = flags.mode1();
    const auto mode2 = flags.mode2();
    const auto mode3 = flags.mode3();
    const auto mode4 = flags.mode4();
    if (mode1 == M_GEN || mode2 == M_GEN)
        opc &= ~((7 << 11) | 0xFF);
    const auto dstReg = (opc >> 8) & 7;
    if (mode1 == M_RD || mode1 == M_RDS) {
        if (dstReg == 7)
            return false;
        opc &= ~(7 << 8);
    }
    if (mode1 == M_RDG) {
        if (dstReg >= 6)
            return false;
        opc &= ~(7 << 8);
    }
    const auto srcReg = opc & 7;
    if (mode1 == M_RS || mode2 == M_RS || mode1 == M_RDS) {
        if (srcReg == 7)
            return false;
        opc &= ~7;
    }
    if (mode1 == M_RSG) {
        if (srcReg >= 6)
            return false;  // no STR
        opc &= ~7;
    }
    if (mode1 == M_RI || mode2 == M_RI)
        opc &= ~3;
    if (mode3 == M_RIAU) {
        const auto idirMode = (opc >> 6) & 3;
        if (idirMode == 0)
            return false;
        opc &= ~((3 << 6) | 3);
    }
    if (mode2 == M_SB)
        opc &= ~(3 << 4);
    if (mode1 == M_RBW || mode2 == M_RBW) {
        const auto segBase = (opc >> 4) & 7;
        if (segBase == 0)
            return false;  // no CSBR
        opc &= ~(7 << 4);
    }
    if (mode1 == M_RB || mode2 == M_RB)
        opc &= ~(7 << 4);
    if (mode1 == M_RP || mode2 == M_RP) {
        const auto cntlReg = (opc >> 4) & 7;
        if (cntlReg >= 3)
            return false;
        opc &= ~(7 << 4);
    }
    if (mode2 == M_RHR || mode2 == M_RHW) {
        const auto hardReg = (opc >> 4) & 7;
        if (hardReg == 7)
            return false;
        opc &= ~(7 << 4);
    }
    if (mode2 == M_SKIP || mode3 == M_SKIP || mode4 == M_SKIP)
        opc &= ~(0xF << 4);
    if (mode2 == M_IM8 || mode2 == M_IOA)
        opc &= ~0xFF;
    if (mode2 == M_IM4 || mode2 == M_BIT)
        opc &= ~0xF;
    if (mode1 == M_ILVL || mode2 == M_EOP)
        opc &= ~3;
    if (mode2 == M_COP || mode3 == M_COP)
        opc &= ~(1 << 3);
    return opc == entry->readOpCode();
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

const /*PROGMEM*/ char *TableMn1610::listCpu_P() const {
    return TEXT_MN1610_LIST;
}

const /*PROGMEM*/ char *TableMn1610::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableMn1610::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    auto t = Cpu::search(name, ARRAY_RANGE(CPU_TABLE));
    if (t) {
        cpuType = t->readCpuType();
    } else if (name.iequals_P(TEXT_CPU_MN1610 + 2)) {
        cpuType = MN1610;
    } else if (name.iequals_P(TEXT_CPU_MN1613 + 2)) {
        cpuType = MN1613;
    } else if (name.iequals_P(TEXT_CPU_MN1613A + 2)) {
        cpuType = MN1613A;
    } else {
        return UNSUPPORTED_CPU;
    }
    return OK;
}

const TableMn1610 TABLE;

}  // namespace mn1610
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
