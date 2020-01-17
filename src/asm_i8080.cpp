#include <ctype.h>
#include <string.h>

#include "asm_i8080.h"

bool AsmI8080::acceptCpu(const char *cpu) {
    return strcasecmp(cpu, "8080") == 0;
}

Error AsmI8080::encodePointerReg(Insn &insn) {
    const RegName regName = RegI8080::parsePointerReg(_scan);
    const host::int_t num = RegI8080::encodePointerReg(regName);
    if (num < 0) return setError(UNKNOWN_REGISTER);
    insn.setInsnCode(insn.insnCode() | (num << 4));
    _scan += RegI8080::regNameLen(regName);
    return setError(OK);
}

Error AsmI8080::encodeStackReg(Insn &insn) {
    const RegName regName = RegI8080::parseStackReg(_scan);
    const host::int_t num = RegI8080::encodeStackReg(regName);
    if (num < 0) return setError(UNKNOWN_REGISTER);
    insn.setInsnCode(insn.insnCode() | (num << 4));
    _scan += RegI8080::regNameLen(regName);
    return setError(OK);
}

Error AsmI8080::encodeIndexReg(Insn &insn) {
    const RegName regName = RegI8080::parseIndexReg(_scan);
    const host::int_t num = RegI8080::encodeIndexReg(regName);
    if (num < 0) return setError(UNKNOWN_REGISTER);
    insn.setInsnCode(insn.insnCode() | (num << 4));
    _scan += RegI8080::regNameLen(regName);
    return setError(OK);
}

Error AsmI8080::encodeDataReg(Insn &insn) {
    const RegName regName = RegI8080::parseDataReg(_scan);
    const host::int_t num = RegI8080::encodeDataReg(regName);
    if (num < 0) return setError(UNKNOWN_REGISTER);
    if (insn.insnFormat() == DATA_REG)
        insn.setInsnCode(insn.insnCode() | (num << 3));
    if (insn.insnFormat() == LOW_DATA_REG)
        insn.setInsnCode(insn.insnCode() | num);
    _scan += RegI8080::regNameLen(regName);
    return setError(OK);
}

Error AsmI8080::encodeDataDataReg(Insn &insn) {
    const RegName dstReg = RegI8080::parseDataReg(_scan);
    if (dstReg == REG_UNDEF)
        return setError(UNKNOWN_REGISTER);
    _scan += RegI8080::regNameLen(dstReg);
    if (*_scan != ',') return setError(UNKNOWN_OPERAND);
    const RegName srcReg = RegI8080::parseDataReg(++_scan);
    if (srcReg == REG_UNDEF) return setError(UNKNOWN_REGISTER);
    _scan += RegI8080::regNameLen(srcReg);

    const host::uint_t dstNum = RegI8080::encodeDataReg(dstReg);
    const host::uint_t srcNum = RegI8080::encodeDataReg(srcReg);
    insn.setInsnCode(insn.insnCode() | (dstNum << 3) | srcNum);
    return setError(OK);
}

Error AsmI8080::encodeVectorNo(Insn &insn) {
    uint8_t vecNo;
    if (getOperand8(vecNo)) return getError();
    if (vecNo >= 8) return setError(OVERFLOW_RANGE);
    insn.setInsnCode(insn.insnCode() | (vecNo << 3));
    return setError(OK);
}

Error AsmI8080::encodeImmediate(Insn &insn) {
    if (insn.insnFormat() != NO_FORMAT && *_scan++ != ',')
        return setError(UNKNOWN_OPERAND);
    if (insn.addrMode() == IMM8) {
        uint8_t val8;
        if (getOperand8(val8)) return getError();
        insn.emitByte(val8);
    } else if (insn.addrMode() == IMM16) {
        uint16_t val16;
        if (getOperand16(val16)) return getError();
        insn.emitUint16(val16);
    }
    return checkLineEnd();
}

Error AsmI8080::encodeDirect(Insn &insn) {
    uint16_t addr;
    if (getOperand16(addr)) return getError();
    insn.emitUint16(addr);
    return checkLineEnd();
}

Error AsmI8080::encodeIoaddr(Insn &insn) {
    uint8_t addr;
    if (getOperand8(addr)) return getError();
    insn.emitByte(addr);
    return checkLineEnd();
}

Error AsmI8080::encode(Insn &insn) {
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);
    if (TableI8080.searchName(insn))
        return setError(UNKNOWN_INSTRUCTION);
    _scan = skipSpaces(endName);

    switch (insn.insnFormat()) {
    case NO_FORMAT:     setError(OK); break;
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

    insn.emitByte(insn.insnCode());
    switch (insn.addrMode()) {
    case INHR:   return checkLineEnd();
    case IMM8:
    case IMM16:  return encodeImmediate(insn);
    case DIRECT: return encodeDirect(insn);
    case IOADR:  return encodeIoaddr(insn);
    default:     return setError(UNKNOWN_OPERAND);
    }
}
