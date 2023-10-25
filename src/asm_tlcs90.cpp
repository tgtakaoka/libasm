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

#include "asm_tlcs90.h"

#include "table_tlcs90.h"
#include "text_common.h"

namespace libasm {
namespace tlcs90 {

using namespace pseudo;
using namespace reg;
using namespace text::common;

namespace {

// clang-format off
constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_DB,   &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_DEFB, &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_DEFM, &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_DEFS, &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
    {TEXT_DEFW, &Assembler::defineDataConstant, Assembler::DATA_WORD},
    {TEXT_DL,   &Assembler::defineDataConstant, Assembler::DATA_LONG},
    {TEXT_DS,   &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
    {TEXT_DW,   &Assembler::defineDataConstant, Assembler::DATA_WORD},
};
// clang-format off
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

}  // namespace

AsmTlcs90::AsmTlcs90(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE), Config(TABLE) {
    reset();
}

const ValueParser::Plugins &AsmTlcs90::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return IntelNumberParser::singleton(); }
        const SymbolParser &symbol() const override { return _symbol; }
        const PrefixSymbolParser _symbol{nullptr, PSTR_UNDER_QUESTION};
    } PLUGINS{};
    return PLUGINS;
}

void AsmTlcs90::encodeRelative(AsmInsn &insn, AddrMode mode, const Operand &op) const {
    const auto base = insn.address() + 2;
    const auto target = op.getError() ? base : op.val16;
    const auto delta = branchDelta(base, target, insn, op);
    if (mode == M_REL16) {
        insn.emitUint16(delta);
    } else {
        if (overflowInt8(delta))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        insn.emitByte(delta);
    }
}

void AsmTlcs90::encodeOperand(
        AsmInsn &insn, AddrMode mode, const Operand &op, Config::opcode_t opc) const {
    insn.setErrorIf(op);
    switch (mode) {
    case M_IMM8:
        if (overflowUint8(op.val16))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        /* Fall-through */
    case M_DIR:
        insn.emitInsn(opc);
        insn.emitByte(op.val16);
        return;
    case M_IMM16:
    case M_EXT:
        insn.emitInsn(opc);
        insn.emitUint16(op.val16);
        return;
    case M_BIT:
        if (op.val16 >= 8)
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitInsn(opc | (op.val16 & 7));
        return;
    case M_REL8:
    case M_REL16:
        insn.emitInsn(opc);
        encodeRelative(insn, mode, op);
        return;
    case M_IND:
        insn.emitInsn(opc | encodeReg16(op.reg));
        return;
    case M_IDX:
        if (overflowInt8(static_cast<int16_t>(op.val16)))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitInsn(opc | encodeIndexReg(op.reg));
        insn.emitByte(op.val16);
        return;
    case M_CC:
        insn.emitInsn(opc | encodeCcName(op.cc));
        return;
    case M_STACK:
        insn.emitInsn(opc | encodeStackReg(op.reg));
        return;
    case M_REG8:
        insn.emitInsn(opc | encodeReg8(op.reg));
        return;
    case M_REG16:
        insn.emitInsn(opc | encodeReg16(op.reg));
        return;
    case M_REGIX:
        insn.emitInsn(opc | encodeIndexReg(op.reg));
        return;
    case R_A:
    case R_HL:
        return;
    default:
        insn.emitInsn(opc);
        return;
    }
}

Error AsmTlcs90::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    const auto cc = parseCcName(p);
    if (cc != CC_UNDEF) {
        op.mode = M_CC;
        op.cc = cc;
        if (cc == CC_C) {
            // 'C' is either REG_C or CC_C
            op.mode = R_C;
            op.reg = REG_C;
        }
        scan = p;
        return OK;
    }

    auto reg = parseRegName(p);
    if (reg != REG_UNDEF) {
        switch (reg) {
        case REG_IX:
        case REG_IY:
            op.mode = M_REGIX;
            break;
        case REG_B:
        case REG_D:
        case REG_E:
        case REG_H:
        case REG_L:
            op.mode = M_REG8;
            break;
        default:
            op.mode = AddrMode(int8_t(reg) + R_BASE);
            break;
        }
        op.reg = reg;
        scan = p;
        return OK;
    }
    if (p.expect('(')) {
        const auto regp = p.skipSpaces();
        reg = parseRegName(p);
        if (reg == REG_UNDEF) {  // (nnnn)
            op.val16 = parseExpr16(p, op, ')');
            if (op.hasError())
                return op.getError();
            if (!p.skipSpaces().expect(')'))
                return op.setError(p, MISSING_CLOSING_PAREN);
            op.mode = op.getError() ? M_SYM : (op.val16 >= 0xFF00 ? M_DIR : M_EXT);
            scan = p;
            return OK;
        }
        op.reg = reg;
        if (p.skipSpaces().expect(')')) {  // (rr)
            if (isReg16(reg)) {
                op.mode = M_IND;
                scan = p;
                return OK;
            }
            return op.setError(regp, REGISTER_NOT_ALLOWED);
        }
        auto a = p;
        if (reg == REG_HL && a.expect('+')) {
            const auto idxp = a.skipSpaces();
            const auto idx = parseRegName(a);
            if (idx != REG_UNDEF) {
                if (idx != REG_A)
                    return op.setError(idxp, REGISTER_NOT_ALLOWED);
                if (!a.skipSpaces().expect(')'))
                    return op.setError(a, MISSING_CLOSING_PAREN);
                scan = a;
                op.mode = M_BASE;  // (HL+A)
                return OK;
            }
        }
        if (*p == '+' || *p == '-') {  // (rr+n)
            op.val16 = parseExpr16(p, op, ')');
            if (op.hasError())
                return op.getError();
            if (!p.skipSpaces().expect(')'))
                return op.setError(p, MISSING_CLOSING_PAREN);
            if (isRegIndex(reg)) {
                scan = p;
                op.mode = M_IDX;
                return OK;
            }
            return op.setError(regp, REGISTER_NOT_ALLOWED);
        }
        return op.setError(UNKNOWN_OPERAND);
    }
    op.val16 = parseExpr16(p, op);
    if (op.hasError())
        return op.getError();
    op.mode = M_IMM16;
    scan = p;
    return OK;
}

Error AsmTlcs90::encodeImpl(StrScanner &scan, Insn &_insn) {
    AsmInsn insn(_insn);
    if (parseOperand(scan, insn.dstOp) && insn.dstOp.hasError())
        return setError(insn.dstOp);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, insn.srcOp) && insn.srcOp.hasError())
            return setError(insn.srcOp);
        scan.skipSpaces();
    }

    if (setErrorIf(insn.dstOp, TABLE.searchName(cpuType(), insn)))
        return getError();

    const auto pre = insn.pre();
    const auto dst = insn.dst();
    const auto src = insn.src();
    const auto prefix = insn.prefix();
    if (prefix) {
        insn.setEmitInsn();
        if (pre == M_DST)
            encodeOperand(insn, dst, insn.dstOp, prefix);
        if (pre == M_SRC)
            encodeOperand(insn, src, insn.srcOp, prefix);
        if (pre == M_NONE)
            insn.emitByte(prefix);
    }
    const auto opc = insn.opCode();
    insn.setEmitInsn();
    if (pre != M_DST)
        encodeOperand(insn, dst, insn.dstOp, opc);
    if (pre != M_SRC)
        encodeOperand(insn, src, insn.srcOp, opc);
    insn.emitInsn(opc);
    return setError(insn);
}

}  // namespace tlcs90
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
