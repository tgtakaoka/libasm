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

#include "asm_tms9900.h"

#include "table_tms9900.h"
#include "text_common.h"

namespace libasm {
namespace tms9900 {

using namespace pseudo;
using namespace reg;
using namespace text::common;

namespace {

// clang-format off
constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_AORG, &Assembler::defineOrigin},
    {TEXT_BSS,  &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
    {TEXT_BYTE, &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_DATA, &Assembler::defineDataConstant, Assembler::DATA_WORD_ALIGN2},
    {TEXT_EVEN, &Assembler::alignOrigin, 2},
    {TEXT_TEXT, &Assembler::defineDataConstant, Assembler::DATA_BYTE},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

}  // namespace

const ValueParser::Plugins &AsmTms9900::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return TexasNumberParser::singleton(); }
        const CommentParser &comment() const override { return AsteriskCommentParser::singleton(); }
    } PLUGINS{};
    return PLUGINS;
}

AsmTms9900::AsmTms9900(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE), Config(TABLE) {
    reset();
}

void AsmTms9900::encodeRelative(AsmInsn &insn, const Operand &op) const {
    const auto base = insn.address() + 2;
    const auto target = op.getError() ? base : op.val16;
    insn.setErrorIf(op, checkAddr(target, true));
    const auto delta = branchDelta(base, target, insn, op) / 2;
    if (overflowInt8(delta))
        insn.setErrorIf(op, OPERAND_TOO_FAR);
    insn.embed(static_cast<uint8_t>(delta));
}

void AsmTms9900::encodeCruOffset(AsmInsn &insn, const Operand &op) const {
    const int16_t offset = static_cast<int16_t>(op.val16);
    if (overflowInt16(offset))
        insn.setErrorIf(op, OVERFLOW_RANGE);
    insn.embed(static_cast<uint8_t>(offset));
}

void AsmTms9900::encodeModeReg(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    if (mode == M_SRC2 && insn.dst() == M_BIT2 && op.mode == M_INCR)
        insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
    auto opc = encodeRegNumber(op.reg);
    switch (op.mode) {
    case M_REG:
        break;
    case M_IREG:
        opc |= (1 << 4);
        break;
    case M_INCR:
        opc |= (3 << 4);
        break;
    case M_SYBL:
        opc = (2 << 4);
        if (op.getError() != UNDEFINED_SYMBOL)
            insn.setErrorIf(op, checkAddr(op.val16, !insn.byteOp()));
        insn.emitOperand16(op.val16);
        break;
    default:  // M_INDX
        if (op.reg == REG_R0)
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        opc |= (2 << 4);
        insn.emitOperand16(op.val16);
        break;
    }
    switch (mode) {
    case M_SRC:
        insn.embed(opc);
        break;
    case M_SRC2:
        insn.embedPostfix(opc);
        break;
    case M_DST:
        insn.embed(opc << 6);
        break;
    default:
        insn.embedPostfix(opc << 6);
        break;
    }
}

void AsmTms9900::encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    insn.setErrorIf(op);
    auto val16 = op.val16;
    switch (mode) {
    case M_IMM:
        insn.emitOperand16(op.val16);
        break;
    case M_REG:
        insn.embed(encodeRegNumber(op.reg));
        break;
    case M_DREG:
        insn.embed(encodeRegNumber(op.reg) << 6);
        break;
    case M_DST2:
        insn.embedPostfix(0x4000);
        /* Fall-through */
    case M_SRC:
    case M_SRC2:
    case M_DST:
        encodeModeReg(insn, op, mode);
        break;
    case M_REL:
        encodeRelative(insn, op);
        break;
    case M_CRU:
        encodeCruOffset(insn, op);
        break;
    case M_CNT:
        if (val16 == 0 && !op.getError()) {
            insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        } else if (val16 == 16) {
            val16 = 0;
        }
        /* Fall-through */
    case M_XOP:
        if (val16 >= 16)
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.embed((val16 & 0xF) << 6);
        break;
    case M_SCNT:
        if (op.mode == M_REG) {
            if (op.reg != REG_R0)
                insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
            break;
        }
        if (val16 == 0 && !op.getError()) {
            insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        } else if (val16 >= 16)
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.embed((val16 & 0xF) << 4);
        break;
    case M_CNT2:
        insn.embedPostfix(0x4000);
        if (op.mode == M_REG) {
            if (op.reg != REG_R0)
                insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
            break;
        }
        if (val16 == 0 && !op.getError())
            insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        /* Fall-through */
    case M_BIT2:
        if (val16 >= 16)
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.embedPostfix((val16 & 0xF) << 6);
        break;
    case M_RTWP:
        // 0,1,2,4 are acceptable.
        if (val16 == 3 || val16 > 4)
            insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        insn.embed(val16 & 7);
        break;
    default:
        break;
    }
}

Error AsmTms9900::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    if (p.expect('*')) {
        op.reg = parseRegName(p);
        if (op.reg == REG_UNDEF)
            return op.setError(UNKNOWN_OPERAND);
        op.mode = p.expect('+') ? M_INCR : M_IREG;
        scan = p;
        return OK;
    }
    if (p.expect('@')) {
        op.val16 = parseExpr16(p, op);
        if (op.hasError())
            return op.getError();
        if (p.skipSpaces().expect('(')) {
            const auto regp = p.skipSpaces();
            op.reg = parseRegName(p);
            if (op.reg == REG_UNDEF)
                return op.setError(UNKNOWN_OPERAND);
            if (op.reg == REG_R0)
                return op.setError(regp, REGISTER_NOT_ALLOWED);
            if (!p.skipSpaces().expect(')'))
                return op.setError(p, MISSING_CLOSING_PAREN);
            op.mode = M_INDX;
        } else {
            op.mode = M_SYBL;
        }
        scan = p;
        return OK;
    }
    op.reg = parseRegName(p);
    if (op.reg != REG_UNDEF) {
        op.mode = M_REG;
        scan = p;
        return OK;
    }

    op.val16 = parseExpr16(p, op);
    if (op.hasError())
        return op.getError();
    op.mode = M_IMM;
    scan = p;
    return OK;
}

Error AsmTms9900::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    if (parseOperand(scan, insn.srcOp) && insn.srcOp.hasError())
        return _insn.setError(insn.srcOp);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, insn.dstOp) && insn.dstOp.hasError())
            return _insn.setError(insn.dstOp);
        scan.skipSpaces();
    }

    if (_insn.setErrorIf(insn.srcOp, TABLE.searchName(cpuType(), insn)))
        return _insn.getError();

    encodeOperand(insn, insn.srcOp, insn.src());
    encodeOperand(insn, insn.dstOp, insn.dst());
    insn.emitInsn();
    return _insn.setError(insn);
}

}  // namespace tms9900
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
