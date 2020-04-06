/*
 * Copyright 2020 Tadashi G. Takaoka
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "asm_mc6809.h"
#include "table_mc6809.h"

namespace libasm {
namespace mc6809 {

Error AsmMc6809::encodeStackOp(Insn &insn) {
    uint8_t post = 0;
    const char *p = _scan;
    while (*p) {
        host::uint_t bit = 0;
        host::uint_t reg_d_bits = 0;
        for (host::uint_t index = 0, mask = 0x01; index < 8; index++, mask <<= 1) {
            const RegName regName = _regs.getStackReg(index, insn.insnCode());
            if (regName == REG_A || regName == REG_B) reg_d_bits |= mask;
            if (_regs.compareRegName(p, regName)) {
                p += _regs.regNameLen(regName);
                bit = mask;
                break;
            }
        }
        if (bit == 0 && _regs.compareRegName(p, REG_D)) {
            p += _regs.regNameLen(REG_D);
            bit = reg_d_bits;
        }
        if (bit == 0) return setError(UNKNOWN_REGISTER);
        post |= bit;
        while (isspace(*p)) p++;
        if (*p != ',') break;
        p++;
    }
    insn.emitInsn();
    insn.emitByte(post);
    _scan = p;
    return checkLineEnd();
}

Error AsmMc6809::encodeRegisters(Insn &insn) {
    const char *p = _scan;
    const RegName reg1 = _regs.parseDataReg(p);
    if (reg1 == REG_UNDEF)
        return setError(UNKNOWN_REGISTER);
    p += _regs.regNameLen(reg1);
    if (*p != ',') return setError(UNKNOWN_OPERAND);
    _scan = ++p;
    const RegName reg2 = _regs.parseDataReg(p);
    if (reg2 == REG_UNDEF)
        return setError(UNKNOWN_REGISTER);
    p += _regs.regNameLen(reg2);

    const OprSize size1 = RegMc6809::regSize(reg1);
    const OprSize size2 = RegMc6809::regSize(reg2);
    if (size1 != SZ_NONE && size2 != SZ_NONE && size1 != size2)
        return setError(ILLEGAL_SIZE);

    _scan = p;
    insn.emitInsn();
    insn.emitByte((_regs.encodeDataReg(reg1) << 4)
                  | _regs.encodeDataReg(reg2));
    return checkLineEnd();
}

Error AsmMc6809::encodeRelative(Insn &insn) {
    target::uintptr_t addr;
    if (getOperand16(addr)) return getError();
    if (getError() == UNDEFINED_SYMBOL) addr = insn.address();
    const host::uint_t insnLen = (insn.hasPrefix() ? 2 : 1)
        + (insn.oprSize() == SZ_BYTE ? 1 : 2);
    const target::uintptr_t base = insn.address() + insnLen;
    const target::ptrdiff_t delta = addr - base;
    insn.emitInsn();
    if (insn.oprSize() == SZ_BYTE) {
        if (delta >= 128 || delta < -128) return setError(OPERAND_TOO_FAR);
        insn.emitByte(uint8_t(delta));
    } else {
        insn.emitUint16(delta);
    }
    return checkLineEnd();
}

Error AsmMc6809::encodeImmediate(Insn &insn) {
    if (*_scan != '#') return setError(UNKNOWN_OPERAND);
    _scan++;
    insn.emitInsn();
    if (insn.oprSize() == SZ_BYTE) {
        uint8_t val8;
        if (getOperand8(val8)) return getError();
        insn.emitByte(val8);
    } else if (insn.oprSize() == SZ_WORD) {
        uint16_t val16;
        if (getOperand16(val16)) return getError();
        insn.emitUint16(val16);
    } else if (TableMc6809.is6309() && insn.oprSize() == SZ_LONG) {
        uint32_t val32;
        if (getOperand32(val32)) return getError();
        insn.emitUint32(val32);
    } else {
        return setError(UNKNOWN_OPERAND);
    }
    return checkLineEnd();
}

Error AsmMc6809::encodeDirect(Insn &insn, bool emitInsn) {
    if (*_scan == '<') _scan++;
    if (emitInsn) insn.emitInsn();
    target::uintptr_t dir;
    if (getOperand16(dir)) return getError();
    // TODO: Warning if dir isn't on _direct_page.
    insn.emitByte(static_cast<uint8_t>(dir));
    return checkLineEnd();
}

Error AsmMc6809::encodeExtended(Insn &insn, bool emitInsn) {
    if (*_scan == '>') _scan++;
    if (emitInsn) insn.emitInsn();
    target::uintptr_t addr;
    if (getOperand16(addr)) return getError();
    insn.emitUint16(addr);
    return checkLineEnd();
}

Error AsmMc6809::encodeIndexed(Insn &insn, bool emitInsn) {
    if (emitInsn) insn.emitInsn();
    const char *p = _scan;
    const bool indir = (*p == '[');
    RegName base = REG_UNDEF;
    RegName index = REG_UNDEF;
    host::int_t osize = 0;      // offset size, -1 undefined
    host::int_t incr = 0;       // auto decrement/increment
    target::uintptr_t addr;
    bool undef = false;
    if (indir) p++;
    if (*p != ',') {
        if ((index = _regs.parseIndexReg(p)) != REG_UNDEF) {
            p += _regs.regNameLen(index); // index register
        } else {
            if (*p == '>') {
                p++;
                osize = 16;
            } else if (*p == '<') {
                if (*++p == '<') {
                    p++;
                    osize = 5;
                } else {
                    osize = 8;
                }
            } else {
                osize = -1;
            }
            _scan = p;
            if (getOperand16(addr)) return getError();
            if (getError() == UNDEFINED_SYMBOL)
                undef = true;
            p = _scan;
            index = OFFSET;     // offset is in addr
        }
    }
    if (*p == ',') {
        p++;
        if (index == REG_UNDEF) {
            while (*p == '-') {
                p++;
                incr--;
            }
        }
        if ((base = _regs.parseBaseReg(p)) == REG_UNDEF) {
            if (_regs.compareRegName(p, REG_PCR)) {
                base = REG_PCR;
            } else if (_regs.compareRegName(p, REG_PC)) {
                base = REG_PC;
            } else setError(UNKNOWN_OPERAND);
        }
        p += _regs.regNameLen(base);
        if (index == REG_UNDEF && incr == 0) {
            while (*p == '+') {
                p++;
                incr++;
            }
        }
    }
    if (indir) {
        if (*p != ']') return setError(UNKNOWN_OPERAND);
        p++;
    }
    _scan = p;

    uint8_t post;
    if (base == REG_UNDEF) {    // [n16]
        if (index != OFFSET) return setError(UNKNOWN_OPERAND);
        insn.emitByte(0x9F);
        insn.emitUint16(addr);
    } else if (base == REG_PCR || base == REG_PC) { // n,PCR [n,PCR] n,PC [n,PC]
        if (index != OFFSET || osize == 0 || osize == 5 || incr != 0)
            return setError(UNKNOWN_OPERAND);
        post = indir ? 0x10 : 0;
        if (undef && base == REG_PCR) {
            base = REG_PC;
            osize = 16;
        }
        target::ptrdiff_t offset = (base == REG_PC) ? addr
            : addr - (insn.address() + insn.length() + 2);
        if (osize == -1) {
            if (offset >= -128 && offset < 128) {
                osize = 8;
            } else {
                if (base == REG_PCR) offset--;
                osize = 16;
            }
        }
        if (osize == 8) {
            insn.emitByte(0x8C | post);
            insn.emitByte(static_cast<uint8_t>(offset));
            return setError(
                (offset >= -128 && offset < 128) ? OK : OVERFLOW_RANGE);
        }
        insn.emitByte(0x8D | post);
        insn.emitUint16(offset);
    } else if (TableMc6809.is6309() && base == REG_W) {
        uint8_t post;
        if (index == OFFSET) {
            if (osize == -1 && addr == 0) {
                post = 0x8F;    // 0,W [0,W]
                index = REG_UNDEF;
            } else if (osize == -1 || osize == 16) {
                post = 0xAF;    // n16,W [n16,W]
            } else {
                return setError(UNKNOWN_OPERAND);
            }
        } else if (incr == 0) {
            post = 0x8F;        // ,W [,W]
        } else if (incr == 2) {
            post = 0xCF;        // ,W++ [,W++]
        } else if (incr == -2) {
            post = 0xEF;        // ,--W [,--W]
        } else {
            return setError(UNKNOWN_OPERAND);
        }
        if (indir) post++;
        insn.emitByte(post);
        if (index == OFFSET) insn.emitUint16(addr);
    } else {
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
        } else if (index != OFFSET) {      // R,R
            post |= _regs.encodeIndexReg(index);
            insn.emitByte(0x80 | post);
        } else {
            const target::ptrdiff_t offset = addr;
            if (indir && osize == 5) return setError(UNKNOWN_OPERAND);
            if (osize == -1) {
                if (offset == 0) {
                    osize = 0;
                } else if (offset >= -16 && offset < 16 && !indir) { // n5,R
                    osize = 5;
                } else if (offset >= -128 && offset < 128) { // n8,R [n8,R]
                    osize = 8;
                } else {                // n16,R [n16,R]
                    osize = 16;
                }
            }
            if (osize == 0) {
                post |= 0x84;
                insn.emitByte(post);
            } else if (osize == 5) {
                post |= (offset & 0x1F);
                insn.emitByte(post);
                if (offset < -16 || offset >= 16) return setError(OVERFLOW_RANGE);
            } else if (osize == 8) {
                post |= 0x88;
                insn.emitByte(post);
                insn.emitByte(static_cast<uint8_t>(offset));
                if (offset < -128 || offset >= 128) return setError(OVERFLOW_RANGE);
            } else {
                post |= 0x89;
                insn.emitByte(post);
                insn.emitUint16(offset);
            }
        }
    }
    return checkLineEnd();
}

Error AsmMc6809::encodeBitOperation(Insn &insn) {
    const RegName regName = _regs.parseBitOpReg(_scan);
    if (regName == REG_UNDEF) return setError(UNKNOWN_REGISTER);
    _scan += _regs.regNameLen(regName);
    uint8_t post = _regs.encodeBitOpReg(regName) << 6;
    if (*_scan != '.' && *_scan != ',') return setError(UNKNOWN_OPERAND);
    _scan++;
    uint8_t pos;
    if (getOperand8(pos)) return getError();
    Error error = getError();
    if (pos >= 8) return setError(ILLEGAL_BIT_NUMBER);
    post |= pos;
    if (*_scan != ',') return setError(UNKNOWN_OPERAND);
    _scan++;
    bool forcibly = (*_scan == '<');
    if (forcibly) _scan++;
    target::uintptr_t addr;
    if (getOperand16(addr)) return getError();
    if (getError()) error = getError();
    if (!forcibly && (addr >> 8) != _direct_page)
        return setError(OPERAND_TOO_FAR);
    if (*_scan != '.' && *_scan != ',') return setError(UNKNOWN_OPERAND);
    _scan++;
    if (getOperand8(pos)) return getError();
    if (getError()) error = getError();
    if (pos >= 8) return setError(ILLEGAL_BIT_NUMBER);
    post |= (pos << 3);

    insn.emitInsn();
    insn.emitByte(post);
    insn.emitByte(static_cast<uint8_t>(addr));
    setError(error);
    return checkLineEnd();
}

Error AsmMc6809::encodeImmediatePlus(Insn &insn) {
    if (*_scan != '#') return setError(UNKNOWN_OPERAND);
    _scan++;
    uint8_t val8;
    if (getOperand8(val8)) return getError();
    const Error error1 = getError();
    if (*_scan != ',') return setError(UNKNOWN_OPERAND);
    _scan++;

    if (determineAddrMode(_scan, insn)) return getError();
    switch (insn.addrMode()) {
    case DIRP: insn.setAddrMode(IMMDIR); break;
    case EXTD: insn.setAddrMode(IMMEXT); break;
    case INDX: insn.setAddrMode(IMMIDX); break;
    default: return setError(UNKNOWN_OPERAND);
    }
    if (TableMc6809.searchNameAndAddrMode(insn))
        return setError(UNKNOWN_INSTRUCTION);
    insn.emitInsn();
    insn.emitByte(val8);
    Error error2;
    switch (insn.addrMode()) {
    case IMMDIR: error2 = encodeDirect(insn, /* emitInsn */ false); break;
    case IMMEXT: error2 = encodeExtended(insn, /* emitInsn */ false); break;
    case IMMIDX: error2 = encodeIndexed(insn, /* emitInsn */ false); break;
    default:     return setError(UNKNOWN_OPERAND);
    }
    return setError(error1 ? error1 : (error2 ? error2 : OK));
}

Error AsmMc6809::encodeTransferMemory(Insn &insn) {
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
    post |= _regs.encodeTfmBaseReg(regName);

    uint8_t mode = 0;
    while (mode < 4) {
        if (srcMode == _regs.tfmSrcModeChar(mode)
            && dstMode == _regs.tfmDstModeChar(mode)) {
            break;
        }
        mode++;
    }
    if (mode == 4) return setError(UNKNOWN_OPERAND);

    insn.setInsnCode(insn.prefixCode(), 0x38 + mode);
    insn.emitInsn();
    insn.emitByte(post);
    return checkLineEnd();
}

Error AsmMc6809::determineAddrMode(const char *line, Insn &insn) {
    insn.setAddrMode(INDX);
    if (*line == '#') {
        insn.setAddrMode(IMM);
        return OK;
    }
    if (*line == '[' || *line == ',')
        return OK;
    if (_regs.parseIndexReg(line) != REG_UNDEF)
        return OK;
    host::int_t size = -1;
    if (*line == '<') {
        size = 8;
        if (line[1] == '<')     // << for 5bit indexed
            line++;
        line++;
    } else if (*line == '>') {
        size = 16;
        line++;
    }
    const char *saved_scan = _scan;
    _scan = line;
    uint16_t val;
    const Error error = getOperand16(val);
    line = _scan;
    _scan = saved_scan;
    if (error != OK) return error;
    if (*line == ',')
        return OK;
    if (size == 8) {
        insn.setAddrMode(DIRP);
    } else if (size == 16) {
        insn.setAddrMode(EXTD);
    } else if ((val >> 8) == _direct_page) {
        insn.setAddrMode(DIRP);
    } else {
        insn.setAddrMode(EXTD);
    }
    return OK;
}

Error AsmMc6809::processPseudo(Insn &insn) {
    insn.resetAddress(insn.address()); // make generated bytes zero.
    if (insn.insnCode() == PSEUDO_SETDP)
        return setError(getOperand8(_direct_page));
    if (insn.insnCode() == PSEUDO_ASSUME) {
        const char *p = _parser.scanSymbol(_scan);
        if (p - _scan != 3 && strncasecmp(_scan, "dpr", 3))
            return setError(UNKNOWN_REGISTER);
        p = skipSpaces(p);
        if (*p != ':') return setError(UNKNOWN_OPERAND);
        _scan = p + 1;
        return setError(getOperand8(_direct_page));
    }
    return setError(UNKNOWN_INSTRUCTION);
}

Error AsmMc6809::encode(Insn &insn) {
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);
    if (TableMc6809.searchName(insn))
        return setError(UNKNOWN_INSTRUCTION);
    if (insn.is6309() && !TableMc6809.is6309())
        return setError(UNKNOWN_INSTRUCTION);
    _scan = skipSpaces(endName);

    switch (insn.addrMode()) {
    case INHR: insn.emitInsn(); return checkLineEnd();
    case REL:   return encodeRelative(insn);
    case STKOP:
        if (*_scan == '#') return encodeImmediate(insn);
        return encodeStackOp(insn);
    case REGS:  return encodeRegisters(insn);
    case PSEUDO: return processPseudo(insn);
    default:
        if (TableMc6809.is6309()) {
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
    if (TableMc6809.searchNameAndAddrMode(insn))
        return setError(UNKNOWN_INSTRUCTION);
    switch (insn.addrMode()) {
    case IMM:  return encodeImmediate(insn);
    case DIRP: return encodeDirect(insn);
    case EXTD: return encodeExtended(insn);
    case INDX: return encodeIndexed(insn);
    default:   return setError(UNKNOWN_OPERAND);
    }
}

} // namespace mc6809
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
