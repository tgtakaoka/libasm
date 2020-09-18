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

#include "config_i8051.h"

#include "entry_i8051.h"
#include "table_i8051.h"
#include "text_i8051.h"

#include <ctype.h>
#include <string.h>

namespace libasm {
namespace i8051 {

#define E3(_opc, _name, _dst, _src, _ext)                       \
    { _opc, Entry::_flags(_dst ,_src, _ext), TEXT_##_name },
#define E2(_opc, _name, _dst, _src) E3(_opc, _name, _dst, _src, NONE)

static constexpr Entry TABLE_I8051[] PROGMEM = {
    E2(0x00, NOP,   NONE,  NONE)
    E2(0x01, AJMP,  ADR11, NONE)
    E2(0x02, LJMP,  ADR16, NONE)
    E2(0x03, RR,    AREG,  NONE)
    E2(0x04, INC,   AREG,  NONE)
    E2(0x05, INC,   ADR8,  NONE)
    E2(0x06, INC,   IDIRR, NONE)
    E2(0x08, INC,   RREG,  NONE)
    E2(0x10, JBC,   BITAD, REL)
    E2(0x11, ACALL, ADR11, NONE)
    E2(0x12, LCALL, ADR16, NONE)
    E2(0x13, RRC,   AREG,  NONE)
    E2(0x14, DEC,   AREG,  NONE)
    E2(0x15, DEC,   ADR8,  NONE)
    E2(0x16, DEC,   IDIRR, NONE)
    E2(0x18, DEC,   RREG,  NONE)
    E2(0x20, JB,    BITAD, REL)
    E2(0x22, RET,   NONE,  NONE)
    E2(0x23, RL,    AREG,  NONE)
    E2(0x24, ADD,   AREG,  IMM8)
    E2(0x25, ADD,   AREG,  ADR8)
    E2(0x26, ADD,   AREG,  IDIRR)
    E2(0x28, ADD,   AREG,  RREG)
    E2(0x30, JNB,   BITAD, REL)
    E2(0x32, RETI,  NONE,  NONE)
    E2(0x33, RLC,   AREG,  NONE)
    E2(0x34, ADDC,  AREG,  IMM8)
    E2(0x35, ADDC,  AREG,  ADR8)
    E2(0x36, ADDC,  AREG,  IDIRR)
    E2(0x38, ADDC,  AREG,  RREG)
    E2(0x40, JC,    REL,   NONE)
    E2(0x42, ORL,   ADR8,  AREG)
    E2(0x43, ORL,   ADR8,  IMM8)
    E2(0x44, ORL,   AREG,  IMM8)
    E2(0x45, ORL,   AREG,  ADR8)
    E2(0x46, ORL,   AREG,  IDIRR)
    E2(0x48, ORL,   AREG,  RREG)
    E2(0x50, JNC,   REL,   NONE)
    E2(0x52, ANL,   ADR8,  AREG)
    E2(0x53, ANL,   ADR8,  IMM8)
    E2(0x54, ANL,   AREG,  IMM8)
    E2(0x55, ANL,   AREG,  ADR8)
    E2(0x56, ANL,   AREG,  IDIRR)
    E2(0x58, ANL,   AREG,  RREG)
    E2(0x60, JZ,    REL,   NONE)
    E2(0x62, XRL,   ADR8,  AREG)
    E2(0x63, XRL,   ADR8,  IMM8)
    E2(0x64, XRL,   AREG,  IMM8)
    E2(0x65, XRL,   AREG,  ADR8)
    E2(0x66, XRL,   AREG,  IDIRR)
    E2(0x68, XRL,   AREG,  RREG)
    E2(0x70, JNZ,   REL,   NONE)
    E2(0x72, ORL,   CREG,  BITAD)
    E2(0x73, JMP,   INDXD, NONE)
    E2(0x74, MOV,   AREG,  IMM8)
    E2(0x75, MOV,   ADR8,  IMM8)
    E2(0x76, MOV,   IDIRR, IMM8)
    E2(0x78, MOV,   RREG,  IMM8)
    E2(0x80, SJMP,  REL,   NONE)
    E2(0x82, ANL,   CREG,  BITAD)
    E2(0x83, MOVC,  AREG,  INDXP)
    E2(0x84, DIV,   ABREG, NONE)
    E2(0x85, MOV,   ADR8,  ADR8)
    E2(0x86, MOV,   ADR8,  IDIRR)
    E2(0x88, MOV,   ADR8,  RREG)
    E2(0x90, MOV,   DREG,  IMM16)
    E2(0x92, MOV,   BITAD, CREG)
    E2(0x93, MOVC,  AREG,  INDXD)
    E2(0x94, SUBB,  AREG,  IMM8)
    E2(0x95, SUBB,  AREG,  ADR8)
    E2(0x96, SUBB,  AREG,  IDIRR)
    E2(0x98, SUBB,  AREG,  RREG)
    E2(0xA0, ORL,   CREG,  NOTAD)
    E2(0xA2, MOV,   CREG,  BITAD)
    E2(0xA3, INC,   DREG,  NONE)
    E2(0xA4, MUL,   ABREG, NONE)
    E2(0xA6, MOV,   IDIRR, ADR8)
    E2(0xA8, MOV,   RREG,  ADR8)
    E2(0xB0, ANL,   CREG,  NOTAD)
    E2(0xB2, CPL,   BITAD, NONE)
    E2(0xB3, CPL,   CREG,  NONE)
    E3(0xB4, CJNE,  AREG,  IMM8, REL)
    E3(0xB5, CJNE,  AREG,  ADR8, REL)
    E3(0xB6, CJNE,  IDIRR, IMM8, REL)
    E3(0xB8, CJNE,  RREG,  IMM8, REL)
    E2(0xC0, PUSH,  ADR8,  NONE)
    E2(0xC2, CLR,   BITAD, NONE)
    E2(0xC3, CLR,   CREG,  NONE)
    E2(0xC4, SWAP,  AREG,  NONE)
    E2(0xC5, XCH,   AREG,  ADR8)
    E2(0xC6, XCH,   AREG,  IDIRR)
    E2(0xC8, XCH,   AREG,  RREG)
    E2(0xD0, POP,   ADR8,  NONE)
    E2(0xD2, SETB,  BITAD, NONE)
    E2(0xD3, SETB,  CREG,  NONE)
    E2(0xD4, DA,    AREG,  NONE)
    E2(0xD5, DJNZ,  ADR8,  REL)
    E2(0xD6, XCHD,  AREG,  IDIRR)
    E2(0xD8, DJNZ,  RREG,  REL)
    E2(0xE0, MOVX,  AREG,  IDIRD)
    E2(0xE2, MOVX,  AREG,  IDIRR)
    E2(0xE4, CLR,   AREG,  NONE)
    E2(0xE5, MOV,   AREG,  ADR8)
    E2(0xE6, MOV,   AREG,  IDIRR)
    E2(0xE8, MOV,   AREG,  RREG)
    E2(0xF0, MOVX,  IDIRD, AREG)
    E2(0xF2, MOVX,  IDIRR, AREG)
    E2(0xF4, CPL,   AREG,  NONE)
    E2(0xF5, MOV,   ADR8,  AREG)
    E2(0xF6, MOV,   IDIRR, AREG)
    E2(0xF8, MOV,   RREG,  AREG)
};

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table) return true;
    if (opr == IMM16) return table == IMM8;
    if (opr == ADR16)
        return table == ADR8 || table == ADR11 || table == REL
            || table == BITAD;
    return false;
}

static bool acceptModes(uint16_t flags, const Entry *entry) {
    const uint16_t table = pgm_read_word(&entry->flags);
    return acceptMode(Entry::_dstMode(flags), Entry::_dstMode(table))
        && acceptMode(Entry::_srcMode(flags), Entry::_srcMode(table))
        && acceptMode(Entry::_extMode(flags), Entry::_extMode(table));
}

Error TableI8051::searchName(InsnI8051 &insn) const {
    const char *name = insn.name();
    const uint16_t flags = Entry::_flags(
        insn.dstMode(), insn.srcMode(), insn.extMode());
    uint8_t count = 0;
    const Entry *entry =
        TableBase::searchName<Entry, uint16_t>(
            name, flags, ARRAY_RANGE(TABLE_I8051), acceptModes, count);
    if (entry) {
        insn.setOpCode(pgm_read_byte(&entry->opCode));
        insn.setFlags(pgm_read_word(&entry->flags));
        return _error.setOK();
    }
    return _error.setError(count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED);
}

static Config::opcode_t tableCode(
    Config::opcode_t opCode, const Entry *entry) {
    const uint16_t flags = pgm_read_word(&entry->flags);
    const AddrMode dst = Entry::_dstMode(flags);
    const AddrMode src = Entry::_srcMode(flags);
    if (dst == RREG || src == RREG) return opCode & ~7;
    if (dst == IDIRR || src == IDIRR) return opCode & ~1;
    if (dst == ADR11) return opCode & ~0xE0;
    return opCode;
}

Error TableI8051::searchOpCode(InsnI8051 &insn) const {
    const Config::opcode_t opCode = insn.opCode();
    const Entry *entry =
        TableBase::searchCode<Entry, Config::opcode_t>(
            opCode, ARRAY_RANGE(TABLE_I8051), tableCode);
    if (!entry) return _error.setError(UNKNOWN_INSTRUCTION);
    insn.setFlags(pgm_read_word(&entry->flags));
    const /*PROGMEM*/ char *name =
        reinterpret_cast<const char *>(pgm_read_ptr(&entry->name));
    insn.setName_P(name);
    return _error.setOK();
}

const char *TableI8051::listCpu() const {
    return TEXT_CPU_I8051;
}

const char *TableI8051::getCpu() const {
    return TEXT_CPU_8051;
}

bool TableI8051::setCpu(const char *cpu) {
    return strcasecmp_P(cpu, TEXT_CPU_8051) == 0
        || strcasecmp_P(cpu, TEXT_CPU_I8051) == 0;
}

class TableI8051 TableI8051;

} // namespace i8051
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
