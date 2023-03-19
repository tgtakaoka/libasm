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

static const char OPT_INT_SETDP[] PROGMEM = "setdp";
static const char OPT_DESC_SETDP[] PROGMEM = "set direct page register";

AsmMc6809::OptSetdp::OptSetdp(AsmMc6809::PseudoMc6809 &pseudos)
    : IntOptionBase(OPT_INT_SETDP, OPT_DESC_SETDP), _pseudos(pseudos) {}

void AsmMc6809::encodeRelative(InsnMc6809 &insn, const Operand &op, AddrMode mode) {
    const auto length = (insn.hasPrefix() ? 1 : 0) + (mode == M_LREL ? 3 : 2);
    const auto base = static_cast<Config::uintptr_t>(insn.address()) + length;
    const auto target = op.getError() ? base : static_cast<Config::uintptr_t>(op.val32);
    const auto delta = static_cast<Config::ptrdiff_t>(target - base);
    if (mode == M_REL) {
        if (overflowRel8(delta))
            setErrorIf(op, OPERAND_TOO_FAR);
        insn.emitOperand8(delta);
    } else {
        insn.emitOperand16(delta);
    }
}

Config::ptrdiff_t AsmMc6809::calculateDisplacement(
        const InsnMc6809 &insn, const Operand &op) const {
    const auto disp = static_cast<Config::ptrdiff_t>(op.val32);
    if (op.base == REG_PCR && op.isOK()) {
        // assuming 8-bit displacement (post byte + 8-bit displacement)
        const auto length =
                (insn.hasPrefix() ? 1 : 0) + (insn.length() == 0 ? 3 : insn.length() + 2);
        const auto base = static_cast<Config::uintptr_t>(insn.address()) + length;
        const auto target = static_cast<Config::uintptr_t>(op.val32);
        return target - base;
    }
    return disp;
}

void AsmMc6809::encodeIndexed(InsnMc6809 &insn, const Operand &op) {
    PostSpec spec = {op.index, op.base, op.extra, op.indir};
    if (spec.index == REG_0)  // 0,X [0,X]
        spec.index = REG_UNDEF;

    auto disp = calculateDisplacement(insn, op);
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
    const auto postSpec = TableMc6809::TABLE.searchPostSpec(spec);
    if (postSpec < 0)
        setErrorIf(op, UNKNOWN_OPERAND);
    uint8_t post = postSpec;
    const auto size = spec.size;
    if (size == 5)
        post |= disp & 0x1F;
    if (spec.base == REG_X)
        post |= (RegMc6809::encodeBaseReg(op.base) << 5);
    insn.emitOperand8(post);
    if (size == 8)
        insn.emitOperand8(disp);
    if (size == 16)
        insn.emitOperand16(disp);
}

void AsmMc6809::encodeRegisterPair(InsnMc6809 &insn, const Operand &op) {
    const auto reg1 = op.mode == M_RBIT ? op.base : op.index;
    const auto reg2 = op.mode == M_RBIT ? REG_0 : op.base;
    if (!RegMc6809::isDataReg(reg1) || !RegMc6809::isDataReg(reg2))
        setErrorIf(op, UNKNOWN_REGISTER);
    const auto size1 = RegMc6809::regSize(reg1);
    const auto size2 = RegMc6809::regSize(reg2);
    if (size1 != SZ_NONE && size2 != SZ_NONE && size1 != size2)
        setErrorIf(op, ILLEGAL_SIZE);
    const auto post1 = RegMc6809::encodeDataReg(reg1);
    const auto post2 = RegMc6809::encodeDataReg(reg2);
    insn.emitOperand8((post1 << 4) | post2);
}

void AsmMc6809::encodeRegisterList(InsnMc6809 &insn, const Operand &op) {
    if (op.mode == M_NONE || op.mode == M_IM32) {
        insn.emitOperand8(op.val32);
        return;
    }
    const auto userStack = (insn.opCode() & 2) != 0;
    uint8_t post = 0;
    auto p = op.list;
    while (true) {
        p.skipSpaces();
        const auto r = p;
        auto reg = RegMc6809::parseRegName(p);
        if (reg == REG_UNDEF)
            setErrorIf(p, UNKNOWN_OPERAND);
        const auto bit = RegMc6809::encodeStackReg(reg, userStack);
        if (bit == 0)
            setErrorIf(r, REGISTER_NOT_ALLOWED);
        if (post & bit)
            setErrorIf(r, DUPLICATE_REGISTER);
        post |= bit;
        if (endOfLine(p.skipSpaces()))
            break;
        if (!p.expect(',')) {
            setErrorIf(p, UNKNOWN_OPERAND);
            break;
        }
    }
    insn.emitOperand8(post);
}

void AsmMc6809::encodeOperand(InsnMc6809 &insn, const Operand &op, AddrMode mode) {
    switch (mode) {
    case M_REL:
    case M_LREL:
        encodeRelative(insn, op, mode);
        break;
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
            encodeIndexed(insn, op);
            break;
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
            encodeIndexed(insn, op);
            break;
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
        return encodeRegisterPair(insn, op);
    case M_LIST:
        return encodeRegisterList(insn, op);
    case M_RBIT:
        insn.setPost(RegMc6809::encodeBitOpReg(op.base) << 6);
        insn.embedPost(op.extra & 7);
        break;
    case M_DBIT:
        insn.embedPost((op.extra & 7) << 3);
        insn.emitOperand8(insn.post());
        insn.emitOperand8(op.val32);
    default:
        break;
    }
}

char AsmMc6809::transferMemoryMode(Operand &op) const {
    if (op.mode == M_RTFM) {
        return (char)op.extra;
    } else if (op.mode == M_LIST && op.extra == 1) {
        if (!RegMc6809::isTfmBaseReg(op.index))
            op.setErrorIf(REGISTER_NOT_ALLOWED);
        return 0;
    }
    op.setError(UNKNOWN_OPERAND);
    return 0;
}

void AsmMc6809::encodeTransferMemory(InsnMc6809 &insn, Operand &op1, Operand &op2) {
    const auto src = transferMemoryMode(op1);
    const auto dst = transferMemoryMode(op2);
    setErrorIf(op1);
    setErrorIf(op2);
    auto mode = RegMc6809::encodeTfmMode(src, dst);
    if (mode < 0) {
        mode = 0;
        setErrorIf(op1, UNKNOWN_OPERAND);
    }
    insn.embed(mode);

    const auto post1 = RegMc6809::encodeTfmBaseReg(op1.index);
    const auto post2 = RegMc6809::encodeTfmBaseReg(op2.index);
    const auto post = (post1 << 4) | post2;
    insn.emitOperand8(post);
}

bool AsmMc6809::parseBitPosition(StrScanner &scan, Operand &op) const {
    // check leading space
    StrScanner p(op.errorAt());
    p.trimEndAt(scan);
    p.trimEnd(isspace);
    if (p.str() + p.size() != scan.str() && *scan == '.')
        return false;
    // '.n' must be just after register name or direct page address.
    // ',n' can have spaces after ','.
    p = scan;
    if (p.expect('.') || p.skipSpaces().expect(',')) {
        const auto reg = RegMc6809::parseRegName(p.skipSpaces());
        if (reg == REG_0) {
            op.extra = 0;
            scan = p;
            return true;
        }
        if (reg != REG_UNDEF)
            return false;

        ErrorAt error;
        error.setAt(p);
        const auto bitp = parseExpr32(p, error);
        if (parserError())
            return false;
        if (bitp >= 8)
            error.setErrorIf(ILLEGAL_BIT_NUMBER);
        op.setErrorIf(error);
        op.extra = bitp;
        scan = p;
        return true;
    }
    return false;
}

Error AsmMc6809::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    op.list = p;
    if (p.expect('#')) {
        op.val32 = parseExpr32(p, op);
        if (parserError())
            return op.getError();
        op.mode = M_IM32;
        scan = p;
        return OK;
    }

    if ((op.indir = p.expect('[')))
        p.skipSpaces();

    int8_t indexBits = -1;  // nn
    if (p.expect('>')) {
        indexBits = 16;  // >nn
    } else if (p.expect('<')) {
        indexBits = 8;  // <nn
        if (p.expect('<'))
            indexBits = 5;  // <<nn
    } else if (*p == ',') {
        indexBits = 0;  // ,X
    }

    const auto index = RegMc6809::parseRegName(p);
    if (index != REG_UNDEF) {
        auto a = p;
        if (!op.indir && parseBitPosition(a, op)) {
            op.mode = M_RBIT;
            op.base = index;
            scan = a;
            return OK;
        }
        const auto autoindex = a.expect([](char c) { return c == '+' || c == '-'; });
        if (!op.indir && autoindex) {
            if (!RegMc6809::isTfmBaseReg(index))
                op.setError(REGISTER_NOT_ALLOWED);
            op.index = index;
            op.mode = M_RTFM;
            op.extra = autoindex;
            scan = a;
            return OK;
        }
    } else if (indexBits) {
        const auto index = p;
        op.val32 = parseExpr32(p, op);
        if (parserError())
            return op.getError();

        if (parseBitPosition(p, op)) {
            if (op.isOK() && !_pseudos.inDirectPage(op.val32) && indexBits != 8)
                return op.setError(index, OPERAND_NOT_ALLOWED);
            op.mode = M_DBIT;
            scan = p;
            return OK;
        }
    }

    const auto endOfIndex = p.skipSpaces();
    if (!p.expect(',')) {
        if (index == REG_UNDEF) {
            if (op.indir) {
                if (!p.expect(']'))
                    return op.setErrorIf(p, MISSING_CLOSING_BRACKET);
                op.mode = M_IDX;  // [nnnn]
                op.extra = 16;
                scan = p;
                return OK;
            }
            if (indexBits < 0) {
                const auto addr = static_cast<Config::uintptr_t>(op.val32);
                indexBits = _pseudos.inDirectPage(addr) ? 8 : 16;
            }
            op.extra = indexBits;
            op.mode = (indexBits == 8) ? M_DIR : M_EXT;
            scan = p;
            return OK;
        }
        if (endOfLine(p)) {
            op.index = index;
            op.mode = M_LIST;
            op.extra = 1;  // single register
            scan = p;
            return OK;
        }
    }

    int8_t updateBase = 0;
    // Just after ','
    if (p.expect('-')) {
        updateBase--;  // ,-X
        if (p.expect('-'))
            updateBase--;  // ,--X
    }
    if (indexBits)
        p.skipSpaces();
    const auto base = RegMc6809::parseRegName(p);
    if (base == REG_UNDEF)
        return op.setError(UNKNOWN_OPERAND);
    // Just after |base| register
    if (p.expect('+')) {
        if (updateBase)
            return op.setError(UNKNOWN_OPERAND);
        updateBase++;  // ,X+
        if (p.expect('+'))
            updateBase++;  // ,X++
    }

    if (!op.indir && index != REG_UNDEF && updateBase == 1) {
        // found X,Y+, return the first 'X' only
        op.index = index;
        op.mode = M_LIST;
        op.extra = 1;
        scan = endOfIndex;
        return OK;
    }

    if (updateBase && (indexBits > 0 || index != REG_UNDEF))
        return op.setError(UNKNOWN_OPERAND);
    if (op.indir && !p.skipSpaces().expect(']'))
        return op.setError(MISSING_CLOSING_BRACKET);

    if (!op.indir && index != REG_UNDEF && updateBase == 0) {
        op.mode = M_PAIR;
    } else {
        op.mode = M_IDX;
    }
    op.index = index;
    op.base = base;
    op.extra = updateBase ? updateBase : (indexBits < 0 ? 0 : indexBits);
    scan = p;

    if (p.skipSpaces().expect(',')) {
        if (op.indir)
            return op.setError(UNKNOWN_OPERAND);
        // actual parsing happens on |encodeRegisterList|.
        while (!endOfLine(p))
            p += 1;
        op.mode = M_LIST;
        op.extra = 2;  // multiple registers
        scan = p;
        return OK;
    }

    return OK;
}

bool AsmMc6809::PseudoMc6809::endOfLine(const StrScanner &scan, bool headOfLine) const {
    return PseudoBase::endOfLine(scan, headOfLine) || (headOfLine && *scan == '*');
}

Error AsmMc6809::PseudoMc6809::processPseudo(StrScanner &scan, Insn &insn, Assembler &assembler) {
    if (strcasecmp_P(insn.name(), OPT_INT_SETDP) == 0) {
        auto p = scan;
        const auto val = assembler.parseExpr32(scan, assembler);
        if (assembler.isOK())
            setDp(val);
        scan = p;
        return OK;
    }
    return UNKNOWN_DIRECTIVE;
}

Error AsmMc6809::PseudoMc6809::setDp(int32_t value) {
    _direct_page = value;
    return OK;
}

Error AsmMc6809::encodeImpl(StrScanner &scan, Insn &_insn) {
    InsnMc6809 insn(_insn);
    Operand op1, op2;
    if (parseOperand(scan, op1) && op1.hasError())
        return setError(op1);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, op2) && op2.hasError())
            return setError(op2);
        scan.skipSpaces();
    }
    if (!endOfLine(scan))
        return setError(scan, GARBAGE_AT_END);
    setErrorIf(op1);
    setErrorIf(op2);

    insn.setAddrMode(op1.mode, op2.mode);
    const auto error = TableMc6809::TABLE.searchName(insn);
    if (error)
        return setError(op1, error);

    const auto mode1 = insn.mode1();
    if (mode1 == M_RTFM) {
        encodeTransferMemory(insn, op1, op2);
    } else {
        encodeOperand(insn, op1, mode1);
        encodeOperand(insn, op2, insn.mode2());
    }
    insn.emitInsn();
    return setErrorIf(insn);
}

}  // namespace mc6809
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
