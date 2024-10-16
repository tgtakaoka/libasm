/*
 * Copyright 2021 Tadashi G. Takaoka
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

#include "asm_mn1610.h"

#include "table_mn1610.h"
#include "text_common.h"

namespace libasm {
namespace mn1610 {

using namespace pseudo;
using namespace reg;
using namespace text::common;

namespace {

// clang-format off
constexpr char TEXT_LOC[] PROGMEM = "loc";

constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_DC,  &Assembler::defineDataConstant, Assembler::DATA_WORD},
    {TEXT_DS,  &Assembler::allocateSpaces,     Assembler::DATA_WORD},
    {TEXT_LOC, &Assembler::defineOrigin},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

struct Mn1610NumberParser final : IbmNumberParser, Singleton<Mn1610NumberParser> {
    Mn1610NumberParser() : IbmNumberParser('X', 0, 0, 0) {}
};

struct Mn1610SymbolParser final : SymbolParser, Singleton<Mn1610SymbolParser> {
    bool locationSymbol(StrScanner &scan) const override {
        return SymbolParser::locationSymbol(scan, '*');
    }
};

}  // namespace

const ValueParser::Plugins &AsmMn1610::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return Mn1610NumberParser::singleton(); }
        const SymbolParser &symbol() const override { return Mn1610SymbolParser::singleton(); }
        const CommentParser &comment() const override { return StarCommentParser::singleton(); }
        const LetterParser &letter() const override { return IbmLetterParser::singleton(); }
    } PLUGINS{};
    return PLUGINS;
}

AsmMn1610::AsmMn1610(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE), Config(TABLE) {
    reset();
}

void AsmMn1610::encodeIcRelative(AsmInsn &insn, const Operand &op) const {
    const auto delta = branchDelta(insn.address(), op.val.getUnsigned(), insn, op);
    if (overflowDelta(delta, 8))
        insn.setErrorIf(op, OPERAND_TOO_FAR);
    const Config::opcode_t mode = (op.mode == M_IABS) ? 3 : 1;
    insn.embed(mode << 11);
    insn.embed(static_cast<uint8_t>(delta));
}

void AsmMn1610::encodeGenericAddress(AsmInsn &insn, const Operand &op) const {
    switch (op.mode) {
    case M_IABS:
        if (!op.val.overflow(UINT8_MAX)) {  // Zero-page indirect: (D)
            insn.embed(2 << 11);
            insn.embed(static_cast<uint8_t>(op.val.getUnsigned()));
            break;
        }
        // Relative indirect: (abs)
        // Fall-through
    case M_ABS:
    case M_IM16:  // Relative: abs
        encodeIcRelative(insn, op);
        break;
    case M_IXID:
    case M_INDX:
    case M_IDIX:
        if (isIndex(op.reg)) {
            if (op.mode == M_IXID)
                insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
            if (op.val.overflow(UINT8_MAX))
                insn.setErrorIf(op, OVERFLOW_RANGE);
            // Direct index: D(Xj), Indirect index: (D)(Xj)
            insn.embed(static_cast<uint8_t>(op.val.getUnsigned()));
            const Config::opcode_t mode = (op.mode == M_IDIX ? 2 : 0) | encodeIndex(op.reg);
            insn.embed(mode << 11);
            break;
        }
        if (op.reg == REG_IC) {
            if (op.mode == M_IDIX)
                insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
            if (op.val.overflowInt8())
                insn.setErrorIf(op, OPERAND_TOO_FAR);
            // Relative: d(IC), Relative indirect: (d(IC))
            insn.embed(static_cast<uint8_t>(op.val.getUnsigned()));
            insn.embed((op.mode == M_INDX ? 1 : 3) << 11);
            break;
        }
        insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        break;
    case M_IM8:
    case M_IM4:
    case M_ILVL:
        // Zero-page direct: D
        insn.embed((0 << 11) | op.val.getUnsigned());
        break;
    default:
        break;
    }
}

void AsmMn1610::encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    insn.setErrorIf(op);
    uint8_t val8 = op.val.getUnsigned();
    switch (mode) {
    case M_RDG:
        if (op.reg == REG_STR)
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        // Fall-Through
    case M_RD:
        insn.embed(encodeGeneric(op.reg) << 8);
        break;
    case M_RSG:
        if (op.reg == REG_STR)
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        // Fall-Through
    case M_RS:
        insn.embed(encodeGeneric(op.reg));
        break;
    case M_RBW:
        if (op.reg == REG_CSBR)
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        // Fall-Through
    case M_SB:
    case M_RB:
        insn.embed(encodeSegment(op.reg) << 4);
        break;
    case M_RHR:
    case M_RHW:
        if ((mode == M_RHR && op.reg == REG_SOR) || (mode == M_RHW && op.reg == REG_SIR))
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        insn.embed(encodeHardware(op.reg) << 4);
        break;
    case M_RP:
        insn.embed(encodeSpecial(op.reg) << 4);
        break;
    case M_RIAU:
        insn.embed((op.mode == M_RIAU ? (op.val.getUnsigned() == 1 ? 3 : 2) : 1) << 6);
        // Fall-through
    case M_RI:
        insn.embed(encodeIndirect(op.reg));
        break;
    case M_SKIP:
        if (op.mode == M_SKIP)  // op.mode may be M_NONE
            insn.embed(encodeSkip(op.cc) << 4);
        break;
    case M_EOP:
        insn.embed(encodeEop(op.cc));
        break;
    case M_COP:
        if (op.mode == M_COP) {
            insn.embed(encodeCop(op.cc) << 3);
            break;
        }
        if (op.val.overflow(1))
            insn.setErrorIf(op, ILLEGAL_CONSTANT);
        insn.embed((op.val.getUnsigned() & 1) << 3);
        break;
    case M_IM8W:
    case M_IM16:
        if (op.val.overflowUint16())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        // Fall-through
    case M_IABS:
        insn.emitOperand16(op.val.getUnsigned());
        break;
    case M_BIT:
        if (op.val.overflow(15))
            insn.setErrorIf(op, ILLEGAL_BIT_NUMBER);
        insn.embed(static_cast<uint8_t>(op.val.getUnsigned() & 0x0F));
        break;
    case M_ILVL:
        if (op.val.overflow(3)) {
            insn.setErrorIf(op, ILLEGAL_CONSTANT);
            val8 &= 3;
        }
        // Fall-through
    case M_IM4:
        if (op.val.overflow(15)) {
            insn.setErrorIf(op, OVERFLOW_RANGE);
            val8 &= 0xF;
        }
        // Fall-through
    case M_IM8:
    case M_IOA:
        if (op.val.overflowUint8())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.embed(val8);
        break;
    case M_ABS: {
        // TODO: calculate/check segment/offset
        const auto addr = op.val.getUnsigned();
        insn.setErrorIf(op, checkAddr(addr));
        insn.emitOperand16(addr);
        break;
    }
    case M_GEN:
        encodeGenericAddress(insn, op);
        break;
    default:
        break;
    }
}

Error AsmMn1610::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);

    op.cc = parseCcName(p);
    if (op.cc != CC_UNDEF) {
        if (isSkip(op.cc)) {
            op.mode = M_SKIP;
        } else if (isCop(op.cc)) {
            op.mode = M_COP;
        } else if (isEop(op.cc)) {
            op.mode = M_EOP;
        }
        scan = p;
        return OK;
    }

    auto t = p;
    const auto indir = p.expect('(') ? ')' : 0;
    const auto preDec = (*t == '-' && *++t == '(');
    if (indir || preDec)
        ++t;
    const auto r = t;
    op.reg = parseRegName(t);
    if (op.reg != REG_UNDEF) {
        // r, (r), -(r), (r)+
        t.skipSpaces();
        if (indir || preDec) {
            if (op.reg == REG_R1) {
                op.mode = M_RI1;
            } else if (op.reg == REG_R2) {
                op.mode = M_RI2;
            } else if (isIndirect(op.reg)) {
                op.mode = M_RI;
            } else {
                return op.setError(r, REGISTER_NOT_ALLOWED);
            }
            if (!t.expect(')'))
                return op.setError(t, MISSING_CLOSING_PAREN);
            if (preDec) {
                op.mode = M_RIAU;
                op.val.setSigned(-1);
            } else if (t.expect('+')) {
                op.mode = M_RIAU;
                op.val.setUnsigned(1);
            }
        } else {
            if (op.reg == REG_R0) {
                op.mode = M_R0;
            } else if (isGeneric(op.reg)) {
                op.mode = (op.reg == REG_STR) ? M_RD : M_RDG;
            } else if (isSegmentBase(op.reg)) {
                op.mode = M_SB;
            } else if (op.reg == REG_DR0) {
                op.mode = M_DR0;
            } else if (isSegment(op.reg)) {
                op.mode = M_RB;
            } else if (isSpecial(op.reg)) {
                op.mode = M_RP;
            } else if (isHardware(op.reg)) {
                op.mode = M_RHR;
            }
        }
        scan = t;
        return OK;
    }
    if (preDec)
        return op.setError(UNKNOWN_OPERAND);

    // v, (v), (v)(r), v(r), (v(r))
    op.val = parseInteger(p, op, indir);
    if (op.hasError())
        return op.getError();
    if (endOfLine(p) || *p == ',') {
        // v
        if (op.val.overflowUint16()) {
            op.mode = M_ABS;
        } else if (op.val.overflowUint8()) {
            op.mode = M_IM16;
        } else {
            op.mode = M_IM8;
        }
        scan = p;
        return OK;
    }
    if (p.expect('(')) {
        // v(r), (v(r))
        op.reg = parseRegName(p.skipSpaces());
        if (op.reg == REG_UNDEF)
            return op.setError(p, UNKNOWN_OPERAND);
        if (!p.skipSpaces().expect(')'))
            return op.setError(p, MISSING_CLOSING_PAREN);
        p.skipSpaces();
        if (indir && !p.expect(')'))
            return op.setError(p, MISSING_CLOSING_PAREN);
        op.mode = indir ? M_IXID : M_INDX;
        scan = p;
        return OK;
    }
    if (indir && !p.expect(')'))
        return op.setError(p, MISSING_CLOSING_PAREN);
    auto rp = p;
    // (v)(r)
    if (rp.skipSpaces().expect('(')) {
        op.reg = parseRegName(rp.skipSpaces());
        if (op.reg != REG_UNDEF) {
            if (!rp.skipSpaces().expect(')'))
                return op.setError(rp, MISSING_CLOSING_PAREN);
            op.mode = M_IDIX;
            scan = rp;
            return OK;
        }
    }
    // (v)
    if (endOfLine(p) || *p == ',') {
        op.mode = M_IABS;
        scan = p;
        return OK;
    }
    return op.setError(UNKNOWN_OPERAND);
}

Error AsmMn1610::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    if (TABLE.hasOperand(cpuType(), insn)) {
        if (parseOperand(scan, insn.op1) && insn.op1.hasError())
            return _insn.setError(insn.op1);
        if (scan.skipSpaces().expect(',')) {
            if (parseOperand(scan, insn.op2) && insn.op2.hasError())
                return _insn.setError(insn.op2);
        }
        if (scan.skipSpaces().expect(',')) {
            if (parseOperand(scan, insn.op3) && insn.op3.hasError())
                return _insn.setError(insn.op3);
        }
        if (scan.skipSpaces().expect(',')) {
            if (parseOperand(scan, insn.op4) && insn.op4.hasError())
                return _insn.setError(insn.op4);
        }
    }
    scan.skipSpaces();

    if (_insn.setErrorIf(insn.op1, TABLE.searchName(cpuType(), insn)))
        return _insn.getError();

    encodeOperand(insn, insn.op1, insn.mode1());
    encodeOperand(insn, insn.op2, insn.mode2());
    encodeOperand(insn, insn.op3, insn.mode3());
    encodeOperand(insn, insn.op4, insn.mode4());
    insn.emitInsn();
    return _insn.setError(insn);
}

}  // namespace mn1610
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
