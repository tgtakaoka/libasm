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

#include "asm_mc6800.h"
#include "table_mc6800.h"

namespace libasm {
namespace mc6800 {

void AsmMc6800::adjustAccumulator(InsnMc6800 &insn, const Operand &op) {
    if (op.reg != REG_B) return;
    const InsnAdjust iAdjust = insn.insnAdjust();
    if (iAdjust == ADJ_AB01) {
        insn.embed(1);
    } else if (iAdjust == ADJ_AB16) {
        insn.embed(0x10);
    } else if (iAdjust == ADJ_AB64) {
        insn.embed(0x40);
    }
}

Error AsmMc6800::encodeRelative(InsnMc6800 &insn, Config::uintptr_t addr) {
    const Config::uintptr_t base = insn.address() + insn.length() + 1;
    const Config::ptrdiff_t delta = addr - base;
    if (delta >= 128 || delta < -128) return setError(OPERAND_TOO_FAR);
    insn.emitByte(static_cast<uint8_t>(delta));
    return checkLineEnd();
}

AsmMc6800::Token AsmMc6800::nextToken() {
    const char *p = _scan;
    if (endOfLine(p)) {
        _token = EOL;
    } else if (*p == ',') {
        p++;
        _token = COMMA;
    } else if (*p == '#') {
        p++;
        _token = POUND;
    } else if ((_reg = _regs.parseRegName(p)) != REG_UNDEF) {
        p += _regs.regNameLen(_reg);
        _token = (_reg == REG_X || _reg == REG_Y) ? REG_IDX : REG_ACC;
    } else {
        _valSize = SZ_NONE;
        if (*p == '<') {
            p++;
            _valSize = SZ_BYTE;
        } else if (*p == '>') {
            p++;
            _valSize = SZ_WORD;
        }
        _scan = p;
        if (getOperand(_val)) return ERROR;
        _valError = getError();
        _token = VALUE;
        p = _scan;
    }
    _scan = skipSpaces(p);
    return _token;
}

Error AsmMc6800::parseOperand(Operand &op) {
    op.resetError();
    op.reg = REG_UNDEF;
    op.opr = 0;
    host::uint_t comma = 0;
    if (nextToken() == REG_ACC) {
        op.reg = _reg;
        while (nextToken() == COMMA)
            comma++;
    }
    if (_token == EOL && comma == 0) {
        op.mode = INH;
        return setOK();
    }
    bool hasImmediate = false;
    if (_token == POUND) {
        if (comma >= 2)  return setError(UNKNOWN_OPERAND);
        if (nextToken() != VALUE) return setError(UNKNOWN_OPERAND);
        op.imm = _val;
        op.setError(_valError);
        op.mode = IMM;
        hasImmediate = true;
        comma = 0;
        while (nextToken() == COMMA)
            comma++;
        if (_token == EOL && comma == 0) return setOK();
    }
    bool hasBitNum = false;
    OprSize opSize = SZ_NONE;
    if (_token == VALUE) {
        if (comma >= 2) return setError(UNKNOWN_OPERAND);
        op.opr = _val;
        op.setErrorIf(_valError);
        opSize = _valSize;
        op.mode = EXT;
        comma = 0;
        if (nextToken() == COMMA) {
            comma = 1;
            if (nextToken() == VALUE) {
                op.setErrorIf(_valError);
                if (op.reg == REG_UNDEF && !hasImmediate
                    && opSize != SZ_WORD && op.opr < 8) {
                    hasBitNum = true;
                    op.imm = op.opr;
                    op.opr = _val;
                    opSize = _valSize;
                } else return setError(UNKNOWN_OPERAND);
                comma = 0;
                nextToken();
            }
        }
    }
    if (_token == COMMA) {
        comma++;
        nextToken();
    }
    if (_token == REG_IDX) {
        if (comma >= 3) return setError(UNKNOWN_OPERAND);
        if (comma == 0 || comma == 2)
            op.opr = 0;
        if (hasImmediate || hasBitNum) {
            op.mode = hasBitNum ? BIT_IDX : IMM_IDX;
        } else {
            op.mode = (_reg == REG_X) ? IDX : IDY;
        }
        if (nextToken() == EOL) return setOK();
        if (_token == COMMA) nextToken();
    }
    bool hasImmSuffix = false;
    if (_token == POUND) {
        if (hasImmediate || hasBitNum) return setError(UNKNOWN_OPERAND);
        if (nextToken() != VALUE) return setError(UNKNOWN_OPERAND);
        op.setErrorIf(_valError);
        hasImmSuffix = true;
        op.imm = _val;
        nextToken();
    }
    bool hasAddress = false;
    if (_token == COMMA) {
        if (nextToken() != VALUE) return setError(UNKNOWN_OPERAND);
        op.setErrorIf(op.addrError = _valError);
        hasAddress = true;
        op.addr = _val;
        nextToken();
    }
    if (_token == EOL) {
        if (hasImmediate || hasBitNum) {
            op.mode = hasBitNum ? BIT_DIR : IMM_DIR;
        } else if (hasImmSuffix) {
            if (op.mode == EXT) op.mode = hasAddress ? DIR_IMM_REL : DIR_IMM;
            else if (op.mode == IDX) op.mode = hasAddress ? IDX_IMM_REL : IDX_IMM;
            else if (op.mode == IDY) op.mode = hasAddress ? IDY_IMM_REL : IDY_IMM;
            else return setError(UNKNOWN_OPERAND);
        } else if (opSize == SZ_WORD || (opSize == SZ_NONE && op.opr >= 0x100)) {
            op.mode = EXT;
        } else {
            op.mode = DIR;
        }
        return setOK();
    }
    return setError(UNKNOWN_OPERAND);
}

Error AsmMc6800::encode(Insn &_insn) {
    InsnMc6800 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);
    _scan = skipSpaces(endName);
    Operand op;
    if (parseOperand(op)) return setError(op);
    setError(op);

    insn.setAddrMode(op.mode);
    if (TableMc6800.searchNameAndAddrMode(insn))
        return setError(UNKNOWN_INSTRUCTION);

    adjustAccumulator(insn, op);
    insn.emitInsn();
    switch (insn.addrMode()) {
    case INH:
        break;
    case DIR:
    case IDX:
    case IDY:
        insn.emitByte(static_cast<uint8_t>(op.opr));
        break;
    case EXT:
        insn.emitUint16(op.opr);
        break;
    case REL:
        op.addr = op.getError() ? insn.address() : op.opr;
        return encodeRelative(insn, op.addr);
    case IMM:
        if (insn.oprSize() == SZ_BYTE)
            insn.emitByte(static_cast<uint8_t>(op.imm));
        if (insn.oprSize() == SZ_WORD)
            insn.emitUint16(op.imm);
        break;
    case IMM_IDX:
    case IMM_DIR:
        insn.emitByte(static_cast<uint8_t>(op.imm));
        insn.emitByte(static_cast<uint8_t>(op.opr));
        break;
    case BIT_IDX:
    case BIT_DIR:
        op.imm = 1 << op.imm;
        if ((insn.opCode() & 0xF) == 1) // AIM
            op.imm = ~op.imm;
        insn.emitByte(static_cast<uint8_t>(op.imm));
        insn.emitByte(static_cast<uint8_t>(op.opr));
        break;
    case DIR_IMM:
    case IDX_IMM:
    case IDY_IMM:
        insn.emitByte(static_cast<uint8_t>(op.opr));
        insn.emitByte(static_cast<uint8_t>(op.imm));
        break;
    case DIR_IMM_REL:
    case IDX_IMM_REL:
    case IDY_IMM_REL:
        insn.emitByte(static_cast<uint8_t>(op.opr));
        insn.emitByte(static_cast<uint8_t>(op.imm));
        if (op.addrError) op.addr = insn.address();
        return encodeRelative(insn, op.addr);
    default:
        break;
    }
    return checkLineEnd();
}

} // namespace m6800
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
