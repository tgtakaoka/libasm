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

#include "asm_i8080.h"
#include "table_i8080.h"

namespace libasm {
namespace i8080 {

Error AsmI8080::encodePointerReg(InsnI8080 &insn) {
    const RegName reg = RegI8080::parseRegName(_scan);
    if (!RegI8080::isPointerReg(reg)) return setError(UNKNOWN_REGISTER);
    insn.embed(RegI8080::encodePointerReg(reg));
    _scan += RegI8080::regNameLen(reg);
    return OK;
}

Error AsmI8080::encodeStackReg(InsnI8080 &insn) {
    const RegName reg = RegI8080::parseRegName(_scan);
    if (!RegI8080::isStackReg(reg)) return setError(UNKNOWN_REGISTER);
    insn.embed(RegI8080::encodeStackReg(reg));
    _scan += RegI8080::regNameLen(reg);
    return OK;
}

Error AsmI8080::encodeIndexReg(InsnI8080 &insn) {
    const RegName reg = RegI8080::parseRegName(_scan);
    if (!RegI8080::isIndexReg(reg)) return setError(UNKNOWN_REGISTER);
    insn.embed(RegI8080::encodeIndexReg(reg));
    _scan += RegI8080::regNameLen(reg);
    return setOK();
}

Error AsmI8080::encodeDataReg(InsnI8080 &insn) {
    const RegName reg = RegI8080::parseRegName(_scan);
    if (!RegI8080::isDataReg(reg)) return setError(UNKNOWN_REGISTER);
    const uint8_t num = RegI8080::encodeDataReg(reg);
    if (insn.insnFormat() == DATA_REG)
        insn.embed(num << 3);
    if (insn.insnFormat() == LOW_DATA_REG)
        insn.embed(num);
    _scan += RegI8080::regNameLen(reg);
    return OK;
}

Error AsmI8080::encodeDataDataReg(InsnI8080 &insn) {
    const char *p = _scan;
    const RegName dst = RegI8080::parseRegName(p);
    if (!RegI8080::isDataReg(dst)) return setError(UNKNOWN_REGISTER);
    const uint8_t dstNum = RegI8080::encodeDataReg(dst);
    p = skipSpaces(p + RegI8080::regNameLen(dst));
    if (*p != ',') return setError(MISSING_COMMA);
    p = skipSpaces(p + 1);
    const RegName src = RegI8080::parseRegName(p);
    if (!RegI8080::isDataReg(src)) return setError(UNKNOWN_REGISTER);
    const uint8_t srcNum = RegI8080::encodeDataReg(src);
    _scan = skipSpaces(p + RegI8080::regNameLen(src));

    insn.embed(dstNum << 3);
    insn.embed(srcNum);
    return OK;
}

Error AsmI8080::encodeVectorNo(InsnI8080 &insn) {
    uint8_t vecNo;
    if (getOperand(vecNo)) return getError();
    if (vecNo >= 8) return setError(OVERFLOW_RANGE);
    insn.embed(vecNo << 3);
    return OK;
}

Error AsmI8080::encodeImmediate(InsnI8080 &insn) {
    if (insn.insnFormat() != NO_FORMAT) {
        const char *p = skipSpaces(_scan);
        if (*p != ',') return setError(MISSING_COMMA);
        _scan = p + 1;
    }
    if (insn.addrMode() == IMM8) {
        uint8_t val8;
        if (getOperand(val8)) return getError();
        insn.emitByte(val8);
    } else if (insn.addrMode() == IMM16) {
        uint16_t val16;
        if (getOperand(val16)) return getError();
        insn.emitUint16(val16);
    }
    return checkLineEnd();
}

Error AsmI8080::encodeDirect(InsnI8080 &insn) {
    uint16_t addr;
    if (getOperand(addr)) return getError();
    insn.emitUint16(addr);
    return checkLineEnd();
}

Error AsmI8080::encodeIoaddr(InsnI8080 &insn) {
    uint8_t addr;
    if (getOperand(addr)) return getError();
    insn.emitByte(addr);
    return checkLineEnd();
}

Error AsmI8080::encode(Insn &_insn) {
    InsnI8080 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);

    if (TableI8080.searchName(insn))
        return setError(TableI8080.getError());
    _scan = skipSpaces(endName);

    switch (insn.insnFormat()) {
    case NO_FORMAT:     setOK(); break;
    case POINTER_REG:   encodePointerReg(insn); break;
    case STACK_REG:     encodeStackReg(insn); break;
    case INDEX_REG:     encodeIndexReg(insn); break;
    case DATA_REG:
    case LOW_DATA_REG:  encodeDataReg(insn); break;
    case DATA_DATA_REG: encodeDataDataReg(insn); break;
    case VECTOR_NO:     encodeVectorNo(insn); break;
    default: break;
    }
    if (getError()) return getError();

    insn.emitByte(insn.opCode());
    switch (insn.addrMode()) {
    case INHR:   return checkLineEnd();
    case IMM8:
    case IMM16:  return encodeImmediate(insn);
    case DIRECT: return encodeDirect(insn);
    case IOADR:  return encodeIoaddr(insn);
    default:     return setError(UNKNOWN_OPERAND);
    }
}

} // namespace i8080
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
