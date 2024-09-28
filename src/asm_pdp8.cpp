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

#include "asm_pdp8.h"

#include "reg_pdp8.h"
#include "table_pdp8.h"
#include "text_common.h"

namespace libasm {
namespace pdp8 {

using namespace pseudo;
using namespace reg;
using namespace text::common;
using namespace text::option;

namespace {

// clang-format off
constexpr char TEXT_OCTAL[]   PROGMEM = "OCTAL";
constexpr char TEXT_DECIMAL[] PROGMEM = "DECIMAL";
constexpr char TEXT_FIELD[]   PROGMEM = "FIELD";
constexpr char TEXT_PAGE[]    PROGMEM = "PAGE";
constexpr char TEXT_DUBL[]    PROGMEM = "DUBL";

constexpr Pseudo PSEUDOS[] PROGMEM = {
    {PSTR_STAR, &Assembler::defineOrigin},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

}  // namespace

const ValueParser::Plugins &AsmPdp8::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return IntelNumberParser::singleton(); }
        const SymbolParser &symbol() const override { return Pdp8SymbolParser::singleton(); }
        const LetterParser &letter() const override { return Pdp8LetterParser::singleton(); }
        const CommentParser &comment() const override { return Pdp8CommentParser::singleton(); }
        const OperatorParser &operators() const override { return Pdp8OperatorParser::singleton(); }
    } PLUGINS{};
    return PLUGINS;
}

AsmPdp8::AsmPdp8(const ValueParser::Plugins &plugins)
    : Assembler(plugins, PSEUDO_TABLE, &_opt_implicitWord),
      Config(TABLE),
      _opt_implicitWord(
              this, &AsmPdp8::setImplicitWord, OPT_BOOL_IMPLICIT_WORD, OPT_DESC_IMPLICIT_WORD) {
    reset();
}

void AsmPdp8::reset() {
    Assembler::reset();
    setInputRadix(RADIX_8);
    setListRadix(RADIX_8);
    setImplicitWord(false);
}

Error AsmPdp8::setImplicitWord(bool enable) {
    _implicitWord = enable;
    return OK;
}

Error AsmPdp8::defineDoubleDecimal(StrScanner &scan, Insn &insn, uint8_t extra) {
    UNUSED(extra);
    const auto radix = _inputRadix;
    setInputRadix(RADIX_10);
    const auto save = scan;
    const auto val = parseInteger(scan, insn);
    constexpr auto UINT24_MAX = INT32_C(0x00FF'FFFF);
    constexpr auto INT24_MIN = INT32_C(-0x0080'0000);
    if (val.overflow(UINT24_MAX, INT24_MIN))
        insn.setErrorIf(save, OVERFLOW_RANGE);
    if (!insn.hasError()) {
        const auto val24 = val.getUnsigned();
        insn.setErrorIf(insn.emitUint16Be((val24 >> 12) & 07777));
        insn.setErrorIf(insn.emitUint16Be(val24 & 07777));
    }
    setInputRadix(radix);
    return insn.getError();
}

Error AsmPdp8::alignOnPage(StrScanner &scan, Insn &insn, uint8_t extra) {
    UNUSED(extra);
    auto p = scan.skipSpaces();
    auto page = pageOf(insn.address());
    if (endOfLine(p)) {
        page++;
    } else {
        page = parseInteger(p, insn).getUnsigned();
        if (insn.getError())
            return insn.getError();
    }
    const auto field = fieldOf(insn.address());
    const auto addr = memoryAddress(0, page, field);
    if (insn.setErrorIf(scan, setCurrentLocation(addr)))
        return insn.getError();
    scan = p;
    insn.reset(addr);
    return OK;
}

Error AsmPdp8::defineField(StrScanner &scan, Insn &insn, uint8_t extra) {
    UNUSED(extra);
    auto p = scan.skipSpaces();
    const auto field = parseInteger(p, insn).getUnsigned();
    if (insn.getError())
        return insn.getError();
    const auto addr = memoryAddress(0, 0, field);
    if (insn.setErrorIf(scan, setCurrentLocation(addr)))
        return insn.getError();
    scan = p;
    insn.reset(addr);
    return OK;
}

Error AsmPdp8::parseMemReferenceOperand(StrScanner &scan, AsmInsn &insn) const {
    auto p = scan;
    const auto indir = p.iexpect('i') && p.expect(' ');
    if (!indir)
        p = scan;

    const auto at = p;
    const auto addr = parseInteger(p, insn).getUnsigned();
    if (insn.hasError())
        return insn.getError();

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

Error AsmPdp8::parseIoTransferOperand(StrScanner &scan, AsmInsn &insn) const {
    auto p = scan;
    const auto addr = parseInteger(p, insn);
    if (insn.hasError())
        return insn.getError();
    const auto pcont = p.skipSpaces();
    if (endOfLine(p))
        return insn.setError(p, MISSING_OPERAND);
    const auto control = parseInteger(p, insn);
    if (insn.hasError())
        return insn.getError();

    if (addr.overflow(0x3F))
        return insn.setErrorIf(scan, OVERFLOW_RANGE);
    if (control.overflow(7))
        return insn.setErrorIf(pcont, OVERFLOW_RANGE);
    insn.embed((addr.getUnsigned() << 3) | control.getUnsigned());
    scan = p;
    return OK;
}

Error AsmPdp8::encodeMicro(AsmInsn &insn, const AsmInsn &micro, Config::opcode_t &done) const {
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

Error AsmPdp8::parseField(StrScanner &scan, AsmInsn &insn) const {
    auto p = scan;
    auto field = parseInteger(p, insn).getUnsigned();
    if (!insn.hasError()) {
        if (field < 8) {
            field <<= 3;
        } else if (field & ~070) {
            insn.setErrorIf(scan, ILLEGAL_OPERAND);
            field &= ~070;
        }
        insn.embed(field);
        scan = p;
    }
    return insn.getError();
}

Error AsmPdp8::parseMemExtensionOperand(StrScanner &scan, AsmInsn &insn) const {
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
        insn.setErrorIf(scan, encodeMicro(insn, micro, done));
        scan = p;
    }
    return insn.setError(MISSING_OPERAND);
}

Error AsmPdp8::parseOperateOperand(StrScanner &scan, AsmInsn &insn) const {
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
        if (insn.setErrorIf(at, TABLE.searchName(cpuType(), micro)) == UNKNOWN_INSTRUCTION)
            continue;
        if (mode != M_NONE) {
            TABLE.searchMicro(cpuType(), micro, mode);
            if (micro.isOK()) {
                insn.setErrorIf(at, encodeMicro(insn, micro, done));
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
                insn.setErrorIf(at, encodeMicro(insn, micro, done));
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

Error AsmPdp8::defineDec6String(StrScanner &scan, Insn &insn) {
    ErrorAt error;
    do {
        uint8_t count = 0;
        uint16_t val = 0;
        const auto delim = *scan.skipSpaces()++;
        auto p = scan;
        while (!p.expect(delim)) {
            const auto c = *p;
            if (c == 0)
                return insn.setErrorIf(p, MISSING_CLOSING_DELIMITER);
            uint8_t dec6 = (c < 0x40) ? c : c - 0x40;
            if (c < 0x20 || c >= 0x60) {
                error.setErrorIf(p, ILLEGAL_CONSTANT);
                dec6 = 0;
            }
            if (count % 2 == 0) {
                val = static_cast<uint16_t>(dec6) << 6;
            } else {
                val |= dec6;
                error.setErrorIf(p, insn.emitUint16Be(val));
            }
            ++p;
            count++;
        }
        if (count % 2 == 0) {
            error.setErrorIf(p, insn.emitUint16Be(0));
        } else {
            error.setErrorIf(p, insn.emitUint16Be(val));
        }
        scan = p;
    } while (scan.skipSpaces().expect(','));

    return insn.setError(error);
}

Error AsmPdp8::processPseudo(StrScanner &scan, Insn &insn) {
    if (strcasecmp_P(insn.name(), TEXT_OCTAL) == 0)
        return setInputRadix(RADIX_8);
    if (strcasecmp_P(insn.name(), TEXT_DECIMAL) == 0)
        return setInputRadix(RADIX_10);
    if (strcasecmp_P(insn.name(), TEXT_DUBL) == 0)
        return defineDoubleDecimal(scan, insn);
    if (strcasecmp_P(insn.name(), TEXT_PAGE) == 0)
        return alignOnPage(scan, insn);
    if (cpuType() == HD6120 && strcasecmp_P(insn.name(), TEXT_FIELD) == 0)
        return defineField(scan, insn);
    if (strcasecmp_P(insn.name(), TEXT_TEXT) == 0)
        return defineDec6String(scan, insn);
    return Assembler::processPseudo(scan, insn);
}

Error AsmPdp8::encodeImpl(StrScanner &scan, Insn &_insn) const {
    AsmInsn insn(_insn);
    const auto error = TABLE.searchName(cpuType(), insn);
    if (_implicitWord && error != OK) {
        StrScanner p = _insn.errorAt();
        insn.setOK();  // clear UNKNOWN_INSTRUCTION
        const auto opc = parseInteger(p, insn);
        if (insn.hasError())
            return _insn.setError(insn);
        constexpr auto UINT12_MAX = INT32_C(07777);
        constexpr auto INT12_MIN = INT32_C(-04000);
        if (opc.overflow(UINT12_MAX, INT12_MIN))
            insn.setError(_insn.errorAt(), OVERFLOW_RANGE);
        insn.setOpCode(opc.getUnsigned() & 07777);
        scan = p;
        insn.emitInsn();
    } else if (error == OK) {
        scan.skipSpaces();
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
        insn.emitInsn();
    }
    return _insn.setError(insn);
}

}  // namespace pdp8
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
