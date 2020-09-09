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

Error AsmMos6502::encodeLongRelative(InsnMos6502 &insn, const Operand &op) {
    const uint32_t target = op.getError() ? insn.address() : op.val32;
    const uint8_t cbank = static_cast<uint8_t>(insn.address() >> 16);
    const uint8_t tbank = static_cast<uint8_t>(target >> 16);
    if (cbank != tbank) return setError(OPERAND_TOO_FAR);
    const uint32_t base = insn.address() + 3;
    const int32_t delta = op.getError() ? 0 : target - base;
    if (delta >= 32768L || delta < -32768L) return setError(OPERAND_TOO_FAR);
    insn.emitInsn();
    insn.emitUint16(static_cast<uint16_t>(delta));
    return OK;
}

Error AsmMos6502::encodeRelative(InsnMos6502 &insn, const Operand &op) {
    const Config::uintptr_t base = insn.address() + 2;
    const Config::uintptr_t target = op.getError() ? base : op.val32;
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
        extra.getError() ? insn.address() : extra.val32;
    const Config::ptrdiff_t delta = target - base;
    if (delta >= 128 || delta < -128) return setError(OPERAND_TOO_FAR);
    insn.emitInsn();
    insn.emitByte(static_cast<uint8_t>(op.val32));
    insn.emitByte(static_cast<uint8_t>(delta));
    return OK;
}

Error AsmMos6502::selectMode(
    char size, Operand &op, AddrMode labs, AddrMode abs, AddrMode zp) {
    if (size == '}') {
        if (labs == IMPL) return setError(UNKNOWN_OPERAND);
        op.mode = labs;
    } else if (size == '>') {
        op.mode = abs;
    } else if (size == '<') {
        op.mode = zp;
    } else if (op.val32 >= 0x10000L) {
        op.mode = labs;
    } else {
        op.mode = (op.val32 < 0x100) ? zp : abs;
    }
    if (endOfLine(skipSpaces(_scan))) return OK;
    return setError(UNKNOWN_OPERAND);
}

Error AsmMos6502::parseOperand(Operand &op, Operand &extra) {
    const char *p = _scan;

    if (endOfLine(p)) {
        op.mode = IMPL;
        return OK;
    }
    if (*p == '#') {            // #nn
        _scan = p + 1;
        if (getOperand(op.val32)) return getError();
        op.setError(getError());
        p = skipSpaces(_scan);
        if (endOfLine(p)) {
            op.mode = IMMA;
            return OK;
        }
        return setError(UNKNOWN_OPERAND);
    }
    if (_regs.compareRegName(p, REG_A)) { // A
        _scan = p + _regs.regNameLen(REG_A);
        op.mode = ACCM;
        return OK;
    }

    const char indir = ((*p == '(') ? ')' : (*p == '[' ? ']' : 0));
    if (indir) p = skipSpaces(p + 1);
    char size = (*p == '>' || *p == '<') ? *p++ : 0;
    if (*p == '>') { size = '}'; p++; }
    _scan = p;
    if (getOperand(op.val32)) return getError();
    op.setError(getError());
    p = skipSpaces(_scan);
    if (*p == ',') {
        p = skipSpaces(p + 1);
        if (_regs.compareRegName(p, REG_X)) { // al,X abs,X zp,X (abs,X) (zp,X)
            p = skipSpaces(p + _regs.regNameLen(REG_X));
            if (indir) {
                if (*p == ')') {
                    _scan = p + 1;
                    return selectMode(size, op, IMPL, ABS_IDX_IDIR, ZPG_IDX_IDIR);
                }
                return setError(MISSING_CLOSING_PAREN);
            }
            _scan = p;
            return selectMode(size, op, ABS_LONG_IDX, ABS_IDX, ZPG_IDX);
        }
        if (_regs.compareRegName(p, REG_Y)) { // abs,Y zp,Y
            if (indir) return setError(UNKNOWN_OPERAND);
            _scan = p + _regs.regNameLen(REG_Y);
            return selectMode(size, op, IMPL, ABS_IDY, ZPG_IDY);
        }
        if (_regs.compareRegName(p, REG_S)) { // off,S (off,S),Y
            p = skipSpaces(p + _regs.regNameLen(REG_S));
            if (indir == 0) {
                _scan = p;
                op.mode = SP_REL;
                return OK;
            }
            if (indir == ')' && *p == indir) {
                p = skipSpaces(p + 1);
                if (*p != ',') return setError(MISSING_COMMA);
                p = skipSpaces(p + 1);
                if (_regs.compareRegName(p, REG_Y)) {
                    _scan = p + _regs.regNameLen(REG_Y);
                    op.mode = SP_REL_IDIR_IDY;
                    return OK;
                }
            }
            return setError(UNKNOWN_OPERAND);
        }
        _scan = p;
        if (getOperand(extra.val32)) return getError();
        extra.setError(getError());
        op.mode = ZPG_REL;
        return OK;
    }
    if (indir) {
        if (*p != indir) return setError(MISSING_CLOSING_PAREN);
        p = skipSpaces(p + 1);
        if (*p == ',') {        // (zp),Y [zp],Y
            p = skipSpaces(p + 1);
            if (_regs.compareRegName(p, REG_Y)) {
                _scan = p + _regs.regNameLen(REG_Y);
                op.mode = (indir == ']') ? ZPG_IDIR_LONG_IDY : ZPG_IDIR_IDY;
                return OK;
            }
            return setError(UNKNOWN_OPERAND);
        }
        // (abs) (zp) [abs] [zp]
        _scan = p;
        return selectMode(size, op, IMPL,
                          indir == ']' ? ABS_IDIR_LONG : ABS_IDIR,
                          indir == ']' ? ZPG_IDIR_LONG : ZPG_IDIR);
    }
    // al abs zp
    _scan = p;
    return selectMode(size, op, ABS_LONG, ABS, ZPG);
}

static const char TEXT_M[]      PROGMEM = "M";
static const char TEXT_X[]      PROGMEM = "X";
static const char TEXT_ON[]     PROGMEM = "ON";
static const char TEXT_OFF[]    PROGMEM = "OFF";
static const char TEXT_LONGA[]  PROGMEM = "LONGA";
static const char TEXT_LONGI[]  PROGMEM = "LONGI";
static const char TEXT_ASSUME[] PROGMEM = "ASSUME";

Error AsmMos6502::parseOnOff(const char *line, bool &val) {
    const char *end = _parser.scanSymbol(line);
    if (strncasecmp_P(line, TEXT_ON, end - line) == 0) {
        val = true;
        checkLineEnd(end);
    } else if (strncasecmp_P(line, TEXT_OFF, end - line) == 0) {
        val = false;
        checkLineEnd(end);
    } else {
        setError(UNKNOWN_OPERAND);
    }
    return OK;
}

Error AsmMos6502::parseZeroOne(const char *line, bool &val) {
    line = skipSpaces(line);
    if (*line == ':') {
        uint32_t val32;
        _scan = line + 1;
        if (getOperand(val32) == OK) {
            if (val32 == 1) val = false;
            else if (val32 == 0) val = true;
            else setError(UNKNOWN_OPERAND);
            checkLineEnd();
        } else {
            setError(UNKNOWN_OPERAND);
        }
    }
    return OK;
}

Error AsmMos6502::processPseudo(InsnMos6502 &insn, const char *line) {
    insn.resetAddress(insn.address()); // make generated bytes zero.
    if (strcasecmp_P(insn.name(), TEXT_LONGA) == 0)
        return parseOnOff(line, _long_acc);
    if (strcasecmp_P(insn.name(), TEXT_LONGI) == 0)
        return parseOnOff(line, _long_idx);
    if (strcasecmp_P(insn.name(), TEXT_ASSUME) == 0) {
        const char *end = _parser.scanSymbol(line);
        if (strncasecmp(line, TEXT_M, end - line) == 0)
            return parseZeroOne(end, _long_acc);
        if (strncasecmp(line, TEXT_X, end - line) == 0)
            return parseZeroOne(end, _long_idx);
        setError(UNKNOWN_OPERAND);
        return OK;
    }
    return UNKNOWN_INSTRUCTION;
}

Error AsmMos6502::encode(Insn &_insn) {
    InsnMos6502 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);
    _scan = skipSpaces(endName);
    if (processPseudo(insn, _scan) == OK)
        return getError();
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
    case REL_LONG:
        encodeLongRelative(insn, op);
        break;
    case ZPG_REL:
        encodeZeroPageRelative(insn, op, extra);
        break;
    case BLOCK_MOVE:
        insn.emitInsn();
        insn.emitByte(static_cast<uint8_t>(extra.val32 >> 16));;
        insn.emitByte(static_cast<uint8_t>(op.val32 >> 16));
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
    setErrorIf(op.getError());
    setErrorIf(extra.getError());

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
