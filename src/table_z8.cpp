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

#include "config_z8.h"

#include "entry_z8.h"
#include "table_z8.h"
#include "text_z8.h"

#include <ctype.h>
#include <string.h>

namespace libasm {
namespace z8 {

#define E(_opc, _name, _dst, _src)              \
    { _opc,                                     \
      Entry::_flags(M_##_dst, M_##_src),        \
      TEXT_##_name                              \
    },

static constexpr Entry TABLE_Z8[] PROGMEM = {
    E(0x08, LD,   r,   R)
    E(0x09, LD,   R,   r)
    E(0x0C, LD,   r,   IM)
    E(0xC7, LD,   r,   X)
    E(0xD7, LD,   X,   r)
    E(0xE3, LD,   r,   Ir)
    E(0xE4, LD,   R,   R)
    E(0xE5, LD,   R,   IR)
    E(0xE6, LD,   R,   IM)
    E(0xE7, LD,   IR,  IM)
    E(0xF3, LD,   Ir,  r)
    E(0xF5, LD,   IR,  R)
    E(0x82, LDE,  r,   Irr)
    E(0x92, LDE,  Irr, r)
    E(0x83, LDEI, Ir,  Irr)
    E(0x93, LDEI, Irr, Ir)
    E(0xC2, LDC,  r,   Irr)
    E(0xD2, LDC,  Irr, r)
    E(0xC3, LDCI, Ir,  Irr)
    E(0xD3, LDCI, Irr, Ir)
    E(0x0A, DJNZ, r,   RA)
    E(0x8B, JR,   RA,  NO)
    E(0x0B, JR,   cc,  RA)
    E(0x8D, JP,   DA,  NO)
    E(0x0D, JP,   cc,  DA)
    E(0x30, JP,   IRR, NO)
    E(0x31, SRP,  IM,  NO)
    E(0xD6, CALL, DA,  NO)
    E(0xD4, CALL, IRR, NO)
    E(0x02, ADD,  r,   r)
    E(0x03, ADD,  r,   Ir)
    E(0x04, ADD,  R,   R)
    E(0x05, ADD,  R,   IR)
    E(0x06, ADD,  R,   IM)
    E(0x07, ADD,  IR,  IM)
    E(0x12, ADC,  r,   r)
    E(0x13, ADC,  r,   Ir)
    E(0x14, ADC,  R,   R)
    E(0x15, ADC,  R,   IR)
    E(0x16, ADC,  R,   IM)
    E(0x17, ADC,  IR,  IM)
    E(0x22, SUB,  r,   r)
    E(0x23, SUB,  r,   Ir)
    E(0x24, SUB,  R,   R)
    E(0x25, SUB,  R,   IR)
    E(0x26, SUB,  R,   IM)
    E(0x27, SUB,  IR,  IM)
    E(0x32, SBC,  r,   r)
    E(0x33, SBC,  r,   Ir)
    E(0x34, SBC,  R,   R)
    E(0x35, SBC,  R,   IR)
    E(0x36, SBC,  R,   IM)
    E(0x37, SBC,  IR,  IM)
    E(0x42, OR,   r,   r)
    E(0x43, OR,   r,   Ir)
    E(0x44, OR,   R,   R)
    E(0x45, OR,   R,   IR)
    E(0x46, OR,   R,   IM)
    E(0x47, OR,   IR,  IM)
    E(0x52, AND,  r,   r)
    E(0x53, AND,  r,   Ir)
    E(0x54, AND,  R,   R)
    E(0x55, AND,  R,   IR)
    E(0x56, AND,  R,   IM)
    E(0x57, AND,  IR,  IM)
    E(0x62, TCM,  r,   r)
    E(0x63, TCM,  r,   Ir)
    E(0x64, TCM,  R,   R)
    E(0x65, TCM,  R,   IR)
    E(0x66, TCM,  R,   IM)
    E(0x67, TCM,  IR,  IM)
    E(0x72, TM,   r,   r)
    E(0x73, TM,   r,   Ir)
    E(0x74, TM,   R,   R)
    E(0x75, TM,   R,   IR)
    E(0x76, TM,   R,   IM)
    E(0x77, TM,   IR,  IM)
    E(0xA2, CP,   r,   r)
    E(0xA3, CP,   r,   Ir)
    E(0xA4, CP,   R,   R)
    E(0xA5, CP,   R,   IR)
    E(0xA6, CP,   R,   IM)
    E(0xA7, CP,   IR,  IM)
    E(0xB2, XOR,  r,   r)
    E(0xB3, XOR,  r,   Ir)
    E(0xB4, XOR,  R,   R)
    E(0xB5, XOR,  R,   IR)
    E(0xB6, XOR,  R,   IM)
    E(0xB7, XOR,  IR,  IM)
    E(0x00, DEC,  R,   NO)
    E(0x01, DEC,  IR,  NO)
    E(0x10, RLC,  R,   NO)
    E(0x11, RLC,  IR,  NO)
    E(0x0E, INC,  r,   NO)
    E(0x20, INC,  R,   NO)
    E(0x21, INC,  IR,  NO)
    E(0x40, DA,   R,   NO)
    E(0x41, DA,   IR,  NO)
    E(0x50, POP,  R,   NO)
    E(0x51, POP,  IR,  NO)
    E(0x60, COM,  R,   NO)
    E(0x61, COM,  IR,  NO)
    E(0x70, PUSH, R,   NO)
    E(0x71, PUSH, IR,  NO)
    E(0x80, DECW, R,   NO)
    E(0x81, DECW, IR,  NO)
    E(0x90, RL,   R,   NO)
    E(0x91, RL,   IR,  NO)
    E(0xA0, INCW, R,   NO)
    E(0xA1, INCW, IR,  NO)
    E(0xB0, CLR,  R,   NO)
    E(0xB1, CLR,  IR,  NO)
    E(0xC0, RRC,  R,   NO)
    E(0xC1, RRC,  IR,  NO)
    E(0xD0, SRA,  R,   NO)
    E(0xD1, SRA,  IR,  NO)
    E(0xE0, RR,   R,   NO)
    E(0xE1, RR,   IR,  NO)
    E(0xF0, SWAP, R,   NO)
    E(0xF1, SWAP, IR,  NO)
    E(0x6F, STOP, NO,  NO)
    E(0x7F, HALT, NO,  NO)
    E(0x8F, DI,   NO,  NO)
    E(0x9F, EI,   NO,  NO)
    E(0xAF, RET,  NO,  NO)
    E(0xBF, IRET, NO,  NO)
    E(0xCF, RCF,  NO,  NO)
    E(0xDF, SCF,  NO,  NO)
    E(0xEF, CCF,  NO,  NO)
    E(0xFF, NOP,  NO,  NO)
};

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table) return true;
    if (opr == M_DA) return table == M_RA;
    if (opr == M_r) return table == M_R;
    if (opr == M_Ir) return table == M_IR;
    return false;
}

static bool acceptModes(host::uint_t flags, const Entry *entry) {
    const host::uint_t table = pgm_read_word(&entry->flags);
    return acceptMode(Entry::_dstMode(flags), Entry::_dstMode(table))
        && acceptMode(Entry::_srcMode(flags), Entry::_srcMode(table));
}

Error TableZ8::searchName(InsnZ8 &insn) const {
    const char *name = insn.name();
    const host::uint_t flags = Entry::_flags(insn.dstMode(), insn.srcMode());
    const Entry *entry = TableBase::searchName<Entry,host::uint_t>(
        name, flags, ARRAY_RANGE(TABLE_Z8), acceptModes);
    if (entry) {
        insn.setOpCode(pgm_read_byte(&entry->opCode));
        insn.setFlags(pgm_read_byte(&entry->flags));
        return OK;
    }
    return UNKNOWN_INSTRUCTION;
}

static Config::opcode_t maskCode(
    Config::opcode_t opCode, const Entry *entry) {
    const Config::opcode_t table = pgm_read_byte(&entry->opCode);
    return InsnZ8::operandInOpCode(table) ? opCode & 0x0f : opCode;
}

Error TableZ8::searchOpCode(InsnZ8 &insn) const {
    const Entry *entry = TableBase::searchCode<Entry,Config::opcode_t>(
        insn.opCode(), ARRAY_RANGE(TABLE_Z8), maskCode);
    if (entry) {
        insn.setFlags(pgm_read_word(&entry->flags));
        TableBase::setName(insn.insn(), entry->name, Config::NAME_MAX);
        return OK;
    }
    return UNKNOWN_INSTRUCTION;
}

bool TableZ8::setCpu(const char *cpu) {
    return strcasecmp(cpu, "z8") == 0
        || strncasecmp(cpu, "z86", 3) == 0;
}

class TableZ8 TableZ8;

} // namespace z8
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
