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
#include "ibm_float.h"
#include "table_tms9900.h"
#include "text_tms9900.h"

namespace libasm {
namespace tms9900 {

using namespace pseudo;
using namespace reg;
using namespace text::common;
using namespace text::tms9900;

namespace {

// clang-format off
constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_AORG, &Assembler::defineOrigin},
    {TEXT_BSS,  &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
    {TEXT_BYTE, &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_DATA, &Assembler::defineDataConstant, Assembler::DATA_WORD|Assembler::DATA_ALIGN2},
    {TEXT_EVEN, &Assembler::alignOrigin,        Assembler::ALIGN_EVEN},
    {TEXT_TEXT, &Assembler::defineDataConstant, Assembler::DATA_BYTE},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

}  // namespace

const ValueParser::Plugins &AsmTms9900::defaultPlugins() {
    return ValueParser::Plugins::texas();
}

AsmTms9900::AsmTms9900(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE), Config(TABLE) {
    reset();
}

void AsmTms9900::encodeRelative(AsmInsn &insn, const Operand &op) const {
    const auto base = insn.address() + 2;
    const auto target = op.getError() ? base : op.val.getUnsigned();
    insn.setErrorIf(op, checkAddr(target, true));
    const auto delta = branchDelta(base, target, insn, op) / 2;
    if (overflowDelta(delta, 8))
        insn.setErrorIf(op, OPERAND_TOO_FAR);
    insn.embed(static_cast<uint8_t>(delta));
}

void AsmTms9900::encodeCruOffset(AsmInsn &insn, const Operand &op) const {
    const auto offset = op.val.getSigned();
    if (op.val.overflowInt16())
        insn.setErrorIf(op, OVERFLOW_RANGE);
    insn.embed(static_cast<uint8_t>(offset));
}

void AsmTms9900::encodeModeReg(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    if (mode == M_SRC2 && insn.dst() == M_BIT0 && op.mode == M_INCR)
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
        insn.setErrorIf(op, checkAddr(op.val.getUnsigned(), !insn.byteOp()));
        insn.emitOperand16(op.val.getUnsigned());
        break;
    default:  // M_INDX
        if (op.reg == REG_R0)
            insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
        opc |= (2 << 4);
        insn.emitOperand16(op.val.getUnsigned());
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
    auto val16 = op.val.getUnsigned();
    switch (mode) {
    case M_IMM:
    case M_SYBL:
        insn.emitOperand16(val16);
        break;
    case M_REG:
        insn.embed(encodeRegNumber(op.reg));
        break;
    case M_DREG:
        insn.embed(encodeRegNumber(op.reg) << 6);
        break;
    case M_DST4:
        insn.embedPostfix(0x4000);
        /* Fall-through */
    case M_DST0:
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
        // 0 < op.val <= 16
        if (op.val.isZero() || op.val.overflow(16))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        if (val16 == 16)
            val16 = 0;
        insn.embed((val16 & 0xF) << 6);
        break;
    case M_XOP:
        // 0 <= op.val < 16
        if (op.val.overflow(15))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.embed((val16 & 0xF) << 6);
        break;
    case M_SCNT:
        if (op.mode == M_REG) {
            if (op.reg != REG_R0)
                insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
            break;
        }
        if (op.val.isZero()) {
            insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        } else if (op.val.overflow(15)) {
            insn.setErrorIf(op, OVERFLOW_RANGE);
        }
        insn.embed((val16 & 0xF) << 4);
        break;
    case M_CNT4:
        insn.embedPostfix(0x4000);
        if (op.mode == M_REG) {
            if (op.reg != REG_R0)
                insn.setErrorIf(op, REGISTER_NOT_ALLOWED);
            break;
        }
        if (op.val.isZero())
            insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        /* Fall-through */
    case M_BIT0:
        if (op.val.overflow(15))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.embedPostfix((val16 & 0xF) << 6);
        break;
    case M_RTWP:
        // 0,1,2,4 are acceptable.
        if (op.val.isNegative() || val16 == 3 || val16 > 4)
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
    if (endOfLine(p)) {
        return OK;
    }

    if (p.expect('*')) {
        op.reg = parseRegName(p);
        if (op.reg == REG_UNDEF)
            return op.setError(UNKNOWN_OPERAND);
        op.mode = p.expect('+') ? M_INCR : M_IREG;
        scan = p;
        return OK;
    }
    if (p.expect('@')) {
        op.val = parseInteger(p, op);
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

    op.val = parseInteger(p, op);
    if (op.hasError())
        return op.getError();
    op.mode = M_IMM;
    scan = p;
    return OK;
}

Error AsmTms9900::defineFloatConstant(StrScanner &scan, AsmInsn &insn, ErrorAt &_error) const {
    ErrorAt error;
    do {
        scan.skipSpaces();
        ErrorAt exprErr;
        auto p = scan;
        const auto val = parseExpr(p, exprErr);
        if (!endOfLine(p) && *p != ',') {
            error.setErrorIf(scan, ILLEGAL_CONSTANT);
            break;
        }
        if (exprErr.hasError()) {
            error.setErrorIf(exprErr);
            break;
        }
#if defined(LIBASM_ASM_NOFLOAT)
        (void)val;
        exprErr.setErrorIf(scan, FLOAT_NOT_SUPPORTED);
        insn.emitUint32(0);
#else
        ibm_float32_t f32;
        exprErr.setErrorIf(scan, f32.set(val.getFloat()));
        exprErr.setErrorIf(scan, insn.emitUint32(f32.bits()));
#endif
        scan = p;
        if (error.setErrorIf(exprErr) == NO_MEMORY)
            break;
    } while (scan.skipSpaces().expect(','));
    return _error.setError(error);
}

Error AsmTms9900::processPseudo(StrScanner &scan, Insn &_insn) {
    AsmInsn insn(_insn);
    if (strcasecmp_P(insn.name(), TEXT_SINGLE) == 0)
        return defineFloatConstant(scan, insn, _insn);
    return Assembler::processPseudo(scan, _insn);
}

Error AsmTms9900::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    if (hasOperand(cpuType(), insn)) {
        if (parseOperand(scan, insn.srcOp) && insn.srcOp.hasError())
            return _insn.setError(insn.srcOp);
        if (scan.skipSpaces().expect(',')) {
            if (parseOperand(scan, insn.dstOp) && insn.dstOp.hasError())
                return _insn.setError(insn.dstOp);
        }
    }
    scan.skipSpaces();

    if (searchName(cpuType(), insn))
        return _insn.setError(insn.srcOp, insn);

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
