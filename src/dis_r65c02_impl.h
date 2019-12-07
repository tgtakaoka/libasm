/* -*- mode: c++; -*- */
#ifndef __DIS_R65C02_IMPL_H__
#define __DIS_R65C02_IMPL_H__

#include "string_utils.h"
#include "table_r65c02.h"

template<McuType mcuType>
void Dis6502<mcuType>::outText(const char *text) {
    _operands = outStr(_operands, text);
}

template<McuType mcuType>
void Dis6502<mcuType>::outOpr8Hex(uint8_t val) {
    *_operands++ = '$';
    _operands = outHex8(_operands, val);
}

template<McuType mcuType>
void Dis6502<mcuType>::outOpr16Hex(uint16_t val) {
    *_operands++ = '$';
    _operands = outHex16(_operands, val);
}

template<McuType mcuType>
Error Dis6502<mcuType>::decodeImmediate(DisMemory& memory, Insn &insn) {
    uint8_t val;
    if (insn.readByte(memory, val)) return setError(NO_MEMORY);
    outChar('#');
    const char *label = lookup(val);
    if (label) {
        outText(label);
    } else {
        outOpr8Hex(val);
    }
    return setError(OK);
}

template<McuType mcuType>
Error Dis6502<mcuType>::decodeAbsolute(DisMemory& memory, Insn &insn) {
    const bool indirect = (insn.addrMode() == IDX_ABS_IND)
        || (insn.addrMode() == ABS_INDIRECT);
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
    if (insn.readUint16(memory, addr)) return setError(NO_MEMORY);
    if (indirect) outChar('(');
    const char *label = lookup(addr);
    if (label) {
        if (addr < 0x100) outChar('>');
        outText(label);
    } else {
        outOpr16Hex(addr);
    }
    if (index) {
        outChar(',');
        outChar(index);
    }
    if (indirect) outChar(')');
    return setError(OK);
}

template<McuType mcuType>
Error Dis6502<mcuType>::decodeZeroPage(DisMemory &memory, Insn& insn) {
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
    uint8_t zp;
    if (insn.readByte(memory, zp)) return setError(NO_MEMORY);
    if (indirect) outChar('(');
    const char *label = lookup(zp);
    if (label) {
        outText(label);
    } else {
        outOpr8Hex(zp);
    }
    if (indirect && index == 'Y') outChar(')');
    if (index) {
        outChar(',');
        outChar(index);
    }
    if (indirect && index != 'Y') outChar(')');
#ifdef R65C02_ENABLE_BITOPS
    if (insn.addrMode() == ZP_REL8) {
        outChar(',');
        return decodeRelative(memory, insn);
    }
#endif
    return setError(OK);
}

template<McuType mcuType>
Error Dis6502<mcuType>::decodeRelative(DisMemory &memory, Insn &insn) {
    target::ptrdiff_t delta;
    uint8_t val;
    if (insn.readByte(memory, val)) return setError(NO_MEMORY);
    delta = static_cast<int8_t>(val);
#ifdef R65C02_ENABLE_BITOPS
    const host::uint_t insnLen = (insn.addrMode() == ZP_REL8 ? 3 : 2);
#else
    const host::uint_t insnLen = 2;
#endif
    const target::uintptr_t addr = insn.address() + insnLen + delta;
    const char *label = lookup(addr);
    if (label) {
        outText(label);
    } else {
        outOpr16Hex(addr);
    }
    return setError(OK);
}

template<McuType mcuType>
Error Dis6502<mcuType>::decode(
    DisMemory &memory, Insn &insn, char *operands, SymbolTable *symtab) {
    reset(operands, symtab);
    insn.resetAddress(memory.address());

    target::insn_t insnCode;
    if (insn.readByte(memory, insnCode)) return setError(NO_MEMORY);
    insn.setInsnCode(insnCode);

    if (TableR65c02<mcuType>::table()->searchInsnCode(insn))
        return setError(UNKNOWN_INSTRUCTION);

    if (insn.mcuType() == R65C02 && mcuType != R65C02)
        return setError(UNKNOWN_INSTRUCTION);

    switch (insn.addrMode()) {
    case IMPLIED:
        return setError(OK);
    case ACCUMULATOR:
        outChar('A');
        return setError(OK);
    case IMMEDIATE:
        return decodeImmediate(memory, insn);
    case ABSOLUTE:
    case ABS_IDX_X:
    case ABS_IDX_Y:
    case ABS_INDIRECT:
    case IDX_ABS_IND:
        return decodeAbsolute(memory, insn);
    case ZEROPAGE:
    case ZP_IDX_X:
    case ZP_IDX_Y:
    case INDX_IND:
    case INDIRECT_IDX:
    case ZP_INDIRECT:
#ifdef R65C02_ENABLE_BITOPS
    case ZP_REL8:
#endif
        return decodeZeroPage(memory, insn);
    case REL8:
        return decodeRelative(memory, insn);
    default:
        return setError(INTERNAL_ERROR);
    }
}

#endif // __DIS_R65C02_IMPL_H__
