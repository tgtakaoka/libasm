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

template<McuType mcuType>
Error Asm09<mcuType>::checkLineEnd() {
    if (*skipSpace(_scan) == 0) return setError(OK);
    return setError(GARBAGE_AT_END);
}

template<McuType mcuType>
Error Asm09<mcuType>::getHex16(uint16_t &val) {
    const char *p = _scan;
    if (!isxdigit(*p)) return UNKNOWN_OPERAND;
    uint16_t v = 0;
    while (isxdigit(*p)) {
        v <<= 4;
        v += isdigit(*p) ? *p - '0' : toupper(*p) - 'A' + 10;
        p++;
    }
    val = v;
    _scan = p;
    return OK;
}

template<McuType mcuType>
Error Asm09<mcuType>::getHex32(uint32_t &val) {
    const char *p = _scan;
    if (!isxdigit(*p)) return UNKNOWN_OPERAND;
    uint32_t v = 0;
    while (isxdigit(*p)) {
        v <<= 4;
        v += isdigit(*p) ? *p - '0' : toupper(*p) - 'A' + 10;
        p++;
    }
    val = v;
    _scan = p;
    return OK;
}

template<McuType mcuType>
Error Asm09<mcuType>::getInt16(uint16_t &val) {
    const char *p = _scan;
    const char sign = (*p == '+' || *p == '-') ? *p++ : 0;
    if (!isdigit(*p)) return UNKNOWN_OPERAND;
    uint16_t v = 0;
    while (isdigit(*p)) {
        v *= 10;
        v += *p - '0';
        p++;
    }
    if (sign == '-') v = -(int16_t)v;
    val = v;
    _scan = p;
    return OK;
}

template<McuType mcuType>
Error Asm09<mcuType>::getInt32(uint32_t &val) {
    const char *p = _scan;
    const char sign = (*p == '+' || *p == '-') ? *p++ : 0;
    if (!isdigit(*p)) return UNKNOWN_OPERAND;
    uint32_t v = 0;
    while (isdigit(*p)) {
        v *= 10;
        v += *p - '0';
        p++;
    }
    if (sign == '-') v = -(int32_t)v;
    val = v;
    _scan = p;
    return OK;
}

template<McuType mcuType>
Error Asm09<mcuType>::getOperand16(uint16_t &val) {
    if (*_scan == '$') {
        _scan++;
        return getHex16(val);
    }
    if (getInt16(val) == OK) return OK;
    char symbol_buffer[20];
    host::uint_t idx;
    for (idx = 0; idx < sizeof(symbol_buffer) - 1 && isidchar(_scan[idx]); idx++) {
        symbol_buffer[idx] = _scan[idx];
    }
    symbol_buffer[idx] = 0;
    if (hasSymbol(symbol_buffer)) {
        val = lookup(symbol_buffer);
        _scan += idx;
        return OK;
    }
    return UNKNOWN_OPERAND;
}

template<McuType mcuType>
Error Asm09<mcuType>::getOperand32(uint32_t &val) {
    if (*_scan == '$') {
        _scan++;
        return getHex32(val);
    }
    if (getInt32(val) == OK) return OK;
    char symbol_buffer[20];
    host::uint_t idx;
    for (idx = 0; idx < sizeof(symbol_buffer) - 1 && isidchar(_scan[idx]); idx++) {
        symbol_buffer[idx] = _scan[idx];
    }
    symbol_buffer[idx] = 0;
    if (hasSymbol(symbol_buffer)) {
        val = lookup(symbol_buffer);
        _scan += idx;
        return OK;
    }
    return UNKNOWN_OPERAND;
}

template<McuType mcuType>
Error Asm09<mcuType>::encodeStackOp(Insn &insn) {
    uint8_t post = 0;
    const char *line = _scan;
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
    _scan = line;
    emitInsnCode(insn);
    insn.emitByte(post);
    return checkLineEnd();
}

template<McuType mcuType>
Error Asm09<mcuType>::encodeRegisters(Insn &insn) {
    RegName regName;
    if ((regName = _regs.parseDataReg(_scan)) == REG_UNDEF)
        return setError(UNKNOWN_REGISTER);
    _scan += _regs.regNameLen(regName);
    if (*_scan != ',') return setError(UNKNOWN_OPERAND);
    _scan++;
    uint8_t post = _regs.encodeDataReg(regName) << 4;
    if ((regName = _regs.parseDataReg(_scan)) == REG_UNDEF)
        return setError(UNKNOWN_REGISTER);
    _scan += _regs.regNameLen(regName);
    if (checkLineEnd()) return setError(GARBAGE_AT_END);
    post |= _regs.encodeDataReg(regName);
    emitInsnCode(insn);
    insn.emitByte(post);
    return setError(OK);
}

template<McuType mcuType>
Error Asm09<mcuType>::encodeRelative(Insn &insn) {
    target::uintptr_t addr;
    if (getOperand16(addr)) return setError(UNKNOWN_OPERAND);
    const target::opcode_t prefix = TableHd6309Base::prefixCode(insn.insnCode());
    const host::uint_t insnLen = (TableHd6309Base::isPrefixCode(prefix) ? 2 : 1)
        + (insn.addrMode() == REL8 ? 1 : 2);
    const target::uintptr_t base = insn.address() + insnLen;
    const target::ptrdiff_t delta = addr - base;
    emitInsnCode(insn);
    if (insn.addrMode() == REL8) {
        if (delta >= 128 || delta < -128) return setError(OPERAND_TOO_FAR);
        insn.emitByte(uint8_t(delta));
    } else {
        insn.emitUint16(delta);
    }
    return checkLineEnd();
}

template<McuType mcuType>
Error Asm09<mcuType>::encodeImmediate(Insn &insn) {
    if (*_scan != '#') return setError(UNKNOWN_OPERAND);
    _scan++;
    emitInsnCode(insn);
    if (insn.addrMode() == IMM8 || insn.addrMode() == IMM16) {
        uint16_t val;
        if (getOperand16(val)) return setError(UNKNOWN_OPERAND);
        if (insn.addrMode() == IMM8) insn.emitByte(uint8_t(val));
        else insn.emitUint16(val);
    } else if (mcuType == HD6309 && insn.addrMode() == IMM32) {
        uint32_t val;
        if (getOperand32(val)) return setError(UNKNOWN_OPERAND);
        insn.emitUint32(val);
    } else {
        return setError(UNKNOWN_OPERAND);
    }
    return checkLineEnd();
}

template<McuType mcuType>
Error Asm09<mcuType>::encodeDirect(Insn &insn, bool emitInsn) {
    if (*_scan == '<') _scan++;
    if (emitInsn) emitInsnCode(insn);
    target::uintptr_t dir;
    if (getOperand16(dir)) return setError(UNKNOWN_OPERAND);
    insn.emitByte(uint8_t(dir));
    return checkLineEnd();
}

template<McuType mcuType>
Error Asm09<mcuType>::encodeExtended(Insn &insn, bool emitInsn) {
    if (*_scan == '>') _scan++;
    if (emitInsn) emitInsnCode(insn);
    target::uintptr_t addr;
    if (getOperand16(addr)) return setError(UNKNOWN_OPERAND);
    insn.emitUint16(addr);
    return checkLineEnd();
}

template<McuType mcuType>
Error Asm09<mcuType>::encodeIndexed(Insn &insn, bool emitInsn) {
    if (emitInsn) emitInsnCode(insn);
    const bool indir = (*_scan == '[');
    RegName base = REG_UNDEF;
    RegName index = REG_UNDEF;
    host::int_t incr = 0;       // auto decrement/increment
    target::uintptr_t addr;
    if (indir) _scan++;
    if (*_scan != ',') {
        if ((index = _regs.parseIndexReg(_scan)) != REG_UNDEF) {
            _scan += _regs.regNameLen(index); // index register
        } else {
            if (getOperand16(addr)) return setError(UNKNOWN_OPERAND);
            index = OFFSET;     // index is in addr
        }
    }
    if (*_scan == ',') {
        _scan++;
        if (index == REG_UNDEF) {
            while (*_scan == '-') {
                _scan++;
                incr--;
            }
        }
        if ((base = _regs.parseBaseReg(_scan)) != REG_UNDEF
            || _regs.compareRegName(_scan, REG_PC)) {
            if (base == REG_UNDEF) base = REG_PC;
            _scan += _regs.regNameLen(base);
        } else setError(UNKNOWN_OPERAND);
        if (index == REG_UNDEF && incr == 0) {
            while (*_scan == '+') {
                _scan++;
                incr++;
            }
        }
    }
    if (indir) {
        if (*_scan != ']') return setError(UNKNOWN_OPERAND);
        _scan++;
    }
    if (checkLineEnd()) return setError(GARBAGE_AT_END);

    uint8_t post;
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
        uint8_t post;
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
Error Asm09<mcuType>::encodeBitOperation(Insn &insn) {
    const RegName regName = _regs.parseBitOpReg(_scan);
    if (regName == REG_UNDEF) return setError(UNKNOWN_REGISTER);
    _scan += _regs.regNameLen(regName);
    uint8_t post = _regs.encodeBitOpReg(regName) << 6;
    if (*_scan != ',') return setError(UNKNOWN_OPERAND);
    _scan++;
    uint16_t pos;
    if (getOperand16(pos)) return setError(UNKNOWN_OPERAND);
    if (pos >= 8) return setError(ILLEGAL_BIT_NUMBER);
    if (*_scan != ',') return setError(UNKNOWN_OPERAND);
    _scan++;
    post |= (pos << 3);
    if (getOperand16(pos)) return setError(UNKNOWN_OPERAND);
    if (pos >= 8) return setError(ILLEGAL_BIT_NUMBER);
    if (*_scan != ',') return setError(UNKNOWN_OPERAND);
    _scan++;
    post |= pos;
    emitInsnCode(insn);
    insn.emitByte(post);
    return encodeDirect(insn, /* emitInsn */ false);
}

template<McuType mcuType>
Error Asm09<mcuType>::encodeImmediatePlus(Insn &insn) {
    if (*_scan != '#') return setError(UNKNOWN_OPERAND);
    _scan++;
    uint16_t val;
    if (getOperand16(val)) return setError(UNKNOWN_OPERAND);
    if (*_scan != ',') return setError(UNKNOWN_OPERAND);
    _scan++;

    if (determineAddrMode(_scan, insn)) return getError();
    switch (insn.addrMode()) {
    case DIRP: insn.setAddrMode(IMMDIR); break;
    case EXTD: insn.setAddrMode(IMMEXT); break;
    case INDX: insn.setAddrMode(IMMIDX); break;
    default: return setError(UNKNOWN_OPERAND);
    }
    if (TableHd6309<mcuType>::table()->searchNameAndAddrMode(insn))
        return setError(UNKNOWN_INSTRUCTION);
    emitInsnCode(insn);
    insn.emitByte((uint8_t)val);
    switch (insn.addrMode()) {
    case IMMDIR: return encodeDirect(insn, /* emitInsn */ false);
    case IMMEXT: return encodeExtended(insn, /* emitInsn */ false);
    case IMMIDX: return encodeIndexed(insn, /* emitInsn */ false);
    default:     return setError(UNKNOWN_OPERAND);
    }
}

template<McuType mcuType>
Error Asm09<mcuType>::encodeTransferMemory(Insn &insn) {
    RegName regName = _regs.parseTfmBaseReg(_scan);
    if (regName == REG_UNDEF) return setError(UNKNOWN_REGISTER);
    _scan += _regs.regNameLen(regName);
    char srcMode = 0;
    if (*_scan == '+' || *_scan == '-') srcMode = *_scan++;
    if (*_scan != ',') return setError(UNKNOWN_OPERAND);
    _scan++;
    target::opcode_t post = _regs.encodeTfmBaseReg(regName) << 4;

    regName = _regs.parseTfmBaseReg(_scan);
    if (regName == REG_UNDEF) return setError(UNKNOWN_REGISTER);
    _scan += _regs.regNameLen(regName);
    char dstMode = 0;
    if (*_scan == '+' || *_scan == '-') dstMode = *_scan++;
    if (checkLineEnd()) return setError(GARBAGE_AT_END);
    post |= _regs.encodeTfmBaseReg(regName);

    for (uint8_t mode = 0; mode < 4; mode++) {
        if (srcMode == _regs.tfmSrcModeChar(mode)
            && dstMode == _regs.tfmDstModeChar(mode)) {
            const target::opcode_t prefixCode =
                TableHd6309Base::prefixCode(insn.insnCode());
            insn.setInsnCode(TableHd6309Base::insnCode(prefixCode, 0x38 + mode));
            emitInsnCode(insn);
            insn.emitByte(post);
            return setError(OK);
        }
    }
    return setError(UNKNOWN_OPERAND);
}

template<McuType mcuType>
Error Asm09<mcuType>::determineAddrMode(const char *line, Insn &insn) {
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
        uint16_t val;
        const char *scan = _scan;
        _scan = line;
        if (getOperand16(val)) return setError(UNKNOWN_OPERAND);
        if (*_scan == ',') insn.setAddrMode(INDX);
        else insn.setAddrMode(val < 0x100 ? DIRP : EXTD);
        _scan = scan;
        break;
    }
    return OK;
}

template<McuType mcuType>
Error Asm09<mcuType>::encode(
    const char *line, Insn &insn, target::uintptr_t addr, SymbolTable *symtab) {
    reset(skipSpace(line), symtab);
    insn.resetAddress(addr);
    if (!*_scan) return setError(NO_TEXT);
    const char *endName;
    for (endName = _scan; isidchar(*endName); endName++)
        ;
    insn.setName(_scan, endName);
    _scan = skipSpace(endName);

    if (TableHd6309<mcuType>::table()->searchName(insn))
        return setError(UNKNOWN_INSTRUCTION);
    if (insn.mcuType() == HD6309 && mcuType != HD6309)
        return setError(UNKNOWN_INSTRUCTION);

    switch (insn.addrMode()) {
    case INHR:  emitInsnCode(insn); return checkLineEnd();
    case REL8:
    case REL16: return encodeRelative(insn);
    case STKOP: return encodeStackOp(insn);
    case REGS:  return encodeRegisters(insn);
    default:
        if (mcuType == HD6309) {
            switch (insn.addrMode()) {
            case IMMDIR:
            case IMMEXT:
            case IMMIDX: return encodeImmediatePlus(insn);
            case BITOP:  return encodeBitOperation(insn);
            case TFRM:   return encodeTransferMemory(insn);
            default:     break;
            }
        }
        break;
    }

    if (determineAddrMode(_scan, insn)) return getError();
    if (TableHd6309<mcuType>::table()->searchNameAndAddrMode(insn))
        return setError(UNKNOWN_INSTRUCTION);
    switch (insn.addrMode()) {
    case IMM8:
    case IMM16: return encodeImmediate(insn);
    case DIRP:  return encodeDirect(insn);
    case EXTD:  return encodeExtended(insn);
    case INDX:  return encodeIndexed(insn);
    default:
        if (mcuType == HD6309 && insn.addrMode() == IMM32) {
            return encodeImmediate(insn);
        } else {
            return setError(UNKNOWN_OPERAND);
        }
    }
}

#endif // __ASM_HD6309_IMPL_H__
