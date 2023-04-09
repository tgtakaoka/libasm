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

#include "reg_scn2650.h"
#include "table_scn2650.h"

namespace libasm {
namespace scn2650 {

using namespace reg;

struct AsmScn2650::Operand : public OperandBase {
    AddrMode mode;
    RegName reg;
    CcName cc;
    bool indir;
    char sign;
    uint16_t val16;
    Operand() : mode(M_NONE), reg(REG_UNDEF), cc(CC_UNDEF), indir(false), sign(0), val16(0) {}
};

AsmScn2650::AsmScn2650()
    : Assembler(TableScn2650::TABLE, nullptr, _number, _comment, _symbol, _letter, _location) {
    reset();
}

static constexpr Config::uintptr_t page(const Config::uintptr_t addr) {
    return addr & ~0x1FFF;  // 8k bytes per page
}

static constexpr Config::uintptr_t offset(const Config::uintptr_t addr) {
    return addr & 0x1FFF;
}

static constexpr Config::uintptr_t inpage(
        const Config::uintptr_t addr, const Config::ptrdiff_t delta) {
    return page(addr) | offset(addr + delta);
}

static constexpr Config::uintptr_t inspace(const Config::uintptr_t addr) {
    return addr < 0x8000;
}

Error AsmScn2650::parseOperand(StrScanner &scan, Operand &op) const {
    // Do not skip preceding spaces.
    op.setAt(scan);
    if (endOfLine(scan))
        return OK;

    auto p = scan;
    op.reg = parseRegName(p);
    if (op.reg != REG_UNDEF) {
        op.mode = op.reg == REG_R0 ? M_REG0 : M_R123;
        scan = p;
        return OK;
    }
    op.cc = parseCcName(p);
    if (op.cc != CC_UNDEF) {
        op.mode = op.cc == CC_UN ? M_CCVN : M_C012;
        scan = p;
        return OK;
    }

    op.indir = p.expect('*');
    op.val16 = parseExpr16(p.skipSpaces(), op);
    if (op.hasError())
        return op.getError();
    if (p.expect(',')) {
        op.reg = parseRegName(p.skipSpaces());
        if (op.reg == REG_UNDEF)
            return op.setError(UNKNOWN_OPERAND);
        if (p.skipSpaces().expect(',')) {
            op.sign = p.skipSpaces().expect([](char c) { return c == '+' || c == '-'; });
            if (op.sign) {
                op.mode = M_IX13;
                scan = p;
                return OK;
            }
            return op.setError(UNKNOWN_OPERAND);
        }
        op.mode = op.reg == REG_R3 ? M_IX15 : M_IX13;
        scan = p;
        return OK;
    }
    op.mode = op.indir ? M_AB15 : M_IMM8;
    scan = p;
    return OK;
}

void AsmScn2650::emitAbsolute(InsnScn2650 &insn, const Operand &op, AddrMode mode) {
    const Config::uintptr_t target = op.getError() ? insn.address() : op.val16;
    if (!inspace(target))
        setErrorIf(op, OVERFLOW_RANGE);
    auto opr = target;
    if (op.indir)
        opr |= 0x8000;
    if (mode == M_IX15) {
        if (op.reg != REG_R3 && op.reg != REG_UNDEF)
            setErrorIf(op, REGISTER_NOT_ALLOWED);
        insn.embed(encodeRegName(REG_R3));
    }
    insn.emitOperand16(opr);
}

void AsmScn2650::emitIndexed(InsnScn2650 &insn, const Operand &op, AddrMode mode) {
    const Config::uintptr_t target = op.getError() ? insn.address() : op.val16;
    if (!inspace(target))
        setErrorIf(op, OVERFLOW_RANGE);

    if (page(target) != page(insn.address()))
        setErrorIf(op, OVERWRAP_PAGE);
    auto opr = offset(target);
    if (op.indir)
        opr |= 0x8000;
    if (mode == M_IX13) {
        insn.embed(encodeRegName(op.reg));
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
}

void AsmScn2650::emitZeroPage(InsnScn2650 &insn, const Operand &op) {
    const auto target = op.val16;
    if (page(target) != 0)
        setErrorIf(op, OVERFLOW_RANGE);
    // Sign extends 13-bit number
    const auto offset = signExtend(target, 13);
    if (overflowInt(offset, 7))
        setErrorIf(op, OPERAND_TOO_FAR);
    uint8_t opr = target & 0x7F;
    if (op.indir)
        opr |= 0x80;
    insn.emitOperand8(opr);
}

void AsmScn2650::emitRelative(InsnScn2650 &insn, const Operand &op) {
    const auto base = inpage(insn.address(), 2);
    const auto target = op.getError() ? base : op.val16;
    if (page(target) != page(base))
        setErrorIf(op, OVERWRAP_PAGE);
    // Sign extends 13-bit number.
    const auto delta = signExtend(offset(target) - offset(base), 13);
    if (overflowInt(delta, 7))
        setErrorIf(op, OPERAND_TOO_FAR);
    uint8_t opr = delta & 0x7F;
    if (op.indir)
        opr |= 0x80;
    insn.emitOperand8(opr);
}

void AsmScn2650::encodeOperand(InsnScn2650 &insn, const Operand &op, AddrMode mode) {
    switch (mode) {
    case M_REGN:
    case M_REG0:
    case M_R123:
        insn.embed(encodeRegName(op.reg));
        break;
    case M_CCVN:
    case M_C012:
        insn.embed(encodeCcName(op.cc));
        break;
    case M_IMM8:
        insn.emitOperand8(op.val16);
        break;
    case M_REL7:
        emitRelative(insn, op);
        break;
    case M_ABS7:
        emitZeroPage(insn, op);
        break;
    case M_AB13:
    case M_IX13:
        emitIndexed(insn, op, mode);
        break;
    case M_AB15:
    case M_IX15:
        emitAbsolute(insn, op, mode);
        break;
    default:
        break;
    }
}

Error AsmScn2650::encodeImpl(StrScanner &scan, Insn &_insn) {
    InsnScn2650 insn(_insn);
    Operand opr1, opr2;
    bool insnWithReg = false;
    if (scan.expect(',')) {
        if (parseOperand(scan, opr1) && opr1.hasError())
            return setError(opr1);
        if (opr1.mode == M_REGN || opr1.mode == M_REG0 || opr1.mode == M_R123)
            insnWithReg = true;
        if (!endOfLine(scan.skipSpaces())) {
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

    if (!endOfLine(scan.skipSpaces()))
        return setError(GARBAGE_AT_END);
    setErrorIf(opr1);
    setErrorIf(opr2);

    insn.setAddrMode(opr1.mode, opr2.mode);
    const auto error = TableScn2650::TABLE.searchName(insn);
    if (error)
        return setError(opr1, error);

    if (insnWithReg)
        outRegName(insn.nameBuffer().letter(','), opr1.reg);
    encodeOperand(insn, opr1, insn.mode1());
    encodeOperand(insn, opr2, insn.mode2());
    insn.emitInsn();
    return setErrorIf(insn);
}

}  // namespace scn2650
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
