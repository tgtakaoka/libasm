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
#include "entry_i8096.h"
#include "entry_table.h"
#include "text_i8096.h"

using namespace libasm::text::i8096;

namespace libasm {
namespace i8096 {

#define E3(_opc, _cf, _name, _dst, _src1, _src2) \
    {_opc, Entry::Flags::create(_cf, _dst, _src1, _src2), _name}
#define E2(_opc, _cf, _name, _dst, _src1) E3(_opc, _cf, _name, _dst, _src1, M_NONE)
#define E1(_opc, _cf, _name, _dst) E2(_opc, _cf, _name, _dst, M_NONE)
#define E0(_opc, _cf, _name) E1(_opc, _cf, _name, M_NONE)
#define U2(_opc, _cf, _name, _dst, _src1) {_opc, Entry::Flags::undef(_cf, _dst, _src1), _name}
#define U1(_opc, _cf, _name, _dst) U2(_opc, _cf, _name, _dst, M_NONE)

// clang-format off
constexpr Entry TABLE_00[] PROGMEM = {
    E2(0x64, CF_03, TEXT_ADD,   M_WREG,  M_WAOP),
    E3(0x44, CF_03, TEXT_ADD,   M_WREG,  M_WREG,  M_WAOP),
    E2(0x74, CF_03, TEXT_ADDB,  M_BREG,  M_BAOP),
    E3(0x54, CF_03, TEXT_ADDB,  M_BREG,  M_BREG,  M_BAOP),
    E2(0xA4, CF_03, TEXT_ADDC,  M_WREG,  M_WAOP),
    E2(0xB4, CF_03, TEXT_ADDCB, M_BREG,  M_BAOP),
    E2(0x60, CF_03, TEXT_AND,   M_WREG,  M_WAOP),
    E3(0x40, CF_03, TEXT_AND,   M_WREG,  M_WREG,  M_WAOP),
    E2(0x70, CF_03, TEXT_ANDB,  M_BREG,  M_BAOP),
    E3(0x50, CF_03, TEXT_ANDB,  M_BREG,  M_BREG,  M_BAOP),
    E1(0xE3, CF_00, TEXT_BR,    M_INDIR),
    E1(0x01, CF_00, TEXT_CLR,   M_WREG),
    E1(0x11, CF_00, TEXT_CLRB,  M_BREG),
    E0(0xF8, CF_00, TEXT_CLRC),
    E0(0xFC, CF_00, TEXT_CLRVT),
    E2(0x88, CF_03, TEXT_CMP,   M_WREG,  M_WAOP),
    E2(0x98, CF_03, TEXT_CMPB,  M_BREG,  M_BAOP),
    E1(0x05, CF_00, TEXT_DEC,   M_WREG),
    E1(0x15, CF_00, TEXT_DECB,  M_BREG),
    E0(0xFA, CF_00, TEXT_DI),
    E2(0x8C, CF_03, TEXT_DIVU,  M_LREG,  M_WAOP),
    E2(0x9C, CF_03, TEXT_DIVUB, M_WREG,  M_BAOP),
    E2(0xE0, CF_00, TEXT_DJNZ,  M_BREG,  M_REL8),
    E0(0xFB, CF_00, TEXT_EI),
    E1(0x06, CF_00, TEXT_EXT,   M_LREG),
    E1(0x16, CF_00, TEXT_EXTB,  M_WREG),
    E1(0x07, CF_00, TEXT_INC,   M_WREG),
    E1(0x17, CF_00, TEXT_INCB,  M_BREG),
    E3(0x30, CF_07, TEXT_JBC,   M_BREG,  M_BITNO, M_REL8),
    E3(0x38, CF_07, TEXT_JBS,   M_BREG,  M_BITNO, M_REL8),
    E1(0xDB, CF_00, TEXT_JC,    M_REL8),
    E1(0xDF, CF_00, TEXT_JE,    M_REL8),
    E1(0xD6, CF_00, TEXT_JGE,   M_REL8),
    E1(0xD2, CF_00, TEXT_JGT,   M_REL8),
    E1(0xD9, CF_00, TEXT_JH,    M_REL8),
    E1(0xDA, CF_00, TEXT_JLE,   M_REL8),
    E1(0xDE, CF_00, TEXT_JLT,   M_REL8),
    E1(0xD3, CF_00, TEXT_JNC,   M_REL8),
    E1(0xD7, CF_00, TEXT_JNE,   M_REL8),
    E1(0xD1, CF_00, TEXT_JNH,   M_REL8),
    E1(0xD0, CF_00, TEXT_JNST,  M_REL8),
    E1(0xD5, CF_00, TEXT_JNV,   M_REL8),
    E1(0xD4, CF_00, TEXT_JNVT,  M_REL8),
    E1(0xD8, CF_00, TEXT_JST,   M_REL8),
    E1(0xDD, CF_00, TEXT_JV,    M_REL8),
    E1(0xDC, CF_00, TEXT_JVT,   M_REL8),
    E1(0xEF, CF_00, TEXT_LCALL, M_REL16),
    E2(0xA0, CF_03, TEXT_LD,    M_WREG,  M_WAOP),
    E2(0xB0, CF_03, TEXT_LDB,   M_BREG,  M_BAOP),
    E2(0xBC, CF_03, TEXT_LDBSE, M_WREG,  M_BAOP),
    E2(0xAC, CF_03, TEXT_LDBZE, M_WREG,  M_BAOP),
    E1(0xE7, CF_00, TEXT_LJMP,  M_REL16),
    E2(0x6C, CF_03, TEXT_MULU,  M_LREG,  M_WAOP),
    E3(0x4C, CF_03, TEXT_MULU,  M_LREG,  M_WREG,  M_WAOP),
    E2(0x7C, CF_03, TEXT_MULUB, M_WREG,  M_BAOP),
    E3(0x5C, CF_03, TEXT_MULUB, M_WREG,  M_BREG,  M_BAOP),
    E1(0x03, CF_00, TEXT_NEG,   M_WREG),
    E1(0x13, CF_00, TEXT_NEGB,  M_BREG),
    E0(0xFD, CF_00, TEXT_NOP),
    E2(0x0F, CF_00, TEXT_NORML, M_LREG,  M_BREG),
    E1(0x02, CF_00, TEXT_NOT,   M_WREG),
    E1(0x12, CF_00, TEXT_NOTB,  M_BREG),
    E2(0x80, CF_03, TEXT_OR,    M_WREG,  M_WAOP),
    E2(0x90, CF_03, TEXT_ORB,   M_BREG,  M_BAOP),
    U1(0xCD, CF_00, TEXT_POP,   M_IMM16), // undefined POP immediate instruction
    E1(0xCC, CF_03, TEXT_POP,   M_WAOP),
    E0(0xF3, CF_00, TEXT_POPF),
    E1(0xC8, CF_03, TEXT_PUSH,  M_WAOP),
    E0(0xF2, CF_00, TEXT_PUSHF),
    E0(0xF0, CF_00, TEXT_RET),
    E0(0xFF, CF_00, TEXT_RST),
    E1(0x28, CF_07, TEXT_SCALL, M_REL11),
    E0(0xF9, CF_00, TEXT_SETC),
    E2(0x09, CF_00, TEXT_SHL,   M_WREG,  M_COUNT),
    E2(0x19, CF_00, TEXT_SHLB,  M_BREG,  M_COUNT),
    E2(0x0D, CF_00, TEXT_SHLL,  M_LREG,  M_COUNT),
    E2(0x08, CF_00, TEXT_SHR,   M_WREG,  M_COUNT),
    E2(0x0A, CF_00, TEXT_SHRA,  M_WREG,  M_COUNT),
    E2(0x1A, CF_00, TEXT_SHRAB, M_BREG,  M_COUNT),
    E2(0x0E, CF_00, TEXT_SHRAL, M_LREG,  M_COUNT),
    E2(0x18, CF_00, TEXT_SHRB,  M_BREG,  M_COUNT),
    E2(0x0C, CF_00, TEXT_SHRL,  M_LREG,  M_COUNT),
    E1(0x20, CF_07, TEXT_SJMP,  M_REL11),
    E1(0x00, CF_00, TEXT_SKIP,  M_BREG),
    U2(0xC1, CF_00, TEXT_ST,    M_WREG,  M_IMM16), // undefined ST immediate instruction
    E2(0xC0, CF_03, TEXT_ST,    M_WREG,  M_WAOP),
    U2(0xC5, CF_00, TEXT_STB,   M_BREG,  M_IMM8), // undefined STB immediate instruction
    E2(0xC4, CF_03, TEXT_STB,   M_BREG,  M_BAOP),
    E2(0x68, CF_03, TEXT_SUB,   M_WREG,  M_WAOP),
    E3(0x48, CF_03, TEXT_SUB,   M_WREG,  M_WREG,  M_WAOP),
    E2(0x78, CF_03, TEXT_SUBB,  M_BREG,  M_BAOP),
    E3(0x58, CF_03, TEXT_SUBB,  M_BREG,  M_BREG,  M_BAOP),
    E2(0xA8, CF_03, TEXT_SUBC,  M_WREG,  M_WAOP),
    E2(0xB8, CF_03, TEXT_SUBCB, M_BREG,  M_BAOP),
    E0(0xF7, CF_00, TEXT_TRAP),
    E2(0x84, CF_03, TEXT_XOR,   M_WREG,  M_WAOP),
    E2(0x94, CF_03, TEXT_XORB,  M_BREG,  M_BAOP),
};

constexpr uint8_t INDEX_00[] PROGMEM = {
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

constexpr Entry TABLE_FE[] PROGMEM = {
    E2(0x8C, CF_03, TEXT_DIV,   M_LREG,  M_WAOP),
    E2(0x9C, CF_03, TEXT_DIVB,  M_WREG,  M_BAOP),
    E2(0x6C, CF_03, TEXT_MUL,   M_LREG,  M_WAOP),
    E3(0x4C, CF_03, TEXT_MUL,   M_LREG,  M_WREG,  M_WAOP),
    E2(0x7C, CF_03, TEXT_MULB,  M_WREG,  M_BAOP),
    E3(0x5C, CF_03, TEXT_MULB,  M_WREG,  M_BREG,  M_BAOP),
};

constexpr uint8_t INDEX_FE[] PROGMEM = {
      0,  // TEXT_DIV
      1,  // TEXT_DIVB
      2,  // TEXT_MUL
      3,  // TEXT_MUL
      4,  // TEXT_MULB
      5,  // TEXT_MULB
};
// clang-format on

using EntryPage = entry::PrefixTableBase<Entry>;

constexpr EntryPage I8096_PAGES[] PROGMEM = {
        {0x00, ARRAY_RANGE(TABLE_00), ARRAY_RANGE(INDEX_00)},
        {0xFE, ARRAY_RANGE(TABLE_FE), ARRAY_RANGE(INDEX_FE)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {I8096, TEXT_CPU_8096, ARRAY_RANGE(I8096_PAGES)},
};

const Cpu *cpu(CpuType) {
    return &CPU_TABLE[0];
}

bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_ADDR)
        return table == M_BREG || table == M_WREG || table == M_LREG || table == M_BAOP ||
               table == M_WAOP || table == M_REL8 || table == M_REL11 || table == M_REL16 ||
               table == M_COUNT || table == M_BITNO;
    if (opr == M_IMM16)
        return table == M_BAOP || table == M_WAOP || table == M_COUNT;
    if (opr == M_INDIR || opr == M_AINC || opr == M_IDX16)
        return table == M_BAOP || table == M_WAOP;
    return false;
}

bool acceptModes(AsmInsn &insn, const Entry *entry) {
    const auto table = entry->readFlags();
    if (acceptMode(insn.dstOp.mode, table.dst()) && acceptMode(insn.src1Op.mode, table.src1()) &&
            acceptMode(insn.src2Op.mode, table.src2())) {
        if (table.undefined())
            insn.setErrorIf(OPERAND_NOT_ALLOWED);
        return true;
    }
    return false;
}

Error searchName(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, acceptModes);
    return insn.getError();
}

bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *) {
    auto opc = insn.opCode();
    opc &= ~entry->readFlags().mask();
    return opc == entry->readOpCode();
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    const auto entry = cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    if (entry && entry->readFlags().undefined()) {
        insn.nameBuffer().reset();
        insn.setErrorIf(UNKNOWN_INSTRUCTION);
    }
    return insn.getError();
}

bool isPrefix(CpuType cpuType, Config::opcode_t code) {
    return cpu(cpuType)->isPrefix(code);
}

const /*PROGMEM*/ char *TableI8096::listCpu_P() const {
    return TEXT_CPU_I8096;
}

const /*PROGMEM*/ char *TableI8096::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableI8096::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    name.iexpect('i');
    if (name.iequals(TEXT_CPU_8096)) {
        cpuType = I8096;
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TableI8096 TABLE;

}  // namespace i8096
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
