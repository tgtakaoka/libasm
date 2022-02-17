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

#include "asm_i8086.h"

namespace libasm {
namespace i8086 {

Error AsmI8086::parseStringInst(StrScanner &scan, Operand &op) const {
    Insn _insn(0);
    InsnI8086 insn(_insn);
    StrScanner p(scan);
    insn.setName(_parser.readSymbol(p));
    insn.setAddrMode(M_NONE, M_NONE);
    if (TableI8086.searchName(insn))
        return UNKNOWN_INSTRUCTION;
    if (!insn.stringInst())
        return UNKNOWN_INSTRUCTION;
    scan = p.skipSpaces();
    op.val32 = insn.opCode();
    op.mode = M_ISTR;
    return OK;
}

Error AsmI8086::parsePointerSize(StrScanner &scan, Operand &op) {
    StrScanner p(scan);
    const RegName reg = RegI8086::parseRegName(p);
    if (reg == REG_BYTE || reg == REG_WORD) {
        // Pointer size override
        if (RegI8086::parseRegName(p.skipSpaces()) == REG_PTR) {
            op.ptr = reg;
            scan = p.skipSpaces();
            return OK;
        }
        return setError(p, UNKNOWN_OPERAND);
    }
    return OK;
}

void AsmI8086::parseSegmentOverride(StrScanner &scan, Operand &op) const {
    StrScanner p(scan);
    const RegName reg = RegI8086::parseRegName(p);
    if (RegI8086::isSegmentReg(reg)) {
        // Segment Override
        if (p.skipSpaces().expect(':')) {
            op.seg = reg;
            scan = p;
        }
    }
}

void AsmI8086::parseBaseRegister(StrScanner &scan, Operand &op) const {
    StrScanner p(scan);
    const RegName reg = RegI8086::parseRegName(p);
    if (reg == REG_BX || reg == REG_BP) {
        op.reg = reg;
        scan = p.skipSpaces();
    }
}

void AsmI8086::parseIndexRegister(StrScanner &scan, Operand &op) const {
    StrScanner p(scan);
    if (op.reg != REG_UNDEF) {
        if (!p.expect('+'))
            return;
        p.skipSpaces();
    }
    const RegName reg = RegI8086::parseRegName(p);
    if (reg == REG_SI || reg == REG_DI) {
        op.index = reg;
        scan = p.skipSpaces();
    }
}

Error AsmI8086::parseDisplacement(StrScanner &scan, Operand &op) {
    StrScanner p(scan);
    if (endOfLine(*p) || *p == ']')
        return OK;
    if (op.reg != REG_UNDEF || op.index != REG_UNDEF) {
        if (*p != '+' && *p != '-')
            return setError(p, UNKNOWN_OPERAND);
    }
    op.val32 = parseExpr32(p, op);
    if (parserError())
        return getError();
    if (overflowUint16(static_cast<int32_t>(op.val32)))
        return setError(op, OVERFLOW_RANGE);
    op.hasVal = true;
    scan = p.skipSpaces();
    return OK;
}

Error AsmI8086::parseOperand(StrScanner &scan, Operand &op) {
    StrScanner p(scan.skipSpaces());
    op.setAt(p);
    if (endOfLine(*p))
        return OK;

    if (parseStringInst(p, op) == OK) {
        scan = p;
        return OK;
    }
    if (parsePointerSize(p, op))
        return getError();
    parseSegmentOverride(p, op);
    if (p.skipSpaces().expect('[')) {
        parseBaseRegister(p.skipSpaces(), op);
        parseIndexRegister(p, op);
        if (parseDisplacement(p, op))
            return getError();
        if (!p.skipSpaces().expect(']'))
            return setError(p, MISSING_CLOSING_BRACKET);
        scan = p;
        if (op.reg == REG_UNDEF && op.index == REG_UNDEF) {
            if (!op.hasVal)
                return setError(op, UNKNOWN_OPERAND);
            op.mode = (op.ptr == REG_UNDEF) ? M_DIR : (op.ptr == REG_BYTE ? M_BDIR : M_WDIR);
            return OK;
        }
        op.mode = (op.ptr == REG_UNDEF) ? M_MEM : (op.ptr == REG_BYTE ? M_BMEM : M_WMEM);
        return OK;
    }
    if (op.ptr != REG_UNDEF || op.seg != REG_UNDEF)
        return setError(op, UNKNOWN_OPERAND);

    StrScanner a(p);
    const RegName reg = RegI8086::parseRegName(a);
    if (RegI8086::isGeneralReg(reg)) {
        op.reg = reg;
        switch (reg) {
        case REG_AL:
            op.mode = M_AL;
            break;
        case REG_CL:
            op.mode = M_CL;
            break;
        case REG_AX:
            op.mode = M_AX;
            break;
        case REG_DX:
            op.mode = M_DX;
            break;
        default:
            op.mode = (RegI8086::generalRegSize(reg) == SZ_BYTE) ? M_BREG : M_WREG;
            break;
        }
        scan = a;
        return OK;
    }
    if (RegI8086::isSegmentReg(reg)) {
        op.reg = reg;
        op.mode = (reg == REG_CS) ? M_CS : M_SREG;
        scan = a;
        return OK;
    }
    if (reg != REG_UNDEF)
        return setError(p, UNKNOWN_OPERAND);

    op.val32 = parseExpr32(p, op);
    if (parserError())
        return getError();
    if (p.skipSpaces().expect(':')) {
        if (op.val32 >= 0x10000UL)
            return setError(op, OVERFLOW_RANGE);
        op.seg16 = op.val32;
        op.val32 = parseExpr32(p, op);
        if (op.val32 >= 0x10000UL)
            return setError(op, OVERFLOW_RANGE);
        if (parserError())
            return getError();
        op.mode = M_FAR;
        scan = p;
        return OK;
    }
    op.mode = op.immediateMode();
    if (op.mode == M_NONE)
        return setError(op, OVERFLOW_RANGE);
    scan = p;
    return OK;
}

AddrMode AsmI8086::Operand::immediateMode() const {
    if (getError())
        return M_IMM;
    if (val32 == 1)
        return M_VAL1;
    if (val32 == 3)
        return M_VAL3;
    if (!overflowUint8(val32))
        return M_IMM8;
    if (!overflowUint16(val32))
        return M_IMM;
    return M_NONE;
}

Error AsmI8086::emitImmediate(InsnI8086 &insn, const Operand &op, OprSize size, uint32_t val) {
    if (size == SZ_BYTE) {
        if (overflowUint8(val))
            return setError(op, OVERFLOW_RANGE);
        insn.emitOperand8(val);
    }
    if (size == SZ_WORD) {
        if (overflowUint16(val))
            return setError(op, OVERFLOW_RANGE);
        insn.emitOperand16(val);
    }
    return OK;
}

Error AsmI8086::emitRelative(InsnI8086 &insn, const Operand &op, AddrMode mode) {
    const Config::uintptr_t base = insn.address() + (mode == M_REL8 ? 2 : 3);
    const Config::uintptr_t target = op.getError() ? base : op.val32;
    const Config::ptrdiff_t delta = target - base;
    if (mode == M_REL8) {
        const bool overflow = (delta < -0x80 || delta >= 0x80);
        if (insn.opCode() == 0xEB && (overflow || op.getError())) {
            insn.setOpCode(0xE9, 0);
            return emitRelative(insn, op, M_REL);
        }
        if (overflow)
            return setError(op, OPERAND_TOO_FAR);
        insn.emitOperand8(static_cast<uint8_t>(delta));
        return OK;
    }
    // M_REL
    if (delta < -0x8000 || delta >= 0x8000)
        return setError(op, OPERAND_TOO_FAR);
    insn.emitOperand16(static_cast<uint16_t>(delta));
    return OK;
}

Error AsmI8086::emitRegister(InsnI8086 &insn, const Operand &op, OprPos pos) {
    const uint8_t num = RegI8086::encodeRegNum(op.reg);
    switch (pos) {
    case P_OREG:
        insn.embed(num);
        break;
    case P_OSEG:
        insn.embed(num << 3);
        break;
    case P_OMOD:
        insn.embed(0300 | num);
        break;
    case P_REG:
        insn.embedModReg(num << 3);
        break;
    case P_MOD:
        insn.embedModReg(0300 | num);
        break;
    default:
        break;
    }
    return OK;
}

uint8_t AsmI8086::Operand::encodeMod() const {
    const bool needDisp =
            (reg == REG_BP && index == REG_UNDEF) || (hasVal && (val32 || getError()));
    if (needDisp) {
        const int32_t val = static_cast<int32_t>(val32);
        return (val < -0x80 || val >= 0x80 || getError()) ? 2 : 1;
    }
    return 0;
}

uint8_t AsmI8086::Operand::encodeR_m() const {
    uint8_t r_m = 0;
    if (reg == REG_UNDEF) {
        r_m = (index == REG_SI) ? 4 : 5;
    } else if (index == REG_UNDEF) {
        r_m = (reg == REG_BP) ? 6 : 7;
    } else {
        if (reg == REG_BP)
            r_m |= 2;
        if (index == REG_DI)
            r_m |= 1;
    }
    return r_m;
}

Config::opcode_t AsmI8086::encodeSegmentOverride(RegName seg, RegName base) {
    if (seg == REG_UNDEF)
        return 0;
    const Config::opcode_t segPrefix = TableI8086.segOverridePrefix(seg);
    if (_optimizeSegment) {
        if (base == REG_BP || base == REG_SP)
            return seg == REG_SS ? 0 : segPrefix;
        return seg == REG_DS ? 0 : segPrefix;
    }
    return segPrefix;
}

Error AsmI8086::emitModReg(InsnI8086 &insn, const Operand &op, OprPos pos) {
    uint8_t mod;
    uint8_t modReg;
    switch (op.mode) {
    case M_AL:
    case M_CL:
    case M_AX:
    case M_DX:
    case M_BREG:
    case M_WREG:
        return emitRegister(insn, op, pos);
    case M_BDIR:
    case M_WDIR:
    case M_DIR:
        return emitDirect(insn, op, pos);
    case M_BMEM:
    case M_WMEM:
    case M_MEM:
        insn.setSegment(encodeSegmentOverride(op.seg, op.reg));
        mod = op.encodeMod();
        modReg = mod << 6;
        modReg |= op.encodeR_m();
        if (pos == P_OMOD) {
            insn.embed(modReg);
        } else {
            insn.embedModReg(modReg);
        }
        if (mod == 1) {
            if (overflowRel8(static_cast<int32_t>(op.val32)))
                return setError(op, OVERFLOW_RANGE);
            insn.emitOperand8(op.val32);
        } else if (mod == 2) {
            if (overflowUint16(static_cast<int32_t>(op.val32)))
                return setError(op, OVERFLOW_RANGE);
            insn.emitOperand16(op.val32);
        }
        break;
    default:
        break;
    }
    return OK;
}

Error AsmI8086::emitDirect(InsnI8086 &insn, const Operand &op, OprPos pos) {
    insn.setSegment(encodeSegmentOverride(op.seg, REG_UNDEF));
    if (pos == P_MOD)
        insn.embedModReg(0006);
    if (pos == P_OMOD)
        insn.embed(0006);
    return emitImmediate(insn, op, SZ_WORD, op.val32);
}

Error AsmI8086::emitOperand(InsnI8086 &insn, AddrMode mode, const Operand &op, OprPos pos) {
    switch (mode) {
    case M_CS:
        if (pos == P_NONE)  // POP CS
            return setError(op, REGISTER_NOT_ALLOWED);
        /* Fall-through */
    case M_BREG:
    case M_WREG:
    case M_SREG:
        return emitRegister(insn, op, pos);
    case M_BMOD:
    case M_WMOD:
    case M_BMEM:
    case M_WMEM:
        return emitModReg(insn, op, pos);
    case M_BDIR:
    case M_WDIR:
        return emitDirect(insn, op, P_OPR);
    case M_UI16:
        if (static_cast<int32_t>(op.val32) >= 0x10000 || static_cast<int32_t>(op.val32) < 0)
            return setError(op, OVERFLOW_RANGE);
        insn.emitOperand16(op.val32);
        return OK;
    case M_IMM:
        return emitImmediate(insn, op, insn.oprSize(), op.val32);
    case M_IOA:
        if (op.val32 >= 0x100)
            return setError(op, OVERFLOW_RANGE);
        /* Fall-through */
    case M_IMM8:
        return emitImmediate(insn, op, SZ_BYTE, op.val32);
    case M_REL:
    case M_REL8:
        return emitRelative(insn, op, mode);
    case M_FAR:
        if (op.val32 >= 0x10000)
            return setError(op, OVERFLOW_RANGE);
        emitImmediate(insn, op, SZ_WORD, op.val32);
        return emitImmediate(insn, op, SZ_WORD, op.seg16);
    case M_ISTR:
        insn.emitOperand8(op.val32);
        return OK;
    default:
        return OK;
    }
}

Error AsmI8086::emitStringOperand(InsnI8086 &insn, const Operand &op, RegName seg, RegName index) {
    if (op.mode == M_NONE)
        return OK;
    if (op.reg != REG_UNDEF || op.index != index || op.hasVal)
        return setError(op, ILLEGAL_OPERAND);
    if (seg == REG_ES && op.seg != REG_ES)
        return setError(op, ILLEGAL_SEGMENT);
    if (seg == REG_DS && op.seg != REG_UNDEF && op.seg != REG_DS)
        insn.setSegment(TableI8086.segOverridePrefix(op.seg));
    return OK;
}

Error AsmI8086::encodeStringInst(InsnI8086 &insn, const Operand &dst, const Operand &src) {
    switch (insn.opCode() & ~1) {
    case 0xA4:  // MOVS ES:[DI],DS:[SI]
        if (emitStringOperand(insn, dst, REG_ES, REG_DI))
            return getError();
        if (emitStringOperand(insn, src, REG_DS, REG_SI))
            return getError();
        /* Fall-through */
    case 0xAA:  // STOS ES:[DI]
    case 0xAE:  // SCAS ES:[DI]
        if (emitStringOperand(insn, dst, REG_ES, REG_DI))
            return getError();
        break;
    case 0xA6:  // CMPS DS:[SI],ES:[DI]
        if (emitStringOperand(insn, src, REG_ES, REG_DI))
            return getError();
        /* Fall-through */
    case 0xAC:  // LODS DS:[SI]
        if (emitStringOperand(insn, dst, REG_DS, REG_SI))
            return getError();
        break;
    }
    insn.emitInsn();
    return setOK();
}

Error AsmI8086::encode(StrScanner &scan, Insn &_insn) {
    InsnI8086 insn(_insn);
    insn.setName(_parser.readSymbol(scan));

    Operand dstOp, srcOp;
    if (parseOperand(scan, dstOp))
        return getError();
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, srcOp))
            return getError();
        scan.skipSpaces();
    }
    if (!endOfLine(*scan))
        return setError(scan, GARBAGE_AT_END);
    setErrorIf(dstOp);
    setErrorIf(srcOp);

    insn.setAddrMode(dstOp.mode, srcOp.mode);
    if (TableI8086.searchName(insn))
        return setError(TableI8086.getError());
    insn.prepairModReg();

    if (insn.stringInst())
        return encodeStringInst(insn, dstOp, srcOp);
    const AddrMode dst = insn.dstMode();
    if (dst != M_NONE) {
        if (emitOperand(insn, dst, dstOp, insn.dstPos()))
            return getError();
    }
    const AddrMode src = insn.srcMode();
    if (src != M_NONE) {
        if (emitOperand(insn, src, srcOp, insn.srcPos()))
            return getError();
    }
    insn.emitInsn();
    return getError();
}

}  // namespace i8086
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
