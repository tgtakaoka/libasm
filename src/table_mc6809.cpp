#include "config_hd6309.h"

#include "entry_hd6309.h"
#include "table_hd6309.h"
#include "text_mc6809.h"

static constexpr Entry MC6809_P00[] PROGMEM = {
    P00(0x00, NEG,   BYTE, MC6809, DIRP)
    P00(0x03, COM,   BYTE, MC6809, DIRP)
    P00(0x04, LSR,   BYTE, MC6809, DIRP)
    P00(0x06, ROR,   BYTE, MC6809, DIRP)
    P00(0x07, ASR,   BYTE, MC6809, DIRP)
    P00(0x08, ASL,   BYTE, MC6809, DIRP)
    P00(0x08, LSL,   BYTE, MC6809, DIRP)
    P00(0x09, ROL,   BYTE, MC6809, DIRP)
    P00(0x0A, DEC,   BYTE, MC6809, DIRP)
    P00(0x0C, INC,   BYTE, MC6809, DIRP)
    P00(0x0D, TST,   BYTE, MC6809, DIRP)
    P00(0x0E, JMP,   NONE, MC6809, DIRP)
    P00(0x0F, CLR,   BYTE, MC6809, DIRP)
    P00(0x12, NOP,   NONE, MC6809, INHR)
    P00(0x13, SYNC,  NONE, MC6809, INHR)
    P00(0x16, LBRA,  WORD, MC6809, REL)
    P00(0x17, LBSR,  WORD, MC6809, REL)
    P00(0x19, DAA,   NONE, MC6809, INHR)
    P00(0x1A, ORCC,  BYTE, MC6809, IMM)
    P00(0x1C, ANDCC, BYTE, MC6809, IMM)
    P00(0x1D, SEX,   BYTE, MC6809, INHR)
    P00(0x1E, EXG,   NONE, MC6809, REGS)
    P00(0x1F, TFR,   NONE, MC6809, REGS)
    P00(0x20, BRA,   BYTE, MC6809, REL)
    P00(0x21, BRN,   BYTE, MC6809, REL)
    P00(0x22, BHI,   BYTE, MC6809, REL)
    P00(0x23, BLS,   BYTE, MC6809, REL)
    P00(0x24, BHS,   BYTE, MC6809, REL)
    P00(0x24, BCC,   BYTE, MC6809, REL)
    P00(0x25, BLO,   BYTE, MC6809, REL)
    P00(0x25, BCS,   BYTE, MC6809, REL)
    P00(0x26, BNE,   BYTE, MC6809, REL)
    P00(0x27, BEQ,   BYTE, MC6809, REL)
    P00(0x28, BVC,   BYTE, MC6809, REL)
    P00(0x29, BVS,   BYTE, MC6809, REL)
    P00(0x2A, BPL,   BYTE, MC6809, REL)
    P00(0x2B, BMI,   BYTE, MC6809, REL)
    P00(0x2C, BGE,   BYTE, MC6809, REL)
    P00(0x2D, BLT,   BYTE, MC6809, REL)
    P00(0x2E, BGT,   BYTE, MC6809, REL)
    P00(0x2F, BLE,   BYTE, MC6809, REL)
    P00(0x30, LEAX,  NONE, MC6809, INDX)
    P00(0x31, LEAY,  NONE, MC6809, INDX)
    P00(0x32, LEAS,  NONE, MC6809, INDX)
    P00(0x33, LEAU,  NONE, MC6809, INDX)
    P00(0x34, PSHS,  NONE, MC6809, STKOP)
    P00(0x35, PULS,  NONE, MC6809, STKOP)
    P00(0x36, PSHU,  NONE, MC6809, STKOP)
    P00(0x37, PULU,  NONE, MC6809, STKOP)
    P00(0x39, RTS,   NONE, MC6809, INHR)
    P00(0x3A, ABX,   NONE, MC6809, INHR)
    P00(0x3B, RTI,   NONE, MC6809, INHR)
    P00(0x3C, CWAI,  BYTE, MC6809, IMM)
    P00(0x3D, MUL,   BYTE, MC6809, INHR)
    P00(0x3F, SWI,   NONE, MC6809, INHR)
    P00(0x40, NEGA,  BYTE, MC6809, INHR)
    P00(0x43, COMA,  BYTE, MC6809, INHR)
    P00(0x44, LSRA,  BYTE, MC6809, INHR)
    P00(0x46, RORA,  BYTE, MC6809, INHR)
    P00(0x47, ASRA,  BYTE, MC6809, INHR)
    P00(0x48, ASLA,  BYTE, MC6809, INHR)
    P00(0x48, LSLA,  BYTE, MC6809, INHR)
    P00(0x49, ROLA,  BYTE, MC6809, INHR)
    P00(0x4A, DECA,  BYTE, MC6809, INHR)
    P00(0x4C, INCA,  BYTE, MC6809, INHR)
    P00(0x4D, TSTA,  BYTE, MC6809, INHR)
    P00(0x4F, CLRA,  BYTE, MC6809, INHR)
    P00(0x50, NEGB,  BYTE, MC6809, INHR)
    P00(0x53, COMB,  BYTE, MC6809, INHR)
    P00(0x54, LSRB,  BYTE, MC6809, INHR)
    P00(0x56, RORB,  BYTE, MC6809, INHR)
    P00(0x57, ASRB,  BYTE, MC6809, INHR)
    P00(0x58, ASLB,  BYTE, MC6809, INHR)
    P00(0x58, LSLB,  BYTE, MC6809, INHR)
    P00(0x59, ROLB,  BYTE, MC6809, INHR)
    P00(0x5A, DECB,  BYTE, MC6809, INHR)
    P00(0x5C, INCB,  BYTE, MC6809, INHR)
    P00(0x5D, TSTB,  BYTE, MC6809, INHR)
    P00(0x5F, CLRB,  BYTE, MC6809, INHR)
    P00(0x60, NEG,   BYTE, MC6809, INDX)
    P00(0x63, COM,   BYTE, MC6809, INDX)
    P00(0x64, LSR,   BYTE, MC6809, INDX)
    P00(0x66, ROR,   BYTE, MC6809, INDX)
    P00(0x67, ASR,   BYTE, MC6809, INDX)
    P00(0x68, ASL,   BYTE, MC6809, INDX)
    P00(0x68, LSL,   BYTE, MC6809, INDX)
    P00(0x69, ROL,   BYTE, MC6809, INDX)
    P00(0x6A, DEC,   BYTE, MC6809, INDX)
    P00(0x6C, INC,   BYTE, MC6809, INDX)
    P00(0x6D, TST,   BYTE, MC6809, INDX)
    P00(0x6E, JMP,   NONE, MC6809, INDX)
    P00(0x6F, CLR,   BYTE, MC6809, INDX)
    P00(0x70, NEG,   BYTE, MC6809, EXTD)
    P00(0x73, COM,   BYTE, MC6809, EXTD)
    P00(0x74, LSR,   BYTE, MC6809, EXTD)
    P00(0x76, ROR,   BYTE, MC6809, EXTD)
    P00(0x77, ASR,   BYTE, MC6809, EXTD)
    P00(0x78, ASL,   BYTE, MC6809, EXTD)
    P00(0x78, LSL,   BYTE, MC6809, EXTD)
    P00(0x79, ROL,   BYTE, MC6809, EXTD)
    P00(0x7A, DEC,   BYTE, MC6809, EXTD)
    P00(0x7C, INC,   BYTE, MC6809, EXTD)
    P00(0x7D, TST,   BYTE, MC6809, EXTD)
    P00(0x7E, JMP,   NONE, MC6809, EXTD)
    P00(0x7F, CLR,   BYTE, MC6809, EXTD)
    P00(0x80, SUBA,  BYTE, MC6809, IMM)
    P00(0x81, CMPA,  BYTE, MC6809, IMM)
    P00(0x82, SBCA,  BYTE, MC6809, IMM)
    P00(0x83, SUBD,  WORD, MC6809, IMM)
    P00(0x84, ANDA,  BYTE, MC6809, IMM)
    P00(0x85, BITA,  BYTE, MC6809, IMM)
    P00(0x86, LDA,   BYTE, MC6809, IMM)
    P00(0x88, EORA,  BYTE, MC6809, IMM)
    P00(0x89, ADCA,  BYTE, MC6809, IMM)
    P00(0x8A, ORA,   BYTE, MC6809, IMM)
    P00(0x8B, ADDA,  BYTE, MC6809, IMM)
    P00(0x8C, CMPX,  WORD, MC6809, IMM)
    P00(0x8D, BSR,   BYTE, MC6809, REL)
    P00(0x8E, LDX,   WORD, MC6809, IMM)
    P00(0x90, SUBA,  BYTE, MC6809, DIRP)
    P00(0x91, CMPA,  BYTE, MC6809, DIRP)
    P00(0x92, SBCA,  BYTE, MC6809, DIRP)
    P00(0x93, SUBD,  WORD, MC6809, DIRP)
    P00(0x94, ANDA,  BYTE, MC6809, DIRP)
    P00(0x95, BITA,  BYTE, MC6809, DIRP)
    P00(0x96, LDA,   BYTE, MC6809, DIRP)
    P00(0x97, STA,   BYTE, MC6809, DIRP)
    P00(0x98, EORA,  BYTE, MC6809, DIRP)
    P00(0x99, ADCA,  BYTE, MC6809, DIRP)
    P00(0x9A, ORA,   BYTE, MC6809, DIRP)
    P00(0x9B, ADDA,  BYTE, MC6809, DIRP)
    P00(0x9C, CMPX,  WORD, MC6809, DIRP)
    P00(0x9D, JSR,   NONE, MC6809, DIRP)
    P00(0x9E, LDX,   WORD, MC6809, DIRP)
    P00(0x9F, STX,   WORD, MC6809, DIRP)
    P00(0xA0, SUBA,  BYTE, MC6809, INDX)
    P00(0xA1, CMPA,  BYTE, MC6809, INDX)
    P00(0xA2, SBCA,  BYTE, MC6809, INDX)
    P00(0xA3, SUBD,  WORD, MC6809, INDX)
    P00(0xA4, ANDA,  BYTE, MC6809, INDX)
    P00(0xA5, BITA,  BYTE, MC6809, INDX)
    P00(0xA6, LDA,   BYTE, MC6809, INDX)
    P00(0xA7, STA,   BYTE, MC6809, INDX)
    P00(0xA8, EORA,  BYTE, MC6809, INDX)
    P00(0xA9, ADCA,  BYTE, MC6809, INDX)
    P00(0xAA, ORA,   BYTE, MC6809, INDX)
    P00(0xAB, ADDA,  BYTE, MC6809, INDX)
    P00(0xAC, CMPX,  WORD, MC6809, INDX)
    P00(0xAD, JSR,   NONE, MC6809, INDX)
    P00(0xAE, LDX,   WORD, MC6809, INDX)
    P00(0xAF, STX,   WORD, MC6809, INDX)
    P00(0xB0, SUBA,  BYTE, MC6809, EXTD)
    P00(0xB1, CMPA,  BYTE, MC6809, EXTD)
    P00(0xB2, SBCA,  BYTE, MC6809, EXTD)
    P00(0xB3, SUBD,  BYTE, MC6809, EXTD)
    P00(0xB4, ANDA,  BYTE, MC6809, EXTD)
    P00(0xB5, BITA,  BYTE, MC6809, EXTD)
    P00(0xB6, LDA,   BYTE, MC6809, EXTD)
    P00(0xB7, STA,   BYTE, MC6809, EXTD)
    P00(0xB8, EORA,  BYTE, MC6809, EXTD)
    P00(0xB9, ADCA,  BYTE, MC6809, EXTD)
    P00(0xBA, ORA,   BYTE, MC6809, EXTD)
    P00(0xBB, ADDA,  BYTE, MC6809, EXTD)
    P00(0xBC, CMPX,  WORD, MC6809, EXTD)
    P00(0xBD, JSR,   NONE, MC6809, EXTD)
    P00(0xBE, LDX,   WORD, MC6809, EXTD)
    P00(0xBF, STX,   WORD, MC6809, EXTD)
    P00(0xC0, SUBB,  BYTE, MC6809, IMM)
    P00(0xC1, CMPB,  BYTE, MC6809, IMM)
    P00(0xC2, SBCB,  BYTE, MC6809, IMM)
    P00(0xC3, ADDD,  WORD, MC6809, IMM)
    P00(0xC4, ANDB,  BYTE, MC6809, IMM)
    P00(0xC5, BITB,  BYTE, MC6809, IMM)
    P00(0xC6, LDB,   BYTE, MC6809, IMM)
    P00(0xC8, EORB,  BYTE, MC6809, IMM)
    P00(0xC9, ADCB,  BYTE, MC6809, IMM)
    P00(0xCA, ORB,   BYTE, MC6809, IMM)
    P00(0xCB, ADDB,  BYTE, MC6809, IMM)
    P00(0xCC, LDD,   WORD, MC6809, IMM)
    P00(0xCE, LDU,   WORD, MC6809, IMM)
    P00(0xD0, SUBB,  BYTE, MC6809, DIRP)
    P00(0xD1, CMPB,  BYTE, MC6809, DIRP)
    P00(0xD2, SBCB,  BYTE, MC6809, DIRP)
    P00(0xD3, ADDD,  WORD, MC6809, DIRP)
    P00(0xD4, ANDB,  BYTE, MC6809, DIRP)
    P00(0xD5, BITB,  BYTE, MC6809, DIRP)
    P00(0xD6, LDB,   BYTE, MC6809, DIRP)
    P00(0xD7, STB,   BYTE, MC6809, DIRP)
    P00(0xD8, EORB,  BYTE, MC6809, DIRP)
    P00(0xD9, ADCB,  BYTE, MC6809, DIRP)
    P00(0xDA, ORB,   BYTE, MC6809, DIRP)
    P00(0xDB, ADDB,  BYTE, MC6809, DIRP)
    P00(0xDC, LDD,   WORD, MC6809, DIRP)
    P00(0xDD, STD,   WORD, MC6809, DIRP)
    P00(0xDE, LDU,   WORD, MC6809, DIRP)
    P00(0xDF, STU,   WORD, MC6809, DIRP)
    P00(0xE0, SUBB,  BYTE, MC6809, INDX)
    P00(0xE1, CMPB,  BYTE, MC6809, INDX)
    P00(0xE2, SBCB,  BYTE, MC6809, INDX)
    P00(0xE3, ADDD,  WORD, MC6809, INDX)
    P00(0xE4, ANDB,  BYTE, MC6809, INDX)
    P00(0xE5, BITB,  BYTE, MC6809, INDX)
    P00(0xE6, LDB,   BYTE, MC6809, INDX)
    P00(0xE7, STB,   BYTE, MC6809, INDX)
    P00(0xE8, EORB,  BYTE, MC6809, INDX)
    P00(0xE9, ADCB,  BYTE, MC6809, INDX)
    P00(0xEA, ORB,   BYTE, MC6809, INDX)
    P00(0xEB, ADDB,  BYTE, MC6809, INDX)
    P00(0xEC, LDD,   WORD, MC6809, INDX)
    P00(0xED, STD,   WORD, MC6809, INDX)
    P00(0xEE, LDU,   WORD, MC6809, INDX)
    P00(0xEF, STU,   WORD, MC6809, INDX)
    P00(0xF0, SUBB,  BYTE, MC6809, EXTD)
    P00(0xF1, CMPB,  BYTE, MC6809, EXTD)
    P00(0xF2, SBCB,  BYTE, MC6809, EXTD)
    P00(0xF3, ADDD,  WORD, MC6809, EXTD)
    P00(0xF4, ANDB,  BYTE, MC6809, EXTD)
    P00(0xF5, BITB,  BYTE, MC6809, EXTD)
    P00(0xF6, LDB,   BYTE, MC6809, EXTD)
    P00(0xF7, STB,   BYTE, MC6809, EXTD)
    P00(0xF8, EORB,  BYTE, MC6809, EXTD)
    P00(0xF9, ADCB,  BYTE, MC6809, EXTD)
    P00(0xFA, ORB,   BYTE, MC6809, EXTD)
    P00(0xFB, ADDB,  BYTE, MC6809, EXTD)
    P00(0xFC, LDD,   WORD, MC6809, EXTD)
    P00(0xFD, STD,   WORD, MC6809, EXTD)
    P00(0xFE, LDU,   WORD, MC6809, EXTD)
    P00(0xFF, STU,   WORD, MC6809, EXTD)

    // Pseudo instruction to set DP on assembler
    P00(PSEUDO_SETDP,  SETDP,  BYTE, MC6809, PSEUDO)
    P00(PSEUDO_ASSUME, ASSUME, BYTE, MC6809, PSEUDO)
};

static constexpr Entry MC6809_P10[] PROGMEM = {
    P10(0x21, LBRN,  WORD, MC6809, REL)
    P10(0x22, LBHI,  WORD, MC6809, REL)
    P10(0x23, LBLS,  WORD, MC6809, REL)
    P10(0x24, LBHS,  WORD, MC6809, REL)
    P10(0x24, LBCC,  WORD, MC6809, REL)
    P10(0x25, LBLO,  WORD, MC6809, REL)
    P10(0x25, LBCS,  WORD, MC6809, REL)
    P10(0x26, LBNE,  WORD, MC6809, REL)
    P10(0x27, LBEQ,  WORD, MC6809, REL)
    P10(0x28, LBVC,  WORD, MC6809, REL)
    P10(0x29, LBVS,  WORD, MC6809, REL)
    P10(0x2A, LBPL,  WORD, MC6809, REL)
    P10(0x2B, LBMI,  WORD, MC6809, REL)
    P10(0x2C, LBGE,  WORD, MC6809, REL)
    P10(0x2D, LBLT,  WORD, MC6809, REL)
    P10(0x2E, LBGT,  WORD, MC6809, REL)
    P10(0x2F, LBLE,  WORD, MC6809, REL)
    P10(0x3F, SWI2,  NONE, MC6809, INHR)
    P10(0x83, CMPD,  WORD, MC6809, IMM)
    P10(0x8C, CMPY,  WORD, MC6809, IMM)
    P10(0x8E, LDY,   WORD, MC6809, IMM)
    P10(0x93, CMPD,  WORD, MC6809, DIRP)
    P10(0x9C, CMPY,  WORD, MC6809, DIRP)
    P10(0x9E, LDY,   WORD, MC6809, DIRP)
    P10(0x9F, STY,   WORD, MC6809, DIRP)
    P10(0xA3, CMPD,  WORD, MC6809, INDX)
    P10(0xAC, CMPY,  WORD, MC6809, INDX)
    P10(0xAE, LDY,   WORD, MC6809, INDX)
    P10(0xAF, STY,   WORD, MC6809, INDX)
    P10(0xB3, CMPD,  WORD, MC6809, EXTD)
    P10(0xBC, CMPY,  WORD, MC6809, EXTD)
    P10(0xBE, LDY,   WORD, MC6809, EXTD)
    P10(0xBF, STY,   WORD, MC6809, EXTD)
    P10(0xCE, LDS,   WORD, MC6809, IMM)
    P10(0xDE, LDS,   WORD, MC6809, DIRP)
    P10(0xDF, STS,   WORD, MC6809, DIRP)
    P10(0xEE, LDS,   WORD, MC6809, INDX)
    P10(0xEF, STS,   WORD, MC6809, INDX)
    P10(0xFE, LDS,   WORD, MC6809, EXTD)
    P10(0xFF, STS,   WORD, MC6809, EXTD)
};

static constexpr Entry MC6809_P11[] PROGMEM = {
    P11(0x3F, SWI3,  NONE, MC6809, INHR)
    P11(0x83, CMPU,  WORD, MC6809, IMM)
    P11(0x8C, CMPS,  WORD, MC6809, IMM)
    P11(0x93, CMPU,  WORD, MC6809, DIRP)
    P11(0x9C, CMPS,  WORD, MC6809, DIRP)
    P11(0xA3, CMPU,  WORD, MC6809, INDX)
    P11(0xAC, CMPS,  WORD, MC6809, INDX)
    P11(0xB3, CMPU,  WORD, MC6809, EXTD)
    P11(0xBC, CMPS,  WORD, MC6809, EXTD)
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

Error TableHd6309Base::searchPages(
    Insn &insn, const char *name, AddrMode addrMode, const EntryPage *pages, const EntryPage *end) {
    for (const EntryPage *page = pages; page < end; page++) {
        for (const Entry *entry = page->table; entry < page->end
                 && (entry = searchEntry(name, entry, page->end)) != nullptr; entry++) {
            const host::uint_t flags = pgm_read_byte(&entry->flags);
            if (addrMode == _addrMode(flags)) {
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
