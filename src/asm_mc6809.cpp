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
            op.val32 = 0;
            op.extra = 0;
        } else if (_regs.isIndexReg(op.index)) {
            op.sub = ACCM_IDX;
            op.val32 = 0;
            op.extra = 0;
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

Error AsmMc6809::encodeRegisters(InsnMc6809 &insn, Operand &op) {
    const RegName reg1 = op.index;
    const RegName reg2 = op.base;
    const int8_t num1 = _regs.encodeDataReg(reg1);
    const int8_t num2 = _regs.encodeDataReg(reg2);
    if (num1 < 0 || num2 < 0) return setError(UNKNOWN_REGISTER);
    const RegSize size1 = RegMc6809::regSize(reg1);
    const RegSize size2 = RegMc6809::regSize(reg2);
    if (size1 != SZ_NONE && size2 != SZ_NONE && size1 != size2)
        return setError(ILLEGAL_SIZE);
    insn.emitInsn();
    insn.emitByte((num1 << 4) | num2);
    return setError(op.setOK());
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
        if (op.getError()) return setError(op);
        if (op.index == op.base) return setError(DUPLICATE_REGISTER);
        RegName r = op.index;
        if (r == REG_S || r == REG_U) stack = r;
        if (r == REG_S) r = REG_U;
        uint8_t p = _regs.encodeStackReg(r, false);
        if (p == 0) return setError(REGISTER_NOT_ALLOWED);
        post |= p;
        r = op.base;
        if ((r == REG_S || r == REG_U) && stack == REG_UNDEF) stack = r;
        if (r == REG_S) r = REG_U;
        p = _regs.encodeStackReg(r, false);
        if (p == 0) return setError(REGISTER_NOT_ALLOWED);
        post |= p;
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
        if (extra.extra != 1 || !_regs.isTfmBaseReg(extra.index))
            return setError(UNKNOWN_OPERAND);
        mode = 2;
    } else if (op.mode == REGLIST && extra.mode == REG_TFM) {
        if (op.extra != 1 || !_regs.isTfmBaseReg(op.index))
            return setError(UNKNOWN_OPERAND);
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

bool AsmMc6809::tokenPointerIndex(const char *p) {
    if (*p++ != ',') return false;
    int8_t auto_disp = 0;
    if (*p == '-') {
        p++; auto_disp = -1;
        if (*p == '-') {
            p++; auto_disp = -2;
        }
    }
    const RegName reg = _regs.parseRegName(p);
    if (!_regs.isBaseReg(reg)) return false;

    p += _regs.regNameLen(reg);
    if (auto_disp) {
        _token = IDX_AUTO;
    } else if (*p == '+') {
        p++; auto_disp = 1;
        if (*p == '+') {
            p++; auto_disp = 2;
        }
        _token = IDX_AUTO;
    } else {
        _token = IDX_PNTR;
    }
    _scan = p;
    _reg = reg;
    _extra = auto_disp;
    return true;
}

bool AsmMc6809::tokenTransferMemory(const char *p) {
    const RegName reg = _regs.parseRegName(p);
    if (!_regs.isTfmBaseReg(reg)) return false;
    p += _regs.regNameLen(reg);
    const char extra = (*p == '+' || *p == '-') ? *p : 0;
    if (extra == 0) return false;
    _token = TFM_MODE;
    _reg = reg;
    _extra = extra;
    _scan = p + 1;
    return true;
}

bool AsmMc6809::tokenRegisterList(const char *p) {
    _reg = _reg2 = _regStack = REG_UNDEF;
    bool push_pull = true;
    uint8_t n = 0;
    uint8_t post = 0;
    while (true) {
        RegName r = _regs.parseRegName(p);
        if (r == REG_UNDEF) return false; // not a register list
        p += _regs.regNameLen(r);
        n++;
        if (_reg == REG_UNDEF) _reg = r;
        else if (_reg2 == REG_UNDEF) _reg2 = r;
        if (r == REG_S || r == REG_U) {
            if (_regStack == REG_UNDEF) _regStack = r;
            if (_regStack != r)
                push_pull = false; // not a valid push/pull list
            if (r == REG_S) r = REG_U;
        }
        const uint8_t bit = _regs.encodeStackReg(r, false);
        if (bit == 0) push_pull = false; // not a valid push/pull list
        if (post & bit) setError(DUPLICATE_REGISTER);
        post |= bit;
        p = skipSpaces(p);
        if (endOfLine(p) || (n == 2 && *p == ']')) {
            _scan = p;
            _val32 = push_pull ? post : -1;
            _extra = n;
            _token = REG_LIST;
            return true;
        }
        if (*p != ',') return false;
        p = skipSpaces(p + 1);
    }
}

bool AsmMc6809::tokenDisplacementIndex(const char *p, int8_t size) {
    p = skipSpaces(p);
    const RegName base = _regs.parseRegName(p);
    if (!_regs.isIndexedBase(base)) return false;
    _scan = p + _regs.regNameLen(base);
    _reg = base;
    _extra = size;
    _token = IDX_DISP;
    return true;
}

bool AsmMc6809::tokenBitPosition(const char *p) {
    const char *a = skipSpaces(p);
    if (*p == '.') {
        p++;
        // bit position separator '.' can't have space around it.
        if (isspace(*p)) {
            _scan = p;
            return false;
        }
    } else if (*a == ',') {
        // bit position separator ',' can have spaces around it.
        p = skipSpaces(a + 1);
    } else return false;

    const char *saved = _scan;
    const Error error = getError();
    _scan = p;
    uint8_t bitp;
    if (getOperand(bitp) || bitp >= 8) {
        _scan = saved;
        return false;
    }
    setErrorIf(error);
    _extra = bitp;
    return true;
}

bool AsmMc6809::tokenConstant(const char *p, const char immediate) {
    int8_t size = 0;
    if (*p == '>') {
        p++; size = 16;
    } else if (*p == '<') {
        p++; size = 8;
        if (*p == '<') {
            p++; size = 5;
        }
    }
    // A value should follow size prefix without space.
    // An immediate value should not have size prefix.
    if (size && (isspace(*p) || immediate)) {
        _scan = p;
        return false;
    }
    _scan = p;
    if (getOperand(_val32)) {
        _scan = p + 1;
        return false;
    }
    p = skipSpaces(_scan);

    if (immediate) {
        _extra = 0;
        _token = VAL_IMM;
        _scan = p;
        return true;
    }

    if (*p == ',' && tokenDisplacementIndex(p + 1, size))
        return true;

    if (size == 0) {
        const Config::uintptr_t addr =
            static_cast<Config::uintptr_t>(_val32);
        if (static_cast<uint8_t>(addr >> 8) == _direct_page) {
            size = 8;
        } else {
            size = 16;
        }
    } else if (size == 5) {
        size = 8;
    }
    // A bit position an follow just after an direct address.
    if (size == 8 && tokenBitPosition(_scan)) {
        _token = DIR_BITP;
        return true;
    }
    _extra = size;
    _token = VAL_ADDR;
    return true;
}

AsmMc6809::Token AsmMc6809::nextToken(Token expect) {
    const char *p = skipSpaces(_scan);
    if (endOfLine(p))
        return _token = EOL;

    if (*p == '[' || *p == ']') {
        _scan = p + 1;
        return _token = Token(*p);
    }

    if (expect == COMMA && *p == ',') {
        p++;
        _scan = p;
        return _token = COMMA;
    }

    if (tokenPointerIndex(p))
        return _token;
    if (tokenTransferMemory(p))
        return _token;
    if (tokenRegisterList(p))
        return _token;

    const RegName reg = _regs.parseRegName(p);
    if (reg != REG_UNDEF) {
        p += _regs.regNameLen(reg);
        if ((reg == REG_A || reg == REG_B || reg == REG_CC)
            && tokenBitPosition(p)) {
            _reg = reg;
            return _token = REG_BITP;
        }
        _scan = p;
        _reg = reg;
        _reg2 = REG_UNDEF;
        _regStack = (reg == REG_U || reg == REG_S) ? reg : REG_UNDEF;
        _val32 = _regs.encodeStackReg((reg == REG_S) ? REG_U : reg, false);
        _extra = 1;
        return _token = REG_LIST;
    }

    const char immediate = (*p == '#') ? *p : 0;
    if (tokenConstant(immediate ? p + 1 : p, immediate))
        return _token;
    return _token = ERROR;
}

Error AsmMc6809::parseOperand(Operand &op) {
    if (nextToken() == EOL) {
        op.mode = NONE;
        return OK;
    }
    if (_token == TFM_MODE) {
        op.mode = REG_TFM;
        op.index = _reg;
        op.extra = _extra;
        return OK;
    }
    if (_token == REG_LIST) {
        op.setError(getError());
        op.index = _reg;
        op.base = _reg2;
        if (_extra == 2) {
            op.mode = REG_REG;
            return OK;
        }
        op.mode = REGLIST;
        op.base = _regStack;
        op.val32 = _val32;
        op.extra = _extra;
        return OK;
    }
    if (_token == REG_BITP) {
        op.setError(getError());
        op.mode = REG_BIT;
        op.base = _reg;
        op.extra = _extra;
        return OK;
    }
    if (_token == DIR_BITP) {
        op.setError(getError());
        op.mode = DIR_BIT;
        op.val32 = _val32;
        op.extra = _extra;
        return OK;
    }
    if (_token == VAL_IMM) {
        op.setError(getError());
        op.mode = IM32;
        op.val32 = _val32;
        op.extra = _extra;
        return OK;
    }

    op.indir = false;
    if (_token == LBRKT) {
        op.indir = true;
        nextToken();
    }
    if (_token == IDX_PNTR) {
        op.mode = IDX;
        op.sub = PNTR_IDX;
        op.base = _reg;
        op.extra = 0;
    } else if (_token == IDX_AUTO) {
        op.mode = IDX;
        op.sub = AUTO_IDX;
        op.base = _reg;
        op.extra = _extra;
    } else if (_token == IDX_DISP) {
        op.setError(getError());
        op.mode = IDX;
        op.sub = DISP_IDX;
        op.base = _reg;
        op.val32 = _val32;
        op.extra = _extra;
    } else if (_token == REG_LIST && _extra == 2) {
        if (!_regs.isIndexedBase(_reg2)) return setError(UNKNOWN_OPERAND);
        op.mode = IDX;
        op.base = _reg2;
        if (_reg == REG_0) {
            op.index = REG_UNDEF;
            op.val32 = 0;
            op.extra = 0;
            op.sub = DISP_IDX;
        } else if (_regs.isIndexReg(_reg)) {
            op.index = _reg;
            op.sub = ACCM_IDX;
        } else {
            return setError(UNKNOWN_OPERAND);
        }
    } else if (_token == VAL_ADDR) {
        op.setError(getError());
        if (op.indir) {
            op.mode = IDX;
            op.sub = ABS_IDIR;
            op.extra = _extra;
        } else {
            op.mode = (_extra == 8) ? DIR : EXT;
        }
        op.val32 = _val32;
    } else {
        return setError(UNKNOWN_OPERAND);
    }
    if (op.indir && nextToken() != RBRKT)
        return setError(UNKNOWN_OPERAND);
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
    _scan = endName;

    Operand op, extra;
    if (parseOperand(op)) return getError();
    nextToken(COMMA);
    if (_token == COMMA) {
        if (parseOperand(extra)) return getError();
        nextToken();
    }
    if (_token != EOL) return setError(UNKNOWN_OPERAND);
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
    setErrorIf(op.getError());
    setErrorIf(extra.getError());
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
