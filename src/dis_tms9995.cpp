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

Error Disassembler::readUint16(Memory &memory, Insn &insn, target::uint16_t &val) {
    if (!memory.hasNext()) return setError(NO_MEMORY);
    val = (target::uint16_t)memory.readByte() << 8;
    if (!memory.hasNext()) return setError(NO_MEMORY);
    val |= memory.readByte();
    insn.readUint16(val);
    return OK;
}

Error Disassembler::decodeOperand(
    Memory &memory, Insn &insn, char *&operands, char *&comments,
    const host::uint_t opr) {
    const host::uint_t regno = opr & 0xf;
    const host::uint_t mode = (opr >> 4) & 0x3;
    if (mode == 1 || mode == 3) *operands++ = '*';
    if (mode == 2) {
        target::uint16_t val;
        if (readUint16(memory, insn, val)) return getError();
        *operands++ = '@';
        const char *label = lookup(val);
        if (label) {
            operands = outStr(operands, label);
            comments = outOpr16Hex(comments, val);
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
    Memory& memory, Insn &insn, char *operands, char *comments) {
    target::uint16_t val;
    if (readUint16(memory, insn, val)) return getError();
    const char *label = lookup(val);
    if (label) {
        outStr(operands, label);
        outOpr16Hex(comments, val);
    } else {
        outOpr16Hex(operands, val);
        if (insn.addrMode() == REG_IMM)
            outInt16(comments, val);
    }
    return setError(OK);
}

Error Disassembler::decodeRelative(
    Insn& insn, char *operands, char *comments) {
    target::int16_t delta = (target::int8_t)(insn.insnCode() & 0xff);
    delta <<= 1;
    const target::uintptr_t addr = insn.address() + 2 + delta;
    const char *label = lookup(addr);
    if (label) {
        operands = outStr(operands, label);
        *operands = 0;
        comments = outOpr16Hex(comments, addr);
    } else {
        operands = outOpr16Hex(operands, addr);
        *comments++ = '$';
        if (delta >= -2) *comments++ = '+';
        comments = outInt16(comments, delta + 2);
    }
    return setError(OK);
}

Error Disassembler::decode(
    Memory &memory, Insn &insn, char *operands, char *comments, SymbolTable *symtab) {
    reset(symtab);
    insn.resetAddress(memory.address());
    *operands = *comments = 0;

    target::insn_t insnCode;
    if (readUint16(memory, insn, insnCode)) return getError();
    insn.setInsnCode(insnCode);
    InsnTable.searchInsnCode(insn);

    switch (insn.addrMode()) {
    case INH:
        return setError(OK);
    case IMM:
        return decodeImmediate(memory, insn, operands, comments);
    case REG:
        outRegister(operands, insnCode);
        return setError(OK);
    case REG_IMM:
        operands = outRegister(operands, insnCode);
        *operands++ = ',';
        return decodeImmediate(memory, insn, operands, comments);
    case CNT_REG: {
        operands = outRegister(operands, insnCode);
        *operands++ = ',';
        const host::uint_t count = (insnCode >> 4) & 0x0f;
        if (count == 0) outRegister(operands, 0);
        else outUint16(operands, count);
        return setError(OK);
    }
    case SRC:
        return decodeOperand(memory, insn, operands, comments, insnCode);
    case REG_SRC:
        if (decodeOperand(memory, insn, operands, comments, insnCode))
            return getError();
        *operands++ = ',';
        outRegister(operands, insnCode >> 6);
        return setError(OK);
    case CNT_SRC:
    case XOP_SRC: {
        const char *c = comments;
        if (decodeOperand(memory, insn, operands, comments, insnCode))
            return getError();
        *operands++ = ',';
        host::uint_t count = (insnCode >> 6) & 0xf;
        if (insn.addrMode() == CNT_SRC && count == 0)
            count = 16;
        const char *label = lookup(count);
        if (label) {
            operands = outStr(operands, label);
            if (comments != c) *comments++ = ',';
            outInt16(comments, count);
        } else {
            outInt16(operands, count);
        }
        return setError(OK);
    }
    case DST_SRC: {
        char *c = comments;
        if (decodeOperand(memory, insn, operands, comments, insnCode))
            return getError();
        *operands++ = ',';
        if (c != comments) *comments++ = ' ';
        c = comments;
        decodeOperand(memory, insn, operands, comments, insnCode >> 6);
        if (c == comments) *--c = 0;
        return getError();
    }
    case REL:
        decodeRelative(insn, operands, comments);
        return setError(OK);
    case CRU_OFF: {
        const target::int8_t offset = (target::int8_t)(insnCode & 0xff);
        const char *label = lookup(offset);
        if (label) {
            outStr(operands, label);
            comments = outOpr8Hex(comments, offset);
            *comments++ = ';';
        } else {
            outOpr8Hex(operands, offset);
        }
        comments = outStr(comments, "R12");
        if (offset >= 0) *comments++ = '+';
        outInt16(comments, offset);
        return setError(OK);
    }
    default:
        return setError(INTERNAL_ERROR);
    }
}
