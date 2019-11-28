/* -*- mode: c++; -*- */
#ifndef __ASM_HD6309_IMPL_H__
#define __ASM_HD6309_IMPL_H__

#include <ctype.h>

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

static const char *skipSpace(const char *line) {
    while (*line == ' ') line++;
    return line;
}

static Error getHex16(const char *&in, target::uint16_t &val) {
    const char *p = in;
    if (!isxdigit(*p)) return UNKNOWN_OPERAND;
    val = 0;
    while (isxdigit(*p)) {
        val <<= 4;
        val += isdigit(*p) ? *p - '0' : toupper(*p) - 'A' + 10;
        p++;
    }
    in = p;
    return OK;
}

static Error getHex32(const char *&in, target::uint32_t &val) {
    const char *p = in;
    if (!isxdigit(*p)) return UNKNOWN_OPERAND;
    val = 0;
    while (isxdigit(*p)) {
        val <<= 4;
        val += isdigit(*p) ? *p - '0' : toupper(*p) - 'A' + 10;
        p++;
    }
    in = p;
    return OK;
}

static Error getInt16(const char *&in, target::uint16_t &val) {
    const char *p = in;
    const char sign = (*p == '+' || *p == '-') ? *p++ : 0;
    if (!isdigit(*p)) return UNKNOWN_OPERAND;
    val = 0;
    while (isdigit(*p)) {
        val *= 10;
        val += *p - '0';
        p++;
    }
    in = p;
    if (sign == '-') val = -(target::int16_t)val;
    return OK;
}

static Error getInt32(const char *&in, target::uint32_t &val) {
    const char *p = in;
    const char sign = (*p == '+' || *p == '-') ? *p++ : 0;
    if (!isdigit(*p)) return UNKNOWN_OPERAND;
    val = 0;
    while (isdigit(*p)) {
        val *= 10;
        val += *p - '0';
        p++;
    }
    in = p;
    if (sign == '-') val = -(target::int32_t)val;
    return OK;
}

template<McuType mcuType>
Error Assembler<mcuType>::getOperand16(const char *&in, target::uint16_t &val) const {
    if (*in == '$') {
        in++;
        return getHex16(in, val);
    }
    if (getInt16(in, val) == OK) return OK;
    char symbol_buffer[20];
    host::uint_t idx;
    for (idx = 0; idx < sizeof(symbol_buffer) - 1 && isidchar(in[idx]); idx++) {
        symbol_buffer[idx] = in[idx];
    }
    symbol_buffer[idx] = 0;
    if (hasSymbol(symbol_buffer)) {
        val = lookup(symbol_buffer);
        in += idx;
        return OK;
    }
    return UNKNOWN_OPERAND;
}

template<McuType mcuType>
Error Assembler<mcuType>::getOperand32(const char *&in, target::uint32_t &val) const {
    if (*in == '$') {
        in++;
        return getHex32(in, val);
    }
    if (getInt32(in, val) == OK) return OK;
    char symbol_buffer[20];
    host::uint_t idx;
    for (idx = 0; idx < sizeof(symbol_buffer) - 1 && isidchar(in[idx]); idx++) {
        symbol_buffer[idx] = in[idx];
    }
    symbol_buffer[idx] = 0;
    if (hasSymbol(symbol_buffer)) {
        val = lookup(symbol_buffer);
        in += idx;
        return OK;
    }
    return UNKNOWN_OPERAND;
}

template<McuType mcuType>
Error Assembler<mcuType>::encodeStackOp(const char *line, Insn &insn) {
    target::byte_t post = 0;
    while (*line) {
        host::uint_t bit = 0;
        host::uint_t reg_d_bits = 0;
        for (host::uint_t index = 0, mask = 0x01; index < 8; index++, mask <<= 1) {
            const RegName regName = _regs.getStackReg(index, insn.insnCode());
            if (regName == REG_A || regName == REG_B) reg_d_bits |= mask;
            if (_regs.compareRegName(line, regName)) {
                line += _regs.regNameLen(regName);
                bit = mask;
                break;
            }
        }
        if (bit == 0 && _regs.compareRegName(line, REG_D)) {
            line += _regs.regNameLen(REG_D);
            bit = reg_d_bits;
        }
        if (bit == 0) return setError(UNKNOWN_REGISTER);
        post |= bit;
        if (*line == ',') line++;
    }
    emitInsnCode(insn);
    insn.emitByte(post);
    return *skipSpace(line) == 0 ? setError(OK) : setError(GARBAGE_AT_END);
}

template<McuType mcuType>
Error Assembler<mcuType>::encodeRegisters(const char *line, Insn &insn) {
    RegName regName;
    if ((regName = _regs.parseDataReg(line)) == REG_UNDEF)
        return setError(UNKNOWN_REGISTER);
    line += _regs.regNameLen(regName);
    if (*line++ != ',') return setError(UNKNOWN_OPERAND);
    target::byte_t post = _regs.encodeDataReg(regName) << 4;
    if ((regName = _regs.parseDataReg(line)) == REG_UNDEF)
        return setError(UNKNOWN_REGISTER);
    line += _regs.regNameLen(regName);
    if (*skipSpace(line)) return setError(GARBAGE_AT_END);
    post |= _regs.encodeDataReg(regName);
    emitInsnCode(insn);
    insn.emitByte(post);
    return setError(OK);
}

template<McuType mcuType>
Error Assembler<mcuType>::encodeRelative(const char *line, Insn &insn) {
    target::uintptr_t addr;
    if (getOperand16(line, addr)) return setError(UNKNOWN_OPERAND);
    const target::opcode_t prefix = InsnTableUtils::prefixCode(insn.insnCode());
    const host::uint_t insnLen = (InsnTableUtils::isPrefixCode(prefix) ? 2 : 1)
        + (insn.addrMode() == REL8 ? 1 : 2);
    const target::uintptr_t base = insn.address() + insnLen;
    const target::ptrdiff_t delta = addr - base;
    emitInsnCode(insn);
    if (insn.addrMode() == REL8) {
        if (delta >= 128 || delta < -128) return setError(OPERAND_TOO_FAR);
        insn.emitByte(target::byte_t(delta));
    } else {
        insn.emitUint16(delta);
    }
    return *skipSpace(line) == 0 ? setError(OK) : setError(GARBAGE_AT_END);
}

template<McuType mcuType>
Error Assembler<mcuType>::encodeImmediate(const char *line, Insn &insn) {
    if (*line++ != '#') return setError(UNKNOWN_OPERAND);
    emitInsnCode(insn);
    if (insn.addrMode() == IMM8 || insn.addrMode() == IMM16) {
        target::uint16_t val;
        if (getOperand16(line, val)) return setError(UNKNOWN_OPERAND);
        if (insn.addrMode() == IMM8) insn.emitByte(target::byte_t(val));
        else insn.emitUint16(val);
    } else if (mcuType == HD6309 && insn.addrMode() == IMM32) {
        uint32_t val;
        if (getOperand32(line, val)) return setError(UNKNOWN_OPERAND);
        insn.emitUint32(val);
    } else {
        return setError(UNKNOWN_OPERAND);
    }
    return *skipSpace(line) == 0 ? setError(OK) : setError(GARBAGE_AT_END);
}

template<McuType mcuType>
Error Assembler<mcuType>::encodeDirect(const char *line, Insn &insn, bool emitInsn) {
    if (*line == '<') line++;
    if (emitInsn) emitInsnCode(insn);
    target::uintptr_t dir;
    if (getOperand16(line, dir)) return setError(UNKNOWN_OPERAND);
    insn.emitByte(target::byte_t(dir));
    return setError(OK);
}

template<McuType mcuType>
Error Assembler<mcuType>::encodeExtended(const char *line, Insn &insn, bool emitInsn) {
    if (*line == '>') line++;
    if (emitInsn) emitInsnCode(insn);
    target::uintptr_t addr;
    if (getOperand16(line, addr)) return setError(UNKNOWN_OPERAND);
    insn.emitUint16(addr);
    return *skipSpace(line) == 0 ? setError(OK) : setError(GARBAGE_AT_END);
}

template<McuType mcuType>
Error Assembler<mcuType>::encodeIndexed(const char *line, Insn &insn, bool emitInsn) {
    if (emitInsn) emitInsnCode(insn);
    const bool indir = (*line == '[');
    RegName base = REG_UNDEF;
    RegName index = REG_UNDEF;
    host::int_t incr = 0;       // auto decrement/increment
    target::uintptr_t addr;
    if (indir) line++;
    if (*line != ',') {
        if ((index = _regs.parseIndexReg(line)) != REG_UNDEF) {
            line += _regs.regNameLen(index); // index register
        } else {
            if (getOperand16(line, addr)) return setError(UNKNOWN_OPERAND);
            index = OFFSET;     // index is in addr
        }
    }
    if (*line == ',') {
        line++;
        if (index == REG_UNDEF) {
            while (*line == '-') {
                line++;
                incr--;
            }
        }
        if ((base = _regs.parseBaseReg(line)) != REG_UNDEF
            || _regs.compareRegName(line, REG_PC)) {
            if (base == REG_UNDEF) base = REG_PC;
            line += _regs.regNameLen(base);
        } else setError(UNKNOWN_OPERAND);
        if (index == REG_UNDEF && incr == 0) {
            while (*line == '+') {
                line++;
                incr++;
            }
        }
    }
    if (indir && *line++ != ']') return setError(UNKNOWN_OPERAND);
    if (*skipSpace(line)) return setError(GARBAGE_AT_END);

    target::byte_t post;
    if (base == REG_UNDEF) {    // [n16]
        if (index != OFFSET) return setError(UNKNOWN_OPERAND);
        insn.emitByte(0x9F);
        insn.emitUint16(addr);
        return setError(OK);
    }
    if (base == REG_PC) {       // n,PC [n,PC]
        if (index != OFFSET || incr != 0) return setError(UNKNOWN_OPERAND);
        post = indir ? 0x10 : 0;
        target::ptrdiff_t delta = addr - (insn.address() + insn.insnLen() + 2);
        if (delta >= -128 && delta < 128) {
            insn.emitByte(0x8C | post);
            insn.emitByte((uint8_t)delta);
            return setError(OK);
        }
        delta = addr - (insn.address() + insn.insnLen() + 3);
        insn.emitByte(0x8D | post);
        insn.emitUint16(delta);
        return setError(OK);
    }
    if (mcuType == HD6309 && base == REG_W) {
        target::byte_t post;
        if (index == OFFSET) post = 0xAF;   // n16,W [n16,W]
        else if (incr == 0) post = 0x8F;    // ,W [,W]
        else if (incr == 2) post = 0xCF;    // ,W++ [,W++]
        else if (incr == -2) post = 0xEF;   // ,--W [,--W]
        else return setError(UNKNOWN_OPERAND);
        if (indir) post++;
        insn.emitByte(post);
        if (index == OFFSET) insn.emitUint16(addr);
        return setError(OK);
    }

    post = _regs.encodeBaseReg(base) << 5;
    if (indir) post |= 0x10;
    if (index == REG_UNDEF) { // ,R [,R] ,R+ ,R- ,R++ ,--R [,R++] [,--R]
        if (incr == 0) post |= 0x84;
        else if (incr == 2) post |= 0x81;
        else if (incr == -2) post |= 0x83;
        else if (!indir && incr == 1) post |= 0x80;
        else if (!indir && incr == -1) post |= 0x82;
        else return setError(UNKNOWN_OPERAND);
        insn.emitByte(post);
        return setError(OK);
    }
    if (index != OFFSET) {      // R,R
        post |= _regs.encodeIndexReg(index);
        insn.emitByte(0x80 | post);
        return setError(OK);
    }
    const target::ptrdiff_t offset = addr;
    if (offset >= -16 && offset < 16 && !indir) { // n5.R
        post |= (offset & 0x1F);
        insn.emitByte(post);
        return setError(OK);
    }
    if (offset >= -128 && offset < 128) { // n8,R [n8,R]
        post |= 0x88;
        insn.emitByte(post);
        insn.emitByte((uint8_t)offset);
        return setError(OK);
    }
    // n16,R [n16,R]
    post |= 0x89;
    insn.emitByte(post);
    insn.emitUint16(offset);
    return setError(OK);
}

template<McuType mcuType>
Error Assembler<mcuType>::encodeBitOperation(const char *line, Insn &insn) {
    const RegName regName = _regs.parseBitOpReg(line);
    if (regName == REG_UNDEF) return setError(UNKNOWN_REGISTER);
    line += _regs.regNameLen(regName);
    uint8_t post = _regs.encodeBitOpReg(regName) << 6;
    if (*line++ != ',') return setError(UNKNOWN_OPERAND);
    uint16_t pos;
    if (getOperand16(line, pos)) return setError(UNKNOWN_OPERAND);
    if (pos >= 8) return setError(ILLEGAL_BIT_NUMBER);
    if (*line++ != ',') return setError(UNKNOWN_OPERAND);
    post |= (pos << 3);
    if (getOperand16(line, pos)) return setError(UNKNOWN_OPERAND);
    if (pos >= 8) return setError(ILLEGAL_BIT_NUMBER);
    if (*line++ != ',') return setError(UNKNOWN_OPERAND);
    post |= pos;
    emitInsnCode(insn);
    insn.emitByte(post);
    return encodeDirect(line, insn, /* emitInsn */ false);
}

template<McuType mcuType>
Error Assembler<mcuType>::encodeImmediatePlus(const char *line, Insn &insn) {
    if (*line++ != '#') return setError(UNKNOWN_OPERAND);
    uint16_t val;
    if (getOperand16(line, val)) return setError(UNKNOWN_OPERAND);
    if (*line++ != ',') return setError(UNKNOWN_OPERAND);

    if (determineAddrMode(line, insn)) return getError();
    switch (insn.addrMode()) {
    case DIRP: insn.setAddrMode(IMMDIR); break;
    case EXTD: insn.setAddrMode(IMMEXT); break;
    case INDX: insn.setAddrMode(IMMIDX); break;
    default: return setError(UNKNOWN_OPERAND);
    }
    if (InsnTable<mcuType>::table()->searchNameAndAddrMode(insn))
        return setError(UNKNOWN_INSTRUCTION);
    emitInsnCode(insn);
    insn.emitByte((uint8_t)val);
    switch (insn.addrMode()) {
    case IMMDIR:
        return encodeDirect(line, insn, /* emitInsn */ false);
    case IMMEXT:
        return encodeExtended(line, insn, /* emitInsn */ false);
    case IMMIDX:
        return encodeIndexed(line, insn, /* emitInsn */ false);
    default: return setError(UNKNOWN_OPERAND);
    }
}

template<McuType mcuType>
Error Assembler<mcuType>::encodeTransferMemory(const char *line, Insn &insn) {
    RegName regName = _regs.parseTfmBaseReg(line);
    if (regName == REG_UNDEF) return setError(UNKNOWN_REGISTER);
    line += _regs.regNameLen(regName);
    char srcMode = 0;
    if (*line == '+' || *line == '-') srcMode = *line++;
    if (*line++ != ',') return setError(UNKNOWN_OPERAND);
    target::opcode_t post = _regs.encodeTfmBaseReg(regName) << 4;

    regName = _regs.parseTfmBaseReg(line);
    if (regName == REG_UNDEF) return setError(UNKNOWN_REGISTER);
    line += _regs.regNameLen(regName);
    char dstMode = 0;
    if (*line == '+' || *line == '-') dstMode = *line++;
    if (*skipSpace(line)) return setError(GARBAGE_AT_END);
    post |= _regs.encodeTfmBaseReg(regName);

    for (uint8_t mode = 0; mode < 4; mode++) {
        if (srcMode == _regs.tfmSrcModeChar(mode)
            && dstMode == _regs.tfmDstModeChar(mode)) {
            const target::opcode_t prefixCode =
                InsnTableUtils::prefixCode(insn.insnCode());
            insn.setInsnCode(InsnTableUtils::insnCode(prefixCode, 0x38 + mode));
            emitInsnCode(insn);
            insn.emitByte(post);
            return setError(OK);
        }
    }
    return setError(UNKNOWN_OPERAND);
}

template<McuType mcuType>
Error Assembler<mcuType>::determineAddrMode(const char *line, Insn &insn) {
    switch (*line) {
    case '#': insn.setAddrMode(IMM8); break;
    case '<': insn.setAddrMode(DIRP); break;
    case '>': insn.setAddrMode(EXTD); break;
    case '[':
    case ',': insn.setAddrMode(INDX); break;
    default:
        RegName index;
        if ((index = _regs.parseIndexReg(line)) != REG_UNDEF) {
            line += _regs.regNameLen(index);
            if (*line == ',') {
                insn.setAddrMode(INDX);
                return OK;
            }
            return setError(UNKNOWN_OPERAND);
        }
        target::uint16_t val;
        if (getOperand16(line, val)) return setError(UNKNOWN_OPERAND);
        if (*line == ',') insn.setAddrMode(INDX);
        else insn.setAddrMode(val < 0x100 ? DIRP : EXTD);
        break;
    }
    return OK;
}

template<McuType mcuType>
Error Assembler<mcuType>::encode(
    const char *line, Insn &insn, target::uintptr_t addr, SymbolTable *symtab) {
    reset(symtab);
    insn.resetAddress(addr);
    line = skipSpace(line);
    if (!*line) return setError(NO_TEXT);
    const char *endName;
    for (endName = line; isidchar(*endName); endName++)
        ;
    insn.setName(line, endName);
    line = skipSpace(endName);

    if (InsnTable<mcuType>::table()->searchName(insn))
        return setError(UNKNOWN_INSTRUCTION);
    if (insn.mcuType() == HD6309 && mcuType != HD6309)
        return setError(UNKNOWN_INSTRUCTION);

    switch (insn.addrMode()) {
    case INHR:
        emitInsnCode(insn);
        return *skipSpace(line) == 0 ? setError(OK) : setError(GARBAGE_AT_END);
    case REL8:
    case REL16:
        return encodeRelative(line, insn);
    case STKOP:
        return encodeStackOp(line, insn);
    case REGS:
        return encodeRegisters(line, insn);
    default:
        if (mcuType == HD6309) {
            switch (insn.addrMode()) {
            case IMMDIR:
            case IMMEXT:
            case IMMIDX:
                return encodeImmediatePlus(line, insn);
            case BITOP:
                return encodeBitOperation(line, insn);
            case TFRM:
                return encodeTransferMemory(line, insn);
            default:
                break;
            }
        }
        break;
    }

    if (determineAddrMode(line, insn)) return getError();
    if (InsnTable<mcuType>::table()->searchNameAndAddrMode(insn))
        return setError(UNKNOWN_INSTRUCTION);
    switch (insn.addrMode()) {
    case IMM8:
    case IMM16:
        return encodeImmediate(line, insn);
    case DIRP:
        return encodeDirect(line, insn);
    case EXTD:
        return encodeExtended(line, insn);
    case INDX:
        return encodeIndexed(line, insn);
    default:
        if (mcuType == HD6309 && insn.addrMode() == IMM32) {
            return encodeImmediate(line, insn);
        } else {
            return setError(UNKNOWN_OPERAND);
        }
    }
}

#endif // __ASM_HD6309_IMPL_H__
