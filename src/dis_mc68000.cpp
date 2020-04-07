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

#include "dis_mc68000.h"
#include "table_mc68000.h"

namespace libasm {
namespace mc68000 {

void DisMc68000::outRegName(RegName regName) {
    _operands = _regs.outRegName(_operands, regName);
}

void DisMc68000::outEaSize(EaSize size) {
    _operands = _regs.outEaSize(_operands, size);
}

Error DisMc68000::decodeImmediateData(
    DisMemory<Config::uintptr_t> &memory, InsnMc68000 &insn, EaSize size) {
    Error error = OK;
    if (size == SZ_BYTE) {
        uint16_t val16;
        if (insn.readUint16(memory, val16)) error = NO_MEMORY;
        else outConstant(static_cast<uint8_t>(val16));
    } else if (size == SZ_WORD) {
        uint16_t val16;
        if (insn.readUint16(memory, val16)) error = NO_MEMORY;
        else outConstant(val16);
    } else if (size == SZ_LONG) {
        uint32_t val32;
        if (insn.readUint32(memory, val32)) error = NO_MEMORY;
        else outConstant(val32);
    } else {
        error = ILLEGAL_SIZE;
    }
    return setError(error);
}

Error DisMc68000::decodeEffectiveAddr(
    DisMemory<Config::uintptr_t> &memory, InsnMc68000 &insn,
    const EaMc68000 &ea) {
    const EaMode mode = ea.mode;
    if (mode == M_ILLEGAL)
        return setError(ILLEGAL_OPERAND);
    if (mode == M_DREG || mode == M_AREG) {
        outRegName(ea.reg);
        return setError(OK);
    }
    if (mode == M_IMM_DATA) {
        *_operands++ = '#';
        return decodeImmediateData(memory, insn, ea.size);
    }

    if (mode == M_PDEC) *_operands++ = '-';
    *_operands++ = '(';
    if (mode == M_DISP || mode == M_PC_DISP) {
        const RegName base = (mode == M_DISP) ? ea.reg : REG_PC;
        uint16_t val16;
        if (insn.readUint16(memory, val16)) return setError(NO_MEMORY);
        if (mode == M_PC_DISP) {
            const Config::uintptr_t addr =
                insn.address() + 2 + static_cast<int16_t>(val16);
            outConstant(addr, 16, false);
        } else {
            if (val16 & 0x8000) {
                const uint16_t disp16 = 0x10000 - val16;
                *_operands++ = '-';
                outConstant(disp16);
            } else {
                outConstant(val16);
            }
        }
        *_operands++ = ',';
        outRegName(base);
    }
    if (mode == M_AIND || mode == M_PINC || mode == M_PDEC) {
        outRegName(ea.reg);
    }
    if (mode == M_ABS_SHORT) {
        uint16_t val16;
        if (insn.readUint16(memory, val16)) return setError(NO_MEMORY);
        if (val16 & 0x8000) {
            const int16_t signed16 = static_cast<int16_t>(val16);
            const int32_t signed32 = static_cast<int32_t>(signed16);
            outConstant(signed32, -16);
        } else {
            outConstant(val16, 16);
        }
    }
    if (mode == M_ABS_LONG) {
        uint32_t val32;
        if (insn.readUint32(memory, val32)) return setError(NO_MEMORY);
        outConstant(val32, 16);
    }
    if (mode == M_INDX || mode == M_PC_INDX) {
        const RegName base = (mode == M_INDX) ? ea.reg : REG_PC;
        BriefExt ext;
        if (insn.readUint16(memory, ext.word)) return setError(NO_MEMORY);
        const uint8_t val8 = ext.disp();
        if (mode == M_PC_INDX) {
            const Config::uintptr_t addr =
                insn.address() + 2 + static_cast<int8_t>(val8);
            outConstant(addr, 16);
        } else {
            if (val8 & 0x80) {
                const uint8_t disp8 = 0x100 - val8;
                *_operands++ = '-';
                outConstant(disp8);
            } else {
                outConstant(val8);
            }
        }
        *_operands++ = ',';
        outRegName(base);
        *_operands++ = ',';
        outRegName(ext.index());
        outEaSize(ext.indexSize());
    }
    *_operands++ = ')';
    if (mode == M_ABS_SHORT) outEaSize(SZ_WORD);
    if (mode == M_ABS_LONG)  outEaSize(SZ_LONG);
    if (mode == M_PINC) *_operands++ = '+';
    *_operands = 0;
    return setError(OK);
}

Error DisMc68000::decodeImplied(
    DisMemory<Config::uintptr_t> &memory, InsnMc68000 &insn) {
    if (insn.insnCode() == 047162) { // STOP
        *_operands++ = '#';
        return decodeImmediateData(memory, insn, SZ_WORD);
    }
    return setError(OK);
}

// ORI, ANDI, SUBI, ADDI, EORI, CMPI
// NEGX, CLR, NEG, NOT, TST
Error DisMc68000::decodeDestSiz(
    DisMemory<Config::uintptr_t> &memory, InsnMc68000 &insn) {
    const Config::insn_t insnCode = insn.insnCode();
    const EaMc68000 ea(insnCode);
    const uint8_t opc = (insnCode >> 9) & 7;

    if ((insnCode >> 12) == 0) { // ORI/ANDI/SUBI/ADDI/EORI/CMPI
        *_operands++ = '#';
        if (decodeImmediateData(memory, insn, ea.size))
            return getError();
        *_operands++ = ',';
        constexpr uint8_t ORI  = 00;
        constexpr uint8_t ANDI = 01;
        constexpr uint8_t EORI = 05;
        constexpr uint8_t CMPI = 06;
        if (opc == ORI || opc == ANDI || opc == EORI) {
            if (ea.mode == M_IMM_DATA) {
                if (ea.size == SZ_BYTE) {
                    outRegName(REG_CCR);
                    return setError(OK);
                } else if (ea.size == SZ_WORD) {
                    outRegName(REG_SR);
                    return setError(OK);
                }
            }
        }
        if (opc == CMPI && !ea.satisfy(CAT_DATA))
            return setError(ILLEGAL_OPERAND_MODE);
        if (!ea.satisfy(CAT_DATA | CAT_ALTERABLE))
            return setError(ILLEGAL_OPERAND_MODE);
    } else {                    // NEGX/CLR/NEG/NOT/TST
        constexpr uint8_t TST = 5;
        if (opc == TST && !ea.satisfy(CAT_DATA))
            return setError(ILLEGAL_OPERAND_MODE);
        if (!ea.satisfy(CAT_DATA | CAT_ALTERABLE))
            return setError(ILLEGAL_OPERAND_MODE);
    }

    insn.appendSize(ea.size, _regs);
    return decodeEffectiveAddr(memory, insn, ea);
}

// LINK, UNLK
Error DisMc68000::decodeAddrReg(
    DisMemory<Config::uintptr_t> &memory, InsnMc68000 &insn) {
    const RegName dest = RegMc68000::decodeAddrReg(insn.insnCode());
    if (insn.insnCode() & 010) { // UNLK
        outRegName(dest);
        return setError(OK);
    }
    // LINK
    outRegName(dest);
    *_operands++ = ',';
    uint16_t val16;
    if (insn.readUint16(memory, val16)) return setError(NO_MEMORY);
    const int16_t disp = static_cast<int16_t>(val16);
    *_operands++ = '#';
    outConstant(disp);
    return setError(OK);
}

// DBcc, SWAP
Error DisMc68000::decodeDataReg(
    DisMemory<Config::uintptr_t> &memory, InsnMc68000 &insn) {
    const RegName dest = RegMc68000::decodeDataReg(insn.insnCode());
    if ((insn.insnCode() >> 12) == 4) { // SWAP
        outRegName(dest);
        return setError(OK);
    }

    // DBcc
    outRegName(dest);
    *_operands++ = ',';
    uint16_t val16;
    if (insn.readUint16(memory, val16)) return setError(NO_MEMORY);
    const int16_t disp = static_cast<int16_t>(val16);
    const Config::uintptr_t addr = insn.address() + 2 + disp;
    const char *label = lookup(addr);
    if (label) {
        outText(label);
    } else {
        outConstant(addr, 16);
    }
    return setError(OK);
}

// MOVE USP
Error DisMc68000::decodeMoveUsp(
    DisMemory<Config::uintptr_t> &memory, InsnMc68000 &insn) {
    const RegName areg = RegMc68000::decodeAddrReg(insn.insnCode());
    if (insn.insnCode() & 010) { // USP->An
        outRegName(REG_USP);
        *_operands++ = ',';
        outRegName(areg);
    } else {                    // An ->USP
        outRegName(areg);
        *_operands++ = ',';
        outRegName(REG_USP);
    }
    return setError(OK);
}

// TRAP
Error DisMc68000::decodeTrapVec(
    DisMemory<Config::uintptr_t> &memory, InsnMc68000 &insn) {
    *_operands++ = '#';
    outConstant(static_cast<uint8_t>(insn.insnCode() & 017), 10);
    return setError(OK);
}

// NBCD, PEA, TAS
Error DisMc68000::decodeDataDst(
    DisMemory<Config::uintptr_t> &memory, InsnMc68000 &insn) {
    const Config::insn_t insnCode = insn.insnCode();
    const uint8_t opc = (insnCode >> 6) & 077;
    constexpr uint8_t NBCD = 040;
    constexpr uint8_t PEA = 041;
    EaSize size = (opc == PEA) ? SZ_LONG : SZ_BYTE;
    const EaMc68000 ea(size,  insnCode >> 3, insnCode);
    if (opc == NBCD) {
        if (!ea.satisfy(CAT_DATA | CAT_ALTERABLE))
            return setError(ILLEGAL_OPERAND_MODE);
    } else if (opc == PEA) {
        if (!ea.satisfy(CAT_CONTROL))
            return setError(ILLEGAL_OPERAND_MODE);
        size = SZ_NONE;
    } else {                    // opc == TAS(053)
        if (!ea.satisfy(CAT_DATA))
            return setError(ILLEGAL_OPERAND_MODE);
    }
    insn.appendSize(size, _regs);
    return decodeEffectiveAddr(memory, insn, ea);
}

// BTST, BCHG, BCLR, BSET
// JSR, JMP, Scc,
// ASR, ASL, LSR, LSL, ROXR, ROXL
Error DisMc68000::decodeDestOpr(
    DisMemory<Config::uintptr_t> &memory, InsnMc68000 &insn) {
    const EaMc68000 ea(insn.insnCode());
    EaSize size = ea.size;

    if ((insn.insnCode() >> 12) == 0) { // BTST/BCHG/BCLR/BSET
        const uint8_t opc = (insn.insnCode() >> 6) & 3;
        constexpr uint8_t BTST = 0;
        if (opc == BTST && !ea.satisfy(CAT_DATA))
            return setError(ILLEGAL_OPERAND_MODE);
        if (!ea.satisfy(CAT_DATA | CAT_ALTERABLE))
            return setError(ILLEGAL_OPERAND_MODE);
        size = (ea.mode == M_DREG) ? SZ_LONG : SZ_BYTE;
        *_operands++ = '#';
        if (decodeImmediateData(memory, insn, SZ_BYTE))
            return getError();
        *_operands++ = ',';
    } else {                    // JSR/JMP/Scc/ASx/LSx/ROXx
        const uint8_t opc = (insn.insnCode() >> 12) & 077;
        constexpr uint8_t JSR_JMP = 004;
        constexpr uint8_t Scc = 005;
        if (opc == JSR_JMP) {
            if (!ea.satisfy(CAT_CONTROL))
                return setError(ILLEGAL_OPERAND_MODE);
            size = SZ_NONE;
        } else if (opc == Scc) {
            if (!ea.satisfy(CAT_DATA | CAT_ALTERABLE))
                return setError(ILLEGAL_OPERAND_MODE);
            size = SZ_NONE;
        } else {
            if (!ea.satisfy(CAT_MEMORY | CAT_ALTERABLE))
                return setError(ILLEGAL_OPERAND_MODE);
        }
    }

    insn.appendSize(size, _regs);
    return decodeEffectiveAddr(memory, insn, ea);
}

// EXT
Error DisMc68000::decodeSignExt(
    DisMemory<Config::uintptr_t> &memory, InsnMc68000 &insn) {
    const Config::insn_t insnCode = insn.insnCode();
    insn.appendSize((insnCode & 0100) ? SZ_LONG : SZ_WORD, _regs);
    outRegName(RegMc68000::decodeDataReg(insnCode));
    return setError(OK);
}

// EXT_BRA: BRA, BSR, Bcc
Error DisMc68000::decodeRelative(
    DisMemory<Config::uintptr_t> &memory, InsnMc68000 &insn) {
    const uint8_t val8 = static_cast<uint8_t>(insn.insnCode());
    Config::uintptr_t addr = insn.address() + 2;
    if (val8) {
        addr += static_cast<int8_t>(val8);
    } else {
        uint16_t val16;
        if (insn.readUint16(memory, val16)) return setError(NO_MEMORY);
        addr += static_cast<int16_t>(val16);
    }
    const char *label = lookup(addr);
    if (label) {
        outText(label);
    } else {
        outConstant(addr, 16);
    }
    return setError(OK);
}

static RegName decodeMoveMltReg(host::int_t regno) {
    return (regno < 8)
        ? RegMc68000::decodeDataReg(regno)
        : RegMc68000::decodeAddrReg(regno - 8);
}

void DisMc68000::decodeMoveMltRegList(
    uint16_t list, bool push,
    void (DisMc68000::*outRegs)(RegName, RegName, char)) {
    host::int_t start = -1;
    host::int_t last = 0;
    uint16_t mask = push ? 0x8000 : 0x0001;
    for (host::int_t i = 0; i < 16; i++) {
        if (list & mask) {
            if (start < 0) {
                start = last = i;
            } else if (i == last + 1) {
                last = i;
            } else {
                (this->*outRegs)(
                    decodeMoveMltReg(start), decodeMoveMltReg(last), '/');
                start = last = i;
            }
        }
        if (push) mask >>= 1;
        else mask <<= 1;
    }
    if (start >= 0)
        (this->*outRegs)(
            decodeMoveMltReg(start), decodeMoveMltReg(last), 0);
}

void DisMc68000::outMoveMltRegs(RegName start, RegName last, char suffix) {
    outRegName(start);
    if (start != last) {
        *_operands++ = '-';
        outRegName(last);
    }
    if (suffix) *_operands++ = suffix;
    *_operands = 0;
}

// MOVEM
Error DisMc68000::decodeMoveMlt(
    DisMemory<Config::uintptr_t> &memory, InsnMc68000 &insn) {
    const Config::insn_t insnCode = insn.insnCode();
    const EaMc68000 ea(
        (insnCode & 0100) ? SZ_LONG : SZ_WORD, insnCode >> 3, insnCode);
    uint16_t list;
    if (insn.readUint16(memory, list)) return setError(NO_MEMORY);
    const bool pop = (insnCode & 02000);
    if (pop) {
        if (!(ea.mode == M_PINC || ea.satisfy(CAT_CONTROL)))
            return setError(ILLEGAL_OPERAND_MODE);
        decodeEffectiveAddr(memory, insn, ea);
        *_operands++ = ',';
        decodeMoveMltRegList(list, false, &DisMc68000::outMoveMltRegs);
        *_operands++ = 0;
    } else {
        bool push;
        if (ea.mode == M_PDEC) {
            push = true;
        } else if (ea.satisfy(CAT_CONTROL | CAT_ALTERABLE)) {
            push = false;
        } else {
            return setError(ILLEGAL_OPERAND_MODE);
        }
        decodeMoveMltRegList(list, push, &DisMc68000::outMoveMltRegs);
        *_operands++ = ',';
        decodeEffectiveAddr(memory, insn, ea);
    }
    insn.appendSize(ea.size, _regs);
    return getError();
}

// MOVE fromSR, toSR, toCCR
Error DisMc68000::decodeMoveSr(
    DisMemory<Config::uintptr_t> &memory, InsnMc68000 &insn) {
    const Config::insn_t insnCode = insn.insnCode();
    const host::uint_t opc = (insnCode >> 8) & 017;
    constexpr host::uint_t toCCR = 4;
    constexpr host::uint_t fromSR = 0;
    const EaSize size = (opc == toCCR) ? SZ_BYTE : SZ_WORD;
    const EaMc68000 ea(size, insnCode >> 3, insnCode);
    if (opc == fromSR) {
        if (!ea.satisfy(CAT_DATA | CAT_ALTERABLE))
            return setError(ILLEGAL_OPERAND_MODE);
        outRegName(REG_SR);
        *_operands++ = ',';
        decodeEffectiveAddr(memory, insn, ea);            
    } else {
        if (!ea.satisfy(CAT_DATA))
            return setError(ILLEGAL_OPERAND_MODE);
        decodeEffectiveAddr(memory, insn, ea);
        *_operands++ = ',';
        outRegName(opc == toCCR ? REG_CCR : REG_SR);
    }
    return getError();
}

// MOVEQ
Error DisMc68000::decodeMoveQic(
    DisMemory<Config::uintptr_t> &memory, InsnMc68000 &insn) {
    const Config::insn_t insnCode = insn.insnCode();
    const uint8_t val8 = static_cast<uint8_t>(insnCode);
    *_operands++ = '#';
    outConstant(static_cast<int8_t>(val8));
    *_operands++ = ',';
    outRegName(RegMc68000::decodeDataReg(insnCode >> 9));
    return setError(OK);
}

// MOVEP
Error DisMc68000::decodeMovePer(
    DisMemory<Config::uintptr_t> &memory, InsnMc68000 &insn) {
    const Config::insn_t insnCode = insn.insnCode();
    const EaMc68000 ea(
        (insnCode & 0100) ? SZ_LONG : SZ_WORD, M_DISP, insnCode);
    const RegName dest = RegMc68000::decodeDataReg(insnCode >> 9);
    if (insnCode & 0200) {      // Dn->EA
        outRegName(dest);
        *_operands++ = ',';
        decodeEffectiveAddr(memory, insn, ea);
    } else {                    // EA->Dn
        decodeEffectiveAddr(memory, insn, ea);
        *_operands++ = ',';
        outRegName(dest);
    }
    insn.appendSize(ea.size, _regs);
    return getError();
}

// AREG_LNG: LEA
// AREG_SIZ: SUBA, CMPA, ADDA
Error DisMc68000::decodeAregSiz(
    DisMemory<Config::uintptr_t> &memory, InsnMc68000 &insn) {
    const Config::insn_t insnCode = insn.insnCode();
    EaSize size = (insnCode & 0400) ? SZ_LONG : SZ_WORD;
    const EaMc68000 ea(size, insnCode >> 3, insnCode);
    const uint8_t opc = (insnCode >> 12) & 017;
    constexpr uint8_t LEA = 004;
    if (opc == LEA) {
        if (size == SZ_WORD)
            return setError(ILLEGAL_SIZE);
        if (!ea.satisfy(CAT_CONTROL))
            return setError(ILLEGAL_OPERAND_MODE);
        size = SZ_NONE;
    }

    decodeEffectiveAddr(memory, insn, ea);
    *_operands++ = ',';
    outRegName(RegMc68000::decodeAddrReg(insnCode >> 9));
    insn.appendSize(size, _regs);
    return getError();
}

// NO_EXT: BTST, BCHG, BCLR, BSET, CHK, DIVU, DIVS, MULU, MULS
Error DisMc68000::decodeDregDst(
    DisMemory<Config::uintptr_t> &memory, InsnMc68000 &insn) {
    const Config::insn_t insnCode = insn.insnCode();
    const RegName dest = RegMc68000::decodeDataReg(insn.insnCode() >> 9);
    EaSize size;

    const uint8_t family = (insnCode >> 12) & 017;
    constexpr uint8_t Bxxx = 0;
    if (family == Bxxx) {
        const uint16_t opc = (insnCode >> 6) & 7;
        constexpr uint8_t BTST = 4;
        const uint8_t mode = (insnCode >> 3) & 7;
        size = (mode == /* M_DREG */0) ? SZ_LONG : SZ_BYTE;
        const EaMc68000 ea(size, mode, insnCode);
        if (opc == BTST && !ea.satisfy(CAT_DATA))
            return setError(ILLEGAL_OPERAND_MODE);
        if (!ea.satisfy(CAT_DATA | CAT_ALTERABLE))
            return setError(ILLEGAL_OPERAND_MODE);
        outRegName(dest);
        *_operands++ = ',';
        decodeEffectiveAddr(memory, insn, ea);
    } else {
        size = SZ_WORD;
        const EaMc68000 ea(size, insnCode >> 3, insnCode);
        if (!ea.satisfy(CAT_DATA))
            return setError(ILLEGAL_OPERAND_MODE);
        decodeEffectiveAddr(memory, insn, ea);
        *_operands++ = ',';
        outRegName(dest);
    }
    insn.appendSize(size, _regs);
    return getError();
}

static EaSize moveSize(host::uint_t moveSize) {
    switch (moveSize & 3) {
    case 1: return SZ_BYTE;
    case 2: return SZ_LONG;
    case 3: return SZ_WORD;
    default: return SZ_INVALID;
    }
}

// ADDQ, SUBQ
Error DisMc68000::decodeDataQic(
    DisMemory<Config::uintptr_t> &memory, InsnMc68000 &insn) {
    const Config::insn_t insnCode = insn.insnCode();
    const EaMc68000 ea(insnCode);
    uint8_t val = (insnCode >> 9) & 7;
    if (val == 0) val = 8;
    if (!ea.satisfy(CAT_ALTERABLE))
        return setError(ILLEGAL_OPERAND_MODE);
    insn.appendSize(ea.size, _regs);
    *_operands++ = '#';
    outConstant(val);
    *_operands++ = ',';
    return decodeEffectiveAddr(memory, insn, ea);
}

// DMEM_SIZ: OR, SUB, AND, ADD
// DREG_SIZ: CMP, EOR
Error DisMc68000::decodeDmemSiz(
    DisMemory<Config::uintptr_t> &memory, InsnMc68000 &insn) {
    const Config::insn_t insnCode = insn.insnCode();
    const EaMc68000 ea(insnCode);
    const RegName dreg = RegMc68000::decodeDataReg(insnCode >> 9);
    insn.appendSize(ea.size, _regs);
    if (insnCode & 0400) {      // Dn,<ea>
        if (!ea.satisfy(CAT_DATA | CAT_ALTERABLE))
            return setError(ILLEGAL_OPERAND_MODE);
        outRegName(dreg);
        *_operands++ = ',';
        decodeEffectiveAddr(memory, insn, ea);
    } else {                    // <ea>,Dn
        decodeEffectiveAddr(memory, insn, ea);
        *_operands++ = ',';
        outRegName(dreg);
    }
    return getError();
}

// ASR, ASL, LSR, LSL, ROXR, ROXL, ROR, ROL
Error DisMc68000::decodeDregRot(
    DisMemory<Config::uintptr_t> &memory, InsnMc68000 &insn) {
    const Config::insn_t insnCode = insn.insnCode();
    const RegName dst = RegMc68000::decodeDataReg(insnCode);
    const EaSize size = EaSize((insnCode >> 6) & 3);
    if (insnCode & 040) {       // Dx
        outRegName(RegMc68000::decodeDataReg(insnCode >> 9));
    } else {                    // #n
        uint8_t val = (insnCode >> 9) & 7;
        if (val == 0) val = 8;
        *_operands++ = '#';
        outConstant(val, 10);
    }
    *_operands++ = ',';
    outRegName(dst);
    insn.appendSize(size, _regs);
    return setError(OK);
}

// DMEM_DST: SBCD, ABCD
// DMEM_OPR: SUBX, ADDX
// CMPM_SIZ: CMPM
Error DisMc68000::decodeDmemOpr(
    DisMemory<Config::uintptr_t> &memory, InsnMc68000 &insn) {
    const Config::insn_t insnCode = insn.insnCode();
    const uint8_t opc = (insnCode >> 12);
    constexpr uint8_t SBCD = 010;
    constexpr uint8_t ABCD = 014;
    constexpr uint8_t CMPM = 013;
    const EaSize size = (opc == SBCD || opc == ABCD) ? SZ_NONE
        : EaSize((insnCode >> 6) & 3);
    insn.appendSize(size, _regs);
    if (insnCode & 010) {       // -(Ay),-(Ax) or (Ay)+,(Ax)+
        const EaMode mode = (opc == CMPM) ? M_PINC : M_PDEC;
        const EaMc68000 src(SZ_BYTE, host::uint_t(mode), insnCode);
        const EaMc68000 dst(SZ_BYTE, host::uint_t(mode), insnCode >> 9);
        decodeEffectiveAddr(memory, insn, src);
        *_operands++ = ',';
        decodeEffectiveAddr(memory, insn, dst);
    } else {                    // Dy,Dx
        outRegName(RegMc68000::decodeDataReg(insnCode));
        *_operands++ = ',';
        outRegName(RegMc68000::decodeDataReg(insnCode >> 9));
    }
    return setError(OK);
}

// EXG
Error DisMc68000::decodeRegsExg(
    DisMemory<Config::uintptr_t> &memory, InsnMc68000 &insn) {
    const Config::insn_t insnCode = insn.insnCode();
    const uint8_t mode = (insnCode >> 3) & 031;
    if (mode == 010) {          // Dx,Dy
        outRegName(RegMc68000::decodeDataReg(insnCode >> 9));
        *_operands++ = ',';
        outRegName(RegMc68000::decodeDataReg(insnCode));
    } else if (mode == 011) {   // Ax,Ay
        outRegName(RegMc68000::decodeAddrReg(insnCode >> 9));
        *_operands++ = ',';
        outRegName(RegMc68000::decodeAddrReg(insnCode));
    } else {                    // Dx,Ay
        outRegName(RegMc68000::decodeDataReg(insnCode >> 9));
        *_operands++ = ',';
        outRegName(RegMc68000::decodeAddrReg(insnCode));
    }
    return setError(OK);
}

// NO_EXT: MOVE, MOVEA
Error DisMc68000::decodeMoveOpr(
    DisMemory<Config::uintptr_t> &memory, InsnMc68000 &insn) {
    const Config::insn_t insnCode = insn.insnCode();
    const EaSize size = moveSize(insnCode >> 12);
    const EaMc68000 src(size, insnCode >> 3, insnCode);
    const EaMc68000 dst(size, insnCode >> 6, insnCode >> 9);
    if (dst.mode == M_AREG) { // MOVEA
        if (size == SZ_BYTE) return setError(ILLEGAL_SIZE);
    } else {
        if (!dst.satisfy(CAT_DATA | CAT_ALTERABLE))
            return setError(ILLEGAL_OPERAND_MODE);
    }
    if (decodeEffectiveAddr(memory, insn, src)) return getError();
    *_operands++ = ',';
    if (decodeEffectiveAddr(memory, insn, dst)) return getError();
    insn.appendSize(size, _regs);
    return setError(OK);
}

Error DisMc68000::decode(
    DisMemory<Config::uintptr_t> &memory, Insn<Config::uintptr_t> &_insn) {
    InsnMc68000 insn(_insn);
    Config::insn_t insnCode;
    if (insn.readUint16(memory, insnCode)) return setError(NO_MEMORY);
    insn.setInsnCode(insnCode);
    if (TableMc68000.searchInsnCode(insn))
        return getError();

    switch (insn.insnFormat()) {
    case IMPLIED:  return decodeImplied(memory, insn);
    case DEST_SIZ: return decodeDestSiz(memory, insn);
    case ADDR_REG: return decodeAddrReg(memory, insn);
    case DATA_REG: return decodeDataReg(memory, insn);
    case MOVE_USP: return decodeMoveUsp(memory, insn);
    case TRAP_VEC: return decodeTrapVec(memory, insn);
    case DATA_DST: return decodeDataDst(memory, insn);
    case DEST_OPR: return decodeDestOpr(memory, insn);
    case SIGN_EXT: return decodeSignExt(memory, insn);
    case RELATIVE: return decodeRelative(memory, insn);
    case MOVE_MLT: return decodeMoveMlt(memory, insn);
    case MOVE_SR:  return decodeMoveSr(memory, insn);
    case AREG_LNG:
    case AREG_SIZ: return decodeAregSiz(memory, insn);
    case DREG_DST: return decodeDregDst(memory, insn);
    case MOVE_QIC: return decodeMoveQic(memory, insn);
    case MOVE_PER: return decodeMovePer(memory, insn);
    case DATA_QIC: return decodeDataQic(memory, insn);
    case DREG_SIZ:
    case DMEM_SIZ: return decodeDmemSiz(memory, insn);
    case DREG_ROT: return decodeDregRot(memory, insn);
    case CMPM_SIZ:
    case DMEM_DST:
    case DMEM_OPR: return decodeDmemOpr(memory, insn);
    case REGS_EXG: return decodeRegsExg(memory, insn);
    case MOVA_OPR:
    case MOVE_OPR: return decodeMoveOpr(memory, insn);
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
