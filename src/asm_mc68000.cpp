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

static const char OPT_BOOL_ALIAS[] PROGMEM = "alias";
static const char OPT_DESC_ALIAS[] PROGMEM = "accept An as destination operand";

AsmMc68000::OptAlias::OptAlias(AsmMc68000 &assembler)
    : BoolOptionBase(OPT_BOOL_ALIAS, OPT_DESC_ALIAS), _assembler(assembler) {}

Error AsmMc68000::OptAlias::set(bool value) const {
    _assembler.setAlias(value);
    return OK;
}

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

void AsmMc68000::emitBriefExtension(InsnMc68000 &insn, const Operand &op, Config::ptrdiff_t disp) {
    if (overflowInt8(disp))
        setErrorIf(op, OVERFLOW_RANGE);
    uint16_t ext = static_cast<uint8_t>(disp);
    ext |= RegMc68000::encodeGeneralRegNo(op.indexReg) << 12;
    if (RegMc68000::isAddrReg(op.indexReg))
        ext |= (1 << 15);
    if (op.indexSize == SZ_LONG)
        ext |= (1 << 11);
    insn.emitOperand16(ext);
}

void AsmMc68000::emitDisplacement(InsnMc68000 &insn, const Operand &op, Config::ptrdiff_t disp) {
    if (overflowInt16(disp))
        setErrorIf(op, OVERFLOW_RANGE);
    insn.emitOperand16(static_cast<uint16_t>(disp));
}

void AsmMc68000::emitRelativeAddr(InsnMc68000 &insn, AddrMode mode, const Operand &op) {
    const auto base = insn.address() + 2;
    const auto target = op.getError() ? base : op.val32;
    const auto disp = branchDelta(base, target, op);
    if (mode == M_REL8) {
        if (!overflowInt8(disp)) {
            insn.embed(static_cast<uint8_t>(disp));
            return;
        }
    }
    if (overflowInt16(disp))
        setErrorIf(op, OPERAND_TOO_FAR);
    insn.emitOperand16(static_cast<uint16_t>(disp));
}

void AsmMc68000::emitImmediateData(
        InsnMc68000 &insn, const Operand &op, OprSize size, uint32_t data) {
    switch (size) {
    case SZ_LONG:
        insn.emitOperand32(data);
        return;
    case SZ_WORD:
        if (overflowUint16(data))
            setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(data);
        return;
    case SZ_BYTE:
        if (overflowUint8(data))
            setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(static_cast<uint8_t>(data));
        return;
    default:
        return;
    }
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
    if (mode == M_NONE) {
        if (op.mode != M_NONE)
            setErrorIf(op, UNKNOWN_OPERAND);
        return getError();
    }

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
            return setErrorIf(op, OPERAND_NOT_ALLOWED);
        break;
    case M_INDX:
        emitBriefExtension(insn, op, static_cast<Config::ptrdiff_t>(op.val32));
        break;
    case M_PCIDX:
        emitBriefExtension(insn, op, op.offset(insn));
        break;
    case M_DISP:
        emitDisplacement(insn, op, static_cast<Config::ptrdiff_t>(op.val32));
        break;
    case M_PCDSP:
        if (checkAlignment(size, op))
            setErrorIf(op, getError());
        emitDisplacement(insn, op, op.offset(insn));
        break;
    case M_AWORD:
    case M_ALONG:
        if (checkAlignment(size, op))
            setErrorIf(op, getError());
        if (op.mode == M_AWORD) {
            insn.emitOperand16(op.val32);
        } else {
            insn.emitOperand32(op.val32);
        }
        break;
    case M_IMDAT:
        if (mode == M_IMBIT)
            break;
        if (mode == M_IM3) {
            // "Zero means 2^3" unsigned 3-bit.
            if (op.val32 > 8)
                setErrorIf(op, OVERFLOW_RANGE);
            if (op.val32 == 0 && op.getError() == OK)
                return setErrorIf(op, OPERAND_NOT_ALLOWED);
            const Config::opcode_t count = (op.val32 & 7);  // 8 is encoded to 0.
            const Config::opcode_t data = op.getError() ? 0 : (count << 9);
            insn.embed(data);
            break;
        }
        if (mode == M_IM8) {
            // Signed 8-bit.
            if (overflowInt8(static_cast<int32_t>(op.val32)))
                setErrorIf(op, OVERFLOW_RANGE);
            insn.embed(static_cast<uint8_t>(op.val32));
            break;
        }
        if (mode == M_IMVEC) {
            if (op.val32 >= 16)
                setErrorIf(op, OVERFLOW_RANGE);
            insn.embed(static_cast<uint8_t>(op.val32 & 0xF));
            break;
        }
        if (mode == M_IMDSP) {
            if (overflowInt16(static_cast<int32_t>(op.val32)))
                setErrorIf(op, OVERFLOW_RANGE);
            insn.emitOperand16(static_cast<uint16_t>(op.val32));
            break;
        }
        emitImmediateData(insn, op, size, op.val32);
        break;
    case M_LABEL:
        if (size == SZ_LONG || (size == SZ_BYTE && mode == M_REL16))
            setErrorIf(op, ILLEGAL_SIZE);
        if (size == SZ_WORD && mode == M_REL8)
            mode = M_REL16;
        emitRelativeAddr(insn, mode, op);
        break;
    default:
        break;
    }
    return getError();
}

static uint16_t reverseBits(uint16_t bits) {
    bits = (bits & 0x5555) << 1 | (bits & 0xAAAA) >> 1;
    bits = (bits & 0x3333) << 2 | (bits & 0xCCCC) >> 2;
    bits = (bits & 0x0F0F) << 4 | (bits & 0xF0F0) >> 4;
    return bits << 8 | bits >> 8;
}

void AsmMc68000::emitRegisterList(InsnMc68000 &insn, const Operand &op, bool reverse) {
    auto p = op.list;
    uint16_t bits = 0;
    for (;;) {
        auto a = p;
        const auto start = RegMc68000::parseRegName(a);
        if (!RegMc68000::isGeneralReg(start))
            setErrorIf(p, REGISTER_NOT_ALLOWED);
        const uint8_t s = RegMc68000::encodeGeneralRegPos(start);
        uint8_t e = s;
        if (*a == '-') {
            ++a;
            p = a.skipSpaces();
            const auto last = RegMc68000::parseRegName(a);
            if (!RegMc68000::isGeneralReg(last))
                setErrorIf(p, REGISTER_NOT_ALLOWED);
            e = RegMc68000::encodeGeneralRegPos(last);
            if (e < s)
                setErrorIf(UNKNOWN_OPERAND);
        }
        for (uint8_t i = s; i <= e; i++) {
            const auto bm = shiftLeftOne(i);
            if (bits & bm)
                setErrorIf(p, DUPLICATE_REGISTER);
            bits |= bm;
        }
        if (a.skipSpaces().expect('/')) {
            p = a;
            continue;
        } else if (*a == ',' || endOfLine(a)) {
            break;
        }
        setErrorIf(a, UNKNOWN_OPERAND);
    }
    if (reverse)
        bits = reverseBits(bits);
    insn.emitOperand16(bits);
}

Error AsmMc68000::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(op.list = p);
    if (endOfLine(p))
        return OK;
    if (p.expect('#')) {
        op.val32 = parseExpr32(p, op);
        if (op.hasError())
            return op.getError();
        op.mode = M_IMDAT;
        scan = p;
        return OK;
    }
    auto a = p;
    const bool pdec = (*a++ == '-' && *a == '(');
    if (pdec)
        p = a;
    if (p.expect('(')) {
        op.reg = RegMc68000::parseRegName(p.skipSpaces());
        if (RegMc68000::isAddrReg(op.reg)) {
            if (!p.skipSpaces().expect(')'))
                return op.setError(MISSING_CLOSING_PAREN);
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
        op.val32 = parseExpr32(p, op, ')');
        if (op.hasError())
            return op.getError();
        if (p.skipSpaces().expect(')')) {
            const auto size = RegMc68000::parseSize(p.skipSpaces());
            bool over16 = overflowInt16(op.val32);
            if (over16) {
                // check if it is near the end of address space.
                const auto max = shiftLeftOne(uint8_t(addressWidth()));
                if (op.val32 <= max - 1 && op.val32 >= (max - 0x8000))
                    over16 = false;
            }
            if (over16 && size == SZ_WORD)
                op.setErrorIf(OVERFLOW_RANGE);
            op.mode = (size == SZ_WORD || (size == SZ_NONE && !over16)) ? M_AWORD : M_ALONG;
            scan = p;
            return OK;
        }
        if (p.expect(',')) {
            a = p.skipSpaces();
            op.reg = RegMc68000::parseRegName(a);
            if (!RegMc68000::isAddrReg(op.reg) && op.reg != REG_PC)
                return op.setError(a, REGISTER_NOT_ALLOWED);
            if (a.skipSpaces().expect(')')) {
                op.mode = (op.reg == REG_PC) ? M_PCDSP : M_DISP;
                scan = a;
                return OK;
            }
            if (!a.expect(','))
                return op.setError(a, MISSING_COMMA);
            p = a.skipSpaces();
            op.indexReg = RegMc68000::parseRegName(p);
            if (!RegMc68000::isGeneralReg(op.indexReg))
                return op.setError(UNKNOWN_OPERAND);
            op.indexSize = RegMc68000::parseSize(p);
            if (op.indexSize == SZ_ERROR)
                return op.setError(UNKNOWN_OPERAND);
            if (op.indexSize == SZ_NONE)
                op.indexSize = SZ_WORD;
            if (!p.skipSpaces().expect(')'))
                return op.setError(p, MISSING_CLOSING_PAREN);
            op.mode = (op.reg == REG_PC) ? M_PCIDX : M_INDX;
            scan = p;
            return OK;
        }
        return op.setError(UNKNOWN_OPERAND);
    }

    op.reg = RegMc68000::parseRegName(a = p);
    if (op.reg != REG_UNDEF) {
        a.skipSpaces();
        if ((*a == '/' || *a == '-') && RegMc68000::isGeneralReg(op.reg)) {
            while (*a != ',' && !endOfLine(a))
                ++a;
            op.mode = M_MULT;
            scan = a;
            return op.getError();
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
            return op.setError(p, REGISTER_NOT_ALLOWED);
        scan = a;
        return OK;
    }
    op.val32 = parseExpr32(p, op);
    if (op.hasError())
        return op.getError();
    op.mode = M_LABEL;
    scan = p;
    return OK;
}

OprSize InsnMc68000::parseInsnSize() {
    StrScanner p(name());
    p.trimStart([](char c) { return c != '.'; });
    char *eos = const_cast<char *>(p.str());
    const auto isize = RegMc68000::parseSize(p);
    *eos = 0;
    return isize;
}

Error AsmMc68000::encodeImpl(StrScanner &scan, Insn &_insn) {
    InsnMc68000 insn(_insn);
    const auto isize = insn.parseInsnSize();
    if (isize == SZ_ERROR)
        return setError(scan, ILLEGAL_SIZE);
    insn.setInsnSize(isize);

    Operand srcOp, dstOp;
    if (parseOperand(scan, srcOp) && srcOp.hasError())
        return setError(srcOp);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, dstOp) && dstOp.hasError())
            return setError(dstOp);
        scan.skipSpaces();
    }
    if (!endOfLine(scan))
        return setError(scan, GARBAGE_AT_END);
    setErrorIf(srcOp);
    setErrorIf(dstOp);

    insn.setAddrMode(srcOp.mode, dstOp.mode);
    const auto error = TableMc68000::TABLE.searchName(insn);
    if (error)
        return setError(srcOp, error);

    const auto src = insn.src();
    const auto dst = insn.dst();
    if (src == M_MULT)
        emitRegisterList(insn, srcOp, dstOp.mode == M_PDEC);
    if (dst == M_MULT)
        emitRegisterList(insn, dstOp);
    if (src == M_IMBIT) {
        auto bitno = srcOp.val32;
        if (srcOp.mode != M_IMDAT) {
            setErrorIf(srcOp, OPERAND_NOT_ALLOWED);
            bitno = 0;
        }
        if (insn.oprSize() == SZ_BYTE && bitno >= 8) {
            setErrorIf(srcOp, ILLEGAL_BIT_NUMBER);
            bitno &= 7;
        }
        if (insn.oprSize() == SZ_LONG && bitno >= 32) {
            setErrorIf(srcOp, ILLEGAL_BIT_NUMBER);
            bitno &= 0x1F;
        }
        insn.emitOperand16(bitno);
    }
    emitOprSize(insn, isize);
    insn.setInsnSize(isize);
    const auto osize = (isize == SZ_NONE) ? insn.oprSize() : isize;
    if (emitEffectiveAddr(insn, osize, srcOp, src, insn.srcPos()) == OPERAND_NOT_ALLOWED)
        return getError();
    if (emitEffectiveAddr(insn, osize, dstOp, dst, insn.dstPos()) == OPERAND_NOT_ALLOWED)
        return getError();
    insn.emitInsn();
    return setErrorIf(insn);
}

}  // namespace mc68000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
