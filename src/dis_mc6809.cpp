#include "dis_mc6809.h"

#include "string_utils.h"

static const RegName TABLE_INDEX_REGS[] = {
    NONE, NONE, NONE, NONE, NONE, B, A, NONE, NONE, NONE, NONE, D
};
constexpr host::uindex_t LENGTH_INDEXES =
    sizeof(TABLE_INDEX_REGS) / sizeof(TABLE_INDEX_REGS[0]);
static const RegName TABLE_BASE_REGS[] = { X, Y, U, S };
constexpr host::uindex_t LENGTH_BASES =
    sizeof(TABLE_BASE_REGS) / sizeof(TABLE_BASE_REGS[0]);
static const RegName TABLE_REGISTERS[] = {
    D, X, Y, U, S, PC, NONE, NONE, A, B, CC, DP
};
constexpr host::uindex_t LENGTH_REGISTERS =
    sizeof(TABLE_REGISTERS) / sizeof(TABLE_REGISTERS[0]);

static const char TABLE_CCR_BITS[8] = { 'E', 'F', 'H', 'I', 'N', 'Z', 'V', 'C' };
static const RegName TABLE_STACK_OP_S[8] = { CC, A, B, DP, X, Y, U, PC };
static const RegName TABLE_STACK_OP_U[8] = { CC, A, B, DP, X, Y, S, PC };
static const uint8_t BIT_MASKS[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

static char regName1stChar(const RegName regName) {
    return (regName == DP) ? 'D' : char(regName);
}

static char regName2ndChar(const RegName regName) {
    if (regName == PC) return 'C';
    if (regName == CC) return 'C';
    if (regName == DP) return 'P';
    return 0;
}

char *DisMc6809::outOpr8Hex(char *out, target::byte_t val) const {
    *out++ = '$';
    return outHex8(out, val);
}

char *DisMc6809::outOpr16Hex(char *out, target::uint16_t val) const {
    *out++ = '$';
    return outHex16(out, val);
}

char *DisMc6809::outRegName(char *out, const RegName regName) const {
    *out++ = regName1stChar(regName);
    const char r2 = regName2ndChar(regName);
    if (r2) *out++ = r2;
    *out = 0;
    return out;
}

char *DisMc6809::outCCRBits(char *out, target::byte_t val) const {
    host::int_t n = 0;
    for (host::uint_t mask = 0x80, i = 0; i < 8; mask >>= 1, i++) {
        if (val & mask) {
            if (n == 1) { char b = *--out; *out++ = '('; *out++ = b; }
            if (n) *out++ = '|';
            *out++ = TABLE_CCR_BITS[i];
            n++;
        }
    }
    if (n >= 2) *out++ = ')';
    *out = 0;
    return out;
}

RegName DisMc6809::decodeIndexReg(target::byte_t regNum) const {
    if (regNum >= LENGTH_INDEXES) return NONE;
    return TABLE_INDEX_REGS[regNum];
}

RegName DisMc6809::decodeBaseReg(target::byte_t regNum) const {
    if (regNum >= LENGTH_BASES) return NONE;
    return TABLE_BASE_REGS[regNum];
}

RegName DisMc6809::decodeRegName(target::byte_t regNum) const {
    if (regNum >= LENGTH_REGISTERS) return NONE;
    return TABLE_REGISTERS[regNum];
}

Error DisMc6809::readByte(Memory &memory, Insn &insn, target::byte_t &val) {
    if (!memory.hasNext()) return setError(NO_MEMORY);
    val = memory.readByte();
    emitByte(insn, val);
    return OK;
}

Error DisMc6809::readUint16(Memory &memory, Insn &insn, target::uint16_t &val) {
    if (!memory.hasNext()) return setError(NO_MEMORY);
    val = (target::uint16_t)memory.readByte() << 8;
    if (!memory.hasNext()) return setError(NO_MEMORY);
    val |= memory.readByte();
    emitUint16(insn, val);
    return OK;
}

Error DisMc6809::decodeDirectPage(
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

Error DisMc6809::decodeExtended(
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

Error DisMc6809::decodeIndexed(
    Memory &memory, Insn &insn, char *operands, char *comments) {
    target::byte_t post;
    if (readByte(memory, insn, post)) return getError();
    const target::byte_t mode = post & 0x8F;

    target::byte_t indir = (post & 0x10);
    const char *label = nullptr;
    target::uintptr_t addr = 0;
    target::intptr_t offset = 0;
    host::int_t offSize = 0;
    RegName base = decodeBaseReg((post >> 5) & 3);
    RegName index = NONE;
    host::int_t incr = 0;

    if (decodeIndexedExtra(memory, insn, post, index, addr, offSize, base, incr) == OK) {
        if (getError()) return getError();
        if (offSize) offset = static_cast<target::int16_t>(addr);
    } else if (mode == 0x84) {
        // ,R [,R]
        ;
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
    } else if (decodeIndexReg(post & 0xf) != NONE) {
        // R,R [R,R]
        index = decodeIndexReg(mode & 0xf);
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
                operands = outRegName(operands, index);
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
        operands = outRegName(operands, base);
        for (; incr > 0; incr--) *operands++ = '+';
    }
    if (indir) *operands++ = ']';
    *operands = 0;
    *comments = 0;
    return setError(OK);
}

Error DisMc6809::decodeRelative(
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

Error DisMc6809::decodeImmediate(
    Memory& memory, Insn &insn, char *operands, char *comments) {
    if (insn.oprLen() == 1) {
        target::byte_t val;
        if (readByte(memory, insn, val)) return getError();
        *operands++ = '#';
        outOpr8Hex(operands, val);
        if (insn.insnCode() == 0x1A) {
            outCCRBits(comments, val);
        } else if (insn.insnCode() == 0x1C || insn.insnCode() == 0x3C) {
            if (val != 0xFF) {
                *comments++ = '~';
                outCCRBits(comments, ~val);
            }
        } else {
            outInt16(comments, val);
        }
        return setError(OK);
    }
    if (insn.oprLen() == 2) {
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
    }
    if (decodeImmediateExtra(memory, insn, operands, comments) == OK)
        return getError();
    return setError(OK);
}

Error DisMc6809::decodeStackOp(
    Memory &memory, Insn &insn, char *operands, char *comments) {
    target::byte_t post;
    if (readByte(memory, insn, post)) return getError();
    if (post == 0) {
        *operands++ = '#';
        outOpr8Hex(operands, post);
        return setError(OK);
    }
    const RegName *table = (insn.insnCode() & 2) == 0
        ? &TABLE_STACK_OP_S[0] : &TABLE_STACK_OP_U[0];
    const bool push = (insn.insnCode() & 1) == 0;
    for (host::uint_t i = 0, n = 0; i < 8; i++) {
        const host::uint_t bit = push ? 7 - i : i;
        if (post & BIT_MASKS[bit]) {
            if (n != 0) *operands++ = ',';
            operands = outRegName(operands, table[bit]);
            n++;
        }
    }
    *operands = 0;
    return setError(OK);
}

Error DisMc6809::decodeRegisters(
    Memory &memory, Insn &insn, char *operands, char *comments) {
    target::byte_t post;
    if (readByte(memory, insn, post)) return getError();
    const RegName src = decodeRegName(post >> 4);
    const RegName dst = decodeRegName(post & 0xf);
    if (src == NONE || dst == NONE) return setError(ILLEGAL_REGISTER);
    operands = outRegName(operands, src);
    *operands++ = ',';
    outRegName(operands, dst);
    return setError(OK);
}

Error DisMc6809::decode(
    Memory &memory, Insn &insn, char *operands, char *comments, SymbolTable *symtab) {
    reset(symtab);
    resetAddress(insn, memory.address());
    *operands = *comments = 0;

    target::opcode_t opCode;
    if (readByte(memory, insn, opCode)) return getError();
    target::insn_t insnCode = opCode;
    if (TableMc6809.isPrefixCode(opCode)) {
        const target::opcode_t prefix = opCode;
        if (readByte(memory, insn, opCode)) return getError();
        insnCode = TableMc6809.insnCode(prefix, opCode);
    }
    setInsnCode(insn, insnCode);

    if (TableMc6809.search(insn, insn.insnCode()))
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
    default:
        return setError(INTERNAL_ERROR);
    }
}

#if 0
static const char T_FCB[]   PROGMEM = "FCB";

Error DisMc6809::formConstantBytes(
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
