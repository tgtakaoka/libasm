#include "dis_i8080.h"

#include "string_utils.h"
#include "table_i8080.h"

void Disassembler::outText(const char *text) {
    _operands = outStr(_operands, text);
}

void Disassembler::outOpr8Hex(uint8_t val) {
    char *out = _operands;
    if (val >= 0xA0) *out++ = '0';
    out = outHex8(out, val);
    *out++ = 'H';
    *(_operands = out) = 0;
}

void Disassembler::outOpr16Hex(uint16_t val) {
    char *out = _operands;
    if (val >= 0xA000) *out++ = '0';
    out = outHex16(out, val);
    *out++ = 'H';
    *(_operands = out) = 0;
}

void Disassembler::outOpr16Int(uint16_t val) {
    _operands = outInt16(_operands, val);
}

void Disassembler::outRegister(RegName regName) {
    _operands = Registers::outRegName(_operands, regName);
}

Error Disassembler::decodeImmediate8(Memory& memory, Insn &insn) {
    uint8_t val;
    if (insn.readByte(memory, val)) return setError(NO_MEMORY);
    if (insn.insnFormat() != NO_FORMAT) outChar(',');
    outOpr8Hex(val);
    return setError(OK);
}

Error Disassembler::decodeImmediate16(Memory& memory, Insn &insn) {
    uint16_t val;
    if (insn.readUint16(memory, val)) return setError(NO_MEMORY);
    if (insn.insnFormat() != NO_FORMAT) outChar(',');
    const char *label = lookup(val);
    if (label) {
        outText(label);
    } else {
        outOpr16Hex(val);
    }
    return setError(OK);
}

Error Disassembler::decodeDirect(Memory &memory, Insn& insn) {
    target::uintptr_t addr;
    if (insn.readUint16(memory, addr)) return setError(NO_MEMORY);
    const char *label = lookup(addr);
    if (label) {
        outText(label);
    } else {
        outOpr16Hex(addr);
    }
    return setError(OK);
}

Error Disassembler::decodeIoaddr(Memory &memory, Insn& insn) {
    uint8_t ioaddr;
    if (insn.readByte(memory, ioaddr)) return setError(NO_MEMORY);
    outOpr8Hex(ioaddr);
    return setError(OK);
}

Error Disassembler::decode(
    Memory &memory, Insn &insn, char *operands, SymbolTable *symtab) {
    reset(operands, symtab);
    insn.resetAddress(memory.address());

    target::insn_t insnCode;
    if (insn.readByte(memory, insnCode)) return setError(NO_MEMORY);
    insn.setInsnCode(insnCode);
    if (InsnTable.searchInsnCode(insn))
        return setError(UNKNOWN_INSTRUCTION);

    switch (insn.insnFormat()) {
    case POINTER_REG:
        outRegister(Registers::decodePointerReg((insnCode >> 4) & 3));
        break;
    case STACK_REG:
        outRegister(Registers::decodeStackReg((insnCode >> 4) & 3));
        break;
    case INDEX_REG:
        outRegister(Registers::decodeIndexReg((insnCode >> 4) & 1));
        break;
    case DATA_REG:
        outRegister(Registers::decodeDataReg((insnCode >> 3) & 7));
        break;
    case LOW_DATA_REG:
        outRegister(Registers::decodeDataReg(insnCode & 7));
        break;
    case DATA_DATA_REG:
        outRegister(Registers::decodeDataReg((insnCode >> 3) & 7));
        outChar(',');
        outRegister(Registers::decodeDataReg(insnCode & 7));
        break;
    case VECTOR_NO:
        outOpr16Int((insnCode >> 3) & 7);
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
