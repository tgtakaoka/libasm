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

#include "table_ins8070.h"

#include <ctype.h>
#include <string.h>

#include "config_ins8070.h"
#include "entry_ins8070.h"
#include "text_ins8070.h"

namespace libasm {
namespace ins8070 {

#define E(_opc, _name, _dst, _src, _size, _mode) \
    { _opc, Entry::Flags::create(_mode, _dst, _src, SZ_##_size), _name }

// clang-format off
static constexpr Entry TABLE_INS8070[] PROGMEM = {
    E(0x00, TEXT_NOP,  OPR_NO, OPR_NO, NONE, M_IMP),
    E(0x01, TEXT_XCH,  OPR_A,  OPR_E,  BYTE, M_IMP),
    E(0x06, TEXT_LD,   OPR_A,  OPR_S,  BYTE, M_IMP),
    E(0x07, TEXT_LD,   OPR_S,  OPR_A,  BYTE, M_IMP),
    E(0x08, TEXT_PUSH, OPR_EA, OPR_NO, WORD, M_IMP),
    E(0x09, TEXT_LD,   OPR_T,  OPR_EA, WORD, M_IMP),
    E(0x0A, TEXT_PUSH, OPR_A,  OPR_NO, BYTE, M_IMP),
    E(0x0B, TEXT_LD,   OPR_EA, OPR_T,  WORD, M_IMP),
    E(0x0C, TEXT_SR,   OPR_EA, OPR_NO, WORD, M_IMP),
    E(0x0D, TEXT_DIV,  OPR_EA, OPR_T,  WORD, M_IMP),
    E(0x0E, TEXT_SL,   OPR_A,  OPR_NO, BYTE, M_IMP),
    E(0x0F, TEXT_SL,   OPR_EA, OPR_NO, WORD, M_IMP),
    E(0x10, TEXT_CALL, OPR_4,  OPR_NO, NONE, M_IMP),
    E(0x20, TEXT_JSR,  OPR_16, OPR_NO, WORD, M_ABS),
    E(0x22, TEXT_PLI,  OPR_PN, OPR_IM, WORD, M_IMM),
    E(0x24, TEXT_JMP,  OPR_16, OPR_NO, WORD, M_ABS),
    E(0x24, TEXT_LD,   OPR_BR, OPR_IM, WORD, M_IMM),
    E(0x2C, TEXT_MPY,  OPR_EA, OPR_T,  WORD, M_IMP),
    E(0x2D, TEXT_BND,  OPR_RL, OPR_NO, NONE, M_REL),
    E(0x2E, TEXT_SSM,  OPR_PN, OPR_NO, BYTE, M_IMP),
    E(0x30, TEXT_LD,   OPR_EA, OPR_BR, WORD, M_IMP),
    E(0x38, TEXT_POP,  OPR_A,  OPR_NO, BYTE, M_IMP),
    E(0x39, TEXT_AND,  OPR_S,  OPR_IM, BYTE, M_IMM),
    E(0x3A, TEXT_POP,  OPR_EA, OPR_NO, WORD, M_IMP),
    E(0x3B, TEXT_OR,   OPR_S,  OPR_IM, BYTE, M_IMM),
    E(0x3C, TEXT_SR,   OPR_A,  OPR_NO, BYTE, M_IMP),
    E(0x3D, TEXT_SRL,  OPR_A,  OPR_NO, BYTE, M_IMP),
    E(0x3E, TEXT_RR,   OPR_A,  OPR_NO, BYTE, M_IMP),
    E(0x3F, TEXT_RRL,  OPR_A,  OPR_NO, BYTE, M_IMP),
    E(0x40, TEXT_LD,   OPR_A,  OPR_E,  BYTE, M_IMP),
    E(0x44, TEXT_LD,   OPR_BR, OPR_EA, WORD, M_IMP),
    E(0x48, TEXT_LD,   OPR_E,  OPR_A,  BYTE, M_IMP),
    E(0x4C, TEXT_XCH,  OPR_EA, OPR_BR, WORD, M_IMP),
    E(0x50, TEXT_AND,  OPR_A,  OPR_E,  BYTE, M_IMP),
    E(0x55, TEXT_PUSH, OPR_SP, OPR_NO, NONE, M_UNDEF), // undefined PUSH SP
    E(0x54, TEXT_PUSH, OPR_BR, OPR_NO, WORD, M_IMP),
    E(0x58, TEXT_OR,   OPR_A,  OPR_E,  BYTE, M_IMP),
    E(0x5C, TEXT_RET,  OPR_NO, OPR_NO, NONE, M_IMP),
    E(0x5E, TEXT_POP,  OPR_PN, OPR_NO, WORD, M_IMP),
    E(0x60, TEXT_XOR,  OPR_A,  OPR_E,  BYTE, M_IMP),
    E(0x64, TEXT_BP,   OPR_RL, OPR_NO, NONE, M_REL),
    E(0x66, TEXT_BP,   OPR_PR, OPR_NO, NONE, M_REL),
    E(0x6C, TEXT_BZ,   OPR_RL, OPR_NO, NONE, M_REL),
    E(0x6E, TEXT_BZ,   OPR_PR, OPR_NO, NONE, M_REL),
    E(0x70, TEXT_ADD,  OPR_A,  OPR_E,  BYTE, M_IMP),
    E(0x74, TEXT_BRA,  OPR_RL, OPR_NO, NONE, M_REL),
    E(0x76, TEXT_BRA,  OPR_PR, OPR_NO, NONE, M_REL),
    E(0x78, TEXT_SUB,  OPR_A,  OPR_E,  BYTE, M_IMP),
    E(0x7C, TEXT_BNZ,  OPR_RL, OPR_NO, NONE, M_REL),
    E(0x7E, TEXT_BNZ,  OPR_PR, OPR_NO, NONE, M_REL),
    E(0x80, TEXT_LD,   OPR_EA, OPR_GN, WORD, M_GEN),
    E(0x8C, TEXT_ST,   OPR_EA, OPR_IM, NONE, M_UNDEF), // undefined ST EA immediate
    E(0x88, TEXT_ST,   OPR_EA, OPR_GN, WORD, M_GEN),
    E(0x94, TEXT_ILD,  OPR_A,  OPR_IM, NONE, M_UNDEF), // undefined ILD immediate
    E(0x90, TEXT_ILD,  OPR_A,  OPR_GN, BYTE, M_GEN),
    E(0x9C, TEXT_DLD,  OPR_A,  OPR_IM, NONE, M_UNDEF), // undefined DLD immediate
    E(0x98, TEXT_DLD,  OPR_A,  OPR_GN, BYTE, M_GEN),
    E(0xA0, TEXT_LD,   OPR_T,  OPR_GN, WORD, M_GEN),
    E(0xB0, TEXT_ADD,  OPR_EA, OPR_GN, WORD, M_GEN),
    E(0xB8, TEXT_SUB,  OPR_EA, OPR_GN, WORD, M_GEN),
    E(0xC0, TEXT_LD,   OPR_A,  OPR_GN, BYTE, M_GEN),
    E(0xCC, TEXT_ST,   OPR_A,  OPR_IM, NONE, M_UNDEF), // undefined ST A immediate
    E(0xC8, TEXT_ST,   OPR_A,  OPR_GN, BYTE, M_GEN),
    E(0xD0, TEXT_AND,  OPR_A,  OPR_GN, BYTE, M_GEN),
    E(0xD8, TEXT_OR,   OPR_A,  OPR_GN, BYTE, M_GEN),
    E(0xE0, TEXT_XOR,  OPR_A,  OPR_GN, BYTE, M_GEN),
    E(0xF0, TEXT_ADD,  OPR_A,  OPR_GN, BYTE, M_GEN),
    E(0xF8, TEXT_SUB,  OPR_A,  OPR_GN, BYTE, M_GEN),
};

static constexpr uint8_t INDEX_INS8070[] PROGMEM = {
     44,  // TEXT_ADD
     58,  // TEXT_ADD
     66,  // TEXT_ADD
     22,  // TEXT_AND
     33,  // TEXT_AND
     63,  // TEXT_AND
     18,  // TEXT_BND
     48,  // TEXT_BNZ
     49,  // TEXT_BNZ
     40,  // TEXT_BP
     41,  // TEXT_BP
     45,  // TEXT_BRA
     46,  // TEXT_BRA
     42,  // TEXT_BZ
     43,  // TEXT_BZ
     12,  // TEXT_CALL
      9,  // TEXT_DIV
     55,  // TEXT_DLD
     56,  // TEXT_DLD
     53,  // TEXT_ILD
     54,  // TEXT_ILD
     15,  // TEXT_JMP
     13,  // TEXT_JSR
      2,  // TEXT_LD
      3,  // TEXT_LD
      5,  // TEXT_LD
      7,  // TEXT_LD
     16,  // TEXT_LD
     20,  // TEXT_LD
     29,  // TEXT_LD
     30,  // TEXT_LD
     31,  // TEXT_LD
     50,  // TEXT_LD
     57,  // TEXT_LD
     60,  // TEXT_LD
     17,  // TEXT_MPY
      0,  // TEXT_NOP
     24,  // TEXT_OR
     36,  // TEXT_OR
     64,  // TEXT_OR
     14,  // TEXT_PLI
     21,  // TEXT_POP
     23,  // TEXT_POP
     38,  // TEXT_POP
      4,  // TEXT_PUSH
      6,  // TEXT_PUSH
     34,  // TEXT_PUSH
     35,  // TEXT_PUSH
     37,  // TEXT_RET
     27,  // TEXT_RR
     28,  // TEXT_RRL
     10,  // TEXT_SL
     11,  // TEXT_SL
      8,  // TEXT_SR
     25,  // TEXT_SR
     26,  // TEXT_SRL
     19,  // TEXT_SSM
     51,  // TEXT_ST
     52,  // TEXT_ST
     61,  // TEXT_ST
     62,  // TEXT_ST
     47,  // TEXT_SUB
     59,  // TEXT_SUB
     67,  // TEXT_SUB
      1,  // TEXT_XCH
     32,  // TEXT_XCH
     39,  // TEXT_XOR
     65,  // TEXT_XOR
};
// clang-format on

struct TableIns8070::EntryPage : EntryPageBase<Entry> {
    constexpr EntryPage(
            const Entry *table, const Entry *end, const uint8_t *index, const uint8_t *iend)
        : EntryPageBase(table, end, index, iend) {}
};

static constexpr TableIns8070::EntryPage INS8070_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_INS8070), ARRAY_RANGE(INDEX_INS8070)},
};

static bool acceptOprFormat(OprFormat opr, OprFormat table) {
    if (opr == table)
        return true;
    if (opr == OPR_16)
        return table == OPR_GN || table == OPR_RL;
    if (opr == OPR_4)
        return table == OPR_GN || table == OPR_RL || table == OPR_16;
    if (opr == OPR_PR)
        return table == OPR_GN;
    if (opr == OPR_IM)
        return table == OPR_GN;
    if (opr == OPR_PN)
        return table == OPR_BR;
    if (opr == OPR_SP)
        return table == OPR_BR;
    return false;
}

static bool acceptOprFormats(Entry::Flags flags, const Entry *entry) {
    auto table = entry->flags();
    return acceptOprFormat(flags.dstOpr(), table.dstOpr()) &&
           acceptOprFormat(flags.srcOpr(), table.srcOpr());
}

Error TableIns8070::searchName(InsnIns8070 &insn) {
    uint8_t count = 0;
    auto entry = TableBase::searchName<EntryPage, Entry, Entry::Flags>(
            insn.name(), insn.flags(), INS8070_PAGES, acceptOprFormats, count);
    if (entry) {
        insn.setOpCode(entry->opCode());
        insn.setFlags(entry->flags());
        if (insn.addrMode() == M_UNDEF)
            return setError(OPERAND_NOT_ALLOWED);
        return setOK();
    }
    return setError(count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED);
}

static Config::opcode_t maskCode(OprFormat format) {
    switch (format) {
    case OPR_4:
        return 0x0F;
    case OPR_PR:
    case OPR_PN:
        return 0x01;
    case OPR_BR:
        return 0x03;
    case OPR_GN:
        return 0x07;
    default:
        return 0;
    }
}

static Config::opcode_t tableCode(Config::opcode_t opCode, const Entry *entry) {
    auto flags = entry->flags();
    opCode &= ~maskCode(flags.dstOpr());
    opCode &= ~maskCode(flags.srcOpr());
    return opCode;
}

Error TableIns8070::searchOpCode(InsnIns8070 &insn) {
    auto entry = TableBase::searchCode<Entry, Config::opcode_t>(
            insn.opCode(), ARRAY_RANGE(TABLE_INS8070), tableCode);
    if (!entry)
        return setError(UNKNOWN_INSTRUCTION);
    insn.setFlags(entry->flags());
    if (insn.addrMode() == M_UNDEF)
        return setError(UNKNOWN_INSTRUCTION);
    insn.setName_P(entry->name_P());
    return setOK();
}

const /* PROGMEM */ char *TableIns8070::listCpu_P() const {
    return TEXT_CPU_INS8070;
}

const /* PROGMEM */ char *TableIns8070::cpu_P() const {
    return TEXT_CPU_8070;
}

bool TableIns8070::setCpu(const char *cpu) {
    return strcasecmp_P(cpu, TEXT_CPU_INS8070) == 0 || strcasecmp_P(cpu, TEXT_CPU_8070) == 0;
}

class TableIns8070 TableIns8070;

}  // namespace ins8070
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
