#include "dis_i8080.h"
#include "table_i8080.h"

void DisI8080::outRegister(RegName regName) {
    _operands = _regs.outRegName(_operands, regName);
}

Error DisI8080::decodeImmediate8(
    DisMemory<target::uintptr_t>& memory, Insn &insn) {
    uint8_t val;
    if (insn.readByte(memory, val)) return setError(NO_MEMORY);
    if (insn.insnFormat() != NO_FORMAT) *_operands++ = ',';
    outConstant(val);
    return setError(OK);
}

Error DisI8080::decodeImmediate16(
    DisMemory<target::uintptr_t>& memory, Insn &insn) {
    uint16_t val;
    if (insn.readUint16(memory, val)) return setError(NO_MEMORY);
    if (insn.insnFormat() != NO_FORMAT) *_operands++ = ',';
    const char *label = lookup(val);
    if (label) {
        outText(label);
    } else {
        outConstant(val);
    }
    return setError(OK);
}

Error DisI8080::decodeDirect(
    DisMemory<target::uintptr_t> &memory, Insn& insn) {
    target::uintptr_t addr;
    if (insn.readUint16(memory, addr)) return setError(NO_MEMORY);
    const char *label = lookup(addr);
    if (label) {
        outText(label);
    } else {
        outConstant(addr, 16, false);
    }
    return setError(OK);
}

Error DisI8080::decodeIoaddr(
    DisMemory<target::uintptr_t> &memory, Insn& insn) {
    uint8_t ioaddr;
    if (insn.readByte(memory, ioaddr)) return setError(NO_MEMORY);
    outConstant(ioaddr, 16, false);
    return setError(OK);
}

Error DisI8080::decode(
    DisMemory<target::uintptr_t> &memory, Insn &insn) {
    target::insn_t insnCode;
    if (insn.readByte(memory, insnCode)) return setError(NO_MEMORY);
    insn.setInsnCode(insnCode);
    if (TableI8080.searchInsnCode(insn))
        return setError(UNKNOWN_INSTRUCTION);

    switch (insn.insnFormat()) {
    case POINTER_REG:
        outRegister(RegI8080::decodePointerReg((insnCode >> 4) & 3));
        break;
    case STACK_REG:
        outRegister(RegI8080::decodeStackReg((insnCode >> 4) & 3));
        break;
    case INDEX_REG:
        outRegister(RegI8080::decodeIndexReg((insnCode >> 4) & 1));
        break;
    case DATA_REG:
        outRegister(RegI8080::decodeDataReg((insnCode >> 3) & 7));
        break;
    case LOW_DATA_REG:
        outRegister(RegI8080::decodeDataReg(insnCode & 7));
        break;
    case DATA_DATA_REG:
        outRegister(RegI8080::decodeDataReg((insnCode >> 3) & 7));
        *_operands++ = ',';
        outRegister(RegI8080::decodeDataReg(insnCode & 7));
        break;
    case VECTOR_NO:
        outConstant((insnCode >> 3) & 7);
        break;
    default:
        break;
    }

    switch (insn.addrMode()) {
    case INHR:
        return setError(OK);
    case IMM8:
        return decodeImmediate8(memory, insn);
    case IMM16:
        return decodeImmediate16(memory, insn);
    case DIRECT:
        return decodeDirect(memory, insn);
    case IOADR:
        return decodeIoaddr(memory, insn);
    default:
        return setError(INTERNAL_ERROR);
    }
}
