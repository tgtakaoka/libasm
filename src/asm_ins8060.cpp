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
        delta = op.val16;       // E(Pn)
        insn.embed(RegIns8060::encodePointerReg(op.reg));
    } else {
        // PC points the last byte of instruction.
        const Config::uintptr_t base = insn.address() + 1;
        // PC will be incremented before fetching next instruction.
        const uint8_t fetch = (insn.addrMode() == REL8) ? 1 : 0;
        // Program space is paged by 4kB.
        const Config::uintptr_t target = op.getError() ? base
            : ((op.val16 & 0xFFF) | (base & ~0xFFF)) - fetch;
        delta = target - base;
        // delta -128 is for E reg.
        if (delta <= -128 || delta >= 128) return setError(OPERAND_TOO_FAR);
        insn.embed(RegIns8060::encodePointerReg(REG_PC));
    }
    insn.emitInsn();
    insn.emitByte(delta);
    return getError();
}

Error AsmIns8060::encodeIndx(InsnIns8060 &insn, const Operand &op) {
    if (op.mode == REL8)
        return encodeRel8(insn, op);
    insn.embed(RegIns8060::encodePointerReg(op.reg));
    if (op.mode == INDX) { // auto displacement mode
        if (insn.addrMode() != INDX) return setError(OPERAND_NOT_ALLOWED);
        insn.embed(4);
    }
    insn.emitInsn();
    insn.emitByte(op.val16);
    return getError();
}

Error AsmIns8060::parseOperand(Operand &op) {
    const char *p = _scan;
    if (endOfLine(p)) {
        op.mode = INHR;
        return OK;
    }

    const bool autoDisp = (*p == '@');
    if (autoDisp) p++;

    const RegName reg = RegIns8060::parseRegName(p);
    if (reg == REG_E) {
        p += RegIns8060::regNameLen(reg);
        op.val16 = 0x80;
    } else if (reg != REG_UNDEF) {
        _scan = p + RegIns8060::regNameLen(reg);
        op.mode = PNTR;
        op.reg = reg;
        return OK;
    } else {
        op.val16 = parseExpr16(p);
        if (parserError()) return getError();
        op.setError(getError());
        p = _scan;
    }
    p = skipSpaces(p);
    if (*p == '(') {
        p++;
        const RegName base = RegIns8060::parseRegName(p);
        if (!RegIns8060::isPointerReg(base))
            return setError(UNKNOWN_OPERAND);
        p += RegIns8060::regNameLen(base);
        if (*p != ')') return setError(MISSING_CLOSING_PAREN);
        _scan = p + 1;
        op.reg = base;
        op.mode = autoDisp ? INDX : DISP;
        return OK;
    }

    if (autoDisp || reg == REG_E) return setError(UNKNOWN_OPERAND);
    _scan = p;
    op.mode = REL8; // May be IMM8 too
    return OK;
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
        insn.embed(RegIns8060::encodePointerReg(op.reg));
        insn.emitInsn();
        break;
    case DISP:
    case INDX:
        encodeIndx(insn, op);
        break;
    case IMM8:
        insn.emitInsn();
        insn.emitByte(op.val16);
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
