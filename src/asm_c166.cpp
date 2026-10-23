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
#include "table_c166.h"
#include "text_common.h"

namespace libasm {
namespace c166 {

using namespace pseudo;
using namespace reg;
using namespace text::common;

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
// clang-format on

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

struct C166SymbolParser final : PrefixSymbolParser, Singleton<C166SymbolParser> {
    C166SymbolParser() : PrefixSymbolParser(nullptr, text::common::PSTR_UNDER_QUESTION) {}
    bool instructionLetter(char c) const override {
        return PrefixSymbolParser::instructionLetter(c) || c == '.';
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
    } PLUGINS{};
    return PLUGINS;
}

AsmC166::AsmC166(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE), Config(TABLE) {
    reset();
}

Error AsmC166::parseIndirect(StrScanner &scan, Operand &op) const {
    auto p = scan;
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
            op.mode = M_PI;
        }
    } else {
        op.mode = M_IR;
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
            op.mode = M_CC;
            scan = p;
            return OK;
        }
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
        op.bit = parseInteger(p, bitOp);
        if (bitOp.hasError())
            return op.setError(bitOp);
        if (op.bit.overflow(15))
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
    const auto num = shortReg(op.val.getUnsigned());
    if (num < 0) {
        insn.setErrorIf(op, OVERFLOW_RANGE);
    } else {
        insn.embedPost(num);
    }
}

uint8_t AsmC166::encodeBitOff(AsmInsn &insn, const Operand &op) const {
    if (isWordReg(op.reg))
        return 0xF0 | encodeRegNum(op.reg);
    const auto num = bitOff(op.val.getUnsigned());
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
        if (op.val.overflow(UINT16_MAX))
            insn.setErrorIf(op, OVERFLOW_RANGE);
    } else if (!op.val.isUndefined()) {
        // A target in the code segment of the instruction.
        insn.setErrorIf(op, checkAddr(target));
        if ((target & ~UINT32_C(0xFFFF)) != (insn.address() & ~UINT32_C(0xFFFF)))
            insn.setErrorIf(op, OPERAND_TOO_FAR);
    }
    insn.setWord2(target);
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
    case M_BITO:
        insn.embedPost(encodeBitOff(insn, op));
        insn.embed(op.bit.getUnsigned() << 4);
        break;
    case M_BOFF:
        insn.embedPost(encodeBitOff(insn, op));
        break;
    case M_BITQ:
        insn.embedPost(encodeBitOff(insn, op));
        insn.embedByte4(op.bit.getUnsigned() << 4);
        break;
    case M_BITZ:
        insn.embedByte3(encodeBitOff(insn, op));
        insn.embedByte4(op.bit.getUnsigned());
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
        if (op.val.overflow(UINT16_MAX))
            insn.setErrorIf(op, OVERFLOW_RANGE);
        insn.setWord2(op.val.getUnsigned());
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

Error AsmC166::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
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

    if (searchName(cpuType(), insn))
        return _insn.setError(insn.dstOp, insn);

    encodeOperand(insn, insn.dst(), insn.dstOp);
    const auto src = insn.src();
    encodeOperand(insn, src, insn.srcOp);
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
    if (insn.cf() == CF_NN)
        insn.embedPost(insn.post() >> 4);
    if (insn.cf() == CF_PD)
        insn.setWord2(insn.opCode() | (static_cast<uint16_t>(insn.opCode()) << 8));
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
