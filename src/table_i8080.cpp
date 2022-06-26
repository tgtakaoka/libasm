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

#include "table_i8080.h"

#include <ctype.h>
#include <string.h>

#include "config_i8080.h"
#include "entry_i8080.h"
#include "text_i8080.h"

namespace libasm {
namespace i8080 {

#define E(_opc, _name, _dst, _src) \
    { _opc, Entry::Flags::create(_dst, _src), _name }

// clang-format off
static constexpr Entry TABLE_I8080[] PROGMEM = {
    E(0x00, TEXT_NOP,  M_NO,   M_NO),
    E(0x01, TEXT_LXI,  M_PTR,  M_IM16),
    E(0x09, TEXT_DAD,  M_PTR,  M_NO),
    E(0x02, TEXT_STAX, M_IDX,  M_NO),
    E(0x22, TEXT_SHLD, M_ABS,  M_NO),
    E(0x32, TEXT_STA,  M_ABS,  M_NO),
    E(0x0A, TEXT_LDAX, M_IDX,  M_NO),
    E(0x2A, TEXT_LHLD, M_ABS,  M_NO),
    E(0x3A, TEXT_LDA,  M_ABS,  M_NO),
    E(0x03, TEXT_INX,  M_PTR,  M_NO),
    E(0x0B, TEXT_DCX,  M_PTR,  M_NO),
    E(0x04, TEXT_INR,  M_DST,  M_NO),
    E(0x05, TEXT_DCR,  M_DST,  M_NO),
    E(0x06, TEXT_MVI,  M_DST,  M_IM8),
    E(0x07, TEXT_RLC,  M_NO,   M_NO),
    E(0x0F, TEXT_RRC,  M_NO,   M_NO),
    E(0x17, TEXT_RAL,  M_NO,   M_NO),
    E(0x1F, TEXT_RAR,  M_NO,   M_NO),
    E(0x27, TEXT_DAA,  M_NO,   M_NO),
    E(0x2F, TEXT_CMA,  M_NO,   M_NO),
    E(0x37, TEXT_STC,  M_NO,   M_NO),
    E(0x3F, TEXT_CMC,  M_NO,   M_NO),
    E(0x76, TEXT_HLT,  M_NO,   M_NO),
    E(0x40, TEXT_MOV,  M_DST,  M_REG),
    E(0x80, TEXT_ADD,  M_REG,  M_NO),
    E(0x88, TEXT_ADC,  M_REG,  M_NO),
    E(0x90, TEXT_SUB,  M_REG,  M_NO),
    E(0x98, TEXT_SBB,  M_REG,  M_NO),
    E(0xA0, TEXT_ANA,  M_REG,  M_NO),
    E(0xA8, TEXT_XRA,  M_REG,  M_NO),
    E(0xB0, TEXT_ORA,  M_REG,  M_NO),
    E(0xB8, TEXT_CMP,  M_REG,  M_NO),
    E(0xC0, TEXT_RNZ,  M_NO,   M_NO),
    E(0xC8, TEXT_RZ,   M_NO,   M_NO),
    E(0xD0, TEXT_RNC,  M_NO,   M_NO),
    E(0xD8, TEXT_RC,   M_NO,   M_NO),
    E(0xE0, TEXT_RPO,  M_NO,   M_NO),
    E(0xE8, TEXT_RPE,  M_NO,   M_NO),
    E(0xF0, TEXT_RP,   M_NO,   M_NO),
    E(0xF8, TEXT_RM,   M_NO,   M_NO),
    E(0xC1, TEXT_POP,  M_STK,  M_NO),
    E(0xC9, TEXT_RET,  M_NO,   M_NO),
    E(0xE9, TEXT_PCHL, M_NO,   M_NO),
    E(0xF9, TEXT_SPHL, M_NO,   M_NO),
    E(0xC2, TEXT_JNZ,  M_ABS,  M_NO),
    E(0xCA, TEXT_JZ,   M_ABS,  M_NO),
    E(0xD2, TEXT_JNC,  M_ABS,  M_NO),
    E(0xDA, TEXT_JC,   M_ABS,  M_NO),
    E(0xE2, TEXT_JPO,  M_ABS,  M_NO),
    E(0xEA, TEXT_JPE,  M_ABS,  M_NO),
    E(0xF2, TEXT_JP,   M_ABS,  M_NO),
    E(0xFA, TEXT_JM,   M_ABS,  M_NO),
    E(0xC3, TEXT_JMP,  M_ABS,  M_NO),
    E(0xD3, TEXT_OUT,  M_IOA,  M_NO),
    E(0xDB, TEXT_IN,   M_IOA,  M_NO),
    E(0xE3, TEXT_XTHL, M_NO,   M_NO),
    E(0xEB, TEXT_XCHG, M_NO,   M_NO),
    E(0xF3, TEXT_DI,   M_NO,   M_NO),
    E(0xFB, TEXT_EI,   M_NO,   M_NO),
    E(0xC4, TEXT_CNZ,  M_ABS,  M_NO),
    E(0xCC, TEXT_CZ,   M_ABS,  M_NO),
    E(0xD4, TEXT_CNC,  M_ABS,  M_NO),
    E(0xDC, TEXT_CC,   M_ABS,  M_NO),
    E(0xE4, TEXT_CPO,  M_ABS,  M_NO),
    E(0xEC, TEXT_CPE,  M_ABS,  M_NO),
    E(0xF4, TEXT_CP,   M_ABS,  M_NO),
    E(0xFC, TEXT_CM,   M_ABS,  M_NO),
    E(0xC5, TEXT_PUSH, M_STK,  M_NO),
    E(0xCD, TEXT_CALL, M_ABS,  M_NO),
    E(0xC6, TEXT_ADI,  M_IM8,  M_NO),
    E(0xCE, TEXT_ACI,  M_IM8,  M_NO),
    E(0xD6, TEXT_SUI,  M_IM8,  M_NO),
    E(0xDE, TEXT_SBI,  M_IM8,  M_NO),
    E(0xE6, TEXT_ANI,  M_IM8,  M_NO),
    E(0xEE, TEXT_XRI,  M_IM8,  M_NO),
    E(0xF6, TEXT_ORI,  M_IM8,  M_NO),
    E(0xFE, TEXT_CPI,  M_IM8,  M_NO),
    E(0xC7, TEXT_RST,  M_VEC,  M_NO),
};

static constexpr uint8_t INDEX_I8080[] PROGMEM = {
     70,  // TEXT_ACI
     25,  // TEXT_ADC
     24,  // TEXT_ADD
     69,  // TEXT_ADI
     28,  // TEXT_ANA
     73,  // TEXT_ANI
     68,  // TEXT_CALL
     62,  // TEXT_CC
     66,  // TEXT_CM
     19,  // TEXT_CMA
     21,  // TEXT_CMC
     31,  // TEXT_CMP
     61,  // TEXT_CNC
     59,  // TEXT_CNZ
     65,  // TEXT_CP
     64,  // TEXT_CPE
     76,  // TEXT_CPI
     63,  // TEXT_CPO
     60,  // TEXT_CZ
     18,  // TEXT_DAA
      2,  // TEXT_DAD
     12,  // TEXT_DCR
     10,  // TEXT_DCX
     57,  // TEXT_DI
     58,  // TEXT_EI
     22,  // TEXT_HLT
     54,  // TEXT_IN
     11,  // TEXT_INR
      9,  // TEXT_INX
     47,  // TEXT_JC
     51,  // TEXT_JM
     52,  // TEXT_JMP
     46,  // TEXT_JNC
     44,  // TEXT_JNZ
     50,  // TEXT_JP
     49,  // TEXT_JPE
     48,  // TEXT_JPO
     45,  // TEXT_JZ
      8,  // TEXT_LDA
      6,  // TEXT_LDAX
      7,  // TEXT_LHLD
      1,  // TEXT_LXI
     23,  // TEXT_MOV
     13,  // TEXT_MVI
      0,  // TEXT_NOP
     30,  // TEXT_ORA
     75,  // TEXT_ORI
     53,  // TEXT_OUT
     42,  // TEXT_PCHL
     40,  // TEXT_POP
     67,  // TEXT_PUSH
     16,  // TEXT_RAL
     17,  // TEXT_RAR
     35,  // TEXT_RC
     41,  // TEXT_RET
     14,  // TEXT_RLC
     39,  // TEXT_RM
     34,  // TEXT_RNC
     32,  // TEXT_RNZ
     38,  // TEXT_RP
     37,  // TEXT_RPE
     36,  // TEXT_RPO
     15,  // TEXT_RRC
     77,  // TEXT_RST
     33,  // TEXT_RZ
     27,  // TEXT_SBB
     72,  // TEXT_SBI
      4,  // TEXT_SHLD
     43,  // TEXT_SPHL
      5,  // TEXT_STA
      3,  // TEXT_STAX
     20,  // TEXT_STC
     26,  // TEXT_SUB
     71,  // TEXT_SUI
     56,  // TEXT_XCHG
     29,  // TEXT_XRA
     74,  // TEXT_XRI
     55,  // TEXT_XTHL
};

static constexpr Entry TABLE_I8085[] PROGMEM = {
    E(0x20, TEXT_RIM,  M_NO,   M_NO),
    E(0x30, TEXT_SIM,  M_NO,   M_NO),
};

static constexpr uint8_t INDEX_I8085[] PROGMEM = {
      0,  // TEXT_RIM
      1,  // TEXT_SIM
};
// clang-format on

static constexpr TableI8080::EntryPage I8080_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_I8080), ARRAY_RANGE(INDEX_I8080)},
};

static constexpr TableI8080::EntryPage I8085_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_I8080), ARRAY_RANGE(INDEX_I8080)},
        {ARRAY_RANGE(TABLE_I8085), ARRAY_RANGE(INDEX_I8085)},
};

static constexpr TableI8080::Cpu CPU_TABLES[] PROGMEM = {
        {I8080, TEXT_CPU_8080, ARRAY_RANGE(I8080_PAGES)},
        {I8085, TEXT_CPU_8085, ARRAY_RANGE(I8085_PAGES)},
};

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_REG)
        return table == M_DST;
    if (opr == M_IDX || opr == M_REGH)
        return table == M_PTR || table == M_STK || table == M_REG || table == M_DST;
    if (opr == M_IM16)
        return table == M_IM8 || table == M_ABS || table == M_IOA || table == M_VEC;
    return false;
}

static bool acceptModes(Entry::Flags flags, const Entry *entry) {
    auto table = entry->flags();
    return acceptMode(flags.dstMode(), table.dstMode()) &&
           acceptMode(flags.srcMode(), table.srcMode());
}

Error TableI8080::searchName(InsnI8080 &insn) {
    uint8_t count = 0;
    for (auto page = _cpu->table(); page < _cpu->end(); page++) {
        auto entry = TableBase::searchName<EntryPage, Entry, Entry::Flags>(
                insn.name(), insn.flags(), page, acceptModes, count);
        if (entry) {
            insn.setOpCode(entry->opCode());
            insn.setFlags(entry->flags());
            return setOK();
        }
    }
    return setError(count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED);
}

static Config::opcode_t tableCode(Config::opcode_t opCode, const Entry *entry) {
    const auto &flags = entry->flags();
    const auto dst = flags.dstMode();
    const auto src = flags.srcMode();
    Config::opcode_t mask = 0;
    if (dst == M_REG || src == M_REG)
        mask |= 07;
    if (dst == M_DST || dst == M_VEC)
        mask |= 070;
    if (dst == M_PTR || dst == M_STK)
        mask |= 0x30;
    if (dst == M_IDX)
        mask |= 0x10;
    return opCode & ~mask;
}

Error TableI8080::searchOpCode(InsnI8080 &insn) {
    for (auto page = _cpu->table(); page < _cpu->end(); page++) {
        auto entry = TableBase::searchCode<Entry, Config::opcode_t>(
                insn.opCode(), page->table(), page->end(), tableCode);
        if (entry) {
            insn.setFlags(entry->flags());
            insn.setName_P(entry->name_P());
            return setOK();
        }
    }
    return setError(UNKNOWN_INSTRUCTION);
}

TableI8080::TableI8080() {
    setCpu(I8080);
}

bool TableI8080::setCpu(CpuType cpuType) {
    auto t = Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLES));
    if (t == nullptr)
        return false;
    _cpu = t;
    return true;
}

const /* PROGMEM */ char *TableI8080::listCpu_P() const {
    return TEXT_CPU_LIST;
}

const /* PROGMEM */ char *TableI8080::cpu_P() const {
    return _cpu->name_P();
}

bool TableI8080::setCpu(const char *cpu) {
    if (toupper(*cpu) == 'I')
        cpu++;
    auto t = Cpu::search(cpu, ARRAY_RANGE(CPU_TABLES));
    if (t)
        return setCpu(t->cpuType());
    return false;
}

class TableI8080 TableI8080;

}  // namespace i8080
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
