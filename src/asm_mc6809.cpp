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

#include <ctype.h>

namespace libasm {
namespace mc6809 {

Error AsmMc6809::encodeRelative(InsnMc6809 &insn, const Operand &op, AddrMode mode) {
    const Config::uintptr_t base =
            insn.address() + (mode == M_LREL ? 3 : 2) + (insn.hasPrefix() ? 1 : 0);
    const Config::uintptr_t target = op.getError() ? base : op.val32;
    const Config::ptrdiff_t delta = target - base;
    if (mode == M_REL) {
        if (overflowRel8(delta))
            return setError(op, OPERAND_TOO_FAR);
        insn.emitOperand8(delta);
    } else {
        insn.emitOperand16(delta);
    }
    return OK;
}

Config::ptrdiff_t AsmMc6809::calculateDisplacement(
        const InsnMc6809 &insn, const Operand &op) const {
    const Config::ptrdiff_t disp = static_cast<Config::ptrdiff_t>(op.val32);
    if (op.base == REG_PCR && op.isOK()) {
        // assuming 8-bit displacement (post byte + 8-bit displacement)
        const Config::uintptr_t base = insn.address() +
                                       (insn.length() == 0 ? 3 : insn.length() + 2) +
                                       (insn.hasPrefix() ? 1 : 0);
        const Config::uintptr_t target = op.val32;
        return target - base;
    }
    return disp;
}

Error AsmMc6809::encodeIndexed(InsnMc6809 &insn, const Operand &op) {
    PostSpec spec = {op.index, op.base, op.extra, op.indir};
    if (op.mode == M_PAIR && spec.index == REG_0)  // 0,X
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
        } else if (!overflowRel8(disp)) {
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
        return setError(op, UNKNOWN_OPERAND);
    uint8_t post = _post;
    if (spec.base == REG_X)
        post |= (RegMc6809::encodeBaseReg(op.base) << 5);
    const uint8_t size = spec.size;
    if (size == 5)
        post |= disp & 0x1F;
    insn.emitOperand8(post);
    if (size == 8)
        insn.emitOperand8(disp);
    if (size == 16)
        insn.emitOperand16(disp);
    return OK;
}

Error AsmMc6809::encodeRegisters(InsnMc6809 &insn, const Operand &op) {
    const RegName reg1 = op.mode == M_RBIT ? op.base : op.index;
    const RegName reg2 = op.mode == M_RBIT ? REG_0 : op.base;
    if (!RegMc6809::isDataReg(reg1) || !RegMc6809::isDataReg(reg2))
        return setError(op, UNKNOWN_REGISTER);
    const RegSize size1 = RegMc6809::regSize(reg1);
    const RegSize size2 = RegMc6809::regSize(reg2);
    if (size1 != SZ_NONE && size2 != SZ_NONE && size1 != size2)
        return setError(op, ILLEGAL_SIZE);
    const uint8_t num1 = RegMc6809::encodeDataReg(reg1);
    const uint8_t num2 = RegMc6809::encodeDataReg(reg2);
    insn.emitOperand8((num1 << 4) | num2);
    return OK;
}

Error AsmMc6809::encodePushPull(InsnMc6809 &insn, const Operand &op) {
    uint8_t post = 0;
    RegName stack = REG_UNDEF;
    if (op.mode == M_NONE) {
        post = 0;
    } else if (op.mode == M_IM32) {
        post = op.val32;
    } else if (op.mode == M_LIST) {
        if (op.getError())
            return setError(op);
        if (static_cast<int32_t>(op.val32) < 0)
            return setError(op, REGISTER_NOT_ALLOWED);
        post = op.val32;
        stack = op.base;
    } else if (op.mode == M_PAIR || op.mode == M_IDX) {
        RegName reg = op.index;
        if (reg == REG_S || reg == REG_U) {
            stack = reg;
            reg = REG_U;
        }
        uint8_t bit = RegMc6809::encodeStackReg(reg);
        if (bit == 0)
            return setError(op, REGISTER_NOT_ALLOWED);
        post = bit;
        reg = op.base;
        if (reg == REG_S || reg == REG_U) {
            if (stack == REG_UNDEF)
                stack = reg;
            if (stack != reg)
                return setError(op, REGISTER_NOT_ALLOWED);
            reg = REG_U;
        }
        bit = RegMc6809::encodeStackReg(reg);
        if (bit == 0)
            return setError(op, REGISTER_NOT_ALLOWED);
        if (post & bit)
            return setError(op, DUPLICATE_REGISTER);
        post |= bit;
    } else {
        return setError(op, OPERAND_NOT_ALLOWED);
    }
    const bool onUserStack = (insn.opCode() & 2) != 0;
    if (onUserStack && stack == REG_U)
        return setError(op, REGISTER_NOT_ALLOWED);
    if (!onUserStack && stack == REG_S)
        return setError(op, REGISTER_NOT_ALLOWED);
    insn.emitOperand8(post);
    return OK;
}

Error AsmMc6809::encodeOperand(InsnMc6809 &insn, const Operand &op, AddrMode mode) {
    switch (mode) {
    case M_REL:
    case M_LREL:
        return encodeRelative(insn, op, mode);
    case M_GEN16:
        if (op.mode == M_IM32) {
            insn.emitOperand16(op.val32);
            break;
        }
        // Fall through
    case M_GEN8:
        if (op.mode == M_DIR) {
            insn.embed(0x10);
            insn.emitOperand8(op.val32);
            break;
        }
        if (op.mode == M_IDX || op.mode == M_PAIR) {
            insn.embed(0x20);
            return encodeIndexed(insn, op);
        }
        if (op.mode == M_EXT) {
            insn.embed(0x30);
            insn.emitOperand16(op.val32);
            break;
        }
        // Fall through
    case M_IM8:
        insn.emitOperand8(op.val32);
        break;
    case M_IM32:
        insn.emitOperand32(op.val32);
        break;
    case M_GMEM:
        if (op.mode == M_IDX || op.mode == M_PAIR) {
            insn.embed(0x60);
            return encodeIndexed(insn, op);
        }
        if (op.mode == M_EXT) {
            insn.embed(0x70);
            insn.emitOperand16(op.val32);
            break;
        }
        // Fall-through
    case M_DIR:
        insn.emitOperand8(op.val32);
        break;
    case M_EXT:
        insn.emitOperand16(op.val32);
        break;
    case M_IDX:
        return encodeIndexed(insn, op);
    case M_PAIR:
        return encodeRegisters(insn, op);
    case M_LIST:
        return encodePushPull(insn, op);
    case M_RBIT:
        insn.setPost(RegMc6809::encodeBitOpReg(op.base) << 6);
        insn.embedPost(op.extra);
        break;
    case M_DBIT:
        insn.embedPost(op.extra << 3);
        insn.emitOperand8(insn.post());
        insn.emitOperand8(op.val32);
    default:
        break;
    }
    return OK;
}

char AsmMc6809::transferMemoryMode(const Operand &op) {
    if (op.mode == M_LIST) {
        if (op.extra != 1)
            setError(op, UNKNOWN_OPERAND);
        if (!RegMc6809::isTfmBaseReg(op.index))
            setError(op, OPERAND_NOT_ALLOWED);
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
        return setError(op1, UNKNOWN_OPERAND);
    insn.embed(mode);

    const uint8_t post =
            (RegMc6809::encodeTfmBaseReg(op1.index) << 4) | RegMc6809::encodeTfmBaseReg(op2.index);
    insn.emitOperand8(post);
    insn.emitInsn();
    return OK;
}

bool AsmMc6809::parsePointerMode(StrScanner &scan, Operand &op, bool indir) {
    StrScanner p(scan);
    if (!p.expect(','))
        return false;
    int8_t size = 0;
    if (p.expect('-')) {
        size--;
        if (p.expect('-'))
            size--;
    }
    const RegName reg = RegMc6809::parseRegName(p);
    if (!RegMc6809::isBaseReg(reg))
        return false;
    if (size == 0) {
        if (p.expect('+')) {
            size++;
            if (p.expect('+'))
                size++;
        }
    }
    p.skipSpaces();
    if (indir && !p.expect(']'))
        setError(p, MISSING_CLOSING_BRACKET);
    op.base = reg;
    op.mode = M_IDX;
    op.extra = size;
    op.indir = indir;
    scan = p;
    return true;
}

bool AsmMc6809::parseIndexedMode(StrScanner &scan, Operand &op, bool indir) {
    StrScanner p(scan);
    const RegName base = RegMc6809::parseRegName(p);
    if (!RegMc6809::isIndexedBase(base))
        return false;
    p.skipSpaces();
    if (indir && !p.expect(']'))
        setError(p, MISSING_CLOSING_BRACKET);
    op.base = base;
    op.mode = M_IDX;
    op.indir = indir;
    scan = p;
    return true;
}

bool AsmMc6809::parseBitPosition(StrScanner &p, Operand &op) {
    StrScanner a(p);
    if (p.expect('.')) {
        // bit position separator '.' can't have space around it.
        if (isspace(*p))
            return false;
        a = p;
    } else if (a.skipSpaces().expect(',')) {
        // bit position separator ',' can have spaces around it.
        a.skipSpaces();
    } else {
        return false;
    }
    const RegName reg = RegMc6809::parseRegName(a);
    if (reg == REG_0) {
        op.extra = 0;
        p = a;
        return true;
    }
    if (reg != REG_UNDEF)
        return false;
    const uint32_t bitp = parseExpr32(p = a, op);
    if (parserError() || bitp >= 8) {
        setError(op.setOK());
        return false;
    }
    op.extra = bitp;
    return true;
}

bool AsmMc6809::parseRegisterList(StrScanner &scan, Operand &op, bool indir) {
    RegName stack = REG_UNDEF;
    bool push_pull = true;
    uint8_t n = 0;
    uint8_t post = 0;
    StrScanner p(scan);
    StrScanner lastComma(p);
    while (true) {
        RegName reg = RegMc6809::parseRegName(p);
        if (reg == REG_UNDEF)
            return false;
        if (*p == '+' || *p == '-') {
            if (!RegMc6809::isTfmBaseReg(reg)) {
                op.setError(p, REGISTER_NOT_ALLOWED);
                scan = ++p;
                return true;
            }
            if (n == 0) {
                op.index = reg;
                op.mode = M_RTFM;
                op.extra = *p++;
                scan = p;
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
            op.setError(p, DUPLICATE_REGISTER);
        post |= bit;
        lastComma = p.skipSpaces();
        if (endOfLine(*p) || !p.expect(','))
            break;
        p.skipSpaces();
    }
    if (indir) {
        if (!p.expect(']'))
            setError(p, MISSING_CLOSING_BRACKET);
        if (n != 2)
            setErrorIf(p, UNKNOWN_OPERAND);
        scan = p;
        op.mode = M_IDX;  // [A,X] [0,X]
        if (op.index == REG_0)
            op.index = REG_UNDEF;
        op.indir = true;
        return true;
    }
    scan = p;
    if (n == 2) {
        op.mode = M_PAIR;
        op.setOK();  // Clear possible DUPLICATE_REGISTER
        return true;
    }
    op.mode = M_LIST;
    op.base = stack;
    op.val32 = push_pull ? post : -1;
    op.extra = n;
    return true;
}

Error AsmMc6809::parseOperand(StrScanner &scan, Operand &op) {
    StrScanner p(scan.skipSpaces());
    op.setAt(p);
    if (endOfLine(*p))
        return OK;

    const bool indir = p.expect('[');
    if (parsePointerMode(p.skipSpaces(), op, indir)) {
        scan = p;
        return getError();
    }
    StrScanner a(p);
    const RegName reg = RegMc6809::parseRegName(a);
    if (!indir && RegMc6809::isBitOpReg(reg)) {
        if (parseBitPosition(a, op)) {
            op.mode = M_RBIT;
            op.base = reg;
            scan = a;
            return OK;
        }
    }
    if (parseRegisterList(p, op, indir)) {
        scan = p;
        return getError();
    }

    if (p.expect('#')) {
        if (indir)
            return setError(op, UNKNOWN_OPERAND);
        op.val32 = parseExpr32(p, op);
        if (parserError())
            return getError();
        op.mode = M_IM32;
        scan = p;
        return OK;
    }

    int8_t size = 0;
    if (p.expect('>')) {
        size = 16;
    } else if (p.expect('<')) {
        size = 8;
        if (p.expect('<'))
            size = 5;
    }
    op.extra = size;
    if (*p.skipSpaces() == ',')
        return setError(scan, UNKNOWN_OPERAND);
    op.val32 = parseExpr32(p, op);
    if (parserError())
        return getError();
    setOK();  // Clear error

    if (*p.skipSpaces() == ',') {
        StrScanner a(p);
        ++a;
        if (parseIndexedMode(a.skipSpaces(), op, indir)) {
            scan = a;
            return getError();
        }
    }

    if (size == 0 || size == 5) {
        const Config::uintptr_t addr = static_cast<Config::uintptr_t>(op.val32);
        size = static_cast<uint8_t>(addr >> 8) == _direct_page ? 8 : 16;
        op.extra = size;
    }

    if (!indir && parseBitPosition(p, op)) {
        op.mode = M_DBIT;
        scan = p;
        return OK;
    }

    if (indir) {
        if (*p == ',')
            return setError(p, UNKNOWN_OPERAND);
        if (!p.expect(']'))
            return setError(p, MISSING_CLOSING_BRACKET);
        op.mode = M_IDX;  // [nnnn]
        op.extra = 16;
        scan = p;
        return OK;
    }

    op.mode = (size == 8) ? M_DIR : M_EXT;
    scan = p;
    return OK;
}

Error AsmMc6809::processPseudo(StrScanner &scan, InsnMc6809 &insn) {
    insn.clear();  // make generated bytes zero.
    if (strcasecmp_P(insn.name(), PSTR("setdp")) == 0) {
        StrScanner p(scan);
        const uint32_t val = parseExpr32(p, *this);
        if (isOK())
            _direct_page = val;
        scan = p;
        return OK;
    }
    return UNKNOWN_INSTRUCTION;
}

Error AsmMc6809::encode(StrScanner &scan, Insn &_insn) {
    InsnMc6809 insn(_insn);
    insn.setName(_parser.readSymbol(scan));

    if (processPseudo(scan, insn) == OK)
        return getError();

    Operand op1, op2;
    if (parseOperand(scan, op1))
        return getError();
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, op2))
            return getError();
        scan.skipSpaces();
    }
    if (!endOfLine(*scan))
        return setError(scan, GARBAGE_AT_END);
    setErrorIf(op1);
    setErrorIf(op2);

    insn.setAddrMode(op1.mode, op2.mode);
    if (TableMc6809.searchName(insn))
        return setError(TableMc6809.getError());

    const AddrMode mode1 = insn.mode1();
    if (mode1 == M_RTFM)
        return encodeTransferMemory(insn, op1, op2);
    if (encodeOperand(insn, op1, mode1)) {
        insn.reset();
        return getError();
    }
    const AddrMode mode2 = insn.mode2();
    if (encodeOperand(insn, op2, mode2)) {
        insn.reset();
        return getError();
    }
    insn.emitInsn();
    return getError();
}

}  // namespace mc6809
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
