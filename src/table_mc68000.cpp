/*
 * Copyright 2020 Tadashi G. Takaoka
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "config_mc68000.h"

#include "entry_mc68000.h"
#include "table_mc68000.h"
#include "text_mc68000.h"

#include <string.h>

namespace libasm {
namespace mc68000 {

#define E(_opc, _name, _iformat)                        \
    { _opc,  Entry::_flags(_iformat), TEXT_##_name },

static constexpr Entry TABLE_MC68000[] PROGMEM = {
    E(0000000, ORI,   DEST_SIZ)
    E(0001000, ANDI,  DEST_SIZ)
    E(0002000, SUBI,  DEST_SIZ)
    E(0003000, ADDI,  DEST_SIZ)
    E(0005000, EORI,  DEST_SIZ)
    E(0006000, CMPI,  DEST_SIZ)
    E(0004000, BTST,  DEST_OPR) // BTST #,<ea>
    E(0004100, BCHG,  DEST_OPR) // BCHG #,<ea>
    E(0004200, BCLR,  DEST_OPR) // BCLR #,<ea>
    E(0004300, BSET,  DEST_OPR) // BSET #,<ea>
    E(0000410, MOVEP, MOVE_PER)
    E(0000400, BTST,  DREG_DST) // BTST Dn,<ea>
    E(0000500, BCHG,  DREG_DST) // BCHG Dn,<ea>
    E(0000600, BCLR,  DREG_DST) // BCLR Dn,<ea>
    E(0000700, BSET,  DREG_DST) // BSET Dn,<ea>
    E(0020100, MOVEA, MOVA_OPR) // MOVEA.L <ea>,An
    E(0030100, MOVEA, MOVA_OPR) // MOVEA.W <ea>,An
    E(0010000, MOVE,  MOVE_OPR) // MOVE.B <ea>,<ea>
    E(0020000, MOVE,  MOVE_OPR) // MOVE.W <ea>,<ea>
    E(0030000, MOVE,  MOVE_OPR) // MOVE.L <ea>,<ea>
    E(0040300, MOVE,  MOVE_SR)  // MOVE   SR,<ea>
    E(0042300, MOVE,  MOVE_SR)  // MOVE   <ea>,CCR
    E(0043300, MOVE,  MOVE_SR)  // MOVE   <ea>,SR
    E(0040000, NEGX,  DEST_SIZ)
    E(0041000, CLR,   DEST_SIZ)
    E(0042000, NEG,   DEST_SIZ)
    E(0043000, NOT,   DEST_SIZ)
    E(0044200, EXT,   SIGN_EXT)
    E(0044000, NBCD,  DATA_DST)
    E(0044100, SWAP,  DATA_REG)
    E(0044100, PEA,   DATA_DST)
    E(0045374, ILLEGAL,IMPLIED)
    E(0045300, TAS,   DATA_DST)
    E(0045000, TST,   DEST_SIZ)
    E(0047100, TRAP,  TRAP_VEC)
    E(0047120, LINK,  ADDR_REG)
    E(0047130, UNLK,  ADDR_REG)
    E(0047140, MOVE,  MOVE_USP)
    E(0047160, RESET, IMPLIED)
    E(0047161, NOP,   IMPLIED)
    E(0047162, STOP,  IMPLIED)
    E(0047163, RTE,   IMPLIED)
    E(0047165, RTS,   IMPLIED)
    E(0047166, TRAPV, IMPLIED)
    E(0047167, RTR,   IMPLIED)
    E(0047200, JSR,   DEST_OPR)
    E(0047300, JMP,   DEST_OPR)
    E(0044200, MOVEM, MOVE_MLT)
    E(0040700, LEA,   AREG_LNG)
    E(0040600, CHK,   DREG_DST)
    E(0050310, DBT,   DATA_REG)
    E(0050710, DBRA,  DATA_REG)
    E(0050710, DBF,   DATA_REG)
    E(0051310, DBHI,  DATA_REG)
    E(0051710, DBLS,  DATA_REG)
    E(0052310, DBCC,  DATA_REG)
    E(0052310, DBHS,  DATA_REG)
    E(0052710, DBCS,  DATA_REG)
    E(0052710, DBLO,  DATA_REG)
    E(0053310, DBNE,  DATA_REG)
    E(0053710, DBEQ,  DATA_REG)
    E(0054310, DBVC,  DATA_REG)
    E(0054710, DBVS,  DATA_REG)
    E(0055310, DBPL,  DATA_REG)
    E(0055710, DBMI,  DATA_REG)
    E(0056310, DBGE,  DATA_REG)
    E(0056710, DBLT,  DATA_REG)
    E(0057310, DBGT,  DATA_REG)
    E(0057710, DBLE,  DATA_REG)
    E(0050300, ST,    DEST_OPR)
    E(0050700, SF,    DEST_OPR)
    E(0051300, SHI,   DEST_OPR)
    E(0051700, SLS,   DEST_OPR)
    E(0052300, SCC,   DEST_OPR)
    E(0052300, SHS,   DEST_OPR)
    E(0052700, SCS,   DEST_OPR)
    E(0052700, SLO,   DEST_OPR)
    E(0053300, SNE,   DEST_OPR)
    E(0053700, SEQ,   DEST_OPR)
    E(0054300, SVC,   DEST_OPR)
    E(0054700, SVS,   DEST_OPR)
    E(0055300, SPL,   DEST_OPR)
    E(0055700, SMI,   DEST_OPR)
    E(0056300, SGE,   DEST_OPR)
    E(0056700, SLT,   DEST_OPR)
    E(0057300, SGT,   DEST_OPR)
    E(0057700, SLE,   DEST_OPR)
    E(0050000, ADDQ,  DATA_QIC)
    E(0050400, SUBQ,  DATA_QIC)
    E(0060000, BRA,   RELATIVE)
    E(0060000, BT,    RELATIVE)
    E(0060400, BSR,   RELATIVE)
    E(0061000, BHI,   RELATIVE)
    E(0061400, BLS,   RELATIVE)
    E(0062000, BCC,   RELATIVE)
    E(0062000, BHS,   RELATIVE)
    E(0062400, BCS,   RELATIVE)
    E(0062400, BLO,   RELATIVE)
    E(0063000, BNE,   RELATIVE)
    E(0063400, BEQ,   RELATIVE)
    E(0064000, BVC,   RELATIVE)
    E(0064400, BVS,   RELATIVE)
    E(0065000, BPL,   RELATIVE)
    E(0065400, BMI,   RELATIVE)
    E(0066000, BGE,   RELATIVE)
    E(0066400, BLT,   RELATIVE)
    E(0067000, BGT,   RELATIVE)
    E(0067400, BLE,   RELATIVE)
    E(0070000, MOVEQ, MOVE_QIC)
    E(0100400, SBCD,  DMEM_DST)
    E(0100300, DIVU,  DREG_DST)
    E(0100700, DIVS,  DREG_DST)
    E(0100000, OR,    DMEM_SIZ)
    E(0110300, SUBA,  AREG_SIZ)
    E(0110400, SUBX,  DMEM_OPR)
    E(0110000, SUB,   DMEM_SIZ)
    E(0130300, CMPA,  AREG_SIZ)
    E(0130410, CMPM,  CMPM_SIZ)
    E(0130000, CMP,   DREG_SIZ)
    E(0130400, EOR,   DREG_SIZ)
    E(0140300, MULU,  DREG_DST)
    E(0140700, MULS,  DREG_DST)
    E(0140400, ABCD,  DMEM_DST)
    E(0140500, EXG,   REGS_EXG) // EXG Dx,Dy
    E(0140510, EXG,   REGS_EXG) // EXG Ax,Ay
    E(0140610, EXG,   REGS_EXG) // EXG Dx,Ay
    E(0140000, AND,   DMEM_SIZ)
    E(0150300, ADDA,  AREG_SIZ)
    E(0150400, ADDX,  DMEM_OPR)
    E(0150000, ADD,   DMEM_SIZ)
    E(0160300, ASR,   DEST_OPR) // ASR.W <ea>
    E(0160700, ASL,   DEST_OPR) // ASL.W <ea>
    E(0161300, LSR,   DEST_OPR) // LSR.W <ea>
    E(0161700, LSL,   DEST_OPR) // LSL.W <ea>
    E(0162300, ROXR,  DEST_OPR) // ROXR.W <ea>
    E(0162700, ROXL,  DEST_OPR) // ROXL.W <ea>
    E(0163300, ROR,   DEST_OPR) // ROR.W <ea>
    E(0163700, ROL,   DEST_OPR) // ROL.W <ea>
    E(0160000, ASR,   DREG_ROT) // ASR.BWL #/Dx,Dn
    E(0160400, ASL,   DREG_ROT) // ASL.BWL #/Dx,Dn
    E(0160010, LSR,   DREG_ROT) // LSR.BWL #/Dx,Dn
    E(0160410, LSL,   DREG_ROT) // LSL.BWL #/Dx,Dn
    E(0160020, ROXR,  DREG_ROT) // ROXR.BWL #/Dx,Dn
    E(0160420, ROXL,  DREG_ROT) // ROXL.BWL #/Dx,Dn
    E(0160030, ROR,   DREG_ROT) // ROR.BWL #/Dx,Dn
    E(0160430, ROL,   DREG_ROT) // ROL.BWL #/Dx,Dn
};

static Config::opcode_t getInsnMask(InsnFormat iformat) {
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

Error TableMc68000::searchName(InsnMc68000 &insn, const char *name) const {
    const Entry *entry =
        TableBase::searchName<Entry>(name, ARRAY_RANGE(TABLE_MC68000));
    if (!entry) return UNKNOWN_INSTRUCTION;
    insn.setOpCode(pgm_read_word(&entry->opCode));
    insn.setFlags(pgm_read_byte(&entry->flags));
    return OK;
}

static Config::opcode_t maskCode(Config::opcode_t opCode, const Entry *entry) {
    const host::uint_t flags = pgm_read_byte(&entry->flags);
    const Config::opcode_t mask = getInsnMask(Entry::_insnFormat(flags));
    return opCode & ~mask;
}

Error TableMc68000::searchOpCode(InsnMc68000 &insn) const {
    const Config::opcode_t opCode = insn.opCode();
    const Entry *entry = TableBase::searchCode<Entry, Config::opcode_t>(
        opCode, ARRAY_RANGE(TABLE_MC68000), maskCode);
    if (!entry) return UNKNOWN_INSTRUCTION;
    insn.setFlags(pgm_read_byte(&entry->flags));
    const char *name =
        reinterpret_cast<const char *>(pgm_read_ptr(&entry->name));
    TableBase::setName(insn.insn(), name, Config::NAME_MAX);
    return OK;
}

bool TableMc68000::setCpu(const char *cpu) {
    const char *p = cpu + (strncasecmp(cpu, "MC", 2) ? 0 : 2);
    return strcmp(p, "68000") == 0
        || strcmp(p, "68008") == 0
        || strcasecmp(p, "68k") == 0;
}

class TableMc68000 TableMc68000;

} // namespace mc68000
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
