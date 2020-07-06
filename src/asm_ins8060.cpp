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

#include "asm_ins8060.h"
#include "table_ins8060.h"

namespace libasm {
namespace ins8060 {

Error AsmIns8060::encodeRel8(
    InsnIns8060 &insn, const Operand &op) {
    Config::ptrdiff_t delta;
    if (op.mode == DISP) {
        delta = op.val;     // E(Pn)
        insn.embed(_regs.encodePointerReg(op.reg));
    } else {
        // PC points the last byte of instruction.
        const Config::uintptr_t base = insn.address() + 1;
        // PC will be incremented before fetching next instruction.
        const uint8_t fetch = (insn.addrMode() == REL8) ? 1 : 0;
        // Program space is paged by 4kB.
        const Config::uintptr_t target = op.getError() ? base
            : ((op.val & 0xFFF) | (base & ~0xFFF)) - fetch;
        delta = target - base;
        // delta -128 is for E reg.
        if (delta <= -128 || delta >= 128) return setError(OPERAND_TOO_FAR);
        insn.embed(_regs.encodePointerReg(REG_PC));
    }
    insn.emitInsn();
    insn.emitByte(static_cast<uint8_t>(delta));
    return getError();
}

Error AsmIns8060::encodeIndx(InsnIns8060 &insn, const Operand &op) {
    if (op.mode == REL8)
        return encodeRel8(insn, op);
    insn.embed(_regs.encodePointerReg(op.reg));
    if (op.mode == INDX) { // auto displacement mode
        if (insn.addrMode() != INDX) return setError(ILLEGAL_OPERAND);
        insn.embed(4);
    }
    insn.emitInsn();
    insn.emitByte(static_cast<uint8_t>(op.val));
    return getError();
}

Error AsmIns8060::parseOperand(Operand &op) {
    op.setOK();
    op.mode = UNDEF;
    const char *p = _scan;
    const bool autoDisp = (*p == '@');
    if (autoDisp) {
        p++;
        if (isspace(*p)) return setError(UNKNOWN_OPERAND);
    }

    if (!autoDisp && endOfLine(p)) {
        op.mode = INHR;
        return OK;
    } else if ((op.reg = _regs.parseRegister(p)) != REG_UNDEF) {
        p += _regs.regNameLen(op.reg);
        if (op.reg == REG_E) {
            op.reg = REG_UNDEF;
            op.val = 0x80;
            if (*p != '(') return setError(UNKNOWN_OPERAND);
        }
    } else {
        if (*p == '(' && _regs.parseRegister(p + 1) != REG_UNDEF)
            return setError(UNKNOWN_OPERAND);
        _scan = p;
        if (getOperand(op.val)) return getError();
        op.setError(getError());
        p = _scan;
    }
    if (*p == '(') {
        p = skipSpaces(p + 1);
        if (op.reg != REG_UNDEF) return setError(UNKNOWN_OPERAND);
        op.reg = _regs.parsePointerReg(p);
        if (op.reg == REG_UNDEF) return setError(UNKNOWN_OPERAND);
        p = skipSpaces(p + _regs.regNameLen(op.reg));
        if (*p != ')') return setError(MISSING_CLOSING_PAREN);
        op.mode = autoDisp ? INDX : DISP;
        _scan = p + 1;
        return OK;
    } else if (!autoDisp) {
        op.mode = (op.reg != REG_UNDEF) ? PNTR
            : REL8; // May be IMM8 too
        _scan = p;
        return OK;
    }
    return setError(UNKNOWN_OPERAND);
}

Error AsmIns8060::encode(Insn &_insn) {
    InsnIns8060 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);
    _scan = skipSpaces(endName);

    Operand op;
    if (parseOperand(op)) return getError();
    _scan = skipSpaces(_scan);
    if (!endOfLine(_scan)) return setError(GARBAGE_AT_END);
    setErrorIf(op.getError());

    insn.setAddrMode(op.mode);
    if (TableIns8060.searchName(insn))
        return setError(TableIns8060.getError());

    switch (insn.addrMode()) {
    case INHR:
        insn.emitInsn();
        break;
    case REL8:
        encodeRel8(insn, op);
        break;
    case PNTR:
        insn.embed(_regs.encodePointerReg(op.reg));
        insn.emitInsn();
        break;
    case DISP:
    case INDX:
        encodeIndx(insn, op);
        break;
    case IMM8:
        insn.emitInsn();
        insn.emitByte(static_cast<uint8_t>(op.val));
        break;
    default:
        return setError(INTERNAL_ERROR);
    }
    return getError();
}

} // namespace ins8060
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
