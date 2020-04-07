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

#include "asm_tms9900.h"
#include "table_tms9900.h"

namespace libasm {
namespace tms9900 {

Error AsmTms9900::checkComma() {
    _scan = skipSpaces(_scan);
    if (*_scan != ',') return UNKNOWN_OPERAND;
    _scan = skipSpaces(_scan + 1);
    return OK;
}

Error AsmTms9900::encodeImm(InsnTms9900 &insn, bool emitInsn) {
    uint16_t val;
    if (getOperand16(val)) return getError();
    if (emitInsn) insn.emitInsn();
    insn.emitOperand(val);
    return setError(getError());
}

Error AsmTms9900::encodeReg(InsnTms9900 &insn, bool emitInsn) {
    RegName regName = _regs.parseRegName(_scan);
    if (regName == REG_UNDEF) return setError(UNKNOWN_OPERAND);
    _scan += _regs.regNameLen(regName);
    uint16_t operand = _regs.encodeRegNumber(regName);
    switch (insn.addrMode()) {
    case REG:
    case REG_IMM:
    case CNT_REG: break;
    case REG_SRC: operand <<= 6; break;
    default: return setError(INTERNAL_ERROR);
    }
    insn.embed(operand);
    if (emitInsn) insn.emitInsn();
    return setError(OK);
}

Error AsmTms9900::encodeCnt(InsnTms9900 &insn, bool acceptR0, bool accept16) {
    uint16_t count;
    if (acceptR0 && _regs.parseRegName(_scan) == REG_R0) {
        _scan += 2;
        count = 0;
    } else {
        uint8_t val8;
        if (getOperand8(val8)) return getError();
        if (val8 > 16 || (!accept16 && val8 == 16))
            return setError(UNKNOWN_OPERAND);
        count = val8 & 0xf;
    }
    switch (insn.addrMode()) {
    case CNT_REG: count <<= 4; break;
    case CNT_SRC:
    case XOP_SRC: count <<= 6; break;
    default: return setError(INTERNAL_ERROR);
    }
    insn.embed(count);
    insn.emitInsn();
    return getError();
}

Error AsmTms9900::encodeOpr(
    InsnTms9900 &insn, bool emitInsn, bool destinationa) {
    const char *p = _scan;
    RegName regName;
    uint8_t mode = 0;
    uint16_t val16;
    if ((regName = _regs.parseRegName(p)) != REG_UNDEF) {
        p += _regs.regNameLen(regName);
        mode = 0;
        setError(OK);
    } else if (*p == '*') {
        p++;
        mode = 1;
        if ((regName = _regs.parseRegName(p)) == REG_UNDEF)
            return setError(UNKNOWN_OPERAND);
        p += _regs.regNameLen(regName);
        if (*p == '+') {
            p++;
            mode = 3;
        }
        setError(OK);
    } else if (*p == '@') {
        mode = 2;
        _scan = p + 1;
        if (getOperand16(val16)) return getError();
        p = _scan;
        if (*p == '(') {
            p++;
            regName = _regs.parseRegName(p);
            if (regName == REG_UNDEF || regName == REG_R0)
                return setError(UNKNOWN_OPERAND);
            p += _regs.regNameLen(regName);
            if (*p != ')')
                return setError(UNKNOWN_OPERAND);
            p++;
        } else {
            regName = REG_R0;
        }
    }
    uint16_t operand = (mode << 4) | _regs.encodeRegNumber(regName);
    if (destinationa) operand <<= 6;
    insn.embed(operand);
    if (emitInsn)
        insn.emitInsn();
    if (mode == 2)
        insn.emitOperand(val16);
    _scan = p;
    return getError();
}

Error AsmTms9900::encodeRel(InsnTms9900 &insn) {
    target::uintptr_t addr;
    if (getOperand16(addr)) return getError();
    if (getError() == UNDEFINED_SYMBOL) addr = insn.address();
    if (addr % 2) return setError(ILLEGAL_OPERAND);
    const target::uintptr_t base = insn.address() + 2;
    const target::ptrdiff_t delta = (addr - base) >> 1;
    if (delta >= 128 || delta < -128) return setError(OPERAND_TOO_FAR);
    insn.embed(static_cast<uint8_t>(delta));
    insn.emitInsn();
    return getError();
}

Error AsmTms9900::encodeCruOff(InsnTms9900 &insn) {
    uint8_t val8;
    if (getOperand8(val8)) return getError();
    insn.embed(val8);
    insn.emitInsn();
    return getError();
}

Error AsmTms9900::encode(Insn &_insn) {
    InsnTms9900 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);
    if (TableTms9900.searchName(insn))
        return setError(UNKNOWN_INSTRUCTION);
    _scan = skipSpaces(endName);

    Error error = OK;
    switch (insn.addrMode()) {
    case INH:
        insn.emitInsn();
        setError(OK);
        break;
    case IMM:
        encodeImm(insn, true);
        break;
    case REG:
        encodeReg(insn, true);
        break;
    case REG_IMM:
        error = encodeReg(insn, true);
        if (checkComma()) return setError(UNKNOWN_OPERAND);
        encodeImm(insn, false);
        break;
    case CNT_REG:
        error = encodeReg(insn, false);
        if (checkComma()) return setError(UNKNOWN_OPERAND);
        encodeCnt(insn, /* R0 */true, /* 16 */false);
        break;
    case SRC:
        encodeOpr(insn, true);
        break;
    case CNT_SRC:
        error = encodeOpr(insn, false);
        if (checkComma()) return setError(UNKNOWN_OPERAND);
        encodeCnt(insn, /* R0 */false, /* 16 */true);
        break;
    case XOP_SRC:
        error = encodeOpr(insn, false);
        if (checkComma()) return setError(UNKNOWN_OPERAND);
        encodeCnt(insn, /* R0 */false, /* 16 */false);
        break;
    case REG_SRC:
        error = encodeOpr(insn, false);
        if (checkComma()) return setError(UNKNOWN_OPERAND);
        encodeReg(insn, true);
        break;
    case DST_SRC:
        error = encodeOpr(insn, false);
        if (checkComma()) return setError(UNKNOWN_OPERAND);
        encodeOpr(insn, true, true);
        break;
    case REL:
        encodeRel(insn);
        break;
    case CRU_OFF:
        encodeCruOff(insn);
        break;
    default:
        return setError(INTERNAL_ERROR);
    }
    if (getError() == OK) setError(error);
    return checkLineEnd();
}

} // namespace tms9900
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
