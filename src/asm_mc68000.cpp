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

#include "asm_mc68000.h"

#include "table_mc68000.h"
#include "text_common.h"

namespace libasm {
namespace mc68000 {

using namespace pseudo;
using namespace reg;
using namespace text::common;

namespace {

// clang-format off
constexpr char TEXT_DC_B[] PROGMEM = "dc.b";
constexpr char TEXT_DC_L[] PROGMEM = "dc.l";
constexpr char TEXT_DC_W[] PROGMEM = "dc.w";
constexpr char TEXT_DS_B[] PROGMEM = "ds.b";
constexpr char TEXT_DS_L[] PROGMEM = "ds.l";
constexpr char TEXT_DS_W[] PROGMEM = "ds.w";

constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_dALIGN, &Assembler::alignOrigin},
    {TEXT_DC,     &Assembler::defineDataConstant, Assembler::DATA_WORD_ALIGN2},
    {TEXT_DC_B,   &Assembler::defineDataConstant, Assembler::DATA_BYTE},
    {TEXT_DC_L,   &Assembler::defineDataConstant, Assembler::DATA_LONG_ALIGN2},
    {TEXT_DC_W,   &Assembler::defineDataConstant, Assembler::DATA_WORD_ALIGN2},
    {TEXT_DS,     &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
    {TEXT_DS_B,   &Assembler::allocateSpaces,     Assembler::DATA_BYTE},
    {TEXT_DS_L,   &Assembler::allocateSpaces,     Assembler::DATA_LONG_ALIGN2},
    {TEXT_DS_W,   &Assembler::allocateSpaces,     Assembler::DATA_WORD_ALIGN2},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

struct Mc68000SymbolParser final : PrefixSymbolParser {
    Mc68000SymbolParser() : PrefixSymbolParser(PSTR_DOT, PSTR_UNDER_DOT_DOLLER) {}
    bool instructionLetter(char c) const override {
        return PrefixSymbolParser::instructionLetter(c) || c == '.';
    }
};

}  // namespace

const ValueParser::Plugins &AsmMc68000::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return MotorolaNumberParser::singleton(); }
        const CommentParser &comment() const override { return AsteriskCommentParser::singleton(); }
        const SymbolParser &symbol() const override { return _symbol; }
        const LocationParser &location() const override {
            return AsteriskLocationParser::singleton();
        }
        const Mc68000SymbolParser _symbol{};
    } PLUGINS{};
    return PLUGINS;
}

AsmMc68000::AsmMc68000(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE), Config(TABLE) {
    reset();
}

namespace {

int8_t modePos(OprPos pos) {
    switch (pos) {
    case OP_10:
        return 3;
    case OP_23:
        return 6;
    default:
        return -1;
    }
}

int8_t regPos(OprPos pos) {
    switch (pos) {
    case OP_10:
    case OP__0:
        return 0;
    case OP_23:
    case OP__3:
        return 9;
    default:
        return -1;
    }
}

}  // namespace

void emitOprSize(AsmInsn &insn, OprSize size) {
    Config::opcode_t sz = 0;
    switch (insn.oprSize()) {
    case SZ_DATA:
        if (size == SZ_NONE)
            size = SZ_WORD;
        sz = static_cast<uint8_t>(size) << 6;
        break;
    case SZ_ADR6:
        if (size == SZ_NONE)
            size = SZ_WORD;
        sz = (size == SZ_LONG) ? (1 << 6) : 0;
        break;
    case SZ_ADR8:
        if (size == SZ_NONE)
            size = SZ_WORD;
        sz = (size == SZ_LONG) ? (1 << 8) : 0;
        break;
    default:
        return;
    }
    insn.embed(sz);
}

Error AsmMc68000::checkAlignment(AsmInsn &insn, OprSize size, const Operand &op) const {
    if (size == SZ_WORD && (op.val32 % 2))
        return insn.setError(op, OPERAND_NOT_ALIGNED);
    if (size == SZ_LONG && (op.val32 % 2))
        return insn.setError(op, OPERAND_NOT_ALIGNED);
    return OK;
}

void AsmMc68000::emitBriefExtension(
        AsmInsn &insn, const Operand &op, Config::ptrdiff_t disp) const {
    if (overflowInt8(disp))
        insn.setErrorIf(op, OVERFLOW_RANGE);
    uint16_t ext = static_cast<uint8_t>(disp);
    ext |= encodeGeneralRegNo(op.indexReg) << 12;
    if (isAddrReg(op.indexReg))
        ext |= (1 << 15);
    if (op.indexSize == SZ_LONG)
        ext |= (1 << 11);
    insn.emitOperand16(ext);
}

void AsmMc68000::emitDisplacement(AsmInsn &insn, const Operand &op, Config::ptrdiff_t disp) const {
    if (overflowInt16(disp))
        insn.setErrorIf(op, OVERFLOW_RANGE);
    insn.emitOperand16(static_cast<uint16_t>(disp));
}

void AsmMc68000::emitRelativeAddr(AsmInsn &insn, AddrMode mode, const Operand &op) const {
    const auto base = insn.address() + 2;
    const auto target = op.getError() ? base : op.val32;
    const auto disp = branchDelta(base, target, insn, op);
    if (mode == M_REL8) {
        if (!overflowInt8(disp)) {
            insn.embed(static_cast<uint8_t>(disp));
            return;
        }
    }
    if (overflowInt16(disp))
        insn.setErrorIf(op, OPERAND_TOO_FAR);
    insn.emitOperand16(static_cast<uint16_t>(disp));
}

void AsmMc68000::emitImmediateData(
        AsmInsn &insn, const Operand &op, OprSize size, uint32_t data) const {
    switch (size) {
    case SZ_LONG:
        insn.emitOperand32(data);
        return;
    case SZ_WORD:
        if (overflowUint16(data))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(data);
        return;
    case SZ_BYTE:
        if (overflowUint8(data))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(static_cast<uint8_t>(data));
        return;
    default:
        return;
    }
}

Config::uintptr_t Operand::offset(const AsmInsn &insn) const {
    if (getError())
        return 0;
    uint8_t len = insn.length();
    if (len == 0)
        len = sizeof(Config::opcode_t);
    return val32 - (insn.address() + len);
}

Error AsmMc68000::emitEffectiveAddr(
        AsmInsn &insn, OprSize size, const Operand &op, AddrMode mode, OprPos pos) const {
    if (mode == M_NONE) {
        if (op.mode != M_NONE)
            insn.setErrorIf(op, UNKNOWN_OPERAND);
        return getError();
    }

    const int8_t mode_gp = modePos(pos);
    if (mode_gp >= 0) {
        const Config::opcode_t m = EaMc68000::encodeMode(op.mode);
        insn.embed(m << mode_gp);
    }

    const int8_t reg_gp = regPos(pos);
    if (reg_gp >= 0) {
        const Config::opcode_t r = EaMc68000::encodeRegNo(op.mode, op.reg);
        insn.embed(r << reg_gp);
    }

    switch (op.mode) {
    case M_AREG:
        if (size == SZ_BYTE)
            insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
        break;
    case M_INDX:
        emitBriefExtension(insn, op, static_cast<Config::ptrdiff_t>(op.val32));
        break;
    case M_PCIDX:
        emitBriefExtension(insn, op, op.offset(insn));
        break;
    case M_DISP:
        emitDisplacement(insn, op, static_cast<Config::ptrdiff_t>(op.val32));
        break;
    case M_PCDSP:
        if (checkAlignment(insn, size, op))
            insn.setErrorIf(op, getError());
        emitDisplacement(insn, op, op.offset(insn));
        break;
    case M_AWORD:
    case M_ALONG:
        if (checkAlignment(insn, size, op))
            insn.setErrorIf(op, getError());
        if (op.mode == M_AWORD) {
            insn.emitOperand16(op.val32);
        } else {
            insn.emitOperand32(op.val32);
        }
        break;
    case M_IMDAT:
        if (mode == M_IMBIT)
            break;
        if (mode == M_IM3) {
            // "Zero means 2^3" unsigned 3-bit.
            if (op.val32 > 8)
                insn.setErrorIf(op, OVERFLOW_RANGE);
            if (op.val32 == 0 && op.getError() == OK)
                insn.setErrorIf(op, OPERAND_NOT_ALLOWED);
            const Config::opcode_t count = (op.val32 & 7);  // 8 is encoded to 0.
            const Config::opcode_t data = op.getError() ? 0 : (count << 9);
            insn.embed(data);
            break;
        }
        if (mode == M_IM8) {
            // Signed 8-bit.
            if (overflowInt8(static_cast<int32_t>(op.val32)))
                insn.setErrorIf(op, OVERFLOW_RANGE);
            insn.embed(static_cast<uint8_t>(op.val32));
            break;
        }
        if (mode == M_IMVEC) {
            if (op.val32 >= 16)
                insn.setErrorIf(op, OVERFLOW_RANGE);
            insn.embed(static_cast<uint8_t>(op.val32 & 0xF));
            break;
        }
        if (mode == M_IMDSP) {
            if (overflowInt16(static_cast<int32_t>(op.val32)))
                insn.setErrorIf(op, OVERFLOW_RANGE);
            insn.emitOperand16(static_cast<uint16_t>(op.val32));
            break;
        }
        emitImmediateData(insn, op, size, op.val32);
        break;
    case M_LABEL:
        if (size == SZ_LONG || (size == SZ_BYTE && mode == M_REL16))
            insn.setErrorIf(op, ILLEGAL_SIZE);
        if (size == SZ_WORD && mode == M_REL8)
            mode = M_REL16;
        emitRelativeAddr(insn, mode, op);
        break;
    default:
        break;
    }
    return getError();
}

namespace {

uint16_t reverseBits(uint16_t bits) {
    bits = (bits & 0x5555) << 1 | (bits & 0xAAAA) >> 1;
    bits = (bits & 0x3333) << 2 | (bits & 0xCCCC) >> 2;
    bits = (bits & 0x0F0F) << 4 | (bits & 0xF0F0) >> 4;
    return bits << 8 | bits >> 8;
}

}  // namespace

void AsmMc68000::emitRegisterList(AsmInsn &insn, const Operand &op, bool reverse) const {
    auto p = op.list;
    uint16_t bits = 0;
    for (;;) {
        auto a = p;
        const auto start = parseRegName(a);
        if (!isGeneralReg(start))
            insn.setErrorIf(p, REGISTER_NOT_ALLOWED);
        const uint8_t s = encodeGeneralRegPos(start);
        uint8_t e = s;
        if (*a == '-') {
            ++a;
            p = a.skipSpaces();
            const auto last = parseRegName(a);
            if (!isGeneralReg(last))
                insn.setErrorIf(p, REGISTER_NOT_ALLOWED);
            e = encodeGeneralRegPos(last);
            if (e < s)
                insn.setErrorIf(UNKNOWN_OPERAND);
        }
        for (uint8_t i = s; i <= e; i++) {
            const auto bm = 1U << i;
            if (bits & bm)
                insn.setErrorIf(p, DUPLICATE_REGISTER);
            bits |= bm;
        }
        if (a.skipSpaces().expect('/')) {
            p = a;
            continue;
        } else if (*a == ',' || endOfLine(a)) {
            break;
        }
        insn.setErrorIf(a, UNKNOWN_OPERAND);
    }
    if (reverse)
        bits = reverseBits(bits);
    insn.emitOperand16(bits);
}

Error AsmMc68000::parseOperand(StrScanner &scan, Operand &op) const {
    auto p = scan.skipSpaces();
    op.setAt(op.list = p);
    if (endOfLine(p))
        return OK;
    if (p.expect('#')) {
        op.val32 = parseExpr32(p, op);
        if (op.hasError())
            return op.getError();
        op.mode = M_IMDAT;
        scan = p;
        return OK;
    }
    auto a = p;
    const bool pdec = (*a++ == '-' && *a == '(');
    if (pdec)
        p = a;
    if (p.expect('(')) {
        op.reg = parseRegName(p.skipSpaces());
        if (isAddrReg(op.reg)) {
            if (!p.skipSpaces().expect(')'))
                return op.setError(MISSING_CLOSING_PAREN);
            if (pdec) {
                op.mode = M_PDEC;
            } else if (p.expect('+')) {
                op.mode = M_PINC;
            } else {
                op.mode = M_AIND;
            }
            scan = p;
            return OK;
        }
        op.val32 = parseExpr32(p, op, ')');
        if (op.hasError())
            return op.getError();
        if (p.skipSpaces().expect(')')) {
            const auto size = parseSize(p.skipSpaces());
            bool over16 = overflowInt16(op.val32);
            if (over16) {
                // check if it is near the end of address space.
                const auto max = 1UL << uint8_t(addressWidth());
                if (op.val32 <= max - 1 && op.val32 >= (max - 0x8000))
                    over16 = false;
            }
            if (over16 && size == SZ_WORD)
                op.setErrorIf(OVERFLOW_RANGE);
            op.mode = (size == SZ_WORD || (size == SZ_NONE && !over16)) ? M_AWORD : M_ALONG;
            scan = p;
            return OK;
        }
        if (p.expect(',')) {
            a = p.skipSpaces();
            op.reg = parseRegName(a);
            if (!isAddrReg(op.reg) && op.reg != REG_PC)
                return op.setError(a, REGISTER_NOT_ALLOWED);
            if (a.skipSpaces().expect(')')) {
                op.mode = (op.reg == REG_PC) ? M_PCDSP : M_DISP;
                scan = a;
                return OK;
            }
            if (!a.expect(','))
                return op.setError(a, MISSING_COMMA);
            p = a.skipSpaces();
            op.indexReg = parseRegName(p);
            if (!isGeneralReg(op.indexReg))
                return op.setError(UNKNOWN_OPERAND);
            op.indexSize = parseSize(p);
            if (op.indexSize == SZ_ERROR)
                return op.setError(UNKNOWN_OPERAND);
            if (op.indexSize == SZ_NONE)
                op.indexSize = SZ_WORD;
            if (!p.skipSpaces().expect(')'))
                return op.setError(p, MISSING_CLOSING_PAREN);
            op.mode = (op.reg == REG_PC) ? M_PCIDX : M_INDX;
            scan = p;
            return OK;
        }
        return op.setError(UNKNOWN_OPERAND);
    }

    op.reg = parseRegName(a = p);
    if (op.reg != REG_UNDEF) {
        a.skipSpaces();
        if ((*a == '/' || *a == '-') && isGeneralReg(op.reg)) {
            while (*a != ',' && !endOfLine(a))
                ++a;
            op.mode = M_MULT;
            scan = a;
            return op.getError();
        }
        if (isAddrReg(op.reg)) {
            op.mode = M_AREG;
        } else if (isDataReg(op.reg)) {
            op.mode = M_DREG;
        } else if (op.reg == REG_USP) {
            op.mode = M_USP;
        } else if (op.reg == REG_CCR) {
            op.mode = M_CCR;
        } else if (op.reg == REG_SR) {
            op.mode = M_SR;
        } else
            return op.setError(p, REGISTER_NOT_ALLOWED);
        scan = a;
        return OK;
    }
    op.val32 = parseExpr32(p, op);
    if (op.hasError())
        return op.getError();
    op.mode = M_LABEL;
    scan = p;
    return OK;
}

OprSize AsmInsn::parseInsnSize() {
    StrScanner p(name());
    p.trimStart([](char c) { return c != '.'; });
    char *eos = const_cast<char *>(p.str());
    const auto isize = parseSize(p);
    *eos = 0;
    return isize;
}

Error AsmMc68000::encodeImpl(StrScanner &scan, Insn &_insn) {
    AsmInsn insn(_insn);
    const auto isize = insn.parseInsnSize();
    if (isize == SZ_ERROR)
        return setError(scan, ILLEGAL_SIZE);
    insn.setInsnSize(isize);

    if (parseOperand(scan, insn.srcOp) && insn.srcOp.hasError())
        return setError(insn.srcOp);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, insn.dstOp) && insn.dstOp.hasError())
            return setError(insn.dstOp);
        scan.skipSpaces();
    }

    if (setErrorIf(insn.srcOp, TABLE.searchName(cpuType(), insn)))
        return getError();

    insn.setErrorIf(insn.srcOp);
    insn.setErrorIf(insn.dstOp);
    const auto src = insn.src();
    const auto dst = insn.dst();
    if (src == M_MULT)
        emitRegisterList(insn, insn.srcOp, insn.dstOp.mode == M_PDEC);
    if (dst == M_MULT)
        emitRegisterList(insn, insn.dstOp);
    if (src == M_IMBIT) {
        auto bitno = insn.srcOp.val32;
        if (insn.srcOp.mode != M_IMDAT) {
            insn.setErrorIf(insn.srcOp, OPERAND_NOT_ALLOWED);
            bitno = 0;
        }
        if (insn.oprSize() == SZ_BYTE && bitno >= 8) {
            insn.setErrorIf(insn.srcOp, ILLEGAL_BIT_NUMBER);
            bitno &= 7;
        }
        if (insn.oprSize() == SZ_LONG && bitno >= 32) {
            insn.setErrorIf(insn.srcOp, ILLEGAL_BIT_NUMBER);
            bitno &= 0x1F;
        }
        insn.emitOperand16(bitno);
    }
    emitOprSize(insn, isize);
    insn.setInsnSize(isize);
    const auto osize = (isize == SZ_NONE) ? insn.oprSize() : isize;
    emitEffectiveAddr(insn, osize, insn.srcOp, src, insn.srcPos());
    emitEffectiveAddr(insn, osize, insn.dstOp, dst, insn.dstPos());
    insn.emitInsn();
    return setError(insn);
}

}  // namespace mc68000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
