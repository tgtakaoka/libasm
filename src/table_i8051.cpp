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

#include "table_i8051.h"
#include "entry_i8051.h"
#include "entry_table.h"
#include "text_i8051.h"

using namespace libasm::text::i8051;

namespace libasm {
namespace i8051 {

#define E2(_opc, _cf, _name, _dst, _src) {_opc, Entry::Flags::create(_cf, _dst, _src), _name}
#define E1(_opc, _cf, _name, _dst) E2(_opc, _cf, _name, _dst, M_NONE)
#define E0(_opc, _cf, _name) E1(_opc, _cf, _name, M_NONE)

// clang-format off
constexpr Entry TABLE_I8051[] PROGMEM = {
    E0(0x00, CF_00, TEXT_NOP),
    E1(0x01, CF_E0, TEXT_AJMP,  M_ADR11),
    E1(0x02, CF_00, TEXT_LJMP,  M_ADR16),
    E1(0x03, CF_00, TEXT_RR,    M_AREG),
    E1(0x04, CF_00, TEXT_INC,   M_AREG),
    E1(0x05, CF_00, TEXT_INC,   M_ADR8),
    E1(0x06, CF_01, TEXT_INC,   M_IDIRR),
    E1(0x08, CF_07, TEXT_INC,   M_RREG),
    E2(0x10, CF_00, TEXT_JBC,   M_BITAD, M_REL),
    E1(0x11, CF_E0, TEXT_ACALL, M_ADR11),
    E1(0x12, CF_00, TEXT_LCALL, M_ADR16),
    E1(0x13, CF_00, TEXT_RRC,   M_AREG),
    E1(0x14, CF_00, TEXT_DEC,   M_AREG),
    E1(0x15, CF_00, TEXT_DEC,   M_ADR8),
    E1(0x16, CF_01, TEXT_DEC,   M_IDIRR),
    E1(0x18, CF_07, TEXT_DEC,   M_RREG),
    E2(0x20, CF_00, TEXT_JB,    M_BITAD, M_REL),
    E0(0x22, CF_00, TEXT_RET),
    E1(0x23, CF_00, TEXT_RL,    M_AREG),
    E2(0x24, CF_00, TEXT_ADD,   M_AREG,  M_IMM8),
    E2(0x25, CF_00, TEXT_ADD,   M_AREG,  M_ADR8),
    E2(0x26, CF_01, TEXT_ADD,   M_AREG,  M_IDIRR),
    E2(0x28, CF_07, TEXT_ADD,   M_AREG,  M_RREG),
    E2(0x30, CF_00, TEXT_JNB,   M_BITAD, M_REL),
    E0(0x32, CF_00, TEXT_RETI),
    E1(0x33, CF_00, TEXT_RLC,   M_AREG),
    E2(0x34, CF_00, TEXT_ADDC,  M_AREG,  M_IMM8),
    E2(0x35, CF_00, TEXT_ADDC,  M_AREG,  M_ADR8),
    E2(0x36, CF_01, TEXT_ADDC,  M_AREG,  M_IDIRR),
    E2(0x38, CF_07, TEXT_ADDC,  M_AREG,  M_RREG),
    E1(0x40, CF_00, TEXT_JC,    M_REL),
    E2(0x42, CF_00, TEXT_ORL,   M_ADR8,  M_AREG),
    E2(0x43, CF_00, TEXT_ORL,   M_ADR8,  M_IMM8),
    E2(0x44, CF_00, TEXT_ORL,   M_AREG,  M_IMM8),
    E2(0x45, CF_00, TEXT_ORL,   M_AREG,  M_ADR8),
    E2(0x46, CF_01, TEXT_ORL,   M_AREG,  M_IDIRR),
    E2(0x48, CF_07, TEXT_ORL,   M_AREG,  M_RREG),
    E1(0x50, CF_00, TEXT_JNC,   M_REL),
    E2(0x52, CF_00, TEXT_ANL,   M_ADR8,  M_AREG),
    E2(0x53, CF_00, TEXT_ANL,   M_ADR8,  M_IMM8),
    E2(0x54, CF_00, TEXT_ANL,   M_AREG,  M_IMM8),
    E2(0x55, CF_00, TEXT_ANL,   M_AREG,  M_ADR8),
    E2(0x56, CF_01, TEXT_ANL,   M_AREG,  M_IDIRR),
    E2(0x58, CF_07, TEXT_ANL,   M_AREG,  M_RREG),
    E1(0x60, CF_00, TEXT_JZ,    M_REL),
    E2(0x62, CF_00, TEXT_XRL,   M_ADR8,  M_AREG),
    E2(0x63, CF_00, TEXT_XRL,   M_ADR8,  M_IMM8),
    E2(0x64, CF_00, TEXT_XRL,   M_AREG,  M_IMM8),
    E2(0x65, CF_00, TEXT_XRL,   M_AREG,  M_ADR8),
    E2(0x66, CF_01, TEXT_XRL,   M_AREG,  M_IDIRR),
    E2(0x68, CF_07, TEXT_XRL,   M_AREG,  M_RREG),
    E1(0x70, CF_00, TEXT_JNZ,   M_REL),
    E2(0x72, CF_00, TEXT_ORL,   M_CREG,  M_BITAD),
    E1(0x73, CF_00, TEXT_JMP,   M_INDXD),
    E2(0x74, CF_00, TEXT_MOV,   M_AREG,  M_IMM8),
    E2(0x75, CF_00, TEXT_MOV,   M_ADR8,  M_IMM8),
    E2(0x76, CF_01, TEXT_MOV,   M_IDIRR, M_IMM8),
    E2(0x78, CF_07, TEXT_MOV,   M_RREG,  M_IMM8),
    E1(0x80, CF_00, TEXT_SJMP,  M_REL),
    E2(0x82, CF_00, TEXT_ANL,   M_CREG,  M_BITAD),
    E2(0x83, CF_00, TEXT_MOVC,  M_AREG,  M_INDXP),
    E1(0x84, CF_00, TEXT_DIV,   M_ABREG),
    E2(0x85, CF_00, TEXT_MOV,   M_ADR8,  M_ADR8),
    E2(0x86, CF_01, TEXT_MOV,   M_ADR8,  M_IDIRR),
    E2(0x88, CF_07, TEXT_MOV,   M_ADR8,  M_RREG),
    E2(0x90, CF_00, TEXT_MOV,   M_DREG,  M_IMM16),
    E2(0x92, CF_00, TEXT_MOV,   M_BITAD, M_CREG),
    E2(0x93, CF_00, TEXT_MOVC,  M_AREG,  M_INDXD),
    E2(0x94, CF_00, TEXT_SUBB,  M_AREG,  M_IMM8),
    E2(0x95, CF_00, TEXT_SUBB,  M_AREG,  M_ADR8),
    E2(0x96, CF_01, TEXT_SUBB,  M_AREG,  M_IDIRR),
    E2(0x98, CF_07, TEXT_SUBB,  M_AREG,  M_RREG),
    E2(0xA0, CF_00, TEXT_ORL,   M_CREG,  M_NOTAD),
    E2(0xA2, CF_00, TEXT_MOV,   M_CREG,  M_BITAD),
    E1(0xA3, CF_00, TEXT_INC,   M_DREG),
    E1(0xA4, CF_00, TEXT_MUL,   M_ABREG),
    E2(0xA6, CF_01, TEXT_MOV,   M_IDIRR, M_ADR8),
    E2(0xA8, CF_07, TEXT_MOV,   M_RREG,  M_ADR8),
    E2(0xB0, CF_00, TEXT_ANL,   M_CREG,  M_NOTAD),
    E1(0xB2, CF_00, TEXT_CPL,   M_BITAD),
    E1(0xB3, CF_00, TEXT_CPL,   M_CREG),
    E2(0xB4, CF_00, TEXT_CJNE,  M_AREG,  M_IMM8_REL),
    E2(0xB5, CF_00, TEXT_CJNE,  M_AREG,  M_ADR8_REL),
    E2(0xB6, CF_01, TEXT_CJNE,  M_IDIRR, M_IMM8_REL),
    E2(0xB8, CF_07, TEXT_CJNE,  M_RREG,  M_IMM8_REL),
    E1(0xC0, CF_00, TEXT_PUSH,  M_ADR8),
    E1(0xC2, CF_00, TEXT_CLR,   M_BITAD),
    E1(0xC3, CF_00, TEXT_CLR,   M_CREG),
    E1(0xC4, CF_00, TEXT_SWAP,  M_AREG),
    E2(0xC5, CF_00, TEXT_XCH,   M_AREG,  M_ADR8),
    E2(0xC6, CF_01, TEXT_XCH,   M_AREG,  M_IDIRR),
    E2(0xC8, CF_07, TEXT_XCH,   M_AREG,  M_RREG),
    E1(0xD0, CF_00, TEXT_POP,   M_ADR8),
    E1(0xD2, CF_00, TEXT_SETB,  M_BITAD),
    E1(0xD3, CF_00, TEXT_SETB,  M_CREG),
    E1(0xD4, CF_00, TEXT_DA,    M_AREG),
    E2(0xD5, CF_00, TEXT_DJNZ,  M_ADR8,  M_REL),
    E2(0xD6, CF_01, TEXT_XCHD,  M_AREG,  M_IDIRR),
    E2(0xD8, CF_07, TEXT_DJNZ,  M_RREG,  M_REL),
    E2(0xE0, CF_00, TEXT_MOVX,  M_AREG,  M_IDIRD),
    E2(0xE2, CF_01, TEXT_MOVX,  M_AREG,  M_IDIRR),
    E1(0xE4, CF_00, TEXT_CLR,   M_AREG),
    E2(0xE5, CF_00, TEXT_MOV,   M_AREG,  M_ADR8),
    E2(0xE6, CF_01, TEXT_MOV,   M_AREG,  M_IDIRR),
    E2(0xE8, CF_07, TEXT_MOV,   M_AREG,  M_RREG),
    E2(0xF0, CF_00, TEXT_MOVX,  M_IDIRD, M_AREG),
    E2(0xF2, CF_01, TEXT_MOVX,  M_IDIRR, M_AREG),
    E1(0xF4, CF_00, TEXT_CPL,   M_AREG),
    E2(0xF5, CF_00, TEXT_MOV,   M_ADR8,  M_AREG),
    E2(0xF6, CF_01, TEXT_MOV,   M_IDIRR, M_AREG),
    E2(0xF8, CF_07, TEXT_MOV,   M_RREG,  M_AREG),
};

constexpr uint8_t INDEX_I8051[] PROGMEM = {
      9,  // TEXT_ACALL
     19,  // TEXT_ADD
     20,  // TEXT_ADD
     21,  // TEXT_ADD
     22,  // TEXT_ADD
     26,  // TEXT_ADDC
     27,  // TEXT_ADDC
     28,  // TEXT_ADDC
     29,  // TEXT_ADDC
      1,  // TEXT_AJMP
     38,  // TEXT_ANL
     39,  // TEXT_ANL
     40,  // TEXT_ANL
     41,  // TEXT_ANL
     42,  // TEXT_ANL
     43,  // TEXT_ANL
     59,  // TEXT_ANL
     78,  // TEXT_ANL
     81,  // TEXT_CJNE
     82,  // TEXT_CJNE
     83,  // TEXT_CJNE
     84,  // TEXT_CJNE
     86,  // TEXT_CLR
     87,  // TEXT_CLR
    101,  // TEXT_CLR
     79,  // TEXT_CPL
     80,  // TEXT_CPL
    107,  // TEXT_CPL
     95,  // TEXT_DA
     12,  // TEXT_DEC
     13,  // TEXT_DEC
     14,  // TEXT_DEC
     15,  // TEXT_DEC
     61,  // TEXT_DIV
     96,  // TEXT_DJNZ
     98,  // TEXT_DJNZ
      4,  // TEXT_INC
      5,  // TEXT_INC
      6,  // TEXT_INC
      7,  // TEXT_INC
     74,  // TEXT_INC
     16,  // TEXT_JB
      8,  // TEXT_JBC
     30,  // TEXT_JC
     53,  // TEXT_JMP
     23,  // TEXT_JNB
     37,  // TEXT_JNC
     51,  // TEXT_JNZ
     44,  // TEXT_JZ
     10,  // TEXT_LCALL
      2,  // TEXT_LJMP
     54,  // TEXT_MOV
     55,  // TEXT_MOV
     56,  // TEXT_MOV
     57,  // TEXT_MOV
     62,  // TEXT_MOV
     63,  // TEXT_MOV
     64,  // TEXT_MOV
     65,  // TEXT_MOV
     66,  // TEXT_MOV
     73,  // TEXT_MOV
     76,  // TEXT_MOV
     77,  // TEXT_MOV
    102,  // TEXT_MOV
    103,  // TEXT_MOV
    104,  // TEXT_MOV
    108,  // TEXT_MOV
    109,  // TEXT_MOV
    110,  // TEXT_MOV
     60,  // TEXT_MOVC
     67,  // TEXT_MOVC
     99,  // TEXT_MOVX
    100,  // TEXT_MOVX
    105,  // TEXT_MOVX
    106,  // TEXT_MOVX
     75,  // TEXT_MUL
      0,  // TEXT_NOP
     31,  // TEXT_ORL
     32,  // TEXT_ORL
     33,  // TEXT_ORL
     34,  // TEXT_ORL
     35,  // TEXT_ORL
     36,  // TEXT_ORL
     52,  // TEXT_ORL
     72,  // TEXT_ORL
     92,  // TEXT_POP
     85,  // TEXT_PUSH
     17,  // TEXT_RET
     24,  // TEXT_RETI
     18,  // TEXT_RL
     25,  // TEXT_RLC
      3,  // TEXT_RR
     11,  // TEXT_RRC
     93,  // TEXT_SETB
     94,  // TEXT_SETB
     58,  // TEXT_SJMP
     68,  // TEXT_SUBB
     69,  // TEXT_SUBB
     70,  // TEXT_SUBB
     71,  // TEXT_SUBB
     88,  // TEXT_SWAP
     89,  // TEXT_XCH
     90,  // TEXT_XCH
     91,  // TEXT_XCH
     97,  // TEXT_XCHD
     45,  // TEXT_XRL
     46,  // TEXT_XRL
     47,  // TEXT_XRL
     48,  // TEXT_XRL
     49,  // TEXT_XRL
     50,  // TEXT_XRL
};
// clang-format on

using EntryPage = entry::TableBase<Entry>;

constexpr EntryPage I8051_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_I8051), ARRAY_RANGE(INDEX_I8051)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {I8051, TEXT_CPU_8051, ARRAY_RANGE(I8051_PAGES)},
};

const Cpu *cpu(CpuType) {
    return &CPU_TABLE[0];
}

bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_IMM16)
        return table == M_IMM8;
    if (opr == M_ADR16)
        return table == M_ADR8 || table == M_ADR11 || table == M_REL || table == M_BITAD;
    return false;
}

bool acceptModes(AsmInsn &insn, const Entry *entry) {
    const auto table = entry->readFlags();
    return acceptMode(insn.dstOp.mode, table.dst()) && acceptMode(insn.srcOp.mode, table.src()) &&
           acceptMode(insn.extOp.mode, table.ext());
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
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

bool invalidDirect(Config::opcode_t opc, uint16_t addr) {
    // MOV A, ACC is not a valid instruction
    return opc == 0xE5 && addr == 0xE0;
}

const /*PROGMEM*/ char *TableI8051::listCpu_P() const {
    return TEXT_CPU_I8051;
}

const /*PROGMEM*/ char *TableI8051::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableI8051::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    name.iexpect('i');
    if (name.iequals_P(TEXT_CPU_8051)) {
        cpuType = I8051;
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TableI8051 TABLE;

}  // namespace i8051
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
