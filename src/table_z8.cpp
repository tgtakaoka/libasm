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

#define E(_opc, _name, _dst, _src)                      \
    { _opc, Entry::_flags(_dst, _src), TEXT_##_name },

static constexpr Entry TABLE_Z8[] PROGMEM = {
    E(0x08, LD,   M_r,   M_R)
    E(0x09, LD,   M_R,   M_r)
    E(0x0C, LD,   M_r,   M_IM)
    E(0xC7, LD,   M_r,   M_X)
    E(0xD7, LD,   M_X,   M_r)
    E(0xE3, LD,   M_r,   M_Ir)
    E(0xE4, LD,   M_R,   M_R)
    E(0xE5, LD,   M_R,   M_IR)
    E(0xE6, LD,   M_R,   M_IM)
    E(0xE7, LD,   M_IR,  M_IM)
    E(0xF3, LD,   M_Ir,  M_r)
    E(0xF5, LD,   M_IR,  M_R)
    E(0x82, LDE,  M_r,   M_Irr)
    E(0x92, LDE,  M_Irr, M_r)
    E(0x83, LDEI, M_Ir,  M_Irr)
    E(0x93, LDEI, M_Irr, M_Ir)
    E(0xC2, LDC,  M_r,   M_Irr)
    E(0xD2, LDC,  M_Irr, M_r)
    E(0xC3, LDCI, M_Ir,  M_Irr)
    E(0xD3, LDCI, M_Irr, M_Ir)
    E(0x0A, DJNZ, M_r,   M_RA)
    E(0x8B, JR,   M_RA,  M_NO)
    E(0x0B, JR,   M_cc,  M_RA)
    E(0x8D, JP,   M_DA,  M_NO)
    E(0x0D, JP,   M_cc,  M_DA)
    E(0x30, JP,   M_IRR, M_NO)
    E(0x31, SRP,  M_IM,  M_NO)
    E(0xD6, CALL, M_DA,  M_NO)
    E(0xD4, CALL, M_IRR, M_NO)
    E(0x02, ADD,  M_r,   M_r)
    E(0x03, ADD,  M_r,   M_Ir)
    E(0x04, ADD,  M_R,   M_R)
    E(0x05, ADD,  M_R,   M_IR)
    E(0x06, ADD,  M_R,   M_IM)
    E(0x07, ADD,  M_IR,  M_IM)
    E(0x12, ADC,  M_r,   M_r)
    E(0x13, ADC,  M_r,   M_Ir)
    E(0x14, ADC,  M_R,   M_R)
    E(0x15, ADC,  M_R,   M_IR)
    E(0x16, ADC,  M_R,   M_IM)
    E(0x17, ADC,  M_IR,  M_IM)
    E(0x22, SUB,  M_r,   M_r)
    E(0x23, SUB,  M_r,   M_Ir)
    E(0x24, SUB,  M_R,   M_R)
    E(0x25, SUB,  M_R,   M_IR)
    E(0x26, SUB,  M_R,   M_IM)
    E(0x27, SUB,  M_IR,  M_IM)
    E(0x32, SBC,  M_r,   M_r)
    E(0x33, SBC,  M_r,   M_Ir)
    E(0x34, SBC,  M_R,   M_R)
    E(0x35, SBC,  M_R,   M_IR)
    E(0x36, SBC,  M_R,   M_IM)
    E(0x37, SBC,  M_IR,  M_IM)
    E(0x42, OR,   M_r,   M_r)
    E(0x43, OR,   M_r,   M_Ir)
    E(0x44, OR,   M_R,   M_R)
    E(0x45, OR,   M_R,   M_IR)
    E(0x46, OR,   M_R,   M_IM)
    E(0x47, OR,   M_IR,  M_IM)
    E(0x52, AND,  M_r,   M_r)
    E(0x53, AND,  M_r,   M_Ir)
    E(0x54, AND,  M_R,   M_R)
    E(0x55, AND,  M_R,   M_IR)
    E(0x56, AND,  M_R,   M_IM)
    E(0x57, AND,  M_IR,  M_IM)
    E(0x62, TCM,  M_r,   M_r)
    E(0x63, TCM,  M_r,   M_Ir)
    E(0x64, TCM,  M_R,   M_R)
    E(0x65, TCM,  M_R,   M_IR)
    E(0x66, TCM,  M_R,   M_IM)
    E(0x67, TCM,  M_IR,  M_IM)
    E(0x72, TM,   M_r,   M_r)
    E(0x73, TM,   M_r,   M_Ir)
    E(0x74, TM,   M_R,   M_R)
    E(0x75, TM,   M_R,   M_IR)
    E(0x76, TM,   M_R,   M_IM)
    E(0x77, TM,   M_IR,  M_IM)
    E(0xA2, CP,   M_r,   M_r)
    E(0xA3, CP,   M_r,   M_Ir)
    E(0xA4, CP,   M_R,   M_R)
    E(0xA5, CP,   M_R,   M_IR)
    E(0xA6, CP,   M_R,   M_IM)
    E(0xA7, CP,   M_IR,  M_IM)
    E(0xB2, XOR,  M_r,   M_r)
    E(0xB3, XOR,  M_r,   M_Ir)
    E(0xB4, XOR,  M_R,   M_R)
    E(0xB5, XOR,  M_R,   M_IR)
    E(0xB6, XOR,  M_R,   M_IM)
    E(0xB7, XOR,  M_IR,  M_IM)
    E(0x00, DEC,  M_R,   M_NO)
    E(0x01, DEC,  M_IR,  M_NO)
    E(0x10, RLC,  M_R,   M_NO)
    E(0x11, RLC,  M_IR,  M_NO)
    E(0x0E, INC,  M_r,   M_NO)
    E(0x20, INC,  M_R,   M_NO)
    E(0x21, INC,  M_IR,  M_NO)
    E(0x40, DA,   M_R,   M_NO)
    E(0x41, DA,   M_IR,  M_NO)
    E(0x50, POP,  M_R,   M_NO)
    E(0x51, POP,  M_IR,  M_NO)
    E(0x60, COM,  M_R,   M_NO)
    E(0x61, COM,  M_IR,  M_NO)
    E(0x70, PUSH, M_R,   M_NO)
    E(0x71, PUSH, M_IR,  M_NO)
    E(0x80, DECW, M_R,   M_NO)
    E(0x81, DECW, M_IR,  M_NO)
    E(0x90, RL,   M_R,   M_NO)
    E(0x91, RL,   M_IR,  M_NO)
    E(0xA0, INCW, M_R,   M_NO)
    E(0xA1, INCW, M_IR,  M_NO)
    E(0xB0, CLR,  M_R,   M_NO)
    E(0xB1, CLR,  M_IR,  M_NO)
    E(0xC0, RRC,  M_R,   M_NO)
    E(0xC1, RRC,  M_IR,  M_NO)
    E(0xD0, SRA,  M_R,   M_NO)
    E(0xD1, SRA,  M_IR,  M_NO)
    E(0xE0, RR,   M_R,   M_NO)
    E(0xE1, RR,   M_IR,  M_NO)
    E(0xF0, SWAP, M_R,   M_NO)
    E(0xF1, SWAP, M_IR,  M_NO)
    E(0x6F, STOP, M_NO,  M_NO)
    E(0x7F, HALT, M_NO,  M_NO)
    E(0x8F, DI,   M_NO,  M_NO)
    E(0x9F, EI,   M_NO,  M_NO)
    E(0xAF, RET,  M_NO,  M_NO)
    E(0xBF, IRET, M_NO,  M_NO)
    E(0xCF, RCF,  M_NO,  M_NO)
    E(0xDF, SCF,  M_NO,  M_NO)
    E(0xEF, CCF,  M_NO,  M_NO)
    E(0xFF, NOP,  M_NO,  M_NO)
};

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table) return true;
    if (opr == M_w)
        return table == M_r || table == M_R || table == M_DA || table == M_RA;
    if (opr == M_R) return table == M_DA || table == M_RA;
    if (opr == M_DA) return table == M_RA;
    if (opr == M_r) return table == M_R;
    if (opr == M_Iw) return table == M_Ir || table == M_IR || table == M_IRR;
    if (opr == M_Ir) return table == M_IR;
    if (opr == M_IR) return table == M_IRR;
    if (opr == M_Irr) return table == M_IRR;
    if (opr == M_Iww) return table == M_IR || table == M_Ir
                          || table == M_IRR || table == M_Irr;
    return false;
}

static bool acceptModes(uint8_t flags, const Entry *entry) {
    const uint8_t table = pgm_read_word(&entry->flags);
    return acceptMode(Entry::_dstMode(flags), Entry::_dstMode(table))
        && acceptMode(Entry::_srcMode(flags), Entry::_srcMode(table));
}

Error TableZ8::searchName(InsnZ8 &insn) const {
    uint8_t count = 0;
    const uint8_t flags = Entry::_flags(insn.dstMode(), insn.srcMode());
    const Entry *entry = TableBase::searchName<Entry,uint8_t>(
        insn.name(), flags, ARRAY_RANGE(TABLE_Z8), acceptModes, count);
    if (entry) {
        insn.setOpCode(pgm_read_byte(&entry->opCode));
        insn.setFlags(pgm_read_byte(&entry->flags));
        return _error.setOK();
    }
    return _error.setError(count == 0 ? UNKNOWN_INSTRUCTION : UNKNOWN_OPERAND);
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
        const char *name =
            reinterpret_cast<const char *>(pgm_read_ptr(&entry->name));
        TableBase::setName(insn.insn(), name, Config::NAME_MAX);
        return _error.setOK();
    }
    return _error.setError(UNKNOWN_INSTRUCTION);
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
