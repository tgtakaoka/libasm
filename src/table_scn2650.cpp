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

#include <ctype.h>
#include <string.h>

#include "config_scn2650.h"
#include "entry_scn2650.h"
#include "text_scn2650.h"

namespace libasm {
namespace scn2650 {

#define E2(_opc, _name, _mode1, _mode2) \
    { _opc, Entry::Flags::create(_mode1, _mode2), _name }
#define E1(_opc, _name, _mode1) E2(_opc, _name, _mode1, NONE)
#define E0(_opc, _name) E1(_opc, _name, NONE)

// clang-format off
static constexpr Entry TABLE_2650[] PROGMEM = {
    E1(0x00, TEXT_null, UNDF),
    E1(0x00, TEXT_LODZ, R123),
    E2(0x04, TEXT_LODI, REGN, IMM8),
    E2(0x08, TEXT_LODR, REGN, REL7),
    E2(0x0C, TEXT_LODA, REG0, IX13),
    E2(0x0C, TEXT_LODA, REGN, AB13),
    E0(0x12, TEXT_SPSU),
    E0(0x13, TEXT_SPSL),
    E1(0x14, TEXT_RETC, CCVN),
    E2(0x18, TEXT_BCTR, CCVN, REL7),
    E2(0x1C, TEXT_BCTA, CCVN, AB15),
    E1(0x20, TEXT_EORZ, REGN),
    E2(0x24, TEXT_EORI, REGN, IMM8),
    E2(0x28, TEXT_EORR, REGN, REL7),
    E2(0x2C, TEXT_EORA, REG0, IX13),
    E2(0x2C, TEXT_EORA, REGN, AB13),
    E1(0x30, TEXT_REDC, REGN),
    E1(0x34, TEXT_RETE, CCVN),
    E2(0x38, TEXT_BSTR, CCVN, REL7),
    E2(0x3C, TEXT_BSTA, CCVN, AB15),
    E0(0x40, TEXT_HALT),
    E1(0x40, TEXT_ANDZ, R123),
    E2(0x44, TEXT_ANDI, REGN, IMM8),
    E2(0x48, TEXT_ANDR, REGN, REL7),
    E2(0x4C, TEXT_ANDA, REG0, IX13),
    E2(0x4C, TEXT_ANDA, REGN, AB13),
    E1(0x50, TEXT_RRR,  REGN),
    E2(0x54, TEXT_REDE, REGN, IMM8),
    E2(0x58, TEXT_BRNR, REGN, REL7),
    E2(0x5C, TEXT_BRNA, REGN, AB15),
    E1(0x60, TEXT_LODZ, REG0),
    E1(0x60, TEXT_IORZ, REGN),
    E2(0x64, TEXT_IORI, REGN, IMM8),
    E2(0x68, TEXT_IORR, REGN, REL7),
    E2(0x6C, TEXT_IORA, REG0, IX13),
    E2(0x6C, TEXT_IORA, REGN, AB13),
    E1(0x70, TEXT_REDD, REGN),
    E1(0x74, TEXT_CPSU, IMM8),
    E1(0x75, TEXT_CPSL, IMM8),
    E1(0x76, TEXT_PPSU, IMM8),
    E1(0x77, TEXT_PPSL, IMM8),
    E2(0x78, TEXT_BSNR, REGN, REL7),
    E2(0x7C, TEXT_BSNA, REGN, AB15),
    E1(0x80, TEXT_ADDZ, REGN),
    E2(0x84, TEXT_ADDI, REGN, IMM8),
    E2(0x88, TEXT_ADDR, REGN, REL7),
    E2(0x8C, TEXT_ADDA, REG0, IX13),
    E2(0x8C, TEXT_ADDA, REGN, AB13),
    E0(0x92, TEXT_LPSU),
    E0(0x93, TEXT_LPSL),
    E1(0x94, TEXT_DAR,  REGN),
    E1(0x9B, TEXT_ZBRR, ABS7),
    E2(0x98, TEXT_BCFR, C012, REL7),
    E1(0x9F, TEXT_BXA,  IX15),
    E2(0x9C, TEXT_BCFA, C012, AB15),
    E1(0xA0, TEXT_SUBZ, REGN),
    E2(0xA4, TEXT_SUBI, REGN, IMM8),
    E2(0xA8, TEXT_SUBR, REGN, REL7),
    E2(0xAC, TEXT_SUBA, REG0, IX13),
    E2(0xAC, TEXT_SUBA, REGN, AB13),
    E1(0xB0, TEXT_WRTC, REGN),
    E1(0xB4, TEXT_TPSU, IMM8),
    E1(0xB5, TEXT_TPSL, IMM8),
    E1(0xBB, TEXT_ZBSR, ABS7),
    E2(0xB8, TEXT_BSFR, C012, REL7),
    E1(0xBF, TEXT_BSXA, IX15),
    E2(0xBC, TEXT_BSFA, C012, AB15),
    E0(0xC0, TEXT_NOP),
    E1(0xC0, TEXT_STRZ, R123),
    E2(0xC8, TEXT_STRR, REGN, REL7),
    E2(0xCC, TEXT_STRA, REG0, IX13),
    E2(0xCC, TEXT_STRA, REGN, AB13),
    E1(0xD0, TEXT_RRL,  REGN),
    E2(0xD4, TEXT_WRTE, REGN, IMM8),
    E2(0xD8, TEXT_BIRR, REGN, REL7),
    E2(0xDC, TEXT_BIRA, REGN, AB15),
    E1(0xE0, TEXT_COMZ, REGN),
    E2(0xE4, TEXT_COMI, REGN, IMM8),
    E2(0xE8, TEXT_COMR, REGN, REL7),
    E2(0xEC, TEXT_COMA, REG0, IX13),
    E2(0xEC, TEXT_COMA, REGN, AB13),
    E1(0xF0, TEXT_WRTD, REGN),
    E2(0xF4, TEXT_TMI,  REGN, IMM8),
    E2(0xF8, TEXT_BDRR, REGN, REL7),
    E2(0xFC, TEXT_BDRA, REGN, AB15),
};

static constexpr uint8_t INDEX_2650[] PROGMEM = {
      0,  // TEXT_null
     46,  // TEXT_ADDA
     47,  // TEXT_ADDA
     44,  // TEXT_ADDI
     45,  // TEXT_ADDR
     43,  // TEXT_ADDZ
     24,  // TEXT_ANDA
     25,  // TEXT_ANDA
     22,  // TEXT_ANDI
     23,  // TEXT_ANDR
     21,  // TEXT_ANDZ
     54,  // TEXT_BCFA
     52,  // TEXT_BCFR
     10,  // TEXT_BCTA
      9,  // TEXT_BCTR
     84,  // TEXT_BDRA
     83,  // TEXT_BDRR
     75,  // TEXT_BIRA
     74,  // TEXT_BIRR
     29,  // TEXT_BRNA
     28,  // TEXT_BRNR
     66,  // TEXT_BSFA
     64,  // TEXT_BSFR
     42,  // TEXT_BSNA
     41,  // TEXT_BSNR
     19,  // TEXT_BSTA
     18,  // TEXT_BSTR
     65,  // TEXT_BSXA
     53,  // TEXT_BXA
     79,  // TEXT_COMA
     80,  // TEXT_COMA
     77,  // TEXT_COMI
     78,  // TEXT_COMR
     76,  // TEXT_COMZ
     38,  // TEXT_CPSL
     37,  // TEXT_CPSU
     50,  // TEXT_DAR
     14,  // TEXT_EORA
     15,  // TEXT_EORA
     12,  // TEXT_EORI
     13,  // TEXT_EORR
     11,  // TEXT_EORZ
     20,  // TEXT_HALT
     34,  // TEXT_IORA
     35,  // TEXT_IORA
     32,  // TEXT_IORI
     33,  // TEXT_IORR
     31,  // TEXT_IORZ
      4,  // TEXT_LODA
      5,  // TEXT_LODA
      2,  // TEXT_LODI
      3,  // TEXT_LODR
      1,  // TEXT_LODZ
     30,  // TEXT_LODZ
     49,  // TEXT_LPSL
     48,  // TEXT_LPSU
     67,  // TEXT_NOP
     40,  // TEXT_PPSL
     39,  // TEXT_PPSU
     16,  // TEXT_REDC
     36,  // TEXT_REDD
     27,  // TEXT_REDE
      8,  // TEXT_RETC
     17,  // TEXT_RETE
     72,  // TEXT_RRL
     26,  // TEXT_RRR
      7,  // TEXT_SPSL
      6,  // TEXT_SPSU
     70,  // TEXT_STRA
     71,  // TEXT_STRA
     69,  // TEXT_STRR
     68,  // TEXT_STRZ
     58,  // TEXT_SUBA
     59,  // TEXT_SUBA
     56,  // TEXT_SUBI
     57,  // TEXT_SUBR
     55,  // TEXT_SUBZ
     82,  // TEXT_TMI
     62,  // TEXT_TPSL
     61,  // TEXT_TPSU
     60,  // TEXT_WRTC
     81,  // TEXT_WRTD
     73,  // TEXT_WRTE
     51,  // TEXT_ZBRR
     63,  // TEXT_ZBSR
};

// clang-format on

typedef EntryPageBase<Entry> EntryPage;

static constexpr EntryPage SCN2650_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_2650), ARRAY_RANGE(INDEX_2650)},
};

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == REG0 || opr == R123)
        return table == REGN;
    if (opr == C012)
        return table == CCVN;
    if (opr == IMM8)
        return table == ABS7 || table == AB13 || table == AB15 || table == REL7 || table == IX15;
    if (opr == AB15)
        return table == ABS7 || table == AB13 || table == REL7 || table == IX15;
    if (opr == IX15)
        return table == AB15 || table == IX13 || table == AB13;
    return false;
}

static bool acceptModes(Entry::Flags flags, const Entry *entry) {
    const Entry::Flags table = entry->flags();
    return acceptMode(flags.mode1(), table.mode1()) && acceptMode(flags.mode2(), table.mode2());
}

Error TableScn2650::searchName(InsnScn2650 &insn) {
    uint8_t count = 0;
    auto entry = searchEntry(insn.name(), insn.flags(), SCN2650_PAGES, acceptModes, count);
    if (entry) {
        insn.setOpCode(entry->opCode());
        insn.setFlags(entry->flags());
        return setOK();
    }
    return setError(count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED);
}

static Config::opcode_t tableCode(Config::opcode_t opCode, const Entry *entry) {
    //const Entry::Flags flags = entry->flags();
    //const AddrMode mode1 = flags.mode1();
    //const AddrMode mode2 = flags.mode2();
    Config::opcode_t mask = 0;
    // TODO
    return opCode & ~mask;
}

Error TableScn2650::searchOpCode(InsnScn2650 &insn) {
    auto opCode = insn.opCode();
    auto entry = searchEntry(opCode, ARRAY_RANGE(TABLE_2650), tableCode);
    if (!entry)
        return setError(UNKNOWN_INSTRUCTION);
    insn.setFlags(entry->flags());
    insn.nameBuffer().text_P(entry->name_P());
    return setOK();
}

const /* PROGMEM */ char *TableScn2650::listCpu_P() const {
    return TEXT_CPU_LIST;
}

const /* PROGMEM */ char *TableScn2650::cpu_P() const {
    return TEXT_CPU_2650;
}

bool TableScn2650::setCpu(const char *cpu) {
    return strcmp_P(cpu, TEXT_CPU_2650) == 0 || strcasecmp_P(cpu, TEXT_CPU_SCN2650) == 0;
}

TableScn2650 TableScn2650::TABLE;

}  // namespace scn2650
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
