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

#include "table_z80.h"
#include "config_z80.h"
#include "entry_z80.h"
#include "str_scanner.h"
#include "text_z80.h"

#include <ctype.h>
#include <string.h>

using namespace libasm::text::z80;

namespace libasm {
namespace z80 {

#define E2(_opc, _name, _dst, _src) \
    { _opc, Entry::Flags::create(_dst, _src), _name }
#define E1(_opc, _name, _dst) E2(_opc, _name, _dst, M_NONE)
#define E0(_opc, _name) E1(_opc, _name, M_NONE)
#define I2(_opc, _name, _dst, _src) \
    { _opc, Entry::Flags::ixbit(_dst, _src), _name }
#define I1(_opc, _name, _dst) I2(_opc, _name, _dst, M_NONE)
#define U2(_opc, _name, _dst, _src) \
    { _opc, Entry::Flags::undef(_dst, _src), _name }

// clang-format off
static constexpr Entry TABLE_I8080[] PROGMEM = {
    E0(0x00, TEXT_NOP),
    E2(0x01, TEXT_LD,   M_PTR,  M_IM16),
    E2(0x09, TEXT_ADD,  R_HL,   M_PTR),
    E2(0x02, TEXT_LD,   I_BCDE, R_A),
    E2(0x0A, TEXT_LD,   R_A,    I_BCDE),
    E2(0x22, TEXT_LD,   M_ABS,  R_HL),
    E2(0x2A, TEXT_LD,   R_HL,   M_ABS),
    E2(0x32, TEXT_LD,   M_ABS,  R_A),
    E2(0x3A, TEXT_LD,   R_A,    M_ABS),
    E1(0x03, TEXT_INC,  M_PTR),
    E1(0x0B, TEXT_DEC,  M_PTR),
    E1(0x04, TEXT_INC,  M_DST),
    E1(0x05, TEXT_DEC,  M_DST),
    E2(0x06, TEXT_LD,   M_DST,  M_IM8),
    E0(0x07, TEXT_RLCA),
    E0(0x0F, TEXT_RRCA),
    E0(0x17, TEXT_RLA),
    E0(0x1F, TEXT_RRA),
    E0(0x27, TEXT_DAA),
    E0(0x2F, TEXT_CPL),
    E0(0x37, TEXT_SCF),
    E0(0x3F, TEXT_CCF),
    E0(0x76, TEXT_HALT),
    E2(0x40, TEXT_LD,   M_DST,  M_REG),
    E2(0x80, TEXT_ADD,  R_A,    M_REG),
    E2(0x88, TEXT_ADC,  R_A,    M_REG),
    E2(0x90, TEXT_SUB,  R_A,    M_REG),
    E1(0x90, TEXT_SUB,  M_REG),
    E2(0x98, TEXT_SBC,  R_A,    M_REG),
    E2(0xA0, TEXT_AND,  R_A,    M_REG),
    E1(0xA0, TEXT_AND,  M_REG),
    E2(0xA8, TEXT_XOR,  R_A,    M_REG),
    E1(0xA8, TEXT_XOR,  M_REG),
    E2(0xB0, TEXT_OR,   R_A,    M_REG),
    E1(0xB0, TEXT_OR,   M_REG),
    E2(0xB8, TEXT_CP,   R_A,    M_REG),
    E1(0xB8, TEXT_CP,   M_REG),
    E1(0xC0, TEXT_RET,  M_CC8),
    E1(0xC1, TEXT_POP,  M_STK),
    E0(0xC9, TEXT_RET),
    E2(0xF9, TEXT_LD,   R_SP,   R_HL),
    E2(0xC2, TEXT_JP,   M_CC8,  M_IM16),
    E1(0xC3, TEXT_JP,   M_IM16),
    E1(0xE9, TEXT_JP,   I_HL),
    E2(0xD3, TEXT_OUT,  M_IOA,  R_A),
    E2(0xDB, TEXT_IN,   R_A,    M_IOA),
    E2(0xE3, TEXT_EX,   I_SP,   R_HL),
    E2(0xE3, TEXT_EX,   R_HL,   I_SP),
    E2(0xEB, TEXT_EX,   R_DE,   R_HL),
    E2(0xEB, TEXT_EX,   R_HL,   R_DE),
    E0(0xF3, TEXT_DI),
    E0(0xFB, TEXT_EI),
    E2(0xC4, TEXT_CALL, M_CC8,  M_IM16),
    E1(0xC5, TEXT_PUSH, M_STK),
    E1(0xCD, TEXT_CALL, M_IM16),
    E2(0xC6, TEXT_ADD,  R_A,    M_IM8),
    E2(0xCE, TEXT_ADC,  R_A,    M_IM8),
    E2(0xD6, TEXT_SUB,  R_A,    M_IM8),
    E1(0xD6, TEXT_SUB,  M_IM8),
    E2(0xDE, TEXT_SBC,  R_A,    M_IM8),
    E2(0xE6, TEXT_AND,  R_A,    M_IM8),
    E1(0xE6, TEXT_AND,  M_IM8),
    E2(0xEE, TEXT_XOR,  R_A,    M_IM8),
    E1(0xEE, TEXT_XOR,  M_IM8),
    E2(0xF6, TEXT_OR,   R_A,    M_IM8),
    E1(0xF6, TEXT_OR,   M_IM8),
    E2(0xFE, TEXT_CP,   R_A,    M_IM8),
    E1(0xFE, TEXT_CP,   M_IM8),
    E1(0xC7, TEXT_RST,  M_VEC),
};

static constexpr uint8_t INDEX_I8080[] PROGMEM = {
     25,  // TEXT_ADC
     56,  // TEXT_ADC
      2,  // TEXT_ADD
     24,  // TEXT_ADD
     55,  // TEXT_ADD
     29,  // TEXT_AND
     30,  // TEXT_AND
     60,  // TEXT_AND
     61,  // TEXT_AND
     52,  // TEXT_CALL
     54,  // TEXT_CALL
     21,  // TEXT_CCF
     35,  // TEXT_CP
     36,  // TEXT_CP
     66,  // TEXT_CP
     67,  // TEXT_CP
     19,  // TEXT_CPL
     18,  // TEXT_DAA
     10,  // TEXT_DEC
     12,  // TEXT_DEC
     50,  // TEXT_DI
     51,  // TEXT_EI
     46,  // TEXT_EX
     47,  // TEXT_EX
     48,  // TEXT_EX
     49,  // TEXT_EX
     22,  // TEXT_HALT
     45,  // TEXT_IN
      9,  // TEXT_INC
     11,  // TEXT_INC
     41,  // TEXT_JP
     42,  // TEXT_JP
     43,  // TEXT_JP
      1,  // TEXT_LD
      3,  // TEXT_LD
      4,  // TEXT_LD
      5,  // TEXT_LD
      6,  // TEXT_LD
      7,  // TEXT_LD
      8,  // TEXT_LD
     13,  // TEXT_LD
     23,  // TEXT_LD
     40,  // TEXT_LD
      0,  // TEXT_NOP
     33,  // TEXT_OR
     34,  // TEXT_OR
     64,  // TEXT_OR
     65,  // TEXT_OR
     44,  // TEXT_OUT
     38,  // TEXT_POP
     53,  // TEXT_PUSH
     37,  // TEXT_RET
     39,  // TEXT_RET
     16,  // TEXT_RLA
     14,  // TEXT_RLCA
     17,  // TEXT_RRA
     15,  // TEXT_RRCA
     68,  // TEXT_RST
     28,  // TEXT_SBC
     59,  // TEXT_SBC
     20,  // TEXT_SCF
     26,  // TEXT_SUB
     27,  // TEXT_SUB
     57,  // TEXT_SUB
     58,  // TEXT_SUB
     31,  // TEXT_XOR
     32,  // TEXT_XOR
     62,  // TEXT_XOR
     63,  // TEXT_XOR
};

static constexpr Entry TABLE_I8085[] PROGMEM = {
    E2(0x20, TEXT_LD,   R_A,    R_IM),
    E2(0x30, TEXT_LD,   R_IM,   R_A),
};
static constexpr uint8_t INDEX_I8085[] PROGMEM = {
      0,  // TEXT_LD
      1,  // TEXT_LD
};

static constexpr Entry TABLE_Z80[] PROGMEM = {
    E2(0x08, TEXT_EX,   R_AF,   R_AFP),
    E1(0x10, TEXT_DJNZ, M_REL),
    E2(0x20, TEXT_JR,   M_CC4,  M_REL),
    E1(0x18, TEXT_JR,   M_REL),
    E0(0xD9, TEXT_EXX),
};
static constexpr uint8_t INDEX_Z80[] PROGMEM = {
      1,  // TEXT_DJNZ
      0,  // TEXT_EX
      4,  // TEXT_EXX
      2,  // TEXT_JR
      3,  // TEXT_JR
};

static constexpr Entry TABLE_CB[] PROGMEM = {
    E1(0x00, TEXT_RLC,  M_REG),
    E1(0x08, TEXT_RRC,  M_REG),
    E1(0x10, TEXT_RL,   M_REG),
    E1(0x18, TEXT_RR,   M_REG),
    E1(0x20, TEXT_SLA,  M_REG),
    E1(0x28, TEXT_SRA,  M_REG),
    E1(0x38, TEXT_SRL,  M_REG),
    E2(0x40, TEXT_BIT,  M_BIT,  M_REG),
    E2(0x80, TEXT_RES,  M_BIT,  M_REG),
    E2(0xC0, TEXT_SET,  M_BIT,  M_REG),
    I1(0x06, TEXT_RLC,  M_INDX),
    I1(0x0E, TEXT_RRC,  M_INDX),
    I1(0x16, TEXT_RL,   M_INDX),
    I1(0x1E, TEXT_RR,   M_INDX),
    I1(0x26, TEXT_SLA,  M_INDX),
    I1(0x2E, TEXT_SRA,  M_INDX),
    I1(0x3E, TEXT_SRL,  M_INDX),
    I2(0x46, TEXT_BIT,  M_BIT,  M_INDX),
    I2(0x86, TEXT_RES,  M_BIT,  M_INDX),
    I2(0xC6, TEXT_SET,  M_BIT,  M_INDX),
};
static constexpr uint8_t INDEX_CB[] PROGMEM = {
      7,  // TEXT_BIT
     17,  // TEXT_BIT
      8,  // TEXT_RES
     18,  // TEXT_RES
      2,  // TEXT_RL
     12,  // TEXT_RL
      0,  // TEXT_RLC
     10,  // TEXT_RLC
      3,  // TEXT_RR
     13,  // TEXT_RR
      1,  // TEXT_RRC
     11,  // TEXT_RRC
      9,  // TEXT_SET
     19,  // TEXT_SET
      4,  // TEXT_SLA
     14,  // TEXT_SLA
      5,  // TEXT_SRA
     15,  // TEXT_SRA
      6,  // TEXT_SRL
     16,  // TEXT_SRL
};

static constexpr Entry TABLE_ED[] PROGMEM = {
    U2(0x70, TEXT_IN,   I_HL,   I_C), // IN (HL),(C)
    E2(0x40, TEXT_IN,   M_DST,  I_C),
    U2(0x71, TEXT_OUT,  I_C,    I_HL), // OUT (C),(HL)
    E2(0x41, TEXT_OUT,  I_C,    M_DST),
    E2(0x42, TEXT_SBC,  R_HL,   M_PTR),
    E2(0x4A, TEXT_ADC,  R_HL,   M_PTR),
    U2(0x63, TEXT_LD,   M_ABS,  R_HL), // LD (ABS),HL
    E2(0x43, TEXT_LD,   M_ABS,  M_PTR),
    U2(0x6B, TEXT_LD,   R_HL,   M_ABS), // LD HL,(ABS)
    E2(0x4B, TEXT_LD,   M_PTR,  M_ABS),
    E0(0x44, TEXT_NEG),
    E0(0x45, TEXT_RETN),
    E0(0x4D, TEXT_RETI),
    E1(0x46, TEXT_IM,   M_IMMD),
    E2(0x47, TEXT_LD,   R_IR,   R_A),
    E2(0x57, TEXT_LD,   R_A,    R_IR),
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
static constexpr uint8_t INDEX_ED[] PROGMEM = {
      5,  // TEXT_ADC
     23,  // TEXT_CPD
     25,  // TEXT_CPDR
     22,  // TEXT_CPI
     24,  // TEXT_CPIR
     13,  // TEXT_IM
      0,  // TEXT_IN
      1,  // TEXT_IN
     27,  // TEXT_IND
     29,  // TEXT_INDR
     26,  // TEXT_INI
     28,  // TEXT_INIR
      6,  // TEXT_LD
      7,  // TEXT_LD
      8,  // TEXT_LD
      9,  // TEXT_LD
     14,  // TEXT_LD
     15,  // TEXT_LD
     19,  // TEXT_LDD
     21,  // TEXT_LDDR
     18,  // TEXT_LDI
     20,  // TEXT_LDIR
     10,  // TEXT_NEG
     33,  // TEXT_OTDR
     32,  // TEXT_OTIR
      2,  // TEXT_OUT
      3,  // TEXT_OUT
     31,  // TEXT_OUTD
     30,  // TEXT_OUTI
     12,  // TEXT_RETI
     11,  // TEXT_RETN
     17,  // TEXT_RLD
     16,  // TEXT_RRD
      4,  // TEXT_SBC
};

static constexpr Entry TABLE_IX[] PROGMEM = {
    E2(0x09, TEXT_ADD,  R_IXIY, M_PIX),
    E2(0x21, TEXT_LD,   R_IXIY, M_IM16),
    E2(0x22, TEXT_LD,   M_ABS,  R_IXIY),
    E2(0x2A, TEXT_LD,   R_IXIY, M_ABS),
    E1(0x23, TEXT_INC,  R_IXIY),
    E1(0x2B, TEXT_DEC,  R_IXIY),
    E1(0x34, TEXT_INC,  M_INDX),
    E1(0x35, TEXT_DEC,  M_INDX),
    E2(0x36, TEXT_LD,   M_INDX, M_IM8),
    U2(0x76, TEXT_LD,   M_INDX, I_HL),   // LD (Ix+nn),(HL)
    U2(0x76, TEXT_LD,   I_HL,   M_INDX), // LD (HL),(Ix+nn)
    E2(0x46, TEXT_LD,   M_DST,  M_INDX),
    E2(0x70, TEXT_LD,   M_INDX, M_REG),
    E2(0x86, TEXT_ADD,  R_A,    M_INDX),
    E2(0x8E, TEXT_ADC,  R_A,    M_INDX),
    E2(0x96, TEXT_SUB,  R_A,    M_INDX),
    E1(0x96, TEXT_SUB,  M_INDX),
    E2(0x9E, TEXT_SBC,  R_A,    M_INDX),
    E2(0xA6, TEXT_AND,  R_A,    M_INDX),
    E1(0xA6, TEXT_AND,  M_INDX),
    E2(0xAE, TEXT_XOR,  R_A,    M_INDX),
    E1(0xAE, TEXT_XOR,  M_INDX),
    E2(0xB6, TEXT_OR,   R_A,    M_INDX),
    E1(0xB6, TEXT_OR,   M_INDX),
    E2(0xBE, TEXT_CP,   R_A,    M_INDX),
    E1(0xBE, TEXT_CP,   M_INDX),
    E2(0xCB, TEXT_BIT,  T_IXB,  T_IXB),  // to TABLE_CB[]
    E1(0xE1, TEXT_POP,  R_IXIY),
    E1(0xE9, TEXT_JP,   I_IXIY),
    E2(0xF9, TEXT_LD,   R_SP,   R_IXIY),
    E2(0xE3, TEXT_EX,   I_SP,   R_IXIY),
    E1(0xE5, TEXT_PUSH, R_IXIY),
};
static constexpr uint8_t INDEX_IX[] PROGMEM = {
     14,  // TEXT_ADC
      0,  // TEXT_ADD
     13,  // TEXT_ADD
     18,  // TEXT_AND
     19,  // TEXT_AND
     26,  // TEXT_BIT
     24,  // TEXT_CP
     25,  // TEXT_CP
      5,  // TEXT_DEC
      7,  // TEXT_DEC
     30,  // TEXT_EX
      4,  // TEXT_INC
      6,  // TEXT_INC
     28,  // TEXT_JP
      1,  // TEXT_LD
      2,  // TEXT_LD
      3,  // TEXT_LD
      8,  // TEXT_LD
      9,  // TEXT_LD
     10,  // TEXT_LD
     11,  // TEXT_LD
     12,  // TEXT_LD
     29,  // TEXT_LD
     22,  // TEXT_OR
     23,  // TEXT_OR
     27,  // TEXT_POP
     31,  // TEXT_PUSH
     17,  // TEXT_SBC
     15,  // TEXT_SUB
     16,  // TEXT_SUB
     20,  // TEXT_XOR
     21,  // TEXT_XOR
};

static constexpr Entry TABLE_V30EMU[] PROGMEM = {
    E1(0xED, TEXT_CALLN, M_IM8),
    E0(0xFD, TEXT_RETEM),
};

static constexpr uint8_t INDEX_V30EMU[] PROGMEM = {
      0,  // TEXT_CALLN
      1,  // TEXT_RETEM
};
// clang-format on

static constexpr TableZ80::EntryPage I8080_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(TABLE_I8080), ARRAY_RANGE(INDEX_I8080)},
};

static constexpr TableZ80::EntryPage I8085_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(TABLE_I8080), ARRAY_RANGE(INDEX_I8080)},
        {0x00, ARRAY_RANGE(TABLE_I8085), ARRAY_RANGE(INDEX_I8085)},
};

static constexpr TableZ80::EntryPage Z80_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(TABLE_Z80), ARRAY_RANGE(INDEX_Z80)},
        {0x00, ARRAY_RANGE(TABLE_I8080), ARRAY_RANGE(INDEX_I8080)},
        {0xCB, ARRAY_RANGE(TABLE_CB), ARRAY_RANGE(INDEX_CB)},
        {0xED, ARRAY_RANGE(TABLE_ED), ARRAY_RANGE(INDEX_ED)},
        {TableZ80::PREFIX_IX, ARRAY_RANGE(TABLE_IX), ARRAY_RANGE(INDEX_IX)},
        {TableZ80::PREFIX_IY, ARRAY_RANGE(TABLE_IX), ARRAY_RANGE(INDEX_IX)},
};

static constexpr TableZ80::EntryPage V30EMU_PAGES[] PROGMEM = {
        {0xED, ARRAY_RANGE(TABLE_V30EMU), ARRAY_RANGE(INDEX_V30EMU)},
        {0x00, ARRAY_RANGE(TABLE_I8080), ARRAY_RANGE(INDEX_I8080)},
};

static constexpr TableZ80::Cpu CPU_TABLE[] PROGMEM = {
        {Z80, TEXT_CPU_Z80, ARRAY_RANGE(Z80_PAGES)},
        {I8080, TEXT_CPU_8080, ARRAY_RANGE(I8080_PAGES)},
        {I8085, TEXT_CPU_8085, ARRAY_RANGE(I8085_PAGES)},
        {V30EMU, TEXT_CPU_V30EMU, ARRAY_RANGE(V30EMU_PAGES)},
};

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_REG)
        return table == M_DST;
    if (opr == R_A || opr == I_HL)
        return table == M_REG || table == M_DST;
    if (opr == R_C)
        return table == M_REG || table == M_DST || table == M_CC4 || table == M_CC8;
    if (opr == R_BC || opr == R_DE)
        return table == M_PTR || table == M_PIX || table == M_STK;
    if (opr == R_HL)
        return table == M_PTR || table == M_STK;
    if (opr == R_SP)
        return table == M_PTR || table == M_PIX;
    if (opr == R_AF)
        return table == M_STK;
    if (opr == R_IXIY)
        return table == M_PIX;
    if (opr == M_IM16)
        return table == M_IM8 || table == M_REL || table == M_BIT || table == M_VEC ||
               table == M_IMMD;
    if (opr == M_ABS)
        return table == M_IOA;
    if (opr == M_CC4)
        return table == M_CC8;
    return false;
}

static bool acceptModes(InsnZ80 &insn, const Entry *entry) {
    auto flags = insn.flags();
    auto table = entry->flags();
    if (acceptMode(flags.dst(), table.dst()) && acceptMode(flags.src(), table.src())) {
        if (table.undefined())
            insn.setError(OPERAND_NOT_ALLOWED);
        return true;
    }
    return false;
}

Error TableZ80::searchName(InsnZ80 &insn) const {
    _cpu->searchName(insn, acceptModes);
    return insn.getError();
}

static bool matchOpCode(InsnZ80 &insn, const Entry *entry, const TableZ80::EntryPage *page) {
    auto opCode = insn.opCode();
    const auto flags = entry->flags();
    const auto dst = flags.dst();
    const auto src = flags.src();

    if (dst == M_REG || src == M_REG)
        opCode &= ~7;
    if (dst == M_CC8 || src == M_DST || dst == M_DST || dst == M_VEC || dst == M_BIT) {
        opCode &= ~(7 << 3);
    } else if (dst == M_PTR || src == M_PTR || dst == M_PIX || src == M_PIX || dst == M_STK ||
               src == M_STK) {
        opCode &= ~(3 << 4);
    } else if (dst == I_BCDE || src == I_BCDE) {
        opCode &= ~(1 << 4);
    } else if (dst == M_CC4 || dst == M_IMMD) {
        opCode &= ~(3 << 3);
    } else if (dst == R_IR || src == R_IR) {
        opCode &= ~(1 << 3);
    }
    return opCode == entry->opCode();
}

Error TableZ80::searchOpCode(InsnZ80 &insn, StrBuffer &out) const {
    auto entry = _cpu->searchOpCode(insn, out, matchOpCode);
    return insn.setError(entry && !entry->flags().undefined() ? OK : UNKNOWN_INSTRUCTION);
}

TableZ80::TableZ80() {
    setCpu(Z80);
}

bool TableZ80::setCpu(CpuType cpuType) {
    auto t = Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
    if (t == nullptr)
        return false;
    _cpu = t;
    return true;
}

bool TableZ80::setCpu(const char *cpu) {
    const auto t = Cpu::search(cpu, ARRAY_RANGE(CPU_TABLE));
    if (t)
        return setCpu(t->cpuType());
    if (strncasecmp_P(cpu, TEXT_CPU_V30EMU, 6) == 0 && toupper(cpu[6]) == 'Z')
        return setCpu(V30EMU);
    if (toupper(*cpu) == 'I')
        cpu++;
    const auto z80syn = toupper(cpu[4]) == 'Z' || cpu[4] == 0;
    if (strncasecmp_P(cpu, TEXT_CPU_8080, 4) == 0 && z80syn)
        return setCpu(I8080);
    if (strncasecmp_P(cpu, TEXT_CPU_8085, 4) == 0 && z80syn)
        return setCpu(I8085);
    return false;
}

TableZ80 TableZ80::TABLE;

}  // namespace z80
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
