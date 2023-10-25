/*
 * Copyright 2023 Tadashi G. Takaoka
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

#include "asm_im6100.h"

#include "reg_im6100.h"
#include "table_im6100.h"
#include "text_common.h"

namespace libasm {
namespace im6100 {

using namespace pseudo;
using namespace reg;
using namespace text::common;

namespace {

// clang-format off
constexpr char TEXT_star[]    PROGMEM = "*";
constexpr char TEXT_OCTAL[]   PROGMEM = "OCTAL";
constexpr char TEXT_DECIMAL[] PROGMEM = "DECIMAL";
constexpr char TEXT_FIELD[]   PROGMEM = "FIELD";
constexpr char TEXT_PAGE[]    PROGMEM = "PAGE";
constexpr char TEXT_DUBL[]    PROGMEM = "DUBL";

constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_star, &Assembler::defineOrigin},
    {TEXT_TEXT, &Assembler::defineString, Assembler::STR_DEC_6BIT},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

struct RadixNumberParser final : NumberParser {
    RadixNumberParser() : _radix(RADIX_8) {}
    Radix radix() const { return _radix; }
    void setRadix(Radix radix) { _radix = radix; }
    Error parseNumber(StrScanner &scan, Value &val) const {
        auto p = scan;
        const auto radix = IntelNumberParser::hasSuffix(p);
        if (radix != RADIX_NONE)
            return IntelNumberParser::singleton().parseNumber(scan, val, radix, p);
        return val.parseNumber(scan, _radix);
    }

private:
    Radix _radix;
} NUMBER_PARSER;

struct DecSymbolParser final : SymbolParser {
    bool labelDelimitor(StrScanner &scan) const override { return scan.expect(','); }
    bool instructionLetter(char c) const override {
        return SymbolParser::symbolLetter(c) || c == '*' || c == '=';
    }
    bool instructionTerminator(char c) const override { return c == '*' || c == '='; }
};

struct DecLetterParser final : LetterParser {
    Error parseLetter(StrScanner &scan, char &letter) const override {
        auto p = scan;
        if (!p.expect('"'))
            return NOT_AN_EXPECTED;
        ErrorAt error;
        letter = readLetter(p, error);
        if (error.isOK())
            scan = p;
        return error.getError();
    }

    char readLetter(StrScanner &scan, ErrorAt &error) const override {
        const auto c = *scan;
        if (c == 0 || (c >= 0x60 && c < 0x7F)) {
            error.setErrorIf(ILLEGAL_CONSTANT);  // no lowercase letter
            return 0;
        }
        ++scan;
        return c + 0x80;  // convert to DEC 8-Bit form.
    }
};

struct DecCommentParser final : CommentParser {
    bool endOfLine(const StrScanner &scan) const override {
        return *scan == 0 || *scan == '/' || *scan == ';';
    };
};

}  // namespace

const ValueParser::Plugins &AsmIm6100::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return NUMBER_PARSER; }
        const SymbolParser &symbol() const override { return _symbol; }
        const LetterParser &letter() const override { return _letter; }
        const CommentParser &comment() const override { return _comment; }
        const OperatorParser &operators() const override { return DecOperatorParser::singleton(); }
        const LocationParser &location() const override { return _location; }
        const DecSymbolParser _symbol{};
        const DecLetterParser _letter{};
        const DecCommentParser _comment{};
        const SimpleLocationParser _location{PSTR_DOT_DOLLAR};
    } PLUGINS{};
    return PLUGINS;
}

AsmIm6100::AsmIm6100(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE), Config(TABLE) {
    reset();
}

void AsmIm6100::reset() {
    Assembler::reset();
    setListRadix(RADIX_8);
}

Error AsmIm6100::setInputRadix(StrScanner &scan, Insn &insn, uint8_t extra) {
    UNUSED(scan);
    UNUSED(insn);
    NUMBER_PARSER.setRadix(static_cast<Radix>(extra));
    return OK;
}

Error AsmIm6100::defineDoubleDecimal(StrScanner &scan, Insn &insn, uint8_t extra) {
    UNUSED(extra);
    const auto save = NUMBER_PARSER.radix();
    NUMBER_PARSER.setRadix(RADIX_10);
    const auto val24 = parseExpr(scan, *this).getUnsigned();
    if (isOK()) {
        setErrorIf(insn.emitUint16Be((val24 >> 12) & 07777));
        setErrorIf(insn.emitUint16Be(val24 & 07777));
    }
    NUMBER_PARSER.setRadix(save);
    return getError();
}

Error AsmIm6100::alignOnPage(StrScanner &scan, Insn &insn, uint8_t extra) {
    UNUSED(extra);
    auto p = scan.skipSpaces();
    auto page = pageOf(insn.address());
    if (endOfLine(p)) {
        page++;
    } else {
        page = parseExpr(p, *this).getUnsigned();
        if (getError())
            return getError();
    }
    const auto field = fieldOf(insn.address());
    const auto addr = memoryAddress(0, page, field);
    const auto error = setCurrentLocation(addr);
    if (error)
        return setError(scan, error);
    insn.reset(addr);
    return OK;
}

Error AsmIm6100::defineField(StrScanner &scan, Insn &insn, uint8_t extra) {
    UNUSED(extra);
    auto p = scan.skipSpaces();
    const auto field = parseExpr(p, *this).getUnsigned();
    if (getError())
        return setError(scan, getError());
    const auto addr = memoryAddress(0, 0, field);
    const auto error = setCurrentLocation(addr);
    if (error)
        return setError(scan, error);
    insn.reset(addr);
    return OK;
}

Error AsmIm6100::parseMemReferenceOperand(StrScanner &scan, AsmInsn &insn) const {
    auto p = scan;
    const auto indir = p.iexpect('i') && p.expect(' ');
    if (!indir)
        p = scan;

    const auto at = p;
    const auto addr = parseExpr(p, insn).getUnsigned();
    if (hasError())
        return getError();

    if (indir)
        insn.embed(0400);
    if (pageOf(addr) == 0) {
        insn.embed(offsetOf(addr));
    } else if (pageOf(insn.address()) == pageOf(addr)) {
        insn.embed(0200 | offsetOf(addr));
    } else {
        return insn.setError(at, OPERAND_TOO_FAR);
    }
    scan = p;
    return OK;
}

Error AsmIm6100::parseIoTransferOperand(StrScanner &scan, AsmInsn &insn) const {
    auto p = scan;
    const auto addr = parseExpr(p, insn).getUnsigned();
    if (hasError())
        return getError();
    const auto pcont = p.skipSpaces();
    if (endOfLine(p))
        return insn.setError(p, MISSING_OPERAND);
    const auto control = parseExpr(p, insn).getUnsigned();
    if (hasError())
        return getError();

    if (overflowUint(addr, 6))
        return insn.setErrorIf(scan, OVERFLOW_RANGE);
    if (overflowUint(control, 3))
        return insn.setErrorIf(pcont, OVERFLOW_RANGE);
    insn.embed((addr << 3) | control);
    scan = p;
    return OK;
}

Error AsmIm6100::encodeMicro(AsmInsn &insn, const AsmInsn &micro, Config::opcode_t &done) const {
    const auto bits = micro.bits();
    if (done & bits)
        return INVALID_INSTRUCTION;
    const auto selector = micro.selector();
    if ((insn.opCode() & selector) != (micro.opCode() & selector))
        return INVALID_INSTRUCTION;
    done |= bits;
    insn.embed(micro.opCode());
    return OK;
}

Error AsmIm6100::parseField(StrScanner &scan, AsmInsn &insn) const {
    auto p = scan;
    auto field = parseExpr(p, insn).getUnsigned();
    if (!hasError()) {
        if (field < 8) {
            field <<= 3;
        } else if (field & ~070) {
            insn.setErrorIf(scan, ILLEGAL_OPERAND);
            field &= ~070;
        }
        insn.embed(field);
        scan = p;
    }
    return getError();
}

Error AsmIm6100::parseMemExtensionOperand(StrScanner &scan, AsmInsn &insn) const {
    Insn _micro{0};
    AsmInsn micro{_micro};
    Config::opcode_t done = insn.opCode() & ~insn.bits();
    while (!endOfLine(scan.skipSpaces())) {
        auto p = scan;
        StrScanner name;
        if (_parser.readInstruction(p, name) != OK)
            return parseField(scan, insn);
        micro.nameBuffer().reset().text(name);
        if (TABLE.searchName(cpuType(), micro) != OK)
            return parseField(scan, insn);
        if (TABLE.searchMicro(cpuType(), micro, M_MEX) != OK)
            return parseField(scan, insn);
        const auto error = encodeMicro(insn, micro, done);
        if (error)
            insn.setErrorIf(scan, error);
        scan = p;
    }
    return insn.setError(MISSING_OPERAND);
}

Error AsmIm6100::parseOperateOperand(StrScanner &scan, AsmInsn &insn) const {
    Insn _micro{0};
    AsmInsn micro{_micro};
    auto mode = insn.multiGroup() ? M_NONE : insn.mode();
    Config::opcode_t done = insn.bits();
    auto p = scan;
    while (!endOfLine(p.skipSpaces())) {
        const auto at = p;
        StrScanner name;
        if (_parser.readInstruction(p, name) != OK)
            return insn.setError(at, UNKNOWN_INSTRUCTION);
        micro.nameBuffer().reset().text(name);
        const auto error = TABLE.searchName(cpuType(), micro);
        if (error == UNKNOWN_INSTRUCTION) {
            insn.setErrorIf(at, error);
            continue;
        }
        if (mode != M_NONE) {
            TABLE.searchMicro(cpuType(), micro, mode);
            if (micro.isOK()) {
                const auto error = encodeMicro(insn, micro, done);
                if (error)
                    insn.setErrorIf(at, error);
            } else if (micro.getError() == OPERAND_NOT_ALLOWED) {
                insn.setErrorIf(at, INVALID_INSTRUCTION);
            } else {
                insn.setErrorIf(at, micro.getError());
            }
            continue;
        }
        auto m = M_GR1;
        for (;;) {
            TABLE.searchMicro(cpuType(), micro, m);
            if (micro.isOK()) {
                mode = m;
                const auto error = encodeMicro(insn, micro, done);
                if (error)
                    insn.setErrorIf(at, error);
                break;
            }
            if (m == M_GR3) {
                insn.setErrorIf(at, UNKNOWN_INSTRUCTION);
                break;
            }
            m = AddrMode(m + 1);
        }
    }
    scan = p;
    return OK;
}

Error AsmIm6100::processPseudo(StrScanner &scan, Insn &insn) {
    if (strcasecmp_P(insn.name(), TEXT_OCTAL) == 0)
        return setInputRadix(scan, insn, RADIX_8);
    if (strcasecmp_P(insn.name(), TEXT_DECIMAL) == 0)
        return setInputRadix(scan, insn, RADIX_10);
    if (strcasecmp_P(insn.name(), TEXT_DUBL) == 0)
        return defineDoubleDecimal(scan, insn);
    if (strcasecmp_P(insn.name(), TEXT_PAGE) == 0)
        return alignOnPage(scan, insn);
    if (cpuType() == HD6120 && strcasecmp_P(insn.name(), TEXT_FIELD) == 0)
        return defineField(scan, insn);
    return Assembler::processPseudo(scan, insn);
}

Error AsmIm6100::encodeImpl(StrScanner &scan, Insn &_insn) {
    AsmInsn insn(_insn);
    scan.skipSpaces();
    const auto error = TABLE.searchName(cpuType(), insn);
    if (error == OK) {
        const auto mode = insn.mode();
        if (mode == M_MEM) {
            parseMemReferenceOperand(scan, insn);
        } else if (mode == M_GR1 || mode == M_GR2 || mode == M_GR3) {
            parseOperateOperand(scan, insn);
        } else if (mode == M_IOT) {
            parseIoTransferOperand(scan, insn);
        } else if (mode == M_MEX) {
            parseMemExtensionOperand(scan, insn);
        }
    } else {
        insn.setOK();  // clear UNKNOWN_INSTRUCTION
        StrScanner p{errorAt()};
        const auto val12 = parseExpr(p, insn).getUnsigned();
        if (insn.hasError())
            return setError(insn);
        insn.setOpCode(val12 & 07777);
        scan = p;
    }
    if (!endOfLine(scan))
        setErrorIf(scan, GARBAGE_AT_END);

    insn.emitInsn();
    return setError(insn);
}

}  // namespace im6100
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
