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

#include "asm_mc68000.h"
#include "table_mc68000.h"

#include <ctype.h>

namespace libasm {
namespace mc68000 {

static const char *parseSize(const char *line, EaSize &size) {
    const char *p = line;
    if (*p++ != '.') {
        size = SZ_NONE;
        return line;
    }
    const char c = toupper(*p++);
    if (c == 'B') {
        size = SZ_BYTE;
    } else if (c == 'W') {
        size = SZ_WORD;
    } else if (c == 'L') {
        size = SZ_LONG;
    } else {
        size = SZ_INVALID;
        return line;
    }
    if (isalnum(*p) || *p == '_') {
        size = SZ_INVALID;
        return line;
    }
    return p;
}

Error AsmMc68000::checkSize(InsnMc68000 &insn, const EaSize size) {
    if (size == insn.size()) return setOK();
    if (size == SZ_NONE && insn.size() != SZ_NONE)
        return setError(ILLEGAL_SIZE);
    if (insn.size() == SZ_NONE) {
        insn.setSize(size);
        return setOK();
    }
    return setError(ILLEGAL_SIZE);
}

Error AsmMc68000::checkSize(const uint32_t val32, const EaSize size, bool uint) {
    const int32_t signed32 = static_cast<int32_t>(val32);
    if (size == SZ_BYTE) {
        if (!uint && signed32 >= -128 && signed32 < 128)
            return setOK();
        if (uint && (val32 < 0x100 || (signed32 < 0 && signed32 >= -128)))
            return setOK();
    }
    if (size == SZ_WORD) {
        if (!uint && signed32 >= -32768L && signed32 < 32768L)
            return setOK();
        if (uint && (val32 < 0x10000L || (signed32 < 0 && signed32 >= -32768)))
            return setOK();
    }
    return setError(OVERFLOW_RANGE);
}

Error AsmMc68000::emitImmediateData(
    InsnMc68000 &insn, EaSize size, uint32_t val32, Error error) {
    if (size == SZ_LONG) {
        insn.emitOperand32(val32);
        return setError(error);
    }
    if (size == SZ_WORD && checkSize(val32, size, true) == OK) {
        insn.emitOperand16(static_cast<uint16_t>(val32));
        return setError(error);
    } else if (size == SZ_BYTE && checkSize(val32, size, true) == OK) {
        insn.emitOperand16(static_cast<uint8_t>(val32));
        return setError(error);
    }
    return setError(OVERFLOW_RANGE);
}

Error AsmMc68000::emitEffectiveAddr(
    InsnMc68000 &insn, const Operand &ea,
    host::int_t size_gp, host::int_t mode_gp, host::uint_t reg_gp) {
    if (size_gp >= 0) {
        if (insn.size() == SZ_NONE) return setError(ILLEGAL_SIZE);
        insn.embed(static_cast<Config::opcode_t>(insn.size()), size_gp);
    }
    const EaMode mode = (ea.mode == M_LABEL)
        ? (checkSize(ea.val32, SZ_WORD, false) ? M_ABS_LONG : M_ABS_SHORT)
        : ea.mode;
    if (mode_gp >= 0) {
        insn.embed(EaMc68000::encodeMode(mode), mode_gp);
        if (mode == M_DREG && !RegMc68000::isDreg(ea.reg))
            return setError(ILLEGAL_OPERAND_MODE);
        insn.embed(EaMc68000::encodeRegNo(mode, ea.reg), reg_gp);
    } else {
        if (!RegMc68000::isADreg(ea.reg))
            return setError(ILLEGAL_OPERAND_MODE);
        insn.embed(RegMc68000::encodeRegNo(ea.reg), reg_gp);
    }

    insn.emitInsn();
    if (mode == M_INDX || mode == M_PC_INDX) {
        Config::ptrdiff_t disp;
        if (mode == M_PC_INDX) {
            Config::uintptr_t addr = ea.val32;
            if (ea.getError() == UNDEFINED_SYMBOL) addr = insn.address();
            disp = addr - (insn.address() + 2);
        } else {
            disp = static_cast<Config::ptrdiff_t>(ea.val32);
        }
        if (checkSize(disp, SZ_BYTE, false)) return getError();
        uint16_t extWord = static_cast<uint8_t>(disp);
        extWord |= (RegMc68000::encodeRegNo(ea.index) << 12);
        if (RegMc68000::isAreg(ea.index)) extWord |= (1 << 15);
        if (ea.size == SZ_LONG) extWord |= (1 << 11);
        insn.emitOperand16(extWord);
        return setError(ea);
    }
    if (mode == M_DISP || mode == M_PC_DISP || mode == M_ABS_SHORT) {
        Config::ptrdiff_t disp;
        if (mode == M_PC_DISP) {
            Config::uintptr_t addr = ea.val32;
            if (ea.getError() == UNDEFINED_SYMBOL) addr = insn.address();
            disp = addr - (insn.address() + 2);
        } else {
            disp = static_cast<Config::ptrdiff_t>(ea.val32);
        }
        if (checkSize(disp, SZ_WORD, false)) return getError();
        insn.emitOperand16(static_cast<uint16_t>(disp));
        return setError(ea);
    }
    if (mode == M_ABS_LONG) {
        insn.emitOperand32(ea.val32);
        return setError(ea);
    }
    if (mode == M_IMM_DATA)
        return emitImmediateData(insn, insn.size(), ea.val32, ea.getError());
    return setOK();
}

Error AsmMc68000::encodeImplied(
    InsnMc68000 &insn, const Operand &op1, const Operand &op2) {
    if (op2.mode != M_NONE) return setError(UNKNOWN_OPERAND);
    constexpr Config::opcode_t STOP = 047162;
    if (insn.opCode() == STOP) {
        if (op1.mode != M_IMM_DATA) return setError(ILLEGAL_OPERAND_MODE);
        insn.emitInsn();
        return emitImmediateData(insn, SZ_WORD, op1.val32, op1.getError());
    }
    insn.emitInsn();
    return setOK();
}

// ORI, ANDI, SUBI, ADDI, EORI, CMPI
// NEGX, CLR, NEG, NOT, TST
Error AsmMc68000::encodeDestSiz(
    InsnMc68000 &insn, const Operand &op1, const Operand &op2) {
    const uint8_t opc = (insn.opCode() >> 9) & 7;
    constexpr uint8_t SUBI = 2;
    constexpr uint8_t ADDI = 3;
    constexpr uint8_t CMPI = 6;
    const char *alias = nullptr;
    if (opc == SUBI) alias = "SUBA";
    if (opc == ADDI) alias = "ADDA";
    if (opc == CMPI) alias = "CMPA";
    if ((insn.opCode() >> 12) == 0) { // ORI/ANDI/SUBI/ADDI/EORI/CMPI
        if (op1.mode != M_IMM_DATA)
            return setError(UNKNOWN_OPERAND);
        if (_optimize && op2.mode == M_AREG && alias) {
            TableMc68000.searchName(insn, alias);
            return encodeAregSiz(insn, op1, op2);
        }
        if (op2.reg == REG_CCR || op2.reg == REG_SR) {
            if (alias) return setError(UNKNOWN_OPERAND);
            insn.setSize(SZ_BYTE);
            insn.embed(0074);
            if (op2.reg == REG_SR) {
                insn.setSize(SZ_WORD);
                insn.embed(0100);
            }
            insn.emitInsn();
            return emitImmediateData(
                insn, insn.size(), op1.val32, op1.getError());
        }
        if (!op2.satisfy(CAT_DATA | CAT_ALTERABLE))
            return setError(ILLEGAL_OPERAND_MODE);
        const Error error = emitImmediateData(
            insn, insn.size(), op1.val32, op1.getError());
        emitEffectiveAddr(insn, op2);
        return setError(error ? error : getError());
    }
    // NEGX/CLR/NEG/NOT/TST
    if (op2.mode != M_NONE) setError(UNKNOWN_OPERAND);
    constexpr uint8_t TST = 5;
    if (opc == TST && !op1.satisfy(CAT_DATA))
        return setError(ILLEGAL_OPERAND_MODE);
    if (!op1.satisfy(CAT_DATA | CAT_ALTERABLE))
        return setError(ILLEGAL_OPERAND_MODE);
    return emitEffectiveAddr(insn, op1);
}

// LINK, UNLK
Error AsmMc68000::encodeAddrReg(
    InsnMc68000 &insn, const Operand &op1, const Operand &op2) {
    if (insn.opCode() & 010) { // UNLK
        if (checkSize(insn, SZ_NONE)) return getError();
        if (op1.mode != M_AREG) return setError(ILLEGAL_OPERAND_MODE);
        if (op2.mode != M_NONE) return setError(UNKNOWN_OPERAND);
        insn.embed(RegMc68000::encodeRegNo(op1.reg));
        insn.emitInsn();
        return setOK();
    }
    // LINK
    if (checkSize(insn, SZ_WORD)) return getError();
    if (op1.mode != M_AREG) return setError(ILLEGAL_OPERAND_MODE);
    if (op2.mode != M_IMM_DATA) return setError(ILLEGAL_OPERAND_MODE);
    if (checkSize(op2.val32, SZ_WORD, false)) return getError();
    insn.embed(RegMc68000::encodeRegNo(op1.reg));
    insn.emitInsn();
    return emitImmediateData(insn, SZ_WORD, op2.val32, op2.getError());
}

// SWAP, DBcc
Error AsmMc68000::encodeDataReg(
    InsnMc68000 &insn, const Operand &op1, const Operand &op2) {
    if (checkSize(insn, SZ_WORD)) return getError();
    if (!RegMc68000::isDreg(op1.reg)) return setError(ILLEGAL_OPERAND_MODE);
    const uint8_t opc = (insn.opCode() >> 12) & 017;
    constexpr uint8_t SWAP = 004;
    if (opc == SWAP) {
        if (op2.mode != M_NONE) return setError(UNKNOWN_OPERAND);
        insn.embed(RegMc68000::encodeRegNo(op1.reg));
        insn.emitInsn();
        return setOK();
    }
    // DBcc
    if (op2.mode == M_ABS_LONG || op2.mode == M_ABS_SHORT
        || op2.mode == M_LABEL) {
        Config::uintptr_t addr = op2.val32;
        if (op2.getError() == UNDEFINED_SYMBOL) addr = insn.address();
        const Config::ptrdiff_t disp = addr - (insn.address() + 2);
        if (checkSize(disp, SZ_WORD, false))
            return setError(OPERAND_TOO_FAR);
        insn.embed(RegMc68000::encodeRegNo(op1.reg));
        insn.emitInsn();
        return emitImmediateData(insn, SZ_WORD, disp, op2.getError());
    }
    return setError(ILLEGAL_OPERAND_MODE);
}

// TRAP
Error AsmMc68000::encodeTrapVec(
    InsnMc68000 &insn, const Operand &op1, const Operand &op2) {
    if (op2.mode != M_NONE) return setError(UNKNOWN_OPERAND);
    if (op1.mode != M_IMM_DATA) return setError(UNKNOWN_OPERAND);
    if (op1.val32 >= 16) return setError(OVERFLOW_RANGE);
    insn.embed(static_cast<uint8_t>(op1.val32));
    insn.emitInsn();
    return setOK();
}

// NBCD, PEA, TAS
Error AsmMc68000::encodeDataDst(
    InsnMc68000 &insn, const Operand &op1, const Operand &op2) {
    const Config::opcode_t opCode = insn.opCode();
    if (op2.mode != M_NONE) return setError(UNKNOWN_OPERAND);
    const uint8_t opc = (opCode >> 6) & 077;
    constexpr uint8_t PEA = 041;
    if (opc == PEA) {
        if (checkSize(insn, SZ_LONG)) return getError();
        if (!op1.satisfy(CAT_CONTROL))
            return setError(ILLEGAL_OPERAND_MODE);
        return emitEffectiveAddr(insn, op1, -1);
    }
    // NBCD, TAS
    if (checkSize(insn, SZ_BYTE)) return getError();
    if (!op1.satisfy(CAT_DATA | CAT_ALTERABLE))
        return setError(ILLEGAL_OPERAND_MODE);
    return emitEffectiveAddr(insn, op1, -1);
}

// BTST, BCHG, BCLR, BSET
// JSR, JMP, Scc,
// ASR, ASL, LSR, LSL, ROXR, ROXL
Error AsmMc68000::encodeDestOpr(
    InsnMc68000 &insn, const Operand &op1, const Operand &op2) {
    if ((insn.opCode() >> 12) == 0) { // BTST/BCHG/BCLR/BSET
        const uint8_t opc = (insn.opCode() >> 6) & 3;
        constexpr uint8_t BTST = 0;
        if (opc == BTST && !op2.satisfy(CAT_DATA))
            return setError(ILLEGAL_OPERAND_MODE);
        if (opc != BTST && !op2.satisfy(CAT_DATA | CAT_ALTERABLE))
            return setError(ILLEGAL_OPERAND_MODE);

        if (RegMc68000::isDreg(op1.reg))
            return encodeDregDst(insn, op1, op2);

        if (RegMc68000::isDreg(op2.reg) && checkSize(insn, SZ_LONG))
            return getError();
        if (!RegMc68000::isDreg(op2.reg) && checkSize(insn, SZ_BYTE))
            return getError();
        if (op1.mode != M_IMM_DATA)
            return setError(UNKNOWN_OPERAND);
        if (op1.val32 >= (RegMc68000::isDreg(op2.reg) ? 32 : 8))
            return setError(OVERFLOW_RANGE);
        const Error error1 = emitEffectiveAddr(insn, op2, -1);
        const Error error2 =
            emitImmediateData(insn, SZ_BYTE, op1.val32, op1.getError());
        return setError(error1 ? error1 : (error2 ? error2 : OK));
    }
    // JSR/JMP/Scc/ASd/LSd/ROXd/ROd
    const uint8_t opc = (insn.opCode() >> 12) & 077;
    constexpr uint8_t JSR_JMP = 004;
    constexpr uint8_t Scc = 005;
    if (opc == JSR_JMP) {
        if (op2.mode != M_NONE) return setError(UNKNOWN_OPERAND);
        if (checkSize(insn, SZ_NONE)) return getError();
        if (!op1.satisfy(CAT_CONTROL))
            return setError(ILLEGAL_OPERAND_MODE);
        return emitEffectiveAddr(insn, op1, -1);
    }
    if (opc == Scc) {
        if (op2.mode != M_NONE) return setError(UNKNOWN_OPERAND);
        if (checkSize(insn, SZ_BYTE)) return getError();
        if (!op1.satisfy(CAT_DATA | CAT_ALTERABLE))
            return setError(ILLEGAL_OPERAND_MODE);
        return emitEffectiveAddr(insn, op1, -1);
    }
    // ASd/LSd/ROXd/ROd
    if (op2.mode == M_NONE) {   // DEST_OPR <ea>
        if (checkSize(insn, SZ_WORD)) return getError();
        if (!op1.satisfy(CAT_MEMORY | CAT_ALTERABLE))
            return setError(ILLEGAL_OPERAND_MODE);
        return emitEffectiveAddr(insn, op1, -1);
    }
    if (!RegMc68000::isDreg(op2.reg))
        return setError(UNKNOWN_OPERAND);
    // DREG_ROT
    if (insn.size() == SZ_NONE) insn.setSize(SZ_WORD);
    const uint8_t rotOpc = (insn.opCode() >> 9) & 3;
    insn.setOpCode(0160000 | (insn.opCode() & 0400));
    insn.embed(rotOpc, 3);
    if (RegMc68000::isDreg(op1.reg)) { // Dx,Dy
        insn.embed(040);
        insn.embed(RegMc68000::encodeRegNo(op1.reg), 9);
        return emitEffectiveAddr(insn, op2, 6, -1);
    }
    if (op1.mode == M_IMM_DATA) { // #<data>,Dy
        if (op1.val32 > 8 || (op1.getError() == OK && op1.val32 == 0))
            return setError(OVERFLOW_RANGE);
        uint8_t rot = static_cast<uint8_t>(op1.val32);
        if (rot == 8) rot = 0;
        insn.embed(static_cast<Config::opcode_t>(rot), 9);
        emitEffectiveAddr(insn, op2, 6, -1);
        return setError(op1.getError() ? op1.getError() : getError());
    }
    return setError(ILLEGAL_OPERAND_MODE);
}

// EXT
Error AsmMc68000::encodeSignExt(
    InsnMc68000 &insn, const Operand &op1, const Operand &op2) {
    if (insn.size() == SZ_BYTE) return setError(ILLEGAL_SIZE);
    if (insn.size() == SZ_NONE) insn.setSize(SZ_WORD);
    if (op2.mode != M_NONE) return setError(UNKNOWN_OPERAND);
    if (!RegMc68000::isDreg(op1.reg)) return setError(ILLEGAL_OPERAND_MODE);
    insn.embed(insn.size() == SZ_WORD ? 044200 : 044300);
    insn.embed(RegMc68000::encodeRegNo(op1.reg));
    insn.emitInsn();
    return setOK();
}

// BRA, BSR, Bcc
Error AsmMc68000::encodeRelative(
    InsnMc68000 &insn, const Operand &op1, const Operand &op2) {
    if (insn.size() == SZ_LONG) return setError(ILLEGAL_SIZE);
    if (op2.mode != M_NONE) return setError(UNKNOWN_OPERAND);
    if (op1.mode != M_LABEL) return setError(ILLEGAL_OPERAND_MODE);
    Config::uintptr_t addr = op1.val32;
    if (op1.getError() == UNDEFINED_SYMBOL) addr = insn.address();
    const Config::ptrdiff_t disp = addr - (insn.address() + 2);
    if (insn.size() == SZ_NONE)
        insn.setSize(checkSize(disp, SZ_BYTE, false) == OK ? SZ_BYTE : SZ_WORD);
    if (insn.size() == SZ_BYTE && checkSize(disp, SZ_BYTE, false))
        return setError(OPERAND_TOO_FAR);
    if (insn.size() == SZ_WORD && checkSize(disp, SZ_WORD, false))
        return setError(OPERAND_TOO_FAR);
    if (insn.size() == SZ_BYTE) {
        insn.embed(static_cast<uint8_t>(disp));
        insn.emitInsn();
        return setError(op1);
    }
    insn.emitInsn();
    return emitImmediateData(insn, SZ_WORD, disp, op1.getError());
}

static uint16_t reverseBits(uint16_t bits) {
    uint16_t reverse = 0;
    for (host::uint_t i = 0; ; i++) {
        if (bits & 1) reverse |= 1;
        if (i == 15) break;
        bits >>= 1;
        reverse <<= 1;
    }
    return reverse;
}

// MOVEM
Error AsmMc68000::encodeMoveMlt(
    InsnMc68000 &insn, const Operand &op1, const Operand &op2) {
    if (insn.size() == SZ_BYTE) return setError(ILLEGAL_SIZE);
    if (insn.size() == SZ_NONE) insn.setSize(SZ_WORD);
    if (insn.size() == SZ_LONG) insn.embed(0100);
    if (op2.mode == M_PDEC || op2.satisfy(CAT_CONTROL | CAT_ALTERABLE)) {
        uint16_t list;
        if (op1.mode == M_MULT_REGS) {
            list = static_cast<uint16_t>(op1.val32);
        } else if (op1.mode == M_AREG || RegMc68000::isDreg(op1.reg)) {
            // TODO: MOVEM An|Dn,<ea> -> MOVE An|Dn,<ea>
            list = 1 << RegMc68000::encodeRegPos(op1.reg);
        } else {
            return setError(UNKNOWN_OPERAND);
        }
        if (op2.mode == M_PDEC) {
            insn.emitOperand16(reverseBits(list));
        } else {
            insn.emitOperand16(list);
        }
        return emitEffectiveAddr(insn, op2, -1);
    }
    if (op1.mode == M_PINC || op1.satisfy(CAT_CONTROL)) {
        insn.embed(02000);
        uint16_t list;
        if (op2.mode == M_MULT_REGS) {
            list = static_cast<uint16_t>(op2.val32);
        } else if (op2.mode == M_AREG || RegMc68000::isDreg(op2.reg)) {
            // TODO: MOVE <ea>,An -> MOVEA <ea>,An or MOVE <ea>,Dn
            list = 1 << RegMc68000::encodeRegPos(op2.reg);
        } else {
            return setError(UNKNOWN_OPERAND);
        }
        insn.emitOperand16(list);
        return emitEffectiveAddr(insn, op1, -1);
    }
    return setError(ILLEGAL_OPERAND_MODE);
}

// AREG_LNG: LEA
// AREG_SIZ: SUBA, CMPA, ADDA
Error AsmMc68000::encodeAregSiz(
    InsnMc68000 &insn, const Operand &op1, const Operand &op2) {
    Config::opcode_t opCode = insn.opCode();
    const uint8_t opc = (opCode >> 12) & 017;
    constexpr uint8_t LEA = 004;
    if (opc == LEA) {
        if (checkSize(insn, SZ_LONG)) return getError();
        if (!op1.satisfy(CAT_CONTROL)) return setError(ILLEGAL_OPERAND_MODE);
    } else { // SUBA, CMPA, ADDA
        if (insn.size() == SZ_NONE) insn.setSize(SZ_WORD);
        if (insn.size() == SZ_BYTE) return setError(ILLEGAL_SIZE);
        if (insn.size() == SZ_LONG) insn.embed(0400);
    }
    if (op2.mode != M_AREG) return setError(ILLEGAL_OPERAND_MODE);
    insn.embed(RegMc68000::encodeRegNo(op2.reg), 9);
    return emitEffectiveAddr(insn, op1, -1);
}


// BTST, BCHG, BCLR, BSET (redirect from DEST_OPR)
// CHK, DIVU, DIVS, MULU, MULS
Error AsmMc68000::encodeDregDst(
    InsnMc68000 &insn, const Operand &op1, const Operand& op2) {
    const Config::opcode_t opCode = insn.opCode();
    const uint8_t opc = opCode >> 12;
    constexpr uint8_t Bxxx = 0;
    if (opc == Bxxx) {
        insn.setOpCode(opCode & ~07077);
        insn.embed(0400);
        // Error check has dobe at DEST_OPR.
        if (RegMc68000::isDreg(op2.reg) && checkSize(insn, SZ_LONG))
            return getError();
        if (!RegMc68000::isDreg(op2.reg) && checkSize(insn, SZ_BYTE))
            return getError();
        insn.embed(RegMc68000::encodeRegNo(op1.reg), 9);
        return emitEffectiveAddr(insn, op2, -1);
    }

    // CHK, DIVU, DIVS, MULU, MULS
    if (!RegMc68000::isDreg(op2.reg)) return setError(UNKNOWN_OPERAND);
    if (checkSize(insn, SZ_WORD)) return getError();
    if (!op1.satisfy(CAT_DATA)) return setError(ILLEGAL_OPERAND_MODE);
    insn.embed(RegMc68000::encodeRegNo(op2.reg), 9);
    return emitEffectiveAddr(insn, op1, -1);
}

// MOVEQ
Error AsmMc68000::encodeMoveQic(
    InsnMc68000 &insn, const Operand &op1, const Operand &op2) {
    if (checkSize(insn, SZ_LONG)) return getError();
    if (op1.mode != M_IMM_DATA) return setError(ILLEGAL_OPERAND_MODE);
    if (checkSize(op1.val32, SZ_BYTE, false)) return getError();
    if (!RegMc68000::isDreg(op2.reg)) return setError(ILLEGAL_OPERAND_MODE);
    const uint8_t data = static_cast<uint8_t>(op1.val32);
    insn.embed(RegMc68000::encodeRegNo(op2.reg), 9);
    insn.embed(data);
    insn.emitInsn();
    return setError(op1);
}

// MOVEP
Error AsmMc68000::encodeMovePer(
    InsnMc68000 &insn, const Operand &op1, const Operand &op2) {
    if (insn.size() == SZ_BYTE) return setError(ILLEGAL_SIZE);
    if (insn.size() == SZ_NONE) insn.setSize(SZ_WORD);
    if (insn.size() == SZ_LONG) insn.embed(0100);
    if (RegMc68000::isDreg(op1.reg) && op2.mode == M_DISP) {
        insn.embed(0200);
        insn.embed(RegMc68000::encodeRegNo(op1.reg), 9);
        insn.embed(RegMc68000::encodeRegNo(op2.reg));
        insn.emitInsn();
        return emitImmediateData(insn, SZ_WORD, op2.val32, op2.getError());
    }
    if (op1.mode == M_DISP && RegMc68000::isDreg(op2.reg)) {
        insn.embed(RegMc68000::encodeRegNo(op2.reg), 9);
        insn.embed(RegMc68000::encodeRegNo(op1.reg));
        insn.emitInsn();
        return emitImmediateData(insn, SZ_WORD, op1.val32, op1.getError());
    }
    return setError(ILLEGAL_OPERAND_MODE);
}

// ADDQ, SUBQ
Error AsmMc68000::encodeDataQic(
    InsnMc68000 &insn, const Operand &op1, const Operand &op2) {
    if (insn.size() == SZ_NONE) insn.setSize(SZ_WORD);
    if (op2.mode == M_AREG && insn.size() == SZ_BYTE)
        return setError(ILLEGAL_SIZE);
    if (op1.mode != M_IMM_DATA)
        return setError(ILLEGAL_OPERAND_MODE);
    uint8_t data = static_cast<uint8_t>(op1.val32);
    if (op1.val32 > 8 || (op1.getError() == OK && op1.val32 == 0))
        return setError(OVERFLOW_RANGE);
    if (data == 8) data = 0;
    if (!op2.satisfy(CAT_ALTERABLE)) return setError(ILLEGAL_OPERAND_MODE);
    insn.embed(data, 9);
    emitEffectiveAddr(insn, op2);
    return setError(op1.getError() ? op1.getError() : getError());
}

// DMEM_SIZ: OR, SUB, AND, ADD
// DREG_SIZ: CMP, EOR
Error AsmMc68000::encodeDmemSiz(
    InsnMc68000 &insn, const Operand &op1, const Operand &op2) {
    if (insn.size() == SZ_NONE) insn.setSize(SZ_WORD);
    const uint8_t opc = (insn.opCode() >> 8) & ~0xE;
    constexpr uint8_t CMP = 0xB0;      // <ea>,Dn only
    constexpr uint8_t EOR = 0xB1;      // Dn,<ea> only
    const uint8_t op4 = (opc >> 4);
    constexpr uint8_t OR = 0x8;
    constexpr uint8_t SUB = 0x9;
    constexpr uint8_t AND = 0xC;
    constexpr uint8_t ADD = 0xD;
    if (opc != EOR && RegMc68000::isDreg(op2.reg)) { // <ea>,Dn
        if (op1.mode == M_AREG && insn.size() == SZ_BYTE
            && (opc == CMP || op4 == SUB || op4 == ADD))
            return setError(ILLEGAL_SIZE);
        if (op1.mode == M_AREG && (op4 == OR || op4 == AND))
            return setError(ILLEGAL_OPERAND_MODE);
        insn.embed(RegMc68000::encodeRegNo(op2.reg), 9);
        return emitEffectiveAddr(insn, op1);
    }
    const char *alias = nullptr;
    if (op4 == SUB) alias = "SUBA";
    if (op4 == ADD) alias = "ADDA";
    if (_optimize && op2.mode == M_AREG && alias) {
        TableMc68000.searchName(insn, alias);
        return encodeAregSiz(insn, op1, op2);
    }
    if (RegMc68000::isDreg(op1.reg)) { // Dn,<ea>
        const host::uint_t categories = (opc == EOR ? CAT_DATA : CAT_MEMORY)
            | CAT_ALTERABLE;
        if (!op2.satisfy(categories))
            return setError(ILLEGAL_OPERAND_MODE);
        insn.embed(0400);
        insn.embed(RegMc68000::encodeRegNo(op1.reg), 9);
        return emitEffectiveAddr(insn, op2);
    }
    return setError(ILLEGAL_OPERAND_MODE);
}

// DMEM_DST: SBCD, ABCD
// DMEM_OPR: SUBX, ADDX
// CMPM_SIZ: CMPM
Error AsmMc68000::encodeDmemOpr(
    InsnMc68000 &insn, const Operand &op1, const Operand &op2) {
    const uint8_t opc = (insn.opCode() >> 12);
    constexpr uint8_t SBCD = 010;
    constexpr uint8_t ABCD = 014;
    constexpr uint8_t CMPM = 013;
    if ((opc == SBCD || opc == ABCD) && checkSize(insn, SZ_BYTE)) return getError();
    if (insn.size() == SZ_NONE) insn.setSize(SZ_WORD);
    if (opc == CMPM) {
        if (op1.mode != M_PINC || op2.mode != M_PINC)
            return setError(ILLEGAL_OPERAND_MODE);
        insn.embed(RegMc68000::encodeRegNo(op2.reg), 9);
        return emitEffectiveAddr(insn, op1, 6, -1);
    }
    // SBCD, ABCD, SUBX, ADDX
    if (op1.mode == M_PDEC && op2.mode == M_PDEC) {
        insn.embed(010);
        insn.embed(RegMc68000::encodeRegNo(op2.reg), 9);
        return emitEffectiveAddr(insn, op1, 6, -1);
    }
    if (RegMc68000::isDreg(op1.reg) && RegMc68000::isDreg(op2.reg)) {
        insn.embed(RegMc68000::encodeRegNo(op2.reg), 9);
        return emitEffectiveAddr(insn, op1, 6, -1);
    }
    return setError(ILLEGAL_OPERAND_MODE);
}

// EXG
Error AsmMc68000::encodeRegsExg(
    InsnMc68000 &insn, const Operand &op1, const Operand &op2) {
    if (checkSize(insn, SZ_LONG)) return getError();
    if (!RegMc68000::isADreg(op1.reg) || !RegMc68000::isADreg(op2.reg))
        return setError(ILLEGAL_OPERAND_MODE);
    insn.setOpCode(0xC100);
    if (op1.mode == M_AREG && op2.mode == M_AREG) {
        insn.embed(0110);
        insn.embed(RegMc68000::encodeRegNo(op1.reg), 9);
        insn.embed(RegMc68000::encodeRegNo(op2.reg), 0);
    } else if (op1.mode == M_AREG) {
        insn.embed(0210);
        insn.embed(RegMc68000::encodeRegNo(op2.reg), 9);
        insn.embed(RegMc68000::encodeRegNo(op1.reg), 0);
    } else if (op2.mode == M_AREG) {
        insn.embed(0210);
        insn.embed(RegMc68000::encodeRegNo(op1.reg), 9);
        insn.embed(RegMc68000::encodeRegNo(op2.reg), 0);
    } else {
        insn.embed(0100);
        insn.embed(RegMc68000::encodeRegNo(op1.reg), 9);
        insn.embed(RegMc68000::encodeRegNo(op2.reg), 0);
    }
    insn.emitInsn();
    return setOK();
}

// MOVE, MOVEA
Error AsmMc68000::encodeMoveOpr(
    InsnMc68000 &insn, const Operand &op1, const Operand &op2) {
    if (op1.reg == REG_SR) {
        if (checkSize(insn, SZ_WORD)) return getError();
        if (!op2.satisfy(CAT_DATA | CAT_ALTERABLE))
            return setError(ILLEGAL_OPERAND_MODE);
        insn.setOpCode(040300);
        return emitEffectiveAddr(insn, op2);
    }
    if (op2.reg == REG_SR) {
        if (checkSize(insn, SZ_WORD)) return getError();
        if (!op1.satisfy(CAT_DATA)) return setError(ILLEGAL_OPERAND_MODE);
        insn.setOpCode(043300);
        return emitEffectiveAddr(insn, op1);
    }
    if (op2.reg == REG_CCR) {
        if (checkSize(insn, SZ_BYTE)) return getError();
        if (!op1.satisfy(CAT_DATA)) return setError(ILLEGAL_OPERAND_MODE);
        insn.setOpCode(042300);
        return emitEffectiveAddr(insn, op1);
    }
    if (op1.reg == REG_USP || op2.reg == REG_USP) {
        if (checkSize(insn, SZ_LONG)) return getError();
        if (op1.reg == REG_USP && op2.mode == M_AREG) {
            insn.setOpCode(047150);
            insn.embed(RegMc68000::encodeRegNo(op2.reg));
            insn.emitInsn();
            return setOK();
        }
        if (op1.mode == M_AREG && op2.reg == REG_USP) {
            insn.setOpCode(047140);
            insn.embed(RegMc68000::encodeRegNo(op1.reg));
            insn.emitInsn();
            return setOK();
        }
        return setError(ILLEGAL_OPERAND_MODE);
    }
    EaSize size = insn.size();
    // MOVE.L #<data>,Dn => MOVEQ #<data>,Dn
    if (_optimize && op1.mode == M_IMM_DATA && insn.size() == SZ_LONG
        && checkSize(op1.val32, SZ_BYTE, false) == OK
        && RegMc68000::isDreg(op2.reg)) {
        TableMc68000.searchName(insn, "MOVEQ");
        return encodeMoveQic(insn, op1, op2);
    }
    if (op2.mode == M_AREG) {
        if (size == SZ_BYTE) return setError(ILLEGAL_SIZE);
        if (size == SZ_NONE) size = SZ_LONG;
    } else if (op1.mode == M_AREG) {
        if (size == SZ_BYTE) return setError(ILLEGAL_SIZE);
    } else {
        if (size == SZ_NONE) size = SZ_WORD;
        if (!op2.satisfy(CAT_DATA | CAT_ALTERABLE))
            return setError(ILLEGAL_OPERAND_MODE);
    }
    insn.setSize(size);
    if (size == SZ_WORD) insn.setOpCode(030000);
    else if (size == SZ_LONG) insn.setOpCode(020000);
    else insn.setOpCode(010000);
    const Error error1 = emitEffectiveAddr(insn, op1, -1);
    const Error error2 = emitEffectiveAddr(insn, op2, -1, 6, 9);
    return setError(error1 ? error1 : (error2 ? error2 : OK));
}

Error AsmMc68000::parseMoveMultiRegList(Operand &opr) {
    const char *p = _scan;
    Error error = OK;
    for (;;) {
        RegName start = RegMc68000::parseRegName(p);
        if (!RegMc68000::isADreg(start))
            return opr.setError(UNKNOWN_OPERAND);
        p = skipSpaces(p + RegMc68000::regNameLen(start));
        host::uint_t s = RegMc68000::encodeRegPos(start);
        host::uint_t e = s;
        if (*p == '-') {
            p = skipSpaces(p + 1);
            RegName last = RegMc68000::parseRegName(p);
            if (!RegMc68000::isADreg(start))
                return opr.setError(UNKNOWN_OPERAND);
            e = RegMc68000::encodeRegPos(last);
            p = skipSpaces(p + RegMc68000::regNameLen(last));
        }
        if (s > e) return opr.setError(UNKNOWN_OPERAND);
        for (host::uint_t i = s; i <= e; i++) {
            const uint32_t bm = (1 << i);
            if (opr.val32 & bm) error = DUPLICATE_REGISTER;
            opr.val32 |= bm;
        }
        if (*p != '/') break;
        p++;
    }
    opr.mode = M_MULT_REGS;
    _scan = p;
    return opr.setError(error);
}

Error AsmMc68000::parseOperand(Operand &opr) {
    opr.reset();
    const char *p = _scan;
    if (endOfLine(p))
        return opr.setOK();
    if (*p == '#') {
        _scan = p + 1;
        if (getOperand(opr.val32)) return getError();
        opr.setError(getError());
        opr.mode = M_IMM_DATA;
        return setOK();
    }
    const char pdec = *p;
    if (pdec == '-' && p[1] == '(')
        p++;
    if (*p == '(') {
        p = skipSpaces(p + 1);
        opr.reg = RegMc68000::parseRegName(p);
        if (RegMc68000::isAreg(opr.reg)) {
            p = skipSpaces(p + RegMc68000::regNameLen(opr.reg));
            if (*p++ != ')')
                return opr.setError(UNKNOWN_OPERAND);
            if (*p == '+') {
                p++;
                opr.mode = M_PINC;
            } else {
                opr.mode = (pdec == '-') ? M_PDEC : M_AIND;
            }
            _scan = p;
            return setError(opr.setOK());
        }
        _scan = p;
        if (getOperand(opr.val32)) return opr.setError(getError());
        opr.setErrorIf(getError());
        p = skipSpaces(_scan);
        if (*p == ')') {
            p = parseSize(p + 1, opr.size);
            if (opr.size == SZ_NONE) {
                opr.mode = checkSize(opr.val32, SZ_WORD, false) ? M_ABS_LONG : M_ABS_SHORT;
            } else if (opr.size == SZ_WORD) {
                opr.mode = M_ABS_SHORT;
            } else if (opr.size == SZ_LONG) {
                opr.mode = M_ABS_LONG;
            } else {
                return opr.setError(UNKNOWN_OPERAND);
            }
            _scan = p;
            return setOK();
        }
        if (*p == ',') {
            p = skipSpaces(p + 1);
            opr.reg = RegMc68000::parseRegName(p);
            if (!RegMc68000::isAreg(opr.reg) && opr.reg != REG_PC)
                return opr.setError(UNKNOWN_OPERAND);
            p = skipSpaces(p + RegMc68000::regNameLen(opr.reg));
            if (*p == ')') {
                opr.mode = (opr.reg == REG_PC) ? M_PC_DISP : M_DISP;
                _scan = p + 1;
                if (opr.mode == M_DISP && checkSize(opr.val32, SZ_WORD, false))
                    return opr.setError(*this);
                return setOK();
            }
            if (*p != ',')
                return opr.setError(UNKNOWN_OPERAND);
            p = skipSpaces(p + 1);
            opr.index = RegMc68000::parseRegName(p);
            if (!RegMc68000::isADreg(opr.index))
                return opr.setError(UNKNOWN_OPERAND);
            p += RegMc68000::regNameLen(opr.index);
            p = skipSpaces(parseSize(p, opr.size));
            if (opr.size == SZ_INVALID || *p++ != ')')
                return opr.setError(UNKNOWN_OPERAND);
            opr.mode = (opr.reg == REG_PC) ? M_PC_INDX : M_INDX;
            if (opr.mode == M_INDX && checkSize(opr.val32, SZ_BYTE, false))
                return opr.setError(*this);
            _scan = p;
            return setOK();
        }
        return opr.setError(UNKNOWN_OPERAND);
    }

    opr.reg = RegMc68000::parseRegName(p);
    if (opr.reg != REG_UNDEF) {
        p = skipSpaces(p + RegMc68000::regNameLen(opr.reg));
        if ((*p == '/' || *p == '-') && RegMc68000::isADreg(opr.reg))
            return parseMoveMultiRegList(opr);
        if (RegMc68000::isAreg(opr.reg)) {
            opr.mode = M_AREG;
        } else {
            opr.mode = M_DREG;  // include other register SR/CCR/USP
        }
        _scan = p;
        return opr.setOK();
    }
    _scan = p;
    if (getOperand(opr.val32)) return opr.setError(getError());
    opr.setErrorIf(getError());
    opr.mode = M_LABEL;
    return setOK();
}

Error AsmMc68000::encode(Insn &_insn) {
    InsnMc68000 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);

    EaSize size;
    const char *endSize = parseSize(endName, size);
    if (size == SZ_INVALID) return setError(ILLEGAL_SIZE);
    insn.setSize(size);

    if (TableMc68000.searchName(insn, insn.name()))
        return setError(UNKNOWN_INSTRUCTION);
    _scan = skipSpaces(endSize);
    Operand op1;
    Operand op2;
    if (parseOperand(op1))
        return setError(op1);
    _scan = skipSpaces(_scan);
    if (*_scan == ',') {
        _scan = skipSpaces(_scan + 1);
        if (parseOperand(op2))
            return setError(op2);
    } else {
        op2.reset();
    }
    if (checkLineEnd()) return setError(GARBAGE_AT_END);

    switch (insn.insnFormat()) {
    case IMPLIED:  return encodeImplied(insn, op1, op2);
    case DEST_SIZ: return encodeDestSiz(insn, op1, op2);
    case ADDR_REG: return encodeAddrReg(insn, op1, op2);
    case DATA_REG: return encodeDataReg(insn, op1, op2);
    case TRAP_VEC: return encodeTrapVec(insn, op1, op2);
    case DATA_DST: return encodeDataDst(insn, op1, op2);
    case DEST_OPR: return encodeDestOpr(insn, op1, op2);
    case SIGN_EXT: return encodeSignExt(insn, op1, op2);
    case RELATIVE: return encodeRelative(insn, op1, op2);
    case MOVE_MLT: return encodeMoveMlt(insn, op1, op2);
    case AREG_LNG:
    case AREG_SIZ: return encodeAregSiz(insn, op1, op2);
    case DREG_DST: return encodeDregDst(insn, op1, op2);
    case MOVE_QIC: return encodeMoveQic(insn, op1, op2);
    case MOVE_PER: return encodeMovePer(insn, op1, op2);
    case DATA_QIC: return encodeDataQic(insn, op1, op2);
    case DREG_SIZ:
    case DMEM_SIZ: return encodeDmemSiz(insn, op1, op2);
    case CMPM_SIZ:
    case DMEM_DST:
    case DMEM_OPR: return encodeDmemOpr(insn, op1, op2);
    case REGS_EXG: return encodeRegsExg(insn, op1, op2);
    case MOVA_OPR:
    case MOVE_OPR: return encodeMoveOpr(insn, op1, op2);
    default: break;
    }
    return setError(UNKNOWN_INSTRUCTION);
}

} // namespace mc68000
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
