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

Error Assembler::setCurrentLocation(uint32_t location) {
    return config().checkAddr(_currentLocation = location);
}

Error Assembler::encode(const char *line, Insn &insn, SymbolTable *symtab) {
    _symtab = symtab;
    setAt(line);
    StrScanner scan(line);
    setError(scan, OK);
    if (_parser.commentLine(scan.skipSpaces()))
        return setError(scan, OK);

    StrScanner symbol;
    if (_parser.readInstruction(scan, symbol) != OK)
        return setError(scan, UNKNOWN_INSTRUCTION);

    insn.clearNameBuffer().text(symbol);
    auto error = processPseudo(scan, insn);
    if (error != UNKNOWN_DIRECTIVE)
        return setError(error);

    error = setCurrentLocation(insn.address());
    if (error)
        return setError(error);

    error = encodeImpl(scan, insn);
    if (error == UNKNOWN_INSTRUCTION)
        setAt(line);
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

int32_t Assembler::branchDelta(uint32_t base, uint32_t target, const ErrorAt &at) {
    const auto err = config().checkAddr(target);
    if (err)
        setErrorIf(at, err);
    const auto delta = config().signExtend(target - base, config().addressWidth());
    if ((delta >= 0 && target < base) || (delta < 0 && target >= base))
        setErrorIf(at, OVERFLOW_RANGE);
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
    if (!endOfLine(p.skipSpaces()))
        return GARBAGE_AT_END;
    return setOption(name, text);
}

Error Assembler::defineOrigin(StrScanner &scan, Insn &insn, uint8_t extra) {
    UNUSED(extra);
    auto p = scan;
    ErrorAt error;
    const auto value = parseExpr(p, error);
    if (error.getError())
        return setError(error);
    if (!endOfLine(p))
        return setError(p, GARBAGE_AT_END);
    const auto addr = value.getUnsigned();
    const auto err = setCurrentLocation(addr);
    if (err)
        return setError(scan, err);
    insn.reset(addr);
    return OK;
}

Error Assembler::alignOrigin(StrScanner &scan, Insn &insn, uint8_t extra) {
    auto p = scan;
    Value value;
    if (extra) {
        value.setUnsigned(extra);
    } else {
        ErrorAt error;
        value = parseExpr(p, error);
        if (error.getError())
            return setError(error);
    }
    if (!endOfLine(p))
        return setError(p, GARBAGE_AT_END);
    if (value.overflowUint8())
        return setError(scan, OVERFLOW_RANGE);
    insn.align(value.getUnsigned());
    return setCurrentLocation(insn.address());
}

Error Assembler::allocateSpaces(StrScanner &scan, Insn &insn, uint8_t extra) {
    const auto type = static_cast<DataType>(extra);
    if (type == DATA_WORD_ALIGN2 || type == DATA_LONG_ALIGN2)
        setCurrentLocation(insn.align(2));
    ErrorAt error;
    auto value = parseExpr(scan, error);
    if (error.getError())
        return setError(error);
    if (value.overflowUint16())
        return setError(OVERFLOW_RANGE);

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
    const auto size = value.getUnsigned() * unit / config().addressUnit();
    const auto addr = insn.address() + size;
    setCurrentLocation(addr);
    return OK;
}

Error Assembler::defineString(StrScanner &scan, Insn &insn, uint8_t extra) {
    UNUSED(extra);
    do {
        const auto delim = *scan.skipSpaces()++;
        auto p = scan;
        while (!p.expect(delim)) {
            if (*p == 0)
                return setError(p, MISSING_CLOSING_DELIMITER);
            const auto c = *p++;
            insn.emitByte(c);
        }
        scan = p;
    } while (scan.skipSpaces().expect(','));

    if (!endOfLine(scan.skipSpaces()))
        return setError(scan, GARBAGE_AT_END);
    return OK;
}

/**
 * Returns OK if |scan| points a string constant, and update |scan| at the end of string content.
 */
Error Assembler::isString(StrScanner &scan, ErrorAt &error) const {
    auto p = scan;
    // Check prefix for string constant if any.
    const auto prefix = parser().stringPrefix();
    if (prefix)
        p.iexpect(prefix);  // string prefix is optional

    // Check opening string delimiter.
    const auto delim = parser().stringDelimiter();
    if (!p.expect(delim))
        return NOT_AN_EXPECTED;

    while (true) {
        // If reached to the end of line, it means we can't find closing delimiter.
        if (endOfLine(p))
            return error.setError(
                    scan, delim == '"' ? MISSING_CLOSING_DQUOTE : MISSING_CLOSING_QUOTE);
        parser().readLetterInString(p, error);
        if (error.getError())
            return error.getError();
        if (*p == delim) {
            auto a = p;
            a += 1;  // skip possible delimiter and check the end.
            if (endOfLine(a.skipSpaces()) || *a == ',') {
                scan = p;
                return OK;
            }
            // Susccessive two delimiters for escaping.
        }
    }
}

Error Assembler::defineDataConstant(StrScanner &scan, Insn &insn, uint8_t extra) {
    const auto type = static_cast<DataType>(extra);
    // Do alignment if requested.
    if (type == DATA_WORD_ALIGN2 || type == DATA_LONG_ALIGN2)
        setCurrentLocation(insn.align(2));

    const auto big = config().endian() == ENDIAN_BIG;
    const auto noString = (type == DATA_BYTE_NO_STRING || type == DATA_WORD_NO_STRING);
    do {
        auto p = scan.skipSpaces();
        ErrorAt strErr;
        if (!noString && isString(p, strErr) == OK) {
            // a string surrounded by string delimiters
            const auto prefix = parser().stringPrefix();
            if (prefix)
                scan.iexpect(prefix);  // skip prefix
            ++scan;                    // skip opening delimiter
            uint8_t count = 0;
            while (scan.str() < p.str()) {
                ErrorAt error;
                const auto c = parser().readLetterInString(scan, error);
                insn.emitByte(c);
                count++;
            }
            ++scan;  // skip closing delimiter
            // emit padding bytes
            switch (type) {
            case DATA_WORD:
            case DATA_WORD_ALIGN2:
                if (count % 2)
                    insn.emitByte(0);
                break;
            case DATA_LONG:
            case DATA_LONG_ALIGN2:
                while (count % 4) {
                    insn.emitByte(0);
                    count++;
                }
                break;
            default:
                if (config().addressUnit() == ADDRESS_WORD && count % 2)
                    insn.emitByte(0);
                break;
            }
            continue;
        }

        ErrorAt exprErr;
        const auto value = parseExpr(p, exprErr);
        if (!endOfLine(p.skipSpaces()) && *p != ',')
            exprErr.setError(scan, ILLEGAL_CONSTANT);
        if (!exprErr.hasError()) {
            auto v = value.getUnsigned();
            switch (type) {
            case DATA_LONG:
            case DATA_LONG_ALIGN2:
                if (big) {
                    insn.emitUint32Be(v);
                } else {
                    insn.emitUint32Le(v);
                }
                break;
            case DATA_BYTE_IN_WORD:
                v &= 0xFF;
                // Fall-through
            case DATA_WORD:
            case DATA_WORD_ALIGN2:
            case DATA_WORD_NO_STRING:
            emit_word:
                if (big) {
                    insn.emitUint16Be(v);
                } else {
                    insn.emitUint16Le(v);
                }
                break;
            case DATA_BYTE_OR_WORD:
                if (value.overflowUint8())
                    goto emit_word;
                goto emit_byte;
            default:  // DATA_BYTE, DATA_BYTE_NO_STRING
            emit_byte:
                insn.emitByte(v);
                break;
            }
            scan = p;
            continue;
        }

        return setError(strErr.getError() ? strErr : exprErr);
    } while (scan.skipSpaces().expect(','));

    if (!endOfLine(scan.skipSpaces()))
        return setError(scan, GARBAGE_AT_END);
    return OK;
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
