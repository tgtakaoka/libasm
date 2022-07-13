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

namespace libasm {
namespace mc68000 {

const char AsmMc68000::OPT_BOOL_ALIAS[] PROGMEM = "alias";

static int8_t modePos(OprPos pos) {
    switch (pos) {
    case OP_10:
        return 3;
    case OP_23:
        return 6;
    default:
        return -1;
    }
}

static int8_t regPos(OprPos pos) {
    switch (pos) {
    case OP_10:
    case OP__0:
        return 0;
    case OP_23:
    case OP__3:
        return 9;
    default:
        return -1;
    }
}

void emitOprSize(InsnMc68000 &insn, OprSize size) {
    Config::opcode_t sz = 0;
    switch (insn.oprSize()) {
    case SZ_DATA:
        if (size == SZ_NONE)
            size = SZ_WORD;
        sz = static_cast<uint8_t>(size) << 6;
        break;
    case SZ_ADR6:
        if (size == SZ_NONE)
            size = SZ_WORD;
        sz = (size == SZ_LONG) ? (1 << 6) : 0;
        break;
    case SZ_ADR8:
        if (size == SZ_NONE)
            size = SZ_WORD;
        sz = (size == SZ_LONG) ? (1 << 8) : 0;
        break;
    default:
        return;
    }
    insn.embed(sz);
}

Error AsmMc68000::checkAlignment(OprSize size, const Operand &op) {
    if (size == SZ_WORD && (op.val32 % 2))
        return setError(op, OPERAND_NOT_ALIGNED);
    if (size == SZ_LONG && (op.val32 % 4))
        return setError(op, OPERAND_NOT_ALIGNED);
    return OK;
}

Error AsmMc68000::emitBriefExtension(
        InsnMc68000 &insn, RegName index, OprSize size, Config::ptrdiff_t disp) {
    if (overflowRel8(disp))
        return setError(OVERFLOW_RANGE);
    uint16_t ext = static_cast<uint8_t>(disp);
    ext |= RegMc68000::encodeGeneralRegNo(index) << 12;
    if (RegMc68000::isAddrReg(index))
        ext |= (1 << 15);
    if (size == SZ_LONG)
        ext |= (1 << 11);
    insn.emitOperand16(ext);
    return OK;
}

Error AsmMc68000::emitDisplacement(InsnMc68000 &insn, Config::ptrdiff_t disp) {
    if (overflowRel16(disp))
        return setError(OVERFLOW_RANGE);
    insn.emitOperand16(static_cast<uint16_t>(disp));
    return OK;
}

Error AsmMc68000::emitRelativeAddr(InsnMc68000 &insn, AddrMode mode, const Operand &op) {
    const Config::uintptr_t base = insn.address() + 2;
    const Config::uintptr_t target = op.getError() ? base : op.val32;
    if (target % 2)
        return setError(op, OPERAND_NOT_ALIGNED);
    const Config::ptrdiff_t disp = target - base;
    if (mode == M_REL8) {
        if (!overflowRel8(disp)) {
            insn.embed(static_cast<uint8_t>(disp));
            return OK;
        }
    }
    if (overflowRel16(disp))
        return setError(op, OPERAND_TOO_FAR);
    insn.emitOperand16(static_cast<uint16_t>(disp));
    return OK;
}

Error AsmMc68000::emitImmediateData(InsnMc68000 &insn, OprSize size, uint32_t data) {
    if (size == SZ_LONG) {
        insn.emitOperand32(data);
        return OK;
    }
    if (size == SZ_WORD) {
        if (!overflowUint16(data)) {
            insn.emitOperand16(static_cast<uint16_t>(data));
            return OK;
        }
    }
    if (size == SZ_BYTE) {
        if (!overflowUint8(data)) {
            insn.emitOperand16(static_cast<uint8_t>(data));
            return OK;
        }
    }
    return setError(OVERFLOW_RANGE);
}

Config::uintptr_t AsmMc68000::Operand::offset(const InsnMc68000 &insn) const {
    if (getError())
        return 0;
    uint8_t len = insn.length();
    if (len == 0)
        len = sizeof(Config::opcode_t);
    return val32 - (insn.address() + len);
}

Error AsmMc68000::emitEffectiveAddr(
        InsnMc68000 &insn, OprSize size, const Operand &op, AddrMode mode, OprPos pos) {
    if (mode == M_NONE)
        return op.mode == M_NONE ? OK : UNKNOWN_OPERAND;

    const int8_t mode_gp = modePos(pos);
    if (mode_gp >= 0) {
        const Config::opcode_t m = EaMc68000::encodeMode(op.mode);
        insn.embed(m << mode_gp);
    }

    const int8_t reg_gp = regPos(pos);
    if (reg_gp >= 0) {
        const Config::opcode_t r = EaMc68000::encodeRegNo(op.mode, op.reg);
        insn.embed(r << reg_gp);
    }

    switch (op.mode) {
    case M_AREG:
        if (size == SZ_BYTE)
            return setError(OPERAND_NOT_ALLOWED);
        break;
    case M_INDX:
        return emitBriefExtension(
                insn, op.indexReg, op.indexSize, static_cast<Config::ptrdiff_t>(op.val32));
    case M_PCIDX:
        return emitBriefExtension(insn, op.indexReg, op.indexSize, op.offset(insn));
    case M_DISP:
        return emitDisplacement(insn, static_cast<Config::ptrdiff_t>(op.val32));
    case M_PCDSP:
        if (checkAlignment(size, op))
            return getError();
        return emitDisplacement(insn, op.offset(insn));
    case M_AWORD:
    case M_ALONG:
        if (checkAlignment(size, op))
            return getError();
        if (op.mode == M_AWORD) {
            insn.emitOperand16(op.val32);
        } else {
            insn.emitOperand32(op.val32);
        }
        return OK;
    case M_IMDAT:
        if (mode == M_IMBIT)
            return OK;
        if (mode == M_IM3) {
            // "Zero means 2^3" unsigned 3-bit.
            if (op.val32 > 8)
                return setError(op, OVERFLOW_RANGE);
            if (op.val32 == 0 && op.getError() == OK)
                return setError(op, OPERAND_NOT_ALLOWED);
            const Config::opcode_t count = (op.val32 & 7);  // 8 is encoded to 0.
            const Config::opcode_t data = op.getError() ? 0 : (count << 9);
            insn.embed(data);
            return OK;
        }
        if (mode == M_IM8) {
            // Signed 8-bit.
            if (overflowRel8(static_cast<int32_t>(op.val32)))
                return setError(op, OVERFLOW_RANGE);
            insn.embed(static_cast<uint8_t>(op.val32));
            return OK;
        }
        if (mode == M_IMVEC) {
            if (op.val32 >= 16)
                return setError(op, OVERFLOW_RANGE);
            insn.embed(static_cast<uint8_t>(op.val32));
            return OK;
        }
        if (mode == M_IMDSP) {
            if (overflowRel16(static_cast<int32_t>(op.val32)))
                return setError(op, OVERFLOW_RANGE);
            insn.emitOperand16(static_cast<uint16_t>(op.val32));
            return OK;
        }
        return emitImmediateData(insn, size, op.val32);
    case M_LABEL:
        if (size == SZ_LONG || (size == SZ_BYTE && mode == M_REL16))
            return setError(op, ILLEGAL_SIZE);
        if (size == SZ_WORD && mode == M_REL8)
            mode = M_REL16;
        return emitRelativeAddr(insn, mode, op);
    default:
        break;
    }
    return OK;
}

static uint16_t reverseBits(uint16_t bits) {
    bits = (bits & 0x5555) << 1 | (bits & 0xAAAA) >> 1;
    bits = (bits & 0x3333) << 2 | (bits & 0xCCCC) >> 2;
    bits = (bits & 0x0F0F) << 4 | (bits & 0xF0F0) >> 4;
    return bits << 8 | bits >> 8;
}

void AsmMc68000::Operand::fixupMultiRegister() {
    if (mode == M_DREG || mode == M_AREG) {
        val32 = (1 << RegMc68000::encodeGeneralRegPos(reg));
        mode = M_MULT;
    }
}

Error AsmMc68000::parseMoveMultiRegList(StrScanner &scan, Operand &op) {
    StrScanner p(scan);
    Error error = OK;
    for (;;) {
        StrScanner a(p);
        const RegName start = RegMc68000::parseRegName(a);
        if (!RegMc68000::isGeneralReg(start))
            return setError(p, REGISTER_NOT_ALLOWED);
        const uint8_t s = RegMc68000::encodeGeneralRegPos(start);
        uint8_t e = s;
        if (*a == '-') {
            ++a;
            p = a.skipSpaces();
            const RegName last = RegMc68000::parseRegName(a);
            if (!RegMc68000::isGeneralReg(last))
                return op.setError(p, REGISTER_NOT_ALLOWED);
            e = RegMc68000::encodeGeneralRegPos(last);
            if (e < s)
                return setError(a, UNKNOWN_OPERAND);
        }
        for (uint8_t i = s; i <= e; i++) {
            const uint32_t bm = (1 << i);
            if (op.val32 & bm)
                error = DUPLICATE_REGISTER;
            op.val32 |= bm;
        }
        if (!a.skipSpaces().expect('/')) {
            op.mode = M_MULT;
            scan = a;
            return setError(error);
        }
        p = a.skipSpaces();
    }
}

Error AsmMc68000::parseOperand(StrScanner &scan, Operand &op) {
    StrScanner p(scan.skipSpaces());
    op.setAt(p);
    if (endOfLine(*p))
        return OK;
    if (p.expect('#')) {
        op.val32 = parseExpr32(p, op);
        if (parserError())
            return getError();
        op.mode = M_IMDAT;
        scan = p;
        return OK;
    }
    StrScanner a(p);
    const bool pdec = (*a++ == '-' && *a == '(');
    if (pdec)
        p = a;
    if (p.expect('(')) {
        op.reg = RegMc68000::parseRegName(p.skipSpaces());
        if (RegMc68000::isAddrReg(op.reg)) {
            if (!p.skipSpaces().expect(')'))
                return setError(p, MISSING_CLOSING_PAREN);
            if (pdec) {
                op.mode = M_PDEC;
            } else if (p.expect('+')) {
                op.mode = M_PINC;
            } else {
                op.mode = M_AIND;
            }
            scan = p;
            return OK;
        }
        op.val32 = parseExpr32(p, op);
        if (parserError())
            return getError();
        if (p.skipSpaces().expect(')')) {
            const OprSize size = RegMc68000::parseSize(p.skipSpaces());
            const Config::uintptr_t max = 1UL << uint8_t(addressWidth());
            if (static_cast<int32_t>(op.val32) >= 0 && op.val32 >= max)
                return setError(op, OVERFLOW_RANGE);
            // Sign extends 24-bit number.
            const Config::uintptr_t sign = max >> 1;
            const Config::uintptr_t addr = (op.val32 & (sign - 1)) - (op.val32 & sign);
            const bool over16 = overflowRel16(addr);
            if (size == SZ_WORD && over16)
                return setError(op, OVERFLOW_RANGE);
            op.mode = (size == SZ_WORD || (size == SZ_NONE && !over16)) ? M_AWORD : M_ALONG;
            scan = p;
            return OK;
        }
        if (p.expect(',')) {
            a = p.skipSpaces();
            op.reg = RegMc68000::parseRegName(a);
            if (!RegMc68000::isAddrReg(op.reg) && op.reg != REG_PC)
                return setError(p, REGISTER_NOT_ALLOWED);
            if (a.skipSpaces().expect(')')) {
                op.mode = (op.reg == REG_PC) ? M_PCDSP : M_DISP;
                scan = a;
                return OK;
            }
            if (!a.expect(','))
                return setError(a, MISSING_COMMA);
            p = a.skipSpaces();
            op.indexReg = RegMc68000::parseRegName(p);
            if (!RegMc68000::isGeneralReg(op.indexReg))
                return setError(a, UNKNOWN_OPERAND);
            op.indexSize = RegMc68000::parseSize(p);
            if (op.indexSize == SZ_ERROR)
                return setError(p, UNKNOWN_OPERAND);
            if (op.indexSize == SZ_NONE)
                op.indexSize = SZ_WORD;
            if (!p.skipSpaces().expect(')'))
                return setError(p, MISSING_CLOSING_PAREN);
            op.mode = (op.reg == REG_PC) ? M_PCIDX : M_INDX;
            scan = p;
            return OK;
        }
        return setError(p, UNKNOWN_OPERAND);
    }

    op.reg = RegMc68000::parseRegName(a = p);
    if (op.reg != REG_UNDEF) {
        a.skipSpaces();
        if ((*a == '/' || *a == '-') && RegMc68000::isGeneralReg(op.reg)) {
            parseMoveMultiRegList(p, op);
            scan = p;
            return getError();
        }            
        if (RegMc68000::isAddrReg(op.reg)) {
            op.mode = M_AREG;
        } else if (RegMc68000::isDataReg(op.reg)) {
            op.mode = M_DREG;
        } else if (op.reg == REG_USP) {
            op.mode = M_USP;
        } else if (op.reg == REG_CCR) {
            op.mode = M_CCR;
        } else if (op.reg == REG_SR) {
            op.mode = M_SR;
        } else
            return setError(p, REGISTER_NOT_ALLOWED);
        scan = a;
        return OK;
    }
    op.val32 = parseExpr32(p, op);
    if (parserError())
        return getError();
    op.mode = M_LABEL;
    scan = p;
    return OK;
}

Error AsmMc68000::encode(StrScanner &scan, Insn &_insn) {
    InsnMc68000 insn(_insn);
    insn.setName(_parser.readSymbol(scan));

    const OprSize isize = RegMc68000::parseSize(scan);
    if (isize == SZ_ERROR)
        return setError(scan, ILLEGAL_SIZE);
    insn.setInsnSize(isize);

    Operand srcOp, dstOp;
    if (parseOperand(scan, srcOp))
        return getError();
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, dstOp))
            return getError();
        scan.skipSpaces();
    }
    if (!endOfLine(*scan))
        return setError(scan, GARBAGE_AT_END);
    setErrorIf(srcOp);
    setErrorIf(dstOp);

    insn.setAddrMode(srcOp.mode, dstOp.mode);
    if (TableMc68000.searchName(insn))
        return setError(TableMc68000.getError());

    const AddrMode src = insn.srcMode();
    const AddrMode dst = insn.dstMode();
    if (src == M_MULT)
        srcOp.fixupMultiRegister();
    if (dst == M_MULT)
        dstOp.fixupMultiRegister();
    if (src == M_MULT && dstOp.mode == M_PDEC)
        srcOp.val32 = reverseBits(srcOp.val32);
    if (src == M_MULT)
        insn.emitOperand16(static_cast<uint16_t>(srcOp.val32));
    if (dst == M_MULT)
        insn.emitOperand16(static_cast<uint16_t>(dstOp.val32));
    if (src == M_IMBIT) {
        if (srcOp.mode != M_IMDAT)
            return setError(srcOp, OPERAND_NOT_ALLOWED);
        if (insn.oprSize() == SZ_BYTE && srcOp.val32 >= 8)
            return setError(srcOp, ILLEGAL_BIT_NUMBER);
        if (insn.oprSize() == SZ_LONG && srcOp.val32 >= 32)
            return setError(srcOp, ILLEGAL_BIT_NUMBER);
        insn.emitOperand16(static_cast<uint16_t>(srcOp.val32));
    }
    emitOprSize(insn, isize);
    insn.setInsnSize(isize);
    const OprSize osize = (isize == SZ_NONE) ? insn.oprSize() : isize;
    if (emitEffectiveAddr(insn, osize, srcOp, src, insn.srcPos()))
        return getError();
    if (emitEffectiveAddr(insn, osize, dstOp, dst, insn.dstPos()))
        return getError();
    insn.emitInsn();
    return getError();
}

}  // namespace mc68000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
