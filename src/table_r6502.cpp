#include "config_r65c02.h"

#include "entry_r65c02.h"
#include "table_r65c02.h"
#include "text_r6502.h"

static constexpr Entry TABLE[] PROGMEM = {
    E(0x00, BRK, R6502, IMPLIED)
    E(0x40, RTI, R6502, IMPLIED)
    E(0x60, RTS, R6502, IMPLIED)
    E(0xEA, NOP, R6502, IMPLIED)
    E(0x08, PHP, R6502, IMPLIED)
    E(0x28, PLP, R6502, IMPLIED)
    E(0x48, PHA, R6502, IMPLIED)
    E(0x68, PLA, R6502, IMPLIED)
    E(0x88, DEY, R6502, IMPLIED)
    E(0xC8, INY, R6502, IMPLIED)
    E(0xCA, DEX, R6502, IMPLIED)
    E(0xE8, INX, R6502, IMPLIED)
    E(0x98, TYA, R6502, IMPLIED)
    E(0xA8, TAY, R6502, IMPLIED)
    E(0x8A, TXA, R6502, IMPLIED)
    E(0xAA, TAX, R6502, IMPLIED)
    E(0x9A, TXS, R6502, IMPLIED)
    E(0xBA, TSX, R6502, IMPLIED)
    E(0x18, CLC, R6502, IMPLIED)
    E(0x38, SEC, R6502, IMPLIED)
    E(0x58, CLI, R6502, IMPLIED)
    E(0x78, SEI, R6502, IMPLIED)
    E(0xB8, CLV, R6502, IMPLIED)
    E(0xD8, CLD, R6502, IMPLIED)
    E(0xF8, SED, R6502, IMPLIED)
    E(0x0A, ASL, R6502, ACCUMULATOR)
    E(0x2A, ROL, R6502, ACCUMULATOR)
    E(0x4A, LSR, R6502, ACCUMULATOR)
    E(0x6A, ROR, R6502, ACCUMULATOR)
    E(0xA0, LDY, R6502, IMMEDIATE)
    E(0xA2, LDX, R6502, IMMEDIATE)
    E(0xC0, CPY, R6502, IMMEDIATE)
    E(0xE0, CPX, R6502, IMMEDIATE)
    E(0x09, ORA, R6502, IMMEDIATE)
    E(0x29, AND, R6502, IMMEDIATE)
    E(0x49, EOR, R6502, IMMEDIATE)
    E(0x69, ADC, R6502, IMMEDIATE)
    E(0xA9, LDA, R6502, IMMEDIATE)
    E(0xC9, CMP, R6502, IMMEDIATE)
    E(0xE9, SBC, R6502, IMMEDIATE)
    E(0x24, BIT, R6502, ZEROPAGE)
    E(0x05, ORA, R6502, ZEROPAGE)
    E(0x25, AND, R6502, ZEROPAGE)
    E(0x45, EOR, R6502, ZEROPAGE)
    E(0x65, ADC, R6502, ZEROPAGE)
    E(0x85, STA, R6502, ZEROPAGE)
    E(0xA5, LDA, R6502, ZEROPAGE)
    E(0xC5, CMP, R6502, ZEROPAGE)
    E(0xE5, SBC, R6502, ZEROPAGE)
    E(0x84, STY, R6502, ZEROPAGE)
    E(0xA4, LDY, R6502, ZEROPAGE)
    E(0xC4, CPY, R6502, ZEROPAGE)
    E(0x86, STX, R6502, ZEROPAGE)
    E(0xA6, LDX, R6502, ZEROPAGE)
    E(0xE4, CPX, R6502, ZEROPAGE)
    E(0xC6, DEC, R6502, ZEROPAGE)
    E(0xE6, INC, R6502, ZEROPAGE)
    E(0x06, ASL, R6502, ZEROPAGE)
    E(0x26, ROL, R6502, ZEROPAGE)
    E(0x46, LSR, R6502, ZEROPAGE)
    E(0x66, ROR, R6502, ZEROPAGE)
    E(0x15, ORA, R6502, ZP_IDX_X)
    E(0x35, AND, R6502, ZP_IDX_X)
    E(0x55, EOR, R6502, ZP_IDX_X)
    E(0x75, ADC, R6502, ZP_IDX_X)
    E(0x95, STA, R6502, ZP_IDX_X)
    E(0xB5, LDA, R6502, ZP_IDX_X)
    E(0xD5, CMP, R6502, ZP_IDX_X)
    E(0xF5, SBC, R6502, ZP_IDX_X)
    E(0x94, STY, R6502, ZP_IDX_X)
    E(0xB4, LDY, R6502, ZP_IDX_X)
    E(0x96, STX, R6502, ZP_IDX_Y)
    E(0xB6, LDX, R6502, ZP_IDX_Y)
    E(0xD6, DEC, R6502, ZP_IDX_X)
    E(0xF6, INC, R6502, ZP_IDX_X)
    E(0x16, ASL, R6502, ZP_IDX_X)
    E(0x36, ROL, R6502, ZP_IDX_X)
    E(0x56, LSR, R6502, ZP_IDX_X)
    E(0x76, ROR, R6502, ZP_IDX_X)
    E(0x2C, BIT, R6502, ABSOLUTE)
    E(0x0D, ORA, R6502, ABSOLUTE)
    E(0x2D, AND, R6502, ABSOLUTE)
    E(0x4D, EOR, R6502, ABSOLUTE)
    E(0x6D, ADC, R6502, ABSOLUTE)
    E(0x8D, STA, R6502, ABSOLUTE)
    E(0xAD, LDA, R6502, ABSOLUTE)
    E(0xCD, CMP, R6502, ABSOLUTE)
    E(0xED, SBC, R6502, ABSOLUTE)
    E(0x8C, STY, R6502, ABSOLUTE)
    E(0xAC, LDY, R6502, ABSOLUTE)
    E(0xCC, CPY, R6502, ABSOLUTE)
    E(0x8E, STX, R6502, ABSOLUTE)
    E(0xAE, LDX, R6502, ABSOLUTE)
    E(0xEC, CPX, R6502, ABSOLUTE)
    E(0xCE, DEC, R6502, ABSOLUTE)
    E(0xEE, INC, R6502, ABSOLUTE)
    E(0x0E, ASL, R6502, ABSOLUTE)
    E(0x2E, ROL, R6502, ABSOLUTE)
    E(0x4E, LSR, R6502, ABSOLUTE)
    E(0x6E, ROR, R6502, ABSOLUTE)
    E(0x4C, JMP, R6502, ABSOLUTE)
    E(0x20, JSR, R6502, ABSOLUTE)
    E(0x1D, ORA, R6502, ABS_IDX_X)
    E(0x3D, AND, R6502, ABS_IDX_X)
    E(0x5D, EOR, R6502, ABS_IDX_X)
    E(0x7D, ADC, R6502, ABS_IDX_X)
    E(0x9D, STA, R6502, ABS_IDX_X)
    E(0xBD, LDA, R6502, ABS_IDX_X)
    E(0xDD, CMP, R6502, ABS_IDX_X)
    E(0xFD, SBC, R6502, ABS_IDX_X)
    E(0x19, ORA, R6502, ABS_IDX_Y)
    E(0x39, AND, R6502, ABS_IDX_Y)
    E(0x59, EOR, R6502, ABS_IDX_Y)
    E(0x79, ADC, R6502, ABS_IDX_Y)
    E(0x99, STA, R6502, ABS_IDX_Y)
    E(0xB9, LDA, R6502, ABS_IDX_Y)
    E(0xD9, CMP, R6502, ABS_IDX_Y)
    E(0xF9, SBC, R6502, ABS_IDX_Y)
    E(0xBC, LDY, R6502, ABS_IDX_X)
    E(0xBE, LDX, R6502, ABS_IDX_Y)
    E(0xDE, DEC, R6502, ABS_IDX_X)
    E(0xFE, INC, R6502, ABS_IDX_X)
    E(0x1E, ASL, R6502, ABS_IDX_X)
    E(0x3E, ROL, R6502, ABS_IDX_X)
    E(0x5E, LSR, R6502, ABS_IDX_X)
    E(0x7E, ROR, R6502, ABS_IDX_X)
    E(0x6C, JMP, R6502, ABS_INDIRECT)
    E(0x01, ORA, R6502, INDX_IND)
    E(0x21, AND, R6502, INDX_IND)
    E(0x41, EOR, R6502, INDX_IND)
    E(0x61, ADC, R6502, INDX_IND)
    E(0x81, STA, R6502, INDX_IND)
    E(0xA1, LDA, R6502, INDX_IND)
    E(0xC1, CMP, R6502, INDX_IND)
    E(0xE1, SBC, R6502, INDX_IND)
    E(0x11, ORA, R6502, INDIRECT_IDX)
    E(0x31, AND, R6502, INDIRECT_IDX)
    E(0x51, EOR, R6502, INDIRECT_IDX)
    E(0x71, ADC, R6502, INDIRECT_IDX)
    E(0x91, STA, R6502, INDIRECT_IDX)
    E(0xB1, LDA, R6502, INDIRECT_IDX)
    E(0xD1, CMP, R6502, INDIRECT_IDX)
    E(0xF1, SBC, R6502, INDIRECT_IDX)
    E(0x10, BPL, R6502, REL8)
    E(0x30, BMI, R6502, REL8)
    E(0x50, BVC, R6502, REL8)
    E(0x70, BVS, R6502, REL8)
    E(0x90, BCC, R6502, REL8)
    E(0xB0, BCS, R6502, REL8)
    E(0xD0, BNE, R6502, REL8)
    E(0xF0, BEQ, R6502, REL8)
};

const Entry *TableR65c02Base::searchEntry(
    const char *name,
    const Entry *table, const Entry *end) {
    for (const Entry *entry = table; entry < end; entry++) {
        if (pgm_strcasecmp(name, entry->name) == 0)
            return entry;
    }
    return nullptr;
}

const Entry *TableR65c02Base::searchEntry(
    const target::insn_t insnCode,
    const Entry *table, const Entry *end) {
    for (const Entry *entry = table; entry < end; entry++) {
        if (insnCode == pgm_read_byte(&entry->insnCode))
            return entry;
    }
    return nullptr;
}

Error TableR65c02Base::searchPages(
    Insn &insn, const char *name, const Entry *table, const Entry *end) {
    const Entry *entry = searchEntry(name, table, end);
    if (entry) {
        insn.setInsnCode(pgm_read_byte(&entry->insnCode));
        insn.setFlags(pgm_read_byte(&entry->flags));
        return OK;
    }
    return UNKNOWN_INSTRUCTION;
}

static bool acceptAddrMode(AddrMode opr, AddrMode table) {
    if (opr == table) return true;
    if (opr == INDX_IND) return table == IDX_ABS_IND;
    if (opr == ABS_INDIRECT) return table == ZP_INDIRECT;
    if (opr == ZP_IDX_X) return table == ABS_IDX_X;
    if (opr == ZP_IDX_Y) return table == ABS_IDX_Y;
    return false;
}

Error TableR65c02Base::searchPages(
    Insn &insn, const char *name, AddrMode addrMode, const Entry *table, const Entry *end) {
    for (const Entry *entry = table; entry < end
             && (entry = searchEntry(name, entry, end)) != nullptr; entry++) {
        const host::uint_t flags = pgm_read_byte(&entry->flags);
        if (acceptAddrMode(addrMode, _addrMode(flags))) {
            insn.setInsnCode(pgm_read_byte(&entry->insnCode));
            insn.setFlags(flags);
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

Error TableR65c02Base::searchPages(
    Insn &insn, target::insn_t insnCode, const Entry *table, const Entry *end) {
    const Entry *entry = searchEntry(insnCode, table, end);
    if (entry) {
        insn.setFlags(pgm_read_byte(&entry->flags));
        char name[8];
        pgm_strcpy(name, entry->name);
        insn.setName(name);
        return OK;
    }
    return UNKNOWN_INSTRUCTION;
}

template<>
Error TableR65c02<R6502>::searchName(Insn &insn) const {
    if (searchPages(insn, insn.name(), ARRAY_RANGE(TABLE)) == OK)
        return OK;
    return UNKNOWN_INSTRUCTION;
}

template<>
Error TableR65c02<R6502>::searchNameAndAddrMode(Insn &insn) const {
    if (searchPages(insn, insn.name(), insn.addrMode(), ARRAY_RANGE(TABLE)) == OK)
        return OK;
    return UNKNOWN_INSTRUCTION;
}

template<>
Error TableR65c02<R6502>::searchInsnCode(Insn &insn) const {
    if (searchPages(insn, insn.insnCode(), ARRAY_RANGE(TABLE)) == OK)
        return OK;
    return UNKNOWN_INSTRUCTION;
}

static const TableR65c02<R6502> R6502Table;

template<>
const TableR65c02<R6502> *TableR65c02<R6502>::table() {
    return &R6502Table;
}
