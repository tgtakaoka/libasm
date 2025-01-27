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

#include "table_pdp8.h"
#include "entry_pdp8.h"
#include "entry_table.h"
#include "text_pdp8.h"

using namespace libasm::text::pdp8;

namespace libasm {
namespace pdp8 {

#define E(_opc, _name, _mode, _bits) {_opc, Entry::Flags::create(_mode, _bits), _name}
#define C(_opc, _name, _mode, _bits, _selector) \
    {_opc, Entry::Flags::create(_mode, _bits, _selector), _name}
#define M(_opc, _name, _mode, _bits, _selector) \
    {_opc, Entry::Flags::create(_mode, _bits, _selector, true), _name}

// clang-format off

/** Bit mask to extract |groupId| from |opCode| */
constexpr Config::opcode_t MASK_MEM = 07000;
constexpr Config::opcode_t MASK_GR1 = 07400;
constexpr Config::opcode_t MASK_GR2 = 07401;
constexpr Config::opcode_t MASK_GR3 = 07401;
constexpr Config::opcode_t MASK_MEX = 07704;
constexpr Config::opcode_t MASK_I20 = 07704;
constexpr Config::opcode_t MASK_IOT = 07000;
/** Trunk of |opCode| to identify group */
constexpr Config::opcode_t GID_MEM = 0; // wildcard
constexpr Config::opcode_t GID_GR1 = 07000;
constexpr Config::opcode_t GID_GR2 = 07400;
constexpr Config::opcode_t GID_GR3 = 07401;
constexpr Config::opcode_t GID_MEX = 06200;
constexpr Config::opcode_t GID_I20 = 06204;
constexpr Config::opcode_t GID_IOT = 06000;
/** Clear variable part mask */
constexpr Config::opcode_t CLEAR_MEX = 00070;

constexpr Entry IM6100_MEM[] PROGMEM = {
    E(00000, TEXT_AND,  M_MEM, 0),
    E(01000, TEXT_TAD,  M_MEM, 0),
    E(02000, TEXT_ISZ,  M_MEM, 0),
    E(03000, TEXT_DCA,  M_MEM, 0),
    E(04000, TEXT_JMS,  M_MEM, 0),
    E(05000, TEXT_JMP,  M_MEM, 0),
};

constexpr uint8_t INDEX_IM6100_MEM[] PROGMEM = {
      0,  // TEXT_AND
      3,  // TEXT_DCA
      2,  // TEXT_ISZ
      5,  // TEXT_JMP
      4,  // TEXT_JMS
      1,  // TEXT_TAD
};

constexpr uint8_t CLA = 0200;
constexpr uint8_t CLL = 0100;
constexpr uint8_t CMA = 0040;
constexpr uint8_t CML = 0020;
constexpr uint8_t R1 = 0010; // RAL/RTL
constexpr uint8_t R2 = 0004; // RAR/RTR
constexpr uint8_t R3 = 0002; // BSW
constexpr uint8_t IAC = 0001;

constexpr Entry IM6100_GR1[] PROGMEM = {
    C(07000, TEXT_NOP,  M_GR1, CLA|CLL|CMA|CML|IAC, R1|R2|R3),
    C(07204, TEXT_GLK,  M_GR1, CLA|R1|R2|R3, 0),
    C(07120, TEXT_STL,  M_GR1, CLL|CML, 0),
    C(07240, TEXT_STA,  M_GR1, CLA|CMA, 0),
    M(07200, TEXT_CLA,  M_GR1, CLA, 0), // Logical sequence 1
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

constexpr uint8_t INDEX_IM6100_GR1[] PROGMEM = {
      9,  // TEXT_BSW
      6,  // TEXT_CIA
      4,  // TEXT_CLA
      5,  // TEXT_CLL
      7,  // TEXT_CMA
      8,  // TEXT_CML
      1,  // TEXT_GLK
     14,  // TEXT_IAC
      0,  // TEXT_NOP
     10,  // TEXT_RAL
     11,  // TEXT_RAR
     12,  // TEXT_RTL
     13,  // TEXT_RTR
      3,  // TEXT_STA
      2,  // TEXT_STL
};

constexpr Entry HD6120_GR1[] PROGMEM = {
    C(07000, TEXT_NOP,  M_GR1, CLA|CLL|CMA|CML|IAC, R1|R2|R3),
    C(07204, TEXT_GLK,  M_GR1, CLA|R1|R2|R3, 0),
    C(07120, TEXT_STL,  M_GR1, CLL|CML, 0),
    C(07240, TEXT_STA,  M_GR1, CLA|CMA, 0),
    M(07200, TEXT_CLA,  M_GR1, CLA, 0), // Logical sequence 1
    C(07100, TEXT_CLL,  M_GR1, CLL, 0), // Logical sequence 1
    C(07041, TEXT_CIA,  M_GR1, CMA|IAC, 0),
    C(07040, TEXT_CMA,  M_GR1, CMA, 0),      // Logical sequence 2
    C(07020, TEXT_CML,  M_GR1, CML, 0),      // Logical sequence 2
    C(07002, TEXT_BSW,  M_GR1, R1|R2|R3, 0), // Logical sequence 4
    C(07004, TEXT_RAL,  M_GR1, R1|R2|R3, 0), // Logical sequence 4
    C(07010, TEXT_RAR,  M_GR1, R1|R2|R3, 0), // Logical sequence 4
    C(07006, TEXT_RTL,  M_GR1, R1|R2|R3, 0), // Logical sequence 4
    C(07012, TEXT_RTR,  M_GR1, R1|R2|R3, 0), // Logical sequence 4
    C(07014, TEXT_R3L,  M_GR1, R1|R2|R3, 0), // Logical sequence 4
    C(07001, TEXT_IAC,  M_GR1, IAC, 0),      // Logical sequence 3
};

constexpr uint8_t INDEX_HD6120_GR1[] PROGMEM = {
      9,  // TEXT_BSW
      6,  // TEXT_CIA
      4,  // TEXT_CLA
      5,  // TEXT_CLL
      7,  // TEXT_CMA
      8,  // TEXT_CML
      1,  // TEXT_GLK
     15,  // TEXT_IAC
      0,  // TEXT_NOP
     14,  // TEXT_R3L
     10,  // TEXT_RAL
     11,  // TEXT_RAR
     12,  // TEXT_RTL
     13,  // TEXT_RTR
      3,  // TEXT_STA
      2,  // TEXT_STL
};

constexpr uint8_t SMA = 0100;
constexpr uint8_t SPA = 0100;
constexpr uint8_t SZA = 0040;
constexpr uint8_t SNA = 0040;
constexpr uint8_t SNL = 0020;
constexpr uint8_t SZL = 0020;
constexpr uint8_t B08 = 0010;
constexpr uint8_t OSR = 0004;
constexpr uint8_t HLT = 0002;

constexpr Entry IM6100_GR2[] PROGMEM = {
    E(07400, TEXT_NOP,  M_GR2, CLA|SMA|SZA|SNL|B08|OSR|HLT),
    E(07410, TEXT_SKP,  M_GR2, SMA|SZA|SNL|B08),
    C(07500, TEXT_SMA,  M_GR2, SMA, B08), // Logical sequence 1
    C(07510, TEXT_SPA,  M_GR2, SPA, B08), // Logical sequence 1
    C(07440, TEXT_SZA,  M_GR2, SZA, B08), // Logical sequence 1
    C(07450, TEXT_SNA,  M_GR2, SNA, B08), // Logical sequence 1
    C(07420, TEXT_SNL,  M_GR2, SNL, B08), // Logical sequence 1
    C(07430, TEXT_SZL,  M_GR2, SZL, B08), // Logical sequence 1
    C(07604, TEXT_LAS,  M_GR2, CLA|OSR, 0),
    M(07600, TEXT_CLA,  M_GR2, CLA, 0), // Logical sequence 2
    C(07404, TEXT_OSR,  M_GR2, OSR, 0), // Logical sequence 3
    C(07402, TEXT_HLT,  M_GR2, HLT, 0), // Logical sequence 3
};

constexpr uint8_t INDEX_IM6100_GR2[] PROGMEM = {
      9,  // TEXT_CLA
     11,  // TEXT_HLT
      8,  // TEXT_LAS
      0,  // TEXT_NOP
     10,  // TEXT_OSR
      1,  // TEXT_SKP
      2,  // TEXT_SMA
      5,  // TEXT_SNA
      6,  // TEXT_SNL
      3,  // TEXT_SPA
      4,  // TEXT_SZA
      7,  // TEXT_SZL
};

constexpr uint8_t MQA = 0100;
constexpr uint8_t MQL = 0020;

constexpr Entry IM6100_GR3[] PROGMEM = {
    E(07401, TEXT_NOP,  M_GR3, CLA|MQA|MQL),
    C(07701, TEXT_ACL,  M_GR3, CLA|MQA, MQL),
    C(07621, TEXT_CAM,  M_GR3, CLA|MQL, MQA),
    C(07601, TEXT_CLA,  M_GR3, CLA, 0), // Logical sequence 1
    C(07521, TEXT_SWP,  M_GR3, MQA|MQL, 0),
    C(07501, TEXT_MQA,  M_GR3, MQA, 0), // Logical sequence 2
    C(07421, TEXT_MQL,  M_GR3, MQL, 0), // Logical sequence 3
};

constexpr uint8_t INDEX_IM6100_GR3[] PROGMEM = {
      1,  // TEXT_ACL
      2,  // TEXT_CAM
      3,  // TEXT_CLA
      5,  // TEXT_MQA
      6,  // TEXT_MQL
      0,  // TEXT_NOP
      4,  // TEXT_SWP
};

constexpr Entry IM6100_IOT[] PROGMEM = {
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

constexpr uint8_t INDEX_IM6100_IOT[] PROGMEM = {
      7,  // TEXT_CAF
      4,  // TEXT_GTF
      2,  // TEXT_IOF
      1,  // TEXT_ION
      8,  // TEXT_IOT
      5,  // TEXT_RTF
      6,  // TEXT_SGT
      0,  // TEXT_SKON
      3,  // TEXT_SRQ
};

constexpr uint8_t CDF = 0001;
constexpr uint8_t CIF = 0002;
constexpr uint8_t B09 = 0004;

constexpr Entry HD6120_MEX[] PROGMEM = {
    C(06201, TEXT_CDF,  M_MEX, CDF, B09),
    C(06202, TEXT_CIF,  M_MEX, CIF, B09),
};

constexpr uint8_t INDEX_HD6120_MEX[] PROGMEM = {
      0,  // TEXT_CDF
      1,  // TEXT_CIF
};

constexpr Entry HD6120_I20[] PROGMEM = {
    E(06205, TEXT_PPC1, M_CMP, 00073),
    E(06245, TEXT_PPC2, M_CMP, 00073),
    E(06215, TEXT_PAC1, M_CMP, 00073),
    E(06255, TEXT_PAC2, M_CMP, 00073),
    E(06225, TEXT_RTN1, M_CMP, 00073),
    E(06265, TEXT_RTN2, M_CMP, 00073),
    E(06235, TEXT_POP1, M_CMP, 00073),
    E(06275, TEXT_POP2, M_CMP, 00073),
    E(06207, TEXT_RSP1, M_CMP, 00073),
    E(06227, TEXT_RSP2, M_CMP, 00073),
    E(06217, TEXT_LSP1, M_CMP, 00073),
    E(06237, TEXT_LSP2, M_CMP, 00073),
    E(06246, TEXT_WSR,  M_CMP, 00073),
    E(06256, TEXT_GCF,  M_CMP, 00073),
    E(06214, TEXT_RDF,  M_CMP, 00073),
    E(06224, TEXT_RIF,  M_CMP, 00073),
    E(06234, TEXT_RIB,  M_CMP, 00073),
    E(06244, TEXT_RMF,  M_CMP, 00073),
    E(06206, TEXT_PR0,  M_CMP, 00073),
    E(06216, TEXT_PR1,  M_CMP, 00073),
    E(06226, TEXT_PR2,  M_CMP, 00073),
    E(06236, TEXT_PR3,  M_CMP, 00073),
};

constexpr uint8_t INDEX_HD6120_I20[] PROGMEM = {
     13,  // TEXT_GCF
     10,  // TEXT_LSP1
     11,  // TEXT_LSP2
      2,  // TEXT_PAC1
      3,  // TEXT_PAC2
      6,  // TEXT_POP1
      7,  // TEXT_POP2
      0,  // TEXT_PPC1
      1,  // TEXT_PPC2
     18,  // TEXT_PR0
     19,  // TEXT_PR1
     20,  // TEXT_PR2
     21,  // TEXT_PR3
     14,  // TEXT_RDF
     16,  // TEXT_RIB
     15,  // TEXT_RIF
     17,  // TEXT_RMF
      8,  // TEXT_RSP1
      9,  // TEXT_RSP2
      4,  // TEXT_RTN1
      5,  // TEXT_RTN2
     12,  // TEXT_WSR
};

// clang-format on

struct EntryPage : entry::TableBase<Entry> {
    AddrMode readGroup() const { return _entries.readHead()->readFlags().mode(); }
    Config::opcode_t readGroupMask() const { return pgm_read_word(&_groupMask_P); }
    Config::opcode_t readGroupId() const { return pgm_read_word(&_groupId_P); }

    constexpr EntryPage(const Entry *head_P, const Entry *tail_P, const uint8_t *index_P,
            const uint8_t *itail_P, Config::opcode_t groupMask, Config::opcode_t groupId)
        : TableBase(head_P, tail_P, index_P, itail_P),
          _groupMask_P(groupMask),
          _groupId_P(groupId) {}

    Config::opcode_t appendMicros(
            const DisInsn &insn, Config::opcode_t micros, StrBuffer &out) const;

private:
    const Config::opcode_t _groupMask_P;
    const Config::opcode_t _groupId_P;
};

Config::opcode_t EntryPage::appendMicros(
        const DisInsn &insn, Config::opcode_t micros, StrBuffer &out) const {
    const auto gmask = readGroupMask();
    auto clearMask = insn.selector();
    bool addSep = false;
    const auto *tail = _entries.readTail();
    for (const auto *entry = _entries.readHead(); micros && entry != tail; entry++) {
        const auto flags = entry->readFlags();
        if (!flags.combination())
            continue;
        const auto mask = flags.bits() | flags.selector();
        if ((micros & mask) == (entry->readOpCode() & ~gmask)) {
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

struct Cpu : entry::CpuBase<CpuType, EntryPage> {
    constexpr Cpu(CpuType cpuType, const /*PROGMEM*/ char *name_P, const EntryPage *head_P,
            const EntryPage *tail_P)
        : CpuBase(cpuType, name_P, head_P, tail_P) {}

    void appendMicros(DisInsn &insn, StrBuffer &out) const;
};

void Cpu::appendMicros(DisInsn &insn, StrBuffer &out) const {
    auto micros = insn.opCode() & ~insn.bits();
    const auto *pageTail = _pages.readTail();
    for (const auto *page = _pages.readHead(); page != pageTail; page++) {
        if (page->readGroup() == insn.mode()) {
            micros &= ~page->readGroupMask();
            if (insn.mode() == M_MEX)
                micros &= ~CLEAR_MEX;
            micros = page->appendMicros(insn, micros, out);
        }
    }
    if (micros)
        insn.setErrorIf(INVALID_INSTRUCTION);
}

constexpr EntryPage IM6100_PAGES[] PROGMEM = {
        {ARRAY_RANGE(IM6100_GR1), ARRAY_RANGE(INDEX_IM6100_GR1), MASK_GR1, GID_GR1},
        {ARRAY_RANGE(IM6100_GR2), ARRAY_RANGE(INDEX_IM6100_GR2), MASK_GR2, GID_GR2},
        {ARRAY_RANGE(IM6100_GR3), ARRAY_RANGE(INDEX_IM6100_GR3), MASK_GR3, GID_GR3},
        {ARRAY_RANGE(IM6100_IOT), ARRAY_RANGE(INDEX_IM6100_IOT), MASK_IOT, GID_IOT},
        {ARRAY_RANGE(IM6100_MEM), ARRAY_RANGE(INDEX_IM6100_MEM), MASK_MEM, GID_MEM},
};

constexpr EntryPage HD6120_PAGES[] PROGMEM = {
        {ARRAY_RANGE(HD6120_GR1), ARRAY_RANGE(INDEX_HD6120_GR1), MASK_GR1, GID_GR1},
        {ARRAY_RANGE(IM6100_GR1), ARRAY_RANGE(INDEX_IM6100_GR1), MASK_GR1, GID_GR1},
        {ARRAY_RANGE(IM6100_GR2), ARRAY_RANGE(INDEX_IM6100_GR2), MASK_GR2, GID_GR2},
        {ARRAY_RANGE(IM6100_GR3), ARRAY_RANGE(INDEX_IM6100_GR3), MASK_GR3, GID_GR3},
        {ARRAY_RANGE(HD6120_MEX), ARRAY_RANGE(INDEX_HD6120_MEX), MASK_MEX, GID_MEX},
        {ARRAY_RANGE(HD6120_I20), ARRAY_RANGE(INDEX_HD6120_I20), MASK_I20, GID_I20},
        {ARRAY_RANGE(IM6100_IOT), ARRAY_RANGE(INDEX_IM6100_IOT), MASK_IOT, GID_IOT},
        {ARRAY_RANGE(IM6100_MEM), ARRAY_RANGE(INDEX_IM6100_MEM), MASK_MEM, GID_MEM},
};

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {IM6100, TEXT_CPU_6100, ARRAY_RANGE(IM6100_PAGES)},
        {HD6120, TEXT_CPU_6120, ARRAY_RANGE(HD6120_PAGES)},
};
const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

Error searchName(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, Cpu::acceptAll<AsmInsn, Entry>);
    return insn.getError();
}

bool acceptSameMode(AsmInsn &insn, const Entry *entry) {
    return insn.mode() == entry->readFlags().mode();
}

Error searchMicro(CpuType cpuType, AsmInsn &micro, AddrMode mode) {
    micro.setOK();
    micro.setAddrMode(mode);
    cpu(cpuType)->searchName(micro, acceptSameMode);
    return micro.getError();
}

bool pageMatcher(DisInsn &insn, const EntryPage *page) {
    const auto groupId = page->readGroupId();
    return (insn.opCode() & page->readGroupMask()) == groupId || groupId == 0;
}

bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *page) {
    const auto flags = entry->readFlags();
    const auto mask = flags.bits() | flags.selector() | page->readGroupMask();
    const auto masked = insn.opCode() & mask;
    return masked == entry->readOpCode();
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    const auto *entry = cpu(cpuType)->searchOpCode(insn, out, matchOpCode, pageMatcher);
    if (entry) {
        const auto mode = insn.mode();
        if (mode == M_GR1 || mode == M_GR2 || mode == M_GR3 || mode == M_MEX)
            cpu(cpuType)->appendMicros(insn, out);
    }
    return insn.getError();
}

const /*PROGMEM*/ char *TablePdp8::listCpu_P() const {
    return TEXT_PDP8_LIST;
}

const /*PROGMEM*/ char *TablePdp8::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TablePdp8::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    if (name.iequals(TEXT_CPU_6100) || name.iequals(TEXT_CPU_IM6100)) {
        cpuType = IM6100;
        return OK;
    }
    if (name.iequals(TEXT_CPU_6120) || name.iequals(TEXT_CPU_HD6120)) {
        cpuType = HD6120;
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TablePdp8 TABLE;

}  // namespace pdp8
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
