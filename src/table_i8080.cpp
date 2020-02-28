#include "config_i8080.h"

#include "entry_i8080.h"
#include "table_i8080.h"
#include "text_i8080.h"

#include <ctype.h>
#include <string.h>

#define E(_opc, _name, _amode, _iformat)                    \
    { _opc, Entry::_flags(_amode, _iformat), { _name } },

static constexpr Entry TABLE_I8080[] PROGMEM = {
    E(0x00, TEXT_NOP,  INHR,   NO_FORMAT)
    E(0x01, TEXT_LXI,  IMM16,  POINTER_REG)
    E(0x09, TEXT_DAD,  INHR,   POINTER_REG)
    E(0x02, TEXT_STAX, INHR,   INDEX_REG)
    E(0x22, TEXT_SHLD, DIRECT, NO_FORMAT)
    E(0x32, TEXT_STA,  DIRECT, NO_FORMAT)
    E(0x0A, TEXT_LDAX, INHR,   INDEX_REG)
    E(0x2A, TEXT_LHLD, DIRECT, NO_FORMAT)
    E(0x3A, TEXT_LDA,  DIRECT, NO_FORMAT)
    E(0x03, TEXT_INX,  INHR,   POINTER_REG)
    E(0x0B, TEXT_DCX,  INHR,   POINTER_REG)
    E(0x04, TEXT_INR,  INHR,   DATA_REG)
    E(0x05, TEXT_DCR,  INHR,   DATA_REG)
    E(0x06, TEXT_MVI,  IMM8,   DATA_REG)
    E(0x07, TEXT_RLC,  INHR,   NO_FORMAT)
    E(0x0F, TEXT_RRC,  INHR,   NO_FORMAT)
    E(0x17, TEXT_RAL,  INHR,   NO_FORMAT)
    E(0x1F, TEXT_RAR,  INHR,   NO_FORMAT)
    E(0x27, TEXT_DAA,  INHR,   NO_FORMAT)
    E(0x2F, TEXT_CMA,  INHR,   NO_FORMAT)
    E(0x37, TEXT_STC,  INHR,   NO_FORMAT)
    E(0x3F, TEXT_CMC,  INHR,   NO_FORMAT)
    E(0x76, TEXT_HLT,  INHR,   NO_FORMAT)
    E(0x40, TEXT_MOV,  INHR,   DATA_DATA_REG)
    E(0x80, TEXT_ADD,  INHR,   LOW_DATA_REG)
    E(0x88, TEXT_ADC,  INHR,   LOW_DATA_REG)
    E(0x90, TEXT_SUB,  INHR,   LOW_DATA_REG)
    E(0x98, TEXT_SBB,  INHR,   LOW_DATA_REG)
    E(0xA0, TEXT_ANA,  INHR,   LOW_DATA_REG)
    E(0xA8, TEXT_XRA,  INHR,   LOW_DATA_REG)
    E(0xB0, TEXT_ORA,  INHR,   LOW_DATA_REG)
    E(0xB8, TEXT_CMP,  INHR,   LOW_DATA_REG)
    E(0xC0, TEXT_RNZ,  INHR,   NO_FORMAT)
    E(0xC8, TEXT_RZ,   INHR,   NO_FORMAT)
    E(0xD0, TEXT_RNC,  INHR,   NO_FORMAT)
    E(0xD8, TEXT_RC,   INHR,   NO_FORMAT)
    E(0xE0, TEXT_RPO,  INHR,   NO_FORMAT)
    E(0xE8, TEXT_RPE,  INHR,   NO_FORMAT)
    E(0xF0, TEXT_RP,   INHR,   NO_FORMAT)
    E(0xF8, TEXT_RM,   INHR,   NO_FORMAT)
    E(0xC1, TEXT_POP,  INHR,   STACK_REG)
    E(0xC9, TEXT_RET,  INHR,   NO_FORMAT)
    E(0xE9, TEXT_PCHL, INHR,   NO_FORMAT)
    E(0xF9, TEXT_SPHL, INHR,   NO_FORMAT)
    E(0xC2, TEXT_JNZ,  DIRECT, NO_FORMAT)
    E(0xCA, TEXT_JZ,   DIRECT, NO_FORMAT)
    E(0xD2, TEXT_JNC,  DIRECT, NO_FORMAT)
    E(0xDA, TEXT_JC,   DIRECT, NO_FORMAT)
    E(0xE2, TEXT_JPO,  DIRECT, NO_FORMAT)
    E(0xEA, TEXT_JPE,  DIRECT, NO_FORMAT)
    E(0xF2, TEXT_JP,   DIRECT, NO_FORMAT)
    E(0xFA, TEXT_JM,   DIRECT, NO_FORMAT)
    E(0xC3, TEXT_JMP,  DIRECT, NO_FORMAT)
    E(0xD3, TEXT_OUT,  IOADR,  NO_FORMAT)
    E(0xDB, TEXT_IN,   IOADR,  NO_FORMAT)
    E(0xE3, TEXT_XTHL, INHR,   NO_FORMAT)
    E(0xEB, TEXT_XCHG, INHR,   NO_FORMAT)
    E(0xF3, TEXT_DI,   INHR,   NO_FORMAT)
    E(0xFB, TEXT_EI,   INHR,   NO_FORMAT)
    E(0xC4, TEXT_CNZ,  DIRECT, NO_FORMAT)
    E(0xCC, TEXT_CZ,   DIRECT, NO_FORMAT)
    E(0xD4, TEXT_CNC,  DIRECT, NO_FORMAT)
    E(0xDC, TEXT_CC,   DIRECT, NO_FORMAT)
    E(0xE4, TEXT_CPO,  DIRECT, NO_FORMAT)
    E(0xEC, TEXT_CPE,  DIRECT, NO_FORMAT)
    E(0xF4, TEXT_CP,   DIRECT, NO_FORMAT)
    E(0xFC, TEXT_CM,   DIRECT, NO_FORMAT)
    E(0xC5, TEXT_PUSH, INHR,   STACK_REG)
    E(0xCD, TEXT_CALL, DIRECT, NO_FORMAT)
    E(0xC6, TEXT_ADI,  IMM8,   NO_FORMAT)
    E(0xCE, TEXT_ACI,  IMM8,   NO_FORMAT)
    E(0xD6, TEXT_SUI,  IMM8,   NO_FORMAT)
    E(0xDE, TEXT_SBI,  IMM8,   NO_FORMAT)
    E(0xE6, TEXT_ANI,  IMM8,   NO_FORMAT)
    E(0xEE, TEXT_XRI,  IMM8,   NO_FORMAT)
    E(0xF6, TEXT_ORI,  IMM8,   NO_FORMAT)
    E(0xFE, TEXT_CPI,  IMM8,   NO_FORMAT)
    E(0xC7, TEXT_RST,  INHR,   VECTOR_NO)
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
        const InsnFormat iformat =
            Entry::_insnFormat(pgm_read_byte(&entry->flags));
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

Error TableI8080::searchName(Insn &insn) const {
    const char *name = insn.name();
    const Entry *entry = searchEntry(name, ARRAY_RANGE(TABLE_I8080));
    if (!entry) return UNKNOWN_INSTRUCTION;
    insn.setInsnCode(pgm_read_byte(&entry->insnCode));
    insn.setFlags(pgm_read_byte(&entry->flags));
    insn.setName(name);
    return OK;
}

Error TableI8080::searchInsnCode(Insn &insn) const {
    const target::insn_t insnCode = insn.insnCode();
    const Entry *entry = searchEntry(insnCode, ARRAY_RANGE(TABLE_I8080));
    if (!entry) return UNKNOWN_INSTRUCTION;
    insn.setFlags(pgm_read_byte(&entry->flags));
    char name[Entry::name_max + 1];
    pgm_strncpy(name, entry->name, sizeof(Entry::name));
    name[Entry::name_max] = 0;
    insn.setName(name);
    return OK;
}

const char *TableI8080::listCpu() {
    return "[i]8080";
}

bool TableI8080::setCpu(const char *cpu) {
    if (toupper(*cpu) == 'I') cpu++;
    return strcasecmp(cpu, "8080") == 0;
}

class TableI8080 TableI8080;
