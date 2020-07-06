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

#include "asm_ins8070.h"
#include "table_ins8070.h"

namespace libasm {
namespace ins8070 {

Error AsmIns8070::encodeImplied(
    InsnIns8070 &insn, const Operand &op) {
    if (insn.dstOpr() == OPR_4) {
        if (op.dst != OPR_16) return setError(UNKNOWN_OPERAND);
        if (op.val >= 16) return setError(OVERFLOW_RANGE);
        insn.embed(op.val & 0xf);
    }
    if (insn.dstOpr() == OPR_IX)
        insn.embed(_regs.encodePointerReg(op.reg));
    if (insn.dstOpr() == OPR_PN)
        insn.embed(_regs.encodePointerReg(op.reg));
    if (insn.srcOpr() == OPR_PN)
        insn.embed(_regs.encodePointerReg(op.reg));
    insn.emitInsn();
    return getError();
}

Error AsmIns8070::encodeAbsolute(
    InsnIns8070 &insn, const Operand &op) {
    const Config::uintptr_t target =
        op.getError() ? 0x0001 : op.val;
    insn.emitInsn();
    // PC will be +1 before fetching instruction.
    insn.emitUint16(target - 1);
    return getError();
}

Error AsmIns8070::encodeImmediate(
    InsnIns8070 &insn, const Operand &op) {
    if (insn.dstOpr() == OPR_IX)
        insn.embed(_regs.encodePointerReg(op.reg));
    if (insn.dstOpr() == OPR_PN)
        insn.embed(_regs.encodePointerReg(op.reg));
    if (insn.addrMode() == GENERIC)
        insn.embed(4);
    insn.emitInsn();
    if (insn.oprSize() == SZ_WORD)
        insn.emitUint16(op.val);
    if (insn.oprSize() == SZ_BYTE)
        insn.emitByte(static_cast<uint8_t>(op.val));
    return getError();
}

Error AsmIns8070::encodeRelative(
    InsnIns8070 &insn, const Operand &op) {
    if (op.autoIndex) return setError(UNKNOWN_OPERAND);
    if (insn.srcOpr() == OPR_IX)
        return encodeIndexed(insn, op);

    const Config::uintptr_t base = insn.address() + 1;
    // PC will be +1 before feting instruction
    const uint8_t fetch = (insn.addrMode() == RELATIVE) ? 1 : 0;
    const Config::uintptr_t target = (op.getError() ? base + fetch : op.val) - fetch;
    const Config::ptrdiff_t offset = target - base;
    if (offset < -128 || offset >= 128)
        return setError(OPERAND_TOO_FAR);
    insn.emitInsn();
    insn.emitByte(static_cast<uint8_t>(offset));
    return getError();
}

Error AsmIns8070::encodeIndexed(
    InsnIns8070 &insn, const Operand &op) {
    const Config::ptrdiff_t offset =
        static_cast<Config::uintptr_t>(op.val);
    if (offset < -128 || offset >= 128)
        return setError(OVERFLOW_RANGE);
    insn.embed(_regs.encodePointerReg(op.reg));
    if (op.autoIndex) insn.embed(4);
    insn.emitInsn();
    insn.emitByte(static_cast<uint8_t>(offset));
    return getError();
}

Error AsmIns8070::encodeGeneric(
    InsnIns8070 &insn, const Operand &op) {
    if (op.src == OPR_IM)
        return encodeImmediate(insn, op);
    if (op.src == OPR_16 && op.reg == REG_UNDEF) {
        const Config::uintptr_t target =
            op.getError() ? 0xFF00 : op.val;
        if (target < 0xFF00) return setError(OVERFLOW_RANGE);
        insn.embed(5);
        insn.emitInsn();
        insn.emitByte(static_cast<uint8_t>(target));
        return getError();
    }
    if (op.reg == REG_PC)
        return encodeRelative(insn, op);
    if (op.reg == REG_SP || op.reg == REG_P2 || op.reg == REG_P3)
        return encodeIndexed(insn, op);
    return setError(UNKNOWN_OPERAND);
}

Error AsmIns8070::nextToken(Operand &op, OprFormat &opr) {
    const char *p = _scan;
    if (*p == '@') {
        if (op.autoIndex || isspace(*++p))
            return setError(UNKNOWN_OPERAND);
        op.autoIndex = true;
    }
    const RegName reg = _regs.parseRegister(p);
    if (reg == REG_UNDEF) {
        if (op.hasVal) return setError(UNKNOWN_OPERAND);
        const bool immediate = (*p == '=' || *p == '#');
        if (immediate) {
            if (op.autoIndex) return setError(UNKNOWN_OPERAND);
            p++;
        }
        _scan = p;
        if (getOperand(op.val)) return getError();
        op.setError(getError());
        op.hasVal = true;
        opr = immediate ? OPR_IM : OPR_16;
        return OK;
    }

    switch (reg) {
    case REG_A: opr = OPR_A; break;
    case REG_E: opr = OPR_E; break;
    case REG_S: opr = OPR_S; break;
    case REG_EA: opr = OPR_EA; break;
    case REG_T: opr = OPR_T; break;
    case REG_P2:
    case REG_P3:
        op.reg = reg;
        opr = OPR_IX;
        break;
    case REG_SP:
        op.reg = reg;
        opr = OPR_SP;
        break;
    case REG_PC:
        op.reg = reg;
        opr = OPR_PN;
        break;
    default:
        return setError(UNKNOWN_REGISTER);
    }
    _scan = p + _regs.regNameLen(reg);
    return setOK();
}

Error AsmIns8070::parseOperand(Operand &op) {
    op.setOK();
    op.dst = op.src = OPR_NO;
    op.reg = REG_UNDEF;
    op.autoIndex = op.hasVal = false;
    op.val = 0;

    const char *p = _scan;
    if (endOfLine(p))
        return setOK();
    if (nextToken(op, op.dst))
        return getError();
    p = skipSpaces(_scan);
    if (endOfLine(p))
        return setOK();
    if (*p != ',') return setError(MISSING_COMMA);
    _scan = skipSpaces(p + 1);
    if (nextToken(op, op.src))
        return getError();
    p = skipSpaces(_scan);
    if (endOfLine(p))
        return setOK();
    if (*p != ',') return setError(MISSING_COMMA);
    _scan = skipSpaces(p + 1);
    OprFormat extra;
    if (nextToken(op, extra)) return getError();
    if (op.src != OPR_16)
        return setError(UNKNOWN_OPERAND);
    if (extra != OPR_IX && extra != OPR_SP && extra != OPR_PN)
        return setError(UNKNOWN_OPERAND);
    op.src = OPR_GN;
    return checkLineEnd();
}

Error AsmIns8070::encode(Insn &_insn) {
    InsnIns8070 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);
    _scan = skipSpaces(endName);

    Operand op;
    if (parseOperand(op)) return getError();
    setError(op);

    insn.setOprFormats(op.dst, op.src);
    if (TableIns8070.searchName(insn))
        return setError(TableIns8070.getError());

    switch (insn.addrMode()) {
    case IMPLIED:   return encodeImplied(insn, op);
    case ABSOLUTE:  return encodeAbsolute(insn, op);
    case IMMEDIATE: return encodeImmediate(insn, op);
    case RELATIVE:  return encodeRelative(insn, op);
    case GENERIC:   return encodeGeneric(insn, op);
    default:        return setError(INTERNAL_ERROR);
    }
}

} // namespace ins8070
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
