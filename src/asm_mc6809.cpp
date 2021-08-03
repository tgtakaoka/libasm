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

Error AsmMc6809::encodeRelative(InsnMc6809 &insn, const Operand &op, AddrMode mode) {
    const Config::uintptr_t base = insn.address() + insn.length() + (mode == LREL ? 2 : 1);
    const Config::uintptr_t target = op.getError() ? base : op.val32;
    const Config::ptrdiff_t delta = target - base;
    if (mode == REL) {
        if (delta >= 128 || delta < -128)
            return setError(OPERAND_TOO_FAR);
        insn.emitByte(delta);
    } else {
        insn.emitUint16(delta);
    }
    return OK;
}

Config::ptrdiff_t AsmMc6809::calculateDisplacement(
        const InsnMc6809 &insn, const Operand &op) const {
    const Config::ptrdiff_t disp = static_cast<Config::ptrdiff_t>(op.val32);
    if (op.base == REG_PCR && op.isOK()) {
        // assuming 8-bit displacement (post byte + 8-bit displacement)
        const Config::uintptr_t base = insn.address() + insn.length() + 2;
        const Config::uintptr_t target = op.val32;
        return target - base;
    }
    return disp;
}

Error AsmMc6809::encodeIndexed(InsnMc6809 &insn, const Operand &op) {
    PostSpec spec = {op.index, op.base, op.extra, op.indir};
    if (op.mode == REG_REG && spec.index == REG_0)  // 0,X
        spec.index = REG_UNDEF;

    Config::ptrdiff_t disp = calculateDisplacement(insn, op);
    if (spec.size == 0) {
        const bool pc = (spec.base == REG_PC || spec.base == REG_PCR);
        uint8_t size;
        if (op.getError()) {
            size = 16;                  // assume 16-bit displacement for undefined symbol.
        } else if (!pc && disp == 0) {  // ,X
            size = 0;
        } else if (!pc && !spec.indir && disp >= -16 && disp < 16) {
            size = 5;
        } else if (disp >= -128 && disp < 128) {
            size = 8;
        } else {
            if (spec.base == REG_PCR)
                disp--;  // adjust to 16-bit displacement
            size = 16;
        }
        spec.size = size;
    }
    const int16_t _post = TableMc6809.searchPostSpec(spec);
    if (_post < 0)
        return setError(UNKNOWN_OPERAND);
    uint8_t post = _post;
    if (spec.base == REG_X)
        post |= (RegMc6809::encodeBaseReg(op.base) << 5);
    const uint8_t size = spec.size;
    if (size == 5)
        post |= disp & 0x1F;
    insn.emitByte(post);
    if (size == 8)
        insn.emitByte(disp);
    if (size == 16)
        insn.emitUint16(disp);
    return OK;
}

Error AsmMc6809::encodeRegisters(InsnMc6809 &insn, const Operand &op) {
    const RegName reg1 = op.mode == REG_BIT ? op.base : op.index;
    const RegName reg2 = op.mode == REG_BIT ? REG_0 : op.base;
    if (!RegMc6809::isDataReg(reg1) || !RegMc6809::isDataReg(reg2))
        return setError(UNKNOWN_REGISTER);
    const RegSize size1 = RegMc6809::regSize(reg1);
    const RegSize size2 = RegMc6809::regSize(reg2);
    if (size1 != SZ_NONE && size2 != SZ_NONE && size1 != size2)
        return setError(ILLEGAL_SIZE);
    const uint8_t num1 = RegMc6809::encodeDataReg(reg1);
    const uint8_t num2 = RegMc6809::encodeDataReg(reg2);
    insn.emitByte((num1 << 4) | num2);
    return OK;
}

Error AsmMc6809::encodePushPull(InsnMc6809 &insn, const Operand &op) {
    uint8_t post = 0;
    RegName stack = REG_UNDEF;
    if (op.mode == NONE) {
        post = 0;
    } else if (op.mode == IM32) {
        post = op.val32;
    } else if (op.mode == REGLIST) {
        if (op.getError())
            return setError(op);
        if (static_cast<int32_t>(op.val32) < 0)
            return setError(REGISTER_NOT_ALLOWED);
        post = op.val32;
        stack = op.base;
    } else if (op.mode == REG_REG || op.mode == IDX) {
        RegName reg = op.index;
        if (reg == REG_S || reg == REG_U) {
            stack = reg;
            reg = REG_U;
        }
        uint8_t bit = RegMc6809::encodeStackReg(reg);
        if (bit == 0)
            return setError(REGISTER_NOT_ALLOWED);
        post = bit;
        reg = op.base;
        if (reg == REG_S || reg == REG_U) {
            if (stack == REG_UNDEF)
                stack = reg;
            if (stack != reg)
                return setError(REGISTER_NOT_ALLOWED);
            reg = REG_U;
        }
        bit = RegMc6809::encodeStackReg(reg);
        if (bit == 0)
            return setError(REGISTER_NOT_ALLOWED);
        if (post & bit)
            return setError(DUPLICATE_REGISTER);
        post |= bit;
    } else {
        return setError(OPERAND_NOT_ALLOWED);
    }
    const bool onUserStack = (insn.opCode() & 2) != 0;
    if (onUserStack && stack == REG_U)
        return setError(REGISTER_NOT_ALLOWED);
    if (!onUserStack && stack == REG_S)
        return setError(REGISTER_NOT_ALLOWED);
    insn.emitByte(post);
    return OK;
}

Error AsmMc6809::encodeOperand(InsnMc6809 &insn, const Operand &op, AddrMode mode) {
    switch (mode) {
    case REL:
    case LREL:
        return encodeRelative(insn, op, mode);
    case IM8:
        insn.emitByte(op.val32);
        break;
    case IM16:
        insn.emitUint16(op.val32);
        break;
    case IM32:
        insn.emitUint32(op.val32);
        break;
    case DIR:
        insn.emitByte(op.val32);
        break;
    case EXT:
        insn.emitUint16(op.val32);
        break;
    case IDX:
        return encodeIndexed(insn, op);
    case REG_REG:
        return encodeRegisters(insn, op);
    case REGLIST:
        return encodePushPull(insn, op);
    case REG_BIT:
        insn.setPost(RegMc6809::encodeBitOpReg(op.base) << 6);
        insn.embedPost(op.extra);
        break;
    case DIR_BIT:
        insn.embedPost(op.extra << 3);
        insn.emitByte(insn.post());
        insn.emitByte(op.val32);
    default:
        break;
    }
    return OK;
}

char AsmMc6809::transferMemoryMode(const Operand &op) {
    if (op.mode == REGLIST) {
        if (op.extra != 1)
            setError(UNKNOWN_OPERAND);
        if (!RegMc6809::isTfmBaseReg(op.index))
            setError(OPERAND_NOT_ALLOWED);
        return 0;
    }
    return op.extra;
}

Error AsmMc6809::encodeTransferMemory(InsnMc6809 &insn, const Operand &op1, const Operand &op2) {
    const char srcMode = transferMemoryMode(op1);
    const char dstMode = transferMemoryMode(op2);
    if (getError())
        return getError();

    const int8_t mode = RegMc6809::encodeTfmMode(srcMode, dstMode);
    if (mode < 0)
        return setError(UNKNOWN_OPERAND);
    insn.embed(mode);

    const uint8_t post =
            (RegMc6809::encodeTfmBaseReg(op1.index) << 4) | RegMc6809::encodeTfmBaseReg(op2.index);
    insn.emitInsn();
    insn.emitByte(post);
    return OK;
}

bool AsmMc6809::parsePointerMode(const char *p, Operand &op, bool indir) {
    if (*p++ != ',')
        return false;
    int8_t size = 0;
    if (*p == '-') {
        p++;
        size--;
        if (*p == '-') {
            p++;
            size--;
            ;
        }
    }
    const RegName reg = RegMc6809::parseRegName(p);
    if (!RegMc6809::isBaseReg(reg))
        return false;
    p += RegMc6809::regNameLen(reg);
    if (size == 0 && *p == '+') {
        p++;
        size++;
        if (*p == '+') {
            p++;
            size++;
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
    op.extra = size;
    op.indir = indir;
    return true;
}

bool AsmMc6809::parseIndexedMode(const char *scan, Operand &op, bool indir) {
    const char *p = skipSpaces(scan);
    const RegName base = RegMc6809::parseRegName(p);
    if (!RegMc6809::isIndexedBase(base))
        return false;
    p = skipSpaces(p + RegMc6809::regNameLen(base));
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
    op.indir = indir;
    return true;
}

bool AsmMc6809::parseBitPosition(const char *p, Operand &op) {
    const char *a = skipSpaces(p);
    if (*p == '.') {
        p++;
        // bit position separator '.' can't have space around it.
        if (*p == ' ' || *p == '\t')
            return false;
    } else if (*a == ',') {
        // bit position separator ',' can have spaces around it.
        p = skipSpaces(a + 1);
    } else
        return false;
    const RegName reg = RegMc6809::parseRegName(p);
    if (reg != REG_0 && reg != REG_UNDEF)
        return false;
    const uint32_t bitp = parseExpr32(p);
    op.setErrorIf(getError());
    if (parserError() || bitp >= 8) {
        setOK();
        return false;
    }
    op.extra = bitp;
    return true;
}

bool AsmMc6809::parseRegisterList(const char *p, Operand &op, bool indir) {
    RegName stack = REG_UNDEF;
    bool push_pull = true;
    uint8_t n = 0;
    uint8_t post = 0;
    const char *lastComma = p;
    while (true) {
        RegName reg = RegMc6809::parseRegName(p);
        if (reg == REG_UNDEF)
            return false;
        p += RegMc6809::regNameLen(reg);
        if (*p == '+' || *p == '-') {
            if (!RegMc6809::isTfmBaseReg(reg)) {
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
        if (n == 0)
            op.index = reg;
        if (n == 1)
            op.base = reg;
        n++;
        if (reg == REG_S || reg == REG_U) {
            if (stack == REG_UNDEF)
                stack = reg;
            if (stack != reg)
                push_pull = false;
            reg = REG_U;
        }
        const uint8_t bit = RegMc6809::encodeStackReg(reg);
        if (bit == 0)
            push_pull = false;
        if (post & bit)
            op.setError(DUPLICATE_REGISTER);
        post |= bit;
        p = skipSpaces(p);
        lastComma = p;
        if (endOfLine(p) || *p != ',')
            break;
        p = skipSpaces(p + 1);
    }
    if (indir) {
        if (*p == ']') {
            p++;
        } else {
            setError(MISSING_CLOSING_PAREN);
        }
        if (n != 2)
            setErrorIf(UNKNOWN_OPERAND);
        _scan = p;
        op.mode = IDX;  // [A,X] [0,X]
        if (op.index == REG_0)
            op.index = REG_UNDEF;
        op.indir = true;
        return true;
    }
    if (n == 2) {
        _scan = p;
        op.mode = REG_REG;
        op.setOK();  // Clear possible DUPLICATE_REGISTER
        return true;
    }
    _scan = p;
    op.mode = REGLIST;
    op.base = stack;
    op.val32 = push_pull ? post : -1;
    op.extra = n;
    return true;
}

Error AsmMc6809::parseOperand(const char *scan, Operand &op) {
    const char *p = skipSpaces(scan);
    _scan = p;
    if (endOfLine(p))
        return OK;

    const bool indir = (*p == '[');
    if (indir)
        p = skipSpaces(p + 1);

    if (parsePointerMode(p, op, indir))
        return getError();
    const RegName reg = RegMc6809::parseRegName(p);
    if (!indir && RegMc6809::isBitOpReg(reg)) {
        const char *a = p + RegMc6809::regNameLen(reg);
        if (parseBitPosition(a, op)) {
            op.mode = REG_BIT;
            op.base = reg;
            return OK;
        }
    }
    if (parseRegisterList(p, op, indir))
        return getError();

    if (*p == '#') {
        if (indir)
            return setError(UNKNOWN_OPERAND);
        op.val32 = parseExpr32(p + 1);
        if (parserError())
            return getError();
        op.setError(getError());
        op.mode = IM32;
        return OK;
    }

    int8_t size = 0;
    if (*p == '>') {
        p++;
        size = 16;
    } else if (*p == '<') {
        p++;
        size = 8;
        if (*p == '<') {
            p++;
            size = 5;
        }
    }
    op.extra = size;
    p = skipSpaces(p);
    if (*p == ',')
        return setError(UNKNOWN_OPERAND);
    op.val32 = parseExpr32(p);
    if (parserError())
        return getError();
    op.setError(getError());
    setOK();  // Cleat error
    p = skipSpaces(_scan);

    if (*p == ',' && parseIndexedMode(p + 1, op, indir))
        return getError();

    if (size == 0 || size == 5) {
        const Config::uintptr_t addr = static_cast<Config::uintptr_t>(op.val32);
        size = static_cast<uint8_t>(addr >> 8) == _direct_page ? 8 : 16;
        op.extra = size;
    }

    if (!indir && parseBitPosition(p, op)) {
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
        op.mode = IDX;  // [nnnn]
        op.extra = 16;
        return OK;
    }

    op.mode = (size == 8) ? DIR : EXT;
    return OK;
}

static const char TEXT_SETDP[] PROGMEM = "SETDP";

Error AsmMc6809::processPseudo(const char *scan, InsnMc6809 &insn) {
    const char *p = skipSpaces(scan);
    insn.resetAddress(insn.address());  // make generated bytes zero.
    if (strcasecmp_P(insn.name(), TEXT_SETDP) == 0) {
        const uint32_t val = parseExpr32(p);
        if (isOK())
            _direct_page = val;
        return OK;
    }
    return UNKNOWN_INSTRUCTION;
}

Error AsmMc6809::encode(Insn &_insn) {
    InsnMc6809 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);

    if (processPseudo(endName, insn) == OK)
        return getError();

    Operand op1, op2;
    if (parseOperand(endName, op1))
        return getError();
    const char *p = skipSpaces(_scan);
    if (*p == ',') {
        if (parseOperand(p + 1, op2))
            return getError();
        p = skipSpaces(_scan);
    }
    if (!endOfLine(p))
        return setError(GARBAGE_AT_END);
    setErrorIf(op1.getError());
    setErrorIf(op2.getError());

    insn.setAddrMode(op1.mode, op2.mode);
    if (TableMc6809.searchName(insn))
        return setError(TableMc6809.getError());

    const AddrMode mode1 = insn.mode1();
    if (mode1 == REG_TFM)
        return encodeTransferMemory(insn, op1, op2);
    insn.emitInsn();
    if (encodeOperand(insn, op1, mode1)) {
        insn.resetAddress(insn.address());
        return getError();
    }
    const AddrMode mode2 = insn.mode2();
    if (mode2 == NONE)
        return getError();
    return encodeOperand(insn, op2, mode2);
}

}  // namespace mc6809
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
