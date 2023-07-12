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

#include "asm_i8051.h"

#include "operators.h"
#include "reg_i8051.h"
#include "table_i8051.h"
#include "text_common.h"

namespace libasm {
namespace i8051 {

using namespace pseudo;
using namespace reg;
using namespace text::common;

namespace {

constexpr Pseudo PSEUDOS[] PROGMEM = {
        Pseudo{TEXT_ALIGN, &Assembler::alignOrigin},
        Pseudo{TEXT_DB, &Assembler::defineDataConstant, Assembler::DATA_BYTE},
        Pseudo{TEXT_DS, &Assembler::allocateSpaces, Assembler::DATA_BYTE},
        Pseudo{TEXT_DW, &Assembler::defineDataConstant, Assembler::DATA_WORD},
        Pseudo{TEXT_ORG, &Assembler::defineOrigin},
};

}  // namespace

const ValueParser::Plugins &AsmI8051::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return IntelNumberParser::singleton(); }
        const SymbolParser &symbol() const override { return _symbol; }
        const SimpleSymbolParser _symbol{SymbolParser::QUESTION, SymbolParser::NONE};
        const OperatorParser &operators() const override {
            return IntelOperatorParser::singleton();
        }
    } PLUGINS{};
    return PLUGINS;
}

struct AsmI8051::Operand final : ErrorAt {
    AddrMode mode;
    RegName reg;
    uint16_t val16;
    Operand() : mode(M_NONE), reg(REG_UNDEF), val16(0) {}
};

AsmI8051::AsmI8051(const ValueParser::Plugins &plugins)
    : Assembler(plugins, ARRAY_RANGE(PSEUDOS)), Config(TABLE) {
    reset();
}

Error AsmI8051::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    if (p.expect('#')) {
        op.val16 = parseExpr16(p, op);
        if (op.hasError())
            return op.getError();
        op.mode = M_IMM16;
        scan = p;
        return OK;
    }

    const auto indir = p.expect('@');
    if (indir && isspace(*p))
        return op.setError(UNKNOWN_OPERAND);

    const auto regp = p;
    op.reg = parseRegName(p);
    if (op.reg != REG_UNDEF) {
        if (indir && op.reg == REG_A && p.expect('+')) {
            const auto base = parseRegName(p);
            if (base == REG_DPTR || base == REG_PC) {
                op.mode = (base == REG_DPTR) ? M_INDXD : M_INDXP;
                scan = p;
                return OK;
            }
            return op.setError(UNKNOWN_OPERAND);
        }
        scan = p;
        if (indir) {
            if (op.reg == REG_R0 || op.reg == REG_R1) {
                op.mode = M_IDIRR;
                return OK;
            }
            if (op.reg == REG_DPTR) {
                op.mode = M_IDIRD;
                return OK;
            }
            return op.setError(UNKNOWN_OPERAND);
        }
        if (isRReg(op.reg)) {
            op.mode = M_RREG;
            return OK;
        }
        switch (op.reg) {
        case REG_A:
            op.mode = M_AREG;
            return OK;
        case REG_C:
            op.mode = M_CREG;
            return OK;
        case REG_DPTR:
            op.mode = M_DREG;
            return OK;
        case REG_AB:
            op.mode = M_ABREG;
            return OK;
        default:
            return op.setError(regp, UNKNOWN_OPERAND);
        }
    }
    if (indir)
        return op.setError(UNKNOWN_OPERAND);

    const auto addrp = p;
    const auto bitNot = p.expect('/');
    op.val16 = parseExpr16(p.skipSpaces(), op);
    if (op.hasError())
        return getError();
    if (p.expect('.')) {
        if (op.getError())
            op.val16 = 0x20;
        const auto bitp = p;
        ErrorAt bitOp;
        auto bitNo = parseExpr16(p, bitOp);
        if (bitOp.getError())
            op.setErrorIf(bitOp);
        if (bitNo >= 8)
            return op.setError(bitp, ILLEGAL_BIT_NUMBER);
        auto val16 = op.val16;
        if ((val16 & ~0x0F) == 0x20 || (val16 & ~0x78) == 0x80) {
            op.mode = bitNot ? M_NOTAD : M_BITAD;
            if ((val16 & 0x80) == 0)
                op.val16 = (val16 & 0xF) << 3;
            op.val16 |= bitNo;
            scan = p;
            return OK;
        }
        return op.setError(addrp, NOT_BIT_ADDRESSABLE);
    }
    op.mode = bitNot ? M_NOTAD : M_ADR16;
    scan = p;
    return OK;
}

void AsmI8051::encodeOperand(AsmInsn &insn, const AddrMode mode, const Operand &op) {
    switch (mode) {
    case M_REL: {
        auto len = insn.length();
        if (len == 0)
            len = 1;
        const Config::uintptr_t base = insn.address() + len + 1;
        const Config::uintptr_t target = op.getError() ? base : op.val16;
        const Config::ptrdiff_t delta = target - base;
        if (overflowInt8(delta))
            setErrorIf(op, OPERAND_TOO_FAR);
        insn.emitOperand8(delta);
        return;
    }
    case M_RREG:
    case M_IDIRR:
        insn.embed(encodeRRegName(op.reg));
        return;
    case M_ADR8:
        if (op.val16 >= 0x100)
            setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val16);
        return;
    case M_IMM8: {
        if (overflowUint8(op.val16))
            setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val16);
        return;
    }
    case M_ADR11: {
        const Config::uintptr_t base = insn.address() + 2;
        const Config::uintptr_t target = op.getError() ? (base & ~0x7FF) : op.val16;
        if ((base & ~0x7FF) != (target & ~0x7FF))
            setErrorIf(op, OPERAND_TOO_FAR);
        insn.embed((target & 0x700) >> 3);
        insn.emitOperand8(target);
        return;
    }
    case M_ADR16:
    case M_IMM16:
        insn.emitOperand16(op.val16);
        return;
    case M_BITAD:
    case M_NOTAD:
        if (op.val16 >= 0x100)
            setErrorIf(op, NOT_BIT_ADDRESSABLE);
        insn.emitOperand8(op.val16);
        return;
    default:
        return;
    }
}

Error AsmI8051::encodeImpl(StrScanner &scan, Insn &_insn) {
    AsmInsn insn(_insn);
    Operand dstOp, srcOp, extOp;
    if (parseOperand(scan, dstOp) && dstOp.hasError())
        return setError(dstOp);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, srcOp) && srcOp.hasError())
            return setError(srcOp);
        scan.skipSpaces();
    }
    if (scan.expect(',')) {
        if (parseOperand(scan, extOp) && extOp.hasError())
            return setError(extOp);
        scan.skipSpaces();
    }
    if (!endOfLine(scan))
        return setError(GARBAGE_AT_END);
    setErrorIf(dstOp);
    setErrorIf(srcOp);
    setErrorIf(extOp);

    insn.setAddrMode(dstOp.mode, srcOp.mode, extOp.mode);
    const auto error = TABLE.searchName(cpuType(), insn);
    if (error)
        return setError(dstOp, error);

    const auto dst = insn.dst();
    const auto src = insn.src();
    if (dst == M_ADR8 && src == M_ADR8) {
        encodeOperand(insn, src, srcOp);
        encodeOperand(insn, dst, dstOp);
    } else {
        encodeOperand(insn, dst, dstOp);
        encodeOperand(insn, src, srcOp);
        encodeOperand(insn, insn.ext(), extOp);
    }
    insn.emitInsn();
    return setErrorIf(insn);
}

}  // namespace i8051
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
