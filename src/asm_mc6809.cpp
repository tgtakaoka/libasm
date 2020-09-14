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

Error AsmMc6809::encodeRelative(InsnMc6809 &insn, const Operand &op) {
    const Config::uintptr_t base = insn.address()
        + (insn.hasPrefix() ? 2 : 1)
        + (insn.addrMode() == LREL ? 2 : 1);
    const Config::uintptr_t target = op.getError() ? base
        : static_cast<Config::uintptr_t>(op.val32);
    const Config::ptrdiff_t delta = target - base;
    if (insn.addrMode() == REL) {
        if (delta >= 128 || delta < -128) return setError(OPERAND_TOO_FAR);
        insn.emitInsn();
        insn.emitByte(static_cast<uint8_t>(delta));
    } else {
        insn.emitInsn();
        insn.emitUint16(delta);
    }
    return OK;
}

Error AsmMc6809::encodeImmediate(
        InsnMc6809 &insn, const Operand &op, Operand &extra) {
    insn.emitInsn();
    switch (insn.addrMode()) {
    case IM8:
        insn.emitByte(static_cast<uint8_t>(op.val32));
        break;
    case IM16:
        insn.emitUint16(static_cast<uint16_t>(op.val32));
        break;
    case IM32:
        insn.emitUint32(op.val32);
        break;
    default:
        break;
    }
    switch (insn.extraMode()) {
    case DIR:
        insn.emitByte(static_cast<uint8_t>(extra.val32));
        break;
    case EXT:
        insn.emitUint16(static_cast<uint16_t>(extra.val32));
        break;
    case IDX:
        return encodeIndexed(insn, extra, false);
    default:
        break;
    }
    return OK;
}

Config::ptrdiff_t AsmMc6809::calculateDisplacement(
    const InsnMc6809 &insn, const Operand &op) const {
    Config::ptrdiff_t disp = static_cast<Config::ptrdiff_t>(op.val32);
    if (op.base == REG_PCR) {
        const Config::uintptr_t base = insn.address()
            + (insn.hasPrefix() ? 3 : 2) + 1; // assuming 8-bit displacement
        const Config::uintptr_t target = op.getError()
            ? insn.address() : static_cast<Config::uintptr_t>(disp);
        disp = target - base;
    }
    return disp;
}

Error AsmMc6809::encodeIndexed(
    InsnMc6809 &insn, Operand &op, bool emitInsn) {
    PostSpec spec;
    if (op.mode == REG_REG) {
        if (!_regs.isIndexedBase(op.base)) return setError(UNKNOWN_OPERAND);
        if (op.index == REG_0) {
            op.index = REG_UNDEF;
            op.sub = DISP_IDX;
        } else if (_regs.isIndexReg(op.index)) {
            op.sub = ACCM_IDX;
        } else {
            return setError(REGISTER_NOT_ALLOWED);
        }
    }
    spec.mode = op.sub;
    spec.index = op.index;
    spec.base =  op.base;
    spec.size = (op.base == REG_PCR) ? 0 : op.extra;
    spec.indir = op.indir;
    if (op.sub == ABS_IDIR) spec.size = 16;
    Config::ptrdiff_t disp = calculateDisplacement(insn, op);
    if (spec.mode == DISP_IDX && spec.size == 0) {
        const bool pc = (spec.base == REG_PC || spec.base == REG_PCR);
        if (spec.base == REG_PCR && op.getError()) {
            disp = 0;
            spec.size = 16;
        } else if (!pc && disp == 0) {
            spec.mode = PNTR_IDX;
            spec.size = 0;
        } else if (!pc && !spec.indir && disp >= -16 && disp < 16) {
            spec.size = 5;
        } else if (disp >= -128 && disp < 128) {
            spec.size = 8;
        } else {
            if (spec.base == REG_PCR) disp--; // adjust to 16-bit displacement
            spec.size = 16;
        }
    }
    uint8_t post;
    if (TableMc6809.searchPostSpec(spec, post))
        return setError(UNKNOWN_OPERAND);
    if (spec.base == REG_X)
        post |= (_regs.encodeBaseReg(op.base) << 5);
    if (spec.mode == DISP_IDX && spec.size == 5)
        post |= static_cast<uint8_t>(disp & 0x1F);
    if (emitInsn) insn.emitInsn();
    insn.emitByte(post);
    if (spec.size == 8) insn.emitByte(static_cast<uint8_t>(disp));
    if (spec.size == 16) insn.emitUint16(disp);
    return OK;
}

Error AsmMc6809::encodeRegisters(InsnMc6809 &insn, const Operand &op) {
    const RegName reg1 = op.mode == REG_BIT ? op.base : op.index;
    const RegName reg2 = op.mode == REG_BIT ? REG_0 : op.base;
    const int8_t num1 = _regs.encodeDataReg(reg1);
    const int8_t num2 = _regs.encodeDataReg(reg2);
    if (num1 < 0 || num2 < 0) return setError(UNKNOWN_REGISTER);
    const RegSize size1 = _regs.regSize(reg1);
    const RegSize size2 = _regs.regSize(reg2);
    if (size1 != SZ_NONE && size2 != SZ_NONE && size1 != size2)
        return setError(ILLEGAL_SIZE);
    insn.emitInsn();
    insn.emitByte((num1 << 4) | num2);
    return setOK();
}

Error AsmMc6809::encodePushPull(InsnMc6809 &insn, const Operand &op) {
    uint8_t post = 0;
    RegName stack = REG_UNDEF;
    if (op.mode == NONE) {
        post = 0;
    } else if (op.mode == IM32) {
        post = static_cast<uint8_t>(op.val32);
    } else if (op.mode == REGLIST) {
        if (op.getError()) return setError(op);
        if (static_cast<int32_t>(op.val32) < 0) return setError(REGISTER_NOT_ALLOWED);
        post = op.val32;
        stack = op.base;
    } else if (op.mode == REG_REG || (op.mode == IDX && op.sub == ACCM_IDX)) {
        RegName reg = op.index;
        if (reg == REG_S || reg == REG_U) {
            stack = reg;
            if (reg == REG_S) reg = REG_U;
        }
        uint8_t bit = _regs.encodeStackReg(reg, false);
        if (bit == 0) return setError(REGISTER_NOT_ALLOWED);
        post = bit;
        reg = op.base;
        if (reg == REG_S || reg == REG_U) {
            if (stack == REG_UNDEF) stack = reg;
            if (stack != reg) return setError(REGISTER_NOT_ALLOWED);
            if (reg == REG_S) reg = REG_U;
        }
        bit = _regs.encodeStackReg(reg, false);
        if (bit == 0) return setError(REGISTER_NOT_ALLOWED);
        if (post & bit) return setError(DUPLICATE_REGISTER);
        post |= bit;
    } else {
        return setError(OPERAND_NOT_ALLOWED);
    }
    const bool onUserStack = (insn.opCode() & 2) != 0;
    if (onUserStack && stack == REG_U) return setError(REGISTER_NOT_ALLOWED);
    if (!onUserStack && stack == REG_S) return setError(REGISTER_NOT_ALLOWED);
    insn.emitInsn();
    insn.emitByte(post);
    return OK;
}

Error AsmMc6809::encodeBitOperation(
    InsnMc6809 &insn, const Operand &op, const Operand &extra) {
    const uint8_t opbit = op.extra;
    const uint8_t exbit = extra.extra;
    const uint8_t post = (_regs.encodeBitOpReg(op.base) << 6)
        | (exbit << 3) | opbit;
    insn.emitInsn();
    insn.emitByte(post);
    insn.emitByte(static_cast<uint8_t>(extra.val32));
    return OK;
}

Error AsmMc6809::encodeTransferMemory(
        InsnMc6809 &insn, const Operand &op, const Operand &extra) {
    uint8_t mode = 0;
    if (op.mode == REG_TFM && extra.mode == REG_TFM) {
        if (op.extra != extra.extra) return setError(UNKNOWN_OPERAND);
        mode = (op.extra == '+') ? 0 : 1;
    } else if (op.mode == REG_TFM && extra.mode == REGLIST) {
        if (op.extra != '+') return setError(UNKNOWN_OPERAND);
        if (extra.extra != 1) return setError(UNKNOWN_OPERAND);
        if (!_regs.isTfmBaseReg(extra.index)) return setError(OPERAND_NOT_ALLOWED);
        mode = 2;
    } else if (op.mode == REGLIST && extra.mode == REG_TFM) {
        if (op.extra != 1) return setError(UNKNOWN_OPERAND);
        if (!_regs.isTfmBaseReg(op.index)) return setError(OPERAND_NOT_ALLOWED);
        if (extra.extra != '+') return setError(UNKNOWN_OPERAND);
        mode = 3;
    } else return setError(UNKNOWN_OPERAND);

    insn.embed(mode);
    const uint8_t post = (_regs.encodeTfmBaseReg(op.index) << 4)
        | _regs.encodeTfmBaseReg(extra.index);
    insn.emitInsn();
    insn.emitByte(post);
    return OK;
}

bool AsmMc6809::parsePointerMode(Operand &op, const char *p, bool indir) {
    if (*p++ != ',') return false;
    int8_t autoIndex = 0;
    if (*p == '-') {
        p++; autoIndex = -1;
        if (*p == '-') {
            p++; autoIndex = -2;
        }
    }
    const RegName reg = _regs.parseRegName(p);
    if (!_regs.isBaseReg(reg)) return false;
    p += _regs.regNameLen(reg);
    if (autoIndex == 0 && *p == '+') {
        p++; autoIndex = 1;
        if (*p == '+') {
            p++; autoIndex = 2;
        }
    }
    p = skipSpaces(p);
    if (indir) {
        if (*p == ']') {
            p++;
        } else {
            setError(MISSING_CLOSING_PAREN);
        }
    }
    _scan = p;
    op.base = reg;
    op.mode = IDX;
    op.sub = autoIndex ? AUTO_IDX : PNTR_IDX;
    op.extra = autoIndex;
    op.indir = indir;
    return true;
}

bool AsmMc6809::parseIndexedMode(Operand &op, const char *p, bool indir) {
    p = skipSpaces(p);
    const RegName base = _regs.parseRegName(p);
    if (!_regs.isIndexedBase(base)) return false;
    p = skipSpaces(p + _regs.regNameLen(base));
    if (indir) {
        if (*p == ']') {
            p++;
        } else {
            setError(MISSING_CLOSING_PAREN);
        }
    }
    _scan = p;
    op.base = base;
    op.mode = IDX;
    op.sub = DISP_IDX;
    op.indir = indir;
    return true;
}

bool AsmMc6809::parseBitPosition(Operand &op, const char *p) {
    const char *a = skipSpaces(p);
    if (*p == '.') {
        p++;
        // bit position separator '.' can't have space around it.
        if (*p == ' ' || *p == '\t') return false;
    } else if (*a == ',') {
        // bit position separator ',' can have spaces around it.
        p = skipSpaces(a + 1);
    } else return false;
    const RegName reg = _regs.parseRegName(p);
    if (reg != REG_0 && reg != REG_UNDEF) return false;
    const char *saved = _scan;
    _scan = p;
    uint32_t bitp;
    if (getOperand(bitp) || bitp >= 8) {
        _scan = saved;
        setOK();
        return false;
    }
    op.setErrorIf(getError());
    op.extra = bitp;
    return true;
}

bool AsmMc6809::parseRegisterList(Operand &op, const char *p, bool indir) {
    RegName stack = REG_UNDEF;
    bool push_pull = true;
    uint8_t n = 0;
    uint8_t post = 0;
    const char *lastComma = p;
    while (true) {
        RegName reg = _regs.parseRegName(p);
        if (reg == REG_UNDEF) return false;
        p += _regs.regNameLen(reg);
        if (*p == '+' || *p == '-') {
            if (!_regs.isTfmBaseReg(reg)) {
                op.setError(REGISTER_NOT_ALLOWED);
                _scan = p + 1;
                return true;
            }
            if (n == 0) {
                _scan = p + 1;
                op.index = reg;
                op.mode = REG_TFM;
                op.extra = *p;
                return true;
            }
            p = lastComma;
            break;
        }
        if (n == 0) op.index = reg;
        if (n == 1) op.base  = reg;
        n++;
        if (reg == REG_S || reg == REG_U ) {
            if (stack == REG_UNDEF) stack = reg;
            if (stack != reg) push_pull = false;
            if (reg == REG_S) reg = REG_U;
        }
        const uint8_t bit = _regs.encodeStackReg(reg, false);
        if (bit == 0) push_pull = false;
        if (post & bit) op.setError(DUPLICATE_REGISTER);
        post |= bit;
        p = skipSpaces(p);
        lastComma = p;
        if (endOfLine(p) || *p != ',') break;
        p = skipSpaces(p + 1);
    }
    if (indir) {
        if (*p == ']') {
            p++;
        } else {
            setError(MISSING_CLOSING_PAREN);
        }
        if (n != 2) setErrorIf(UNKNOWN_OPERAND);
        _scan = p;
        op.mode = IDX;
        if (op.index == REG_0) {
            op.index = REG_UNDEF;
            op.sub = DISP_IDX;
        } else {
            op.sub = ACCM_IDX;
        }
        op.indir = true;
        return true;
    }
    if (n == 2) {
        _scan = p;
        op.mode = REG_REG;
        op.setOK(); // Clear possible DUPLICATE_REGISTER
        return true;
    }
    _scan = p;
    op.mode = REGLIST;
    op.base = stack;
    op.val32 = push_pull ? post : -1;
    op.extra = n;
    return true;
}

Error AsmMc6809::parseOperand(Operand &op) {
    const char *p = _scan;
    if (endOfLine(p)) return OK;

    const bool indir = (*p == '[');
    if (indir) p = skipSpaces(p + 1);

    if (parsePointerMode(op, p, indir))
        return getError();
    const RegName reg = _regs.parseRegName(p);
    if (!indir && _regs.isBitOpReg(reg)) {
        const char *a = p + _regs.regNameLen(reg);
        if (parseBitPosition(op, a)) {
            op.mode = REG_BIT;
            op.base = reg;
            return OK;
        }
    }
    if (parseRegisterList(op, p, indir))
        return getError();

    if (*p == '#') {
        if (indir) return setError(UNKNOWN_OPERAND);
        _scan = p + 1;
        if (getOperand(op.val32)) return getError();
        op.setError(getError());
        op.mode = IM32;
        return OK;
    }

    int8_t size = 0;
    if (*p == '>') {
        p++; size = 16;
    } else if (*p == '<') {
        p++; size = 8;
        if (*p == '<') {
            p++; size = 5;
        }
    }
    op.extra = size;
    p = skipSpaces(p);
    if (*p == ',') return setError(UNKNOWN_OPERAND);
    _scan = p;
    if (getOperand(op.val32)) return getError();
    op.setError(getError());
    setOK(); // Cleat error
    p = skipSpaces(_scan);

    if (*p == ',' && parseIndexedMode(op, p + 1, indir))
        return getError();

    if (size == 0 || size == 5) {
        const Config::uintptr_t addr =
            static_cast<Config::uintptr_t>(op.val32);
        size = static_cast<uint8_t>(addr >> 8) == _direct_page ? 8 : 16;
        op.extra = size;
    }

    if (!indir && parseBitPosition(op, p)) {
        op.mode = DIR_BIT;
        return OK;
    }

    if (indir) {
        if (*p == ']') {
            p++;
        } else if (*p == ',') {
            return setError(UNKNOWN_OPERAND);
        } else {
            return setError(MISSING_CLOSING_PAREN);
        }
        _scan = p;
        op.mode = IDX;
        op.sub = ABS_IDIR;
        return OK;
    }

    op.mode = (size == 8) ? DIR : EXT;
    return OK;
}

static const char TEXT_SETDP[]  PROGMEM = "SETDP";
static const char TEXT_ASSUME[] PROGMEM = "ASSUME";
static const char TEXT_DPR[]    PROGMEM = "DPR";

Error AsmMc6809::processPseudo(InsnMc6809 &insn) {
    insn.resetAddress(insn.address()); // make generated bytes zero.
    if (strcasecmp_P(insn.name(), TEXT_SETDP) == 0) {
        setError(getOperand(_direct_page));
        return OK;
    }
    if (strcasecmp_P(insn.name(), TEXT_ASSUME) == 0) {
        const char *p = _parser.scanSymbol(_scan);
        if (p - _scan == 3 && strncasecmp_P(_scan, TEXT_DPR, 3) == 0) {
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
    _scan = skipSpaces(endName);

    if (processPseudo(insn) == OK)
        return getError();

    Operand op, extra;
    if (parseOperand(op))
        return getError();
    const char *p = skipSpaces(_scan);
    if (*p == ',') {
        _scan = skipSpaces(p + 1);
        if (parseOperand(extra)) return getError();
        p = skipSpaces(_scan);
    }
    if (!endOfLine(p))
        return setError(GARBAGE_AT_END);
    setErrorIf(op.getError());
    setErrorIf(extra.getError());

    insn.setAddrMode(op.mode, extra.mode);
    if (TableMc6809.searchName(insn))
        return setError(TableMc6809.getError());
    switch (insn.addrMode()) {
    case NONE:
        insn.emitInsn();
        break;
    case REL:
    case LREL:
        encodeRelative(insn, op);
        break;
    case IM8:
    case IM16:
    case IM32:
        encodeImmediate(insn, op, extra);
        break;
    case DIR:
        insn.emitInsn();
        insn.emitByte(static_cast<uint8_t>(op.val32));
        break;
    case EXT:
        insn.emitInsn();
        insn.emitUint16(static_cast<uint16_t>(op.val32));
        break;
    case IDX:
        encodeIndexed(insn, op);
        break;
    case REG_REG:
        encodeRegisters(insn, op);
        break;
    case REGLIST:
        encodePushPull(insn, op);
        break;
    case REG_BIT:
        encodeBitOperation(insn, op, extra);
        break;
    case REG_TFM:
        encodeTransferMemory(insn, op, extra);
        break;
    default:
        return setError(UNKNOWN_OPERAND);
    }
    return getError();
}

} // namespace mc6809
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
