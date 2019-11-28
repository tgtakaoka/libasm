/* -*- mode: c++; -*- */
#ifndef __DIS_R65C02_IMPL_H__
#define __DIS_R65C02_IMPL_H__

#include "string_utils.h"
#include "table_r65c02.h"

static char *outOpr8Hex(char *out, target::byte_t val) {
    *out++ = '$';
    return outHex8(out, val);
}

static char *outOpr16Hex(char *out, target::uint16_t val) {
    *out++ = '$';
    return outHex16(out, val);
}

template<McuType mcuType>
Error Disassembler<mcuType>::readByte(Memory &memory, Insn &insn, target::byte_t &val) {
    if (!memory.hasNext()) return setError(NO_MEMORY);
    val = memory.readByte();
    insn.emitByte(val);
    return OK;
}

template<McuType mcuType>
Error Disassembler<mcuType>::readUint16(Memory &memory, Insn &insn, target::uint16_t &val) {
    if (!memory.hasNext()) return setError(NO_MEMORY);
    val = memory.readByte();
    if (!memory.hasNext()) return setError(NO_MEMORY);
    val |= (target::uint16_t)memory.readByte() << 8;
    insn.emitUint16(val);
    return OK;
}

template<McuType mcuType>
Error Disassembler<mcuType>::decodeImmediate(
    Memory& memory, Insn &insn, char *operands) {
    target::byte_t val;
    if (readByte(memory, insn, val)) return getError();
    *operands++ = '#';
    const char *label = lookup(val);
    if (label) {
        outStr(operands, label);
    } else {
        outOpr8Hex(operands, val);
    }
    return setError(OK);
}

template<McuType mcuType>
Error Disassembler<mcuType>::decodeAbsolute(
    Memory& memory, Insn &insn, char *operands) {
    const bool indirect = (insn.addrMode() == IDX_ABS_IND
                           || insn.addrMode() == ABS_INDIRECT);
    char index;
    switch (insn.addrMode()) {
    case ABS_IDX_X:
    case IDX_ABS_IND:
        index = 'X';
        break;
    case ABS_IDX_Y:
        index = 'Y';
        break;
    default:
        index = 0;
        break;
    }
    target::uintptr_t addr;
    if (readUint16(memory, insn, addr)) return getError();
    if (indirect) *operands++ = '(';
    const char *label = lookup(addr);
    if (label) {
        if (addr < 0x100) *operands++ = '>';
        operands = outStr(operands, label);
    } else {
        operands = outOpr16Hex(operands, addr);
    }
    if (index) {
        *operands++ = ',';
        *operands++ = index;
        *operands = 0;
    }
    if (indirect) {
        *operands++ = ')';
        *operands = 0;
    }
    return setError(OK);
}

template<McuType mcuType>
Error Disassembler<mcuType>::decodeZeroPage(
    Memory &memory, Insn& insn, char *operands) {
    const bool indirect = insn.addrMode() == INDX_IND
        || insn.addrMode() == INDIRECT_IDX
        || insn.addrMode() == ZP_INDIRECT;
    char index;
    switch (insn.addrMode()) {
    case ZP_IDX_X:
    case INDX_IND:
        index = 'X';
        break;
    case ZP_IDX_Y:
    case INDIRECT_IDX:
        index = 'Y';
        break;
    default:
        index = 0;
        break;
    }
    target::byte_t zp;
    if (readByte(memory, insn, zp)) return getError();
    if (indirect) *operands++ = '(';
    const char *label = lookup(zp);
    if (label) {
        operands = outStr(operands, label);
    } else {
        operands = outOpr8Hex(operands, zp);
    }
    if (indirect && index == 'Y') *operands++ = ')';
    if (index) {
        *operands++ = ',';
        *operands++ = index;
        *operands = 0;
    }
    if (indirect && index != 'Y') {
        *operands++ = ')';
        *operands = 0;
    }
    if (insn.addrMode() == ZP_REL8) {
        *operands++ = ',';
        return decodeRelative(memory, insn, operands);
    }
    return setError(OK);
}

template<McuType mcuType>
Error Disassembler<mcuType>::decodeRelative(
    Memory &memory, Insn &insn, char *operands) {
    target::ptrdiff_t delta;
    target::byte_t val;
    if (readByte(memory, insn, val)) return getError();
    delta = static_cast<target::int8_t>(val);
    const host::uint_t insnLen = (insn.addrMode() == ZP_REL8 ? 3 : 2);
    const target::uintptr_t addr = insn.address() + insnLen + delta;
    const char *label = lookup(addr);
    if (label) {
        outStr(operands, label);
    } else {
        outOpr16Hex(operands, addr);
    }
    return setError(OK);
}

template<McuType mcuType>
Error Disassembler<mcuType>::decode(
    Memory &memory, Insn &insn, char *operands, SymbolTable *symtab) {
    reset(symtab);
    insn.resetAddress(memory.address());
    *operands = 0;

    target::insn_t insnCode;
    if (readByte(memory, insn, insnCode)) return getError();
    insn.setInsnCode(insnCode);

    if (InsnTable<mcuType>::table()->searchInsnCode(insn))
        return setError(UNKNOWN_INSTRUCTION);

    if (insn.mcuType() == R65C02 && mcuType != R65C02)
        return setError(UNKNOWN_INSTRUCTION);

    switch (insn.addrMode()) {
    case IMPLIED:
        return setError(OK);
    case ACCUMULATOR:
        *operands++ = 'A';
        *operands = 0;
        return setError(OK);
    case IMMEDIATE:
        return decodeImmediate(memory, insn, operands);
    case ABSOLUTE:
    case ABS_IDX_X:
    case ABS_IDX_Y:
    case ABS_INDIRECT:
    case IDX_ABS_IND:
        return decodeAbsolute(memory, insn, operands);
    case ZEROPAGE:
    case ZP_IDX_X:
    case ZP_IDX_Y:
    case INDX_IND:
    case INDIRECT_IDX:
    case ZP_INDIRECT:
    case ZP_REL8:
        return decodeZeroPage(memory, insn, operands);
    case REL8:
        return decodeRelative(memory, insn, operands);
    default:
        return setError(INTERNAL_ERROR);
    }
}

#endif // __DIS_R65C02_IMPL_H__
