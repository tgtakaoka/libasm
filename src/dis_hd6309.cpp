#include "dis_hd6309.h"

#include "string_utils.h"
#include "table_hd6309.h"

static char *outOpr8Hex(char *out, target::byte_t val) {
    *out++ = '$';
    return outHex8(out, val);
}

static char *outOpr16Hex(char *out, target::uint16_t val) {
    *out++ = '$';
    return outHex16(out, val);
}

static char *outOpr32Hex(char *out, uint32_t val) {
    *out++ = '$';
    return outHex32(out, val);
}

Error DisHd6309::readByte(Memory &memory, Insn &insn, target::byte_t &val) {
    if (!memory.hasNext()) return setError(NO_MEMORY);
    val = memory.readByte();
    insn.emitByte(val);
    return OK;
}

Error DisHd6309::readUint16(Memory &memory, Insn &insn, target::uint16_t &val) {
    if (!memory.hasNext()) return setError(NO_MEMORY);
    val = (target::uint16_t)memory.readByte() << 8;
    if (!memory.hasNext()) return setError(NO_MEMORY);
    val |= memory.readByte();
    insn.emitUint16(val);
    return OK;
}

Error DisHd6309::readUint32(Memory &memory, Insn &insn, target::uint32_t &val) {
    if (!memory.hasNext()) return setError(NO_MEMORY);
    val = (target::uint32_t)memory.readByte() << 24;
    if (!memory.hasNext()) return setError(NO_MEMORY);
    val |= (target::uint32_t)memory.readByte() << 16;
    if (!memory.hasNext()) return setError(NO_MEMORY);
    val |= (target::uint16_t)memory.readByte() << 8;
    if (!memory.hasNext()) return setError(NO_MEMORY);
    val |= memory.readByte();
    insn.emitUint32(val);
    return OK;
}

Error DisHd6309::decodeDirectPage(
    Memory &memory, Insn& insn, char *operands, char *comments) {
    target::byte_t dir;
    if (readByte(memory, insn, dir)) return getError();
    const char *label = lookup(dir);
    if (label) {
        *operands++ = '<';
        outStr(operands, label);
        outOpr8Hex(comments, dir);
    } else {
        outOpr8Hex(operands, dir);
    }
    return setError(OK);
}

Error DisHd6309::decodeExtended(
    Memory& memory, Insn &insn, char *operands, char *comments) {
    target::uintptr_t addr;
    if (readUint16(memory, insn, addr)) return getError();
    const char *label = lookup(addr);
    if (label) {
        if (addr < 0x100) *operands++ = '>';
        outStr(operands, label);
        outOpr16Hex(comments, addr);
    } else {
        outOpr16Hex(operands, addr);
    }
    return setError(OK);
}

Error DisHd6309::decodeIndexed(
    Memory &memory, Insn &insn, char *operands, char *comments) {
    target::byte_t post;
    if (readByte(memory, insn, post)) return getError();
    const target::byte_t mode = post & 0x8F;

    target::byte_t indir = (post & 0x10);
    const char *label = nullptr;
    target::uintptr_t addr = 0;
    target::intptr_t offset = 0;
    host::int_t offSize = 0;
    RegName base = _regs.decodeBaseReg((post >> 5) & 3);
    RegName index = NONE;
    host::int_t incr = 0;

    if (mode == 0x84) {
        // ,R [,R]
        ;
    } else if (_regs.isHd6309() && (post == 0x8F || post == 0x90)) {
        // ,W [,W]
        base = W;
    } else if (_regs.isHd6309() && (post == 0xAF || post == 0xB0)) {
        // n16,W [n16,W]
        base = W;
        offSize = 16;
        if (readUint16(memory, insn, addr)) return getError();
        offset = static_cast<target::int16_t>(addr);
    } else if (_regs.isHd6309()
               && (post == 0xCF || post == 0xD0 || post == 0xEF || post == 0xF0)) {
        // ,W++ ,--W [,W++] [,--W]
        base = W;
        incr = (post < 0xE0) ? 2 : -2;
    } else if ((post & 0x80) == 0) {
        // n5,R
        offSize = 5;
        addr = post & 0x1F;
        if (post & 0x10) addr |= 0xFFE0;
        offset = static_cast<int16_t>(addr);
        indir = 0;
    } else if (mode == 0x88) {
        // n8,R [n8,R]
        offSize = 8;
        target::byte_t val;
        if (readByte(memory, insn, val)) return getError();
        addr = val;
        offset = static_cast<target::int8_t>(val);
    } else if (mode == 0x89) {
        // n16,R [n16,R]
        offSize = 16;
        if (readUint16(memory, insn, addr)) return getError();
        offset = static_cast<target::int16_t>(addr);
    } else if (_regs.decodeIndexReg(post & 0xf) != NONE) {
        // R,R [R,R]
        index = _regs.decodeIndexReg(mode & 0xf);
    } else if (mode == 0x80) {
        // ,R+
        if (indir) return setError(UNKNOWN_POSTBYTE);
        incr = 1;
    } else if (mode == 0x82) {
        // ,-R
        if (indir) return setError(UNKNOWN_POSTBYTE);
        incr = -1;
    } else if (mode == 0x81 || mode == 0x83) {
        // ,R++ ,--R, [,R++] [,--R]
        incr = (mode == 0x81) ? 2 : -2;
    } else if (mode == 0x8C || mode == 0x8D) {
        // [n8,PC] [n16,PC]
        base = PC;
        offSize = -1;
        if (mode == 0x8C) {
            target::byte_t val;
            if (readByte(memory, insn, val)) return getError();
            offset = static_cast<target::int8_t>(val);
        } else {
            target::uint16_t val;
            if (readUint16(memory, insn, val)) return getError();
            offset = static_cast<target::int16_t>(val);
        }
        addr = insn.address() + insn.insnLen() + offset;
        label = lookup(addr);
    } else if (post == 0x9F) {
        // [n16]
        base = NONE;
        if (readUint16(memory, insn, addr)) return getError();
        label = lookup(addr);
        offSize = 16;
    } else {
        return setError(UNKNOWN_POSTBYTE);
    }

    if (indir) *operands++ = '[';
    if (label) {
        operands = outStr(operands, label);
        comments = outOpr16Hex(comments, addr);
    } else {
        if (base) {
            if (index) {
                operands = Registers::outRegName(operands, index);
            } else if (offSize < 0) {
                operands = outOpr16Hex(operands, addr);
                comments = outInt16(comments, offset);
            } else {
                if (offSize != 0) operands = outInt16(operands, offset);
                switch (offSize) {
                case 16:
                    comments = outOpr16Hex(comments, addr); break;
                case 8:
                    comments = outOpr8Hex(comments, addr); break;
                case 5:
                    break;
                }
            }
        } else {
            operands = outOpr16Hex(operands, addr);
        }
    }
    if (base) {
        *operands++ = ',';
        for (; incr < 0; incr++) *operands++ = '-';
        operands = Registers::outRegName(operands, base);
        for (; incr > 0; incr--) *operands++ = '+';
    }
    if (indir) *operands++ = ']';
    *operands = 0;
    *comments = 0;
    return setError(OK);
}

Error DisHd6309::decodeRelative(
    Memory &memory, Insn &insn, char *operands, char *comments) {
    target::ptrdiff_t delta;
    if (insn.oprLen() == 1) {
        target::byte_t val;
        if (readByte(memory, insn, val)) return getError();
        delta = static_cast<target::int8_t>(val);
    } else {
        target::uint16_t val;
        if (readUint16(memory, insn, val)) return getError();
        delta = static_cast<target::ptrdiff_t>(val);
    }
    const target::uintptr_t addr = insn.address() + insn.insnLen() + delta;
    const char *label = lookup(addr);
    if (label) {
        outStr(operands, label);
        comments = outOpr16Hex(comments, addr);
        *comments++ = ' '; *comments++ = '(';
        if (delta >= 0) *comments++ = '+';
        comments = outInt16(comments, delta);
        *comments++ = ')';
        *comments = 0;
    } else {
        outOpr16Hex(operands, addr);
        if (delta >= 0) *comments++ = '+';
        outInt16(comments, delta);
    }
    return setError(OK);
}

Error DisHd6309::decodeImmediate(
    Memory& memory, Insn &insn, char *operands, char *comments) {
    if (insn.oprLen() == 1) {
        target::byte_t val;
        if (readByte(memory, insn, val)) return getError();
        *operands++ = '#';
        outOpr8Hex(operands, val);
        if (insn.insnCode() == 0x1A) {
            _regs.outCCRBits(comments, val);
        } else if (insn.insnCode() == 0x1C || insn.insnCode() == 0x3C) {
            if (val != 0xFF) {
                *comments++ = '~';
                Registers::outCCRBits(comments, ~val);
            }
        } else {
            outInt16(comments, val);
        }
    } else if (insn.oprLen() == 2) {
        target::uint16_t val;
        if (readUint16(memory, insn, val)) return getError();
        const char *label = lookup(val);
        *operands++ = '#';
        if (label) {
            outStr(operands, label);
            outOpr16Hex(comments, val);
        } else {
            outOpr16Hex(operands, val);
            outInt16(comments, val);
        }
    } else if (_regs.isHd6309() && insn.oprLen() == 4) {
        target::uint32_t val;
        if (readUint32(memory, insn, val)) return getError();
        *operands++ = '#';
        outOpr32Hex(operands, val);
        outInt32(comments, val);
    } else {
        return setError(UNKNOWN_INSTRUCTION);
    }
    return setError(OK);
}

Error DisHd6309::decodeStackOp(
    Memory &memory, Insn &insn, char *operands, char *comments) {
    target::byte_t post;
    if (readByte(memory, insn, post)) return getError();
    if (post == 0) {
        *operands++ = '#';
        outOpr8Hex(operands, post);
        return setError(OK);
    }
    const bool push = (insn.insnCode() & 1) == 0;
    for (host::uint_t i = 0, n = 0; i < 8; i++) {
        const host::uint_t bit = push ? 7 - i : i;
        if (post & (1 << bit)) {
            if (n != 0) *operands++ = ',';
            operands = Registers::outRegName(
                operands, Registers::getStackReg(bit, insn.insnCode()));
            n++;
        }
    }
    *operands = 0;
    return setError(OK);
}

Error DisHd6309::decodeRegisters(
    Memory &memory, Insn &insn, char *operands, char *comments) {
    target::byte_t post;
    if (readByte(memory, insn, post)) return getError();
    const RegName src = _regs.decodeRegName(post >> 4);
    const RegName dst = _regs.decodeRegName(post & 0xf);
    if (src == NONE || dst == NONE) return setError(ILLEGAL_REGISTER);
    operands = Registers::outRegName(operands, src);
    *operands++ = ',';
    Registers::outRegName(operands, dst);
    return setError(OK);
}

Error DisHd6309::decodeImmediatePlus(
    Memory& memory, Insn &insn, char *operands, char *comments) {
    *operands++ = '#';
    target::byte_t val;
    if (readByte(memory, insn, val)) return getError();
    operands = outOpr8Hex(operands, val);
    *operands++ = ',';
    switch (insn.addrMode()) {
    case IMM_DIRECT:
        return decodeDirectPage(memory, insn, operands, comments);
    case IMM_EXTENDED:
        return decodeExtended(memory, insn, operands, comments);
    case IMM_INDEXED:
        return decodeIndexed(memory, insn, operands, comments);
    default:
        return setError(INTERNAL_ERROR);
    }
}

Error DisHd6309::decodeBitOperation(
    Memory &memory, Insn &insn, char *operands, char *comments) {
    target::byte_t post;
    if (readByte(memory, insn, post)) return getError();
    const RegName reg = Registers::decodeBitOpReg(post >> 6);
    if (reg == NONE) return setError(ILLEGAL_REGISTER);
    operands = Registers::outRegName(operands, reg);
    *operands++ = ',';
    const host::uint_t dstBit = (post >> 3) & 0x7;
    operands = outInt16(operands, dstBit);
    *operands++ = ',';
    operands = outInt16(operands, post & 0x7);
    if (reg == 'C') {
        comments = Registers::outRegName(comments, reg);
        *comments++ = '.';
        comments = Registers::outCCRBits(comments, 1 << dstBit);
        *comments++ = ' ';
        *comments = 0;
    }
    *operands++ = ',';
    return decodeDirectPage(memory, insn, operands, comments);
}

Error DisHd6309::decodeTransferMemory(
    Memory &memory, Insn &insn, char *operands, char *comments) {
    target::byte_t post;
    if (readByte(memory, insn, post)) return getError();
    const RegName src = Registers::decodeTfmBaseReg(post >> 4);
    const RegName dst = Registers::decodeTfmBaseReg(post & 0xf);
    const uint8_t mode = insn.insnCode() & 0x3;
    if (src == NONE || dst == NONE) return setError(ILLEGAL_REGISTER);
    operands = Registers::outRegName(operands, src);
    const char srcModeChar = Registers::tfmSrcModeChar(mode);
    if (srcModeChar) *operands++ = srcModeChar;
    *operands++ = ',';
    operands = Registers::outRegName(operands, dst);
    const char dstModeChar = Registers::tfmDstModeChar(mode);
    if (dstModeChar) *operands++ = dstModeChar;
    *operands = 0;
    return setError(OK);
}

Error DisHd6309::decode(
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

    if (insn.mcuMode() == HD6309 && _regs.isMc6809())
        return setError(UNKNOWN_INSTRUCTION);

    switch (insn.addrMode()) {
    case INHERENT:
        return setError(OK);
    case DIRECT_PG:
        return decodeDirectPage(memory, insn, operands, comments);
    case EXTENDED:
        return decodeExtended(memory, insn, operands, comments);
    case INDEXED:
        return decodeIndexed(memory, insn, operands, comments);
    case RELATIVE:
        return decodeRelative(memory, insn, operands, comments);
    case IMMEDIATE:
        return decodeImmediate(memory, insn, operands, comments);
    case STACK_OP:
        return decodeStackOp(memory, insn, operands, comments);
    case REGISTERS:
        return decodeRegisters(memory, insn, operands, comments);
    case IMM_DIRECT:
    case IMM_EXTENDED:
    case IMM_INDEXED:
        return decodeImmediatePlus(memory, insn, operands, comments);
    case BIT_OPERATION:
        return decodeBitOperation(memory, insn, operands, comments);
    case TRANSFER_MEM:
        return decodeTransferMemory(memory, insn, operands, comments);
    default:
        return setError(INTERNAL_ERROR);
    }
}

#if 0
static const char T_FCB[]   PROGMEM = "FCB";

Error DisHd6309::formConstantBytes(
    Memory &memory, Symtab *symtab, char *operands, char *comments,
    uint8_t len) {
    reset(memory.address(), symtab);
    *operands = *comments = 0;

    strcpy_P(_name, T_FCB);
    for (uint8_t i = 0; i < len; i++) {
        if (readUint8(memory)) break;
        const uint8_t val = asUint8();
        const char *label = lookup(val);
        if (i != 0) {
            *operands++ = ',';
            *comments++ = ',';
        }
        if (label) {
            operands = outStr(operands, label);
            comments = outInt16(comments, val);
        } else {
            operands = outOpr8Hex(operands, val);
            comments = outInt16(comments, val);
        }
    }
    return setError(memory);
}
#endif
