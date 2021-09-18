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

#include "table_i8080.h"

namespace libasm {
namespace i8080 {

Error AsmI8080::encodeOperand(InsnI8080 &insn, const Operand &op, AddrMode mode) {
    switch (mode) {
    case M_IOA:
        if (op.val16 >= 0x100)
            return setError(OVERFLOW_RANGE);
        /* Fall-through */
    case M_IM8:
        if (overflowUint8(op.val16))
            return setError(OVERFLOW_RANGE);
        insn.emitOperand8(op.val16);
        return OK;
    case M_IM16:
    case M_ABS:
        insn.emitOperand16(op.val16);
        return OK;
    case M_PTR:
        insn.embed(RegI8080::encodePointerReg(op.reg) << 4);
        return OK;
    case M_STK:
        insn.embed(RegI8080::encodeStackReg(op.reg) << 4);
        return OK;
    case M_IDX:
        insn.embed(RegI8080::encodeIndexReg(op.reg) << 4);
        return OK;
    case M_REG:
        insn.embed(RegI8080::encodeDataReg(op.reg));
        return OK;
    case M_DST:
        insn.embed(RegI8080::encodeDataReg(op.reg) << 3);
        return OK;
    case M_VEC:
        if (op.val16 >= 8)
            return setError(OVERFLOW_RANGE);
        insn.embed((op.val16 & 7) << 3);
        return OK;
    default:
        return OK;
    }
}

Error AsmI8080::parseOperand(const char *scan, Operand &op) {
    const char *p = skipSpaces(scan);
    _scan = p;
    if (endOfLine(p))
        return OK;

    op.reg = RegI8080::parseRegName(p);
    if (op.reg != REG_UNDEF) {
        _scan = p + RegI8080::regNameLen(op.reg);
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
        return OK;
    }
    op.val16 = parseExpr16(p);
    if (parserError())
        return getError();
    op.setError(getError());
    op.mode = M_IM16;
    return OK;
}

Error AsmI8080::encode(Insn &_insn) {
    InsnI8080 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);

    Operand dstOp, srcOp;
    if (parseOperand(endName, dstOp))
        return getError();
    const char *p = skipSpaces(_scan);
    if (*p == ',') {
        if (parseOperand(p + 1, srcOp))
            return getError();
        p = skipSpaces(_scan);
    }
    if (!endOfLine(p))
        return setError(GARBAGE_AT_END);
    setErrorIf(dstOp.getError());
    setErrorIf(srcOp.getError());

    insn.setAddrMode(dstOp.mode, srcOp.mode);
    if (TableI8080.searchName(insn))
        return setError(TableI8080.getError());

    const AddrMode dst = insn.dstMode();
    if (dst != M_NO && encodeOperand(insn, dstOp, dst))
        return getError();
    const AddrMode src = insn.srcMode();
    if (src != M_NO && encodeOperand(insn, srcOp, src))
        return getError();
    insn.emitInsn();
    return getError();
}

}  // namespace i8080
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
