#include <ctype.h>

#include "config_i8080.h"

#include "entry_i8080.h"
#include "table_i8080.h"
#include "text_i8080.h"

static const Entry TABLE_I8080[] PROGMEM = {
    E(0x00, TEXT_NOP,  INHERENT,     NO_FORMAT)
    E(0x01, TEXT_LXI,  IMMEDIATE_16, POINTER_REG)
    E(0x09, TEXT_DAD,  INHERENT,     POINTER_REG)
    E(0x02, TEXT_STAX, INHERENT,     INDEX_REG)
    E(0x22, TEXT_SHLD, DIRECT,       NO_FORMAT)
    E(0x32, TEXT_STA,  DIRECT,       NO_FORMAT)
    E(0x0A, TEXT_LDAX, INHERENT,     INDEX_REG)
    E(0x2A, TEXT_LHLD, DIRECT,       NO_FORMAT)
    E(0x3A, TEXT_LDA,  DIRECT,       NO_FORMAT)
    E(0x03, TEXT_INX,  INHERENT,     POINTER_REG)
    E(0x0B, TEXT_DCX,  INHERENT,     POINTER_REG)
    E(0x04, TEXT_INR,  INHERENT,     DATA_REG)
    E(0x05, TEXT_DCR,  INHERENT,     DATA_REG)
    E(0x06, TEXT_MVI,  IMMEDIATE_8,  DATA_REG)
    E(0x07, TEXT_RLC,  INHERENT,     NO_FORMAT)
    E(0x0F, TEXT_RRC,  INHERENT,     NO_FORMAT)
    E(0x17, TEXT_RAL,  INHERENT,     NO_FORMAT)
    E(0x1F, TEXT_RAR,  INHERENT,     NO_FORMAT)
    E(0x27, TEXT_DAA,  INHERENT,     NO_FORMAT)
    E(0x2F, TEXT_CMA,  INHERENT,     NO_FORMAT)
    E(0x37, TEXT_STC,  INHERENT,     NO_FORMAT)
    E(0x3F, TEXT_CMC,  INHERENT,     NO_FORMAT)
    E(0x76, TEXT_HLT,  INHERENT,     NO_FORMAT)
    E(0x40, TEXT_MOV,  INHERENT,     DATA_DATA_REG)
    E(0x80, TEXT_ADD,  INHERENT,     LOW_DATA_REG)
    E(0x88, TEXT_ADC,  INHERENT,     LOW_DATA_REG)
    E(0x90, TEXT_SUB,  INHERENT,     LOW_DATA_REG)
    E(0x98, TEXT_SBB,  INHERENT,     LOW_DATA_REG)
    E(0xA0, TEXT_ANA,  INHERENT,     LOW_DATA_REG)
    E(0xA8, TEXT_XRA,  INHERENT,     LOW_DATA_REG)
    E(0xB0, TEXT_ORA,  INHERENT,     LOW_DATA_REG)
    E(0xB8, TEXT_CMP,  INHERENT,     LOW_DATA_REG)
    E(0xC0, TEXT_RNZ,  INHERENT,     NO_FORMAT)
    E(0xC8, TEXT_RZ,   INHERENT,     NO_FORMAT)
    E(0xD0, TEXT_RNC,  INHERENT,     NO_FORMAT)
    E(0xD8, TEXT_RC,   INHERENT,     NO_FORMAT)
    E(0xE0, TEXT_RPO,  INHERENT,     NO_FORMAT)
    E(0xE8, TEXT_RPE,  INHERENT,     NO_FORMAT)
    E(0xF0, TEXT_RP,   INHERENT,     NO_FORMAT)
    E(0xF8, TEXT_RM,   INHERENT,     NO_FORMAT)
    E(0xC1, TEXT_POP,  INHERENT,     STACK_REG)
    E(0xC9, TEXT_RET,  INHERENT,     NO_FORMAT)
    E(0xE9, TEXT_PCHL, INHERENT,     NO_FORMAT)
    E(0xF9, TEXT_SPHL, INHERENT,     NO_FORMAT)
    E(0xC2, TEXT_JNZ,  DIRECT,       NO_FORMAT)
    E(0xCA, TEXT_JZ,   DIRECT,       NO_FORMAT)
    E(0xD2, TEXT_JNC,  DIRECT,       NO_FORMAT)
    E(0xDA, TEXT_JC,   DIRECT,       NO_FORMAT)
    E(0xE2, TEXT_JPO,  DIRECT,       NO_FORMAT)
    E(0xEA, TEXT_JPE,  DIRECT,       NO_FORMAT)
    E(0xF2, TEXT_JP,   DIRECT,       NO_FORMAT)
    E(0xFA, TEXT_JM,   DIRECT,       NO_FORMAT)
    E(0xC3, TEXT_JMP,  DIRECT,       NO_FORMAT)
    E(0xD3, TEXT_OUT,  IOADDR,       NO_FORMAT)
    E(0xDB, TEXT_IN,   IOADDR,       NO_FORMAT)
    E(0xE3, TEXT_XTHL, INHERENT,     NO_FORMAT)
    E(0xEB, TEXT_XCHG, INHERENT,     NO_FORMAT)
    E(0xF3, TEXT_DI,   INHERENT,     NO_FORMAT)
    E(0xFB, TEXT_EI,   INHERENT,     NO_FORMAT)
    E(0xC4, TEXT_CNZ,  DIRECT,       NO_FORMAT)
    E(0xCC, TEXT_CZ,   DIRECT,       NO_FORMAT)
    E(0xD4, TEXT_CNC,  DIRECT,       NO_FORMAT)
    E(0xDC, TEXT_CC,   DIRECT,       NO_FORMAT)
    E(0xE4, TEXT_CPO,  DIRECT,       NO_FORMAT)
    E(0xEC, TEXT_CPE,  DIRECT,       NO_FORMAT)
    E(0xF4, TEXT_CP,   DIRECT,       NO_FORMAT)
    E(0xFC, TEXT_CM,   DIRECT,       NO_FORMAT)
    E(0xC5, TEXT_PUSH, INHERENT,     STACK_REG)
    E(0xCD, TEXT_CALL, DIRECT,       NO_FORMAT)
    E(0xC6, TEXT_ADI,  IMMEDIATE_8,  NO_FORMAT)
    E(0xCE, TEXT_ACI,  IMMEDIATE_8,  NO_FORMAT)
    E(0xD6, TEXT_SUI,  IMMEDIATE_8,  NO_FORMAT)
    E(0xDE, TEXT_SBI,  IMMEDIATE_8,  NO_FORMAT)
    E(0xE6, TEXT_ANI,  IMMEDIATE_8,  NO_FORMAT)
    E(0xEE, TEXT_XRI,  IMMEDIATE_8,  NO_FORMAT)
    E(0xF6, TEXT_ORI,  IMMEDIATE_8,  NO_FORMAT)
    E(0xFE, TEXT_CPI,  IMMEDIATE_8,  NO_FORMAT)
    E(0xC7, TEXT_RST,  INHERENT,     VECTOR_NO)
};
constexpr host::uindex_t LENGTH_I8080 =
    sizeof(TABLE_I8080) / sizeof(TABLE_I8080[0]);

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
        const InsnFormat iformat = _insnFormat(pgm_read_byte(&entry->flags));
        switch (iformat) {
        case INDEX_REG: i &= ~0x10; break;
        case POINTER_REG:
        case STACK_REG: i &= ~0x30; break;
        case DATA_REG:
        case VECTOR_NO: i &= ~0x38; break;
        case LOW_DATA_REG: i &= ~0x07; break;
        case DATA_DATA_REG: i &= ~0x3F; break;
        default: break;
        }
        if (i == pgm_read_byte(&entry->insnCode))
            return entry;
    }
    return nullptr;
}

Error InsnTable::searchName(Insn &insn) const {
    const char *name = insn.name();
    const Entry *entry =
        searchEntry(name, &TABLE_I8080[0], &TABLE_I8080[LENGTH_I8080]);
    if (!entry) return UNKNOWN_INSTRUCTION;
    insn.setInsnCode(pgm_read_byte(&entry->insnCode));
    insn.setFlags(pgm_read_byte(&entry->flags));
    insn.setName(name);
    return OK;
}

Error InsnTable::searchInsnCode(Insn &insn) const {
    const target::insn_t insnCode = insn.insnCode();
    const Entry *entry =
        searchEntry(insnCode, &TABLE_I8080[0], &TABLE_I8080[LENGTH_I8080]);
    if (!entry) return UNKNOWN_INSTRUCTION;
    insn.setFlags(pgm_read_byte(&entry->flags));
    char name[5];
    pgm_strncpy(name, entry->name, sizeof(Entry::name));
    name[4] = 0;
    insn.setName(name);
    return OK;
}

class InsnTable InsnTable;
