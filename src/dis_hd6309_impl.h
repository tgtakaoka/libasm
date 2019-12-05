/* -*- mode: c++; -*- */
#ifndef __DIS_HD6309_IMPL_H__
#define __DIS_HD6309_IMPL_H__

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

template<McuType mcuType>
Error Disassembler<mcuType>::decodeDirectPage(
    Memory &memory, Insn& insn, char *operands) {
    target::byte_t dir;
    if (insn.readByte(memory, dir)) return setError(NO_MEMORY);
    const char *label = lookup(dir);
    if (label) {
        *operands++ = '<';
        outStr(operands, label);
    } else {
        outOpr8Hex(operands, dir);
    }
    return setError(OK);
}

template<McuType mcuType>
Error Disassembler<mcuType>::decodeExtended(
    Memory& memory, Insn &insn, char *operands) {
    target::uintptr_t addr;
    if (insn.readUint16(memory, addr)) return setError(NO_MEMORY);
    const char *label = lookup(addr);
    if (label) {
        if (addr < 0x100) *operands++ = '>';
        outStr(operands, label);
    } else {
        outOpr16Hex(operands, addr);
    }
    return setError(OK);
}

template<McuType mcuType>
Error Disassembler<mcuType>::decodeIndexed(
    Memory &memory, Insn &insn, char *operands) {
    target::byte_t post;
    if (insn.readByte(memory, post)) return setError(NO_MEMORY);
    const target::byte_t mode = post & 0x8F;

    target::byte_t indir = (post & 0x10);
    const char *label = nullptr;
    target::uintptr_t addr = 0;
    target::ptrdiff_t offset = 0;
    host::int_t offSize = 0;
    RegName base = _regs.decodeBaseReg((post >> 5) & 3);
    RegName index = REG_UNDEF;
    host::int_t incr = 0;

    if (mode == 0x84) {
        // ,R [,R]
        ;
    } else if (mcuType == HD6309 && (post == 0x8F || post == 0x90)) {
        // ,W [,W]
        base = REG_W;
    } else if (mcuType == HD6309 && (post == 0xAF || post == 0xB0)) {
        // n16,W [n16,W]
        base = REG_W;
        offSize = 16;
        if (insn.readUint16(memory, addr)) return setError(NO_MEMORY);
        offset = static_cast<target::int16_t>(addr);
    } else if (mcuType == HD6309
               && (post == 0xCF || post == 0xD0 || post == 0xEF || post == 0xF0)) {
        // ,W++ ,--W [,W++] [,--W]
        base = REG_W;
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
        if (insn.readByte(memory, val)) return setError(NO_MEMORY);
        addr = val;
        offset = static_cast<target::int8_t>(val);
    } else if (mode == 0x89) {
        // n16,R [n16,R]
        offSize = 16;
        if (insn.readUint16(memory, addr)) return setError(NO_MEMORY);
        offset = static_cast<target::int16_t>(addr);
    } else if (_regs.decodeIndexReg(post & 0xf) != REG_UNDEF) {
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
        base = REG_PC;
        offSize = -1;
        if (mode == 0x8C) {
            target::byte_t val;
            if (insn.readByte(memory, val)) return setError(NO_MEMORY);
            offset = static_cast<target::int8_t>(val);
        } else {
            target::uint16_t val;
            if (insn.readUint16(memory, val)) return setError(NO_MEMORY);
            offset = static_cast<target::int16_t>(val);
        }
        addr = insn.address() + insn.insnLen() + offset;
        label = lookup(addr);
    } else if (post == 0x9F) {
        // [n16]
        base = REG_UNDEF;
        if (insn.readUint16(memory, addr)) return setError(NO_MEMORY);
        label = lookup(addr);
        offSize = 16;
    } else {
        return setError(UNKNOWN_POSTBYTE);
    }

    if (indir) *operands++ = '[';
    if (label) {
        operands = outStr(operands, label);
    } else {
        if (base) {
            if (index) {
                operands = _regs.outRegName(operands, index);
            } else if (offSize < 0) {
                operands = outOpr16Hex(operands, addr);
            } else {
                if (offSize != 0) operands = outInt16(operands, offset);
            }
        } else {
            operands = outOpr16Hex(operands, addr);
        }
    }
    if (base) {
        *operands++ = ',';
        for (; incr < 0; incr++) *operands++ = '-';
        operands = _regs.outRegName(operands, base);
        for (; incr > 0; incr--) *operands++ = '+';
    }
    if (indir) *operands++ = ']';
    *operands = 0;
    return setError(OK);
}

template<McuType mcuType>
Error Disassembler<mcuType>::decodeRelative(
    Memory &memory, Insn &insn, char *operands) {
    target::ptrdiff_t delta;
    if (insn.addrMode() == REL8) {
        target::byte_t val;
        if (insn.readByte(memory, val)) return setError(NO_MEMORY);
        delta = static_cast<target::int8_t>(val);
    } else {
        target::uint16_t val;
        if (insn.readUint16(memory, val)) return setError(NO_MEMORY);
        delta = static_cast<target::ptrdiff_t>(val);
    }
    const target::uintptr_t addr = insn.address() + insn.insnLen() + delta;
    const char *label = lookup(addr);
    if (label) {
        outStr(operands, label);
    } else {
        outOpr16Hex(operands, addr);
    }
    return setError(OK);
}

template<McuType mcuType>
Error Disassembler<mcuType>::decodeImmediate(
    Memory& memory, Insn &insn, char *operands) {
    if (insn.addrMode() == IMM8) {
        target::byte_t val;
        if (insn.readByte(memory, val)) return setError(NO_MEMORY);
        *operands++ = '#';
        outOpr8Hex(operands, val);
    } else if (insn.addrMode() == IMM16) {
        target::uint16_t val;
        if (insn.readUint16(memory, val)) return setError(NO_MEMORY);
        const char *label = lookup(val);
        *operands++ = '#';
        if (label) {
            outStr(operands, label);
        } else {
            outOpr16Hex(operands, val);
        }
    } else if (mcuType == HD6309 && insn.addrMode() == IMM32) {
        target::uint32_t val;
        if (insn.readUint32(memory, val)) return setError(NO_MEMORY);
        *operands++ = '#';
        outOpr32Hex(operands, val);
    } else {
        return setError(UNKNOWN_INSTRUCTION);
    }
    return setError(OK);
}

template<McuType mcuType>
Error Disassembler<mcuType>::decodeStackOp(
    Memory &memory, Insn &insn, char *operands) {
    target::byte_t post;
    if (insn.readByte(memory, post)) return setError(NO_MEMORY);
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
            operands = _regs.outRegName(
                operands, _regs.getStackReg(bit, insn.insnCode()));
            n++;
        }
    }
    *operands = 0;
    return setError(OK);
}

template<McuType mcuType>
Error Disassembler<mcuType>::decodeRegisters(
    Memory &memory, Insn &insn, char *operands) {
    target::byte_t post;
    if (insn.readByte(memory, post)) return setError(NO_MEMORY);
    const RegName src = _regs.decodeRegName(post >> 4);
    const RegName dst = _regs.decodeRegName(post & 0xf);
    if (src == REG_UNDEF || dst == REG_UNDEF) return setError(ILLEGAL_REGISTER);
    operands = _regs.outRegName(operands, src);
    *operands++ = ',';
    _regs.outRegName(operands, dst);
    return setError(OK);
}

template<McuType mcuType>
Error Disassembler<mcuType>::decodeImmediatePlus(
    Memory& memory, Insn &insn, char *operands) {
    *operands++ = '#';
    target::byte_t val;
    if (insn.readByte(memory, val)) return setError(NO_MEMORY);
    operands = outOpr8Hex(operands, val);
    *operands++ = ',';
    switch (insn.addrMode()) {
    case IMMDIR:   return decodeDirectPage(memory, insn, operands);
    case IMMEXT: return decodeExtended(memory, insn, operands);
    case IMMIDX:  return decodeIndexed(memory, insn, operands);
    default:           return setError(INTERNAL_ERROR);
    }
}

template<McuType mcuType>
Error Disassembler<mcuType>::decodeBitOperation(
    Memory &memory, Insn &insn, char *operands) {
    target::byte_t post;
    if (insn.readByte(memory, post)) return setError(NO_MEMORY);
    const RegName reg = _regs.decodeBitOpReg(post >> 6);
    if (reg == REG_UNDEF) return setError(ILLEGAL_REGISTER);
    operands = _regs.outRegName(operands, reg);
    *operands++ = ',';
    const host::uint_t dstBit = (post >> 3) & 0x7;
    operands = outInt16(operands, dstBit);
    *operands++ = ',';
    operands = outInt16(operands, post & 0x7);
    *operands++ = ',';
    return decodeDirectPage(memory, insn, operands);
}

template<McuType mcuType>
Error Disassembler<mcuType>::decodeTransferMemory(
    Memory &memory, Insn &insn, char *operands) {
    target::byte_t post;
    if (insn.readByte(memory, post)) return setError(NO_MEMORY);
    const RegName src = _regs.decodeTfmBaseReg(post >> 4);
    const RegName dst = _regs.decodeTfmBaseReg(post & 0xf);
    const uint8_t mode = insn.insnCode() & 0x3;
    if (src == REG_UNDEF || dst == REG_UNDEF) return setError(ILLEGAL_REGISTER);
    operands = _regs.outRegName(operands, src);
    const char srcModeChar = _regs.tfmSrcModeChar(mode);
    if (srcModeChar) *operands++ = srcModeChar;
    *operands++ = ',';
    operands = _regs.outRegName(operands, dst);
    const char dstModeChar = _regs.tfmDstModeChar(mode);
    if (dstModeChar) *operands++ = dstModeChar;
    *operands = 0;
    return setError(OK);
}

template<McuType mcuType>
Error Disassembler<mcuType>::decode(
    Memory &memory, Insn &insn, char *operands, SymbolTable *symtab) {
    reset(symtab);
    insn.resetAddress(memory.address());
    *operands = 0;

    target::opcode_t opCode;
    if (insn.readByte(memory, opCode)) return setError(NO_MEMORY);
    target::insn_t insnCode = opCode;
    if (InsnTableUtils::isPrefixCode(opCode)) {
        const target::opcode_t prefix = opCode;
        if (insn.readByte(memory, opCode)) return setError(NO_MEMORY);
        insnCode = InsnTableUtils::insnCode(prefix, opCode);
    }
    insn.setInsnCode(insnCode);

    if (InsnTable<mcuType>::table()->searchInsnCode(insn))
        return setError(UNKNOWN_INSTRUCTION);

    if (insn.mcuType() == HD6309 && mcuType == MC6809)
        return setError(UNKNOWN_INSTRUCTION);

    switch (insn.addrMode()) {
    case INHR:    return setError(OK);
    case DIRP:   return decodeDirectPage(memory, insn, operands);
    case EXTD:    return decodeExtended(memory, insn, operands);
    case INDX:     return decodeIndexed(memory, insn, operands);
    case REL8:
    case REL16:  return decodeRelative(memory, insn, operands);
    case STKOP:    return decodeStackOp(memory, insn, operands);
    case REGS:   return decodeRegisters(memory, insn, operands);
    case IMM8:
    case IMM16: return decodeImmediate(memory, insn, operands);
    default:
        if (mcuType == HD6309) {
            switch (insn.addrMode()) {
            case IMM32:   return decodeImmediate(memory, insn, operands);
            case IMMDIR:
            case IMMEXT:
            case IMMIDX:   return decodeImmediatePlus(memory, insn, operands);
            case BITOP: return decodeBitOperation(memory, insn, operands);
            case TFRM:  return decodeTransferMemory(memory, insn, operands);
            default:            break;
            }
        }
        return setError(INTERNAL_ERROR);
    }
}

#endif // __DIS_HD6309_IMPL_H__
