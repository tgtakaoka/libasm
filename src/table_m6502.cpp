#include "config_m6502.h"

#include "entry_m6502.h"
#include "table_m6502.h"
#include "text_m6502.h"

static constexpr Entry M6502_TABLE[] PROGMEM = {
    E(0x00, BRK, M6502, IMPLIED)
    E(0x40, RTI, M6502, IMPLIED)
    E(0x60, RTS, M6502, IMPLIED)
    E(0xEA, NOP, M6502, IMPLIED)
    E(0x08, PHP, M6502, IMPLIED)
    E(0x28, PLP, M6502, IMPLIED)
    E(0x48, PHA, M6502, IMPLIED)
    E(0x68, PLA, M6502, IMPLIED)
    E(0x88, DEY, M6502, IMPLIED)
    E(0xC8, INY, M6502, IMPLIED)
    E(0xCA, DEX, M6502, IMPLIED)
    E(0xE8, INX, M6502, IMPLIED)
    E(0x98, TYA, M6502, IMPLIED)
    E(0xA8, TAY, M6502, IMPLIED)
    E(0x8A, TXA, M6502, IMPLIED)
    E(0xAA, TAX, M6502, IMPLIED)
    E(0x9A, TXS, M6502, IMPLIED)
    E(0xBA, TSX, M6502, IMPLIED)
    E(0x18, CLC, M6502, IMPLIED)
    E(0x38, SEC, M6502, IMPLIED)
    E(0x58, CLI, M6502, IMPLIED)
    E(0x78, SEI, M6502, IMPLIED)
    E(0xB8, CLV, M6502, IMPLIED)
    E(0xD8, CLD, M6502, IMPLIED)
    E(0xF8, SED, M6502, IMPLIED)
    E(0x0A, ASL, M6502, ACCUMULATOR)
    E(0x2A, ROL, M6502, ACCUMULATOR)
    E(0x4A, LSR, M6502, ACCUMULATOR)
    E(0x6A, ROR, M6502, ACCUMULATOR)
    E(0xA0, LDY, M6502, IMMEDIATE)
    E(0xA2, LDX, M6502, IMMEDIATE)
    E(0xC0, CPY, M6502, IMMEDIATE)
    E(0xE0, CPX, M6502, IMMEDIATE)
    E(0x09, ORA, M6502, IMMEDIATE)
    E(0x29, AND, M6502, IMMEDIATE)
    E(0x49, EOR, M6502, IMMEDIATE)
    E(0x69, ADC, M6502, IMMEDIATE)
    E(0xA9, LDA, M6502, IMMEDIATE)
    E(0xC9, CMP, M6502, IMMEDIATE)
    E(0xE9, SBC, M6502, IMMEDIATE)
    E(0x24, BIT, M6502, ZEROPAGE)
    E(0x05, ORA, M6502, ZEROPAGE)
    E(0x25, AND, M6502, ZEROPAGE)
    E(0x45, EOR, M6502, ZEROPAGE)
    E(0x65, ADC, M6502, ZEROPAGE)
    E(0x85, STA, M6502, ZEROPAGE)
    E(0xA5, LDA, M6502, ZEROPAGE)
    E(0xC5, CMP, M6502, ZEROPAGE)
    E(0xE5, SBC, M6502, ZEROPAGE)
    E(0x84, STY, M6502, ZEROPAGE)
    E(0xA4, LDY, M6502, ZEROPAGE)
    E(0xC4, CPY, M6502, ZEROPAGE)
    E(0x86, STX, M6502, ZEROPAGE)
    E(0xA6, LDX, M6502, ZEROPAGE)
    E(0xE4, CPX, M6502, ZEROPAGE)
    E(0xC6, DEC, M6502, ZEROPAGE)
    E(0xE6, INC, M6502, ZEROPAGE)
    E(0x06, ASL, M6502, ZEROPAGE)
    E(0x26, ROL, M6502, ZEROPAGE)
    E(0x46, LSR, M6502, ZEROPAGE)
    E(0x66, ROR, M6502, ZEROPAGE)
    E(0x15, ORA, M6502, ZP_IDX_X)
    E(0x35, AND, M6502, ZP_IDX_X)
    E(0x55, EOR, M6502, ZP_IDX_X)
    E(0x75, ADC, M6502, ZP_IDX_X)
    E(0x95, STA, M6502, ZP_IDX_X)
    E(0xB5, LDA, M6502, ZP_IDX_X)
    E(0xD5, CMP, M6502, ZP_IDX_X)
    E(0xF5, SBC, M6502, ZP_IDX_X)
    E(0x94, STY, M6502, ZP_IDX_X)
    E(0xB4, LDY, M6502, ZP_IDX_X)
    E(0x96, STX, M6502, ZP_IDX_Y)
    E(0xB6, LDX, M6502, ZP_IDX_Y)
    E(0xD6, DEC, M6502, ZP_IDX_X)
    E(0xF6, INC, M6502, ZP_IDX_X)
    E(0x16, ASL, M6502, ZP_IDX_X)
    E(0x36, ROL, M6502, ZP_IDX_X)
    E(0x56, LSR, M6502, ZP_IDX_X)
    E(0x76, ROR, M6502, ZP_IDX_X)
    E(0x2C, BIT, M6502, ABSOLUTE)
    E(0x0D, ORA, M6502, ABSOLUTE)
    E(0x2D, AND, M6502, ABSOLUTE)
    E(0x4D, EOR, M6502, ABSOLUTE)
    E(0x6D, ADC, M6502, ABSOLUTE)
    E(0x8D, STA, M6502, ABSOLUTE)
    E(0xAD, LDA, M6502, ABSOLUTE)
    E(0xCD, CMP, M6502, ABSOLUTE)
    E(0xED, SBC, M6502, ABSOLUTE)
    E(0x8C, STY, M6502, ABSOLUTE)
    E(0xAC, LDY, M6502, ABSOLUTE)
    E(0xCC, CPY, M6502, ABSOLUTE)
    E(0x8E, STX, M6502, ABSOLUTE)
    E(0xAE, LDX, M6502, ABSOLUTE)
    E(0xEC, CPX, M6502, ABSOLUTE)
    E(0xCE, DEC, M6502, ABSOLUTE)
    E(0xEE, INC, M6502, ABSOLUTE)
    E(0x0E, ASL, M6502, ABSOLUTE)
    E(0x2E, ROL, M6502, ABSOLUTE)
    E(0x4E, LSR, M6502, ABSOLUTE)
    E(0x6E, ROR, M6502, ABSOLUTE)
    E(0x4C, JMP, M6502, ABSOLUTE)
    E(0x20, JSR, M6502, ABSOLUTE)
    E(0x1D, ORA, M6502, ABS_IDX_X)
    E(0x3D, AND, M6502, ABS_IDX_X)
    E(0x5D, EOR, M6502, ABS_IDX_X)
    E(0x7D, ADC, M6502, ABS_IDX_X)
    E(0x9D, STA, M6502, ABS_IDX_X)
    E(0xBD, LDA, M6502, ABS_IDX_X)
    E(0xDD, CMP, M6502, ABS_IDX_X)
    E(0xFD, SBC, M6502, ABS_IDX_X)
    E(0x19, ORA, M6502, ABS_IDX_Y)
    E(0x39, AND, M6502, ABS_IDX_Y)
    E(0x59, EOR, M6502, ABS_IDX_Y)
    E(0x79, ADC, M6502, ABS_IDX_Y)
    E(0x99, STA, M6502, ABS_IDX_Y)
    E(0xB9, LDA, M6502, ABS_IDX_Y)
    E(0xD9, CMP, M6502, ABS_IDX_Y)
    E(0xF9, SBC, M6502, ABS_IDX_Y)
    E(0xBC, LDY, M6502, ABS_IDX_X)
    E(0xBE, LDX, M6502, ABS_IDX_Y)
    E(0xDE, DEC, M6502, ABS_IDX_X)
    E(0xFE, INC, M6502, ABS_IDX_X)
    E(0x1E, ASL, M6502, ABS_IDX_X)
    E(0x3E, ROL, M6502, ABS_IDX_X)
    E(0x5E, LSR, M6502, ABS_IDX_X)
    E(0x7E, ROR, M6502, ABS_IDX_X)
    E(0x6C, JMP, M6502, ABS_INDIRECT)
    E(0x01, ORA, M6502, INDX_IND)
    E(0x21, AND, M6502, INDX_IND)
    E(0x41, EOR, M6502, INDX_IND)
    E(0x61, ADC, M6502, INDX_IND)
    E(0x81, STA, M6502, INDX_IND)
    E(0xA1, LDA, M6502, INDX_IND)
    E(0xC1, CMP, M6502, INDX_IND)
    E(0xE1, SBC, M6502, INDX_IND)
    E(0x11, ORA, M6502, INDIRECT_IDX)
    E(0x31, AND, M6502, INDIRECT_IDX)
    E(0x51, EOR, M6502, INDIRECT_IDX)
    E(0x71, ADC, M6502, INDIRECT_IDX)
    E(0x91, STA, M6502, INDIRECT_IDX)
    E(0xB1, LDA, M6502, INDIRECT_IDX)
    E(0xD1, CMP, M6502, INDIRECT_IDX)
    E(0xF1, SBC, M6502, INDIRECT_IDX)
    E(0x10, BPL, M6502, REL8)
    E(0x30, BMI, M6502, REL8)
    E(0x50, BVC, M6502, REL8)
    E(0x70, BVS, M6502, REL8)
    E(0x90, BCC, M6502, REL8)
    E(0xB0, BCS, M6502, REL8)
    E(0xD0, BNE, M6502, REL8)
    E(0xF0, BEQ, M6502, REL8)
};

static constexpr Entry W65C02_TABLE[] PROGMEM = {
    E(0x5A, PHY, W65C02, IMPLIED)
    E(0x7A, PLY, W65C02, IMPLIED)
    E(0xDA, PHX, W65C02, IMPLIED)
    E(0xFA, PLX, W65C02, IMPLIED)
    E(0x1A, INC, W65C02, ACCUMULATOR)
    E(0x3A, DEC, W65C02, ACCUMULATOR)
    E(0x89, BIT, W65C02, IMMEDIATE)
    E(0x04, TSB, W65C02, ZEROPAGE)
    E(0x14, TRB, W65C02, ZEROPAGE)
    E(0x64, STZ, W65C02, ZEROPAGE)
    E(0x34, BIT, W65C02, ZP_IDX_X)
    E(0x74, STZ, W65C02, ZP_IDX_X)
    E(0x0C, TSB, W65C02, ABSOLUTE)
    E(0x1C, TRB, W65C02, ABSOLUTE)
    E(0x9C, STZ, W65C02, ABSOLUTE)
    E(0x3C, BIT, W65C02, ABS_IDX_X)
    E(0x9E, STZ, W65C02, ABS_IDX_X)
    E(0x7C, JMP, W65C02, IDX_ABS_IND)
    E(0x12, ORA, W65C02, ZP_INDIRECT)
    E(0x32, AND, W65C02, ZP_INDIRECT)
    E(0x52, EOR, W65C02, ZP_INDIRECT)
    E(0x72, ADC, W65C02, ZP_INDIRECT)
    E(0x92, STA, W65C02, ZP_INDIRECT)
    E(0xB2, LDA, W65C02, ZP_INDIRECT)
    E(0xD2, CMP, W65C02, ZP_INDIRECT)
    E(0xF2, SBC, W65C02, ZP_INDIRECT)
    E(0x80, BRA, W65C02, REL8)
#ifdef W65C02_ENABLE_BITOPS
    E(0x07, RMB0, W65C02, ZEROPAGE)
    E(0x17, RMB1, W65C02, ZEROPAGE)
    E(0x27, RMB2, W65C02, ZEROPAGE)
    E(0x37, RMB3, W65C02, ZEROPAGE)
    E(0x47, RMB4, W65C02, ZEROPAGE)
    E(0x57, RMB5, W65C02, ZEROPAGE)
    E(0x67, RMB6, W65C02, ZEROPAGE)
    E(0x77, RMB7, W65C02, ZEROPAGE)
    E(0x87, SMB0, W65C02, ZEROPAGE)
    E(0x97, SMB1, W65C02, ZEROPAGE)
    E(0xA7, SMB2, W65C02, ZEROPAGE)
    E(0xB7, SMB3, W65C02, ZEROPAGE)
    E(0xC7, SMB4, W65C02, ZEROPAGE)
    E(0xD7, SMB5, W65C02, ZEROPAGE)
    E(0xE7, SMB6, W65C02, ZEROPAGE)
    E(0xF7, SMB7, W65C02, ZEROPAGE)
    E(0x0F, BBR0, W65C02, ZP_REL8)
    E(0x1F, BBR1, W65C02, ZP_REL8)
    E(0x2F, BBR2, W65C02, ZP_REL8)
    E(0x3F, BBR3, W65C02, ZP_REL8)
    E(0x4F, BBR4, W65C02, ZP_REL8)
    E(0x5F, BBR5, W65C02, ZP_REL8)
    E(0x6F, BBR6, W65C02, ZP_REL8)
    E(0x7F, BBR7, W65C02, ZP_REL8)
    E(0x8F, BBS0, W65C02, ZP_REL8)
    E(0x9F, BBS1, W65C02, ZP_REL8)
    E(0xAF, BBS2, W65C02, ZP_REL8)
    E(0xBF, BBS3, W65C02, ZP_REL8)
    E(0xCF, BBS4, W65C02, ZP_REL8)
    E(0xDF, BBS5, W65C02, ZP_REL8)
    E(0xEF, BBS6, W65C02, ZP_REL8)
    E(0xFF, BBS7, W65C02, ZP_REL8)
#endif
};

const Entry *TableM6502::searchEntry(
    const char *name,
    const Entry *table, const Entry *end) {
    for (const Entry *entry = table; entry < end; entry++) {
        if (pgm_strcasecmp(name, entry->name) == 0)
            return entry;
    }
    return nullptr;
}

const Entry *TableM6502::searchEntry(
    const target::insn_t insnCode,
    const Entry *table, const Entry *end) {
    for (const Entry *entry = table; entry < end; entry++) {
        if (insnCode == pgm_read_byte(&entry->insnCode))
            return entry;
    }
    return nullptr;
}

Error TableM6502::searchPages(
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

Error TableM6502::searchPages(
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

Error TableM6502::searchPages(
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

Error TableM6502::searchName(Insn &insn) const {
    if (searchPages(insn, insn.name(), ARRAY_RANGE(M6502_TABLE)) == OK)
        return OK;
    if (searchPages(insn, insn.name(), ARRAY_RANGE(W65C02_TABLE)) == OK)
        return OK;
    return UNKNOWN_INSTRUCTION;
}

Error TableM6502::searchNameAndAddrMode(Insn &insn) const {
    if (searchPages(
            insn, insn.name(), insn.addrMode(), ARRAY_RANGE(M6502_TABLE)) == OK)
        return OK;
    if (searchPages(
            insn, insn.name(), insn.addrMode(), ARRAY_RANGE(W65C02_TABLE)) == OK)
        return OK;
    return UNKNOWN_INSTRUCTION;
}

Error TableM6502::searchInsnCode(Insn &insn) const {
    if (searchPages(insn, insn.insnCode(), ARRAY_RANGE(M6502_TABLE)) == OK)
        return OK;
    if (searchPages(insn, insn.insnCode(), ARRAY_RANGE(W65C02_TABLE)) == OK)
        return OK;
    return UNKNOWN_INSTRUCTION;
}

class TableM6502 TableM6502;
