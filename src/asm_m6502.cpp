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

#include "asm_m6502.h"
#include "table_m6502.h"

namespace libasm {
namespace m6502 {

Error AsmM6502::encodeLongRelative(Insn &insn) {
    uint32_t addr;
    if (getOperand32(addr)) return getError();
    if (getError() == UNDEFINED_SYMBOL) addr = insn.address();
    const uint32_t base = insn.address() + 3;
    const int32_t delta = addr - base;
    insn.emitInsn();
    if (delta >= 32768L || delta < -32768L) return setError(OPERAND_TOO_FAR);
    insn.emitUint16(static_cast<uint16_t>(delta));
    return checkLineEnd();
}

Error AsmM6502::encodeRelative(Insn &insn, bool emitInsn) {
    target::uintptr_t addr;
    if (getOperand16(addr)) return getError();
    if (getError() == UNDEFINED_SYMBOL) addr = insn.address();
    const target::uintptr_t base = insn.address() + (emitInsn ? 2 : 3);
    const target::ptrdiff_t delta = addr - base;
    if (emitInsn) insn.emitInsn();
    if (delta >= 128 || delta < -128) return setError(OPERAND_TOO_FAR);
    insn.emitByte(static_cast<uint8_t>(delta));
    return checkLineEnd();
}

Error AsmM6502::encodeZeroPageRelative(Insn &insn) {
    if (*_scan == '<') _scan++;
    uint8_t zp;
    if (getOperand8(zp)) return getError();
    const Error error = setError(getError());
    if (*_scan != ',') return setError(UNKNOWN_OPERAND);
    _scan++;
    insn.emitInsn();
    insn.emitByte(zp);
    encodeRelative(insn, /* emitInsn */ false);
    return setError(error ? error : getError());
}

Error AsmM6502::encodeBlockMove(Insn &insn) {
    uint8_t srcpg, dstpg;
    if (getOperand8(srcpg)) return getError();
    Error error = getError();
    if (*_scan != ',') return setError(UNKNOWN_OPERAND);
    _scan++;
    if (getOperand8(dstpg)) return getError();
    if (getError()) error = getError();
    insn.emitInsn();
    insn.emitByte(srcpg);
    insn.emitByte(dstpg);
    return setError(error ? error : getError());
};

Error AsmM6502::selectMode(char modifier, Operand &op, AddrMode labs, AddrMode abs, AddrMode zp) {
    if (modifier == '}' || op.val32 >= 0x10000) {
        if (labs == IMPL) return setError(OPERAND_NOT_ZP);
        op.mode = labs;
        return OK;
    }
    if (modifier == '>' || op.val32 >= 0x100) {
        op.mode = abs;
        return OK;
    }
    if (modifier == '<' || op.val32 < 0x100) {
        op.mode = zp;
        return OK;
    }
    return setError(OPERAND_NOT_ZP);
}

Error AsmM6502::parseOperand(Operand &op) {
    op.resetError();
    const char *p = _scan;
    if (*p == '#') {            // #nn
        uint8_t val8;
        _scan = p + 1;
        if (getOperand8(val8)) return getError();
        op.setError(getError());
        p = _scan;
        op.mode = IMM;
        op.val32 = val8;
    } else if (_regs.compareRegName(p, REG_A)) { // A
        p += _regs.regNameLen(REG_A);
        op.mode = ACCM;
        op.setError(OK);
    } else {
        const char indir = (*p == '(' || *p == '[') ? *p : 0;;
        if (indir) p = skipSpaces(p + 1);
        char modifier = *p;
        if (modifier == '<' || modifier == '>') p++;
        if (*p == '>') { modifier = '}'; p++; }
        _scan = p;
        if (getOperand32(op.val32)) return getError();
        op.setError(getError());
        p = skipSpaces(_scan);
        if (*p == ',') {
            p = skipSpaces(p + 1);
            if (_regs.compareRegName(p, REG_X)) { // al,X abs,X zp,X (abs,X) (zp,X)
                p = skipSpaces(p + _regs.regNameLen(REG_X));
                if (indir == '(') {
                    if (*p != ')') return setError(MISSING_CLOSING_PAREN);
                    p++;
                    if (selectMode(modifier, op, IMPL, ABS_IDX_IDIR, ZPG_IDX_IDIR))
                        return getError();
                    if (op.mode == ABS_IDX_IDIR && TableM6502.is6502())
                        return setError(UNKNOWN_OPERAND);
                } else if (indir == 0) {
                    if (selectMode(modifier, op, ABS_LONG_IDX, ABS_IDX, ZPG_IDX))
                        return getError();
                } else return setError(UNKNOWN_OPERAND);
            } else if (_regs.compareRegName(p, REG_Y)) { // abs,Y zp,Y
                if (indir) return setError(UNKNOWN_OPERAND);
                p = skipSpaces(p + _regs.regNameLen(REG_Y));
                if (selectMode(modifier, op, IMPL, ABS_IDY, ZPG_IDY))
                    return getError();
            } else if (_regs.compareRegName(p, REG_S)) { // off,S (off,S),Y
                p = skipSpaces(p + _regs.regNameLen(REG_S));
                if (indir == 0) {
                    op.mode = SP_REL;
                } else if (indir == '(') {
                    if (*p != ')') return setError(MISSING_CLOSING_PAREN);
                    p = skipSpaces(p + 1);
                    if (*p != ',') return setError(UNKNOWN_OPERAND);
                    p = skipSpaces(p + 1);
                    if (_regs.compareRegName(p, REG_Y)) {
                        p += _regs.regNameLen(REG_Y);
                        op.mode = SP_REL_IDIR_IDY;
                    } else return setError(UNKNOWN_OPERAND);
                } else return setError(UNKNOWN_OPERAND);
            } else return setError(UNKNOWN_OPERAND);
        } else if (indir) {
            if ((indir == '(' && *p != ')') || (indir == '[' && *p != ']'))
                return setError(MISSING_CLOSING_PAREN);
            p = skipSpaces(p + 1);
            if (*p == ',') {
                p = skipSpaces(p + 1);
                if (_regs.compareRegName(p, REG_Y)) { // (zp),Y [zp],Y
                    p += _regs.regNameLen(REG_Y);
                    if (modifier == '<' || op.val32< 0x100) {
                        op.mode = indir == '('
                            ? ZPG_IDIR_IDY : ZPG_IDIR_LONG_IDY;
                    } else return setError(OPERAND_NOT_ZP);
                } else return setError(UNKNOWN_OPERAND);
            } else {            // (abs), (zp), [abs], [zp]
                if (selectMode(modifier, op, IMPL,
                               indir == '[' ? ABS_IDIR_LONG : ABS_IDIR,
                               indir == '[' ? ZPG_IDIR_LONG : ZPG_IDIR))
                    return getError();
            }
        } else {                // al, abs, zp
            if (indir) return setError(UNKNOWN_OPERAND);
            if (selectMode(modifier, op, ABS_LONG, ABS, ZPG))
                return getError();
        }
    }
    _scan = skipSpaces(p);
    return setError(OK);
}

Error AsmM6502::encode(Insn &insn) {
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);
    if (TableM6502.searchName(insn))
        return setError(UNKNOWN_INSTRUCTION);
    _scan = skipSpaces(endName);

    switch (insn.addrMode()) {
    case IMPL:
        insn.emitInsn();
        return checkLineEnd();
    case REL:
        return encodeRelative(insn, /* emitInsn */ true);
    case REL_LONG:
        return encodeLongRelative(insn);
    case ZPG_REL:
        return encodeZeroPageRelative(insn);
    case BLOCK_MOVE:
        return encodeBlockMove(insn);
    default:
        break;
    }

    Operand op;
    if (parseOperand(op)) return setError(op);
    insn.setAddrMode(op.mode);
    if (TableM6502.searchNameAndAddrMode(insn))
        return setError(UNKNOWN_INSTRUCTION);
    switch (insn.addrMode()) {
    case ACCM:
        insn.emitInsn();
        break;
    case IMM:
    case ZPG:
    case ZPG_IDX:
    case ZPG_IDY:
    case ZPG_IDX_IDIR:
    case ZPG_IDIR_IDY:
    case ZPG_IDIR:
    case SP_REL:
    case SP_REL_IDIR_IDY:
    case ZPG_IDIR_LONG:
    case ZPG_IDIR_LONG_IDY:
        insn.emitInsn();
        insn.emitByte(static_cast<uint8_t>(op.val32));
        break;
    case ABS:
    case ABS_IDX:
    case ABS_IDY:
    case ABS_IDIR:
    case ABS_IDX_IDIR:
    case ABS_IDIR_LONG:
        insn.emitInsn();
        insn.emitUint16(static_cast<uint16_t>(op.val32));
        break;
    case ABS_LONG:
    case ABS_LONG_IDX:
        insn.emitInsn();
        insn.emitUint16(static_cast<uint16_t>(op.val32));
        insn.emitByte(static_cast<uint8_t>(op.val32 >> 16));
        break;
    default:
        return setError(INTERNAL_ERROR);
    }
    setError(op);

    return checkLineEnd();
}

} // namespace m6502
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
