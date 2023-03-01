/*
 * Copyright 2023 Tadashi G. Takaoka
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

#include "asm_f3850.h"

#include <ctype.h>

namespace libasm {
namespace f3850 {

Error AsmF3850::parseOperand(StrScanner &scan, Operand &op) const {
    op.setAt(scan.skipSpaces());
    if (endOfLine(*scan))
        return OK;

    StrScanner p(scan);
    RegName reg = RegF3850::parseRegName(p);
    if (reg != REG_UNDEF) {
        if (uint8_t(reg) < uint8_t(REG_alias)) {
            op.mode = AddrMode(uint8_t(reg));
        } else if (reg == REG_J) {
            op.mode = M_J;
        } else {
            op.mode = M_REG;
        }
        op.val16 = uint8_t(reg) - uint8_t(REG_alias);
        scan = p;
        return OK;
    }

    Value val = parseExpr(p.skipSpaces(), op);
    if (parserError())
        return op.getError();
    op.val16 = val.getUnsigned();
    const int16_t v = val.isSigned() ? val.getSigned() : val.getUnsigned();
    if (v == 1) {
        op.mode = M_C1;
    } else if (v == 4) {
        op.mode = M_C4;
    } else if (v >= 0 && v < 8) {
        op.mode = M_IM3;
    } else if (v >= 0 && v < 16) {
        op.mode = M_IM4;
    } else if (!val.overflowUint8()) {
        op.mode = M_IM8;
    } else if (!val.overflowUint16()) {
        op.mode = M_ADDR;
    } else {
        op.setError(OVERFLOW_RANGE);
    }
    scan = p;
    return OK;
}

void AsmF3850::emitRelative(InsnF3850 &insn, const Operand &op) {
    const Config::uintptr_t base = insn.address() + 1;
    const Config::uintptr_t target = op.getError() ? base : op.val16;
    const Config::ptrdiff_t delta = target - base;
    if (overflowRel8(delta))
        setErrorIf(op, OPERAND_TOO_FAR);
    insn.emitOperand8(delta);
}

void AsmF3850::encodeOperand(InsnF3850 &insn, const Operand &op, AddrMode mode) {
    switch (mode) {
    case M_REG:
        if (op.val16 >= 16)
            setErrorIf(op, OVERFLOW_RANGE);
        insn.embed(op.val16 & 0xF);
        break;
    case M_C1:
        if (op.val16 != 1 && op.val16 != 4) {
            setErrorIf(op, OPERAND_NOT_ALLOWED);
        } else if (op.val16 == 4) {
            insn.setOpCode(insn.opCode() + 2);
        }
        break;
    case M_IM3:
        if (op.val16 >= 8)
            setErrorIf(op, OVERFLOW_RANGE);
        insn.embed(op.val16 & 7);
        break;
    case M_IM4:
        if (op.val16 >= 16)
            setErrorIf(op, OVERFLOW_RANGE);
        insn.embed(op.val16 & 0xF);
        break;
    case M_IM8:
        insn.emitOperand8(op.val16);
        break;
    case M_ADDR:
        insn.emitOperand16(op.val16);
        break;
    case M_REL:
        emitRelative(insn, op);
        break;
    default:
        break;
    }
}

Error AsmF3850::encodeImpl(StrScanner &scan, Insn &_insn) {
    InsnF3850 insn(_insn);
    insn.nameBuffer().text(_parser.readSymbol(scan));

    Operand op1, op2;
    if (parseOperand(scan, op1) && op1.hasError())
        return setError(op1);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, op2) && op2.hasError())
            return setError(op2);
        scan.skipSpaces();
    }
    if (!endOfLine(*scan))
        return setError(scan, GARBAGE_AT_END);
    setErrorIf(op1);
    setErrorIf(op2);

    insn.setAddrMode(op1.mode, op2.mode);
    const auto error = TableF3850::TABLE.searchName(insn);
    if (error)
        return setError(op1, error);

    encodeOperand(insn, op1, insn.mode1());
    encodeOperand(insn, op2, insn.mode2());
    insn.emitInsn();
    return setErrorIf(insn);
}

}  // namespace f3850
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4: