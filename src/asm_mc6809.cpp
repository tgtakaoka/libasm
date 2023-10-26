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
#include "text_common.h"

namespace libasm {
namespace mc6809 {

using namespace pseudo;
using namespace reg;
using namespace text::common;

namespace {

// clang-format off
constexpr char OPT_INT_SETDP[]  PROGMEM = "setdp";
constexpr char OPT_DESC_SETDP[] PROGMEM = "set direct page register";

constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_FCB, &Assembler::defineDataConstant, Assembler::DATA_BYTE_NO_STRING},
    {TEXT_FCC, &Assembler::defineString},
    {TEXT_FDB, &Assembler::defineDataConstant, Assembler::DATA_WORD_NO_STRING},
    {TEXT_RMB, &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

}  // namespace

const ValueParser::Plugins &AsmMc6809::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return MotorolaNumberParser::singleton(); }
        const CommentParser &comment() const override { return AsteriskCommentParser::singleton(); }
        const SymbolParser &symbol() const override { return _symbol; }
        const LetterParser &letter() const override { return MotorolaLetterParser::singleton(); }
        const LocationParser &location() const override {
            return AsteriskLocationParser::singleton();
        }
        const OperatorParser &operators() const override {
            return Mc68xxOperatorParser::singleton();
        }
        const PrefixSymbolParser _symbol{PSTR_DOT, PSTR_UNDER_DOT_DOLLER};
    } PLUGINS{};
    return PLUGINS;
}

AsmMc6809::AsmMc6809(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE, &_opt_setdp),
      Config(TABLE),
      _opt_setdp(this, &AsmMc6809::setDirectPage, OPT_INT_SETDP, OPT_DESC_SETDP) {
    reset();
}

void AsmMc6809::reset() {
    Assembler::reset();
    setDirectPage(0);
}

Error AsmMc6809::setDirectPage(int32_t val) {
    if (overflowUint8(static_cast<uint32_t>(val)))
        return OVERFLOW_RANGE;
    _direct_page = val;
    return OK;
}

bool AsmMc6809::onDirectPage(Config::uintptr_t addr) const {
    return static_cast<uint8_t>(addr >> 8) == _direct_page;
}

void AsmMc6809::encodeRelative(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    const auto length = (insn.hasPrefix() ? 1 : 0) + (mode == M_LREL ? 3 : 2);
    const auto base = insn.address() + length;
    const auto target = op.getError() ? base : op.val32;
    const auto delta = branchDelta(base, target, insn, op);
    if (mode == M_REL) {
        if (overflowInt8(delta))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        insn.emitOperand8(delta);
    } else {
        insn.emitOperand16(delta);
    }
}

Config::ptrdiff_t AsmMc6809::calculateDisplacement(const AsmInsn &insn, const Operand &op) const {
    const auto disp = static_cast<Config::ptrdiff_t>(op.val32);
    if (op.base == REG_PCR && op.isOK()) {
        const auto length = (insn.hasPrefix() ? 1 : 0) +
                            (insn.length() == 0 ? 3 : insn.length() + 2) + (op.extra == 16 ? 1 : 0);
        const auto base = static_cast<Config::uintptr_t>(insn.address()) + length;
        const auto target = static_cast<Config::uintptr_t>(op.val32);
        return target - base;
    }
    return disp;
}

void AsmMc6809::encodeIndexed(AsmInsn &insn, const Operand &op) const {
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
        } else if (!overflowInt8(disp)) {
            size = 8;
        } else {
            if (spec.base == REG_PCR)
                disp--;  // adjust to 16-bit displacement
            size = 16;
        }
        spec.size = size;
    }
    const auto postSpec = TABLE.searchPostSpec(cpuType(), spec);
    if (postSpec < 0)
        insn.setErrorIf(op, UNKNOWN_OPERAND);
    uint8_t post = postSpec;
    const auto size = spec.size;
    if (size == 5)
        post |= disp & 0x1F;
    if (spec.base == REG_X)
        post |= (encodeBaseReg(op.base) << 5);
    insn.emitOperand8(post);
    if (size == 8)
        insn.emitOperand8(disp);
    if (size == 16)
        insn.emitOperand16(disp);
}

void AsmMc6809::encodeRegisterPair(AsmInsn &insn, const Operand &op) const {
    const auto reg1 = op.mode == M_RBIT ? op.base : op.index;
    const auto reg2 = op.mode == M_RBIT ? REG_0 : op.base;
    const auto cpu = cpuType();
    if (!isDataReg(cpu, reg1) || !isDataReg(cpu, reg2))
        insn.setErrorIf(op, UNKNOWN_REGISTER);
    const auto size1 = regSize(reg1);
    const auto size2 = regSize(reg2);
    if (size1 != SZ_NONE && size2 != SZ_NONE && size1 != size2)
        insn.setErrorIf(op, ILLEGAL_SIZE);
    const auto post1 = encodeDataReg(reg1);
    const auto post2 = encodeDataReg(reg2);
    insn.emitOperand8((post1 << 4) | post2);
}

void AsmMc6809::encodeRegisterList(AsmInsn &insn, const Operand &op) const {
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
        auto reg = parseRegName(p);
        if (reg == REG_UNDEF)
            insn.setErrorIf(p, UNKNOWN_OPERAND);
        const auto bit = encodeStackReg(reg, userStack);
        if (bit == 0)
            insn.setErrorIf(r, REGISTER_NOT_ALLOWED);
        if (post & bit)
            insn.setErrorIf(r, DUPLICATE_REGISTER);
        post |= bit;
        if (endOfLine(p.skipSpaces()))
            break;
        if (!p.expect(',')) {
            insn.setErrorIf(p, UNKNOWN_OPERAND);
            break;
        }
    }
    insn.emitOperand8(post);
}

void AsmMc6809::encodeOperand(AsmInsn &insn, const Operand &op, AddrMode mode) const {
    insn.setErrorIf(op);
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
        insn.setPost(encodeBitOpReg(op.base) << 6);
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
        if (!isTfmBaseReg(op.index))
            op.setErrorIf(REGISTER_NOT_ALLOWED);
        return 0;
    }
    op.setError(UNKNOWN_OPERAND);
    return 0;
}

void AsmMc6809::encodeTransferMemory(AsmInsn &insn, Operand &op1, Operand &op2) const {
    const auto src = transferMemoryMode(op1);
    const auto dst = transferMemoryMode(op2);
    insn.setErrorIf(op1);
    insn.setErrorIf(op2);
    auto mode = encodeTfmMode(src, dst);
    if (mode < 0) {
        mode = 0;
        insn.setErrorIf(op1, UNKNOWN_OPERAND);
    }
    insn.embed(mode);

    const auto post1 = encodeTfmBaseReg(op1.index);
    const auto post2 = encodeTfmBaseReg(op2.index);
    const auto post = (post1 << 4) | post2;
    insn.emitOperand8(post);
}

bool AsmMc6809::parseBitPosition(StrScanner &scan, Operand &op) const {
    auto p = scan;
    if (*p == '.' || *p.skipSpaces() == ',') {
        const auto sep = *p++;
        if (sep == ',')
            p.skipSpaces();
        if (endOfLine(p) || (sep == '.' && isspace(*p)))
            return false;

        auto r = p;
        // Exclude register list or index addressing
        const auto reg = parseRegName(r);
        if (reg == REG_0) {
            // Constant zero is paresd as REG_0
            op.extra = 0;
            scan = r;
            return true;
        }
        if (reg != REG_UNDEF)
            return false;

        ErrorAt save(op);
        op.setError(p, OK);
        const auto bitp = parseExpr32(p, op);
        if (op.hasError()) {
            op.setError(save);
            return false;
        }
        if (save.getError())
            op.setError(save);
        if (bitp >= 8)
            op.setErrorIf(scan, ILLEGAL_BIT_NUMBER);
        op.extra = bitp;
        scan = p;
        return true;
    }
    return false;
}

bool AsmMc6809::parseMemBit(StrScanner &scan, Operand &op) const {
    auto end = scan;
    while (!endOfLine(end))
        ++end;
    StrScanner operand{scan.str(), end.str()};
    operand.trimEnd([](char c) { return c != '.'; });

    // There is no '.' in operand.
    if (operand.size() < 2) {
        op.val32 = parseExpr32(scan, op);
        return parseBitPosition(scan, op);
    } else if (isspace(operand[operand.size() - 2])) {
        op.val32 = parseExpr32(scan, op);
        return false;
    }

    // |operand| points '.'
    operand += operand.size() - 1;
    StrScanner expr{scan.str(), operand.str()};
    op.val32 = parseExpr32(expr, op);
    StrScanner bit{operand.str(), scan.str() + scan.size()};
    if (parseBitPosition(bit, op)) {
        scan = bit;
        return true;
    }
    return false;
}

Error AsmMc6809::parseOperand(StrScanner &scan, Operand &op, AddrMode hint) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    op.list = p;
    if (p.expect('#')) {
        op.val32 = parseExpr32(p, op);
        if (op.hasError())
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

    const auto index = parseRegName(p);
    if (index != REG_UNDEF) {
        auto a = p;
        if (hint == M_RBIT) {
            if (op.indir || index == REG_0)
                return op.setError(UNKNOWN_OPERAND);
            if (parseBitPosition(a, op)) {
                if (!isBitOpReg(index))
                    return op.setError(scan, ILLEGAL_REGISTER);
                op.mode = M_RBIT;
                op.base = index;
                scan = a;
                return OK;
            }
            return op.setError(UNKNOWN_OPERAND);
        }
        const auto autoindex = a.expect([](char c) { return c == '+' || c == '-'; });
        if (!op.indir && autoindex) {
            if (!isTfmBaseReg(index))
                op.setError(REGISTER_NOT_ALLOWED);
            op.index = index;
            op.mode = M_RTFM;
            op.extra = autoindex;
            scan = a;
            return OK;
        }
    } else if (indexBits) {
        if (hint == M_DBIT) {
            if (parseMemBit(p, op)) {
                if (op.isOK() && !onDirectPage(op.val32) && indexBits != 8)
                    return op.setError(scan, OPERAND_NOT_ALLOWED);
                op.mode = M_DBIT;
                scan = p;
                return OK;
            }
            return op.setError(scan, UNKNOWN_OPERAND);
        }
        op.val32 = parseExpr32(p, op);
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
                indexBits = onDirectPage(addr) ? 8 : 16;
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
    const auto base = parseRegName(p);
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

Error AsmMc6809::processPseudo(StrScanner &scan, Insn &insn) {
    const auto at = scan;
    if (strcasecmp_P(insn.name(), OPT_INT_SETDP) == 0) {
        const auto error = _opt_setdp.set(scan);
        return error ? insn.setError(at, error) : OK;
    }
    return Assembler::processPseudo(scan, insn);
}

Error AsmMc6809::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    auto error = TABLE.hasName(cpuType(), insn);
    if (error)
        return _insn.setError(error);

    if (parseOperand(scan, insn.op1, insn.mode1()) && insn.op1.hasError())
        return _insn.setError(insn.op1);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, insn.op2, insn.mode2()) && insn.op2.hasError())
            return _insn.setError(insn.op2);
        scan.skipSpaces();
    }

    if (_insn.setErrorIf(insn.op1, TABLE.searchName(cpuType(), insn)))
        return _insn.getError();

    const auto mode1 = insn.mode1();
    if (mode1 == M_RTFM) {
        encodeTransferMemory(insn, insn.op1, insn.op2);
    } else {
        encodeOperand(insn, insn.op1, mode1);
        encodeOperand(insn, insn.op2, insn.mode2());
    }
    insn.emitInsn();
    return _insn.setError(insn);
}

}  // namespace mc6809
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
