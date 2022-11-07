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

#include "table_i8096.h"

#include <string.h>

#include "config_i8096.h"
#include "entry_i8096.h"
#include "text_i8096.h"

namespace libasm {
namespace i8096 {

#define E3(_opc, _name, _dst, _src1, _src2) \
    { _opc, Entry::Flags::create(_dst, _src1, _src2), _name }
#define E2(_opc, _name, _dst, _src1) E3(_opc, _name, _dst, _src1, M_NONE)
#define E1(_opc, _name, _dst) E2(_opc, _name, _dst, M_NONE)
#define E0(_opc, _name) E1(_opc, _name, M_NONE)
#define U2(_opc, _name, _dst, _src1) \
    { _opc, Entry::Flags::undef(_dst, _src1), _name }
#define U1(_opc, _name, _dst) U2(_opc, _name, _dst, M_NONE)

// clang-format off
static const Entry TABLE_00[] PROGMEM = {
    E2(0x64, TEXT_ADD,   M_WREG,  M_WAOP),
    E3(0x44, TEXT_ADD,   M_WREG,  M_WREG,  M_WAOP),
    E2(0x74, TEXT_ADDB,  M_BREG,  M_BAOP),
    E3(0x54, TEXT_ADDB,  M_BREG,  M_BREG,  M_BAOP),
    E2(0xA4, TEXT_ADDC,  M_WREG,  M_WAOP),
    E2(0xB4, TEXT_ADDCB, M_BREG,  M_BAOP),
    E2(0x60, TEXT_AND,   M_WREG,  M_WAOP),
    E3(0x40, TEXT_AND,   M_WREG,  M_WREG,  M_WAOP),
    E2(0x70, TEXT_ANDB,  M_BREG,  M_BAOP),
    E3(0x50, TEXT_ANDB,  M_BREG,  M_BREG,  M_BAOP),
    E1(0xE3, TEXT_BR,    M_INDIR),
    E1(0x01, TEXT_CLR,   M_WREG),
    E1(0x11, TEXT_CLRB,  M_BREG),
    E0(0xF8, TEXT_CLRC),
    E0(0xFC, TEXT_CLRVT),
    E2(0x88, TEXT_CMP,   M_WREG,  M_WAOP),
    E2(0x98, TEXT_CMPB,  M_BREG,  M_BAOP),
    E1(0x05, TEXT_DEC,   M_WREG),
    E1(0x15, TEXT_DECB,  M_BREG),
    E0(0xFA, TEXT_DI),
    E2(0x8C, TEXT_DIVU,  M_LREG,  M_WAOP),
    E2(0x9C, TEXT_DIVUB, M_WREG,  M_BAOP),
    E2(0xE0, TEXT_DJNZ,  M_BREG,  M_REL8),
    E0(0xFB, TEXT_EI),
    E1(0x06, TEXT_EXT,   M_LREG),
    E1(0x16, TEXT_EXTB,  M_WREG),
    E1(0x07, TEXT_INC,   M_WREG),
    E1(0x17, TEXT_INCB,  M_BREG),
    E3(0x30, TEXT_JBC,   M_BREG,  M_BITNO, M_REL8),
    E3(0x38, TEXT_JBS,   M_BREG,  M_BITNO, M_REL8),
    E1(0xDB, TEXT_JC,    M_REL8),
    E1(0xDF, TEXT_JE,    M_REL8),
    E1(0xD6, TEXT_JGE,   M_REL8),
    E1(0xD2, TEXT_JGT,   M_REL8),
    E1(0xD9, TEXT_JH,    M_REL8),
    E1(0xDA, TEXT_JLE,   M_REL8),
    E1(0xDE, TEXT_JLT,   M_REL8),
    E1(0xD3, TEXT_JNC,   M_REL8),
    E1(0xD7, TEXT_JNE,   M_REL8),
    E1(0xD1, TEXT_JNH,   M_REL8),
    E1(0xD0, TEXT_JNST,  M_REL8),
    E1(0xD5, TEXT_JNV,   M_REL8),
    E1(0xD4, TEXT_JNVT,  M_REL8),
    E1(0xD8, TEXT_JST,   M_REL8),
    E1(0xDD, TEXT_JV,    M_REL8),
    E1(0xDC, TEXT_JVT,   M_REL8),
    E1(0xEF, TEXT_LCALL, M_REL16),
    E2(0xA0, TEXT_LD,    M_WREG,  M_WAOP),
    E2(0xB0, TEXT_LDB,   M_BREG,  M_BAOP),
    E2(0xBC, TEXT_LDBSE, M_WREG,  M_BAOP),
    E2(0xAC, TEXT_LDBZE, M_WREG,  M_BAOP),
    E1(0xE7, TEXT_LJMP,  M_REL16),
    E2(0x6C, TEXT_MULU,  M_LREG,  M_WAOP),
    E3(0x4C, TEXT_MULU,  M_LREG,  M_WREG,  M_WAOP),
    E2(0x7C, TEXT_MULUB, M_WREG,  M_BAOP),
    E3(0x5C, TEXT_MULUB, M_WREG,  M_BREG,  M_BAOP),
    E1(0x03, TEXT_NEG,   M_WREG),
    E1(0x13, TEXT_NEGB,  M_BREG),
    E0(0xFD, TEXT_NOP),
    E2(0x0F, TEXT_NORML, M_LREG,  M_BREG),
    E1(0x02, TEXT_NOT,   M_WREG),
    E1(0x12, TEXT_NOTB,  M_BREG),
    E2(0x80, TEXT_OR,    M_WREG,  M_WAOP),
    E2(0x90, TEXT_ORB,   M_BREG,  M_BAOP),
    U1(0xCD, TEXT_POP,   M_IMM16), // undefined POP immediate instruction
    E1(0xCC, TEXT_POP,   M_WAOP),
    E0(0xF3, TEXT_POPF),
    E1(0xC8, TEXT_PUSH,  M_WAOP),
    E0(0xF2, TEXT_PUSHF),
    E0(0xF0, TEXT_RET),
    E0(0xFF, TEXT_RST),
    E1(0x28, TEXT_SCALL, M_REL11),
    E0(0xF9, TEXT_SETC),
    E2(0x09, TEXT_SHL,   M_WREG,  M_COUNT),
    E2(0x19, TEXT_SHLB,  M_BREG,  M_COUNT),
    E2(0x0D, TEXT_SHLL,  M_LREG,  M_COUNT),
    E2(0x08, TEXT_SHR,   M_WREG,  M_COUNT),
    E2(0x0A, TEXT_SHRA,  M_WREG,  M_COUNT),
    E2(0x1A, TEXT_SHRAB, M_BREG,  M_COUNT),
    E2(0x0E, TEXT_SHRAL, M_LREG,  M_COUNT),
    E2(0x18, TEXT_SHRB,  M_BREG,  M_COUNT),
    E2(0x0C, TEXT_SHRL,  M_LREG,  M_COUNT),
    E1(0x20, TEXT_SJMP,  M_REL11),
    E1(0x00, TEXT_SKIP,  M_BREG),
    U2(0xC1, TEXT_ST,    M_WREG,  M_IMM16), // undefined ST immediate instruction
    E2(0xC0, TEXT_ST,    M_WREG,  M_WAOP),
    U2(0xC5, TEXT_STB,   M_BREG,  M_IMM8), // undefined STB immediate instruction
    E2(0xC4, TEXT_STB,   M_BREG,  M_BAOP),
    E2(0x68, TEXT_SUB,   M_WREG,  M_WAOP),
    E3(0x48, TEXT_SUB,   M_WREG,  M_WREG,  M_WAOP),
    E2(0x78, TEXT_SUBB,  M_BREG,  M_BAOP),
    E3(0x58, TEXT_SUBB,  M_BREG,  M_BREG,  M_BAOP),
    E2(0xA8, TEXT_SUBC,  M_WREG,  M_WAOP),
    E2(0xB8, TEXT_SUBCB, M_BREG,  M_BAOP),
    E0(0xF7, TEXT_TRAP),
    E2(0x84, TEXT_XOR,   M_WREG,  M_WAOP),
    E2(0x94, TEXT_XORB,  M_BREG,  M_BAOP),
};

static const uint8_t INDEX_00[] PROGMEM = {
      0,  // TEXT_ADD
      1,  // TEXT_ADD
      2,  // TEXT_ADDB
      3,  // TEXT_ADDB
      4,  // TEXT_ADDC
      5,  // TEXT_ADDCB
      6,  // TEXT_AND
      7,  // TEXT_AND
      8,  // TEXT_ANDB
      9,  // TEXT_ANDB
     10,  // TEXT_BR
     11,  // TEXT_CLR
     12,  // TEXT_CLRB
     13,  // TEXT_CLRC
     14,  // TEXT_CLRVT
     15,  // TEXT_CMP
     16,  // TEXT_CMPB
     17,  // TEXT_DEC
     18,  // TEXT_DECB
     19,  // TEXT_DI
     20,  // TEXT_DIVU
     21,  // TEXT_DIVUB
     22,  // TEXT_DJNZ
     23,  // TEXT_EI
     24,  // TEXT_EXT
     25,  // TEXT_EXTB
     26,  // TEXT_INC
     27,  // TEXT_INCB
     28,  // TEXT_JBC
     29,  // TEXT_JBS
     30,  // TEXT_JC
     31,  // TEXT_JE
     32,  // TEXT_JGE
     33,  // TEXT_JGT
     34,  // TEXT_JH
     35,  // TEXT_JLE
     36,  // TEXT_JLT
     37,  // TEXT_JNC
     38,  // TEXT_JNE
     39,  // TEXT_JNH
     40,  // TEXT_JNST
     41,  // TEXT_JNV
     42,  // TEXT_JNVT
     43,  // TEXT_JST
     44,  // TEXT_JV
     45,  // TEXT_JVT
     46,  // TEXT_LCALL
     47,  // TEXT_LD
     48,  // TEXT_LDB
     49,  // TEXT_LDBSE
     50,  // TEXT_LDBZE
     51,  // TEXT_LJMP
     52,  // TEXT_MULU
     53,  // TEXT_MULU
     54,  // TEXT_MULUB
     55,  // TEXT_MULUB
     56,  // TEXT_NEG
     57,  // TEXT_NEGB
     58,  // TEXT_NOP
     59,  // TEXT_NORML
     60,  // TEXT_NOT
     61,  // TEXT_NOTB
     62,  // TEXT_OR
     63,  // TEXT_ORB
     64,  // TEXT_POP
     65,  // TEXT_POP
     66,  // TEXT_POPF
     67,  // TEXT_PUSH
     68,  // TEXT_PUSHF
     69,  // TEXT_RET
     70,  // TEXT_RST
     71,  // TEXT_SCALL
     72,  // TEXT_SETC
     73,  // TEXT_SHL
     74,  // TEXT_SHLB
     75,  // TEXT_SHLL
     76,  // TEXT_SHR
     77,  // TEXT_SHRA
     78,  // TEXT_SHRAB
     79,  // TEXT_SHRAL
     80,  // TEXT_SHRB
     81,  // TEXT_SHRL
     82,  // TEXT_SJMP
     83,  // TEXT_SKIP
     84,  // TEXT_ST
     85,  // TEXT_ST
     86,  // TEXT_STB
     87,  // TEXT_STB
     88,  // TEXT_SUB
     89,  // TEXT_SUB
     90,  // TEXT_SUBB
     91,  // TEXT_SUBB
     92,  // TEXT_SUBC
     93,  // TEXT_SUBCB
     94,  // TEXT_TRAP
     95,  // TEXT_XOR
     96,  // TEXT_XORB
};

static const Entry TABLE_FE[] PROGMEM = {
    E2(0x8C, TEXT_DIV,   M_LREG,  M_WAOP),
    E2(0x9C, TEXT_DIVB,  M_WREG,  M_BAOP),
    E2(0x6C, TEXT_MUL,   M_LREG,  M_WAOP),
    E3(0x4C, TEXT_MUL,   M_LREG,  M_WREG,  M_WAOP),
    E2(0x7C, TEXT_MULB,  M_WREG,  M_BAOP),
    E3(0x5C, TEXT_MULB,  M_WREG,  M_BREG,  M_BAOP),
};

static const uint8_t INDEX_FE[] PROGMEM = {
      0,  // TEXT_DIV
      1,  // TEXT_DIVB
      2,  // TEXT_MUL
      3,  // TEXT_MUL
      4,  // TEXT_MULB
      5,  // TEXT_MULB
};
// clang-format on

static const TableI8096::EntryPage I8096_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(TABLE_00), ARRAY_RANGE(INDEX_00)},
        {0xFE, ARRAY_RANGE(TABLE_FE), ARRAY_RANGE(INDEX_FE)},
};

static const TableI8096::Cpu CPU_I8096 PROGMEM = {I8096, TEXT_CPU_8096, ARRAY_RANGE(I8096_PAGES)};

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_ADDR)
        return table == M_BREG || table == M_WREG || table == M_LREG || table == M_BAOP ||
               table == M_WAOP || table == M_REL8 || table == M_REL11 || table == M_REL16 ||
               table == M_COUNT || table == M_BITNO;
    if (opr == M_IMM16)
        return table == M_BAOP || table == M_WAOP || table == M_COUNT;
    if (opr == M_INDIR)
        return table == M_BAOP || table == M_WAOP;
    if (opr == M_IDX16)
        return table == M_BAOP || table == M_WAOP;
    return false;
}

static bool acceptModes(Entry::Flags flags, const Entry *entry) {
    auto table = entry->flags();
    return acceptMode(flags.dst(), table.dst()) && acceptMode(flags.src1(), table.src1()) &&
           acceptMode(flags.src2(), table.src2());
}

Error TableI8096::searchName(InsnI8096 &insn, const EntryPage *pages, const EntryPage *end) const {
    uint8_t count = 0;
    for (auto page = pages; page < end; page++) {
        auto entry = searchEntry(insn.name(), insn.flags(), page, acceptModes, count);
        if (entry) {
            if (entry->flags().undefined())
                return OPERAND_NOT_ALLOWED;
            insn.setOpCode(entry->opCode(), page->prefix());
            insn.setFlags(entry->flags());
            return OK;
        }
    }
    return count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED;
}

static Config::opcode_t maskCode(Config::opcode_t opcode, const Entry *entry) {
    const auto &flags = entry->flags();
    const auto dst = flags.dst();
    const auto src1 = flags.src1();
    const auto src2 = flags.src2();
    Config::opcode_t mask = 0;
    if (dst == M_BAOP || src1 == M_BAOP || src2 == M_BAOP || dst == M_WAOP || src1 == M_WAOP ||
            src2 == M_WAOP)
        mask |= 3;
    if (dst == M_REL11 || src1 == M_BITNO)
        mask |= 7;
    return opcode & ~mask;
}

Error TableI8096::searchOpCode(
        InsnI8096 &insn, const EntryPage *pages, const EntryPage *end) const {
    for (auto page = pages; page < end; page++) {
        if (insn.prefix() != page->prefix())
            continue;
        auto entry = searchEntry(insn.opCode(), page->table(), page->end(), maskCode);
        if (entry) {
            if (entry->flags().undefined())
                break;
            insn.setFlags(entry->flags());
            insn.nameBuffer().text_P(entry->name_P());
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

Error TableI8096::searchName(InsnI8096 &insn) {
    return setError(searchName(insn, ARRAY_RANGE(I8096_PAGES)));
}

Error TableI8096::searchOpCode(InsnI8096 &insn) {
    return setError(searchOpCode(insn, ARRAY_RANGE(I8096_PAGES)));
}

TableI8096::TableI8096() : _cpu(&CPU_I8096) {}

const /* PROGMEM */ char *TableI8096::listCpu_P() const {
    return TEXT_CPU_LIST;
}

const /* PROGMEM */ char *TableI8096::cpu_P() const {
    return _cpu->name_P();
}

bool TableI8096::setCpu(const char *cpu) {
    if (toupper(*cpu) == 'I')
        cpu++;
    return strcmp_P(cpu, TEXT_CPU_8096) == 0;
}

TableI8096 TableI8096::TABLE;

}  // namespace i8096
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
