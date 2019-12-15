/* -*- mode: c++; -*- */
#ifndef __DIS_HD6309_IMPL_H__
#define __DIS_HD6309_IMPL_H__

#include "dis_operand.h"
#include "table_hd6309.h"

template<McuType mcuType>
template<typename T>
void Dis09<mcuType>::outConstant(T val, const uint8_t radix) {
    DisMotoOperand<T> encoder;
    _operands = encoder.outputConstant(_operands, val, radix);
}

template<McuType mcuType>
void Dis09<mcuType>::outRegister(RegName regName) {
    _operands = _regs.outRegName(_operands, regName);
}

template<McuType mcuType>
Error Dis09<mcuType>::decodeDirectPage(
    DisMemory<target::uintptr_t> &memory, Insn& insn) {
    uint8_t dir;
    if (insn.readByte(memory, dir)) return setError(NO_MEMORY);
    const char *label = lookup(dir);
    if (label) {
        *_operands ++ = '<';
        outText(label);
    } else {
        outConstant(dir);
    }
    return setError(OK);
}

template<McuType mcuType>
Error Dis09<mcuType>::decodeExtended(
    DisMemory<target::uintptr_t>& memory, Insn &insn) {
    target::uintptr_t addr;
    if (insn.readUint16(memory, addr)) return setError(NO_MEMORY);
    const char *label = lookup(addr);
    if (label) {
        if (addr < 0x100) *_operands++ = '>';
        outText(label);
    } else {
        outConstant(addr);
    }
    return setError(OK);
}

template<McuType mcuType>
Error Dis09<mcuType>::decodeIndexed(
    DisMemory<target::uintptr_t> &memory, Insn &insn) {
    uint8_t post;
    if (insn.readByte(memory, post)) return setError(NO_MEMORY);
    const uint8_t mode = post & 0x8F;

    uint8_t indir = (post & 0x10);
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
        offset = static_cast<int16_t>(addr);
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
        uint8_t val;
        if (insn.readByte(memory, val)) return setError(NO_MEMORY);
        addr = val;
        offset = static_cast<int8_t>(val);
    } else if (mode == 0x89) {
        // n16,R [n16,R]
        offSize = 16;
        if (insn.readUint16(memory, addr)) return setError(NO_MEMORY);
        offset = static_cast<int16_t>(addr);
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
            uint8_t val;
            if (insn.readByte(memory, val)) return setError(NO_MEMORY);
            offset = static_cast<int8_t>(val);
        } else {
            uint16_t val;
            if (insn.readUint16(memory, val)) return setError(NO_MEMORY);
            offset = static_cast<int16_t>(val);
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

    if (indir) *_operands++ = '[';
    if (label) {
        outText(label);
    } else {
        if (base) {
            if (index) {
                outRegister(index);
            } else if (offSize < 0) {
                outConstant(addr);
            } else {
                if (offSize != 0) {
                    outConstant(offset, 10);
                }
            }
        } else {
            outConstant(addr);
        }
    }
    if (base) {
        *_operands++ = ',';
        for (; incr < 0; incr++) *_operands++ = '-';
        outRegister(base);
        for (; incr > 0; incr--) *_operands++ = '+';
    }
    if (indir) *_operands++ = ']';
    *_operands = 0;
    return setError(OK);
}

template<McuType mcuType>
Error Dis09<mcuType>::decodeRelative(
    DisMemory<target::uintptr_t> &memory, Insn &insn) {
    target::ptrdiff_t delta;
    if (insn.addrMode() == REL8) {
        uint8_t val;
        if (insn.readByte(memory, val)) return setError(NO_MEMORY);
        delta = static_cast<int8_t>(val);
    } else {
        uint16_t val;
        if (insn.readUint16(memory, val)) return setError(NO_MEMORY);
        delta = static_cast<target::ptrdiff_t>(val);
    }
    const target::uintptr_t addr = insn.address() + insn.insnLen() + delta;
    const char *label = lookup(addr);
    if (label) {
        outText(label);
    } else {
        outConstant(addr);
    }
    return setError(OK);
}

template<McuType mcuType>
Error Dis09<mcuType>::decodeImmediate(
    DisMemory<target::uintptr_t>& memory, Insn &insn) {
    *_operands++ = '#';
    if (insn.addrMode() == IMM8) {
        uint8_t val;
        if (insn.readByte(memory, val)) return setError(NO_MEMORY);
        outConstant(val);
    } else if (insn.addrMode() == IMM16) {
        uint16_t val;
        if (insn.readUint16(memory, val)) return setError(NO_MEMORY);
        const char *label = lookup(val);
        if (label) {
            outText(label);
        } else {
            outConstant(val);
        }
    } else if (mcuType == HD6309 && insn.addrMode() == IMM32) {
        uint32_t val;
        if (insn.readUint32(memory, val)) return setError(NO_MEMORY);
        outConstant(val);
    } else {
        return setError(UNKNOWN_INSTRUCTION);
    }
    return setError(OK);
}

template<McuType mcuType>
Error Dis09<mcuType>::decodeStackOp(
    DisMemory<target::uintptr_t> &memory, Insn &insn) {
    uint8_t post;
    if (insn.readByte(memory, post)) return setError(NO_MEMORY);
    if (post == 0) {
        *_operands++ = '#';
        outConstant(post);
        return setError(OK);
    }
    const bool push = (insn.insnCode() & 1) == 0;
    for (host::uint_t i = 0, n = 0; i < 8; i++) {
        const host::uint_t bit = push ? 7 - i : i;
        if (post & (1 << bit)) {
            if (n != 0) *_operands++ = ',';
            outRegister(_regs.getStackReg(bit, insn.insnCode()));
            n++;
        }
    }
    return setError(OK);
}

template<McuType mcuType>
Error Dis09<mcuType>::decodeRegisters(
    DisMemory<target::uintptr_t> &memory, Insn &insn) {
    uint8_t post;
    if (insn.readByte(memory, post)) return setError(NO_MEMORY);
    const RegName src = _regs.decodeRegName(post >> 4);
    const RegName dst = _regs.decodeRegName(post & 0xf);
    if (src == REG_UNDEF || dst == REG_UNDEF) return setError(ILLEGAL_REGISTER);
    outRegister(src);
    *_operands++ = ',';
    outRegister(dst);
    return setError(OK);
}

template<McuType mcuType>
Error Dis09<mcuType>::decodeImmediatePlus(
    DisMemory<target::uintptr_t>& memory, Insn &insn) {
    *_operands++ = '#';
    uint8_t val;
    if (insn.readByte(memory, val)) return setError(NO_MEMORY);
    outConstant(val);
    *_operands++ = ',';
    switch (insn.addrMode()) {
    case IMMDIR: return decodeDirectPage(memory, insn);
    case IMMEXT: return decodeExtended(memory, insn);
    case IMMIDX: return decodeIndexed(memory, insn);
    default:     return setError(INTERNAL_ERROR);
    }
}

template<McuType mcuType>
Error Dis09<mcuType>::decodeBitOperation(
    DisMemory<target::uintptr_t> &memory, Insn &insn) {
    uint8_t post;
    if (insn.readByte(memory, post)) return setError(NO_MEMORY);
    const RegName reg = _regs.decodeBitOpReg(post >> 6);
    if (reg == REG_UNDEF) return setError(ILLEGAL_REGISTER);
    outRegister(reg);
    *_operands++ = ',';
    outConstant((post >> 3) & 7);
    *_operands++ = ',';
    outConstant(post & 7);
    *_operands++ = ',';
    return decodeDirectPage(memory, insn);
}

template<McuType mcuType>
Error Dis09<mcuType>::decodeTransferMemory(
    DisMemory<target::uintptr_t> &memory, Insn &insn) {
    uint8_t post;
    if (insn.readByte(memory, post)) return setError(NO_MEMORY);
    const RegName src = _regs.decodeTfmBaseReg(post >> 4);
    const RegName dst = _regs.decodeTfmBaseReg(post & 0xf);
    const uint8_t mode = insn.insnCode() & 0x3;
    if (src == REG_UNDEF || dst == REG_UNDEF) return setError(ILLEGAL_REGISTER);
    outRegister(src);
    const char srcModeChar = _regs.tfmSrcModeChar(mode);
    if (srcModeChar) *_operands++ = srcModeChar;
    *_operands++ = ',';
    outRegister(dst);
    const char dstModeChar = _regs.tfmDstModeChar(mode);
    if (dstModeChar) *_operands++ = dstModeChar;
    *_operands = 0;
    return setError(OK);
}

template<McuType mcuType>
Error Dis09<mcuType>::decode(
    DisMemory<target::uintptr_t> &memory,
    Insn &insn,
    char *operands,
    SymbolTable<target::uintptr_t> *symtab) {
    reset(operands, symtab);
    insn.resetAddress(memory.address());

    target::opcode_t opCode;
    if (insn.readByte(memory, opCode)) return setError(NO_MEMORY);
    target::insn_t insnCode = opCode;
    if (TableHd6309Base::isPrefixCode(opCode)) {
        const target::opcode_t prefix = opCode;
        if (insn.readByte(memory, opCode)) return setError(NO_MEMORY);
        insnCode = TableHd6309Base::insnCode(prefix, opCode);
    }
    insn.setInsnCode(insnCode);

    if (TableHd6309<mcuType>::table()->searchInsnCode(insn))
        return setError(UNKNOWN_INSTRUCTION);

    if (insn.mcuType() == HD6309 && mcuType == MC6809)
        return setError(UNKNOWN_INSTRUCTION);

    switch (insn.addrMode()) {
    case INHR:  return setError(OK);
    case DIRP:  return decodeDirectPage(memory, insn);
    case EXTD:  return decodeExtended(memory, insn);
    case INDX:  return decodeIndexed(memory, insn);
    case REL8:
    case REL16: return decodeRelative(memory, insn);
    case STKOP: return decodeStackOp(memory, insn);
    case REGS:  return decodeRegisters(memory, insn);
    case IMM8:
    case IMM16: return decodeImmediate(memory, insn);
    default:
        if (mcuType == HD6309) {
            switch (insn.addrMode()) {
            case IMM32: return decodeImmediate(memory, insn);
            case IMMDIR:
            case IMMEXT:
            case IMMIDX:return decodeImmediatePlus(memory, insn);
            case BITOP: return decodeBitOperation(memory, insn);
            case TFRM:  return decodeTransferMemory(memory, insn);
            default:    break;
            }
        }
        return setError(INTERNAL_ERROR);
    }
}

#endif // __DIS_HD6309_IMPL_H__
