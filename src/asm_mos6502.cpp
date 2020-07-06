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

#include "asm_mos6502.h"
#include "table_mos6502.h"

namespace libasm {
namespace mos6502 {

Error AsmMos6502::encodeRelative(InsnMos6502 &insn, const Operand &op) {
    const Config::uintptr_t base = insn.address() + 2;
    const Config::uintptr_t target = op.getError() ? base : op.val16;
    const Config::ptrdiff_t delta = target - base;
    if (delta >= 128 || delta < -128) return setError(OPERAND_TOO_FAR);
    insn.emitInsn();
    insn.emitByte(static_cast<uint8_t>(delta));
    return OK;
}

Error AsmMos6502::encodeZeroPageRelative(
    InsnMos6502 &insn, const Operand &op, const Operand &extra) {
    const Config::uintptr_t base = insn.address() + 3;
    const Config::uintptr_t target =
        extra.getError() ? insn.address() : extra.val16;
    const Config::ptrdiff_t delta = target - base;
    if (delta >= 128 || delta < -128) return setError(OPERAND_TOO_FAR);
    insn.emitInsn();
    insn.emitByte(static_cast<uint8_t>(op.val16));
    insn.emitByte(static_cast<uint8_t>(delta));
    return OK;
}

Error AsmMos6502::selectMode(
    char size, Operand &op, AddrMode abs, AddrMode zp) {
    if (size == '>') {
        op.mode = abs;
    } else if (size == '<') {
        op.mode = zp;
    } else {
        op.mode = (op.val16 < 0x100) ? zp : abs;
    }
    if (endOfLine(skipSpaces(_scan))) return OK;
    return setError(UNKNOWN_OPERAND);
}

Error AsmMos6502::parseOperand(Operand &op, Operand &extra) {
    op.resetError();
    extra.resetError();
    const char *p = _scan;

    if (endOfLine(p)) {
        op.mode = IMPL;
        return OK;
    }
    if (*p == '#') {            // #nn
        _scan = p + 1;
        if (getOperand(op.val16)) return getError();
        op.setError(getError());
        op.mode = IMMA;
        return OK;
    }
    if (_regs.compareRegName(p, REG_A)) { // A
        _scan = p + _regs.regNameLen(REG_A);
        op.mode = ACCM;
        return OK;
    }

    const char indir = (*p == '(') ? ')' : 0;
    if (indir) p = skipSpaces(p + 1);
    const char size = (*p == '>' || *p == '<') ? *p++ : 0;
    _scan = p;
    if (getOperand(op.val16)) return getError();
    op.setError(getError());
    p = skipSpaces(_scan);
    if (*p == ',') {
        p = skipSpaces(p + 1);
        if (_regs.compareRegName(p, REG_X)) { // abs,X zp,X (abs,X) (zp,X)
            p = skipSpaces(p + _regs.regNameLen(REG_X));
            if (indir) {
                if (*p == ')') {
                    _scan = p + 1;
                    return selectMode(size, op, ABS_IDX_IDIR, ZPG_IDX_IDIR);
                }
                return setError(MISSING_CLOSING_PAREN);
            }
            _scan = p;
            return selectMode(size, op, ABS_IDX, ZPG_IDX);
        }
        if (_regs.compareRegName(p, REG_Y)) { // abs,Y zp,Y
            if (indir) return setError(UNKNOWN_OPERAND);
            _scan = p + _regs.regNameLen(REG_Y);
            return selectMode(size, op, ABS_IDY, ZPG_IDY);
        }
        if (indir) return setError(UNKNOWN_OPERAND);
        _scan = p;
        if (getOperand(extra.val16)) return getError();
        extra.setError(getError());
        op.mode = ZPG_REL;
        return OK;
    }
    if (indir) {
        if (*p != indir) return setError(MISSING_CLOSING_PAREN);
        p = skipSpaces(p + 1);
        if (*p == ',') {        // (zp),Y
            p = skipSpaces(p + 1);
            if (_regs.compareRegName(p, REG_Y)) {
                _scan = p + _regs.regNameLen(REG_Y);
                op.mode = ZPG_IDIR_IDY;
                return OK;
            }
            return setError(UNKNOWN_OPERAND);
        }
        // (abs) (zp)
        _scan = p;
        return selectMode(size, op, ABS_IDIR, ZPG_IDIR);
    }
    // abs zp
    _scan = p;
    return selectMode(size, op, ABS, ZPG);
}

Error AsmMos6502::encode(Insn &_insn) {
    InsnMos6502 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);
    _scan = skipSpaces(endName);
    Operand op, extra;
    if (parseOperand(op, extra)) return getError();
    insn.setAddrMode(op.mode);
    if (TableMos6502.searchName(insn))
        return setError(TableMos6502.getError());
    switch (insn.addrMode()) {
    case IMPL:
    case ACCM:
        insn.emitInsn();
        break;
    case REL:
        encodeRelative(insn, op);
        break;
    case ZPG_REL:
        encodeZeroPageRelative(insn, op, extra);
        break;
    case IMMA:
    case IMMX:
    case ZPG:
    case ZPG_IDX:
    case ZPG_IDY:
    case ZPG_IDX_IDIR:
    case ZPG_IDIR_IDY:
    case ZPG_IDIR:
        insn.emitInsn();
        insn.emitByte(static_cast<uint8_t>(op.val16));
        break;
    case ABS:
    case ABS_IDX:
    case ABS_IDY:
    case ABS_IDIR:
    case ABS_IDX_IDIR:
        insn.emitInsn();
        insn.emitUint16(static_cast<uint16_t>(op.val16));
        break;
    default:
        return setError(INTERNAL_ERROR);
    }
    setErrorIf(op.getError());

    return checkLineEnd();
}

} // namespace mos6502
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
