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

Error AsmMc6800::adjustAccumulator(InsnMc6800 &insn, const Operand &op) {
    const InsnAdjust iAdjust = insn.insnAdjust();
    if (iAdjust == ADJ_ZERO)
        return op.reg == REG_UNDEF ? OK : setError(UNKNOWN_OPERAND);
    if (op.reg == REG_A) return OK;
    if (op.reg == REG_B) {
        if (iAdjust == ADJ_AB01) {
            insn.embed(1);
        } else if (iAdjust == ADJ_AB16) {
            insn.embed(0x10);
        } else if (iAdjust == ADJ_AB64) {
            insn.embed(0x40);
        }
        return OK;
    }
    return setError(UNKNOWN_INSTRUCTION);
}

Error AsmMc6800::encodeRelative(InsnMc6800 &insn, Config::uintptr_t addr) {
    const Config::uintptr_t base = insn.address() + insn.length() + 1;
    const Config::ptrdiff_t delta = addr - base;
    if (delta >= 128 || delta < -128) return setError(OPERAND_TOO_FAR);
    insn.emitByte(static_cast<uint8_t>(delta));
    return OK;
}

AsmMc6800::Token AsmMc6800::nextToken() {
    const char *p = skipSpaces(_scan);
    if (endOfLine(p))
        return _token = EOL;
    if (*p == ',') {
        _scan = p + 1;
        return _token = COMMA;
    }
    if ((_reg = _regs.parseRegName(p)) != REG_UNDEF) {
        _scan = p + _regs.regNameLen(_reg);
        return _token = (_reg == REG_X || _reg == REG_Y) ? REG_IDX : REG_ACC;
    }

    const bool imm = *p == '#';
    if (imm) p = skipSpaces(p + 1);
    _valSize = SZ_NONE;
    if (*p == '<') {
        p++;
        _valSize = SZ_BYTE;
    } else if (*p == '>') {
        p++;
        _valSize = SZ_WORD;
    }
    _scan = p;
    if (getOperand(_val))
        return _token = ERROR;
    return _token = imm ? VAL_IMM : VAL_ADR;
}

Error AsmMc6800::parseOperand(Operand &op) {
    op.resetError();
    op.reg = REG_UNDEF;
    op.imm = op.opr = op.addr = 0;
    op.addrError = OK;
    uint8_t comma = 0;
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
    if (_token == VAL_IMM) {
        if (comma >= 2)  return setError(UNKNOWN_OPERAND);
        op.imm = _val;
        op.setError(getError());
        op.mode = IMM;
        hasImmediate = true;
        comma = 0;
        while (nextToken() == COMMA)
            comma++;
        if (_token == EOL && comma == 0) return setOK();
    }
    bool hasBitNum = false;
    OprSize opSize = SZ_NONE;
    if (_token == VAL_ADR) {
        if (comma >= 2) return setError(UNKNOWN_OPERAND);
        op.opr = _val;
        op.setErrorIf(getError());
        opSize = _valSize;
        op.mode = EXT;
        comma = 0;
        if (nextToken() == COMMA) {
            comma = 1;
            if (nextToken() == VAL_ADR) {
                op.setErrorIf(getError());
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
    if (_token == VAL_IMM) {
        if (hasImmediate || hasBitNum) return setError(UNKNOWN_OPERAND);
        op.setErrorIf(getError());
        hasImmSuffix = true;
        op.imm = _val;
        nextToken();
    }
    bool hasAddress = false;
    if (_token == COMMA) {
        if (nextToken() != VAL_ADR) return setError(UNKNOWN_OPERAND);
        op.setErrorIf(op.addrError = getError());
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
    if (parseOperand(op)) return getError();
    setError(op);

    insn.setAddrMode(op.mode);
    if (TableMc6800.searchName(insn))
        return setError(UNKNOWN_INSTRUCTION);

    if (adjustAccumulator(insn, op)) return getError();
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
        encodeRelative(insn, op.addr);
        break;
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
        encodeRelative(insn, op.addr);
        break;
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
