#include "dis_z80.h"

#include "string_utils.h"
#include "table_z80.h"

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

Error DisZ80::readByte(Memory &memory, Insn &insn, target::byte_t &val) {
    if (!memory.hasNext()) return setError(NO_MEMORY);
    val = memory.readByte();
    insn.emitByte(val);
    return OK;
}

Error DisZ80::readUint16(Memory &memory, Insn &insn, target::uint16_t &val) {
    if (!memory.hasNext()) return setError(NO_MEMORY);
    val = memory.readByte();
    if (!memory.hasNext()) return setError(NO_MEMORY);
    val |= (target::uint16_t)memory.readByte() << 8;
    insn.emitUint16(val);
    return OK;
}

static char *outPtrName(char *out, RegName regName) {
    *out++ = '(';
    out = Registers::outRegName(out, regName);
    *out++ = ')';
    *out = 0;
    return out;
}

static char *outReg8Name(char *out, RegName regName) {
    return (regName == NONE) ? outPtrName(out, HL)
        : Registers::outRegName(out, regName);
}

Error DisZ80::decodeInherent(Insn& insn, char *operands, char *comments) {
    const target::opcode_t opc = InsnTable::opCode(insn.insnCode());
    switch (insn.leftFormat()) {
    case A_REG:
        operands = Registers::outRegName(operands, A);
        break;
    case REG8:
        if (insn.insnFormat() == DST_FMT
            || insn.insnFormat() == DST_SRC_FMT) {
            operands = outReg8Name(
                operands, Registers::decodeDataReg((opc >> 3) & 7));
        } else if (insn.insnFormat() == SRC_FMT) {
            operands = outReg8Name(
                operands, Registers::decodeDataReg(opc & 7));
        } else return setError(INTERNAL_ERROR);
        break;
    case IR_REG:
        operands = Registers::outRegName(
            operands, Registers::decodeIrReg(opc >> 3 & 1));
        break;
    case BC_REG:
        operands = Registers::outRegName(operands, BC);
        break;
    case DE_REG:
        operands = Registers::outRegName(operands, DE);
        break;
    case HL_REG:
        operands = Registers::outRegName(operands, HL);
        break;
    case AF_REG:
        operands = Registers::outRegName(operands, AF);
        break;
    case SP_REG:
        operands = Registers::outRegName(operands, SP);
        break;
    case REG16:
        operands = Registers::outRegName(
            operands, Registers::decodePointerReg((opc >> 4) & 3));
        break;
    case STK16:
        operands = Registers::outRegName(
            operands, Registers::decodeStackReg((opc >> 4 & 3)));
        break;
    case IX_REG:
        operands = Registers::outRegName(
            operands, InsnTable::decodeIndexReg(insn.insnCode()));
        break;
    case CC8:
        operands = Registers::outCc8Name(operands, (opc >> 3) & 7);
        break;
    case C_PTR:
        operands = outPtrName(operands, C);
        break;
    case BC_PTR:
        operands = outPtrName(
            operands, Registers::decodeIndexReg((opc >> 4) & 1));
        break;
    case HL_PTR:
        operands = outPtrName(operands, HL);
        break;
    case SP_PTR:
        operands = outPtrName(operands, SP);
        break;
    case IX_PTR:
        operands = outPtrName(
            operands, InsnTable::decodeIndexReg(insn.insnCode()));
        break;
    case VEC_NO:
        operands = outOpr8Hex(operands, opc & 0x38);
        break;
    case BIT_NO:
        operands = outInt16(operands, (opc >> 3) & 7);
        break;
    case IMM_NO:
        if ((opc & ~0x20) == 0x46) *operands++ = '0';
        else if ((opc & ~0x20) == 0x56) *operands++ = '1';
        else if ((opc & ~0x20) == 0x5E) *operands++ = '2';
        *operands = 0;
        break;
    default:
        break;
    }

    if (insn.leftFormat() != NO_OPR && insn.rightFormat() != NO_OPR)
        *operands++ = ',';

    switch (insn.rightFormat()) {
    case A_REG:
        operands = Registers::outRegName(operands, A);
        break;
    case REG8:
        if (insn.insnFormat() == DST_FMT) {
            operands = outReg8Name(
                operands, Registers::decodeDataReg((opc >> 3) & 7));
        } else if (insn.insnFormat() == SRC_FMT
                   || insn.insnFormat() == DST_SRC_FMT) {
            operands = outReg8Name(
                operands, Registers::decodeDataReg(opc & 7));
        } else return setError(INTERNAL_ERROR);
        break;
    case IR_REG:
        operands = Registers::outRegName(
            operands, Registers::decodeIrReg(opc >> 3 & 1));
        break;
    case AFPREG:
        operands = Registers::outRegName(operands, AFP);
        break;
    case HL_REG:
        operands = Registers::outRegName(operands, HL);
        break;
    case REG16:
        operands = Registers::outRegName(
            operands, Registers::decodePointerReg((opc >> 4) & 3));
        break;
    case IX_REG:
        operands = Registers::outRegName(
            operands, InsnTable::decodeIndexReg(insn.insnCode()));
        break;
    case REG16X:
        operands = Registers::outRegName(
            operands, Registers::decodePointerRegIx(
                (opc >> 4) & 3,
                InsnTable::decodeIndexReg(insn.insnCode())));
        break;
    case C_PTR:
        operands = outPtrName(operands, C);
        break;
    case BC_PTR:
        operands = outPtrName(
            operands, Registers::decodeIndexReg((opc >> 4) & 1));
        break;
    default:
        break;
    }

    return setError(OK);
}

Error DisZ80::decodeImmediate8(
    Insn &insn, char *operands, char *comments, target::byte_t val) {
    const target::opcode_t opc = InsnTable::opCode(insn.insnCode());
    switch (insn.leftFormat()) {
    case A_REG:
        operands = Registers::outRegName(operands, A);
        break;
    case REG8:
        operands = outReg8Name(
            operands, Registers::decodeDataReg((opc >> 3) & 7));
        break;
    default:
        break;
    }
    *operands++ = ',';
    operands = outOpr8Hex(operands, val);
    comments = outInt16(comments, val);
    return setError(OK);
}

Error DisZ80::decodeImmediate16(
    Insn &insn, char *operands, char *comments, target::uint16_t val) {
    const target::opcode_t opc = InsnTable::opCode(insn.insnCode());
    switch (insn.leftFormat()) {
    case REG16:
        operands = Registers::outRegName(
            operands, Registers::decodePointerReg((opc >> 4) & 3));
        break;
    case IX_REG:
        operands = Registers::outRegName(
            operands, InsnTable::decodeIndexReg(insn.insnCode()));
        break;
    default:
        break;
    }
    *operands++ = ',';
    operands = outOpr16Hex(operands, val);
    comments = outInt16(comments, val);
    return setError(OK);
}

static char *outAddr16(
    char *operands, char *comments, target::uintptr_t addr, const char *label,
    bool indir = true) {
    if (indir) *operands++ = '(';
    if (label) {
        operands = outStr(operands, label);
        comments = outOpr16Hex(comments, addr);
    } else {
        operands = outOpr16Hex(operands, addr);
    }
    if (indir) *operands++ = ')';
    *operands = *comments = 0;
    return operands;
}

Error DisZ80::decodeDirect(
    Insn &insn, char *operands, char *comments, target::uintptr_t addr) {
    const target::opcode_t opc = InsnTable::opCode(insn.insnCode());
    switch (insn.leftFormat()) {
    case ADDR16:
        operands = outAddr16(operands, comments, addr, lookup(addr));
        break;
    case HL_REG:
        operands = Registers::outRegName(operands, HL);
        break;
    case A_REG:
        operands = Registers::outRegName(operands, A);
        break;
    case CC8:
        operands = Registers::outCc8Name(operands, (opc >> 3) & 7);
        break;
    case IMM16:
        operands = outAddr16(operands, comments, addr, lookup(addr), false);
        break;
    case REG16:
        operands = Registers::outRegName(
            operands, Registers::decodePointerReg((opc >> 4) & 3));
        break;
    case IX_REG:
        operands = Registers::outRegName(
            operands, InsnTable::decodeIndexReg(insn.insnCode()));
        break;
    default:
        break;
    }
    if (insn.rightFormat() != NO_OPR) *operands++ = ',';
    switch (insn.rightFormat()) {
    case HL_REG:
        operands = Registers::outRegName(operands, HL);
        break;
    case ADDR16:
        operands = outAddr16(operands, comments, addr, lookup(addr));
        break;
    case A_REG:
        operands = Registers::outRegName(operands, A);
        break;
    case IMM16:
        operands = outAddr16(operands, comments, addr, lookup(addr), false);
        break;
    case REG16:
        operands = Registers::outRegName(
            operands, Registers::decodePointerReg((opc >> 4) & 3));
        break;
    case IX_REG:
        operands = Registers::outRegName(
            operands, InsnTable::decodeIndexReg(insn.insnCode()));
        break;
    default:
        break;
    }
    return setError(OK);
}

static char *outAddr8(
    char *operands, char *comments, target::byte_t addr, const char *label) {
    *operands++ = '(';
    if (label) {
        operands = outStr(operands, label);
        comments = outOpr8Hex(comments, addr);
    } else {
        operands = outOpr8Hex(operands, addr);
    }
    *operands++ = ')';
    *operands = *comments = 0;
    return operands;
}

Error DisZ80::decodeIoaddr(
    Insn &insn, char *operands, char *comments, target::byte_t ioaddr) {
    switch (insn.leftFormat()) {
    case ADDR8:
        operands = outAddr8(operands, comments, ioaddr, lookup(ioaddr));
        break;
    case A_REG:
        operands = Registers::outRegName(operands, A);
        break;
    default:
        break;
    }
    *operands++ = ',';
    switch (insn.rightFormat()) {
    case ADDR8:
        operands = outAddr8(operands, comments, ioaddr, lookup(ioaddr));
        break;
    case A_REG:
        operands = Registers::outRegName(operands, A);
        break;
    default:
        break;
    }
    return setError(OK);
}

Error DisZ80::decodeRelative(
    Insn &insn, char *operands, char *comments, target::int8_t delta) {
    if (insn.leftFormat() == CC4) {
        const target::opcode_t opc = InsnTable::opCode(insn.insnCode());
        operands = Registers::outCc4Name(operands, (opc >> 3) & 3);
        *operands++ = ',';
    }
    const target::uintptr_t addr = insn.address() + 2 + delta;
    const char *label = lookup(addr);
    if (label) {
        operands = outStr(operands, label);
        comments = outOpr16Hex(comments, addr);
    } else {
        operands = outOpr16Hex(operands, addr);
        if (delta >= 0) *comments++ = '+';
        comments = outInt16(comments, delta);
    }
    *operands = *comments = 0;
    return setError(OK);
}

static char *outIndexOffset(
    char *operands, target::insn_t insnCode, target::int8_t offset) {
    *operands++ = '(';
    operands = Registers::outRegName(
        operands, InsnTable::decodeIndexReg(insnCode));
    if (offset >= 0) *operands++ = '+';
    operands = outInt16(operands, offset);
    *operands++ = ')';
    *operands = 0;
    return operands;
}

Error DisZ80::decodeIndexed(
    Insn &insn, char *operands, char *comments, target::int8_t offset) {
    const target::opcode_t opc = InsnTable::opCode(insn.insnCode());
    switch (insn.leftFormat()) {
    case IX_OFF:
        operands = outIndexOffset(operands, insn.insnCode(), offset);
        break;
    case REG8:
        operands = outReg8Name(
            operands, Registers::decodeDataReg((opc >> 3) & 7));
        break;
    case A_REG:
        operands = Registers::outRegName(operands, A);
        break;
    default:
        break;
    }
    if (insn.rightFormat() != NO_OPR) *operands++ = ',';
    switch (insn.rightFormat()) {
    case IX_OFF:
        operands = outIndexOffset(operands, insn.insnCode(), offset);
        break;
    case REG8:
        operands = outReg8Name(
            operands, Registers::decodeDataReg(opc & 7));
        break;
    default:
        break;
    }
    return setError(OK);
}

Error DisZ80::decodeIndexedImmediate8(
    Insn &insn, char *operands, char *comments, target::int8_t offset,
    target::byte_t val) {
    operands = outIndexOffset(operands, insn.insnCode(), offset);
    *operands++ = ',';
    operands = outOpr8Hex(operands, val);
    comments = outInt16(comments, val);
    return setError(OK);
}

Error DisZ80::decodeIndexedBitOp(
    Insn &insn, char *operands, char *comments, target::int8_t offset,
    target::opcode_t opCode) {
    const target::opcode_t opc = InsnTable::opCode(insn.insnCode());
    Insn ixBit;
    ixBit.resetAddress(insn.address());
    ixBit.setInsnCode(InsnTable::insnCode(opc, opCode));
    if (InsnTable.searchInsnCode(ixBit)) return getError();
    insn.setName(ixBit.name());
    insn.setFlags(ixBit);

    const RegName regName = Registers::decodeDataReg(opCode & 7);
    switch (insn.leftFormat()) {
    case HL_PTR:
        operands = outIndexOffset(operands, insn.insnCode(), offset);
        break;
    case REG8:
        operands = (regName == NONE)
            ? outIndexOffset(operands, insn.insnCode(), offset)
            : outReg8Name(operands, regName);
        break;
    case BIT_NO:
        operands = outInt16(operands, (opCode >> 3) & 7);
        break;
    default:
        break;
    }
    if (insn.rightFormat() != NO_OPR) *operands++ = ',';

    if (insn.rightFormat() == HL_PTR) {
        operands = outIndexOffset(operands, insn.insnCode(), offset);
    } else if (insn.rightFormat() == REG8) {
        operands = (regName == NONE)
            ? outIndexOffset(operands, insn.insnCode(), offset)
            : outReg8Name(operands, regName);
    }
    return setError(OK);
}

Error DisZ80::decode(
    Memory &memory, Insn &insn, char *operands, char *comments, SymbolTable *symtab) {
    reset(symtab);
    insn.resetAddress(memory.address());
    *operands = *comments = 0;

    target::opcode_t opCode;
    if (readByte(memory, insn, opCode)) return getError();
    target::insn_t insnCode = opCode;
    if (InsnTable::isPrefixCode(opCode)) {
        const target::opcode_t prefix = opCode;
        if (readByte(memory, insn, opCode)) return getError();
        insnCode = InsnTable::insnCode(prefix, opCode);
    }
    insn.setInsnCode(insnCode);

    if (InsnTable.searchInsnCode(insn))
        return setError(UNKNOWN_INSTRUCTION);

    target::byte_t u8;
    target::uint16_t u16;
    target::byte_t offset;

    switch (insn.addrMode()) {
    case INHERENT:
        return decodeInherent(insn, operands, comments);
    case IMMEDIATE8:
        if (readByte(memory, insn, u8)) return getError();
        return decodeImmediate8(insn, operands, comments, u8);
    case IMMEDIATE16:
        if (readUint16(memory, insn, u16)) return getError();
        return decodeImmediate16(insn, operands, comments, u16);
    case DIRECT:
        if (readUint16(memory, insn, u16)) return getError();
        return decodeDirect(insn, operands, comments, u16);
    case IOADDR:
        if (readByte(memory, insn, u8)) return getError();
        return decodeIoaddr(insn, operands, comments, u8);
    case RELATIVE:
        if (readByte(memory, insn, u8)) return getError();
        return decodeRelative(insn, operands, comments, u8);
    case INDEXED:
        if (readByte(memory, insn, u8)) return getError();
        return decodeIndexed(insn, operands, comments, u8);
    case INDEXED_IMMEDIATE8:
        if (readByte(memory, insn, offset)) return getError();
        if (readByte(memory, insn, u8)) return getError();
        if (insn.leftFormat() == IX_BIT)
            return decodeIndexedBitOp(insn, operands, comments, offset, u8);
        return decodeIndexedImmediate8(insn, operands, comments, offset, u8);
    default:
        return setError(INTERNAL_ERROR);
    }
}
