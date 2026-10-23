/*
 * Copyright 2026 Tadashi G. Takaoka
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

#include "asm_c166.h"
#include "letter_parser.h"
#include "operators.h"
#include "table_c166.h"
#include "text_c166.h"
#include "text_common.h"

namespace libasm {
namespace c166 {

using namespace pseudo;
using namespace reg;
using namespace text::common;
using namespace text::c166;

namespace {

// clang-format off
constexpr Pseudo PSEUDOS[] PROGMEM = {
    { TEXT_dBYTE, &Assembler::defineDataConstant, Assembler::DATA_BYTE_NO_STRING },
    { TEXT_dWORD, &Assembler::defineDataConstant, Assembler::DATA_WORD_NO_STRING },
    { TEXT_DB, &Assembler::defineDataConstant, Assembler::DATA_BYTE },
    { TEXT_DS, &Assembler::allocateSpaces,     Assembler::DATA_BYTE },
    { TEXT_DW, &Assembler::defineDataConstant, Assembler::DATA_WORD_NO_STRING },
};
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

constexpr const char *DPP_DIRECTIVES[] PROGMEM = {
    TEXT_REG_DPP0,
    TEXT_REG_DPP1,
    TEXT_REG_DPP2,
    TEXT_REG_DPP3,
};
// clang-format on

constexpr uint8_t PAGE_BITS = 14;

bool isRange(AddrMode mode) {
    return mode == M_RNG || mode == M_RNGR;
}

// Remember the sequence an ATOMIC or EXTended instruction begins.
void beginSequence(AsmInsn &insn, ExtState &state) {
    const auto dst = insn.dst();
    const auto &range = isRange(dst) ? insn.dstOp : insn.srcOp;
    const auto mode = isRange(dst) ? dst : insn.src();
    if (!isRange(mode))
        return;
    state.count = (range.val.getUnsigned() - 1) % 4 + 1;
    state.esfr = mode == M_RNGR;
    const auto base = insn.dstOp.val.getUnsigned();
    if (dst == M_PAG) {
        state.kind = EXT_PAGE;
        state.base = base & 0x3FF;
    } else if (dst == M_SEG8) {
        state.kind = EXT_SEGMENT;
        state.base = base & 0xFF;
    } else {
        state.kind = dst == M_RWL ? EXT_UNKNOWN : EXT_NONE;
    }
}

// A letter or a string is enclosed in single or double quotes, either of
// which is doubled to include it.  Between double quotes a backslash begins a
// C escape sequence.
struct C166LetterParser final : LetterParser, Singleton<C166LetterParser> {
    char letterDelimiter(StrScanner &scan) const override { return delimiter(scan); }
    char stringDelimiter(StrScanner &scan) const override { return delimiter(scan); }
    char readLetter(StrScanner &scan, ErrorAt &error, char delim) const override {
        if (delim == '"' && *scan == '\\')
            return CStyleLetterParser::singleton().readLetter(scan, error, delim);
        return LetterParser::readLetter(scan, error, delim);
    }

private:
    static char delimiter(StrScanner &scan) {
        const auto c = scan.expect('\'');
        return c ? c : scan.expect('"');
    }
};

Error segmentOf(ValueStack &stack, ParserContext &) {
    stack.pushUnsigned((stack.pop().getUnsigned() >> 16) & UINT8_MAX);
    return OK;
}

Error pageOf(ValueStack &stack, ParserContext &) {
    stack.pushUnsigned((stack.pop().getUnsigned() >> PAGE_BITS) & 0x3FF);
    return OK;
}

Error segmentOffsetOf(ValueStack &stack, ParserContext &) {
    stack.pushUnsigned(stack.pop().getUnsigned() & UINT16_MAX);
    return OK;
}

Error pageOffsetOf(ValueStack &stack, ParserContext &) {
    stack.pushUnsigned(stack.pop().getUnsigned() & ((1 << PAGE_BITS) - 1));
    return OK;
}

// clang-format off
const Operator OP_SEG(3, Operator::RIGHT, 1, segmentOf);
const Operator OP_PAG(3, Operator::RIGHT, 1, pageOf);
const Operator OP_SOF(3, Operator::RIGHT, 1, segmentOffsetOf);
const Operator OP_POF(3, Operator::RIGHT, 1, pageOffsetOf);

constexpr char TEXT_SEG[] PROGMEM = "SEG";
constexpr char TEXT_PAG[] PROGMEM = "PAG";
constexpr char TEXT_SOF[] PROGMEM = "SOF";
constexpr char TEXT_POF[] PROGMEM = "POF";
// clang-format on

// The segment, page, segment offset and page offset of an address.
struct C166OperatorParser final : OperatorParser, Singleton<C166OperatorParser> {
    const Operator *readPrefix(
            StrScanner &scan, ValueStack &stack, ParserContext &context) const override {
        auto p = scan;
        p.trimStart(isalnum);
        const StrScanner name(scan.str(), p.str());
        const Operator *opr = nullptr;
        if (name.iequals_P(TEXT_SEG)) {
            opr = &OP_SEG;
        } else if (name.iequals_P(TEXT_PAG)) {
            opr = &OP_PAG;
        } else if (name.iequals_P(TEXT_SOF)) {
            opr = &OP_SOF;
        } else if (name.iequals_P(TEXT_POF)) {
            opr = &OP_POF;
        }
        if (opr) {
            scan = p;
            return opr;
        }
        return IntelOperatorParser::singleton().readPrefix(scan, stack, context);
    }
    const Operator *readInfix(
            StrScanner &scan, ValueStack &stack, ParserContext &context) const override {
        return IntelOperatorParser::singleton().readInfix(scan, stack, context);
    }
};

// A mnemonic may end with '+' or '-' (JMPA+, CoMAC-), and a "-USR1" prefix
// begins with '-'.
struct C166SymbolParser final : PrefixSymbolParser, Singleton<C166SymbolParser> {
    C166SymbolParser() : PrefixSymbolParser(nullptr, text::common::PSTR_UNDER_QUESTION) {}
    bool instructionLetter(char c) const override {
        return PrefixSymbolParser::instructionLetter(c) || c == '+' || c == '-' || c == '.';
    }
    // GNU as writes the location counter as '.'.
    bool locationSymbol(StrScanner &scan) const override {
        return SymbolParser::locationSymbol(scan, '$') || SymbolParser::locationSymbol(scan, '.');
    }
};

}  // namespace

const ValueParser::Plugins &AsmC166::defaultPlugins() {
    static const struct final : ValueParser::IntelPlugins {
        const SymbolParser &symbol() const override { return C166SymbolParser::singleton(); }
        const LetterParser &letter() const override { return C166LetterParser::singleton(); }
        const OperatorParser &operators() const override {
            return C166OperatorParser::singleton();
        }
    } PLUGINS{};
    return PLUGINS;
}

AsmC166::AsmC166(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE), Config(TABLE) {
    reset();
}

void AsmC166::reset() {
    Assembler::reset();
    for (auto i = 0; i < 4; i++)
        _dpp[i] = i;
}

Error AsmC166::processPseudo(StrScanner &scan, Insn &insn) {
    for (auto i = 0; i < 4; i++) {
        const auto *name_P = reinterpret_cast<const char *>(pgm_read_ptr(&DPP_DIRECTIVES[i]));
        if (strcasecmp_P(insn.name(), name_P) == 0)
            return processDpp(scan, insn, i);
    }
    return Assembler::processPseudo(scan, insn);
}

// `DPP2 <page>` sets the page DPP2 is assumed to hold; no operand makes it unknown.
Error AsmC166::processDpp(StrScanner &scan, Insn &insn, uint8_t num) {
    auto p = scan.skipSpaces();
    if (endOfLine(p)) {
        _dpp[num] = -1;
        return OK;
    }
    const auto at = p;
    ErrorAt error;
    const auto value = parseInteger(p, error);
    if (error.hasError())
        return insn.setError(error);
    if (value.overflow((1U << (addressWidth() - PAGE_BITS)) - 1))
        return insn.setError(at, OVERFLOW_RANGE);
    _dpp[num] = value.getUnsigned();
    scan = p;
    return OK;
}

// The [IDXi*] and [Rw*] operands of a CoXXX instruction which no other
// instruction takes: an IDX pointer, a post-decrement, or an update by an
// offset register.  The update code is the one of the instruction format.
bool AsmC166::parseCoIndirect(StrScanner &scan, Operand &op) const {
    auto p = scan;
    const auto regp = p.skipSpaces();
    auto reg = parseMacRegName(p, parser());
    const auto idx = reg == REG_IDX0 || reg == REG_IDX1;
    if (!idx) {
        p = regp;
        reg = parseRegName(p, parser());
        if (!isWordReg(reg))
            return false;
    }
    uint8_t update = 1;
    auto q = p;
    const auto sign = q.skipSpaces().expect('+') ? '+' : (q.expect('-') ? '-' : 0);
    if (sign) {
        update = sign == '+' ? 2 : 3;
        auto r = q;
        const auto offset = parseMacRegName(r.skipSpaces(), parser());
        const auto q0 = idx ? REG_QX0 : REG_QR0;
        if (offset == q0 || offset == q0 + 1) {
            update = (sign == '+' ? 4 : 5) + 2 * (offset - q0);
            q = r;
        } else if (!idx && sign == '+') {
            return false;  // [Rw+] and [Rw+#data16]
        }
        p = q;
    } else if (!idx) {
        return false;  // [Rw]
    }
    if (!p.skipSpaces().expect(']')) {
        op.setError(p, MISSING_CLOSING_BRACKET);
        scan = p;
        return true;
    }
    op.reg = reg;
    op.update = idx ? (reg == REG_IDX1 ? 8 : 0) | update : update;
    op.mode = idx ? M_QIX : M_QIRL;
    scan = p;
    return true;
}

Error AsmC166::parseIndirect(StrScanner &scan, Operand &op) const {
    auto p = scan;
    if (cpuType() == XC16X && parseCoIndirect(p, op)) {
        if (op.hasError())
            return op.getError();
        scan = p;
        return OK;
    }
    const auto predec = p.skipSpaces().expect('-');
    const auto regp = p.skipSpaces();
    op.reg = parseRegName(p, parser());
    if (!isWordReg(op.reg))
        return op.setError(regp, UNKNOWN_OPERAND);
    if (predec) {
        op.mode = M_PDL;
    } else if (p.skipSpaces().expect('+')) {
        // The '#' of [Rw+#data16] may be omitted.
        if (p.skipSpaces().expect('#') || *p != ']') {
            op.val = parseInteger(p, op);
            if (op.hasError())
                return op.getError();
            op.mode = M_IXL;
        } else {
            op.mode = M_PINC;
            op.update = 2;
        }
    } else {
        op.mode = M_IDIR;
        op.update = 1;
    }
    if (!p.skipSpaces().expect(']'))
        return op.setError(p, MISSING_CLOSING_BRACKET);
    scan = p;
    return OK;
}

Error AsmC166::parseOperand(StrScanner &scan, Operand &op, bool first) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    if (p.expect('#')) {
        op.val = parseInteger(p, op);
        if (op.hasError())
            return op.getError();
        op.mode = M_IMM;
        scan = p;
        return OK;
    }

    if (p.expect('[')) {
        if (parseIndirect(p, op))
            return op.getError();
        scan = p;
        return OK;
    }

    // A condition code is always the first operand.
    if (first) {
        op.cc = parseCcName(p, parser());
        if (op.cc != CC_UNDEF) {
            op.mode = isXccOnly(op.cc) ? M_XCC : M_CC;
            scan = p;
            return OK;
        }
    }

    if (cpuType() == XC16X) {
        if (p.iexpectWord_P(TEXT_RND)) {
            op.mode = M_RND;
            scan = p;
            return OK;
        }
        const auto regp = p;
        op.reg = parseMacRegName(p, parser());
        if (isCoReg(op.reg)) {
            op.mode = M_COREG;
            scan = p;
            return OK;
        }
        if (op.reg != REG_UNDEF)
            return op.setError(regp, ILLEGAL_REGISTER);
    }

    op.reg = parseRegName(p, parser());
    if (isByteReg(op.reg)) {
        op.mode = M_RB;
        scan = p;
        return OK;
    }
    if (op.reg == REG_UNDEF) {
        // A core SFR name stands for its address.
        const auto addr = parseSfrName(p, parser(), cpuType());
        if (addr) {
            op.val.setUnsigned(addr);
        } else {
            op.val = parseInteger(p, op, '.');
            if (op.hasError())
                return op.getError();
        }
    }
    if (p.expect('.')) {
        const auto bitp = p;
        ErrorAt bitOp;
        op.bitpos = parseInteger(p, bitOp);
        if (bitOp.hasError())
            return op.setError(bitOp);
        if (op.bitpos.overflow(15))
            return op.setError(bitp, ILLEGAL_BIT_NUMBER);
        op.mode = M_BIT;
    } else {
        op.mode = isWordReg(op.reg) ? M_RW : M_ADDR;
    }
    scan = p;
    return OK;
}

void AsmC166::encodeShortReg(AsmInsn &insn, const Operand &op) const {
    if (op.mode == M_RW || op.mode == M_RB) {
        insn.embedPost(0xF0 | encodeRegNum(op.reg));
        return;
    }
    const auto num = shortReg(insn, op.val.getUnsigned());
    if (num < 0) {
        insn.setErrorIf(op, OVERFLOW_RANGE);
    } else {
        insn.embedPost(num);
    }
}

uint8_t AsmC166::encodeBitOff(AsmInsn &insn, const Operand &op) const {
    if (isWordReg(op.reg))
        return 0xF0 | encodeRegNum(op.reg);
    const auto num = bitOff(insn, op.val.getUnsigned());
    if (num < 0) {
        insn.setErrorIf(op, NOT_BIT_ADDRESSABLE);
        return 0;
    }
    return num;
}

uint8_t AsmC166::encodeRelative(AsmInsn &insn, const Operand &op) const {
    // The instruction pointer wraps within the code segment.
    const auto base = insn.address() + insn.insnLength();
    const auto target = op.getError() ? base : op.val.getUnsigned();
    insn.setErrorIf(op, checkAddr(target));
    if ((target & ~UINT32_C(0xFFFF)) != (insn.address() & ~UINT32_C(0xFFFF)))
        insn.setErrorIf(op, OPERAND_TOO_FAR);
    const auto delta = static_cast<int16_t>((target - base) & 0xFFFF);
    if (delta & 1)
        insn.setErrorIf(op, OPERAND_NOT_ALIGNED);
    if (delta < -256 || delta > 254)
        insn.setErrorIf(op, OPERAND_TOO_FAR);
    return delta / 2;
}

void AsmC166::encodeCodeAddress(AsmInsn &insn, const Operand &op) const {
    const auto target = op.val.getUnsigned();
    if (insn.dst() == M_SEG) {
        // A physical target has to be in the segment.
        if (op.val.overflow(UINT16_MAX) && (target >> 16) != insn.dstOp.val.getUnsigned())
            insn.setErrorIf(op, OVERFLOW_RANGE);
    } else if (!op.val.isUndefined()) {
        // A target in the code segment of the instruction.
        insn.setErrorIf(op, checkAddr(target));
        if ((target & ~UINT32_C(0xFFFF)) != (insn.address() & ~UINT32_C(0xFFFF)))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
        // The pre-fetch hint of JMPA marks a short backward loop.
        const auto back = insn.address() - target;
        if (insn.cf() == CF_FA && back > 0 && back <= 32)
            insn.embedPost(2);
    }
    insn.setWord2(target);
}

// A mem operand up to 0FFFFH is the address field itself, a larger one is a physical address, which
// must be in the page or segment of the EXTended sequence it is in, or else in the page of a DPP.
void AsmC166::encodeMemAddress(AsmInsn &insn, const Operand &op) const {
    const auto addr = op.val.getUnsigned();
    if (op.val.isUndefined() || addr <= UINT16_MAX) {
        if (op.val.overflow(UINT16_MAX))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.setWord2(addr);
        return;
    }
    insn.setErrorIf(op, checkAddr(addr));
    const auto page = addr >> PAGE_BITS;
    const auto offset = addr & ((1U << PAGE_BITS) - 1);
    switch (insn.extKind()) {
    case EXT_PAGE:
        if (page != insn.extBase())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.setWord2(offset);
        return;
    case EXT_SEGMENT:
        if ((addr >> 16) != insn.extBase())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.setWord2(addr);
        return;
    case EXT_UNKNOWN:
        insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.setWord2(addr);
        return;
    default:
        break;
    }
    for (auto i = 0; i < 4; i++) {
        if (_dpp[i] >= 0 && static_cast<uint32_t>(_dpp[i]) == page) {
            insn.setWord2((i << PAGE_BITS) | offset);
            return;
        }
    }
    insn.setErrorIf(op, OVERFLOW_RANGE);
    insn.setWord2(addr);
}

void AsmC166::encodeImmediate(AsmInsn &insn, AddrMode mode, const Operand &op) const {
    const auto val = op.val.getUnsigned();
    switch (mode) {
    case M_D3:
        insn.embedPost(val);
        break;
    case M_D4:
        insn.embedPost(val << 4);
        break;
    case M_TRAP:
        if (op.val.overflow(0x7F))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.embedPost((val & 0x7F) << 1);
        break;
    case M_IM16:
        if (op.val.overflowUint16())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.setWord2(val);
        break;
    case M_IM8:
        if (op.val.overflowUint8())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.embedByte3(val);
        break;
    case M_RNG:
    case M_RNGR:
        if (op.val.overflow(4, 1))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.embedPost(((val - 1) & 3) << 4);
        break;
    case M_PAG:
        if (op.val.overflow(0x3FF))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.setWord2(val & 0x3FF);
        break;
    case M_SEG8:
        if (op.val.overflowUint8())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.embedByte3(val);
        break;
    default:
        break;
    }
}

void AsmC166::encodeOperand(AsmInsn &insn, AddrMode mode, const Operand &op) const {
    insn.setErrorIf(op);
    const auto num = encodeRegNum(op.reg);
    switch (mode) {
    case M_RWH:
    case M_RBH:
    case M_IRH:
    case M_PIH:
        insn.embedPost(num << 4);
        break;
    case M_RWL:
    case M_RBL:
    case M_IRL:
    case M_PIL:
    case M_PDL:
    case M_IR2:
    case M_PI2:
        insn.embedPost(num);
        break;
    case M_IXL:
        insn.embedPost(num);
        if (op.val.overflowUint16())
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.setWord2(op.val.getUnsigned());
        break;
    case M_REGW:
    case M_REGB:
        encodeShortReg(insn, op);
        break;
    case M_CC:
        insn.embedPost(encodeCcNum(op.cc) << 4);
        break;
    case M_CCO:
        insn.embed(encodeCcNum(op.cc) << 4);
        break;
    case M_XCC:
        insn.embedPost(encodeXcc(op.cc) << 3);
        break;
    case M_QIX:
        insn.embedPost(op.update << 4);
        break;
    case M_QIRL:
        insn.embedPost(num);
        insn.embedByte4(op.update);
        break;
    case M_QNOP:
        insn.embedByte4(1);
        break;
    case M_D5:
        insn.embedByte4(op.val.getUnsigned());
        break;
    case M_COREG:
        insn.embedByte3(encodeCoReg(op.reg) << 3);
        break;
    case M_RND:
        break;
    case M_BITO:
        insn.embedPost(encodeBitOff(insn, op));
        insn.embed(op.bitpos.getUnsigned() << 4);
        break;
    case M_BOFF:
        insn.embedPost(encodeBitOff(insn, op));
        break;
    case M_BITQ:
        insn.embedPost(encodeBitOff(insn, op));
        insn.embedByte4(op.bitpos.getUnsigned() << 4);
        break;
    case M_BITZ:
        insn.embedByte3(encodeBitOff(insn, op));
        insn.embedByte4(op.bitpos.getUnsigned());
        break;
    case M_REL:
        insn.embedPost(encodeRelative(insn, op));
        break;
    case M_RELW:
        insn.embedByte3(encodeRelative(insn, op));
        break;
    case M_SEG:
        if (op.val.overflow((1 << (addressWidth() - 16)) - 1))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.embedPost(op.val.getUnsigned());
        break;
    case M_MEM:
        encodeMemAddress(insn, op);
        break;
    case M_CADR:
        encodeCodeAddress(insn, op);
        break;
    case M_MDL:
    case M_MDH:
        break;
    default:
        encodeImmediate(insn, mode, op);
        break;
    }
}

// A "- USR0" or "- USR1" prefix, which a CoXXX instruction follows.
Error AsmC166::parseRepeat(StrScanner &scan, AsmInsn &insn) const {
    if (cpuType() != XC16X)
        return insn.setError(scan, UNKNOWN_INSTRUCTION);
    StrScanner name{insn.name()};
    name.expect('-');
    auto p = scan;
    if (name.size() == 0)
        name = parser().readRegName(p);
    uint8_t rrr = 0;
    if (name.iequals_P(TEXT_CC_USR0)) {
        rrr = 2;
    } else if (name.iequals_P(TEXT_CC_USR1)) {
        rrr = 3;
    } else {
        return insn.setError(scan, UNKNOWN_INSTRUCTION);
    }
    StrScanner mnemonic;
    const auto at = p.skipSpaces();
    if (parser().readInstruction(p, mnemonic) || *mnemonic == '-')
        return insn.setError(at, UNKNOWN_INSTRUCTION);
    insn.nameBuffer().reset().text(mnemonic);
    insn.setRepeat(rrr);
    scan = p;
    return OK;
}

Error AsmC166::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    if (*insn.name() == '-' && parseRepeat(scan, insn))
        return _insn.setError(insn);
    auto &state = _insn.state<ExtState>();
    if (state.count) {
        state.count--;
        insn.setEsfr(state.esfr);
        insn.setExt(state.kind, state.base);
    }

    if (parseOperand(scan, insn.dstOp, true) && insn.dstOp.hasError())
        return _insn.setError(insn.dstOp);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, insn.srcOp, false) && insn.srcOp.hasError())
            return _insn.setError(insn.srcOp);
        scan.skipSpaces();
    }
    if (scan.expect(',')) {
        if (parseOperand(scan, insn.extOp, false) && insn.extOp.hasError())
            return _insn.setError(insn.extOp);
        scan.skipSpaces();
    }
    if (!endOfLine(scan))
        return _insn.setError(scan, GARBAGE_AT_END);

    if (searchName(cpuType(), insn))
        return _insn.setError(insn.dstOp, insn);

    if (insn.repeat() && !insn.rrr())
        return _insn.setError(insn.dstOp, OPERAND_NOT_ALLOWED);
    if (insn.flags().hasSub())
        insn.embedByte3(insn.flags().sub());
    if (insn.rrr())
        insn.embedByte4(insn.repeat() << 5);
    encodeOperand(insn, insn.dst(), insn.dstOp);
    const auto src = insn.src();
    encodeOperand(insn, src, insn.srcOp);
    encodeOperand(insn, insn.ext(), insn.extOp);
    if (src == M_MDL || src == M_MDH) {
        // BFLDL takes #mask8 then #data8; BFLDH puts #data8 first.
        const auto &first = src == M_MDL ? insn.srcOp : insn.extOp;
        const auto &second = src == M_MDL ? insn.extOp : insn.srcOp;
        insn.setErrorIf(insn.extOp);
        if (first.val.overflowUint8())
            insn.setErrorIf(first, OVERFLOW_RANGE);
        if (second.val.overflowUint8())
            insn.setErrorIf(second, OVERFLOW_RANGE);
        insn.embedByte3(first.val.getUnsigned());
        insn.embedByte4(second.val.getUnsigned());
    }
    if (insn.cf() == CF_NN) {
        // The register is in either nibble; repeat it in the other.
        const auto num = (insn.post() >> 4) | (insn.post() & 0xF);
        insn.setPost(num | (num << 4));
    }
    if (insn.cf() == CF_PD)
        insn.setWord2(insn.opCode() | (static_cast<uint16_t>(insn.opCode()) << 8));
    beginSequence(insn, state);
    insn.emitInsn();
    return _insn.setError(insn);
}

}  // namespace c166
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
