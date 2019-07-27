#include <ctype.h>

#include "config_z80.h"

#include "entry_z80.h"
#include "table_z80.h"
#include "text_z80.h"

static const Entry TABLE_00[] PROGMEM = {
    E(0x00, NOP,  NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0x08, EX,   NO_FMT,  AF_REG, AFPREG, INHERENT)
    E(0x10, DJNZ, NO_FMT,  NO_OPR, IMM16,  RELATIVE)
    E(0x18, JR,   NO_FMT,  NO_OPR, IMM16,  RELATIVE)
    E(0x20, JR,   CC_FMT,  CC4,    IMM16,  RELATIVE)
    E(0x01, LD,   PTR_FMT, REG16,  IMM16,  IMMEDIATE16)
    E(0x09, ADD,  PTR_FMT, HL_REG, REG16,  INHERENT)
    E(0x02, LD,   IDX_FMT, BC_PTR, A_REG,  INHERENT)
    E(0x0A, LD,   IDX_FMT, A_REG,  BC_PTR, INHERENT)
    E(0x22, LD,   NO_FMT,  ADDR16, HL_REG, DIRECT)
    E(0x2A, LD,   NO_FMT,  HL_REG, ADDR16, DIRECT)
    E(0x32, LD,   NO_FMT,  ADDR16, A_REG,  DIRECT)
    E(0x3A, LD,   NO_FMT,  A_REG,  ADDR16, DIRECT)
    E(0x03, INC,  PTR_FMT, REG16,  NO_OPR, INHERENT)
    E(0x0B, DEC,  PTR_FMT, REG16,  NO_OPR, INHERENT)
    E(0x04, INC,  DST_FMT, REG8,   NO_OPR, INHERENT)
    E(0x05, DEC,  DST_FMT, REG8,   NO_OPR, INHERENT)
    E(0x06, LD,   DST_FMT, REG8,   IMM8,   IMMEDIATE8)
    E(0x07, RLCA, NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0x0F, RRCA, NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0x17, RLA,  NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0x1F, RRA,  NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0x27, DAA,  NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0x2F, CPL,  NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0x37, SCF,  NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0x3F, CCF,  NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0x76, HALT, NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0x40, LD,   DST_SRC_FMT, REG8,   REG8,   INHERENT)
    E(0x80, ADD,  SRC_FMT, A_REG,  REG8,   INHERENT)
    E(0x88, ADC,  SRC_FMT, A_REG,  REG8,   INHERENT)
    E(0x90, SUB,  SRC_FMT, A_REG,  REG8,   INHERENT)
    E(0x98, SBC,  SRC_FMT, A_REG,  REG8,   INHERENT)
    E(0xA0, AND,  SRC_FMT, A_REG,  REG8,   INHERENT)
    E(0xA8, XOR,  SRC_FMT, A_REG,  REG8,   INHERENT)
    E(0xB0, OR,   SRC_FMT, A_REG,  REG8,   INHERENT)
    E(0xB8, CP,   SRC_FMT, A_REG,  REG8,   INHERENT)
    E(0xC0, RET,  DST_FMT, CC8,    NO_OPR, INHERENT)
    E(0xC1, POP,  PTR_FMT, STK16,  NO_OPR, INHERENT)
    E(0xC9, RET,  NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0xD9, EXX,  NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0xE9, JP,   NO_FMT,  HL_PTR, NO_OPR, INHERENT)
    E(0xF9, LD,   NO_FMT,  SP_REG, HL_REG, INHERENT)
    E(0xC2, JP,   DST_FMT, CC8,    IMM16,  DIRECT)
    E(0xC3, JP,   NO_FMT,  NO_OPR, IMM16,  DIRECT)
    E(0xD3, OUT,  NO_FMT,  ADDR8,  A_REG,  IOADDR)
    E(0xDB, IN,   NO_FMT,  A_REG,  ADDR8,  IOADDR)
    E(0xE3, EX,   NO_FMT,  SP_PTR, HL_REG, INHERENT)
    E(0xEB, EX,   NO_FMT,  DE_REG, HL_REG, INHERENT)
    E(0xF3, DI,   NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0xFB, EI,   NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0xC4, CALL, DST_FMT, CC8,    IMM16,  DIRECT)
    E(0xC5, PUSH, PTR_FMT, STK16,  NO_OPR, INHERENT)
    E(0xCD, CALL, NO_FMT,  NO_OPR, IMM16,  DIRECT)
    E(0xC6, ADD,  NO_FMT,  A_REG,  IMM8,   IMMEDIATE8)
    E(0xCE, ADC,  NO_FMT,  A_REG,  IMM8,   IMMEDIATE8)
    E(0xD6, SUB,  NO_FMT,  A_REG,  IMM8,   IMMEDIATE8)
    E(0xDE, SBC,  NO_FMT,  A_REG,  IMM8,   IMMEDIATE8)
    E(0xE6, AND,  NO_FMT,  A_REG,  IMM8,   IMMEDIATE8)
    E(0xEE, XOR,  NO_FMT,  A_REG,  IMM8,   IMMEDIATE8)
    E(0xF6, OR,   NO_FMT,  A_REG,  IMM8,   IMMEDIATE8)
    E(0xFE, CP,   NO_FMT,  A_REG,  IMM8,   IMMEDIATE8)
    E(0xC7, RST,  DST_FMT, VEC_NO, NO_OPR, INHERENT)
};
static constexpr target::opcode_t PREFIX_00 = 0x00;

static const Entry TABLE_CB[] PROGMEM = {
    E(0x00, RLC,  SRC_FMT, REG8,   NO_OPR, INHERENT)
    E(0x08, RRC,  SRC_FMT, REG8,   NO_OPR, INHERENT)
    E(0x10, RL,   SRC_FMT, REG8,   NO_OPR, INHERENT)
    E(0x18, RR,   SRC_FMT, REG8,   NO_OPR, INHERENT)
    E(0x20, SLA,  SRC_FMT, REG8,   NO_OPR, INHERENT)
    E(0x28, SRA,  SRC_FMT, REG8,   NO_OPR, INHERENT)
    E(0x38, SRL,  SRC_FMT, REG8,   NO_OPR, INHERENT)
    E(0x40, BIT,  DST_SRC_FMT, BIT_NO, REG8,   INHERENT)
    E(0x80, RES,  DST_SRC_FMT, BIT_NO, REG8,   INHERENT)
    E(0xC0, SET,  DST_SRC_FMT, BIT_NO, REG8,   INHERENT)
};
static constexpr target::opcode_t PREFIX_CB = 0xCB;

static const Entry TABLE_ED[] PROGMEM = {
    E(0x40, IN,   DST_FMT, REG8,   C_PTR,  INHERENT)
    E(0x41, OUT,  DST_FMT, C_PTR,  REG8,   INHERENT)
    E(0x42, SBC,  PTR_FMT, HL_REG, REG16,  INHERENT)
    E(0x4A, ADC,  PTR_FMT, HL_REG, REG16,  INHERENT)
    E(0x43, LD,   PTR_FMT, ADDR16, REG16,  DIRECT)
    E(0x4B, LD,   PTR_FMT, REG16,  ADDR16, DIRECT)
    E(0x44, NEG,  NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0x45, RETN, NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0x4D, RETI, NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0x46, IM,   IDX_FMT, IMM_01, NO_OPR, INHERENT)
    E(0x5E, IM,   NO_FMT,  IMM_2,  NO_OPR, INHERENT)
    E(0x47, LD,   IR_FMT,  IR_REG, A_REG,  INHERENT)
    E(0x57, LD,   IR_FMT,  A_REG,  IR_REG, INHERENT)
    E(0x67, RRD,  NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0x6F, RLD,  NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0xA0, LDI,  NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0xA8, LDD,  NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0xB0, LDIR, NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0xB8, LDDR, NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0xA1, CPI,  NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0xA9, CPD,  NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0xB1, CPIR, NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0xB9, CPDR, NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0xA2, INI,  NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0xAA, IND,  NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0xB2, INIR, NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0xBA, INDR, NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0xA3, OUTI, NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0xAB, OUTD, NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0xB3, OTIR, NO_FMT,  NO_OPR, NO_OPR, INHERENT)
    E(0xBB, OTDR, NO_FMT,  NO_OPR, NO_OPR, INHERENT)
};
static constexpr target::opcode_t PREFIX_ED = 0xED;

static const Entry TABLE_IX[] PROGMEM = {
    E(0x09, ADD,  PTR_FMT, IX_REG, REG16X, INHERENT)
    E(0x21, LD,   NO_FMT,  IX_REG, IMM16,  IMMEDIATE16)
    E(0x22, LD,   NO_FMT,  ADDR16, IX_REG, DIRECT)
    E(0x2A, LD,   NO_FMT,  IX_REG, ADDR16, DIRECT)
    E(0x23, INC,  NO_FMT,  IX_REG, NO_OPR, INHERENT)
    E(0x2B, DEC,  NO_FMT,  IX_REG, NO_OPR, INHERENT)
    E(0x34, INC,  NO_FMT,  IX_OFF, NO_OPR, INDEXED)
    E(0x35, DEC,  NO_FMT,  IX_OFF, NO_OPR, INDEXED)
    E(0x36, LD,   NO_FMT,  IX_OFF, IMM8,   INDEXED_IMMEDIATE8)
    E(0x46, LD,   DST_FMT, REG8,   IX_OFF, INDEXED)
    E(0x70, LD,   SRC_FMT, IX_OFF, REG8,   INDEXED)
    E(0x86, ADD,  NO_FMT,  A_REG,  IX_OFF, INDEXED)
    E(0x8E, ADC,  NO_FMT,  A_REG,  IX_OFF, INDEXED)
    E(0x96, SUB,  NO_FMT,  A_REG,  IX_OFF, INDEXED)
    E(0x9E, SBC,  NO_FMT,  A_REG,  IX_OFF, INDEXED)
    E(0xA6, AND,  NO_FMT,  A_REG,  IX_OFF, INDEXED)
    E(0xAE, XOR,  NO_FMT,  A_REG,  IX_OFF, INDEXED)
    E(0xB6, OR,   NO_FMT,  A_REG,  IX_OFF, INDEXED)
    E(0xBE, CP,   NO_FMT,  A_REG,  IX_OFF, INDEXED)
    E(0xCB, BIT,  NO_FMT,  IX_BIT, IX_BIT, INDEXED_IMMEDIATE8)
    E(0xE1, POP,  NO_FMT,  IX_REG, NO_OPR, INHERENT)
    E(0xE9, JP,   NO_FMT,  IX_PTR, NO_OPR, INHERENT)
    E(0xF9, LD,   NO_FMT,  SP_REG, IX_REG, INHERENT)
    E(0xE3, EX,   NO_FMT,  SP_PTR, IX_REG, INHERENT)
    E(0xE5, PUSH, NO_FMT,  IX_REG, NO_OPR, INHERENT)
};
static constexpr target::opcode_t PREFIX_IX = 0xDD;
static constexpr target::opcode_t PREFIX_IY = 0xFD;

static const Entry *searchEntry(
    const char *name, const Entry *table, const Entry *end) {
    for (const Entry *entry = table; entry < end; entry++) {
        host::uint_t idx = 0;
        while (name[idx] && idx < sizeof(entry->name)) {
            const char n = pgm_read_byte(&entry->name[idx]);
            if (toupper(name[idx]) != n) break;
            idx++;
        }
        if (name[idx] == 0) {
            if (idx == sizeof(entry->name)) return entry;
            if (pgm_read_byte(&entry->name[idx]) == 0) return entry;
        }
    }
    return nullptr;
}

static const Entry *searchEntry(
    const char *name, OprFormat leftOpr, OprFormat rightOpr,
    const Entry *table, const Entry *end) {
    for (const Entry *entry = table; entry < end
             && (entry = searchEntry(name, entry, end)) != nullptr; entry++) {
        const OprFormat lop = _oprFormat(pgm_read_byte(&entry->flags1));
        const OprFormat rop = _oprFormat(pgm_read_byte(&entry->flags2));
        if (lop == leftOpr && rop == rightOpr) return entry;
    }
    return nullptr;
}

static const Entry *searchEntry(
    const target::opcode_t opcode,
    const Entry *table, const Entry *end) {
    for (const Entry *entry = table; entry < end; entry++) {
        target::opcode_t opc = opcode;
        const InsnFormat iformat = _insnFormat(pgm_read_byte(&entry->flags1));
        switch (iformat) {
        case PTR_FMT: opc &= ~0x30; break;
        case CC_FMT:  opc &= ~0x18; break;
        case IDX_FMT: opc &= ~0x10; break;
        case IR_FMT:  opc &= ~0x08; break;
        case DST_FMT: opc &= ~0x38; break;
        case SRC_FMT: opc &= ~0x07; break;
        case DST_SRC_FMT: opc &= ~0x3F; break;
        default: break;
        }
        if (opc == pgm_read_byte(&entry->opc))
            return entry;
    }
    return nullptr;
}

struct EntryPage {
    const target::opcode_t prefix;
    const Entry *const table;
    const Entry *const end;
};

static Error searchPages(
    Insn &insn, const char *name, OprFormat lop, OprFormat rop,
    const EntryPage *pages, const EntryPage *end) {
    for (const EntryPage *page = pages; page < end; page++) {
        const Entry *entry;
        if ((entry = searchEntry(name, lop, rop, page->table, page->end)) != nullptr) {
            insn.setInsnCode(InsnTable::insnCode(page->prefix, pgm_read_byte(&entry->opc)));
            insn.setFlags(pgm_read_byte(&entry->flags1), pgm_read_byte(&entry->flags2));
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

static Error searchPages(
    Insn &insn, target::insn_t insnCode, const EntryPage *pages, const EntryPage *end) {
    for (const EntryPage *page = pages; page < end; page++) {
        if (InsnTable::prefixCode(insnCode) != page->prefix) continue;
        const Entry *entry =
            searchEntry(InsnTable::opCode(insnCode), page->table, page->end);
        if (entry) {
            insn.setFlags(pgm_read_byte(&entry->flags1), pgm_read_byte(&entry->flags2));
            char name[5];
            pgm_strcpy(name, entry->name);
            insn.setName(name);
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

#define ARRAY_END(array) &(array)[(sizeof(array) / sizeof(array[0]))]
#define ARRAY_RANGE(array) &array[0], ARRAY_END(array)

static const EntryPage PAGES[] = {
    { PREFIX_00, ARRAY_RANGE(TABLE_00) },
    { PREFIX_CB, ARRAY_RANGE(TABLE_CB) },
    { PREFIX_ED, ARRAY_RANGE(TABLE_ED) },
    { PREFIX_IX, ARRAY_RANGE(TABLE_IX) },
    { PREFIX_IY, ARRAY_RANGE(TABLE_IX) },
};

bool InsnTable::isPrefixCode(target::opcode_t opCode) {
    return opCode == PREFIX_CB || opCode == PREFIX_ED
        || opCode == PREFIX_IX || opCode == PREFIX_IY;
}

Error InsnTable::searchNameAndOprFormats(
    Insn &insn, OprFormat leftOpr, OprFormat rightOpr) const {
    return searchPages(insn, insn.name(), leftOpr, rightOpr, ARRAY_RANGE(PAGES));
}

Error InsnTable::searchInsnCode(Insn &insn) const {
    return searchPages(insn, insn.insnCode(), ARRAY_RANGE(PAGES));
}

RegName InsnTable::decodeIndexReg(target::insn_t insnCode) {
    const target::opcode_t prefix = prefixCode(insnCode);
    if (prefix == PREFIX_IX) return IX;
    if (prefix == PREFIX_IY) return IY;
    return NONE;
}

class InsnTable InsnTable;
