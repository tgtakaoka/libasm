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

Error AsmMos6502::encodeRelative(InsnMos6502 &insn, bool emitInsn) {
    Config::uintptr_t addr;
    if (getOperand(addr)) return getError();
    if (getError() == UNDEFINED_SYMBOL) addr = insn.address();
    const Config::uintptr_t base = insn.address() + (emitInsn ? 2 : 3);
    const Config::ptrdiff_t delta = addr - base;
    if (emitInsn) insn.emitInsn();
    if (delta >= 128 || delta < -128) return setError(OPERAND_TOO_FAR);
    insn.emitByte(static_cast<uint8_t>(delta));
    return checkLineEnd();
}

Error AsmMos6502::encodeZeroPageRelative(InsnMos6502 &insn) {
    if (*_scan == '<') _scan++;
    uint8_t zp;
    if (getOperand(zp)) return getError();
    const Error error = setError(getError());
    if (*_scan != ',') return setError(UNKNOWN_OPERAND);
    _scan++;
    insn.emitInsn();
    insn.emitByte(zp);
    encodeRelative(insn, /* emitInsn */ false);
    return setError(error ? error : getError());
}

Error AsmMos6502::selectMode(char modifier, Operand &op, AddrMode abs, AddrMode zp) {
    if (modifier == '>' || op.val16 >= 0x100) {
        op.mode = abs;
        return OK;
    }
    if (modifier == '<' || op.val16 < 0x100) {
        op.mode = zp;
        return OK;
    }
    return setError(OPERAND_NOT_ZP);
}

Error AsmMos6502::parseOperand(Operand &op) {
    op.resetError();
    const char *p = _scan;
    if (*p == '#') {            // #nn
        uint8_t val8;
        _scan = p + 1;
        if (getOperand(val8)) return getError();
        op.setError(getError());
        p = _scan;
        op.mode = IMMA;
        op.val16 = val8;
    } else if (_regs.compareRegName(p, REG_A)) { // A
        p += _regs.regNameLen(REG_A);
        op.mode = ACCM;
        op.setError(OK);
    } else {
        const char indir = (*p == '(') ? *p : 0;;
        if (indir) p = skipSpaces(p + 1);
        char modifier = *p;
        if (modifier == '<' || modifier == '>') p++;
        if (*p == '>') { modifier = '}'; p++; }
        _scan = p;
        if (getOperand(op.val16)) return getError();
        op.setError(getError());
        p = skipSpaces(_scan);
        if (*p == ',') {
            p = skipSpaces(p + 1);
            if (_regs.compareRegName(p, REG_X)) { // al,X abs,X zp,X (abs,X) (zp,X)
                p = skipSpaces(p + _regs.regNameLen(REG_X));
                if (indir == '(') {
                    if (*p != ')') return setError(MISSING_CLOSING_PAREN);
                    p++;
                    if (selectMode(modifier, op, ABS_IDX_IDIR, ZPG_IDX_IDIR))
                        return getError();
                    if (op.mode == ABS_IDX_IDIR && TableMos6502.is6502())
                        return setError(UNKNOWN_OPERAND);
                } else if (indir == 0) {
                    if (selectMode(modifier, op, ABS_IDX, ZPG_IDX))
                        return getError();
                } else return setError(UNKNOWN_OPERAND);
            } else if (_regs.compareRegName(p, REG_Y)) { // abs,Y zp,Y
                if (indir) return setError(UNKNOWN_OPERAND);
                p = skipSpaces(p + _regs.regNameLen(REG_Y));
                if (selectMode(modifier, op, ABS_IDY, ZPG_IDY))
                    return getError();
            } else return setError(UNKNOWN_OPERAND);
        } else if (indir) {
            if ((indir == '(' && *p != ')') || (indir == '[' && *p != ']'))
                return setError(MISSING_CLOSING_PAREN);
            p = skipSpaces(p + 1);
            if (*p == ',') {
                p = skipSpaces(p + 1);
                if (_regs.compareRegName(p, REG_Y)) { // (zp),Y
                    p += _regs.regNameLen(REG_Y);
                    if (modifier == '<' || op.val16 < 0x100) {
                        op.mode = ZPG_IDIR_IDY;
                    } else return setError(OPERAND_NOT_ZP);
                } else return setError(UNKNOWN_OPERAND);
            } else {            // (abs), (zp)
                if (selectMode(modifier, op, ABS_IDIR, ZPG_IDIR))
                    return getError();
            }
        } else {                // abs, zp
            if (indir) return setError(UNKNOWN_OPERAND);
            if (selectMode(modifier, op, ABS, ZPG))
                return getError();
        }
    }
    _scan = skipSpaces(p);
    return setError(OK);
}

Error AsmMos6502::encode(Insn &_insn) {
    InsnMos6502 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);
    if (TableMos6502.searchName(insn))
        return setError(UNKNOWN_INSTRUCTION);
    _scan = skipSpaces(endName);

    switch (insn.addrMode()) {
    case IMPL:
        insn.emitInsn();
        return checkLineEnd();
    case REL:
        return encodeRelative(insn, /* emitInsn */ true);
    case ZPG_REL:
        return encodeZeroPageRelative(insn);
    default:
        break;
    }

    Operand op;
    if (parseOperand(op)) return setError(op);
    insn.setAddrMode(op.mode);
    if (TableMos6502.searchNameAndAddrMode(insn))
        return setError(UNKNOWN_INSTRUCTION);
    switch (insn.addrMode()) {
    case ACCM:
        insn.emitInsn();
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
    setError(op);

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
