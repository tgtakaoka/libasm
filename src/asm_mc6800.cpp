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

Error AsmMc6800::encodeRelative(InsnMc6800 &insn) {
    Config::uintptr_t addr;
    if (getOperand(addr)) return getError();
    if (getError() == UNDEFINED_SYMBOL) addr = insn.address();
    const Config::uintptr_t base = insn.address() + 2;
    const Config::ptrdiff_t delta = addr - base;
    insn.emitInsn();
    if (delta >= 128 || delta < -128) return setError(OPERAND_TOO_FAR);
    insn.emitByte(static_cast<uint8_t>(delta));
    return checkLineEnd();
}

Error AsmMc6800::parseOperand(Operand &op) {
    op.resetError();
    const char *p = _scan;
    host::uint_t comma = 0;
    op.mode = INH;
    op.reg = _regs.parseRegName(p);
    if (op.reg == REG_A || op.reg == REG_B) {
        p = skipSpaces(p + _regs.regNameLen(op.reg));
        while (*p == ',') {
            comma++;
            p = skipSpaces(p + 1);
        }
    }
    if (comma == 0 && (*p == 0 || *p == ';')) {
        _scan = p;
        return setError(OK);
    }
    bool hasImmediate = false;
    if (*p == '#') {
        if (comma >= 2) return setError(UNKNOWN_OPERAND);
        comma = 0;
        _scan = p + 1;
        if (getOperand(op.imm)) return getError();
        op.setError(getError());
        p = _scan;
        op.mode = IMM;
        hasImmediate = true;
        if (op.reg != REG_UNDEF) // A #IMM
            return setError(OK);
        while (*p == ',') {
            comma++;
            p = skipSpaces(p + 1);
        }
        if (comma == 0 && (*p == 0 || *p == ';')) {
            _scan = p;
            return setError(OK);
        }
    } else if (*p == ',') { // ,X
        comma++;
        p = skipSpaces(p + 1);
    }
    if (_regs.compareRegName(p, REG_X)) { // ,X
        if (comma >= 3)
            return op.setError(UNKNOWN_OPERAND);
        _scan = p + _regs.regNameLen(REG_X);
        op.opr = 0;
        op.mode = hasImmediate ? IMM_IDX : IDX;
        return setError(OK);
    }
    host::int_t size = -1;
    if (*p == '<') {
        size = 8;
        p++;
    } else if (*p == '>') {
        size = 16;
        p++;
    }
    _scan = p;
    if (getOperand(op.opr)) return getError();
    if (getError()) op.setError(getError());
    p = _scan;
    if (*p == ',') {            // nn,X
        p = skipSpaces(p + 1);
        if (!_regs.compareRegName(p, REG_X))
            return op.setError(UNKNOWN_OPERAND);
        p += _regs.regNameLen(REG_X);
        if (size == 16 || (size < 0 && op.opr >= 0x100))
            return op.setError(OVERFLOW_RANGE);
        op.mode = hasImmediate ? IMM_IDX : IDX;
    } else if (size == 8 || (size < 0 && op.opr < 0x100)) {
        op.mode = hasImmediate ? IMM_DIR : DIR;
    } else {
        if (hasImmediate) return op.setError(OVERFLOW_RANGE);
        op.mode = EXT;
    }
    _scan = p;
    return setError(OK);
}

Error AsmMc6800::encode(Insn &_insn) {
    InsnMc6800 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);
    if (TableMc6800.searchName(insn))
        return setError(UNKNOWN_INSTRUCTION);
    _scan = skipSpaces(endName);

    if (insn.addrMode() == REL)
        return encodeRelative(insn);

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
        insn.emitByte(static_cast<uint8_t>(op.opr));
        break;
    case EXT:
        insn.emitUint16(op.opr);
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
