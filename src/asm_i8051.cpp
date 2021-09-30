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

#include "table_i8051.h"

namespace libasm {
namespace i8051 {

Error AsmI8051::parseOperand(const char *scan, Operand &op) {
    const char *p = skipSpaces(scan);
    _scan = p;
    if (endOfLine(p))
        return OK;

    if (*p == '#') {
        op.val16 = parseExpr16(p + 1);
        if (parserError())
            return getError();
        op.setError(getError());
        op.mode = IMM16;
        return OK;
    }

    const bool indir = (*p == '@');
    if (indir) {
        p++;
        if (isspace(*p))
            return setError(UNKNOWN_OPERAND);
    }

    op.reg = RegI8051::parseRegName(p);
    if (op.reg != REG_UNDEF) {
        p += RegI8051::regNameLen(op.reg);
        if (indir && op.reg == REG_A && *p == '+') {
            p++;
            const RegName base = RegI8051::parseRegName(p);
            if (base == REG_DPTR || base == REG_PC) {
                _scan = p + RegI8051::regNameLen(base);
                op.mode = (base == REG_DPTR) ? INDXD : INDXP;
                return OK;
            }
            return setError(UNKNOWN_OPERAND);
        }
        _scan = p;
        if (indir) {
            if (op.reg == REG_R0 || op.reg == REG_R1) {
                op.mode = IDIRR;
                return OK;
            }
            if (op.reg == REG_DPTR) {
                op.mode = IDIRD;
                return OK;
            }
            return setError(UNKNOWN_OPERAND);
        }
        if (RegI8051::isRReg(op.reg)) {
            op.mode = RREG;
            return OK;
        }
        switch (op.reg) {
        case REG_A:
            op.mode = AREG;
            return OK;
        case REG_C:
            op.mode = CREG;
            return OK;
        case REG_DPTR:
            op.mode = DREG;
            return OK;
        case REG_AB:
            op.mode = ABREG;
            return OK;
        default:
            return setError(UNKNOWN_OPERAND);
        }
    }
    if (indir)
        return setError(UNKNOWN_OPERAND);

    const bool bitNot = (*p == '/');
    if (bitNot)
        p = skipSpaces(p + 1);

    op.val16 = parseExpr16(p);
    if (parserError())
        return getError();
    op.setError(getError());
    p = _scan;
    if (*p == '.') {
        if (op.getError())
            op.val16 = 0x20;
        uint16_t bitNo = parseExpr16(p + 1);
        if (parserError())
            return getError();
        op.setErrorIf(getError());
        if (bitNo >= 8)
            return setError(ILLEGAL_BIT_NUMBER);
        uint16_t val16 = op.val16;
        if ((val16 & ~0x0F) == 0x20 || (val16 & ~0x78) == 0x80) {
            op.mode = bitNot ? NOTAD : BITAD;
            if ((val16 & 0x80) == 0)
                op.val16 = (val16 & 0xF) << 3;
            op.val16 |= bitNo;
            return OK;
        }
        return setError(NOT_BIT_ADDRESSABLE);
    }
    op.mode = bitNot ? NOTAD : ADR16;
    return OK;
}

Error AsmI8051::encodeOperand(InsnI8051 &insn, const AddrMode mode, const Operand &op) {
    switch (mode) {
    case REL: {
        uint8_t len = insn.length();
        if (len == 0)
            len = 1;
        const Config::uintptr_t base = insn.address() + len + 1;
        const Config::uintptr_t target = op.getError() ? base : op.val16;
        const Config::ptrdiff_t delta = target - base;
        if (overflowRel8(delta))
            return setError(OPERAND_TOO_FAR);
        insn.emitOperand8(delta);
        return OK;
    }
    case RREG:
    case IDIRR:
        insn.embed(RegI8051::encodeRReg(op.reg));
        return OK;
    case ADR8:
        if (op.val16 >= 0x100)
            return setError(OVERFLOW_RANGE);
        insn.emitOperand8(op.val16);
        return OK;
    case IMM8: {
        if (overflowUint8(op.val16))
            return setError(OVERFLOW_RANGE);
        insn.emitOperand8(op.val16);
        return OK;
    }
    case ADR11: {
        const Config::uintptr_t base = insn.address() + 2;
        const Config::uintptr_t target = op.getError() ? (base & ~0x7FF) : op.val16;
        if ((base & ~0x7FF) != (target & ~0x7FF))
            return setError(OPERAND_TOO_FAR);
        insn.embed((target & 0x700) >> 3);
        insn.emitOperand8(target);
        return OK;
    }
    case ADR16:
    case IMM16:
        insn.emitOperand16(op.val16);
        return OK;
    case BITAD:
    case NOTAD:
        if (op.val16 >= 0x100)
            return setError(NOT_BIT_ADDRESSABLE);
        insn.emitOperand8(op.val16);
        return OK;
    default:
        return OK;
    }
}

Error AsmI8051::encode(Insn &_insn) {
    InsnI8051 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);

    Operand dstOp, srcOp, extOp;
    if (parseOperand(endName, dstOp))
        return getError();
    const char *p = skipSpaces(_scan);
    if (*p == ',') {
        if (parseOperand(p + 1, srcOp))
            return getError();
        p = skipSpaces(_scan);
    }
    if (*p == ',') {
        if (parseOperand(p + 1, extOp))
            return getError();
        p = skipSpaces(_scan);
    }
    if (!endOfLine(p))
        return setError(GARBAGE_AT_END);
    setError(dstOp.getError());
    setErrorIf(srcOp.getError());
    setErrorIf(extOp.getError());

    insn.setAddrMode(dstOp.mode, srcOp.mode, extOp.mode);
    if (TableI8051.searchName(insn))
        return setError(TableI8051.getError());

    const AddrMode dst = insn.dstMode();
    const AddrMode src = insn.srcMode();
    const AddrMode ext = insn.extMode();
    if (dst == ADR8 && src == ADR8) {
        if (encodeOperand(insn, src, srcOp))
            return getError();
        if (encodeOperand(insn, dst, dstOp))
            return getError();
    } else {
        if (dst && encodeOperand(insn, dst, dstOp))
            return getError();
        if (src && encodeOperand(insn, src, srcOp)) {
            insn.reset();
            return getError();
        }
        if (ext && encodeOperand(insn, ext, extOp)) {
            insn.reset();
            return getError();
        }
    }
    insn.emitInsn();
    return getError();
}

}  // namespace i8051
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
