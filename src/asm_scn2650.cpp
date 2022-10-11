/*
 * Copyright 2022 Tadashi G. Takaoka
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

#include "asm_scn2650.h"

#include <ctype.h>

namespace libasm {
namespace scn2650 {

static constexpr Config::uintptr_t page(const Config::uintptr_t addr) {
    return addr & ~0x1FFF;  // 8k bytes per page
}

static constexpr Config::uintptr_t offset(const Config::uintptr_t addr) {
    return addr & 0x1FFF;
}

static constexpr Config::ptrdiff_t signedOffset(const Config::uintptr_t addr) {
    return (addr & (0x1000 - 1)) - (addr & 0x1000);
}

static constexpr Config::uintptr_t inpage(
        const Config::uintptr_t addr, const Config::ptrdiff_t delta) {
    return page(addr) | offset(addr + delta);
}

static constexpr Config::uintptr_t inspace(const Config::uintptr_t addr) {
    return addr < 0x8000;
}

static bool overflowInt13(Config::ptrdiff_t delta) {
    return delta < -0x40 || delta >= 0x40;
}

Error AsmScn2650::parseOperand(StrScanner &scan, Operand &op) const {
    // Do not skip preceding spaces.
    op.setAt(scan);
    if (endOfLine(*scan))
        return OK;

    StrScanner p(scan);
    op.reg = RegScn2650::parseRegName(p);
    if (op.reg != REG_UNDEF) {
        op.mode = op.reg == REG_R0 ? REG0 : R123;
        scan = p;
        return OK;
    }
    op.cc = RegScn2650::parseCcName(p);
    if (op.cc != CC_UNDEF) {
        op.mode = op.cc == CC_UN ? CCVN : C012;
        scan = p;
        return OK;
    }

    op.indir = p.expect('*');
    op.val16 = parseExpr16(p.skipSpaces(), op);
    if (parserError())
        return op.getError();
    if (p.expect(',')) {
        op.reg = RegScn2650::parseRegName(p.skipSpaces());
        if (op.reg == REG_UNDEF)
            return op.setError(UNKNOWN_OPERAND);
        if (p.skipSpaces().expect(',')) {
            op.sign = p.skipSpaces().expect([](char c) { return c == '+' || c == '-'; });
            if (op.sign) {
                op.mode = IX13;
                scan = p;
                return OK;
            }
            return op.setError(UNKNOWN_OPERAND);
        }
        op.mode = op.reg == REG_R3 ? IX15 : IX13;
        scan = p;
        return OK;
    }
    op.mode = op.indir ? AB15 : IMM8;
    scan = p;
    return OK;
}

Error AsmScn2650::emitAbsolute(InsnScn2650 &insn, const Operand &op, AddrMode mode) {
    const auto target = op.getError() ? insn.address() : op.val16;
    if (!inspace(target))
        return setError(op, OVERFLOW_RANGE);
    uint16_t opr = target;
    if (op.indir)
        opr |= 0x8000;
    if (mode == IX15) {
        if (op.reg != REG_R3 && op.reg != REG_UNDEF)
            return setError(op, REGISTER_NOT_ALLOWED);
        insn.embed(RegScn2650::encodeRegName(REG_R3));
    }
    insn.emitOperand16(opr);
    return OK;
}

Error AsmScn2650::emitIndexed(InsnScn2650 &insn, const Operand &op, AddrMode mode) {
    const auto target = op.getError() ? insn.address() : op.val16;
    if (!inspace(target))
        return setError(op, OVERFLOW_RANGE);

    if (page(target) != page(insn.address()))
        return setError(op, OVERWRAP_PAGE);
    uint16_t opr = offset(target);
    if (op.indir)
        opr |= 0x8000;
    if (mode == IX13) {
        insn.embed(RegScn2650::encodeRegName(op.reg));
        switch (op.sign) {
        case '+':
            opr |= 0x2000;
            break;
        case '-':
            opr |= 0x4000;
            break;
        default:
            opr |= 0x6000;
            break;
        }
    }
    insn.emitOperand16(opr);
    return OK;
}

Error AsmScn2650::emitZeroPage(InsnScn2650 &insn, const Operand &op) {
    const auto target = op.val16;
    if (page(target) != 0)
        return setError(op, OVERFLOW_RANGE);
    // Sign extends 13-bit number
    const auto offset = signedOffset(target);
    if (overflowInt13(offset))
        return setError(op, OPERAND_TOO_FAR);
    uint8_t opr = op.val16 & 0x7F;
    if (op.indir)
        opr |= 0x80;
    insn.emitOperand8(opr);
    return OK;
}

Error AsmScn2650::emitRelative(InsnScn2650 &insn, const Operand &op) {
    const auto base = inpage(insn.address(), 2);
    const auto target = op.getError() ? base : op.val16;
    if (page(target) != page(base))
        return setError(op, OVERWRAP_PAGE);
    // Sign extends 13-bit number.
    const auto delta = signedOffset(offset(target) - offset(base));
    if (overflowInt13(delta))
        return setError(op, OPERAND_TOO_FAR);
    uint8_t opr = delta & 0x7F;
    if (op.indir)
        opr |= 0x80;
    insn.emitOperand8(opr);
    return OK;
}

Error AsmScn2650::encodeOperand(InsnScn2650 &insn, const Operand &op, AddrMode mode) {
    switch (mode) {
    case REGN:
    case REG0:
    case R123:
        insn.embed(RegScn2650::encodeRegName(op.reg));
        break;
    case CCVN:
    case C012:
        insn.embed(RegScn2650::encodeCcName(op.cc));
        break;
    case IMM8:
        insn.emitOperand8(op.val16);
        break;
    case REL7:
        return emitRelative(insn, op);
    case ABS7:
        return emitZeroPage(insn, op);
    case AB13:
    case IX13:
        return emitIndexed(insn, op, mode);
    case AB15:
    case IX15:
        return emitAbsolute(insn, op, mode);
    default:
        break;
    }
    return OK;
}

Error AsmScn2650::encodeImpl(StrScanner &scan, Insn &_insn) {
    InsnScn2650 insn(_insn);
    insn.nameBuffer().text(_parser.readSymbol(scan));

    Operand opr1, opr2;
    bool insnWithReg = false;
    if (scan.expect(',')) {
        if (parseOperand(scan, opr1) && opr1.hasError())
            return setError(opr1);
        if (opr1.mode == REGN || opr1.mode == REG0 || opr1.mode == R123)
            insnWithReg = true;
        if (!endOfLine(*scan.skipSpaces())) {
            if (parseOperand(scan.skipSpaces(), opr2) && opr2.hasError())
                return setError(opr2);
        }
    } else {
        if (parseOperand(scan.skipSpaces(), opr1) && opr1.hasError())
            return setError(opr1);
        if (scan.skipSpaces().expect(',')) {
            if (parseOperand(scan.skipSpaces(), opr2) && opr2.hasError())
                return setError(opr2);
        }
    }

    if (!endOfLine(*scan.skipSpaces()))
        return setError(GARBAGE_AT_END);
    setErrorIf(opr1);
    setErrorIf(opr2);

    insn.setAddrMode(opr1.mode, opr2.mode);
    const auto error = TableScn2650::TABLE.searchName(insn);
    if (error)
        return setError(opr1, error);

    if (insnWithReg)
        _regs.outRegName(insn.nameBuffer().letter(','), opr1.reg);
    if (encodeOperand(insn, opr1, insn.mode1()))
        return getError();
    if (encodeOperand(insn, opr2, insn.mode2())) {
        insn.reset();
        return getError();
    }

    insn.emitInsn();
    return getError();
}

}  // namespace scn2650
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
