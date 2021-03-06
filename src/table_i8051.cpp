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
#define E2(_opc, _name, _dst, _src) E3(_opc, _name, _dst, _src, NONE)

// clang-format off
static constexpr Entry TABLE_I8051[] PROGMEM = {
    E2(0x00, TEXT_NOP,   NONE,  NONE),
    E2(0x01, TEXT_AJMP,  ADR11, NONE),
    E2(0x02, TEXT_LJMP,  ADR16, NONE),
    E2(0x03, TEXT_RR,    AREG,  NONE),
    E2(0x04, TEXT_INC,   AREG,  NONE),
    E2(0x05, TEXT_INC,   ADR8,  NONE),
    E2(0x06, TEXT_INC,   IDIRR, NONE),
    E2(0x08, TEXT_INC,   RREG,  NONE),
    E2(0x10, TEXT_JBC,   BITAD, REL),
    E2(0x11, TEXT_ACALL, ADR11, NONE),
    E2(0x12, TEXT_LCALL, ADR16, NONE),
    E2(0x13, TEXT_RRC,   AREG,  NONE),
    E2(0x14, TEXT_DEC,   AREG,  NONE),
    E2(0x15, TEXT_DEC,   ADR8,  NONE),
    E2(0x16, TEXT_DEC,   IDIRR, NONE),
    E2(0x18, TEXT_DEC,   RREG,  NONE),
    E2(0x20, TEXT_JB,    BITAD, REL),
    E2(0x22, TEXT_RET,   NONE,  NONE),
    E2(0x23, TEXT_RL,    AREG,  NONE),
    E2(0x24, TEXT_ADD,   AREG,  IMM8),
    E2(0x25, TEXT_ADD,   AREG,  ADR8),
    E2(0x26, TEXT_ADD,   AREG,  IDIRR),
    E2(0x28, TEXT_ADD,   AREG,  RREG),
    E2(0x30, TEXT_JNB,   BITAD, REL),
    E2(0x32, TEXT_RETI,  NONE,  NONE),
    E2(0x33, TEXT_RLC,   AREG,  NONE),
    E2(0x34, TEXT_ADDC,  AREG,  IMM8),
    E2(0x35, TEXT_ADDC,  AREG,  ADR8),
    E2(0x36, TEXT_ADDC,  AREG,  IDIRR),
    E2(0x38, TEXT_ADDC,  AREG,  RREG),
    E2(0x40, TEXT_JC,    REL,   NONE),
    E2(0x42, TEXT_ORL,   ADR8,  AREG),
    E2(0x43, TEXT_ORL,   ADR8,  IMM8),
    E2(0x44, TEXT_ORL,   AREG,  IMM8),
    E2(0x45, TEXT_ORL,   AREG,  ADR8),
    E2(0x46, TEXT_ORL,   AREG,  IDIRR),
    E2(0x48, TEXT_ORL,   AREG,  RREG),
    E2(0x50, TEXT_JNC,   REL,   NONE),
    E2(0x52, TEXT_ANL,   ADR8,  AREG),
    E2(0x53, TEXT_ANL,   ADR8,  IMM8),
    E2(0x54, TEXT_ANL,   AREG,  IMM8),
    E2(0x55, TEXT_ANL,   AREG,  ADR8),
    E2(0x56, TEXT_ANL,   AREG,  IDIRR),
    E2(0x58, TEXT_ANL,   AREG,  RREG),
    E2(0x60, TEXT_JZ,    REL,   NONE),
    E2(0x62, TEXT_XRL,   ADR8,  AREG),
    E2(0x63, TEXT_XRL,   ADR8,  IMM8),
    E2(0x64, TEXT_XRL,   AREG,  IMM8),
    E2(0x65, TEXT_XRL,   AREG,  ADR8),
    E2(0x66, TEXT_XRL,   AREG,  IDIRR),
    E2(0x68, TEXT_XRL,   AREG,  RREG),
    E2(0x70, TEXT_JNZ,   REL,   NONE),
    E2(0x72, TEXT_ORL,   CREG,  BITAD),
    E2(0x73, TEXT_JMP,   INDXD, NONE),
    E2(0x74, TEXT_MOV,   AREG,  IMM8),
    E2(0x75, TEXT_MOV,   ADR8,  IMM8),
    E2(0x76, TEXT_MOV,   IDIRR, IMM8),
    E2(0x78, TEXT_MOV,   RREG,  IMM8),
    E2(0x80, TEXT_SJMP,  REL,   NONE),
    E2(0x82, TEXT_ANL,   CREG,  BITAD),
    E2(0x83, TEXT_MOVC,  AREG,  INDXP),
    E2(0x84, TEXT_DIV,   ABREG, NONE),
    E2(0x85, TEXT_MOV,   ADR8,  ADR8),
    E2(0x86, TEXT_MOV,   ADR8,  IDIRR),
    E2(0x88, TEXT_MOV,   ADR8,  RREG),
    E2(0x90, TEXT_MOV,   DREG,  IMM16),
    E2(0x92, TEXT_MOV,   BITAD, CREG),
    E2(0x93, TEXT_MOVC,  AREG,  INDXD),
    E2(0x94, TEXT_SUBB,  AREG,  IMM8),
    E2(0x95, TEXT_SUBB,  AREG,  ADR8),
    E2(0x96, TEXT_SUBB,  AREG,  IDIRR),
    E2(0x98, TEXT_SUBB,  AREG,  RREG),
    E2(0xA0, TEXT_ORL,   CREG,  NOTAD),
    E2(0xA2, TEXT_MOV,   CREG,  BITAD),
    E2(0xA3, TEXT_INC,   DREG,  NONE),
    E2(0xA4, TEXT_MUL,   ABREG, NONE),
    E2(0xA6, TEXT_MOV,   IDIRR, ADR8),
    E2(0xA8, TEXT_MOV,   RREG,  ADR8),
    E2(0xB0, TEXT_ANL,   CREG,  NOTAD),
    E2(0xB2, TEXT_CPL,   BITAD, NONE),
    E2(0xB3, TEXT_CPL,   CREG,  NONE),
    E3(0xB4, TEXT_CJNE,  AREG,  IMM8, REL),
    E3(0xB5, TEXT_CJNE,  AREG,  ADR8, REL),
    E3(0xB6, TEXT_CJNE,  IDIRR, IMM8, REL),
    E3(0xB8, TEXT_CJNE,  RREG,  IMM8, REL),
    E2(0xC0, TEXT_PUSH,  ADR8,  NONE),
    E2(0xC2, TEXT_CLR,   BITAD, NONE),
    E2(0xC3, TEXT_CLR,   CREG,  NONE),
    E2(0xC4, TEXT_SWAP,  AREG,  NONE),
    E2(0xC5, TEXT_XCH,   AREG,  ADR8),
    E2(0xC6, TEXT_XCH,   AREG,  IDIRR),
    E2(0xC8, TEXT_XCH,   AREG,  RREG),
    E2(0xD0, TEXT_POP,   ADR8,  NONE),
    E2(0xD2, TEXT_SETB,  BITAD, NONE),
    E2(0xD3, TEXT_SETB,  CREG,  NONE),
    E2(0xD4, TEXT_DA,    AREG,  NONE),
    E2(0xD5, TEXT_DJNZ,  ADR8,  REL),
    E2(0xD6, TEXT_XCHD,  AREG,  IDIRR),
    E2(0xD8, TEXT_DJNZ,  RREG,  REL),
    E2(0xE0, TEXT_MOVX,  AREG,  IDIRD),
    E2(0xE2, TEXT_MOVX,  AREG,  IDIRR),
    E2(0xE4, TEXT_CLR,   AREG,  NONE),
    E2(0xE5, TEXT_MOV,   AREG,  ADR8),
    E2(0xE6, TEXT_MOV,   AREG,  IDIRR),
    E2(0xE8, TEXT_MOV,   AREG,  RREG),
    E2(0xF0, TEXT_MOVX,  IDIRD, AREG),
    E2(0xF2, TEXT_MOVX,  IDIRR, AREG),
    E2(0xF4, TEXT_CPL,   AREG,  NONE),
    E2(0xF5, TEXT_MOV,   ADR8,  AREG),
    E2(0xF6, TEXT_MOV,   IDIRR, AREG),
    E2(0xF8, TEXT_MOV,   RREG,  AREG),
};
// clang-format on

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == IMM16)
        return table == IMM8;
    if (opr == ADR16)
        return table == ADR8 || table == ADR11 || table == REL ||
               table == BITAD;
    return false;
}

static bool acceptModes(Entry::Flags flags, const Entry *entry) {
    const Entry::Flags table = entry->flags();
    return acceptMode(flags.dstMode(), table.dstMode()) &&
           acceptMode(flags.srcMode(), table.srcMode()) &&
           acceptMode(flags.extMode(), table.extMode());
}

Error TableI8051::searchName(InsnI8051 &insn) const {
    uint8_t count = 0;
    const Entry *entry = TableBase::searchName<Entry, Entry::Flags>(insn.name(),
            insn.flags(), ARRAY_RANGE(TABLE_I8051), acceptModes, count);
    if (entry) {
        insn.setOpCode(entry->opCode());
        insn.setFlags(entry->flags());
        return setOK();
    }
    return setError(count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED);
}

static Config::opcode_t tableCode(Config::opcode_t opCode, const Entry *entry) {
    const Entry::Flags flags = entry->flags();
    const AddrMode dst = flags.dstMode();
    const AddrMode src = flags.srcMode();
    if (dst == RREG || src == RREG)
        return opCode & ~7;
    if (dst == IDIRR || src == IDIRR)
        return opCode & ~1;
    if (dst == ADR11)
        return opCode & ~0xE0;
    return opCode;
}

Error TableI8051::searchOpCode(InsnI8051 &insn) const {
    const Config::opcode_t opCode = insn.opCode();
    const Entry *entry = TableBase::searchCode<Entry, Config::opcode_t>(
            opCode, ARRAY_RANGE(TABLE_I8051), tableCode);
    if (!entry)
        return setError(UNKNOWN_INSTRUCTION);
    insn.setFlags(entry->flags());
    insn.setName_P(entry->name());
    return setOK();
}

const char *TableI8051::listCpu() const {
    return TEXT_CPU_I8051;
}

const char *TableI8051::getCpu() const {
    return TEXT_CPU_8051;
}

bool TableI8051::setCpu(const char *cpu) {
    return strcasecmp_P(cpu, TEXT_CPU_8051) == 0 ||
           strcasecmp_P(cpu, TEXT_CPU_I8051) == 0;
}

class TableI8051 TableI8051;

}  // namespace i8051
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
