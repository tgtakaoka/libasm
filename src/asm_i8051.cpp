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

#include <stdio.h>

namespace libasm {
namespace i8051 {

Error AsmI8051::parseOperand(Operand &op) {
    setError(op.setOK());
    op.mode = NONE;

    const char *p = _scan;
    if (endOfLine(p)) return OK;

    if (*p == '#') {
        _scan = skipSpaces(p + 1);
        if (getOperand(op.val16)) return getError();
        op.setError(getError());
        op.mode = IMM16;
        return OK;
    }

    const bool indir = (*p == '@');
    if (indir) {
        p++;
        if (isspace(*p)) return setError(UNKNOWN_OPERAND);
    }

    op.reg = _regs.parseRegister(p);
    if (op.reg != REG_UNDEF) {
        p += _regs.regNameLen(op.reg);
        if (indir && op.reg == REG_A && *p == '+') {
            p++;
            const RegName base = _regs.parseRegister(p);
            if (base == REG_DPTR || base == REG_PC) {
                _scan = p + _regs.regNameLen(base);
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
        if (op.reg == REG_A) {
            op.mode = AREG;
        } else if (_regs.isRReg(op.reg)) {
            op.mode = RREG;
        } else if (op.reg == REG_C) {
            op.mode = CREG;
        } else if (op.reg == REG_DPTR) {
            op.mode = DREG;
        } else if (op.reg == REG_AB) {
            op.mode = ABREG;
        } else {
            return setError(UNKNOWN_OPERAND);
        }
        return OK;
    }
    if (indir) return setError(UNKNOWN_OPERAND);

    const bool bitNot = (*p == '/');
    if (bitNot) p = skipSpaces(p + 1);

    _scan = p;
    if (getOperand(op.val16)) return getError();
    op.setError(getError());
    p = _scan;
    if (*p == '.') {
        if (op.getError()) op.val16 = 0x20;
        uint8_t bitNo;
        _scan = p + 1;
        if (getOperand(bitNo)) return getError();
        if (getError()) {
            op.setErrorIf(getError());
            bitNo = 0;
        }
        if (bitNo >= 8) return setError(ILLEGAL_BIT_NUMBER);
        if ((op.val16 & ~0x0F) == 0x20 || (op.val16 & ~0x78) == 0x80) {
            op.mode = bitNot ? NOTAD : BITAD;
            if ((op.val16 & 0x80) == 0)
                op.val16 = (op.val16 & 0xF) << 3;
            op.val16 |= bitNo;
            return OK;
        }
        return setError(NOT_BIT_ADDRESSABLE);
    }
    op.mode = bitNot ? NOTAD : ADR16;
    return OK;
}

Error AsmI8051::encodeOperand(
    InsnI8051 &insn, const AddrMode mode, const Operand &op) {
    Error error = OK;
    if (mode == REL) {
        uint8_t len = insn.length();
        if (len == 0) len = 1;
        const Config::uintptr_t base = insn.address() + len + 1;
        const Config::uintptr_t target = op.getError() ? base : op.val16;
        const Config::ptrdiff_t delta = target - base;
        if (delta < -128 || delta >= 128) {
            error = OPERAND_TOO_FAR;
        } else {
            insn.emitOperand(static_cast<uint8_t>(delta));
        }
    }
    if (mode == RREG || mode == IDIRR) {
        insn.embed(_regs.encodeRReg(op.reg));
        return OK;
    }
    if (mode == ADR8) {
        if (op.val16 >= 0x100) {
            error = OVERFLOW_RANGE;
        } else {
            insn.emitOperand(static_cast<uint8_t>(op.val16));
        }
    }
    if (mode == IMM8) {
        const int16_t val = static_cast<int16_t>(op.val16);
        if (val < -128 || val >= 256) {
            error = OVERFLOW_RANGE;
        } else {
            insn.emitOperand(static_cast<uint8_t>(op.val16));
        }
    }
    if (mode == ADR11) {
        const Config::uintptr_t base = insn.address() + 2;
        const Config::uintptr_t target =
            op.getError() ? (base & ~0x7FF) : op.val16;
        if ((base & ~0x7FF) != (target & ~0x7FF)) {
            error = OPERAND_TOO_FAR;
        } else {
            insn.embed(static_cast<uint8_t>((target & 0x700) >> 3));
            insn.emitOperand(static_cast<uint8_t>(target));
        }
    }
    if (mode == ADR16 || mode == IMM16) {
        insn.emitOperand(op.val16);
        return OK;
    }
    if (mode == BITAD || mode == NOTAD) {
        if (op.val16 >= 0x100) {
            error = NOT_BIT_ADDRESSABLE;
        } else {
            insn.emitOperand(static_cast<uint8_t>(op.val16));
        }
    }
    if (error) {
        insn.resetAddress(insn.address());
        return setError(error);
    }
    return OK;
}

Error AsmI8051::encode(Insn &_insn) {
    InsnI8051 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);
    _scan = skipSpaces(endName);

    Operand dstOp, srcOp, extOp;
    srcOp.setOK(); srcOp.mode = NONE;
    extOp.setOK(); extOp.mode = NONE;
    if (parseOperand(dstOp)) return getError();
    const char *p = skipSpaces(_scan);
    if (*p == ',') {
        _scan = skipSpaces(p + 1);
        if (parseOperand(srcOp)) return getError();
        p = skipSpaces(_scan);
        if (*p == ',') {
            _scan = skipSpaces(p + 1);
            if (parseOperand(extOp)) return getError();
            p = skipSpaces(_scan);
        }
    }
    if (!endOfLine(p)) return setError(GARBAGE_AT_END);
    setError(dstOp.getError());
    setErrorIf(srcOp.getError());
    setErrorIf(extOp.getError());

    insn.setAddrMode(dstOp.mode, srcOp.mode, extOp.mode);
    if (TableI8051.searchName(insn))
        return setError(UNKNOWN_INSTRUCTION);
    const AddrMode dst = insn.dstMode();
    const AddrMode src = insn.srcMode();
    const AddrMode ext = insn.extMode();
    if (dst == ADR8 && src == ADR8) {
        if (encodeOperand(insn, src, srcOp)) return getError();
        if (encodeOperand(insn, dst, dstOp)) return getError();
    } else {
        if (dst && encodeOperand(insn, dst, dstOp)) return getError();
        if (src && encodeOperand(insn, src, srcOp)) return getError();
        if (ext && encodeOperand(insn, ext, extOp)) return getError();
    }
    insn.emitInsn();
    return getError();
}

} // namespace i8051
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
