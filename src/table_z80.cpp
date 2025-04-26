/*
 * Copyright 2020 Tadashi G. Takaoka
 *
t * Licensed under the Apache License, Version 2.0 (the "License");
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

#include "table_z80.h"
#include "entry_table.h"
#include "entry_z80.h"
#include "text_z80.h"

using namespace libasm::text::z80;

namespace libasm {
namespace z80 {

#define E2(_opc, _name, _dst, _src) {_opc, Entry::Flags::create(_dst, _src), _name}
#define E1(_opc, _name, _dst) E2(_opc, _name, _dst, M_NONE)
#define E0(_opc, _name) E1(_opc, _name, M_NONE)

// clang-format off
constexpr Entry TABLE_Z80[] PROGMEM = {
    E0(0x00, TEXT_NOP),
    E2(0x01, TEXT_LD,   M_R16, M_IM16),
    E2(0x09, TEXT_ADD,  R_HL,  M_R16),
    E2(0x02, TEXT_LD,   I_PTR, R_A),
    E2(0x0A, TEXT_LD,   R_A,   I_PTR),
    E2(0x22, TEXT_LD,   M_ABS, R_HL),
    E2(0x2A, TEXT_LD,   R_HL,  M_ABS),
    E2(0x32, TEXT_LD,   M_ABS, R_A),
    E2(0x3A, TEXT_LD,   R_A,   M_ABS),
    E1(0x03, TEXT_INC,  M_R16),
    E1(0x0B, TEXT_DEC,  M_R16),
    E1(0x04, TEXT_INC,  M_DST),
    E1(0x05, TEXT_DEC,  M_DST),
    E2(0x06, TEXT_LD,   M_DST, M_IM8),
    E0(0x07, TEXT_RLCA),
    E0(0x0F, TEXT_RRCA),
    E0(0x17, TEXT_RLA),
    E0(0x1F, TEXT_RRA),
    E0(0x27, TEXT_DAA),
    E0(0x2F, TEXT_CPL),
    E0(0x37, TEXT_SCF),
    E0(0x3F, TEXT_CCF),
    E0(0x76, TEXT_HALT),
    E2(0x40, TEXT_LD,   M_DST, M_SR8),
    E2(0x40, TEXT_LD,   M_DR8, M_SRC),
    E2(0x80, TEXT_ADD,  R_A,   M_SRC),
    E2(0x88, TEXT_ADC,  R_A,   M_SRC),
    E1(0x90, TEXT_SUB,  M_SRC),
    E2(0x90, TEXT_SUB,  R_A,   M_SRC),
    E2(0x98, TEXT_SBC,  R_A,   M_SRC),
    E1(0xA0, TEXT_AND,  M_SRC),
    E2(0xA0, TEXT_AND,  R_A,   M_SRC),
    E1(0xA8, TEXT_XOR,  M_SRC),
    E2(0xA8, TEXT_XOR,  R_A,   M_SRC),
    E1(0xB0, TEXT_OR,   M_SRC),
    E2(0xB0, TEXT_OR,   R_A,   M_SRC),
    E1(0xB8, TEXT_CP,   M_SRC),
    E2(0xB8, TEXT_CP,   R_A,   M_SRC),
    E1(0xC0, TEXT_RET,  M_CC8),
    E1(0xC1, TEXT_POP,  M_STK),
    E0(0xC9, TEXT_RET),
    E2(0xF9, TEXT_LD,   R_SP,  R_HL),
    E2(0xC2, TEXT_JP,   M_CC8, M_IM16),
    E1(0xC3, TEXT_JP,   M_IM16),
    E1(0xE9, TEXT_JP,   I_HL),
    E2(0xD3, TEXT_OUT,  M_IOA, R_A),
    E2(0xDB, TEXT_IN,   R_A,   M_IOA),
    E2(0xE3, TEXT_EX,   I_SP,  R_HL),
    E2(0xE3, TEXT_EX,   R_HL,  I_SP),
    E2(0xEB, TEXT_EX,   R_DE,  R_HL),
    E2(0xEB, TEXT_EX,   R_HL,  R_DE),
    E0(0xF3, TEXT_DI),
    E0(0xFB, TEXT_EI),
    E2(0xC4, TEXT_CALL, M_CC8, M_IM16),
    E1(0xC5, TEXT_PUSH, M_STK),
    E1(0xCD, TEXT_CALL, M_IM16),
    E2(0xC6, TEXT_ADD,  R_A,   M_IM8),
    E2(0xCE, TEXT_ADC,  R_A,   M_IM8),
    E1(0xD6, TEXT_SUB,  M_IM8),
    E2(0xD6, TEXT_SUB,  R_A,   M_IM8),
    E2(0xDE, TEXT_SBC,  R_A,   M_IM8),
    E1(0xE6, TEXT_AND,  M_IM8),
    E2(0xE6, TEXT_AND,  R_A,   M_IM8),
    E1(0xEE, TEXT_XOR,  M_IM8),
    E2(0xEE, TEXT_XOR,  R_A,   M_IM8),
    E1(0xF6, TEXT_OR,   M_IM8),
    E2(0xF6, TEXT_OR,   R_A,   M_IM8),
    E1(0xFE, TEXT_CP,   M_IM8),
    E2(0xFE, TEXT_CP,   R_A,   M_IM8),
    E1(0xC7, TEXT_RST,  M_VEC),
    E2(0x08, TEXT_EX,   R_AF,  R_AFP),
    E1(0x10, TEXT_DJNZ, M_REL),
    E2(0x20, TEXT_JR,   M_CC4, M_REL),
    E1(0x18, TEXT_JR,   M_REL),
    E0(0xD9, TEXT_EXX),
};

constexpr uint8_t INDEX_Z80[] PROGMEM = {
     26,  // TEXT_ADC
     57,  // TEXT_ADC
      2,  // TEXT_ADD
     25,  // TEXT_ADD
     56,  // TEXT_ADD
     30,  // TEXT_AND
     31,  // TEXT_AND
     61,  // TEXT_AND
     62,  // TEXT_AND
     53,  // TEXT_CALL
     55,  // TEXT_CALL
     21,  // TEXT_CCF
     36,  // TEXT_CP
     37,  // TEXT_CP
     67,  // TEXT_CP
     68,  // TEXT_CP
     19,  // TEXT_CPL
     18,  // TEXT_DAA
     10,  // TEXT_DEC
     12,  // TEXT_DEC
     51,  // TEXT_DI
     71,  // TEXT_DJNZ
     52,  // TEXT_EI
     47,  // TEXT_EX
     48,  // TEXT_EX
     49,  // TEXT_EX
     50,  // TEXT_EX
     70,  // TEXT_EX
     74,  // TEXT_EXX
     22,  // TEXT_HALT
     46,  // TEXT_IN
      9,  // TEXT_INC
     11,  // TEXT_INC
     42,  // TEXT_JP
     43,  // TEXT_JP
     44,  // TEXT_JP
     72,  // TEXT_JR
     73,  // TEXT_JR
      1,  // TEXT_LD
      3,  // TEXT_LD
      4,  // TEXT_LD
      5,  // TEXT_LD
      6,  // TEXT_LD
      7,  // TEXT_LD
      8,  // TEXT_LD
     13,  // TEXT_LD
     23,  // TEXT_LD
     24,  // TEXT_LD
     41,  // TEXT_LD
      0,  // TEXT_NOP
     34,  // TEXT_OR
     35,  // TEXT_OR
     65,  // TEXT_OR
     66,  // TEXT_OR
     45,  // TEXT_OUT
     39,  // TEXT_POP
     54,  // TEXT_PUSH
     38,  // TEXT_RET
     40,  // TEXT_RET
     16,  // TEXT_RLA
     14,  // TEXT_RLCA
     17,  // TEXT_RRA
     15,  // TEXT_RRCA
     69,  // TEXT_RST
     29,  // TEXT_SBC
     60,  // TEXT_SBC
     20,  // TEXT_SCF
     27,  // TEXT_SUB
     28,  // TEXT_SUB
     58,  // TEXT_SUB
     59,  // TEXT_SUB
     32,  // TEXT_XOR
     33,  // TEXT_XOR
     63,  // TEXT_XOR
     64,  // TEXT_XOR
};

// CB:xx
constexpr Entry TABLE_BIT[] PROGMEM = {
    E1(0x00, TEXT_RLC, M_SRC),
    E1(0x08, TEXT_RRC, M_SRC),
    E1(0x10, TEXT_RL,  M_SRC),
    E1(0x18, TEXT_RR,  M_SRC),
    E1(0x20, TEXT_SLA, M_SRC),
    E1(0x28, TEXT_SRA, M_SRC),
    E1(0x38, TEXT_SRL, M_SRC),
    E2(0x40, TEXT_BIT, M_BIT, M_SRC),
    E2(0x80, TEXT_RES, M_BIT, M_SRC),
    E2(0xC0, TEXT_SET, M_BIT, M_SRC),
};

constexpr uint8_t INDEX_BIT[] PROGMEM = {
      7,  // TEXT_BIT
      8,  // TEXT_RES
      2,  // TEXT_RL
      0,  // TEXT_RLC
      3,  // TEXT_RR
      1,  // TEXT_RRC
      9,  // TEXT_SET
      4,  // TEXT_SLA
      5,  // TEXT_SRA
      6,  // TEXT_SRL
};

// ED:xx
constexpr Entry TABLE_EXT[] PROGMEM = {
    E2(0x40, TEXT_IN,   M_DR8, I_C),
    E2(0x41, TEXT_OUT,  I_C,    M_DR8),
    E2(0x42, TEXT_SBC,  R_HL,   M_R16),
    E2(0x4A, TEXT_ADC,  R_HL,   M_R16),
    E2(0x43, TEXT_LD,   M_ABS,  M_NOHL),
    E2(0x4B, TEXT_LD,   M_NOHL, M_ABS),
    E0(0x44, TEXT_NEG),
    E0(0x45, TEXT_RETN),
    E0(0x4D, TEXT_RETI),
    E1(0x46, TEXT_IM,   M_IMMD),
    E2(0x47, TEXT_LD,   R_I,    R_A),
    E2(0x4F, TEXT_LD,   R_R,    R_A),
    E2(0x57, TEXT_LD,   R_A,    R_I),
    E2(0x5F, TEXT_LD,   R_A,    R_R),
    E0(0x67, TEXT_RRD),
    E0(0x6F, TEXT_RLD),
    E0(0xA0, TEXT_LDI),
    E0(0xA8, TEXT_LDD),
    E0(0xB0, TEXT_LDIR),
    E0(0xB8, TEXT_LDDR),
    E0(0xA1, TEXT_CPI),
    E0(0xA9, TEXT_CPD),
    E0(0xB1, TEXT_CPIR),
    E0(0xB9, TEXT_CPDR),
    E0(0xA2, TEXT_INI),
    E0(0xAA, TEXT_IND),
    E0(0xB2, TEXT_INIR),
    E0(0xBA, TEXT_INDR),
    E0(0xA3, TEXT_OUTI),
    E0(0xAB, TEXT_OUTD),
    E0(0xB3, TEXT_OTIR),
    E0(0xBB, TEXT_OTDR),
};

constexpr uint8_t INDEX_EXT[] PROGMEM = {
      3,  // TEXT_ADC
     21,  // TEXT_CPD
     23,  // TEXT_CPDR
     20,  // TEXT_CPI
     22,  // TEXT_CPIR
      9,  // TEXT_IM
      0,  // TEXT_IN
     25,  // TEXT_IND
     27,  // TEXT_INDR
     24,  // TEXT_INI
     26,  // TEXT_INIR
      4,  // TEXT_LD
      5,  // TEXT_LD
     10,  // TEXT_LD
     11,  // TEXT_LD
     12,  // TEXT_LD
     13,  // TEXT_LD
     17,  // TEXT_LDD
     19,  // TEXT_LDDR
     16,  // TEXT_LDI
     18,  // TEXT_LDIR
      6,  // TEXT_NEG
     31,  // TEXT_OTDR
     30,  // TEXT_OTIR
      1,  // TEXT_OUT
     29,  // TEXT_OUTD
     28,  // TEXT_OUTI
      8,  // TEXT_RETI
      7,  // TEXT_RETN
     15,  // TEXT_RLD
     14,  // TEXT_RRD
      2,  // TEXT_SBC
};

// Table for DD:xx, FD:xx
constexpr Entry TABLE_IDX[] PROGMEM = {
    E2(0x09, TEXT_ADD,  R_IDX, M_R16X),
    E2(0x21, TEXT_LD,   R_IDX, M_IM16),
    E2(0x22, TEXT_LD,   M_ABS, R_IDX),
    E2(0x2A, TEXT_LD,   R_IDX, M_ABS),
    E1(0x23, TEXT_INC,  R_IDX),
    E1(0x2B, TEXT_DEC,  R_IDX),
    E1(0x34, TEXT_INC,  M_IDX),
    E1(0x35, TEXT_DEC,  M_IDX),
    E2(0x36, TEXT_LD,   M_IDX, M_IM8),
    E2(0x46, TEXT_LD,   M_DR8, M_IDX),
    E2(0x70, TEXT_LD,   M_IDX, M_SR8),
    E2(0x86, TEXT_ADD,  R_A,   M_IDX),
    E2(0x8E, TEXT_ADC,  R_A,   M_IDX),
    E1(0x96, TEXT_SUB,  M_IDX),
    E2(0x96, TEXT_SUB,  R_A,   M_IDX),
    E2(0x9E, TEXT_SBC,  R_A,   M_IDX),
    E1(0xA6, TEXT_AND,  M_IDX),
    E2(0xA6, TEXT_AND,  R_A,   M_IDX),
    E1(0xAE, TEXT_XOR,  M_IDX),
    E2(0xAE, TEXT_XOR,  R_A,   M_IDX),
    E1(0xB6, TEXT_OR,   M_IDX),
    E2(0xB6, TEXT_OR,   R_A,   M_IDX),
    E1(0xBE, TEXT_CP,   M_IDX),
    E2(0xBE, TEXT_CP,   R_A,   M_IDX),
    E1(0xE1, TEXT_POP,  R_IDX),
    E1(0xE9, TEXT_JP,   I_IDX),
    E2(0xF9, TEXT_LD,   R_SP,  R_IDX),
    E2(0xE3, TEXT_EX,   I_SP,  R_IDX),
    E2(0xE3, TEXT_EX,   R_IDX, I_SP),
    E1(0xE5, TEXT_PUSH, R_IDX),
};

constexpr uint8_t INDEX_IDX[] PROGMEM = {
     12,  // TEXT_ADC
      0,  // TEXT_ADD
     11,  // TEXT_ADD
     16,  // TEXT_AND
     17,  // TEXT_AND
     22,  // TEXT_CP
     23,  // TEXT_CP
      5,  // TEXT_DEC
      7,  // TEXT_DEC
     27,  // TEXT_EX
     28,  // TEXT_EX
      4,  // TEXT_INC
      6,  // TEXT_INC
     25,  // TEXT_JP
      1,  // TEXT_LD
      2,  // TEXT_LD
      3,  // TEXT_LD
      8,  // TEXT_LD
      9,  // TEXT_LD
     10,  // TEXT_LD
     26,  // TEXT_LD
     20,  // TEXT_OR
     21,  // TEXT_OR
     24,  // TEXT_POP
     29,  // TEXT_PUSH
     15,  // TEXT_SBC
     13,  // TEXT_SUB
     14,  // TEXT_SUB
     18,  // TEXT_XOR
     19,  // TEXT_XOR
};

// DD:CB:dd:xx, FD:CB:dd:xx
constexpr Entry TABLE_IDXBIT[] PROGMEM = {
    E1(0x06, TEXT_RLC, M_IDX),
    E1(0x0E, TEXT_RRC, M_IDX),
    E1(0x16, TEXT_RL,  M_IDX),
    E1(0x1E, TEXT_RR,  M_IDX),
    E1(0x26, TEXT_SLA, M_IDX),
    E1(0x2E, TEXT_SRA, M_IDX),
    E1(0x3E, TEXT_SRL, M_IDX),
    E2(0x46, TEXT_BIT, M_BIT, M_IDX),
    E2(0x86, TEXT_RES, M_BIT, M_IDX),
    E2(0xC6, TEXT_SET, M_BIT, M_IDX),
};

constexpr uint8_t INDEX_IDXBIT[] PROGMEM = {
      7,  // TEXT_BIT
      8,  // TEXT_RES
      2,  // TEXT_RL
      0,  // TEXT_RLC
      3,  // TEXT_RR
      1,  // TEXT_RRC
      9,  // TEXT_SET
      4,  // TEXT_SLA
      5,  // TEXT_SRA
      6,  // TEXT_SRL
};

// Z180 ED:xx
constexpr Entry TABLE_Z180_EXT[] PROGMEM = {
    E1(0x4C, TEXT_MLT,    M_R16),
    E1(0x04, TEXT_TST,    M_DST),
    E1(0x64, TEXT_TST,    M_IM8),
    E1(0x74, TEXT_TSTIO,  M_IM8),
    E2(0x00, TEXT_IN0,    M_DR8, M_IOA),
    E2(0x01, TEXT_OUT0,   M_IOA, M_DR8),
    E0(0x83, TEXT_OTIM),
    E0(0x93, TEXT_OTIMR),
    E0(0x8B, TEXT_OTDM),
    E0(0x9B, TEXT_OTDMR),
    E0(0x76, TEXT_SLP),
};

constexpr uint8_t INDEX_Z180_EXT[] PROGMEM = {
      4,  // TEXT_IN0
      0,  // TEXT_MLT
      8,  // TEXT_OTDM
      9,  // TEXT_OTDMR
      6,  // TEXT_OTIM
      7,  // TEXT_OTIMR
      5,  // TEXT_OUT0
     10,  // TEXT_SLP
      1,  // TEXT_TST
      2,  // TEXT_TST
      3,  // TEXT_TSTIO
};

// clang-format on

using EntryPage = entry::PrefixTableBase<Entry>;

constexpr EntryPage Z80_PAGES[] PROGMEM = {
        {0x0000, ARRAY_RANGE(TABLE_Z80), ARRAY_RANGE(INDEX_Z80)},
        {TableZ80::EXT, ARRAY_RANGE(TABLE_EXT), ARRAY_RANGE(INDEX_EXT)},
        {TableZ80::BIT, ARRAY_RANGE(TABLE_BIT), ARRAY_RANGE(INDEX_BIT)},
        {TableZ80::IX, ARRAY_RANGE(TABLE_IDX), ARRAY_RANGE(INDEX_IDX)},
        {TableZ80::IY, ARRAY_RANGE(TABLE_IDX), ARRAY_RANGE(INDEX_IDX)},
        {TableZ80::IXBIT, ARRAY_RANGE(TABLE_IDXBIT), ARRAY_RANGE(INDEX_IDXBIT)},
        {TableZ80::IYBIT, ARRAY_RANGE(TABLE_IDXBIT), ARRAY_RANGE(INDEX_IDXBIT)},
};

constexpr EntryPage Z180_PAGES[] PROGMEM = {
        {0x0000, ARRAY_RANGE(TABLE_Z80), ARRAY_RANGE(INDEX_Z80)},
        {TableZ80::EXT, ARRAY_RANGE(TABLE_EXT), ARRAY_RANGE(INDEX_EXT)},
        {TableZ80::EXT, ARRAY_RANGE(TABLE_Z180_EXT), ARRAY_RANGE(INDEX_Z180_EXT)},
        {TableZ80::BIT, ARRAY_RANGE(TABLE_BIT), ARRAY_RANGE(INDEX_BIT)},
        {TableZ80::IX, ARRAY_RANGE(TABLE_IDX), ARRAY_RANGE(INDEX_IDX)},
        {TableZ80::IY, ARRAY_RANGE(TABLE_IDX), ARRAY_RANGE(INDEX_IDX)},
        {TableZ80::IXBIT, ARRAY_RANGE(TABLE_IDXBIT), ARRAY_RANGE(INDEX_IDXBIT)},
        {TableZ80::IYBIT, ARRAY_RANGE(TABLE_IDXBIT), ARRAY_RANGE(INDEX_IDXBIT)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {Z80,  TEXT_CPU_Z80,  ARRAY_RANGE(Z80_PAGES)},
        {Z180, TEXT_CPU_Z180, ARRAY_RANGE(Z180_PAGES)},
};

const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_SR8 || opr == R_A)
        return table == M_DR8 || table == M_SRC || table == M_DST || table == M_SR8;
    if (opr == I_HL)
        return table == M_SRC || table == M_DST;
    if (opr == R_C)
        return table == M_SRC || table == M_DST || table == M_SR8 || table == M_DR8 ||
               table == M_CC4 || table == M_CC8;
    if (opr == R_BC || opr == R_DE)
        return table == M_R16 || table == M_NOHL || table == M_R16X || table == M_STK;
    if (opr == R_HL)
        return table == M_R16 || table == M_STK;
    if (opr == R_SP)
        return table == M_R16 || table == M_NOHL || table == M_R16X;
    if (opr == R_AF)
        return table == M_STK;
    if (opr == R_IDX)
        return table == M_R16X;
    if (opr == M_IM16)
        return table == M_IM8 || table == M_REL || table == M_BIT || table == M_VEC ||
               table == M_IMMD;
    if (opr == M_ABS)
        return table == M_IOA;
    if (opr == M_CC4)
        return table == M_CC8;
    return false;
}

bool acceptModes(AsmInsn &insn, const Entry *entry) {
    const auto table = entry->readFlags();
    return acceptMode(insn.dstOp.mode, table.dst()) && acceptMode(insn.srcOp.mode, table.src());
}

Error searchName(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, acceptModes);
    return insn.getError();
}

bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *) {
    auto opc = insn.opCode();
    const auto flags = entry->readFlags();
    const auto dst = flags.dst();
    const auto src = flags.src();

    if (dst == M_DR8 || src == M_DR8) {
        if (((opc >> 3) & 7) == 6)
            return false;
        opc &= ~(7 << 3);
    }
    if (dst == M_DST || src == M_DST)
        opc &= ~(7 << 3);
    if (src == M_SR8) {
        if ((opc & 7) == 6)
            return false;
        opc &= ~7;
    }
    if (dst == M_SRC || src == M_SRC)
        opc &= ~7;

    if (dst == M_CC8 || dst == M_VEC || dst == M_BIT) {
        opc &= ~(7 << 3);
    } else if (dst == M_R16 || src == M_R16 || dst == M_R16X || src == M_R16X || dst == M_STK ||
               src == M_STK) {
        opc &= ~(3 << 4);
    } else if (dst == M_NOHL || src == M_NOHL) {
        if (((opc >> 4) & 3) == 2)
            return false;
        opc &= ~(3 << 4);
    } else if (dst == I_PTR || src == I_PTR) {
        opc &= ~(1 << 4);
    } else if (dst == M_CC4) {
        opc &= ~(3 << 3);
    } else if (dst == M_IMMD) {
        if (((opc >> 3) & 3) == 1)
            return false;  // IM x
        opc &= ~(3 << 3);
    }
    return opc == entry->readOpCode();
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

bool isPrefix(CpuType cpuType, uint16_t code) {
    return cpu(cpuType)->isPrefix(code);
}

bool EntryInsn::ixBit() const {
    return (prefix() == TableZ80::IXBIT || prefix() == TableZ80::IYBIT);
}

const /*PROGMEM*/ char *TableZ80::listCpu_P() const {
    return TEXT_CPU_LIST;
}

const /*PROGMEM*/ char *TableZ80::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableZ80::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    auto t = Cpu::search(name, ARRAY_RANGE(CPU_TABLE));
    if (t) {
        cpuType = t->readCpuType();
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TableZ80 TABLE;

}  // namespace z80
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
