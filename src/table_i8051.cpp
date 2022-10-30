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

#include <ctype.h>
#include <string.h>

#include "config_i8051.h"
#include "entry_i8051.h"
#include "text_i8051.h"

namespace libasm {
namespace i8051 {

#define E3(_opc, _name, _dst, _src, _ext) \
    { _opc, Entry::Flags::create(_dst, _src, _ext), _name }
#define E2(_opc, _name, _dst, _src) E3(_opc, _name, _dst, _src, M_NONE)
#define E1(_opc, _name, _dst) E2(_opc, _name, _dst, M_NONE)
#define E0(_opc, _name) E1(_opc, _name, M_NONE)

// clang-format off
static constexpr Entry TABLE_I8051[] PROGMEM = {
    E0(0x00, TEXT_NOP),
    E1(0x01, TEXT_AJMP,  M_ADR11),
    E1(0x02, TEXT_LJMP,  M_ADR16),
    E1(0x03, TEXT_RR,    M_AREG),
    E1(0x04, TEXT_INC,   M_AREG),
    E1(0x05, TEXT_INC,   M_ADR8),
    E1(0x06, TEXT_INC,   M_IDIRR),
    E1(0x08, TEXT_INC,   M_RREG),
    E2(0x10, TEXT_JBC,   M_BITAD, M_REL),
    E1(0x11, TEXT_ACALL, M_ADR11),
    E1(0x12, TEXT_LCALL, M_ADR16),
    E1(0x13, TEXT_RRC,   M_AREG),
    E1(0x14, TEXT_DEC,   M_AREG),
    E1(0x15, TEXT_DEC,   M_ADR8),
    E1(0x16, TEXT_DEC,   M_IDIRR),
    E1(0x18, TEXT_DEC,   M_RREG),
    E2(0x20, TEXT_JB,    M_BITAD, M_REL),
    E0(0x22, TEXT_RET),
    E1(0x23, TEXT_RL,    M_AREG),
    E2(0x24, TEXT_ADD,   M_AREG,  M_IMM8),
    E2(0x25, TEXT_ADD,   M_AREG,  M_ADR8),
    E2(0x26, TEXT_ADD,   M_AREG,  M_IDIRR),
    E2(0x28, TEXT_ADD,   M_AREG,  M_RREG),
    E2(0x30, TEXT_JNB,   M_BITAD, M_REL),
    E0(0x32, TEXT_RETI),
    E1(0x33, TEXT_RLC,   M_AREG),
    E2(0x34, TEXT_ADDC,  M_AREG,  M_IMM8),
    E2(0x35, TEXT_ADDC,  M_AREG,  M_ADR8),
    E2(0x36, TEXT_ADDC,  M_AREG,  M_IDIRR),
    E2(0x38, TEXT_ADDC,  M_AREG,  M_RREG),
    E1(0x40, TEXT_JC,    M_REL),
    E2(0x42, TEXT_ORL,   M_ADR8,  M_AREG),
    E2(0x43, TEXT_ORL,   M_ADR8,  M_IMM8),
    E2(0x44, TEXT_ORL,   M_AREG,  M_IMM8),
    E2(0x45, TEXT_ORL,   M_AREG,  M_ADR8),
    E2(0x46, TEXT_ORL,   M_AREG,  M_IDIRR),
    E2(0x48, TEXT_ORL,   M_AREG,  M_RREG),
    E1(0x50, TEXT_JNC,   M_REL),
    E2(0x52, TEXT_ANL,   M_ADR8,  M_AREG),
    E2(0x53, TEXT_ANL,   M_ADR8,  M_IMM8),
    E2(0x54, TEXT_ANL,   M_AREG,  M_IMM8),
    E2(0x55, TEXT_ANL,   M_AREG,  M_ADR8),
    E2(0x56, TEXT_ANL,   M_AREG,  M_IDIRR),
    E2(0x58, TEXT_ANL,   M_AREG,  M_RREG),
    E1(0x60, TEXT_JZ,    M_REL),
    E2(0x62, TEXT_XRL,   M_ADR8,  M_AREG),
    E2(0x63, TEXT_XRL,   M_ADR8,  M_IMM8),
    E2(0x64, TEXT_XRL,   M_AREG,  M_IMM8),
    E2(0x65, TEXT_XRL,   M_AREG,  M_ADR8),
    E2(0x66, TEXT_XRL,   M_AREG,  M_IDIRR),
    E2(0x68, TEXT_XRL,   M_AREG,  M_RREG),
    E1(0x70, TEXT_JNZ,   M_REL),
    E2(0x72, TEXT_ORL,   M_CREG,  M_BITAD),
    E1(0x73, TEXT_JMP,   M_INDXD),
    E2(0x74, TEXT_MOV,   M_AREG,  M_IMM8),
    E2(0x75, TEXT_MOV,   M_ADR8,  M_IMM8),
    E2(0x76, TEXT_MOV,   M_IDIRR, M_IMM8),
    E2(0x78, TEXT_MOV,   M_RREG,  M_IMM8),
    E1(0x80, TEXT_SJMP,  M_REL),
    E2(0x82, TEXT_ANL,   M_CREG,  M_BITAD),
    E2(0x83, TEXT_MOVC,  M_AREG,  M_INDXP),
    E1(0x84, TEXT_DIV,   M_ABREG),
    E2(0x85, TEXT_MOV,   M_ADR8,  M_ADR8),
    E2(0x86, TEXT_MOV,   M_ADR8,  M_IDIRR),
    E2(0x88, TEXT_MOV,   M_ADR8,  M_RREG),
    E2(0x90, TEXT_MOV,   M_DREG,  M_IMM16),
    E2(0x92, TEXT_MOV,   M_BITAD, M_CREG),
    E2(0x93, TEXT_MOVC,  M_AREG,  M_INDXD),
    E2(0x94, TEXT_SUBB,  M_AREG,  M_IMM8),
    E2(0x95, TEXT_SUBB,  M_AREG,  M_ADR8),
    E2(0x96, TEXT_SUBB,  M_AREG,  M_IDIRR),
    E2(0x98, TEXT_SUBB,  M_AREG,  M_RREG),
    E2(0xA0, TEXT_ORL,   M_CREG,  M_NOTAD),
    E2(0xA2, TEXT_MOV,   M_CREG,  M_BITAD),
    E1(0xA3, TEXT_INC,   M_DREG),
    E1(0xA4, TEXT_MUL,   M_ABREG),
    E2(0xA6, TEXT_MOV,   M_IDIRR, M_ADR8),
    E2(0xA8, TEXT_MOV,   M_RREG,  M_ADR8),
    E2(0xB0, TEXT_ANL,   M_CREG,  M_NOTAD),
    E1(0xB2, TEXT_CPL,   M_BITAD),
    E1(0xB3, TEXT_CPL,   M_CREG),
    E3(0xB4, TEXT_CJNE,  M_AREG,  M_IMM8, M_REL),
    E3(0xB5, TEXT_CJNE,  M_AREG,  M_ADR8, M_REL),
    E3(0xB6, TEXT_CJNE,  M_IDIRR, M_IMM8, M_REL),
    E3(0xB8, TEXT_CJNE,  M_RREG,  M_IMM8, M_REL),
    E1(0xC0, TEXT_PUSH,  M_ADR8),
    E1(0xC2, TEXT_CLR,   M_BITAD),
    E1(0xC3, TEXT_CLR,   M_CREG),
    E1(0xC4, TEXT_SWAP,  M_AREG),
    E2(0xC5, TEXT_XCH,   M_AREG,  M_ADR8),
    E2(0xC6, TEXT_XCH,   M_AREG,  M_IDIRR),
    E2(0xC8, TEXT_XCH,   M_AREG,  M_RREG),
    E1(0xD0, TEXT_POP,   M_ADR8),
    E1(0xD2, TEXT_SETB,  M_BITAD),
    E1(0xD3, TEXT_SETB,  M_CREG),
    E1(0xD4, TEXT_DA,    M_AREG),
    E2(0xD5, TEXT_DJNZ,  M_ADR8,  M_REL),
    E2(0xD6, TEXT_XCHD,  M_AREG,  M_IDIRR),
    E2(0xD8, TEXT_DJNZ,  M_RREG,  M_REL),
    E2(0xE0, TEXT_MOVX,  M_AREG,  M_IDIRD),
    E2(0xE2, TEXT_MOVX,  M_AREG,  M_IDIRR),
    E1(0xE4, TEXT_CLR,   M_AREG),
    E2(0xE5, TEXT_MOV,   M_AREG,  M_ADR8),
    E2(0xE6, TEXT_MOV,   M_AREG,  M_IDIRR),
    E2(0xE8, TEXT_MOV,   M_AREG,  M_RREG),
    E2(0xF0, TEXT_MOVX,  M_IDIRD, M_AREG),
    E2(0xF2, TEXT_MOVX,  M_IDIRR, M_AREG),
    E1(0xF4, TEXT_CPL,   M_AREG),
    E2(0xF5, TEXT_MOV,   M_ADR8,  M_AREG),
    E2(0xF6, TEXT_MOV,   M_IDIRR, M_AREG),
    E2(0xF8, TEXT_MOV,   M_RREG,  M_AREG),
};

static constexpr uint8_t INDEX_I8051[] PROGMEM = {
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

typedef EntryPageBase<Entry> EntryPage;

static constexpr EntryPage I8051_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_I8051), ARRAY_RANGE(INDEX_I8051)},
};

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_IMM16)
        return table == M_IMM8;
    if (opr == M_ADR16)
        return table == M_ADR8 || table == M_ADR11 || table == M_REL || table == M_BITAD;
    return false;
}

static bool acceptModes(Entry::Flags flags, const Entry *entry) {
    auto table = entry->flags();
    return acceptMode(flags.dst(), table.dst()) && acceptMode(flags.src(), table.src()) &&
           acceptMode(flags.ext(), table.ext());
}

Error TableI8051::searchName(InsnI8051 &insn) {
    uint8_t count = 0;
    auto entry = searchEntry(insn.name(), insn.flags(), I8051_PAGES, acceptModes, count);
    if (entry) {
        insn.setOpCode(entry->opCode());
        insn.setFlags(entry->flags());
        return setOK();
    }
    return setError(count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED);
}

static Config::opcode_t tableCode(Config::opcode_t opCode, const Entry *entry) {
    auto flags = entry->flags();
    auto dst = flags.dst();
    auto src = flags.src();
    if (dst == M_RREG || src == M_RREG)
        return opCode & ~7;
    if (dst == M_IDIRR || src == M_IDIRR)
        return opCode & ~1;
    if (dst == M_ADR11)
        return opCode & ~0xE0;
    return opCode;
}

Error TableI8051::searchOpCode(InsnI8051 &insn) {
    auto opCode = insn.opCode();
    auto entry = searchEntry(opCode, ARRAY_RANGE(TABLE_I8051), tableCode);
    if (!entry)
        return setError(UNKNOWN_INSTRUCTION);
    insn.setFlags(entry->flags());
    insn.nameBuffer().text_P(entry->name_P());
    return setOK();
}

const /* PROGMEM */ char *TableI8051::listCpu_P() const {
    return TEXT_CPU_I8051;
}

const /* PROGMEM */ char *TableI8051::cpu_P() const {
    return TEXT_CPU_8051;
}

bool TableI8051::setCpu(const char *cpu) {
    return strcasecmp_P(cpu, TEXT_CPU_8051) == 0 || strcasecmp_P(cpu, TEXT_CPU_I8051) == 0;
}

TableI8051 TableI8051::TABLE;

}  // namespace i8051
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
