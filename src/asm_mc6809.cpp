
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

//#define DEBUG_TOKEN
#if defined(DEBUG_TOKEN)
#include <stdio.h>
#endif

namespace libasm {
namespace mc6809 {

Error AsmMc6809::encodeRelative(InsnMc6809 &insn, const Operand &op) {
    const Config::uintptr_t target = op.getError() ? insn.address()
        : static_cast<Config::uintptr_t>(op.val32);
    const Config::uintptr_t base = insn.address()
        + (insn.hasPrefix() ? 2 : 1)
        + (insn.oprSize() == SZ_WORD ? 2 : 1);
    const Config::ptrdiff_t delta = target - base;
    if (insn.oprSize() == SZ_BYTE) {
        if (delta >= 128 || delta < -128) return setError(OPERAND_TOO_FAR);
        insn.emitInsn();
        insn.emitByte(static_cast<uint8_t>(delta));
    } else {
        insn.emitInsn();
        insn.emitUint16(delta);
    }
    return OK;
}

Error AsmMc6809::encodeImmediate(InsnMc6809 &insn, const Operand &op) {
    insn.emitInsn();
    if (insn.oprSize() == SZ_BYTE)
            insn.emitByte(static_cast<uint8_t>(op.val32));
    if (insn.oprSize() == SZ_WORD)
        insn.emitUint16(static_cast<uint16_t>(op.val32));
    if (insn.oprSize() == SZ_LONG)
        insn.emitUint32(op.val32);
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
    InsnMc6809 &insn, const Operand &op, bool emitInsn) {
    PostSpec spec;
    spec.mode = (op.mode == REG_REG) ? ACCM_IDX : op.sub;
    spec.index = (op.mode == REG_REG) ? op.list.pair.reg1 : op.index;
    spec.base = (op.mode == REG_REG) ? op.list.pair.reg2 : op.base;
    spec.size = (op.base == REG_PCR) ? 0 : op.extra;
    spec.indir = op.indir;
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
    const RegName reg1 = op.list.pair.reg1;
    const RegName reg2 = op.list.pair.reg2;
    if (op.list.pair.getError()) return setError(op.list.pair);
    const int8_t num1 = _regs.encodeDataReg(reg1);
    const int8_t num2 = _regs.encodeDataReg(reg2);
    if (num1 < 0 || num2 < 0) return setError(UNKNOWN_REGISTER);
    const OprSize size1 = RegMc6809::regSize(reg1);
    const OprSize size2 = RegMc6809::regSize(reg2);
    if (size1 != SZ_NONE && size2 != SZ_NONE && size1 != size2)
        return setError(ILLEGAL_SIZE);
    insn.emitInsn();
    insn.emitByte((num1 << 4) | num2);
    return OK;
}

AsmMc6809::RegPair::RegPair()
    : ErrorReporter(),
      reg1(REG_UNDEF),
      reg2(REG_UNDEF)
{
    setError(UNKNOWN_OPERAND);
}

void AsmMc6809::RegPair::add(const RegName reg) {
    if (reg1 == REG_UNDEF) {
        reg1 = reg;
    } else if (reg2 == REG_UNDEF) {
        reg2 = reg;
        setOK();
    } else {
        setError(UNKNOWN_OPERAND);
    }
}

bool AsmMc6809::RegPair::hasReg() const {
    return reg1 != REG_UNDEF || reg2 != REG_UNDEF;
}

AsmMc6809::RegList::RegList()
    : ErrorReporter(),
      post(0),
      stack(REG_UNDEF)
{}

bool AsmMc6809::RegList::add(
    Token token, RegName reg1, RegName reg2, bool zero) {
    if (token == VAL_ADDR && zero) { // 0 (REG_Z)
        add(REG_Z);
    } else if (token == REG_NAME) {  // R
        add(reg1);
    } else if (token == IDX_PNTR) {  // ,X
        add(reg1);
    } else if (token == IDX_ACCM) {  // A,X
        add(reg1);
        add(reg2);
    } else {
        return false;
    }
    return true;
}

void AsmMc6809::RegList::add(const RegName reg) {
    pair.add(reg);
    if (reg == REG_S || reg == REG_U) {
        if (stack == reg) {
            setErrorIf(DUPLICATE_REGISTER);
        } else if (stack != REG_UNDEF) {
            setErrorIf(REGISTER_NOT_ALLOWED);
        } else {
            stack = reg;
        }
    } else {
        const uint8_t bit = RegMc6809::encodeStackReg(reg, false);
        if (bit == 0) {
            setErrorIf(REGISTER_NOT_ALLOWED);
        } else if (post & bit) {
            setErrorIf(DUPLICATE_REGISTER);
        } else {
            post |= bit;
        }
    }
}

Error AsmMc6809::encodePushPull(InsnMc6809 &insn, const Operand &op) {
    uint8_t post = 0;
    if (op.mode == INH) {
        post = 0;
    } else if (op.mode == IMM) {
        post = static_cast<uint8_t>(op.val32);
    } else if (op.mode == REG_REG || op.mode == PSH_PUL) {
        if (op.list.getError()) return setError(op.list);
        post = op.list.post;
        if (op.list.stack != REG_UNDEF) {
            const bool onUserStack = (insn.opCode() & 2) != 0;
            const int8_t bit =
                _regs.encodeStackReg(op.list.stack, onUserStack);
            if (bit == 0) return setError(REGISTER_NOT_ALLOWED);
            post |= bit;
        }
    }
    insn.emitInsn();
    insn.emitByte(post);
    return OK;
}

Error AsmMc6809::encodeBitOperation(
    InsnMc6809 &insn, const Operand &op, const Operand &extra) {
    const uint8_t post = (_regs.encodeBitOpReg(op.base) << 6)
        | (extra.extra << 3) | op.extra;
    insn.emitInsn();
    insn.emitByte(post);
    insn.emitByte(static_cast<uint8_t>(extra.val32));
    return OK;
}

Error AsmMc6809::encodeImmediatePlus(
    InsnMc6809 &insn, const Operand &op, const Operand &extra) {
    encodeImmediate(insn, extra);
    if (op.mode == IMM_DIR)
        insn.emitByte(static_cast<uint8_t>(op.val32));
    if (op.mode == IMM_EXT)
        insn.emitUint16(static_cast<uint16_t>(op.val32));
    if (op.mode == IMM_IDX)
        encodeIndexed(insn, op, false);
    return OK;
}

Error AsmMc6809::encodeTransferMemory(
    InsnMc6809 &insn, const Operand &op) {
    insn.embed(op.extra);
    const uint8_t post = (_regs.encodeTfmBaseReg(op.index) << 4)
        | _regs.encodeTfmBaseReg(op.base);
    insn.emitInsn();
    insn.emitByte(post);
    return OK;
}

bool AsmMc6809::tokenPointerIndex(const char *p) {
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

bool AsmMc6809::tokenAccumulatorIndex(const char *p, RegName index) {
    if (!_regs.isIndexReg(index)) return false;
    p = skipSpaces(p);
    if (*p != ',') return false;
    p = skipSpaces(p + 1);
    const RegName base = _regs.parseRegName(p);
    if (!_regs.isBaseReg(base)) return false;
    _reg = index;
    _reg2 = base;
    _scan = p + _regs.regNameLen(base);
    _token = IDX_ACCM;
    return true;
}

bool AsmMc6809::tokenTransferMemory(const char *p, RegName reg1) {
    if (!_regs.isTfmBaseReg(reg1)) return false;
    const char mode1 = (*p == '+' || *p == '-') ? *p : 0;
    if (mode1) p++;
    p = skipSpaces(p);
    if (*p != ',') return false;
    p = skipSpaces(p + 1);
    const RegName reg2 = _regs.parseRegName(p);
    if (!_regs.isTfmBaseReg(reg2)) return false;
    p += _regs.regNameLen(reg2);
    const char mode2 = (*p == '+' || *p == '-') ? *p : 0;
    if (mode2) p++;
    if (mode1 == '+' && mode2 == '+') {
        _extra = 0;
    } else if (mode1 == '-' && mode2 == '-') {
        _extra = 1;
    } else if (mode1 == '+' && mode2 == 0) {
        _extra = 2;
    } else if (mode1 == 0 && mode2 == '+') {
        _extra = 3;
    } else return false;
    _reg = reg1;
    _reg2 = reg2;
    _scan = p;
    _token = TFM_MODE;
    return true;
}

bool AsmMc6809::tokenDisplacementIndex(const char *p, int8_t size) {
    p = skipSpaces(p);
    const RegName base = _regs.parseRegName(p);
    if (_regs.isBaseReg(base) || base == REG_PCR || base == REG_PC) {
        _scan = p + _regs.regNameLen(base);
        _reg = base;
        _extra = size;
        _token = IDX_DISP;
        return true;
    }
    return false;
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

    // if register name follows, this isn't a bit position.
    const RegName reg = _regs.parseRegName(p);
    if (reg != REG_UNDEF) return false;

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
        if (*p == ',') {
            p++;
            _token = VAL_IMMC;
        } else {
            _token = VAL_IMM;
        }
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

void AsmMc6809::printToken() const {
#if defined(DEBUG_TOKEN)
    switch (_token) {
    case EOL: printf("EOL\n"); return;
    case ERROR: printf("ERROR(%d) ", getError()); return;
    case COMMA: printf("COMMA "); return;
    case LBRKT: case RBRKT: printf("%c ", _token); return;
    case IDX_PNTR: printf("IDX_PNTR(,%c) ", _reg); return;
    case IDX_AUTO: printf("IDX_AUTO(%c%d) ", _reg, _extra); return;
    case IDX_DISP: printf("IDX_DISP(%d:%d:%c) ", _extra, _val32, _reg); return;
    case IDX_ACCM: printf("IDX_ACCM(%c,%c) ", _reg, _reg2); return;
    case TFM_MODE: printf("TFM_MODE(%d:%c,%c) ", _extra, _reg, _reg2); return;
    case REG_NAME: printf("REG_NAME(%c) ", _reg); return;
    case REG_BITP: printf("REG_BITP(%c.%d) ", _reg, _extra); return;
    case DIR_BITP: printf("DIR_BITP($%02X.%d) ", _val32, _extra); return;
    case VAL_IMM:  printf("VAL_IMM(#$%X) ", _val32); return;
    case VAL_IMMC: printf("VAL_IMMC(#$%X), ", _val32); return;
    case VAL_ADDR: printf("VAL_ADDR(%d:$%X) ", _extra, _val32); return;
    }
#endif
}

AsmMc6809::Token AsmMc6809::nextToken() {
    const char *p = skipSpaces(_scan);
    if (endOfLine(p))
        return _token = EOL;

    if (*p == '[' || *p == ']') {
        _scan = p + 1;
        return _token = Token(*p);
    }

    if (*p == ',') {
        p++;
        if (tokenPointerIndex(p))
            return _token;
        _scan = p;
        return _token = COMMA;
    }

    const RegName reg = _regs.parseRegName(p);
    if (reg != REG_UNDEF) {
        _reg = reg;
        p += _regs.regNameLen(reg);
        if (tokenTransferMemory(p, reg))
            return _token;
        if ((reg == REG_A || reg == REG_B || reg == REG_CC)
            && tokenBitPosition(p))
            return _token = REG_BITP;
        if (tokenAccumulatorIndex(p, reg))
            return _token;
        _scan = p;
        return _token = REG_NAME;
    }

    const char immediate = (*p == '#') ? *p : 0;
    if (tokenConstant(immediate ? p + 1 : p, immediate))
        return _token;
    return _token = ERROR;
}

Error AsmMc6809::parseOperand(Operand &op, Operand &extra) {
    op.resetError();
    extra.resetError();
    extra.mode = INH;
    if (nextToken() == EOL) {
        op.mode = INH;
        return OK;
    }
    if (_token == TFM_MODE) {
        op.mode = TFR_MEM;
        op.index = _reg;
        op.base = _reg2;
        op.extra = _extra;
        if (nextToken() != EOL) return setError(UNKNOWN_OPERAND);
        return OK;
    }
    if (_token == REG_BITP) {
        op.setError(getError());
        op.mode = BITOP;
        op.base = _reg;
        op.extra = _extra;
        if (nextToken() == EOL) {
            if (op.extra == 0) { // A/B/CC,0
                op.list.add(_reg);
                op.list.add(REG_Z);
                op.mode = REG_REG;
                return OK;
            }
            return setError(UNKNOWN_OPERAND);
        }
        if (_token != COMMA) return setError(UNKNOWN_OPERAND);
        if (nextToken() != DIR_BITP) return setError(UNKNOWN_OPERAND);
        extra.setError(getError());
        extra.mode = BITOP;
        extra.val32 = _val32;
        extra.extra = _extra;
        if (nextToken() != EOL) return setError(UNKNOWN_OPERAND);
        return OK;
    }
    bool hasImmediate = false;
    if (_token == VAL_IMM || _token == VAL_IMMC) {
        op.setError(getError());
        op.mode = IMM;
        op.val32 = _val32;
        op.extra = _extra;
        if (_token == VAL_IMM && nextToken() == EOL)
            return OK;
        if (_token != COMMA && _token != VAL_IMMC)
            setError(UNKNOWN_OPERAND);
        extra = op;
        hasImmediate = true;
        nextToken();
    }
    op.indir = false;
    if (_token == LBRKT) {
        op.indir = true;
        nextToken();
    }
    if (_token == IDX_PNTR) {
        op.mode = IDX;
        op.sub = PNTR_IDX;
        op.index = REG_UNDEF;
        op.base = _reg;
        op.extra = 0;
        nextToken();
    } else if (_token == IDX_AUTO) {
        op.mode = IDX;
        op.sub = AUTO_IDX;
        op.index = REG_UNDEF;
        op.base = _reg;
        op.extra = _extra;
        nextToken();
    } else if (_token == IDX_DISP) {
        op.setError(getError());
        op.mode = IDX;
        op.sub = DISP_IDX;
        op.index = REG_UNDEF;
        op.base = _reg;
        op.val32 = _val32;
        op.extra = _extra;
        nextToken();
    } else if (_token == IDX_ACCM) {
        op.mode = IDX;
        op.sub = ACCM_IDX;
        op.index = _reg;
        op.base = _reg2;
        op.extra = 0;
        if (!op.indir && !hasImmediate) {
            op.list.add(_reg);
            op.list.add(_reg2);
        }
        nextToken();
    } else if (_token == VAL_ADDR) {
        op.setError(getError());
        op.val32 = _val32;
        if (op.indir) {
            op.mode = IDX;
            op.index = op.base = REG_UNDEF;
            op.sub = ABS_IDIR;
            op.extra = 16;
        } else {
            op.mode = (_extra == 16) ? EXT : DIR;
            op.extra = _extra;
            if (!hasImmediate && op.val32 == 0)
                op.list.add(REG_Z);
        }
        nextToken();
    } else if (_token == REG_NAME && !op.indir && !hasImmediate) {
        op.list.add(_reg);
        op.mode = PSH_PUL;
        nextToken();
    } else {
        return setError(UNKNOWN_OPERAND);
    }

    if (op.indir) {
        if (_token != RBRKT) return setError(MISSING_CLOSING_PAREN);
        nextToken();
    } else if (!hasImmediate && op.list.pair.hasReg()) {
        while (_token == COMMA || _token == IDX_PNTR) {
            if (_token == COMMA) nextToken();
            if (op.list.add(_token, _reg, _reg2, _val32 == 0)) {
                op.mode = PSH_PUL;
                nextToken();
                continue;
            }
            return setError(UNKNOWN_OPERAND);
        }
    }

    if (_token != EOL) return setError(UNKNOWN_OPERAND);
    if (hasImmediate) {
        if (op.mode == DIR) op.mode = IMM_DIR;
        else if (op.mode == EXT) op.mode = IMM_EXT;
        else if (op.mode == IDX) op.mode = IMM_IDX;
        else return setError(UNKNOWN_OPERAND);
    } else if (!op.indir) {
        if (op.mode == IDX && op.sub == ACCM_IDX)
            op.mode = REG_REG;
        if (op.mode == PSH_PUL) {
            if (op.list.pair.getError() == OK)
                op.mode = REG_REG;
            return OK;
        }
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
    _scan = endName;

#if defined(DEBUG_TOKEN)
    printf("@@ tokens: \"%s\"; ", _scan);
    do {
        nextToken();
        printToken();
    } while (_token != EOL);
    _scan = endName;
#endif

    Operand op, extra;
    if (parseOperand(op, extra)) return getError();
    insn.setAddrMode(op.mode);

    if (TableMc6809.searchName(insn))
        return setError(TableMc6809.getError());
    switch (insn.addrMode()) {
    case INH:
        insn.emitInsn();
        break;
    case REL:
        encodeRelative(insn, op);
        break;
    case IMM:
        encodeImmediate(insn, op);
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
    case PSH_PUL:
        encodePushPull(insn, op);
        break;
    case BITOP:
        encodeBitOperation(insn, op, extra);
        break;
    case TFR_MEM:
        encodeTransferMemory(insn, op);
        break;
    case IMM_DIR:
    case IMM_EXT:
    case IMM_IDX:
        encodeImmediatePlus(insn, op, extra);
        break;
    default:  return setError(UNKNOWN_OPERAND);
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
