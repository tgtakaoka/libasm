/*
 * Copyright 2023 Tadashi G. Takaoka
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

#include "table_im6100.h"

#include "entry_im6100.h"
#include "entry_table.h"
#include "text_im6100.h"

using namespace libasm::text::im6100;

namespace libasm {
namespace im6100 {

#define E(_opc, _name, _mode, _bits) \
    { _opc, Entry::Flags::create(_mode, _bits), _name }
#define C(_opc, _name, _mode, _bits, _selector) \
    { _opc, Entry::Flags::create(_mode, _bits, _selector), _name }

// clang-format off

/** Bit mask to extract |groupId| from |opCode| */
static constexpr Config::opcode_t MASK_MEM = 07000;
static constexpr Config::opcode_t MASK_GR1 = 07400;
static constexpr Config::opcode_t MASK_GR2 = 07401;
static constexpr Config::opcode_t MASK_GR3 = 07401;
static constexpr Config::opcode_t MASK_IOT = 07000;
/** Trunk of |opCode| to identify group */
static constexpr Config::opcode_t GID_MEM = 0; // wildcard
static constexpr Config::opcode_t GID_GR1 = 07000;
static constexpr Config::opcode_t GID_GR2 = 07400;
static constexpr Config::opcode_t GID_GR3 = 07401;
static constexpr Config::opcode_t GID_IOT = 06000;

static constexpr Entry IM6100_MEM[] PROGMEM = {
    E(00000, TEXT_AND,  M_MEM, 0),
    E(01000, TEXT_TAD,  M_MEM, 0),
    E(02000, TEXT_ISZ,  M_MEM, 0),
    E(03000, TEXT_DCA,  M_MEM, 0),
    E(04000, TEXT_JMS,  M_MEM, 0),
    E(05000, TEXT_JMP,  M_MEM, 0),
};

static constexpr uint8_t INDEX_IM6100_MEM[] PROGMEM = {
};

static constexpr uint8_t CLA = 0200;
static constexpr uint8_t CLL = 0100;
static constexpr uint8_t CMA = 0040;
static constexpr uint8_t CML = 0020;
static constexpr uint8_t R1 = 0010; // RAL/RTL
static constexpr uint8_t R2 = 0004; // RAR/RTR
static constexpr uint8_t R3 = 0002; // BSW
static constexpr uint8_t IAC = 0001;

static constexpr Entry IM6100_GR1[] PROGMEM = {
    C(07000, TEXT_NOP,  M_GR1, CLA|CLL|CMA|CML|IAC, R1|R2|R3),
    C(07204, TEXT_GLK,  M_GR1, CLA|R1|R2|R3, 0),
    C(07120, TEXT_STL,  M_GR1, CLL|CML, 0),
    C(07240, TEXT_STA,  M_GR1, CLA|CMA, 0),
    C(07200, TEXT_CLA,  M_GR1, CLA, 0), // Logical sequence 1
    C(07100, TEXT_CLL,  M_GR1, CLL, 0), // Logical sequence 1
    C(07041, TEXT_CIA,  M_GR1, CMA|IAC, 0),
    C(07040, TEXT_CMA,  M_GR1, CMA, 0),      // Logical sequence 2
    C(07020, TEXT_CML,  M_GR1, CML, 0),      // Logical sequence 2
    C(07002, TEXT_BSW,  M_GR1, R1|R2|R3, 0), // Logical sequence 4
    C(07004, TEXT_RAL,  M_GR1, R1|R2|R3, 0), // Logical sequence 4
    C(07010, TEXT_RAR,  M_GR1, R1|R2|R3, 0), // Logical sequence 4
    C(07006, TEXT_RTL,  M_GR1, R1|R2|R3, 0), // Logical sequence 4
    C(07012, TEXT_RTR,  M_GR1, R1|R2|R3, 0), // Logical sequence 4
    C(07001, TEXT_IAC,  M_GR1, IAC, 0),      // Logical sequence 3
};

static constexpr uint8_t INDEX_IM6100_GR1[] PROGMEM = {
};

static constexpr uint8_t SMA = 0100;
static constexpr uint8_t SPA = 0100;
static constexpr uint8_t SZA = 0040;
static constexpr uint8_t SNA = 0040;
static constexpr uint8_t SNL = 0020;
static constexpr uint8_t SZL = 0020;
static constexpr uint8_t B08 = 0010;
static constexpr uint8_t OSR = 0004;
static constexpr uint8_t HLT = 0002;

static constexpr Entry IM6100_GR2[] PROGMEM = {
    E(07400, TEXT_NOP,  M_GR2, CLA|SMA|SZA|SNL|B08|OSR|HLT),
    E(07410, TEXT_SKP,  M_GR2, SMA|SZA|SNL|B08),
    C(07500, TEXT_SMA,  M_GR2, SMA, B08), // Logical sequence 1
    C(07510, TEXT_SPA,  M_GR2, SPA, B08), // Logical sequence 1
    C(07440, TEXT_SZA,  M_GR2, SZA, B08), // Logical sequence 1
    C(07450, TEXT_SNA,  M_GR2, SNA, B08), // Logical sequence 1
    C(07420, TEXT_SNL,  M_GR2, SNL, B08), // Logical sequence 1
    C(07430, TEXT_SZL,  M_GR2, SZL, B08), // Logical sequence 1
    C(07604, TEXT_LAS,  M_GR2, CLA|OSR, 0),
    C(07600, TEXT_CLA,  M_GR2, CLA, 0), // Logical sequence 2
    C(07404, TEXT_OSR,  M_GR2, OSR, 0), // Logical sequence 3
    C(07402, TEXT_HLT,  M_GR2, HLT, 0), // Logical sequence 3
};

static constexpr uint8_t INDEX_IM6100_GR2[] PROGMEM = {
};

static constexpr uint8_t MQA = 0100;
static constexpr uint8_t MQL = 0020;

static constexpr Entry IM6100_GR3[] PROGMEM = {
    E(07401, TEXT_NOP,  M_GR3, CLA|MQA|MQL),
    C(07701, TEXT_ACL,  M_GR3, CLA|MQA, MQL),
    C(07621, TEXT_CAM,  M_GR3, CLA|MQL, MQA),
    C(07601, TEXT_CLA,  M_GR3, CLA, 0), // Logical sequence 1
    C(07521, TEXT_SWP,  M_GR3, MQA|MQL, 0),
    C(07501, TEXT_MQA,  M_GR3, MQA, 0), // Logical sequence 2
    C(07421, TEXT_MQL,  M_GR3, MQL, 0), // Logical sequence 3
};

static constexpr uint8_t INDEX_IM6100_GR3[] PROGMEM = {
};

static constexpr Entry IM6100_IOT[] PROGMEM = {
    E(06000, TEXT_SKON, M_CMP, 00777),
    E(06001, TEXT_ION,  M_CMP, 00777),
    E(06002, TEXT_IOF,  M_CMP, 00777),
    E(06003, TEXT_SRQ,  M_CMP, 00777),
    E(06004, TEXT_GTF,  M_CMP, 00777),
    E(06005, TEXT_RTF,  M_CMP, 00777),
    E(06006, TEXT_SGT,  M_CMP, 00777),
    E(06007, TEXT_CAF,  M_CMP, 00777),
    E(06000, TEXT_IOT,  M_IOT, 0),
};

static constexpr uint8_t INDEX_IM6100_IOT[] PROGMEM = {
};

// clang-format on

struct EntryPage : entry::TableBase<Entry> {
    AddrMode group() const { return _entries.table()->flags().mode(); }
    Config::opcode_t groupMask() const { return pgm_read_word(&_groupMask); }
    Config::opcode_t groupId() const { return pgm_read_word(&_groupId); }

    constexpr EntryPage(const Entry *table, const Entry *end, const uint8_t *index,
            const uint8_t *iend, Config::opcode_t groupMask, Config::opcode_t groupId)
        : TableBase(table, end, index, iend), _groupMask(groupMask), _groupId(groupId) {}

    Config::opcode_t appendMicros(
            const DisInsn &insn, Config::opcode_t micros, StrBuffer &out) const {
        const auto gmask = groupMask();
        auto clearMask = insn.selector();
        bool addSep = false;
        for (const auto *entry = _entries.table(); micros && entry != _entries.end(); entry++) {
            const auto flags = entry->flags();
            if (!flags.combination())
                continue;
            const auto mask = flags.bits() | flags.selector();
            if ((micros & mask) == (entry->opCode() & ~gmask)) {
                micros &= ~flags.bits();
                clearMask |= flags.selector();
                if (addSep)
                    out.letter(' ');
                out.text_P(entry->name_P());
                addSep = true;
            }
        }
        return micros & ~clearMask;
    }

private:
    const Config::opcode_t _groupMask;
    const Config::opcode_t _groupId;
};

struct Cpu : entry::CpuBase<CpuType, EntryPage> {
    constexpr Cpu(CpuType cpuType, const /*PROGMEM*/ char *name_P, const EntryPage *table,
            const EntryPage *end)
        : CpuBase(cpuType, name_P, table, end) {}

    void appendMicros(DisInsn &insn, StrBuffer &out) const {
        auto micros = insn.opCode() & ~insn.bits();
        for (const auto *page = _pages.table(); page != _pages.end(); page++) {
            if (page->group() == insn.mode()) {
                micros &= ~page->groupMask();
                micros = page->appendMicros(insn, micros, out);
            }
        }
        if (micros)
            insn.setErrorIf(INVALID_INSTRUCTION);
    };
};

static constexpr EntryPage IM6100_PAGES[] PROGMEM = {
        {ARRAY_RANGE(IM6100_GR1), ARRAY_RANGE(INDEX_IM6100_GR1), MASK_GR1, GID_GR1},
        {ARRAY_RANGE(IM6100_GR2), ARRAY_RANGE(INDEX_IM6100_GR2), MASK_GR2, GID_GR2},
        {ARRAY_RANGE(IM6100_GR3), ARRAY_RANGE(INDEX_IM6100_GR3), MASK_GR3, GID_GR3},
        {ARRAY_RANGE(IM6100_IOT), ARRAY_RANGE(INDEX_IM6100_IOT), MASK_IOT, GID_IOT},
        {ARRAY_RANGE(IM6100_MEM), ARRAY_RANGE(INDEX_IM6100_MEM), MASK_MEM, GID_MEM},
};

static constexpr Cpu CPU_TABLE[] PROGMEM = {
        {IM6100, TEXT_CPU_6100, ARRAY_RANGE(IM6100_PAGES)},
};
static const Cpu *cpu(CpuType cpuType) {
    UNUSED(cpuType);
    return &CPU_TABLE[0];
}

static bool pageMatcher(DisInsn &insn, const EntryPage *page) {
    const auto groupId = page->groupId();
    return (insn.opCode() & page->groupMask()) == groupId || groupId == 0;
}

static bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *page) {
    UNUSED(page);
    const auto flags = entry->flags();
    const auto mask = flags.bits() | flags.selector() | page->groupMask();
    const auto masked = insn.opCode() & mask;
    return masked == entry->opCode();
}

Error TableIm6100::searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) const {
    const auto *entry = cpu(cpuType)->searchOpCode(insn, out, matchOpCode, pageMatcher);
    if (entry) {
        const auto mode = insn.mode();
        if (mode == M_GR1 || mode == M_GR2 || mode == M_GR3)
            cpu(cpuType)->appendMicros(insn, out);
    }
    return insn.getError();
}

const /*PROGMEM*/ char *TableIm6100::listCpu_P() const {
    return TEXT_CPU_IM6100;
}

const /*PROGMEM*/ char *TableIm6100::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableIm6100::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    if (name.iequals(TEXT_CPU_6100) || name.iequals(TEXT_CPU_IM6100)) {
        cpuType = IM6100;
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TableIm6100 TABLE;

}  // namespace im6100
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
