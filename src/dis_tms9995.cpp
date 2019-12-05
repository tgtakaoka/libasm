#include "dis_tms9995.h"

#include "string_utils.h"
#include "table_tms9995.h"

static char *outOpr8Hex(char *out, const target::byte_t val) {
    *out++ = '>';
    return outHex8(out, val);
}

static char *outOpr16Hex(char *out, const target::uint16_t val) {
    *out++ = '>';
    return outHex16(out, val);
}

static char *outRegister(char *out, const host::uint_t regno) {
    *out++ = 'R';
    return outUint16(out, regno & 0xf);
}

Error Disassembler::decodeOperand(
    Memory &memory, Insn &insn, char *&operands, const host::uint_t opr) {
    const host::uint_t regno = opr & 0xf;
    const host::uint_t mode = (opr >> 4) & 0x3;
    if (mode == 1 || mode == 3) *operands++ = '*';
    if (mode == 2) {
        target::uint16_t val;
        if (insn.readUint16(memory, val)) return setError(NO_MEMORY);
        *operands++ = '@';
        const char *label = lookup(val);
        if (label) {
            operands = outStr(operands, label);
        } else {
            operands = outOpr16Hex(operands, val);
        }
        if (regno) {
            *operands++ = '(';
            operands = outRegister(operands, regno);
            *operands++ = ')';
            *operands = 0;
        }
    } else {
        operands = outRegister(operands, regno);
        if (mode == 3) {
            *operands++ = '+';
            *operands = 0;
        }
    }
    return setError(OK);
}

Error Disassembler::decodeImmediate(
    Memory& memory, Insn &insn, char *operands) {
    target::uint16_t val;
    if (insn.readUint16(memory, val)) return setError(NO_MEMORY);
    const char *label = lookup(val);
    if (label) {
        outStr(operands, label);
    } else {
        outOpr16Hex(operands, val);
    }
    return setError(OK);
}

Error Disassembler::decodeRelative(
    Insn& insn, char *operands) {
    target::int16_t delta = (target::int8_t)(insn.insnCode() & 0xff);
    delta <<= 1;
    const target::uintptr_t addr = insn.address() + 2 + delta;
    const char *label = lookup(addr);
    if (label) {
        operands = outStr(operands, label);
        *operands = 0;
    } else {
        operands = outOpr16Hex(operands, addr);
    }
    return setError(OK);
}

Error Disassembler::decode(
    Memory &memory, Insn &insn, char *operands, SymbolTable *symtab) {
    reset(symtab);
    insn.resetAddress(memory.address());
    *operands = 0;

    target::insn_t insnCode;
    if (insn.readUint16(memory, insnCode)) return setError(NO_MEMORY);
    insn.setInsnCode(insnCode);
    InsnTable.searchInsnCode(insn);

    switch (insn.addrMode()) {
    case INH:
        return setError(OK);
    case IMM:
        return decodeImmediate(memory, insn, operands);
    case REG:
        outRegister(operands, insnCode);
        return setError(OK);
    case REG_IMM:
        operands = outRegister(operands, insnCode);
        *operands++ = ',';
        return decodeImmediate(memory, insn, operands);
    case CNT_REG: {
        operands = outRegister(operands, insnCode);
        *operands++ = ',';
        const host::uint_t count = (insnCode >> 4) & 0x0f;
        if (count == 0) outRegister(operands, 0);
        else outUint16(operands, count);
        return setError(OK);
    }
    case SRC:
        return decodeOperand(memory, insn, operands, insnCode);
    case REG_SRC:
        if (decodeOperand(memory, insn, operands, insnCode))
            return getError();
        *operands++ = ',';
        outRegister(operands, insnCode >> 6);
        return setError(OK);
    case CNT_SRC:
    case XOP_SRC: {
        if (decodeOperand(memory, insn, operands, insnCode))
            return getError();
        *operands++ = ',';
        host::uint_t count = (insnCode >> 6) & 0xf;
        if (insn.addrMode() == CNT_SRC && count == 0)
            count = 16;
        const char *label = lookup(count);
        if (label) {
            operands = outStr(operands, label);
        } else {
            outInt16(operands, count);
        }
        return setError(OK);
    }
    case DST_SRC: {
        if (decodeOperand(memory, insn, operands, insnCode))
            return getError();
        *operands++ = ',';
        decodeOperand(memory, insn, operands, insnCode >> 6);
        return getError();
    }
    case REL:
        decodeRelative(insn, operands);
        return setError(OK);
    case CRU_OFF: {
        const target::int8_t offset = (target::int8_t)(insnCode & 0xff);
        const char *label = lookup(offset);
        if (label) {
            outStr(operands, label);
        } else {
            outOpr8Hex(operands, offset);
        }
        return setError(OK);
    }
    default:
        return setError(INTERNAL_ERROR);
    }
}
