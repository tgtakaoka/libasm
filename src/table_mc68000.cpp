#include <ctype.h>

#include "config_mc68000.h"

#include "entry_mc68000.h"
#include "table_mc68000.h"
#include "text_mc68000.h"

static constexpr Entry TABLE_MC68000[] PROGMEM = {
    E(0000000, ORI,   DEST_SIZ, EXT_IMM)  // 1 111 111 1SS MMM XXX SS:00=B,01=W,10=L (privileged to SR)
    E(0001000, ANDI,  DEST_SIZ, EXT_IMM)  // 1 111 111 1SS MMM XXX SS:00=B,01=W,10=L (privileged to SR)
    E(0002000, SUBI,  DEST_SIZ, EXT_IMM)  // 1 111 111 1SS MMM XXX SS:00=B,01=W,10=L
    E(0003000, ADDI,  DEST_SIZ, EXT_IMM)  // 1 111 111 1SS MMM XXX SS:00=B,01=W,10=L
    E(0005000, EORI,  DEST_SIZ, EXT_IMM)  // 1 111 111 1SS MMM XXX SS:00=B,01=W,10=L (privileged to SR)
    E(0006000, CMPI,  DEST_SIZ, EXT_IMM)  // 1 111 111 1SS MMM XXX SS:00=B,01=W,10=L
    E(0004000, BTST,  DEST_OPR, EXT_BIT)  // 1 111 111 111 MMM XXX
    E(0004100, BCHG,  DEST_OPR, EXT_BIT)  // 1 111 111 111 MMM XXX
    E(0004200, BCLR,  DEST_OPR, EXT_BIT)  // 1 111 111 111 MMM XXX
    E(0004300, BSET,  DEST_OPR, EXT_BIT)  // 1 111 111 111 MMM XXX
    E(0000410, MOVEP, MOVE_PER, EXT_DISP) // 1 111 DDD 1dS 111 AAA d:0=DtoM,1=MtoD S:0=W,1=L
    E(0000400, BTST,  DREG_DST, NO_EXT)   // 1 111 DDD 111 MMM XXX
    E(0000500, BCHG,  DREG_DST, NO_EXT)   // 1 111 DDD 111 MMM XXX
    E(0000600, BCLR,  DREG_DST, NO_EXT)   // 1 111 DDD 111 MMM XXX
    E(0000700, BSET,  DREG_DST, NO_EXT)   // 1 111 DDD 111 MMM XXX
    E(0020100, MOVEA, MOVA_OPR, NO_EXT)   // 1 111 AAA 111 MMM XXX SS:10=L
    E(0030100, MOVEA, MOVA_OPR, NO_EXT)   // 1 111 AAA 111 MMM XXX SS:11=W
    E(0010000, MOVE,  MOVE_OPR, NO_EXT)   // 1 111 XXX MMM MMM YYY SS:01=B
    E(0020000, MOVE,  MOVE_OPR, NO_EXT)   // 1 111 XXX MMM MMM YYY SS:10=L
    E(0030000, MOVE,  MOVE_OPR, NO_EXT)   // 1 111 XXX MMM MMM YYY SS:11=W
    E(0040300, MOVE,  MOVE_SR,  NO_EXT)   // 1 111 111 111 MMM XXX dd:00=fromSR
    E(0042300, MOVE,  MOVE_SR,  NO_EXT)   // 1 111 111 111 MMM XXX dd:10=toCCR
    E(0043300, MOVE,  MOVE_SR,  NO_EXT)   // 1 111 111 111 MMM XXX dd:11=toSR
    E(0040000, NEGX,  DEST_SIZ, NO_EXT)   // 1 111 111 1SS MMM XXX SS:00=B,01=W,10=L
    E(0041000, CLR,   DEST_SIZ, NO_EXT)   // 1 111 111 1SS MMM XXX SS:00=B,01=W,10=L
    E(0042000, NEG,   DEST_SIZ, NO_EXT)   // 1 111 111 1SS MMM XXX SS:00=B,01=W,10=L
    E(0043000, NOT,   DEST_SIZ, NO_EXT)   // 1 111 111 1SS MMM XXX SS:00=B,01=W,10=L
    E(0044200, EXT,   SIGN_EXT, NO_EXT)   // 1 111 111 11S 111 DDD S:0=W,1=L
    E(0044000, NBCD,  DATA_DST, NO_EXT)   // 1 111 111 111 MMM DDD
    E(0044100, SWAP,  DATA_REG, NO_EXT)   // 1 111 111 111 111 DDD
    E(0044100, PEA,   DATA_DST, NO_EXT)   // 1 111 111 111 MMM DDD
    E(0045374, ILLEGAL,IMPLIED, NO_EXT)   // 1 111 111 111 111 111
    E(0045300, TAS,   DATA_DST, NO_EXT)   // 1 111 111 111 MMM DDD
    E(0045000, TST,   DEST_SIZ, NO_EXT)   // 1 111 111 1SS MMM XXX SS:00=B,01=W,10=L
    E(0047100, TRAP,  TRAP_VEC, NO_EXT)   // 1 111 111 111 11V VVV
    E(0047120, LINK,  ADDR_REG, EXT_DISP) // 1 111 111 111 111 AAA
    E(0047130, UNLK,  ADDR_REG, NO_EXT)   // 1 111 111 111 111 AAA
    E(0047140, MOVE,  MOVE_USP, NO_EXT)   // 1 111 111 111 11d AAA d:0=AtoU,1=UtoA
    E(0047160, RESET, IMPLIED,  NO_EXT)   // 1 111 111 111 111 111
    E(0047161, NOP,   IMPLIED,  NO_EXT)   // 1 111 111 111 111 111
    E(0047162, STOP,  IMPLIED,  EXT_IMM)  // 1 111 111 111 111 111 (privileged)
    E(0047163, RTE,   IMPLIED,  NO_EXT)   // 1 111 111 111 111 111 (privileged)
    E(0047165, RTS,   IMPLIED,  NO_EXT)   // 1 111 111 111 111 111
    E(0047166, TRAPV, IMPLIED,  NO_EXT)   // 1 111 111 111 111 111
    E(0047167, RTR,   IMPLIED,  NO_EXT)   // 1 111 111 111 111 111
    E(0047200, JSR,   DEST_OPR, NO_EXT)   // 1 111 111 111 MMM XXX
    E(0047300, JMP,   DEST_OPR, NO_EXT)   // 1 111 111 111 MMM XXX
    E(0044200, MOVEM, MOVE_MLT, EXT_REGM) // 1 111 1d1 11S MMM XXX d:0=RtoM,1=MtoR
    E(0040700, LEA,   AREG_LNG, NO_EXT)   // 1 111 AAA 111 MMM XXX
    E(0040600, CHK,   DREG_DST, NO_EXT)   // 1 111 DDD 111 MMM XXX
    E(0050310, DBT,   DATA_REG, EXT_DBCC) // 1 111 111 111 111 DDD
    E(0050710, DBRA,  DATA_REG, EXT_DBCC) // 1 111 111 111 111 DDD
    E(0050710, DBF,   DATA_REG, EXT_DBCC) // 1 111 111 111 111 DDD
    E(0051310, DBHI,  DATA_REG, EXT_DBCC) // 1 111 111 111 111 DDD
    E(0051710, DBLS,  DATA_REG, EXT_DBCC) // 1 111 111 111 111 DDD
    E(0052310, DBCC,  DATA_REG, EXT_DBCC) // 1 111 111 111 111 DDD
    E(0052310, DBHS,  DATA_REG, EXT_DBCC) // 1 111 111 111 111 DDD
    E(0052710, DBCS,  DATA_REG, EXT_DBCC) // 1 111 111 111 111 DDD
    E(0052710, DBLO,  DATA_REG, EXT_DBCC) // 1 111 111 111 111 DDD
    E(0053310, DBNE,  DATA_REG, EXT_DBCC) // 1 111 111 111 111 DDD
    E(0053710, DBEQ,  DATA_REG, EXT_DBCC) // 1 111 111 111 111 DDD
    E(0054310, DBVC,  DATA_REG, EXT_DBCC) // 1 111 111 111 111 DDD
    E(0054710, DBVS,  DATA_REG, EXT_DBCC) // 1 111 111 111 111 DDD
    E(0055310, DBPL,  DATA_REG, EXT_DBCC) // 1 111 111 111 111 DDD
    E(0055710, DBMI,  DATA_REG, EXT_DBCC) // 1 111 111 111 111 DDD
    E(0056310, DBGE,  DATA_REG, EXT_DBCC) // 1 111 111 111 111 DDD
    E(0056710, DBLT,  DATA_REG, EXT_DBCC) // 1 111 111 111 111 DDD
    E(0057310, DBGT,  DATA_REG, EXT_DBCC) // 1 111 111 111 111 DDD
    E(0057710, DBLE,  DATA_REG, EXT_DBCC) // 1 111 111 111 111 DDD
    E(0050300, ST,    DEST_OPR, NO_EXT)   // 1 111 111 111 MMM XXX
    E(0050700, SF,    DEST_OPR, NO_EXT)   // 1 111 111 111 MMM XXX
    E(0051300, SHI,   DEST_OPR, NO_EXT)   // 1 111 111 111 MMM XXX
    E(0051700, SLS,   DEST_OPR, NO_EXT)   // 1 111 111 111 MMM XXX
    E(0052300, SCC,   DEST_OPR, NO_EXT)   // 1 111 111 111 MMM XXX
    E(0052300, SHS,   DEST_OPR, NO_EXT)   // 1 111 111 111 MMM XXX
    E(0052700, SCS,   DEST_OPR, NO_EXT)   // 1 111 111 111 MMM XXX
    E(0052700, SLO,   DEST_OPR, NO_EXT)   // 1 111 111 111 MMM XXX
    E(0053300, SNE,   DEST_OPR, NO_EXT)   // 1 111 111 111 MMM XXX
    E(0053700, SEQ,   DEST_OPR, NO_EXT)   // 1 111 111 111 MMM XXX
    E(0054300, SVC,   DEST_OPR, NO_EXT)   // 1 111 111 111 MMM XXX
    E(0054700, SVS,   DEST_OPR, NO_EXT)   // 1 111 111 111 MMM XXX
    E(0055300, SPL,   DEST_OPR, NO_EXT)   // 1 111 111 111 MMM XXX
    E(0055700, SMI,   DEST_OPR, NO_EXT)   // 1 111 111 111 MMM XXX
    E(0056300, SGE,   DEST_OPR, NO_EXT)   // 1 111 111 111 MMM XXX
    E(0056700, SLT,   DEST_OPR, NO_EXT)   // 1 111 111 111 MMM XXX
    E(0057300, SGT,   DEST_OPR, NO_EXT)   // 1 111 111 111 MMM XXX
    E(0057700, SLE,   DEST_OPR, NO_EXT)   // 1 111 111 111 MMM XXX
    E(0050000, ADDQ,  DATA_QIC, NO_EXT)   // 1 111 ddd 1SS MMM XXX ddd:0=8 SS:00=B,01=W,10=L
    E(0050400, SUBQ,  DATA_QIC, NO_EXT)   // 1 111 ddd 1SS MMM XXX ddd:0=8 SS:00=B,01=W,10=L
    E(0060000, BRA,   RELATIVE, EXT_BRA)  // 1 111 111 1dd ddd ddd
    E(0060000, BT,    RELATIVE, EXT_BRA)  // 1 111 111 1dd ddd ddd
    E(0060400, BSR,   RELATIVE, EXT_BRA)  // 1 111 111 1dd ddd ddd
    E(0061000, BHI,   RELATIVE, EXT_BRA)  // 1 111 111 1dd ddd ddd
    E(0061400, BLS,   RELATIVE, EXT_BRA)  // 1 111 111 1dd ddd ddd
    E(0062000, BCC,   RELATIVE, EXT_BRA)  // 1 111 111 1dd ddd ddd
    E(0062000, BHS,   RELATIVE, EXT_BRA)  // 1 111 111 1dd ddd ddd
    E(0062400, BCS,   RELATIVE, EXT_BRA)  // 1 111 111 1dd ddd ddd
    E(0062400, BLO,   RELATIVE, EXT_BRA)  // 1 111 111 1dd ddd ddd
    E(0063000, BNE,   RELATIVE, EXT_BRA)  // 1 111 111 1dd ddd ddd
    E(0063400, BEQ,   RELATIVE, EXT_BRA)  // 1 111 111 1dd ddd ddd
    E(0064000, BVC,   RELATIVE, EXT_BRA)  // 1 111 111 1dd ddd ddd
    E(0064400, BVS,   RELATIVE, EXT_BRA)  // 1 111 111 1dd ddd ddd
    E(0065000, BPL,   RELATIVE, EXT_BRA)  // 1 111 111 1dd ddd ddd
    E(0065400, BMI,   RELATIVE, EXT_BRA)  // 1 111 111 1dd ddd ddd
    E(0066000, BGE,   RELATIVE, EXT_BRA)  // 1 111 111 1dd ddd ddd
    E(0066400, BLT,   RELATIVE, EXT_BRA)  // 1 111 111 1dd ddd ddd
    E(0067000, BGT,   RELATIVE, EXT_BRA)  // 1 111 111 1dd ddd ddd
    E(0067400, BLE,   RELATIVE, EXT_BRA)  // 1 111 111 1dd ddd ddd
    E(0070000, MOVEQ, MOVE_QIC, NO_EXT)   // 1 111 DDD 1dd ddd ddd
    E(0100400, SBCD,  DMEM_DST, NO_EXT)   // 1 111 XXX 111 11R YYY R:0=D,1=M
    E(0100300, DIVU,  DREG_DST, NO_EXT)   // 1 111 DDD 111 MMM XXX
    E(0100700, DIVS,  DREG_DST, NO_EXT)   // 1 111 DDD 111 MMM XXX
    E(0100000, OR,    DMEM_SIZ, NO_EXT)   // 1 111 DDD dSS MMM XXX d:0=toD,1=Dto SS:00=B,01=W,10=L
    E(0110300, SUBA,  AREG_SIZ, NO_EXT)   // 1 111 AAA S11 MMM XXX S:0=W,1=L
    E(0110400, SUBX,  DMEM_OPR, NO_EXT)   // 1 111 XXX 1SS 11R YYY R:0=D,1=M SS:00=B,01=W,10=L
    E(0110000, SUB,   DMEM_SIZ, NO_EXT)   // 1 111 DDD dSS MMM XXX d:0=toD,1=Dto SS:00=B,01=W,10=L
    E(0130300, CMPA,  AREG_SIZ, NO_EXT)   // 1 111 AAA S11 MMM XXX S:0=W,1=L
    E(0130410, CMPM,  CMPM_SIZ, NO_EXT)   // 1 111 XXX 1SS 111 YYY SS:00=B,01=W,10=L
    E(0130000, CMP,   DREG_SIZ, NO_EXT)   // 1 111 DDD 1SS MMM XXX SS:00=B,01=W,10=L
    E(0130400, EOR,   DREG_SIZ, NO_EXT)   // 1 111 DDD 1SS MMM XXX SS:00=B,01=W,10=L
    E(0140300, MULU,  DREG_DST, NO_EXT)   // 1 111 DDD 111 MMM XXX
    E(0140700, MULS,  DREG_DST, NO_EXT)   // 1 111 DDD 111 MMM XXX
    E(0140400, ABCD,  DMEM_DST, NO_EXT)   // 1 111 XXX 111 11R YYY R:0=D,1=M
    E(0140500, EXG,   REGS_EXG, NO_EXT)   // 1 111 XXX 1rr 11r YYY rrr:010=DD
    E(0140510, EXG,   REGS_EXG, NO_EXT)   // 1 111 XXX 1rr 11r YYY rrr:011=AA
    E(0140610, EXG,   REGS_EXG, NO_EXT)   // 1 111 XXX 1rr 11r YYY rrr:101=DA
    E(0140000, AND,   DMEM_SIZ, NO_EXT)   // 1 111 DDD dSS MMM XXX d:0=toD,1=Dto SS:00=B,01=W,10=L
    E(0150300, ADDA,  AREG_SIZ, NO_EXT)   // 1 111 AAA S11 MMM XXX S:0=W,1=L
    E(0150400, ADDX,  DMEM_OPR, NO_EXT)   // 1 111 XXX 1SS 11R YYY R:0=D,1=M SS:00=B,01=W,10=L
    E(0150000, ADD,   DMEM_SIZ, NO_EXT)   // 1 111 DDD dSS MMM XXX d:0=toD,1=Dto SS:00=B,01=W,10=L
    E(0160300, ASR,   DEST_OPR, NO_EXT)   // 1 111 111 111 MMM XXX
    E(0160700, ASL,   DEST_OPR, NO_EXT)   // 1 111 111 111 MMM XXX
    E(0161300, LSR,   DEST_OPR, NO_EXT)   // 1 111 111 111 MMM XXX
    E(0161700, LSL,   DEST_OPR, NO_EXT)   // 1 111 111 111 MMM XXX
    E(0162300, ROXR,  DEST_OPR, NO_EXT)   // 1 111 111 111 MMM XXX
    E(0162700, ROXL,  DEST_OPR, NO_EXT)   // 1 111 111 111 MMM XXX
    E(0163300, ROR,   DEST_OPR, NO_EXT)   // 1 111 111 111 MMM XXX
    E(0163700, ROL,   DEST_OPR, NO_EXT)   // 1 111 111 111 MMM XXX
    E(0160000, ASR,   DREG_ROT, NO_EXT)   // 1 111 rrr 1SS d11 DDD d:0=Ir/1=Dr
    E(0160400, ASL,   DREG_ROT, NO_EXT)   // 1 111 rrr 1SS d11 DDD d:0=Ir/1=Dr
    E(0160010, LSR,   DREG_ROT, NO_EXT)   // 1 111 rrr 1SS d11 DDD d:0=Ir/1=Dr
    E(0160410, LSL,   DREG_ROT, NO_EXT)   // 1 111 rrr 1SS d11 DDD d:0=Ir/1=Dr
    E(0160020, ROXR,  DREG_ROT, NO_EXT)   // 1 111 rrr 1SS d11 DDD d:0=Ir/1=Dr
    E(0160420, ROXL,  DREG_ROT, NO_EXT)   // 1 111 rrr 1SS d11 DDD d:0=Ir/1=Dr
    E(0160030, ROR,   DREG_ROT, NO_EXT)   // 1 111 rrr 1SS d11 DDD d:0=Ir/1=Dr
    E(0160430, ROL,   DREG_ROT, NO_EXT)   // 1 111 rrr 1SS d11 DDD d:0=Ir/1=Dr
};

static target::insn_t getInsnMask(InsnFormat iformat) {
    switch (iformat) {
    case MOVA_OPR: return 007077;
    case MOVE_OPR:
    case DMEM_SIZ: return 007777;
    case AREG_SIZ: return 007477;
    case DATA_QIC:
    case DREG_SIZ:
    case MOVE_QIC: return 007377;
    case DREG_ROT: return 007347;
    case DMEM_OPR: return 007317;
    case CMPM_SIZ:
    case MOVE_PER: return 007307;
    case AREG_LNG:
    case DREG_DST: return 007077;
    case DMEM_DST: return 007017;
    case REGS_EXG: return 007007;
    case MOVE_MLT: return 002177;
    case DEST_SIZ:
    case RELATIVE: return 000377;
    case SIGN_EXT: return 000107;
    case MOVE_SR:
    case DATA_DST:
    case DEST_OPR: return 000077;
    case MOVE_USP:
    case TRAP_VEC: return 000017;
    case ADDR_REG:
    case DATA_REG: return 000007;
    default:       return 000000; // IMPLIED
    }
}

static const Entry *searchEntry(
    const char *name,
    const Entry *table, const Entry *end) {
    for (const Entry *entry = table; entry < end; entry++) {
        if (pgm_strcasecmp(name, entry->name) == 0)
            return entry;
    }
    return nullptr;
}

static const Entry *searchEntry(
    const target::insn_t insnCode,
    const Entry *table, const Entry *end) {
    for (const Entry *entry = table; entry < end; entry++) {
        const host::uint_t flags = pgm_read_byte(&entry->flags);
        const target::insn_t mask = getInsnMask(_insnFormat(flags));
        if ((insnCode & ~mask) == pgm_read_word(&entry->insnCode))
            return entry;
    }
    return nullptr;
}

Error TableMc68000::searchName(Insn &insn) const {
    const char *name = insn.name();
    const Entry *entry = searchEntry(name, ARRAY_RANGE(TABLE_MC68000));
    if (!entry) return UNKNOWN_INSTRUCTION;
    insn.setInsnCode(pgm_read_word(&entry->insnCode));
    insn.setFlags(pgm_read_byte(&entry->flags));
    insn.setName(name);
    return OK;
}

Error TableMc68000::searchInsnCode(Insn &insn) const {
    const target::insn_t insnCode = insn.insnCode();
    const Entry *entry = searchEntry(insnCode, ARRAY_RANGE(TABLE_MC68000));
    if (!entry) return UNKNOWN_INSTRUCTION;
    insn.setFlags(pgm_read_byte(&entry->flags));
    char name[8];
    pgm_strcpy(name, entry->name);
    insn.setName(name);
    return OK;
}

class TableMc68000 TableMc68000;
