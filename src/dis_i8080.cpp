#include "dis_i8080.h"

#include "string_utils.h"
#include "table_i8080.h"

static char *outOpr8Hex(char *out, target::byte_t val) {
    if (val >= 0xA0) *out++ = '0';
    out = outHex8(out, val);
    *out++ = 'H';
    *out = 0;
    return out;
}

static char *outOpr16Hex(char *out, target::uint16_t val) {
    if (val >= 0xA000) *out++ = '0';
    out = outHex16(out, val);
    *out++ = 'H';
    *out = 0;
    return out;
}

Error Disassembler::readByte(Memory &memory, Insn &insn, target::byte_t &val) {
    if (!memory.hasNext()) return setError(NO_MEMORY);
    val = memory.readByte();
    insn.emitByte(val);
    return OK;
}

Error Disassembler::readUint16(Memory &memory, Insn &insn, target::uint16_t &val) {
    if (!memory.hasNext()) return setError(NO_MEMORY);
    val = memory.readByte();
    if (!memory.hasNext()) return setError(NO_MEMORY);
    val |= (target::uint16_t)memory.readByte() << 8;
    insn.emitUint16(val);
    return OK;
}

Error Disassembler::decodeImmediate8(
    Memory& memory, Insn &insn, char *operands) {
    target::byte_t val;
    if (readByte(memory, insn, val)) return getError();
    if (insn.insnFormat() != NO_FORMAT) *operands++ = ',';
    outOpr8Hex(operands, val);
    return setError(OK);
}

Error Disassembler::decodeImmediate16(
    Memory& memory, Insn &insn, char *operands) {
    target::uint16_t val;
    if (readUint16(memory, insn, val)) return getError();
    if (insn.insnFormat() != NO_FORMAT) *operands++ = ',';
    const char *label = lookup(val);
    if (label) {
        outStr(operands, label);
    } else {
        outOpr16Hex(operands, val);
    }
    return setError(OK);
}

Error Disassembler::decodeDirect(
    Memory &memory, Insn& insn, char *operands) {
    target::uintptr_t addr;
    if (readUint16(memory, insn, addr)) return getError();
    const char *label = lookup(addr);
    if (label) {
        outStr(operands, label);
    } else {
        outOpr16Hex(operands, addr);
    }
    return setError(OK);
}

Error Disassembler::decodeIoaddr(
    Memory &memory, Insn& insn, char *operands) {
    target::byte_t ioaddr;
    if (readByte(memory, insn, ioaddr)) return getError();
    outOpr8Hex(operands, ioaddr);
    return setError(OK);
}

Error Disassembler::decode(
    Memory &memory, Insn &insn, char *operands, SymbolTable *symtab) {
    reset(symtab);
    insn.resetAddress(memory.address());
    *operands = 0;

    target::insn_t insnCode;
    if (readByte(memory, insn, insnCode)) return getError();
    insn.setInsnCode(insnCode);
    if (InsnTable.searchInsnCode(insn))
        return setError(UNKNOWN_INSTRUCTION);

    switch (insn.insnFormat()) {
    case POINTER_REG:
        operands = Registers::outRegName(operands, Registers::decodePointerReg((insnCode >> 4) & 3));
        break;
    case STACK_REG:
        operands = Registers::outRegName(operands, Registers::decodeStackReg((insnCode >> 4) & 3));
        break;
    case INDEX_REG:
        operands = Registers::outRegName(operands, Registers::decodeIndexReg((insnCode >> 4) & 1));
        break;
    case DATA_REG:
        operands = Registers::outRegName(operands, Registers::decodeDataReg((insnCode >> 3) & 7));
        break;
    case LOW_DATA_REG:
        operands = Registers::outRegName(operands, Registers::decodeDataReg(insnCode & 7));
        break;
    case DATA_DATA_REG:
        operands = Registers::outRegName(operands, Registers::decodeDataReg((insnCode >> 3) & 7));
        *operands++ = ',';
        operands = Registers::outRegName(operands, Registers::decodeDataReg(insnCode & 7));
        break;
    case VECTOR_NO:
        operands = outInt16(operands, (insnCode >> 3) & 7);
        break;
    default:
        break;
    }

    switch (insn.addrMode()) {
    case INHR:
        return setError(OK);
    case IMM8:
        return decodeImmediate8(memory, insn, operands);
    case IMM16:
        return decodeImmediate16(memory, insn, operands);
    case DIRECT:
        return decodeDirect(memory, insn, operands);
    case IOADR:
        return decodeIoaddr(memory, insn, operands);
    default:
        return setError(INTERNAL_ERROR);
    }
}
