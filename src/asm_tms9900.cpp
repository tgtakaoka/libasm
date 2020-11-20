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

namespace libasm {
namespace tms9900 {

Error AsmTms9900::encodeRelative(InsnTms9900 &insn, const Operand &op) {
    const Config::uintptr_t base = insn.address() + 2;
    const Config::uintptr_t target = op.getError() ? base : op.val16;
    ;
    if (target % 2)
        return setError(OPERAND_NOT_ALIGNED);
    const Config::ptrdiff_t delta = (target - base) >> 1;
    if (delta >= 128 || delta < -128)
        return setError(OPERAND_TOO_FAR);
    insn.embed(static_cast<uint8_t>(delta));
    return OK;
}

Error AsmTms9900::encodeCruOffset(InsnTms9900 &insn, const Operand &op) {
    const int16_t offset = static_cast<int16_t>(op.val16);
    if (offset < -128 || offset >= 128)
        return setError(OVERFLOW_RANGE);
    insn.embed(offset & 0xFF);
    return OK;
}

Error AsmTms9900::encodeModeReg(
        InsnTms9900 &insn, const Operand &op, AddrMode mode) {
    if (mode == M_SRC2 && insn.dstMode() == M_BIT2 && op.mode == M_INCR)
        return setError(OPERAND_NOT_ALLOWED);
    Config::opcode_t opc = RegTms9900::encodeRegNumber(op.reg);
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
        insn.emitOperand16(op.val16);
        break;
    default:  // M_INDX
        if (op.reg == REG_R0)
            return setError(REGISTER_NOT_ALLOWED);
        opc |= (2 << 4);
        insn.emitOperand16(op.val16);
        break;
    }
    switch (mode) {
    case M_SRC:
        insn.embed(opc);
        break;
    case M_SRC2:
        insn.embedPost(opc);
        break;
    case M_DST:
        insn.embed(opc << 6);
        break;
    default:
        insn.embedPost(opc << 6);
        break;
    }
    return OK;
}

Error AsmTms9900::encodeOperand(
        InsnTms9900 &insn, const Operand &op, AddrMode mode) {
    uint16_t val16 = op.val16;
    switch (mode) {
    case M_IMM:
        insn.emitOperand16(op.val16);
        return OK;
    case M_REG:
        insn.embed(RegTms9900::encodeRegNumber(op.reg));
        return OK;
    case M_DREG:
        insn.embed(RegTms9900::encodeRegNumber(op.reg) << 6);
        return OK;
    case M_DST2:
        insn.embedPost(0x4000);
        /* Fall-through */
    case M_SRC:
    case M_SRC2:
    case M_DST:
        return encodeModeReg(insn, op, mode);
    case M_REL:
        return encodeRelative(insn, op);
    case M_CRU:
        return encodeCruOffset(insn, op);
    case M_CNT:
        if (val16 == 0 && !op.getError())
            return setError(OPERAND_NOT_ALLOWED);
        if (val16 == 16)
            val16 = 0;
        /* Fall-through */
    case M_XOP:
        if (val16 >= 16)
            return setError(OVERFLOW_RANGE);
        insn.embed(val16 << 6);
        return OK;
    case M_SCNT:
        if (op.mode == M_REG) {
            if (op.reg == REG_R0)
                return OK;
            return setError(REGISTER_NOT_ALLOWED);
        }
        if (val16 == 0 && !op.getError())
            return setError(OPERAND_NOT_ALLOWED);
        if (val16 >= 16)
            return setError(OVERFLOW_RANGE);
        insn.embed(val16 << 4);
        return OK;
    case M_CNT2:
        insn.embedPost(0x4000);
        if (op.mode == M_REG) {
            if (op.reg == REG_R0)
                return OK;
            return setError(REGISTER_NOT_ALLOWED);
        }
        if (val16 == 0 && !op.getError())
            return setError(OPERAND_NOT_ALLOWED);
        /* Fall-through */
    case M_BIT2:
        if (val16 >= 16)
            return setError(OVERFLOW_RANGE);
        insn.embedPost(val16 << 6);
        return OK;
    case M_RTWP:
        if ((val16 == 0 || val16 == 1 || val16 == 2 || val16 == 4) &&
                op.getError() == OK) {
            insn.embed(val16 & 7);
            return OK;
        }
        return setError(OPERAND_NOT_ALLOWED);
    default:
        return OK;
    }
}

Error AsmTms9900::parseOperand(const char *scan, Operand &op) {
    const char *p = skipSpaces(scan);
    _scan = p;
    if (endOfLine(p))
        return OK;

    if (*p == '*') {
        p++;
        op.reg = RegTms9900::parseRegName(p);
        if (op.reg == REG_UNDEF)
            return setError(UNKNOWN_OPERAND);
        p += RegTms9900::regNameLen(op.reg);
        if (*p == '+') {
            p++;
            op.mode = M_INCR;
        } else {
            op.mode = M_IREG;
        }
        _scan = p;
        return OK;
    }
    if (*p == '@') {
        op.val16 = parseExpr16(p + 1);
        if (parserError())
            return getError();
        op.setError(getError());
        p = skipSpaces(_scan);
        if (*p == '(') {
            p = skipSpaces(p + 1);
            op.reg = RegTms9900::parseRegName(p);
            if (op.reg == REG_UNDEF)
                return setError(UNKNOWN_OPERAND);
            if (op.reg == REG_R0)
                return setError(REGISTER_NOT_ALLOWED);
            p += RegTms9900::regNameLen(op.reg);
            p = skipSpaces(p);
            if (*p != ')')
                return setError(MISSING_CLOSING_PAREN);
            p++;
            op.mode = M_INDX;
        } else {
            op.mode = M_SYBL;
        }
        _scan = p;
        return OK;
    }
    op.reg = RegTms9900::parseRegName(p);
    if (op.reg != REG_UNDEF) {
        _scan = p + RegTms9900::regNameLen(op.reg);
        op.mode = M_REG;
        return OK;
    }

    op.val16 = parseExpr16(p);
    if (parserError())
        return getError();
    op.setError(getError());
    op.mode = M_IMM;
    return OK;
}

Error AsmTms9900::encode(Insn &_insn) {
    InsnTms9900 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);

    Operand srcOp, dstOp;
    if (parseOperand(endName, srcOp))
        return getError();
    const char *p = skipSpaces(_scan);
    if (*p == ',') {
        if (parseOperand(p + 1, dstOp))
            return getError();
        p = skipSpaces(_scan);
    }
    if (!endOfLine(p))
        return setError(GARBAGE_AT_END);
    setErrorIf(dstOp.getError());
    setErrorIf(srcOp.getError());

    insn.setAddrMode(srcOp.mode, dstOp.mode);
    if (TableTms9900.searchName(insn))
        return setError(TableTms9900.getError());

    const AddrMode src = insn.srcMode();
    if (src != M_NO && encodeOperand(insn, srcOp, src))
        return getError();
    const AddrMode dst = insn.dstMode();
    if (dst != M_NO && encodeOperand(insn, dstOp, dst)) {
        insn.resetAddress(insn.address());
        return getError();
    }
    insn.emitInsn();
    return getError();
}

}  // namespace tms9900
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
