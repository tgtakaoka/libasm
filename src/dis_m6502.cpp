#include "dis_m6502.h"
#include "dis_operand.h"
#include "table_m6502.h"

Error DisM6502::decodeImmediate(
    DisMemory<target::uintptr_t>& memory, Insn &insn) {
    uint8_t val;
    if (insn.readByte(memory, val)) return setError(NO_MEMORY);
    *_operands++ = '#';
    const char *label = lookup(val);
    if (label) {
        outText(label);
    } else {
        outConstant(val);
    }
    return setError(OK);
}

Error DisM6502::decodeAbsolute(
    DisMemory<target::uintptr_t>& memory, Insn &insn) {
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
    if (indirect) *_operands++ = '(';
    const char *label = lookup(addr);
    if (label) {
        if (addr < 0x100) *_operands++ = '>';
        outText(label);
    } else {
        outConstant(addr, 16, false);
    }
    if (index) {
        *_operands++ = ',';
        *_operands++ = index;
    }
    if (indirect) *_operands++ = ')';
    *_operands = 0;
    return setError(OK);
}

Error DisM6502::decodeZeroPage(
    DisMemory<target::uintptr_t> &memory, Insn& insn) {
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
    if (indirect) *_operands++ = '(';
    const char *label = lookup(zp);
    if (label) {
        outText(label);
    } else {
        outConstant(zp, 16, false);
    }
    if (indirect && index == 'Y') *_operands++ = ')';
    if (index) {
        *_operands++ = ',';
        *_operands++ = index;
    }
    if (indirect && index != 'Y') *_operands++ = ')';
    *_operands = 0;
#ifdef W65C02_ENABLE_BITOPS
    if (insn.addrMode() == ZP_REL8) {
        *_operands++ = ',';
        return decodeRelative(memory, insn);
    }
#endif
    return setError(OK);
}

Error DisM6502::decodeRelative(
    DisMemory<target::uintptr_t> &memory, Insn &insn) {
    target::ptrdiff_t delta;
    uint8_t val;
    if (insn.readByte(memory, val)) return setError(NO_MEMORY);
    delta = static_cast<int8_t>(val);
#ifdef W65C02_ENABLE_BITOPS
    const host::uint_t insnLen = (insn.addrMode() == ZP_REL8 ? 3 : 2);
#else
    const host::uint_t insnLen = 2;
#endif
    const target::uintptr_t addr = insn.address() + insnLen + delta;
    const char *label = lookup(addr);
    if (label) {
        outText(label);
    } else {
        outConstant(addr, 16, false);
    }
    return setError(OK);
}

Error DisM6502::decode(
    DisMemory<target::uintptr_t> &memory, Insn &insn) {
    target::insn_t insnCode;
    if (insn.readByte(memory, insnCode)) return setError(NO_MEMORY);
    insn.setInsnCode(insnCode);

    if (TableM6502.searchInsnCode(insn))
        return setError(UNKNOWN_INSTRUCTION);

    if (insn.mcuType() == W65C02 && _mcuType != W65C02)
        return setError(UNKNOWN_INSTRUCTION);

    switch (insn.addrMode()) {
    case IMPLIED:
        return setError(OK);
    case ACCUMULATOR:
        *_operands++ = 'A';
        *_operands = 0;
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
#ifdef W65C02_ENABLE_BITOPS
    case ZP_REL8:
#endif
        return decodeZeroPage(memory, insn);
    case REL8:
        return decodeRelative(memory, insn);
    default:
        return setError(INTERNAL_ERROR);
    }
}
