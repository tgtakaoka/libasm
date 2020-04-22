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

#include "asm_w65c816.h"
#include "table_w65c816.h"

namespace libasm {
namespace w65c816 {

using mos6502::RegName::REG_A;
using mos6502::RegName::REG_X;
using mos6502::RegName::REG_Y;
#define REG_S mos6502::RegName('S')

Error AsmW65C816::encodeLongRelative(InsnW65C816 &insn) {
    Config::uintptr_t target;
    if (getOperand(target)) return getError();
    if (getError() == UNDEFINED_SYMBOL) target = insn.address();
    const uint8_t cbank = static_cast<uint8_t>(insn.address() >> 16);
    const uint8_t tbank = static_cast<uint8_t>(target >> 16);
    if (cbank != tbank) return setError(OPERAND_TOO_FAR);
    const Config::uintptr_t base = insn.address() + 3;
    const Config::ptrdiff_t delta = target - base;
    insn.emitInsn();
    if (delta >= 32768L || delta < -32768L) return setError(OPERAND_TOO_FAR);
    insn.emitUint16(static_cast<uint16_t>(delta));
    return checkLineEnd();
}

Error AsmW65C816::encodeRelative(InsnW65C816 &insn, bool emitInsn) {
    Config::uintptr_t target;
    if (getOperand(target)) return getError();
    if (getError() == UNDEFINED_SYMBOL) target = insn.address();
    const uint8_t cbank = static_cast<uint8_t>(insn.address() >> 16);
    const uint8_t tbank = static_cast<uint8_t>(target >> 16);
    if (cbank != tbank) return setError(OPERAND_TOO_FAR);
    const Config::uintptr_t base = insn.address() + (emitInsn ? 2 : 3);
    const Config::ptrdiff_t delta = target - base;
    if (emitInsn) insn.emitInsn();
    if (delta >= 128 || delta < -128) return setError(OPERAND_TOO_FAR);
    insn.emitByte(static_cast<uint8_t>(delta));
    return checkLineEnd();
}

Error AsmW65C816::encodeZeroPageRelative(InsnW65C816 &insn) {
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

Error AsmW65C816::encodeBlockMove(InsnW65C816 &insn) {
    Config::uintptr_t src, dst;
    if (getOperand(src)) return getError();
    Error error = getError();
    if (*_scan != ',') return setError(UNKNOWN_OPERAND);
    _scan++;
    if (getOperand(dst)) return getError();
    if (getError()) error = getError();
    insn.emitInsn();
    insn.emitByte(static_cast<uint8_t>(dst >> 16));;
    insn.emitByte(static_cast<uint8_t>(src >> 16));
    return setError(error ? error : getError());
};

Error AsmW65C816::selectMode(char modifier, Operand &op, AddrMode labs, AddrMode abs, AddrMode zp) {
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

Error AsmW65C816::parseOperand(Operand &op) {
    op.resetError();
    const char *p = _scan;
    if (*p == '#') {            // #nn
        _scan = p + 1;
        if (getOperand(op.val32)) return getError();
        op.setError(getError());
        p = _scan;
        op.mode = IMMA;
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
        if (getOperand(op.val32)) return getError();
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
                    }
                    else return setError(UNKNOWN_OPERAND);
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
                    if (modifier == '<' || op.val32 < 0x100) {
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

Error AsmW65C816::parseOnOff(bool &val) {
    const char *end = _parser.scanSymbol(_scan);
    if (strncasecmp(_scan, "ON", end - _scan) == 0) {
        val = true;
        _scan = end;
        return checkLineEnd();
    }
    if (strncasecmp(_scan, "OFF", end - _scan) == 0) {
        val = false;
        _scan = end;
        return checkLineEnd();
    }
    return setError(UNKNOWN_OPERAND);
}

Error AsmW65C816::parseZeroOne(const char *line, bool &val) {
    uint32_t val32;
    _scan = skipSpaces(line);
    if (*_scan != ':') return setError(UNKNOWN_OPERAND);
    _scan++;
    getOperand(val32);
    if (getError()) return getError();
    if (val32 == 1) val = false;
    else if (val32 == 0) val = true;
    else return setError(UNKNOWN_OPERAND);
    return checkLineEnd();
}

Error AsmW65C816::processPseudo(InsnW65C816 &insn) {
    insn.resetAddress(insn.address()); // make generated bytes zero.
    if (insn.opCode() == PSEUDO_LONGA)
        return parseOnOff(_long_acc);
    if (insn.opCode() == PSEUDO_LONGI)
        return parseOnOff(_long_idx);
    if (insn.opCode() == PSEUDO_ASSUME) {
        const char *end = _parser.scanSymbol(_scan);
        if (strncasecmp(_scan, "M", end - _scan) == 0)
            return parseZeroOne(end, _long_acc);
        if (strncasecmp(_scan, "X", end - _scan) == 0)
            return parseZeroOne(end, _long_idx);
        return setError(UNKNOWN_OPERAND);
    }
    return setError(UNKNOWN_INSTRUCTION);
}

Error AsmW65C816::encode(Insn &_insn) {
    InsnW65C816 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);
    if (TableW65C816.searchName(insn))
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
    case PSEUDO:
        return processPseudo(insn);
    default:
        break;
    }

    Operand op;
    if (parseOperand(op)) return setError(op);
    insn.setAddrMode(op.mode);
    if (TableW65C816.searchNameAndAddrMode(insn))
        return setError(UNKNOWN_INSTRUCTION);
    switch (insn.addrMode()) {
    case ACCM:
        insn.emitInsn();
        break;
    case IMMA:
        insn.emitInsn();
        if (_long_acc) {
            insn.emitUint16(static_cast<uint16_t>(op.val32));
        } else {
            insn.emitByte(static_cast<uint8_t>(op.val32));
        }
        break;
    case IMMX:
        insn.emitInsn();
        if (_long_idx) {
            insn.emitUint16(static_cast<uint16_t>(op.val32));
        } else {
            insn.emitByte(static_cast<uint8_t>(op.val32));
        }
        break;
    case IMM8:
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

} // namespace w65c816
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
