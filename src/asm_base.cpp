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

#include "asm_base.h"

#include "text_common.h"

namespace libasm {

using namespace pseudo;
using namespace text::common;

namespace {

// clang-format off
constexpr char OPT_INT_LIST_RADIX[]  PROGMEM = "list-radix";
constexpr char OPT_DESC_LIST_RADIX[] PROGMEM = "set listing radix (8, 16)";

constexpr Pseudo PSEUDOS[] PROGMEM = {
    {TEXT_ALIGN,  &Assembler::alignOrigin},
    {TEXT_OPTION, &Assembler::setOption},
    {TEXT_ORG,    &Assembler::defineOrigin},
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

}  // namespace

Assembler::Assembler(
        const ValueParser::Plugins &plugins, const Pseudos &pseudos, const OptionBase *option)
    : ErrorAt(),
      _parser(plugins, *this),
      _pseudos(pseudos),
      _commonOptions(&_opt_listRadix),
      _options(option),
      _opt_listRadix(this, &Assembler::setListRadix, OPT_INT_LIST_RADIX, OPT_DESC_LIST_RADIX) {
    setListRadix(RADIX_16);
}

Error Assembler::setListRadix(int32_t radix) {
    _listRadix = static_cast<Radix>(radix == 8 ? RADIX_8 : RADIX_16);
    return OK;
}

Error Assembler::setCurrentLocation(uint32_t location, bool align) {
    return config().checkAddr(_currentLocation = location, config().addressWidth(), align);
}

Error Assembler::encode(const char *line, Insn &insn, SymbolTable *symtab) {
    _symtab = symtab;
    StrScanner scan{line};
    setError(scan.skipSpaces(), OK);
    if (_parser.commentLine(scan))
        return OK;

    const auto at = scan;
    StrScanner symbol;
    _parser.readInstruction(scan, symbol);
    insn.nameBuffer().reset().text(symbol);
    auto error = processPseudo(scan.skipSpaces(), insn);

    if (error == UNKNOWN_DIRECTIVE) {
        setError(at, OK);
        if (setErrorIf(at, setCurrentLocation(insn.address())))
            return getError();
        error = encodeImpl(scan, insn);
        if (error == UNKNOWN_INSTRUCTION)
            return setError(at, error);
    }
    if (insn.length())
        setCurrentLocation(insn.address() + insn.length() / config().addressUnit());
    if (!_parser.endOfLine(scan.skipSpaces()))
        return setErrorIf(scan, GARBAGE_AT_END);
    return error;
}

Error Assembler::processPseudo(StrScanner &scan, Insn &insn) {
    const auto *p = _pseudos.search(insn);
    if (p == nullptr)
        p = PSEUDO_TABLE.search(insn);
    return p ? p->invoke(this, scan, insn) : UNKNOWN_DIRECTIVE;
}

uint16_t Assembler::parseExpr16(StrScanner &expr, ErrorAt &error, char delim) const {
    auto p = expr;
    const auto value = _parser.eval(p, error, _symtab, delim);
    if (value.overflowUint16())
        error.setErrorIf(expr, OVERFLOW_RANGE);
    expr = p;
    return value.getUnsigned();
}

uint32_t Assembler::parseExpr32(StrScanner &expr, ErrorAt &error, char delim) const {
    return _parser.eval(expr, error, _symtab, delim).getUnsigned();
}

Value Assembler::parseExpr(StrScanner &expr, ErrorAt &error, char delim) const {
    return _parser.eval(expr, error, _symtab, delim);
}

int32_t Assembler::branchDelta(
        uint32_t base, uint32_t target, ErrorAt &error, const ErrorAt &at) const {
    error.setErrorIf(at, config().checkAddr(target));
    const auto delta = config().signExtend(target - base, config().addressWidth());
    if ((delta >= 0 && target < base) || (delta < 0 && target >= base))
        error.setErrorIf(at, OVERFLOW_RANGE);
    return delta;
}

Error Assembler::setOption(StrScanner &scan, Insn &insn, uint8_t extra) {
    UNUSED(insn);
    UNUSED(extra);
    // parse "name", "text" in |scan|
    auto p = scan.skipSpaces();
    if (!p.expect('"'))
        return ILLEGAL_OPERAND;
    auto name = p;
    p = name.takeWhile([](char c) { return c && c != '"'; });
    if (!p.expect('"'))
        return ILLEGAL_OPERAND;
    if (!p.skipSpaces().expect(','))
        return MISSING_COMMA;
    if (!p.skipSpaces().expect('"'))
        return ILLEGAL_OPERAND;
    auto text = p;
    p = text.takeWhile([](char c) { return c && c != '"'; });
    if (!p.expect('"'))
        return ILLEGAL_OPERAND;
    scan = p;
    return setOption(name, text);
}

Error Assembler::defineOrigin(StrScanner &scan, Insn &insn, uint8_t extra) {
    UNUSED(extra);
    auto p = scan;
    ErrorAt error;
    const auto value = parseExpr(p, error);
    if (error.getError())
        return setError(error);
    const auto addr = value.getUnsigned();
    if (setErrorIf(scan, setCurrentLocation(addr)))
        return getError();
    insn.reset(addr);
    scan = p;
    return OK;
}

Error Assembler::alignOrigin(StrScanner &scan, Insn &insn, uint8_t step) {
    auto p = scan;
    Value value;
    if (step) {
        value.setUnsigned(step);
    } else {
        ErrorAt error;
        value = parseExpr(p, error);
        if (error.getError())
            return setError(error);
    }
    if (value.overflowUint8())
        return setError(scan, OVERFLOW_RANGE);
    insn.align(value.getUnsigned());
    if (setErrorIf(scan, setCurrentLocation(insn.address())))
        return getError();
    scan = p;
    return OK;
}

Error Assembler::allocateSpaces(StrScanner &scan, Insn &insn, uint8_t dataType) {
    const auto type = static_cast<DataType>(dataType);
    const auto align = (type == DATA_WORD_ALIGN2 || type == DATA_LONG_ALIGN2);
    if (align && insn.address() % 2)
        insn.align(2);
    auto p = scan;
    ErrorAt error;
    auto value = parseExpr(p, error);
    if (error.getError())
        return setError(error);
    if (value.overflowUint16())
        return setError(scan, OVERFLOW_RANGE);

    uint8_t unit;
    switch (type) {
    case DATA_WORD:
    case DATA_WORD_ALIGN2:
        unit = sizeof(uint16_t);
        break;
    case DATA_LONG:
    case DATA_LONG_ALIGN2:
        unit = sizeof(uint32_t);
        break;
    default:
        unit = sizeof(uint8_t);
    }
    const auto bytes = value.getUnsigned() * unit;
    const auto addr = insn.address() + (bytes / config().addressUnit());
    setErrorIf(scan, setCurrentLocation(addr, align));
    scan = p;
    return OK;
}

Error Assembler::defineString(StrScanner &scan, Insn &insn, uint8_t stringType) {
    const auto type = static_cast<StringType>(stringType);
    ErrorAt error;
    do {
        uint8_t count = 0;
        uint16_t val = 0;
        const auto delim = *scan.skipSpaces()++;
        auto p = scan;
        while (!p.expect(delim)) {
            const auto c = *p;
            if (c == 0)
                return setErrorIf(p, MISSING_CLOSING_DELIMITER);
            if (type == STR_ASCII) {
                error.setErrorIf(p, insn.emitByte(c));
            } else if (type == STR_DEC_6BIT) {
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
            }
            ++p;
            count++;
        }
        if (type == STR_DEC_6BIT) {
            if (count % 2 == 0) {
                error.setErrorIf(p, insn.emitUint16Be(0));
            } else {
                error.setErrorIf(p, insn.emitUint16Be(val));
            }
        }
        scan = p;
    } while (scan.skipSpaces().expect(','));

    return setErrorIf(error);
}

/**
 * Returns OK if |scan| points a string constant, and update |scan| at the end of string content.
 */
Error Assembler::isString(StrScanner &scan, ErrorAt &error) const {
    auto p = scan;
    // Check prefix for string constant if any.
    parser().stringPrefix(p);  // string prefix is optional

    // Check opening string delimiter.
    const auto delim = parser().stringDelimiter(p);
    if (delim == 0)
        return NOT_AN_EXPECTED;

    while (!endOfLine(p)) {
        if (*p == delim) {
            auto a = p;
            a += 1;  // skip possible delimiter
            if (endOfLine(a.skipSpaces()) || *a == ',') {
                scan = p;
                return OK;
            }
        }
        parser().readLetter(p, error, delim);
        if (error.getError())
            return error.getError();
    }
    // If reached to the end of line, it means we can't find closing delimiter.
    error.setErrorIf(scan, delim == '"' ? MISSING_CLOSING_DQUOTE : MISSING_CLOSING_QUOTE);
    return error.getError();
}

Error Assembler::defineDataConstant(StrScanner &scan, Insn &insn, uint8_t dataType) {
    const auto type = static_cast<DataType>(dataType);
    // Do alignment if requested.
    if (type == DATA_WORD_ALIGN2 || type == DATA_LONG_ALIGN2)
        setCurrentLocation(insn.align(2));

    const auto big = config().endian() == ENDIAN_BIG;
    const auto hasString = !(type == DATA_BYTE_NO_STRING || type == DATA_WORD_NO_STRING);
    ErrorAt error;
    do {
        auto p = scan.skipSpaces();
        ErrorAt strErr;
        if (hasString && isString(p, strErr) == OK) {
            // a string surrounded by string delimiters
            parser().stringPrefix(scan);  // skip prefix if exists
            const auto delim = parser().stringDelimiter(scan);
            uint8_t count = 0;
            while (scan.str() < p.str()) {
                const auto at = scan;
                const auto c = parser().readLetter(scan, strErr, delim);
                error.setErrorIf(at, insn.emitByte(c));
                count++;
            }
            parser().stringDelimiter(p);  // skip closing delimiter
            scan = p;
            // emit padding bytes
            switch (type) {
            case DATA_WORD:
            case DATA_WORD_ALIGN2:
                if (count % 2)
                    error.setErrorIf(p, insn.emitByte(0));
                break;
            case DATA_LONG:
            case DATA_LONG_ALIGN2:
                while (count % 4) {
                    error.setErrorIf(p, insn.emitByte(0));
                    count++;
                }
                break;
            default:
                if (config().addressUnit() == ADDRESS_WORD && count % 2)
                    error.setErrorIf(p, insn.emitByte(0));
                break;
            }
            continue;
        }

        const auto at = p;
        ErrorAt exprErr;
        const auto value = parseExpr(p, exprErr);
        if (!endOfLine(p.skipSpaces()) && *p != ',')
            exprErr.setErrorIf(scan, ILLEGAL_CONSTANT);
        if (!exprErr.hasError()) {
            auto v = value.getUnsigned();
            switch (type) {
            case DATA_LONG:
            case DATA_LONG_ALIGN2:
                error.setErrorIf(at, big ? insn.emitUint32Be(v) : insn.emitUint32Le(v));
                break;
            case DATA_BYTE_IN_WORD:
                v &= 0xFF;
                // Fall-through
            case DATA_WORD:
            case DATA_WORD_ALIGN2:
            case DATA_WORD_NO_STRING:
            emit_word:
                error.setErrorIf(at, big ? insn.emitUint16Be(v) : insn.emitUint16Le(v));
                break;
            case DATA_BYTE_OR_WORD:
                if (value.overflowUint8())
                    goto emit_word;
                goto emit_byte;
            default:  // DATA_BYTE, DATA_BYTE_NO_STRING
            emit_byte:
                error.setErrorIf(at, insn.emitByte(v));
                break;
            }
            scan = p;
            continue;
        }

        return setError(strErr.getError() ? strErr : exprErr);
    } while (scan.skipSpaces().expect(','));

    return setErrorIf(error);
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
