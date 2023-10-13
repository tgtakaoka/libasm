/*
 * Copyright 2022 Tadashi G. Takaoka
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

#include "table_scn2650.h"

#include "entry_scn2650.h"
#include "entry_table.h"
#include "text_scn2650.h"

using namespace libasm::text::scn2650;

namespace libasm {
namespace scn2650 {

#define E2(_opc, _name, _mode1, _mode2) \
    { _opc, Entry::Flags::create(_mode1, _mode2), _name }
#define E1(_opc, _name, _mode1) E2(_opc, _name, _mode1, M_NONE)
#define E0(_opc, _name) E1(_opc, _name, M_NONE)

// clang-format off
static constexpr Entry TABLE_2650[] PROGMEM = {
    E1(0x00, TEXT_LODZ, M_R123),
    E2(0x04, TEXT_LODI, M_REGN, M_IMM8),
    E2(0x08, TEXT_LODR, M_REGN, M_REL7),
    E2(0x0C, TEXT_LODA, M_REG0, M_IX13),
    E2(0x0C, TEXT_LODA, M_REGN, M_AB13),
    E0(0x12, TEXT_SPSU),
    E0(0x13, TEXT_SPSL),
    E1(0x14, TEXT_RETC, M_CCVN),
    E2(0x18, TEXT_BCTR, M_CCVN, M_REL7),
    E2(0x1C, TEXT_BCTA, M_CCVN, M_AB15),
    E1(0x20, TEXT_EORZ, M_REGN),
    E2(0x24, TEXT_EORI, M_REGN, M_IMM8),
    E2(0x28, TEXT_EORR, M_REGN, M_REL7),
    E2(0x2C, TEXT_EORA, M_REG0, M_IX13),
    E2(0x2C, TEXT_EORA, M_REGN, M_AB13),
    E1(0x30, TEXT_REDC, M_REGN),
    E1(0x34, TEXT_RETE, M_CCVN),
    E2(0x38, TEXT_BSTR, M_CCVN, M_REL7),
    E2(0x3C, TEXT_BSTA, M_CCVN, M_AB15),
    E0(0x40, TEXT_HALT),
    E1(0x40, TEXT_ANDZ, M_R123),
    E2(0x44, TEXT_ANDI, M_REGN, M_IMM8),
    E2(0x48, TEXT_ANDR, M_REGN, M_REL7),
    E2(0x4C, TEXT_ANDA, M_REG0, M_IX13),
    E2(0x4C, TEXT_ANDA, M_REGN, M_AB13),
    E1(0x50, TEXT_RRR,  M_REGN),
    E2(0x54, TEXT_REDE, M_REGN, M_IMM8),
    E2(0x58, TEXT_BRNR, M_REGN, M_REL7),
    E2(0x5C, TEXT_BRNA, M_REGN, M_AB15),
    E1(0x60, TEXT_LODZ, M_REG0),
    E1(0x60, TEXT_IORZ, M_REGN),
    E2(0x64, TEXT_IORI, M_REGN, M_IMM8),
    E2(0x68, TEXT_IORR, M_REGN, M_REL7),
    E2(0x6C, TEXT_IORA, M_REG0, M_IX13),
    E2(0x6C, TEXT_IORA, M_REGN, M_AB13),
    E1(0x70, TEXT_REDD, M_REGN),
    E1(0x74, TEXT_CPSU, M_IMM8),
    E1(0x75, TEXT_CPSL, M_IMM8),
    E1(0x76, TEXT_PPSU, M_IMM8),
    E1(0x77, TEXT_PPSL, M_IMM8),
    E2(0x78, TEXT_BSNR, M_REGN, M_REL7),
    E2(0x7C, TEXT_BSNA, M_REGN, M_AB15),
    E1(0x80, TEXT_ADDZ, M_REGN),
    E2(0x84, TEXT_ADDI, M_REGN, M_IMM8),
    E2(0x88, TEXT_ADDR, M_REGN, M_REL7),
    E2(0x8C, TEXT_ADDA, M_REG0, M_IX13),
    E2(0x8C, TEXT_ADDA, M_REGN, M_AB13),
    E0(0x92, TEXT_LPSU),
    E0(0x93, TEXT_LPSL),
    E1(0x94, TEXT_DAR,  M_REGN),
    E1(0x9B, TEXT_ZBRR, M_ABS7),
    E2(0x98, TEXT_BCFR, M_C012, M_REL7),
    E1(0x9F, TEXT_BXA,  M_IX15),
    E2(0x9C, TEXT_BCFA, M_C012, M_AB15),
    E1(0xA0, TEXT_SUBZ, M_REGN),
    E2(0xA4, TEXT_SUBI, M_REGN, M_IMM8),
    E2(0xA8, TEXT_SUBR, M_REGN, M_REL7),
    E2(0xAC, TEXT_SUBA, M_REG0, M_IX13),
    E2(0xAC, TEXT_SUBA, M_REGN, M_AB13),
    E1(0xB0, TEXT_WRTC, M_REGN),
    E1(0xB4, TEXT_TPSU, M_IMM8),
    E1(0xB5, TEXT_TPSL, M_IMM8),
    E1(0xBB, TEXT_ZBSR, M_ABS7),
    E2(0xB8, TEXT_BSFR, M_C012, M_REL7),
    E1(0xBF, TEXT_BSXA, M_IX15),
    E2(0xBC, TEXT_BSFA, M_C012, M_AB15),
    E0(0xC0, TEXT_NOP),
    E1(0xC0, TEXT_STRZ, M_R123),
    E2(0xC8, TEXT_STRR, M_REGN, M_REL7),
    E2(0xCC, TEXT_STRA, M_REG0, M_IX13),
    E2(0xCC, TEXT_STRA, M_REGN, M_AB13),
    E1(0xD0, TEXT_RRL,  M_REGN),
    E2(0xD4, TEXT_WRTE, M_REGN, M_IMM8),
    E2(0xD8, TEXT_BIRR, M_REGN, M_REL7),
    E2(0xDC, TEXT_BIRA, M_REGN, M_AB15),
    E1(0xE0, TEXT_COMZ, M_REGN),
    E2(0xE4, TEXT_COMI, M_REGN, M_IMM8),
    E2(0xE8, TEXT_COMR, M_REGN, M_REL7),
    E2(0xEC, TEXT_COMA, M_REG0, M_IX13),
    E2(0xEC, TEXT_COMA, M_REGN, M_AB13),
    E1(0xF0, TEXT_WRTD, M_REGN),
    E2(0xF4, TEXT_TMI,  M_REGN, M_IMM8),
    E2(0xF8, TEXT_BDRR, M_REGN, M_REL7),
    E2(0xFC, TEXT_BDRA, M_REGN, M_AB15),
};

static constexpr uint8_t INDEX_2650[] PROGMEM = {
     45,  // TEXT_ADDA
     46,  // TEXT_ADDA
     43,  // TEXT_ADDI
     44,  // TEXT_ADDR
     42,  // TEXT_ADDZ
     23,  // TEXT_ANDA
     24,  // TEXT_ANDA
     21,  // TEXT_ANDI
     22,  // TEXT_ANDR
     20,  // TEXT_ANDZ
     53,  // TEXT_BCFA
     51,  // TEXT_BCFR
      9,  // TEXT_BCTA
      8,  // TEXT_BCTR
     83,  // TEXT_BDRA
     82,  // TEXT_BDRR
     74,  // TEXT_BIRA
     73,  // TEXT_BIRR
     28,  // TEXT_BRNA
     27,  // TEXT_BRNR
     65,  // TEXT_BSFA
     63,  // TEXT_BSFR
     41,  // TEXT_BSNA
     40,  // TEXT_BSNR
     18,  // TEXT_BSTA
     17,  // TEXT_BSTR
     64,  // TEXT_BSXA
     52,  // TEXT_BXA
     78,  // TEXT_COMA
     79,  // TEXT_COMA
     76,  // TEXT_COMI
     77,  // TEXT_COMR
     75,  // TEXT_COMZ
     37,  // TEXT_CPSL
     36,  // TEXT_CPSU
     49,  // TEXT_DAR
     13,  // TEXT_EORA
     14,  // TEXT_EORA
     11,  // TEXT_EORI
     12,  // TEXT_EORR
     10,  // TEXT_EORZ
     19,  // TEXT_HALT
     33,  // TEXT_IORA
     34,  // TEXT_IORA
     31,  // TEXT_IORI
     32,  // TEXT_IORR
     30,  // TEXT_IORZ
      3,  // TEXT_LODA
      4,  // TEXT_LODA
      1,  // TEXT_LODI
      2,  // TEXT_LODR
      0,  // TEXT_LODZ
     29,  // TEXT_LODZ
     48,  // TEXT_LPSL
     47,  // TEXT_LPSU
     66,  // TEXT_NOP
     39,  // TEXT_PPSL
     38,  // TEXT_PPSU
     15,  // TEXT_REDC
     35,  // TEXT_REDD
     26,  // TEXT_REDE
      7,  // TEXT_RETC
     16,  // TEXT_RETE
     71,  // TEXT_RRL
     25,  // TEXT_RRR
      6,  // TEXT_SPSL
      5,  // TEXT_SPSU
     69,  // TEXT_STRA
     70,  // TEXT_STRA
     68,  // TEXT_STRR
     67,  // TEXT_STRZ
     57,  // TEXT_SUBA
     58,  // TEXT_SUBA
     55,  // TEXT_SUBI
     56,  // TEXT_SUBR
     54,  // TEXT_SUBZ
     81,  // TEXT_TMI
     61,  // TEXT_TPSL
     60,  // TEXT_TPSU
     59,  // TEXT_WRTC
     80,  // TEXT_WRTD
     72,  // TEXT_WRTE
     50,  // TEXT_ZBRR
     62,  // TEXT_ZBSR
};

// clang-format on

using EntryPage = entry::TableBase<Entry>;

static constexpr EntryPage SCN2650_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_2650), ARRAY_RANGE(INDEX_2650)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

static constexpr Cpu CPU_TABLE[] PROGMEM = {
        {SCN2650, TEXT_CPU_2650, ARRAY_RANGE(SCN2650_PAGES)},
};

static const Cpu *cpu(CpuType cpuType) {
    UNUSED(cpuType);
    return &CPU_TABLE[0];
}

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_REG0 || opr == M_R123)
        return table == M_REGN;
    if (opr == M_C012)
        return table == M_CCVN;
    if (opr == M_IMM8)
        return table == M_ABS7 || table == M_AB13 || table == M_AB15 || table == M_REL7 ||
               table == M_IX15;
    if (opr == M_AB15)
        return table == M_ABS7 || table == M_AB13 || table == M_REL7 || table == M_IX15;
    if (opr == M_IX15)
        return table == M_AB15 || table == M_IX13 || table == M_AB13;
    return false;
}

static bool acceptModes(AsmInsn &insn, const Entry *entry) {
    auto flags = insn.flags();
    auto table = entry->flags();
    return acceptMode(flags.mode1(), table.mode1()) && acceptMode(flags.mode2(), table.mode2());
}

Error TableScn2650::searchName(CpuType cpuType, AsmInsn &insn) const {
    cpu(cpuType)->searchName(insn, acceptModes);
    return insn.getError();
}

static bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *page) {
    UNUSED(page);
    auto opc = insn.opCode();
    const auto flags = entry->flags();
    const auto mode1 = flags.mode1();
    const auto mode2 = flags.mode2();
    if (mode1 == M_R123 && (opc & 3) == 0)
        return false;
    if (mode1 == M_C012 && (opc & 3) == 3)
        return false;
    if (mode1 == M_REGN || mode1 == M_R123 || mode1 == M_CCVN || mode1 == M_C012) {
        opc &= ~3;
    } else if (mode2 == M_IX13) {
        opc &= ~3;
    }
    return opc == entry->opCode();
}

Error TableScn2650::searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) const {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

const /*PROGMEM*/ char *TableScn2650::listCpu_P() const {
    return TEXT_CPU_SCN2650;
}

const /*PROGMEM*/ char *TableScn2650::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableScn2650::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    if (name.iequals(TEXT_CPU_2650) || name.iequals(TEXT_CPU_SCN2650)) {
        cpuType = SCN2650;
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TableScn2650 TABLE;

}  // namespace scn2650
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
