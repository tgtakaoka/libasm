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

Error AsmMc6809::encodePushPull(InsnMc6809 &insn) {
    if (*_scan == '#') return encodeImmediate(insn);
    uint8_t post = 0;
    const char *p = _scan;
    const bool onUserStack = (insn.opCode() & 2) != 0;
    while (!endOfLine(p)) {
        const RegName regName = _regs.parseRegName(p);
        if (regName == REG_UNDEF) {
            setError(UNKNOWN_REGISTER);
            break;
        }
        const int8_t bits = _regs.encodeStackReg(regName, onUserStack);
        if (bits == 0) {
            setError(ILLEGAL_REGISTER);
            break;
        }
        if (post & bits) {
            post |= bits;
            setError(DUPLICATE_REGISTER);
            break;
        }
        post |= bits;
        p = skipSpaces(p + _regs.regNameLen(regName));
        if (*p == ',') {
            p = skipSpaces(p + 1);
            continue;
        }
        break;
    }
    insn.emitInsn();
    insn.emitByte(post);
    _scan = p;
    return checkLineEnd();
}

Error AsmMc6809::encodeRegisters(InsnMc6809 &insn) {
    const char *p = _scan;
    const RegName reg1 = _regs.parseDataReg(p);
    if (reg1 == REG_UNDEF)
        return setError(UNKNOWN_REGISTER);
    p = skipSpaces(p + _regs.regNameLen(reg1));
    if (*p != ',') return setError(UNKNOWN_OPERAND);
    p = skipSpaces(p + 1);
    _scan = p;
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

Error AsmMc6809::encodeRelative(InsnMc6809 &insn) {
    Config::uintptr_t addr;
    if (getOperand(addr)) return getError();
    if (getError() == UNDEFINED_SYMBOL) addr = insn.address();
    insn.emitInsn();
    const Config::uintptr_t base = insn.address() + insn.length()
        + (insn.oprSize() == SZ_BYTE ? 1 : 2);
    const Config::ptrdiff_t delta = addr - base;
    if (insn.oprSize() == SZ_BYTE) {
        if (delta >= 128 || delta < -128) return setError(OPERAND_TOO_FAR);
        insn.emitByte(static_cast<uint8_t>(delta));
    } else {
        insn.emitUint16(delta);
    }
    return checkLineEnd();
}

Error AsmMc6809::encodeImmediate(InsnMc6809 &insn) {
    if (*_scan != '#') return setError(UNKNOWN_OPERAND);
    _scan++;
    insn.emitInsn();
    if (insn.oprSize() == SZ_BYTE) {
        uint8_t val8;
        if (getOperand(val8)) return getError();
        insn.emitByte(val8);
    } else if (insn.oprSize() == SZ_WORD) {
        uint16_t val16;
        if (getOperand(val16)) return getError();
        insn.emitUint16(val16);
    } else if (insn.oprSize() == SZ_LONG) {
        uint32_t val32;
        if (getOperand(val32)) return getError();
        insn.emitUint32(val32);
    } else {
        return setError(UNKNOWN_OPERAND);
    }
    return checkLineEnd();
}

Error AsmMc6809::encodeDirect(InsnMc6809 &insn, bool emitInsn) {
    if (*_scan == '<') _scan++;
    if (emitInsn) insn.emitInsn();
    Config::uintptr_t dir;
    if (getOperand(dir)) return getError();
    // TODO: Warning if dir isn't on _direct_page.
    insn.emitByte(static_cast<uint8_t>(dir));
    return checkLineEnd();
}

Error AsmMc6809::encodeExtended(InsnMc6809 &insn, bool emitInsn) {
    if (*_scan == '>') _scan++;
    if (emitInsn) insn.emitInsn();
    Config::uintptr_t addr;
    if (getOperand(addr)) return getError();
    insn.emitUint16(addr);
    return checkLineEnd();
}

Error AsmMc6809::encodeIndexed(InsnMc6809 &insn, bool emitInsn) {
    RegName base = REG_UNDEF;
    RegName index = REG_UNDEF;
    int8_t osize = 0;      // offset size, -1 undefined
    int8_t incr = 0;       // auto decrement/increment
    Config::uintptr_t addr;
    bool undef = false;

    const char *p = _scan;
    const bool indir = (*p == '[');
    if (indir) p = skipSpaces(p + 1);
    if (*p != ',') {
        if ((index = _regs.parseIndexReg(p)) != REG_UNDEF) {
            p = skipSpaces(p + _regs.regNameLen(index)); // index register
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
            _scan = skipSpaces(p);
            if (getOperand(addr)) return getError();
            if (getError() == UNDEFINED_SYMBOL)
                undef = true;
            p = skipSpaces(_scan);
            index = OFFSET;     // offset is in addr
        }
    }
    if (*p == ',') {
        p = skipSpaces(p + 1);
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
            } else {
                setError(UNKNOWN_OPERAND);
            }
        }
        p += _regs.regNameLen(base);
        if (index == REG_UNDEF && incr == 0) {
            while (*p == '+') {
                p++;
                incr++;
            }
        }
        p = skipSpaces(p);
    }
    if (indir) {
        if (*p != ']') return setError(UNKNOWN_OPERAND);
        p++;
    }
    _scan = p;

    uint8_t post;
    if (base == REG_UNDEF) {    // [n16]
        if (index != OFFSET) return setError(UNKNOWN_OPERAND);
        if (emitInsn) insn.emitInsn();
        insn.emitByte(0x9F);
        insn.emitUint16(addr);
    } else if (base == REG_PCR || base == REG_PC) { // n,PCR [n,PCR] n,PC [n,PC]
        if (index != OFFSET || osize == 0 || osize == 5 || incr != 0)
            return setError(UNKNOWN_OPERAND);
        if (emitInsn) insn.emitInsn();
        post = indir ? 0x10 : 0;
        if (undef && base == REG_PCR) {
            base = REG_PC;
            osize = 16;
        }
        Config::ptrdiff_t offset = (base == REG_PC) ? addr
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
    } else if (base == REG_W) {
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
        if (emitInsn) insn.emitInsn();
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
            if (emitInsn) insn.emitInsn();
            insn.emitByte(post);
        } else if (index != OFFSET) {      // R,R
            post |= _regs.encodeIndexReg(index);
            if (emitInsn) insn.emitInsn();
            insn.emitByte(0x80 | post);
        } else {
            const Config::ptrdiff_t offset = addr;
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
            if (emitInsn) insn.emitInsn();
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

Error AsmMc6809::encodeBitOperation(InsnMc6809 &insn) {
    const char *p = _scan;
    const RegName regName = _regs.parseBitOpReg(p);
    if (regName == REG_UNDEF) return setError(UNKNOWN_REGISTER);
    uint8_t post = _regs.encodeBitOpReg(regName) << 6;
    p = skipSpaces(p + _regs.regNameLen(regName));
    if (*p != '.' && *p != ',') return setError(UNKNOWN_OPERAND);
    _scan = p + 1;
    uint8_t pos;
    if (getOperand(pos)) return getError();
    if (pos >= 8) return setError(ILLEGAL_BIT_NUMBER);
    const Error error1 = getError();
    post |= pos;
    p = skipSpaces(_scan);
    if (*p != ',') return setError(UNKNOWN_OPERAND);
    p = skipSpaces(p + 1);
    const bool forcibly = (*p == '<');
    if (forcibly) p++;
    Config::uintptr_t addr;
    _scan = p;
    if (getOperand(addr)) return getError();
    const Error error2 = getError();
    if (!forcibly && (addr >> 8) != _direct_page)
        return setError(OPERAND_TOO_FAR);
    p = skipSpaces(_scan);
    if (*p != '.' && *p != ',') return setError(UNKNOWN_OPERAND);
    _scan = p + 1;
    if (getOperand(pos)) return getError();
    if (pos >= 8) return setError(ILLEGAL_BIT_NUMBER);
    post |= (pos << 3);

    insn.emitInsn();
    insn.emitByte(post);
    insn.emitByte(static_cast<uint8_t>(addr));
    setErrorIf(error1);
    setErrorIf(error2);
    return checkLineEnd();
}

Error AsmMc6809::encodeImmediatePlus(InsnMc6809 &insn) {
    const char *p = _scan;
    if (*p != '#') return setError(UNKNOWN_OPERAND);
    _scan = p + 1;
    uint8_t val8;
    if (getOperand(val8)) return getError();
    const Error error1 = getError();
    p = skipSpaces(_scan);
    if (*p != ',') return setError(UNKNOWN_OPERAND);
    _scan = skipSpaces(p + 1);

    if (determineAddrMode(_scan, insn)) return getError();
    AddrMode mode;
    switch (insn.addrMode()) {
    case DIR: mode = IMM_DIR; break;
    case EXT: mode = IMM_EXT; break;
    case IDX: mode = IMM_IDX; break;
    default: return setError(UNKNOWN_OPERAND);
    }
    insn.setAddrMode(mode);
    if (TableMc6809.searchNameAndAddrMode(insn))
        return setError(UNKNOWN_INSTRUCTION);
    insn.emitInsn();
    insn.emitByte(val8);
    switch (mode) {
    case IMM_DIR: encodeDirect(insn,   /* emitInsn */ false); break;
    case IMM_EXT: encodeExtended(insn, /* emitInsn */ false); break;
    case IMM_IDX: encodeIndexed(insn,  /* emitInsn */ false); break;
    default:      return setError(UNKNOWN_OPERAND);
    }
    return setErrorIf(error1); // restore error of 1st operand
}

Error AsmMc6809::encodeTransferMemory(InsnMc6809 &insn) {
    const char *p = _scan;
    RegName regName = _regs.parseTfmBaseReg(p);
    if (regName == REG_UNDEF) return setError(UNKNOWN_REGISTER);
    p += _regs.regNameLen(regName);
    const char srcMode = (*p == '+' || *p == '-') ? *p++ : 0;
    p = skipSpaces(p);
    if (*p != ',') return setError(UNKNOWN_OPERAND);
    p = skipSpaces(p + 1);
    Config::opcode_t post = _regs.encodeTfmBaseReg(regName) << 4;

    regName = _regs.parseTfmBaseReg(p);
    if (regName == REG_UNDEF) return setError(UNKNOWN_REGISTER);
    p += _regs.regNameLen(regName);
    const char dstMode = (*p == '+' || *p == '-') ? *p++ : 0;
    _scan = skipSpaces(p);
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

    insn.setOpCode(0x38 + mode, insn.prefixCode());
    insn.emitInsn();
    insn.emitByte(post);
    return checkLineEnd();
}

Error AsmMc6809::determineAddrMode(const char *line, InsnMc6809 &insn) {
    insn.setAddrMode(IDX);
    if (*line == '#') {
        insn.setAddrMode(IMM);
        return OK;
    }
    if (*line == '[' || *line == ',')
        return OK;
    if (_regs.parseIndexReg(line) != REG_UNDEF)
        return OK;
    int8_t size = -1;
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
    const Error error = getOperand(val);
    line = skipSpaces(_scan);
    _scan = saved_scan;
    if (error != OK) return error;
    if (*line == ',')
        return OK;
    if (size == 8) {
        insn.setAddrMode(DIR);
    } else if (size == 16) {
        insn.setAddrMode(EXT);
    } else if ((val >> 8) == _direct_page) {
        insn.setAddrMode(DIR);
    } else {
        insn.setAddrMode(EXT);
    }
    return OK;
}

Error AsmMc6809::processPseudo(InsnMc6809 &insn, const char *line) {
    insn.resetAddress(insn.address()); // make generated bytes zero.
    if (strcasecmp(insn.name(), "SETDP") == 0) {
        _scan = line;
        setError(getOperand(_direct_page));
        return OK;
    }
    if (strcasecmp(insn.name(), "ASSUME") == 0) {
        const char *p = _parser.scanSymbol(line);
        if (p - line == 3 && strncasecmp(line, "DPR", 3) == 0) {
            p = skipSpaces(p);
            if (*p == ':') {
                _scan = p + 1;
                setError(getOperand(_direct_page));
            } else setError(UNKNOWN_OPERAND);
        } else setError(UNKNOWN_REGISTER);
        return OK;
    }
    return UNKNOWN_INSTRUCTION;
}

Error AsmMc6809::encode(Insn &_insn) {
    InsnMc6809 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);
    if (processPseudo(insn, skipSpaces(endName)) == OK)
        return getError();
    if (TableMc6809.searchName(insn))
        return setError(UNKNOWN_INSTRUCTION);
    _scan = skipSpaces(endName);

    switch (insn.addrMode()) {
    case INH: insn.emitInsn(); return checkLineEnd();
    case REL: return encodeRelative(insn);
    case PSH_PUL: return encodePushPull(insn);
    case REG_REG: return encodeRegisters(insn);
        // HD6309
    case IMM_DIR:
    case IMM_EXT:
    case IMM_IDX: return encodeImmediatePlus(insn);
    case BITOP:   return encodeBitOperation(insn);
    case TFR_MEM: return encodeTransferMemory(insn);
    default: break;
    }

    if (determineAddrMode(_scan, insn)) return getError();
    if (TableMc6809.searchNameAndAddrMode(insn))
        return setError(UNKNOWN_INSTRUCTION);
    switch (insn.addrMode()) {
    case IMM: return encodeImmediate(insn);
    case DIR: return encodeDirect(insn);
    case EXT: return encodeExtended(insn);
    case IDX: return encodeIndexed(insn);
    default:  return setError(UNKNOWN_OPERAND);
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
