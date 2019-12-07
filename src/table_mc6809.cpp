#include "config_hd6309.h"

#include "entry_hd6309.h"
#include "table_hd6309.h"
#include "text_mc6809.h"

static constexpr Entry MC6809_P00[] PROGMEM = {
    P00(0x00, NEG,   MC6809, DIRP)
    P00(0x03, COM,   MC6809, DIRP)
    P00(0x04, LSR,   MC6809, DIRP)
    P00(0x06, ROR,   MC6809, DIRP)
    P00(0x07, ASR,   MC6809, DIRP)
    P00(0x08, ASL,   MC6809, DIRP)
    P00(0x08, LSL,   MC6809, DIRP)
    P00(0x09, ROL,   MC6809, DIRP)
    P00(0x0A, DEC,   MC6809, DIRP)
    P00(0x0C, INC,   MC6809, DIRP)
    P00(0x0D, TST,   MC6809, DIRP)
    P00(0x0E, JMP,   MC6809, DIRP)
    P00(0x0F, CLR,   MC6809, DIRP)
    P00(0x12, NOP,   MC6809, INHR)
    P00(0x13, SYNC,  MC6809, INHR)
    P00(0x16, LBRA,  MC6809, REL16)
    P00(0x17, LBSR,  MC6809, REL16)
    P00(0x19, DAA,   MC6809, INHR)
    P00(0x1A, ORCC,  MC6809, IMM8)
    P00(0x1C, ANDCC, MC6809, IMM8)
    P00(0x1D, SEX,   MC6809, INHR)
    P00(0x1E, EXG,   MC6809, REGS)
    P00(0x1F, TFR,   MC6809, REGS)
    P00(0x20, BRA,   MC6809, REL8)
    P00(0x21, BRN,   MC6809, REL8)
    P00(0x22, BHI,   MC6809, REL8)
    P00(0x23, BLS,   MC6809, REL8)
    P00(0x24, BHS,   MC6809, REL8)
    P00(0x24, BCC,   MC6809, REL8)
    P00(0x25, BLO,   MC6809, REL8)
    P00(0x25, BCS,   MC6809, REL8)
    P00(0x26, BNE,   MC6809, REL8)
    P00(0x27, BEQ,   MC6809, REL8)
    P00(0x28, BVC,   MC6809, REL8)
    P00(0x29, BVS,   MC6809, REL8)
    P00(0x2A, BPL,   MC6809, REL8)
    P00(0x2B, BMI,   MC6809, REL8)
    P00(0x2C, BGE,   MC6809, REL8)
    P00(0x2D, BLT,   MC6809, REL8)
    P00(0x2E, BGT,   MC6809, REL8)
    P00(0x2F, BLE,   MC6809, REL8)
    P00(0x30, LEAX,  MC6809, INDX)
    P00(0x31, LEAY,  MC6809, INDX)
    P00(0x32, LEAS,  MC6809, INDX)
    P00(0x33, LEAU,  MC6809, INDX)
    P00(0x34, PSHS,  MC6809, STKOP)
    P00(0x35, PULS,  MC6809, STKOP)
    P00(0x36, PSHU,  MC6809, STKOP)
    P00(0x37, PULU,  MC6809, STKOP)
    P00(0x39, RTS,   MC6809, INHR)
    P00(0x3A, ABX,   MC6809, INHR)
    P00(0x3B, RTI,   MC6809, INHR)
    P00(0x3C, CWAI,  MC6809, IMM8)
    P00(0x3D, MUL,   MC6809, INHR)
    P00(0x3F, SWI,   MC6809, INHR)
    P00(0x40, NEGA,  MC6809, INHR)
    P00(0x43, COMA,  MC6809, INHR)
    P00(0x44, LSRA,  MC6809, INHR)
    P00(0x46, RORA,  MC6809, INHR)
    P00(0x47, ASRA,  MC6809, INHR)
    P00(0x48, ASLA,  MC6809, INHR)
    P00(0x48, LSLA,  MC6809, INHR)
    P00(0x49, ROLA,  MC6809, INHR)
    P00(0x4A, DECA,  MC6809, INHR)
    P00(0x4C, INCA,  MC6809, INHR)
    P00(0x4D, TSTA,  MC6809, INHR)
    P00(0x4F, CLRA,  MC6809, INHR)
    P00(0x50, NEGB,  MC6809, INHR)
    P00(0x53, COMB,  MC6809, INHR)
    P00(0x54, LSRB,  MC6809, INHR)
    P00(0x56, RORB,  MC6809, INHR)
    P00(0x57, ASRB,  MC6809, INHR)
    P00(0x58, ASLB,  MC6809, INHR)
    P00(0x58, LSLB,  MC6809, INHR)
    P00(0x59, ROLB,  MC6809, INHR)
    P00(0x5A, DECB,  MC6809, INHR)
    P00(0x5C, INCB,  MC6809, INHR)
    P00(0x5D, TSTB,  MC6809, INHR)
    P00(0x5F, CLRB,  MC6809, INHR)
    P00(0x60, NEG,   MC6809, INDX)
    P00(0x63, COM,   MC6809, INDX)
    P00(0x64, LSR,   MC6809, INDX)
    P00(0x66, ROR,   MC6809, INDX)
    P00(0x67, ASR,   MC6809, INDX)
    P00(0x68, ASL,   MC6809, INDX)
    P00(0x68, LSL,   MC6809, INDX)
    P00(0x69, ROL,   MC6809, INDX)
    P00(0x6A, DEC,   MC6809, INDX)
    P00(0x6C, INC,   MC6809, INDX)
    P00(0x6D, TST,   MC6809, INDX)
    P00(0x6E, JMP,   MC6809, INDX)
    P00(0x6F, CLR,   MC6809, INDX)
    P00(0x70, NEG,   MC6809, EXTD)
    P00(0x73, COM,   MC6809, EXTD)
    P00(0x74, LSR,   MC6809, EXTD)
    P00(0x76, ROR,   MC6809, EXTD)
    P00(0x77, ASR,   MC6809, EXTD)
    P00(0x78, ASL,   MC6809, EXTD)
    P00(0x78, LSL,   MC6809, EXTD)
    P00(0x79, ROL,   MC6809, EXTD)
    P00(0x7A, DEC,   MC6809, EXTD)
    P00(0x7C, INC,   MC6809, EXTD)
    P00(0x7D, TST,   MC6809, EXTD)
    P00(0x7E, JMP,   MC6809, EXTD)
    P00(0x7F, CLR,   MC6809, EXTD)
    P00(0x80, SUBA,  MC6809, IMM8)
    P00(0x81, CMPA,  MC6809, IMM8)
    P00(0x82, SBCA,  MC6809, IMM8)
    P00(0x83, SUBD,  MC6809, IMM16)
    P00(0x84, ANDA,  MC6809, IMM8)
    P00(0x85, BITA,  MC6809, IMM8)
    P00(0x86, LDA,   MC6809, IMM8)
    P00(0x88, EORA,  MC6809, IMM8)
    P00(0x89, ADCA,  MC6809, IMM8)
    P00(0x8A, ORA,   MC6809, IMM8)
    P00(0x8B, ADDA,  MC6809, IMM8)
    P00(0x8C, CMPX,  MC6809, IMM16)
    P00(0x8D, BSR,   MC6809, REL8)
    P00(0x8E, LDX,   MC6809, IMM16)
    P00(0x90, SUBA,  MC6809, DIRP)
    P00(0x91, CMPA,  MC6809, DIRP)
    P00(0x92, SBCA,  MC6809, DIRP)
    P00(0x93, SUBD,  MC6809, DIRP)
    P00(0x94, ANDA,  MC6809, DIRP)
    P00(0x95, BITA,  MC6809, DIRP)
    P00(0x96, LDA,   MC6809, DIRP)
    P00(0x97, STA,   MC6809, DIRP)
    P00(0x98, EORA,  MC6809, DIRP)
    P00(0x99, ADCA,  MC6809, DIRP)
    P00(0x9A, ORA,   MC6809, DIRP)
    P00(0x9B, ADDA,  MC6809, DIRP)
    P00(0x9C, CMPX,  MC6809, DIRP)
    P00(0x9D, JSR,   MC6809, DIRP)
    P00(0x9E, LDX,   MC6809, DIRP)
    P00(0x9F, STX,   MC6809, DIRP)
    P00(0xA0, SUBA,  MC6809, INDX)
    P00(0xA1, CMPA,  MC6809, INDX)
    P00(0xA2, SBCA,  MC6809, INDX)
    P00(0xA3, SUBD,  MC6809, INDX)
    P00(0xA4, ANDA,  MC6809, INDX)
    P00(0xA5, BITA,  MC6809, INDX)
    P00(0xA6, LDA,   MC6809, INDX)
    P00(0xA7, STA,   MC6809, INDX)
    P00(0xA8, EORA,  MC6809, INDX)
    P00(0xA9, ADCA,  MC6809, INDX)
    P00(0xAA, ORA,   MC6809, INDX)
    P00(0xAB, ADDA,  MC6809, INDX)
    P00(0xAC, CMPX,  MC6809, INDX)
    P00(0xAD, JSR,   MC6809, INDX)
    P00(0xAE, LDX,   MC6809, INDX)
    P00(0xAF, STX,   MC6809, INDX)
    P00(0xB0, SUBA,  MC6809, EXTD)
    P00(0xB1, CMPA,  MC6809, EXTD)
    P00(0xB2, SBCA,  MC6809, EXTD)
    P00(0xB3, SUBD,  MC6809, EXTD)
    P00(0xB4, ANDA,  MC6809, EXTD)
    P00(0xB5, BITA,  MC6809, EXTD)
    P00(0xB6, LDA,   MC6809, EXTD)
    P00(0xB7, STA,   MC6809, EXTD)
    P00(0xB8, EORA,  MC6809, EXTD)
    P00(0xB9, ADCA,  MC6809, EXTD)
    P00(0xBA, ORA,   MC6809, EXTD)
    P00(0xBB, ADDA,  MC6809, EXTD)
    P00(0xBC, CMPX,  MC6809, EXTD)
    P00(0xBD, JSR,   MC6809, EXTD)
    P00(0xBE, LDX,   MC6809, EXTD)
    P00(0xBF, STX,   MC6809, EXTD)
    P00(0xC0, SUBB,  MC6809, IMM8)
    P00(0xC1, CMPB,  MC6809, IMM8)
    P00(0xC2, SBCB,  MC6809, IMM8)
    P00(0xC3, ADDD,  MC6809, IMM16)
    P00(0xC4, ANDB,  MC6809, IMM8)
    P00(0xC5, BITB,  MC6809, IMM8)
    P00(0xC6, LDB,   MC6809, IMM8)
    P00(0xC8, EORB,  MC6809, IMM8)
    P00(0xC9, ADCB,  MC6809, IMM8)
    P00(0xCA, ORB,   MC6809, IMM8)
    P00(0xCB, ADDB,  MC6809, IMM8)
    P00(0xCC, LDD,   MC6809, IMM16)
    P00(0xCE, LDU,   MC6809, IMM16)
    P00(0xD0, SUBB,  MC6809, DIRP)
    P00(0xD1, CMPB,  MC6809, DIRP)
    P00(0xD2, SBCB,  MC6809, DIRP)
    P00(0xD3, ADDD,  MC6809, DIRP)
    P00(0xD4, ANDB,  MC6809, DIRP)
    P00(0xD5, BITB,  MC6809, DIRP)
    P00(0xD6, LDB,   MC6809, DIRP)
    P00(0xD7, STB,   MC6809, DIRP)
    P00(0xD8, EORB,  MC6809, DIRP)
    P00(0xD9, ADCB,  MC6809, DIRP)
    P00(0xDA, ORB,   MC6809, DIRP)
    P00(0xDB, ADDB,  MC6809, DIRP)
    P00(0xDC, LDD,   MC6809, DIRP)
    P00(0xDD, STD,   MC6809, DIRP)
    P00(0xDE, LDU,   MC6809, DIRP)
    P00(0xDF, STU,   MC6809, DIRP)
    P00(0xE0, SUBB,  MC6809, INDX)
    P00(0xE1, CMPB,  MC6809, INDX)
    P00(0xE2, SBCB,  MC6809, INDX)
    P00(0xE3, ADDD,  MC6809, INDX)
    P00(0xE4, ANDB,  MC6809, INDX)
    P00(0xE5, BITB,  MC6809, INDX)
    P00(0xE6, LDB,   MC6809, INDX)
    P00(0xE7, STB,   MC6809, INDX)
    P00(0xE8, EORB,  MC6809, INDX)
    P00(0xE9, ADCB,  MC6809, INDX)
    P00(0xEA, ORB,   MC6809, INDX)
    P00(0xEB, ADDB,  MC6809, INDX)
    P00(0xEC, LDD,   MC6809, INDX)
    P00(0xED, STD,   MC6809, INDX)
    P00(0xEE, LDU,   MC6809, INDX)
    P00(0xEF, STU,   MC6809, INDX)
    P00(0xF0, SUBB,  MC6809, EXTD)
    P00(0xF1, CMPB,  MC6809, EXTD)
    P00(0xF2, SBCB,  MC6809, EXTD)
    P00(0xF3, ADDD,  MC6809, EXTD)
    P00(0xF4, ANDB,  MC6809, EXTD)
    P00(0xF5, BITB,  MC6809, EXTD)
    P00(0xF6, LDB,   MC6809, EXTD)
    P00(0xF7, STB,   MC6809, EXTD)
    P00(0xF8, EORB,  MC6809, EXTD)
    P00(0xF9, ADCB,  MC6809, EXTD)
    P00(0xFA, ORB,   MC6809, EXTD)
    P00(0xFB, ADDB,  MC6809, EXTD)
    P00(0xFC, LDD,   MC6809, EXTD)
    P00(0xFD, STD,   MC6809, EXTD)
    P00(0xFE, LDU,   MC6809, EXTD)
    P00(0xFF, STU,   MC6809, EXTD)
};

static constexpr Entry MC6809_P10[] PROGMEM = {
    P10(0x21, LBRN,  MC6809, REL16)
    P10(0x22, LBHI,  MC6809, REL16)
    P10(0x23, LBLS,  MC6809, REL16)
    P10(0x24, LBHS,  MC6809, REL16)
    P10(0x24, LBCC,  MC6809, REL16)
    P10(0x25, LBLO,  MC6809, REL16)
    P10(0x25, LBCS,  MC6809, REL16)
    P10(0x26, LBNE,  MC6809, REL16)
    P10(0x27, LBEQ,  MC6809, REL16)
    P10(0x28, LBVC,  MC6809, REL16)
    P10(0x29, LBVS,  MC6809, REL16)
    P10(0x2A, LBPL,  MC6809, REL16)
    P10(0x2B, LBMI,  MC6809, REL16)
    P10(0x2C, LBGE,  MC6809, REL16)
    P10(0x2D, LBLT,  MC6809, REL16)
    P10(0x2E, LBGT,  MC6809, REL16)
    P10(0x2F, LBLE,  MC6809, REL16)
    P10(0x3F, SWI2,  MC6809, INHR)
    P10(0x83, CMPD,  MC6809, IMM16)
    P10(0x8C, CMPY,  MC6809, IMM16)
    P10(0x8E, LDY,   MC6809, IMM16)
    P10(0x93, CMPD,  MC6809, DIRP)
    P10(0x9C, CMPY,  MC6809, DIRP)
    P10(0x9E, LDY,   MC6809, DIRP)
    P10(0x9F, STY,   MC6809, DIRP)
    P10(0xA3, CMPD,  MC6809, INDX)
    P10(0xAC, CMPY,  MC6809, INDX)
    P10(0xAE, LDY,   MC6809, INDX)
    P10(0xAF, STY,   MC6809, INDX)
    P10(0xB3, CMPD,  MC6809, EXTD)
    P10(0xBC, CMPY,  MC6809, EXTD)
    P10(0xBE, LDY,   MC6809, EXTD)
    P10(0xBF, STY,   MC6809, EXTD)
    P10(0xCE, LDS,   MC6809, IMM16)
    P10(0xDE, LDS,   MC6809, DIRP)
    P10(0xDF, STS,   MC6809, DIRP)
    P10(0xEE, LDS,   MC6809, INDX)
    P10(0xEF, STS,   MC6809, INDX)
    P10(0xFE, LDS,   MC6809, EXTD)
    P10(0xFF, STS,   MC6809, EXTD)
};

static constexpr Entry MC6809_P11[] PROGMEM = {
    P11(0x3F, SWI3,  MC6809, INHR)
    P11(0x83, CMPU,  MC6809, IMM16)
    P11(0x8C, CMPS,  MC6809, IMM16)
    P11(0x93, CMPU,  MC6809, DIRP)
    P11(0x9C, CMPS,  MC6809, DIRP)
    P11(0xA3, CMPU,  MC6809, INDX)
    P11(0xAC, CMPS,  MC6809, INDX)
    P11(0xB3, CMPU,  MC6809, EXTD)
    P11(0xBC, CMPS,  MC6809, EXTD)
};

static constexpr target::opcode_t PREFIX_P00 = 0x00;
static constexpr target::opcode_t PREFIX_P10 = 0x10;
static constexpr target::opcode_t PREFIX_P11 = 0x11;

bool TableHd6309Base::isPrefixCode(target::opcode_t opCode) {
    return opCode == PREFIX_P10 || opCode == PREFIX_P11;
}

const Entry *TableHd6309Base::searchEntry(
    const char *name,
    const Entry *table, const Entry *end) {
    for (const Entry *entry = table; entry < end; entry++) {
        if (pgm_strcasecmp(name, entry->name) == 0)
            return entry;
    }
    return nullptr;
}

const Entry *TableHd6309Base::searchEntry(
    const target::opcode_t opCode,
    const Entry *table, const Entry *end) {
    for (const Entry *entry = table; entry < end; entry++) {
        if (opCode == pgm_read_byte(&entry->opc))
            return entry;
    }
    return nullptr;
}

Error TableHd6309Base::searchPages(
    Insn &insn, const char *name, const EntryPage *pages, const EntryPage *end) {
    for (const EntryPage *page = pages; page < end; page++) {
        const Entry *entry;
        if ((entry = searchEntry(name, page->table, page->end)) != nullptr) {
            insn.setInsnCode(insnCode(page->prefix, pgm_read_byte(&entry->opc)));
            insn.setFlags(pgm_read_byte(&entry->flags));
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

static bool acceptAddrMode(AddrMode opr, AddrMode table) {
    if (opr == table) return true;
    if (opr == IMM8) return table == IMM16 || table == IMM32;
    return false;
}

Error TableHd6309Base::searchPages(
    Insn &insn, const char *name, AddrMode addrMode, const EntryPage *pages, const EntryPage *end) {
    for (const EntryPage *page = pages; page < end; page++) {
        for (const Entry *entry = page->table; entry < page->end
                 && (entry = searchEntry(name, entry, page->end)) != nullptr; entry++) {
            const host::uint_t flags = pgm_read_byte(&entry->flags);
            if (acceptAddrMode(addrMode, _addrMode(flags))) {
                insn.setInsnCode(insnCode(page->prefix, pgm_read_byte(&entry->opc)));
                insn.setFlags(flags);
                return OK;
            }
        }
    }
    return UNKNOWN_INSTRUCTION;
}

Error TableHd6309Base::searchPages(
    Insn &insn, target::insn_t insnCode, const EntryPage *pages, const EntryPage *end) {
    for (const EntryPage *page = pages; page < end; page++) {
        if (TableHd6309Base::prefixCode(insnCode) != page->prefix) continue;
        const Entry *entry = searchEntry(opCode(insnCode), page->table, page->end);
        if (entry) {
            insn.setFlags(pgm_read_byte(&entry->flags));
            char name[8];
            pgm_strcpy(name, entry->name);
            insn.setName(name);
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

static constexpr EntryPage MC6809_PAGES[] = {
    { PREFIX_P00, &MC6809_P00[0], ARRAY_END(MC6809_P00) },
    { PREFIX_P10, &MC6809_P10[0], ARRAY_END(MC6809_P10) },
    { PREFIX_P11, &MC6809_P11[0], ARRAY_END(MC6809_P11) },
};

template<>
Error TableHd6309<MC6809>::searchName(Insn &insn) const {
    if (searchPages(insn, insn.name(), ARRAY_RANGE(MC6809_PAGES)) == OK)
        return OK;
    return UNKNOWN_INSTRUCTION;
}

template<>
Error TableHd6309<MC6809>::searchNameAndAddrMode(Insn &insn) const {
    if (searchPages(insn, insn.name(), insn.addrMode(), ARRAY_RANGE(MC6809_PAGES)) == OK)
        return OK;
    return UNKNOWN_INSTRUCTION;
}

template<>
Error TableHd6309<MC6809>::searchInsnCode(Insn &insn) const {
    if (searchPages(insn, insn.insnCode(), ARRAY_RANGE(MC6809_PAGES)) == OK)
        return OK;
    return UNKNOWN_INSTRUCTION;
}

static const TableHd6309<MC6809> MC6809Table;

template<>
const TableHd6309<MC6809> *TableHd6309<MC6809>::table() {
    return &MC6809Table;
}
