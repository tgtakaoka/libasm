#include "dis_tms9995.h"

#include "dis_operand.h"
#include "table_tms9995.h"

void DisTms9995::outAddress(target::uintptr_t addr, bool relax) {
    const char *label = lookup(addr);
    if (label) {
        outText(label);
    } else {
        outConstant(addr, 16, relax);
    }
}

void DisTms9995::outRegister(host::uint_t regno) {
    *_operands++ = 'R';
    outConstant(uint8_t(regno & 0xf), 10);
}

Error DisTms9995::decodeOperand(
    DisMemory<target::uintptr_t> &memory, Insn &insn, const host::uint_t opr) {
    const host::uint_t regno = opr & 0xf;
    const host::uint_t mode = (opr >> 4) & 0x3;
    if (mode == 1 || mode == 3) *_operands++ = '*';
    if (mode == 2) {
        uint16_t val;
        if (insn.readUint16(memory, val)) return setError(NO_MEMORY);
        *_operands++ = '@';
        outAddress(val);
        if (regno) {
            *_operands++ = '(';
            outRegister(regno);
            *_operands++ = ')';
        }
    } else {
        outRegister(regno);
        if (mode == 3)
            *_operands++ = '+';
    }
    *_operands = 0;
    return setError(OK);
}

Error DisTms9995::decodeImmediate(
    DisMemory<target::uintptr_t>& memory, Insn &insn) {
    uint16_t val;
    if (insn.readUint16(memory, val)) return setError(NO_MEMORY);
    outAddress(val);
    return setError(OK);
}

Error DisTms9995::decodeRelative(Insn& insn) {
    int16_t delta = static_cast<int8_t>(insn.insnCode() & 0xff);
    delta <<= 1;
    const target::uintptr_t addr = insn.address() + 2 + delta;
    outAddress(addr, false);
    return setError(OK);
}

Error DisTms9995::decode(
    DisMemory<target::uintptr_t> &memory, Insn &insn) {
    target::insn_t insnCode;
    if (insn.readUint16(memory, insnCode)) return setError(NO_MEMORY);
    insn.setInsnCode(insnCode);
    TableTms9995.searchInsnCode(insn);

    switch (insn.addrMode()) {
    case INH:
        return setError(OK);
    case IMM:
        return decodeImmediate(memory, insn);
    case REG:
        outRegister(insnCode);
        return setError(OK);
    case REG_IMM:
        outRegister(insnCode);
        *_operands++ = ',';
        return decodeImmediate(memory, insn);
    case CNT_REG: {
        outRegister(insnCode);
        *_operands++ = ',';
        const host::uint_t count = (insnCode >> 4) & 0x0f;
        if (count == 0) outRegister(0);
        else outConstant(uint8_t(count), 10);
        return setError(OK);
    }
    case SRC:
        return decodeOperand(memory, insn, insnCode);
    case REG_SRC:
        if (decodeOperand(memory, insn, insnCode))
            return getError();
        *_operands++ = ',';
        outRegister(insnCode >> 6);
        return setError(OK);
    case CNT_SRC:
    case XOP_SRC: {
        if (decodeOperand(memory, insn, insnCode))
            return getError();
        *_operands++ = ',';
        host::uint_t count = (insnCode >> 6) & 0xf;
        if (insn.addrMode() == CNT_SRC && count == 0)
            count = 16;
        const char *label = lookup(count);
        if (label) {
            outText(label);
        } else {
            outConstant(uint8_t(count), 10);
        }
        return setError(OK);
    }
    case DST_SRC: {
        if (decodeOperand(memory, insn, insnCode))
            return getError();
        *_operands++ = ',';
        decodeOperand(memory, insn, insnCode >> 6);
        return getError();
    }
    case REL:
        decodeRelative(insn);
        return setError(OK);
    case CRU_OFF: {
        const int8_t offset = static_cast<int8_t>(insnCode & 0xff);
        const char *label = lookup(offset);
        if (label) {
            outText(label);
        } else {
            outConstant(offset, 10);
        }
        return setError(OK);
    }
    default:
        return setError(INTERNAL_ERROR);
    }
}
