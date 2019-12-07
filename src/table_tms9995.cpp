#include <ctype.h>

#include "config_tms9995.h"

#include "entry_tms9995.h"
#include "table_tms9995.h"
#include "text_tms9995.h"

static constexpr Entry TABLE_TMS9995[] PROGMEM = {
    E(0x0080, TEXT_LST,  REG)
    E(0x0090, TEXT_LWP,  REG)
    E(0x0180, TEXT_DIVS, SRC)
    E(0x01C0, TEXT_MPYS, SRC)
    E(0x0200, TEXT_LI,   REG_IMM)
    E(0x0220, TEXT_AI,   REG_IMM)
    E(0x0240, TEXT_ANDI, REG_IMM)
    E(0x0260, TEXT_ORI,  REG_IMM)
    E(0x0280, TEXT_CI,   REG_IMM)
    E(0x02A0, TEXT_STWP, REG)
    E(0x02C0, TEXT_STST, REG)
    E(0x02E0, TEXT_LWPI, IMM)
    E(0x0300, TEXT_LIMI, IMM)
    E(0x0340, TEXT_IDLE, INH)
    E(0x0360, TEXT_RSET, INH)
    E(0x0380, TEXT_RTWP, INH)
    E(0x03A0, TEXT_CKON, INH)
    E(0x03C0, TEXT_CKOF, INH)
    E(0x03E0, TEXT_LREX, INH)
    E(0x0400, TEXT_BLWP, SRC)
    E(0x0440, TEXT_B,    SRC)
    E(0x0480, TEXT_X,    SRC)
    E(0x04C0, TEXT_CLR,  SRC)
    E(0x0500, TEXT_NEG,  SRC)
    E(0x0540, TEXT_INV,  SRC)
    E(0x0580, TEXT_INC,  SRC)
    E(0x05C0, TEXT_INCT, SRC)
    E(0x0600, TEXT_DEC,  SRC)
    E(0x0640, TEXT_DECT, SRC)
    E(0x0680, TEXT_BL,   SRC)
    E(0x06C0, TEXT_SWPB, SRC)
    E(0x0700, TEXT_SETO, SRC)
    E(0X0740, TEXT_ABS,  SRC)
    E(0x0800, TEXT_SRA,  CNT_REG)
    E(0x0900, TEXT_SRL,  CNT_REG)
    E(0x0A00, TEXT_SLA,  CNT_REG)
    E(0x0B00, TEXT_SRC,  CNT_REG)
    E(0x1000, TEXT_JMP,  REL)
    E(0x1100, TEXT_JLT,  REL)
    E(0x1200, TEXT_JLE,  REL)
    E(0x1300, TEXT_JEQ,  REL)
    E(0x1400, TEXT_JHE,  REL)
    E(0x1500, TEXT_JGT,  REL)
    E(0x1600, TEXT_JNE,  REL)
    E(0x1700, TEXT_JNC,  REL)
    E(0x1800, TEXT_JOC,  REL)
    E(0x1900, TEXT_JNO,  REL)
    E(0x1A00, TEXT_JL,   REL)
    E(0x1B00, TEXT_JH,   REL)
    E(0x1C00, TEXT_JOP,  REL)
    E(0x1D00, TEXT_SBO,  CRU_OFF)
    E(0x1E00, TEXT_SBZ,  CRU_OFF)
    E(0x1F00, TEXT_TB,   CRU_OFF)
    E(0x2000, TEXT_COC,  REG_SRC)
    E(0x2400, TEXT_CZC,  REG_SRC)
    E(0x2800, TEXT_XOR,  REG_SRC)
    E(0x2C00, TEXT_XOP,  XOP_SRC)
    E(0x3000, TEXT_LDCR, CNT_SRC)
    E(0x3400, TEXT_STCR, CNT_SRC)
    E(0x3800, TEXT_MPY,  REG_SRC)
    E(0x3C00, TEXT_DIV,  REG_SRC)
    E(0x4000, TEXT_SZC,  DST_SRC)
    E(0x5000, TEXT_SZCB, DST_SRC)
    E(0x6000, TEXT_S,    DST_SRC)
    E(0x7000, TEXT_SB,   DST_SRC)
    E(0x8000, TEXT_C,    DST_SRC)
    E(0x9000, TEXT_CB,   DST_SRC)
    E(0xA000, TEXT_A,    DST_SRC)
    E(0xB000, TEXT_AB,   DST_SRC)
    E(0xC000, TEXT_MOV,  DST_SRC)
    E(0xD000, TEXT_MOVB, DST_SRC)
    E(0xE000, TEXT_SOC,  DST_SRC)
    E(0xF000, TEXT_SOCB, DST_SRC)
};

static constexpr Entry TABLE_MID[] PROGMEM = {
    E(0x0000, TEXT_MID,  INH)
};

static const Entry *searchEntry(
    const char *name,
    const Entry *table, const Entry *end) {
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
    const target::insn_t insnCode,
    const Entry *table, const Entry *end) {
    for (const Entry *entry = table; entry < end; entry++) {
        target::insn_t i = insnCode;
        const AddrMode addrMode = _addrMode(pgm_read_byte(&entry->flags));
        switch (addrMode) {
        case REG:
        case REG_IMM: i &= ~0x000f; break;
        case SRC:     i &= ~0x003f; break;
        case REL:
        case CNT_REG:
        case CRU_OFF: i &= ~0x00ff; break;
        case CNT_SRC:
        case XOP_SRC:
        case REG_SRC: i &= ~0x03ff; break;
        case DST_SRC: i &= ~0x0fff; break;
        default: break;
        }
        if (i == pgm_read_word(&entry->insnCode))
            return entry;
    }
    return nullptr;
}

Error TableTms9995::searchName(Insn &insn) const {
    const char *name = insn.name();
    const Entry *entry = searchEntry(name, ARRAY_RANGE(TABLE_TMS9995));
    if (!entry) return UNKNOWN_INSTRUCTION;
    insn.setInsnCode(pgm_read_word(&entry->insnCode));
    insn.setFlags(pgm_read_byte(&entry->flags));
    insn.setName(name);
    return OK;
}

void TableTms9995::searchInsnCode(Insn &insn) const {
    const target::insn_t insnCode = insn.insnCode();
    const Entry *entry = searchEntry(insnCode, ARRAY_RANGE(TABLE_TMS9995));
    if (!entry)                 // Macro Instruction Detection
        entry = &TABLE_MID[0];
    char name[5];
    pgm_strncpy(name, entry->name, sizeof(Entry::name));
    name[4] = 0;
    insn.setName(name);
    insn.setFlags(pgm_read_byte(&entry->flags));
}

class TableTms9995 TableTms9995;
