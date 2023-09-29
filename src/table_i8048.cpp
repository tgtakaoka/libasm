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

#include "table_i8048.h"

#include "entry_i8048.h"
#include "entry_table.h"
#include "text_i8048.h"

using namespace libasm::text::i8048;

namespace libasm {
namespace i8048 {

#define E2(_opc, _name, _dst, _src) \
    { _opc, Entry::Flags::create(_dst, _src), _name }
#define E1(_opc, _name, _dst) E2(_opc, _name, _dst, M_NONE)
#define E0(_opc, _name) E1(_opc, _name, M_NONE)

// clang-format off
static constexpr Entry TABLE_I8039[] PROGMEM = {
    E2(0x68, TEXT_ADD,  M_A,    M_R),
    E2(0x60, TEXT_ADD,  M_A,    M_IR),
    E2(0x03, TEXT_ADD,  M_A,    M_IMM8),
    E2(0x78, TEXT_ADDC, M_A,    M_R),
    E2(0x70, TEXT_ADDC, M_A,    M_IR),
    E2(0x13, TEXT_ADDC, M_A,    M_IMM8),
    E2(0x58, TEXT_ANL,  M_A,    M_R),
    E2(0x50, TEXT_ANL,  M_A,    M_IR),
    E2(0x53, TEXT_ANL,  M_A,    M_BIT8),
    E2(0x48, TEXT_ORL,  M_A,    M_R),
    E2(0x40, TEXT_ORL,  M_A,    M_IR),
    E2(0x43, TEXT_ORL,  M_A,    M_BIT8),
    E2(0xD8, TEXT_XRL,  M_A,    M_R),
    E2(0xD0, TEXT_XRL,  M_A,    M_IR),
    E2(0xD3, TEXT_XRL,  M_A,    M_BIT8),
    E1(0x17, TEXT_INC,  M_A),
    E1(0x07, TEXT_DEC,  M_A),
    E1(0x27, TEXT_CLR,  M_A),
    E1(0x37, TEXT_CPL,  M_A),
    E1(0x57, TEXT_DA,   M_A),
    E1(0x47, TEXT_SWAP, M_A),
    E1(0xE7, TEXT_RL,   M_A),
    E1(0xF7, TEXT_RLC,  M_A),
    E1(0x77, TEXT_RR,   M_A),
    E1(0x67, TEXT_RRC,  M_A),
    E2(0x08, TEXT_IN,   M_A,    M_P12),
    E2(0x38, TEXT_OUTL, M_P12,  M_A),
    E2(0x98, TEXT_ANL,  M_P12,  M_BIT8),
    E2(0x88, TEXT_ORL,  M_P12,  M_BIT8),
    E2(0x0C, TEXT_MOVD, M_A,    M_PEXT),
    E2(0x3C, TEXT_MOVD, M_PEXT, M_A),
    E2(0x9C, TEXT_ANLD, M_PEXT, M_A),
    E2(0x8C, TEXT_ORLD, M_PEXT, M_A),
    E1(0x18, TEXT_INC,  M_R),
    E1(0x10, TEXT_INC,  M_IR),
    E1(0xC8, TEXT_DEC,  M_R),
    E1(0x04, TEXT_JMP,  M_AD11),
    E1(0xB3, TEXT_JMPP, M_IA),
    E2(0xE8, TEXT_DJNZ, M_R,    M_AD08),
    E1(0xF6, TEXT_JC,   M_AD08),
    E1(0xE6, TEXT_JNC,  M_AD08),
    E1(0xC6, TEXT_JZ,   M_AD08),
    E1(0x96, TEXT_JNZ,  M_AD08),
    E1(0x36, TEXT_JT0,  M_AD08),
    E1(0x26, TEXT_JNT0, M_AD08),
    E1(0x56, TEXT_JT1,  M_AD08),
    E1(0x46, TEXT_JNT1, M_AD08),
    E1(0xB6, TEXT_JF0,  M_AD08),
    E1(0x76, TEXT_JF1,  M_AD08),
    E1(0x16, TEXT_JTF,  M_AD08),
    E1(0x86, TEXT_JNI,  M_AD08),
    E2(0x12, TEXT_JB,   M_BITN,  M_AD08),
    E1(0x14, TEXT_CALL, M_AD11),
    E0(0x83, TEXT_RET),
    E0(0x93, TEXT_RETR),
    E1(0x97, TEXT_CLR,  M_C),
    E1(0xA7, TEXT_CPL,  M_C),
    E1(0x85, TEXT_CLR,  M_F),
    E1(0x95, TEXT_CPL,  M_F),
    E2(0xF8, TEXT_MOV,  M_A,    M_R),
    E2(0xF0, TEXT_MOV,  M_A,    M_IR),
    E2(0x23, TEXT_MOV,  M_A,    M_IMM8),
    E2(0xA8, TEXT_MOV,  M_R,    M_A),
    E2(0xA0, TEXT_MOV,  M_IR,   M_A),
    E2(0xB8, TEXT_MOV,  M_R,    M_IMM8),
    E2(0xB0, TEXT_MOV,  M_IR,   M_IMM8),
    E2(0xC7, TEXT_MOV,  M_A,    M_PSW),
    E2(0xD7, TEXT_MOV,  M_PSW,  M_A),
    E2(0x28, TEXT_XCH,  M_A,    M_R),
    E2(0x20, TEXT_XCH,  M_A,    M_IR),
    E2(0x30, TEXT_XCHD, M_A,    M_IR),
    E2(0x90, TEXT_MOVX, M_IR,   M_A),
    E2(0x80, TEXT_MOVX, M_A,    M_IR),
    E2(0xA3, TEXT_MOVP, M_A,    M_IA),
    E2(0xE3, TEXT_MOVP3, M_A,   M_IA),
    E2(0x42, TEXT_MOV,  M_A,    M_T),
    E2(0x62, TEXT_MOV,  M_T,    M_A),
    E1(0x55, TEXT_STRT, M_T),
    E1(0x45, TEXT_STRT, M_CNT),
    E1(0x65, TEXT_STOP, M_TCNT),
    E1(0x25, TEXT_EN,   M_TCNTI),
    E1(0x35, TEXT_DIS,  M_TCNTI),
    E1(0x05, TEXT_EN,   M_I),
    E1(0x15, TEXT_DIS,  M_I),
    E1(0xC5, TEXT_SEL,  M_RB),
    E1(0xE5, TEXT_SEL,  M_MB),
    E1(0x75, TEXT_ENT0, M_CLK),
    E0(0x00, TEXT_NOP),
};

static constexpr uint8_t INDEX_I8039[] PROGMEM = {
      0,  // TEXT_ADD
      1,  // TEXT_ADD
      2,  // TEXT_ADD
      3,  // TEXT_ADDC
      4,  // TEXT_ADDC
      5,  // TEXT_ADDC
      6,  // TEXT_ANL
      7,  // TEXT_ANL
      8,  // TEXT_ANL
     27,  // TEXT_ANL
     31,  // TEXT_ANLD
     52,  // TEXT_CALL
     17,  // TEXT_CLR
     55,  // TEXT_CLR
     57,  // TEXT_CLR
     18,  // TEXT_CPL
     56,  // TEXT_CPL
     58,  // TEXT_CPL
     19,  // TEXT_DA
     16,  // TEXT_DEC
     35,  // TEXT_DEC
     81,  // TEXT_DIS
     83,  // TEXT_DIS
     38,  // TEXT_DJNZ
     80,  // TEXT_EN
     82,  // TEXT_EN
     86,  // TEXT_ENT0
     25,  // TEXT_IN
     15,  // TEXT_INC
     33,  // TEXT_INC
     34,  // TEXT_INC
     51,  // TEXT_JB
     39,  // TEXT_JC
     47,  // TEXT_JF0
     48,  // TEXT_JF1
     36,  // TEXT_JMP
     37,  // TEXT_JMPP
     40,  // TEXT_JNC
     50,  // TEXT_JNI
     44,  // TEXT_JNT0
     46,  // TEXT_JNT1
     42,  // TEXT_JNZ
     43,  // TEXT_JT0
     45,  // TEXT_JT1
     49,  // TEXT_JTF
     41,  // TEXT_JZ
     59,  // TEXT_MOV
     60,  // TEXT_MOV
     61,  // TEXT_MOV
     62,  // TEXT_MOV
     63,  // TEXT_MOV
     64,  // TEXT_MOV
     65,  // TEXT_MOV
     66,  // TEXT_MOV
     67,  // TEXT_MOV
     75,  // TEXT_MOV
     76,  // TEXT_MOV
     29,  // TEXT_MOVD
     30,  // TEXT_MOVD
     73,  // TEXT_MOVP
     74,  // TEXT_MOVP3
     71,  // TEXT_MOVX
     72,  // TEXT_MOVX
     87,  // TEXT_NOP
      9,  // TEXT_ORL
     10,  // TEXT_ORL
     11,  // TEXT_ORL
     28,  // TEXT_ORL
     32,  // TEXT_ORLD
     26,  // TEXT_OUTL
     53,  // TEXT_RET
     54,  // TEXT_RETR
     21,  // TEXT_RL
     22,  // TEXT_RLC
     23,  // TEXT_RR
     24,  // TEXT_RRC
     84,  // TEXT_SEL
     85,  // TEXT_SEL
     79,  // TEXT_STOP
     77,  // TEXT_STRT
     78,  // TEXT_STRT
     20,  // TEXT_SWAP
     68,  // TEXT_XCH
     69,  // TEXT_XCH
     70,  // TEXT_XCHD
     12,  // TEXT_XRL
     13,  // TEXT_XRL
     14,  // TEXT_XRL
};

static constexpr Entry TABLE_I8048[] PROGMEM = {
    E2(0x08, TEXT_INS,  M_A,    M_BUS),
    E2(0x02, TEXT_OUTL, M_BUS,  M_A),
    E2(0x98, TEXT_ANL,  M_BUS,  M_BIT8),
    E2(0x88, TEXT_ORL,  M_BUS,  M_BIT8),
};

static constexpr uint8_t INDEX_I8048[] PROGMEM = {
      2,  // TEXT_ANL
      0,  // TEXT_INS
      3,  // TEXT_ORL
      1,  // TEXT_OUTL
};

static constexpr Entry TABLE_I80C48[] PROGMEM = {
    E0(0x01, TEXT_HALT),
};

static constexpr uint8_t INDEX_I80C48[] PROGMEM = {
      0,  // TEXT_HALT
};

static constexpr Entry TABLE_MSM80C48[] PROGMEM = {
    E2(0x63, TEXT_MOV,   M_A,  M_P1),
    E2(0x73, TEXT_MOV,   M_A,  M_P2),
    E1(0xC0, TEXT_DEC,   M_IR),
    E2(0xE0, TEXT_DJNZ,  M_IR, M_AD08),
    E2(0xC3, TEXT_MOVP1, M_P,  M_IR3),
    E2(0xF3, TEXT_MOV,   M_P1, M_IR3),
    E0(0x82, TEXT_HLTS),
    E0(0xA2, TEXT_FLT),
    E0(0xC2, TEXT_FLTT),
    E0(0xE2, TEXT_FRES),
};

static constexpr uint8_t INDEX_MSM80C48[] PROGMEM = {
      2,  // TEXT_DEC
      3,  // TEXT_DJNZ
      7,  // TEXT_FLT
      8,  // TEXT_FLTT
      9,  // TEXT_FRES
      6,  // TEXT_HLTS
      0,  // TEXT_MOV
      1,  // TEXT_MOV
      5,  // TEXT_MOV
      4,  // TEXT_MOVP1
};
// clang-format on

using EntryPage = entry::TableBase<Entry>;

static constexpr EntryPage I8039_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_I8039), ARRAY_RANGE(INDEX_I8039)},
};

static constexpr EntryPage I8048_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_I8048), ARRAY_RANGE(INDEX_I8048)},
        {ARRAY_RANGE(TABLE_I8039), ARRAY_RANGE(INDEX_I8039)},
};

static constexpr EntryPage I80C39_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_I8039), ARRAY_RANGE(INDEX_I8039)},
        {ARRAY_RANGE(TABLE_I80C48), ARRAY_RANGE(INDEX_I80C48)},
};

static constexpr EntryPage I80C48_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_I8048), ARRAY_RANGE(INDEX_I8048)},
        {ARRAY_RANGE(TABLE_I8039), ARRAY_RANGE(INDEX_I8039)},
        {ARRAY_RANGE(TABLE_I80C48), ARRAY_RANGE(INDEX_I80C48)},
};

static constexpr EntryPage MSM80C39_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_I8039), ARRAY_RANGE(INDEX_I8039)},
        {ARRAY_RANGE(TABLE_I80C48), ARRAY_RANGE(INDEX_I80C48)},
        {ARRAY_RANGE(TABLE_MSM80C48), ARRAY_RANGE(INDEX_MSM80C48)},
};

static constexpr EntryPage MSM80C48_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_I8048), ARRAY_RANGE(INDEX_I8048)},
        {ARRAY_RANGE(TABLE_I8039), ARRAY_RANGE(INDEX_I8039)},
        {ARRAY_RANGE(TABLE_I80C48), ARRAY_RANGE(INDEX_I80C48)},
        {ARRAY_RANGE(TABLE_MSM80C48), ARRAY_RANGE(INDEX_MSM80C48)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

static constexpr Cpu CPU_TABLE[] PROGMEM = {
        {I8039, TEXT_CPU_8039, ARRAY_RANGE(I8039_PAGES)},
        {I8048, TEXT_CPU_8048, ARRAY_RANGE(I8048_PAGES)},
        {I80C39, TEXT_CPU_80C39, ARRAY_RANGE(I80C39_PAGES)},
        {I80C48, TEXT_CPU_80C48, ARRAY_RANGE(I80C48_PAGES)},
        {MSM80C39, TEXT_CPU_MSM80C39, ARRAY_RANGE(MSM80C39_PAGES)},
        {MSM80C48, TEXT_CPU_MSM80C48, ARRAY_RANGE(MSM80C48_PAGES)},
};

static const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_IMM8)
        return table == M_BIT8;
    if (opr == M_AD11)
        return table == M_AD08 || table == M_BITN;
    if (opr == M_IR3)
        return table == M_IR;
    if (opr == M_P1 || opr == M_P2)
        return table == M_P12;
    return false;
}

static bool acceptModes(AsmInsn &insn, const Entry *entry) {
    auto flags = insn.flags();
    auto table = entry->flags();
    return acceptMode(flags.dst(), table.dst()) && acceptMode(flags.src(), table.src());
}

Error TableI8048::searchName(CpuType cpuType, AsmInsn &insn) const {
    cpu(cpuType)->searchName(insn, acceptModes);
    return insn.getError();
}

static bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *page) {
    UNUSED(page);
    auto opCode = insn.opCode();
    const auto flags = entry->flags();
    const auto dst = flags.dst();
    const auto src = flags.src();
    if (dst == M_R || src == M_R) {
        opCode &= ~7;
    } else if (dst == M_IR || src == M_IR) {
        opCode &= ~1;
    } else if (dst == M_P12 || src == M_P12 || dst == M_PEXT || src == M_PEXT) {
        opCode &= ~3;
    } else if (dst == M_AD11 || dst == M_BITN) {
        opCode &= ~0xE0;
    } else if (dst == M_F) {
        opCode &= ~0x20;
    } else if (dst == M_RB || dst == M_MB) {
        opCode &= ~0x10;
    }
    return opCode == entry->opCode();
}

Error TableI8048::searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) const {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

const /*PROGMEM*/ char *TableI8048::listCpu_P() const {
    return TEXT_I8048_LIST;
}

const /*PROGMEM*/ char *TableI8048::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableI8048::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    auto p = name;
    p.iexpect('i');
    const auto t = Cpu::search(p, ARRAY_RANGE(CPU_TABLE));
    if (t) {
        cpuType = t->cpuType();
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TableI8048 TABLE;

}  // namespace i8048
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
