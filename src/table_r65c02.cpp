#include "config_r65c02.h"

#include "entry_r65c02.h"
#include "table_r65c02.h"
#include "text_r65c02.h"

static constexpr Entry TABLE[] PROGMEM = {
    E(0x5A, PHY, R65C02, IMPLIED)
    E(0x7A, PLY, R65C02, IMPLIED)
    E(0xDA, PHX, R65C02, IMPLIED)
    E(0xFA, PLX, R65C02, IMPLIED)
    E(0x1A, INC, R65C02, ACCUMULATOR)
    E(0x3A, DEC, R65C02, ACCUMULATOR)
    E(0x89, BIT, R65C02, IMMEDIATE)
    E(0x04, TSB, R65C02, ZEROPAGE)
    E(0x14, TRB, R65C02, ZEROPAGE)
    E(0x64, STZ, R65C02, ZEROPAGE)
    E(0x34, BIT, R65C02, ZP_IDX_X)
    E(0x74, STZ, R65C02, ZP_IDX_X)
    E(0x0C, TSB, R65C02, ABSOLUTE)
    E(0x1C, TRB, R65C02, ABSOLUTE)
    E(0x9C, STZ, R65C02, ABSOLUTE)
    E(0x3C, BIT, R65C02, ABS_IDX_X)
    E(0x9E, STZ, R65C02, ABS_IDX_X)
    E(0x7C, JMP, R65C02, IDX_ABS_IND)
    E(0x12, ORA, R65C02, ZP_INDIRECT)
    E(0x32, AND, R65C02, ZP_INDIRECT)
    E(0x52, EOR, R65C02, ZP_INDIRECT)
    E(0x72, ADC, R65C02, ZP_INDIRECT)
    E(0x92, STA, R65C02, ZP_INDIRECT)
    E(0xB2, LDA, R65C02, ZP_INDIRECT)
    E(0xD2, CMP, R65C02, ZP_INDIRECT)
    E(0xF2, SBC, R65C02, ZP_INDIRECT)
    E(0x80, BRA, R65C02, RELATIVE)
    E(0x07, RMB0, R65C02, ZEROPAGE)
    E(0x17, RMB1, R65C02, ZEROPAGE)
    E(0x27, RMB2, R65C02, ZEROPAGE)
    E(0x37, RMB3, R65C02, ZEROPAGE)
    E(0x47, RMB4, R65C02, ZEROPAGE)
    E(0x57, RMB5, R65C02, ZEROPAGE)
    E(0x67, RMB6, R65C02, ZEROPAGE)
    E(0x77, RMB7, R65C02, ZEROPAGE)
    E(0x87, SMB0, R65C02, ZEROPAGE)
    E(0x97, SMB1, R65C02, ZEROPAGE)
    E(0xA7, SMB2, R65C02, ZEROPAGE)
    E(0xB7, SMB3, R65C02, ZEROPAGE)
    E(0xC7, SMB4, R65C02, ZEROPAGE)
    E(0xD7, SMB5, R65C02, ZEROPAGE)
    E(0xE7, SMB6, R65C02, ZEROPAGE)
    E(0xF7, SMB7, R65C02, ZEROPAGE)
    E(0x0F, BBR0, R65C02, ZP_RELATIVE)
    E(0x1F, BBR1, R65C02, ZP_RELATIVE)
    E(0x2F, BBR2, R65C02, ZP_RELATIVE)
    E(0x3F, BBR3, R65C02, ZP_RELATIVE)
    E(0x4F, BBR4, R65C02, ZP_RELATIVE)
    E(0x5F, BBR5, R65C02, ZP_RELATIVE)
    E(0x6F, BBR6, R65C02, ZP_RELATIVE)
    E(0x7F, BBR7, R65C02, ZP_RELATIVE)
    E(0x8F, BBS0, R65C02, ZP_RELATIVE)
    E(0x9F, BBS1, R65C02, ZP_RELATIVE)
    E(0xAF, BBS2, R65C02, ZP_RELATIVE)
    E(0xBF, BBS3, R65C02, ZP_RELATIVE)
    E(0xCF, BBS4, R65C02, ZP_RELATIVE)
    E(0xDF, BBS5, R65C02, ZP_RELATIVE)
    E(0xEF, BBS6, R65C02, ZP_RELATIVE)
    E(0xFF, BBS7, R65C02, ZP_RELATIVE)
};

template<>
Error InsnTable<R65C02>::searchName(Insn &insn) const {
    if (InsnTable<R6502>::table()->searchName(insn) == OK)
        return OK;
    if (searchPages(insn, insn.name(), ARRAY_RANGE(TABLE)) == OK)
        return OK;
    return UNKNOWN_INSTRUCTION;
}

template<>
Error InsnTable<R65C02>::searchNameAndAddrMode(Insn &insn) const {
    if (InsnTable<R6502>::table()->searchNameAndAddrMode(insn) == OK)
        return OK;
    if (searchPages(insn, insn.name(), insn.addrMode(), ARRAY_RANGE(TABLE)) == OK)
        return OK;
    return UNKNOWN_INSTRUCTION;
}

template<>
Error InsnTable<R65C02>::searchInsnCode(Insn &insn) const {
    if (InsnTable<R6502>::table()->searchInsnCode(insn) == OK)
        return OK;
    if (searchPages(insn, insn.insnCode(), ARRAY_RANGE(TABLE)) == OK)
        return OK;
    return UNKNOWN_INSTRUCTION;
}

static const InsnTable<R65C02> R65C02Table;

template<>
const InsnTable<R65C02> *InsnTable<R65C02>::table() {
    return &R65C02Table;
}
