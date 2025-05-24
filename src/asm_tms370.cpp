/*
 * Copyright 2025 Tadashi G. Takaoka
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

#include "asm_tms370.h"
#include "reg_tms370.h"
#include "table_tms370.h"
#include "text_common.h"

namespace libasm {
namespace tms370 {

using namespace pseudo;
using namespace reg;
using namespace text::common;

namespace {

constexpr char TEXT_dBLOCK[] PROGMEM = ".block";

// clang-format off
constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_dBLOCK,  &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
    {TEXT_dBYTE,   &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_dORG,    &Assembler::defineOrigin},
    {TEXT_dSTRING, &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_dWORD,   &Assembler::defineDataConstant, Assembler::DATA_WORD},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

}  // namespace

const ValueParser::Plugins &AsmTms370::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return IntelNumberParser::singleton(); }
        const SymbolParser &symbol() const override { return Tms320SymbolParser::singleton(); }
        const LetterParser &letter() const override { return Tms320LetterParser::singleton(); }
    } PLUGINS{};
    return PLUGINS;
}

AsmTms370::AsmTms370(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE), Config(TABLE) {
    reset();
}

Error AsmTms370::parseIndex(StrScanner &scan, Operand &op, bool immediate) const {
    auto p = scan;
    if (p.skipSpaces().expect('(')) {
        const auto at = p.skipSpaces();
        const auto reg = parseRegName(p);
        if (reg == REG_UNDEF)
            return op.setErrorIf(at, UNKNOWN_OPERAND);
        if (reg == REG_B) {
            op.mode = immediate ? M_BIMM : M_BIDX;
        } else if (isRegName(reg)) {
            op.mode = immediate ? M_RIMM : M_RIDX;
        } else if (!immediate && reg == REG_SP) {
            op.mode = M_SIDX;
        } else {
            return op.setErrorIf(at, OPERAND_NOT_ALLOWED);
        }
        if (p.skipSpaces().expect(')')) {
            op.reg = reg;
            scan = p;
            return OK;
        }
        return op.setError(p, MISSING_CLOSING_PAREN);
    }
    return NOT_AN_EXPECTED;
}

namespace {
AddrMode toAddrMode(RegName name, Value &num) {
    switch (name) {
    case REG_A:
        num.setUnsigned(0);
        return M_A;
    case REG_B:
        num.setUnsigned(1);
        return M_B;
    case REG_ST:
        return M_ST;
    default:
        if (isRegName(name)) {
            num.setUnsigned(toRegNum(name));
            return M_RN;
        }
        if (isPortName(name)) {
            num.setUnsigned(toPortNum(name));
            return M_PN;
        }
    }
    return M_NONE;
}
}  // namespace

Error AsmTms370::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    if (p.expect('#')) {
        op.val = parseInteger(p, op);
        if (op.hasError())
            return op.getError();
        if (parseIndex(p, op, true) == OK) {
            scan = p;
            return OK;
        }
        if (op.hasError())
            return op.getError();
        op.mode = M_IM16;
        scan = p;
        return OK;
    }

    auto indir = p.expect('@');
    op.reg = parseRegName(p.skipSpaces());
    if (op.reg != REG_UNDEF) {
        if (!indir) {
            op.mode = toAddrMode(op.reg, op.val);
        } else if (isRegName(op.reg)) {
            op.mode = M_IDIR;
            op.val.setUnsigned(toRegNum(op.reg));
        } else {
            return op.setError(UNKNOWN_OPERAND);
        }
        scan = p;
        return OK;
    }

    op.val = parseInteger(p, op);
    if (op.hasError())
        return op.getError();
    if (indir) {
        if (op.val.overflow(UINT8_MAX))
            return op.setError(OVERFLOW_RANGE);
        op.mode = M_IDIR;
    } else if (parseIndex(p, op, false) == OK) {
        ;
    } else if (op.hasError()) {
        return op.getError();
    } else {
        if (op.val.overflow(0x1FF)) {
            op.mode = M_REL;
        } else if (op.val.overflow(UINT8_MAX)) {
            op.mode = M_ADRP;
        } else {
            op.mode = M_ADRR;
        }
    }
    scan = p;
    return OK;
}

void AsmTms370::emitRelative(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    const auto deltaLen = (mode == M_REL) ? 1 : 2;
    const auto base = insn.address() + insn.operandPos() + deltaLen;
    const auto target = op.getError() ? base : op.val.getUnsigned();
    if (mode == M_REL) {
        const auto delta = branchDelta(base, target, insn, op);
        if (overflowDelta(delta, 8))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        insn.emitOperand8(delta);
    }
    if (mode == M_LREL || mode == M_BREL) {
        const auto delta = target - base;
        insn.emitOperand16(delta);
    }
}

void AsmTms370::encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    insn.setErrorIf(op);
    switch (mode) {
    case M_SIM8:
        if (op.val.overflowInt8())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val.getUnsigned());
        break;
    case M_IM8:
        if (op.val.overflowUint8())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val.getUnsigned());
        break;
    case M_RN:
    case M_PN:
    case M_IDIR:
        insn.emitOperand8(op.val.getUnsigned());
        break;
    case M_REL:
    case M_LREL:
    case M_BREL:
        emitRelative(insn, op, mode);
        break;
    case M_IM16:
        if (op.val.overflowUint16())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(op.val.getUnsigned());
        break;
    case M_ABS:
    case M_BIDX:
    case M_BIMM:
        if (op.val.overflow(UINT16_MAX))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(op.val.getUnsigned());
        break;
    case M_SIDX:
    case M_RIDX:
    case M_RIMM:
        if (op.val.overflowInt8())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val.getSigned());
        if (mode == M_RIDX || mode == M_RIMM)
            insn.emitOperand8(toRegNum(op.reg));
        break;
    case M_TVEC:
        if (op.val.overflow(15)) {
            insn.setError(op, OVERFLOW_RANGE);
        } else {
            insn.embed(15 - op.val.getUnsigned());
        }
        break;
    default:
        break;
    }
}

Error AsmTms370::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    if (parseOperand(scan, insn.srcOp) && insn.srcOp.hasError())
        return _insn.setError(insn.srcOp);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, insn.dstOp) && insn.dstOp.hasError())
            return _insn.setError(insn.dstOp);
    }
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, insn.extOp) && insn.extOp.hasError())
            return _insn.setError(insn.extOp);
    }

    if (searchName(cpuType(), insn))
        return _insn.setError(insn.srcOp, insn);

    encodeOperand(insn, insn.srcOp, insn.src());
    encodeOperand(insn, insn.dstOp, insn.dst());
    encodeOperand(insn, insn.extOp, insn.ext());
    insn.emitInsn();
    return _insn.setError(insn);
}

}  // namespace tms370
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
