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

#include "asm_i8080.h"

#include "reg_i8080.h"
#include "table_i8080.h"

namespace libasm {
namespace i8080 {

using namespace reg;

struct AsmI8080::Operand final : ErrorAt {
    AddrMode mode;
    RegName reg;
    uint16_t val16;
    Operand() : mode(M_NONE), reg(REG_UNDEF), val16(0) {}
};

AsmI8080::AsmI8080()
    : Assembler(nullptr, _number, _comment, _symbol, _letter, _location), Config(TABLE) {
    reset();
}

void AsmI8080::encodeOperand(InsnI8080 &insn, const Operand &op, AddrMode mode) {
    switch (mode) {
    case M_IOA:
        if (op.val16 >= 0x100)
            setErrorIf(op, OVERFLOW_RANGE);
        /* Fall-through */
    case M_IM8:
        if (overflowUint8(op.val16))
            setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val16);
        return;
    case M_IM16:
    case M_ABS:
        insn.emitOperand16(op.val16);
        return;
    case M_PTR:
        insn.embed(encodePointerReg(op.reg) << 4);
        return;
    case M_STK:
        insn.embed(encodeStackReg(op.reg) << 4);
        return;
    case M_IDX:
        insn.embed(encodeIndexReg(op.reg) << 4);
        return;
    case M_REG:
        insn.embed(encodeDataReg(op.reg));
        return;
    case M_DST:
        insn.embed(encodeDataReg(op.reg) << 3);
        return;
    case M_VEC:
        if (op.val16 >= 8)
            setErrorIf(op, OVERFLOW_RANGE);
        insn.embed((op.val16 & 7) << 3);
        return;
    default:
        return;
    }
}

Error AsmI8080::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    op.reg = parseRegName(p);
    if (op.reg != REG_UNDEF) {
        switch (op.reg) {
        case REG_H:
            op.mode = M_REGH;
            break;
        case REG_SP:
            op.mode = M_PTR;
            break;
        case REG_PSW:
            op.mode = M_STK;
            break;
        case REG_B:
        case REG_D:
            op.mode = M_IDX;
            break;
        default:
            op.mode = M_REG;
            break;
        }
        scan = p;
        return OK;
    }
    op.val16 = parseExpr16(p, op);
    if (op.hasError())
        return op.getError();
    op.mode = M_IM16;
    scan = p;
    return OK;
}

Error AsmI8080::encodeImpl(StrScanner &scan, Insn &_insn) {
    InsnI8080 insn(_insn);
    Operand dstOp, srcOp;
    if (parseOperand(scan, dstOp) && dstOp.hasError())
        return setError(dstOp);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, srcOp) && srcOp.hasError())
            return setError(srcOp);
        scan.skipSpaces();
    }
    if (!endOfLine(scan))
        return setError(GARBAGE_AT_END);
    setErrorIf(dstOp);
    setErrorIf(srcOp);

    insn.setAddrMode(dstOp.mode, srcOp.mode);
    const auto error = TABLE.searchName(cpuType(), insn);
    if (error)
        return setError(dstOp, error);

    encodeOperand(insn, dstOp, insn.dst());
    encodeOperand(insn, srcOp, insn.src());
    insn.emitInsn();
    return setErrorIf(insn);
}

}  // namespace i8080
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
