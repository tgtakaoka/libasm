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
    Memory& memory, Insn &insn, char *operands, char *comments) {
    target::byte_t val;
    if (readByte(memory, insn, val)) return getError();
    *operands++ = '#';
    const char *label = lookup(val);
    if (label) {
        outStr(operands, label);
        outOpr8Hex(comments, val);
    } else {
        outOpr8Hex(operands, val);
        outInt16(comments, val);
    }
    return setError(OK);
}

template<McuType mcuType>
Error Disassembler<mcuType>::decodeAbsolute(
    Memory& memory, Insn &insn, char *operands, char *comments) {
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
        outOpr16Hex(comments, addr);
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
    Memory &memory, Insn& insn, char *operands, char *comments) {
    const bool indirect = (insn.addrMode() == INDEXED_IND
                           || insn.addrMode() == INDIRECT_IDX
                           || insn.addrMode() == ZP_INDIRECT);
    char index;
    switch (insn.addrMode()) {
    case ZP_IDX_X:
    case INDEXED_IND:
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
        outOpr8Hex(comments, zp);
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
    if (insn.addrMode() == ZP_RELATIVE) {
        *operands++ = ',';
        return decodeRelative(memory, insn, operands, comments);
    }
    return setError(OK);
}

template<McuType mcuType>
Error Disassembler<mcuType>::decodeRelative(
    Memory &memory, Insn &insn, char *operands, char *comments) {
    target::ptrdiff_t delta;
    target::byte_t val;
    if (readByte(memory, insn, val)) return getError();
    delta = static_cast<target::int8_t>(val);
    const host::uint_t insnLen = (insn.addrMode() == ZP_RELATIVE ? 3 : 2);
    const target::uintptr_t addr = insn.address() + insnLen + delta;
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

template<McuType mcuType>
Error Disassembler<mcuType>::decode(
    Memory &memory, Insn &insn, char *operands, char *comments, SymbolTable *symtab) {
    reset(symtab);
    insn.resetAddress(memory.address());
    *operands = *comments = 0;

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
        return decodeImmediate(memory, insn, operands, comments);
    case ABSOLUTE:
    case ABS_IDX_X:
    case ABS_IDX_Y:
    case ABS_INDIRECT:
    case IDX_ABS_IND:
        return decodeAbsolute(memory, insn, operands, comments);
    case ZEROPAGE:
    case ZP_IDX_X:
    case ZP_IDX_Y:
    case INDEXED_IND:
    case INDIRECT_IDX:
    case ZP_INDIRECT:
    case ZP_RELATIVE:
        return decodeZeroPage(memory, insn, operands, comments);
    case RELATIVE:
        return decodeRelative(memory, insn, operands, comments);
    default:
        return setError(INTERNAL_ERROR);
    }
}

#endif // __DIS_R65C02_IMPL_H__
