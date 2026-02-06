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
#include <math.h>
#include <stdlib.h>
#include "text_common.h"

namespace libasm {

using namespace pseudo;
using namespace text::common;

namespace {

// clang-format off
constexpr char OPT_INT_LIST_RADIX[]  PROGMEM = "list-radix";
constexpr char OPT_DESC_LIST_RADIX[] PROGMEM = "set listing radix (8, 16)";
constexpr char OPT_BOOL_SMART_BRANCH[] PROGMEM = "smart-branch";
constexpr char OPT_DESC_SMART_BRANCH[] PROGMEM = "optimize branch instruction";

constexpr Pseudo PSEUDOS[] PROGMEM = {
    { TEXT_ALIGN,  &Assembler::alignOrigin  },
    { TEXT_OPTION, &Assembler::setOption    },
    { TEXT_ORG,    &Assembler::defineOrigin },
};
// clang-format on
PROGMEM constexpr Pseudos PSEUDO_TABLE{ARRAY_RANGE(PSEUDOS)};

}  // namespace

Assembler::Assembler(
        const ValueParser::Plugins &plugins, const Pseudos &pseudos, const OptionBase *option)
    : _parser(plugins),
      _pseudos(&pseudos),
      _commonOptions(&_opt_listRadix),
      _options(option),
      _opt_listRadix(this, &Assembler::setListRadix, OPT_INT_LIST_RADIX, OPT_DESC_LIST_RADIX,
              &_opt_smartBranch),
      _opt_smartBranch(
              this, &Assembler::setSmartBranch, OPT_BOOL_SMART_BRANCH, OPT_DESC_SMART_BRANCH) {
    Assembler::reset();
}

void Assembler::reset() {
    setInputRadix(RADIX_10);
    setListRadix(RADIX_16);
    setSmartBranch(false);
}

Error Assembler::setListRadix(int32_t radix) {
    _listRadix = static_cast<Radix>(radix == 8 ? RADIX_8 : RADIX_16);
    return OK;
}

Error Assembler::setSmartBranch(bool enable) {
    _smartBranch = enable;
    return OK;
}

Error Assembler::setCurrentLocation(uint32_t location, bool align) {
    return config().checkAddr(_currentLocation = location, align);
}

Error Assembler::encode(const char *line, Insn &insn, const SymbolTable *symtab) {
    _symtab = symtab;
    StrScanner scan{line};
    insn.setError(scan.skipSpaces(), OK);
    if (_parser.commentLine(scan))
        return OK;

    const auto at = scan;
    StrScanner symbol;
    _parser.readInstruction(scan, symbol);
    insn.nameBuffer().reset().text(symbol);
    auto error = processPseudo(scan.skipSpaces(), insn);

    if (error == UNKNOWN_DIRECTIVE) {
        insn.setError(at, OK);
        if (insn.setErrorIf(at, setCurrentLocation(insn.address())))
            return insn.getError();
        error = encodeImpl(scan, insn);
        if (error == UNKNOWN_INSTRUCTION)
            return insn.setError(at, error);
    }

    if (error == OK)
        insn.setAt(scan);
    return insn.getError();
}

Error Assembler::processPseudo(StrScanner &scan, Insn &insn) {
    const auto *p = _pseudos->search(insn.name());
    if (p == nullptr)
        p = PSEUDO_TABLE.search(insn.name());
    return p ? p->invoke(this, scan, insn) : UNKNOWN_DIRECTIVE;
}

Value Assembler::parseInteger(StrScanner &expr, ErrorAt &error, char delim) const {
    auto p = expr;
    const auto value = parseExpr(p, error, delim);
    if (value.isFloat())
        error.setErrorIf(expr, INTEGER_REQUIRED);
    expr = p;
    return value;
}

ParserContext Assembler::parserContext(const SymbolTable *symtab, char delim) const {
    return ParserContext{_currentLocation, symtab, delim, _inputRadix};
}

Value Assembler::parseExpr(StrScanner &expr, ErrorAt &error, char delim) const {
    auto context = parserContext(_symtab, delim);
    return _parser.eval(expr, error, context);
}

int32_t Assembler::branchDelta(
        uint32_t base, uint32_t target, ErrorAt &error, const ErrorAt &at) const {
    error.setErrorIf(at, config().checkAddr(target));
    const auto delta = config().signExtend(target - base, config().addressWidth());
    if ((delta >= 0 && target < base) || (delta < 0 && target >= base))
        error.setErrorIf(at, OVERFLOW_RANGE);
    return delta;
}

Error Assembler::setOption(const char *name, const char *text) {
    StrScanner scan{text};
    return setOption(name, scan);
}

Error Assembler::setOption(const StrScanner &name, StrScanner &text) {
    if (_commonOptions.setOption(name, text) == OK)
        return OK;
    return options().setOption(name, text);
}

Error Assembler::setOption(StrScanner &scan, Insn &insn, uint16_t) {
    // parse "name", "text" in |scan|
    auto p = scan.skipSpaces();
    const auto dquote = p.expect('"');
    auto option = p;
    p = option.takeWhile([](char c) { return isalnum(c) || c == '-'; });
    if (dquote && !p.expect('"'))
        return MISSING_CLOSING_DQUOTE;
    if (!p.skipSpaces().expect(','))
        return MISSING_COMMA;
    if (insn.setErrorIf(p, setOption(option, p.skipSpaces())))
        return insn.getError();
    scan = p;
    return OK;
}

Error Assembler::defineOrigin(StrScanner &scan, Insn &insn, uint16_t) {
    auto p = scan;
    ErrorAt error;
    const auto value = parseExpr(p, error);
    if (error.getError())
        return insn.setError(error);
    const auto addr = value.getUnsigned();
    if (insn.setErrorIf(scan, setCurrentLocation(addr)))
        return insn.getError();
    insn.reset(addr);
    scan = p;
    return OK;
}

Error Assembler::alignOrigin(StrScanner &scan, Insn &insn, uint16_t step) {
    auto at = insn.errorAt();
    if (step == ALIGN_ODD) {
        if (insn.address() % 2 == 0)
            insn.reset(insn.address() + 1, insn.length());
        return insn.setErrorIf(at, setCurrentLocation(insn.address()));
    }

    if (step == ALIGN_VAR) {
        ErrorAt error;
        auto p = scan;
        const auto value = parseExpr(p, error);
        if (error.getError())
            return insn.setError(error);
        if (value.overflow(UINT8_MAX))
            return insn.setError(scan, OVERFLOW_RANGE);
        step = value.getUnsigned();
        at = scan.str();
        scan = p;
    }
    insn.align(step);
    return insn.setErrorIf(at, setCurrentLocation(insn.address()));
}

Error Assembler::allocateSpaces(StrScanner &scan, Insn &insn, uint16_t dataType) {
    const auto align = (dataType & DATA_ALIGN2) != 0;
    if (align && insn.address() % 2)
        insn.align(2);
    const auto type = static_cast<DataType>(dataType & ~DATA_ALIGN2);
    auto p = scan;
    ErrorAt error;
    auto value = parseExpr(p, error);
    if (error.getError())
        return insn.setError(error);
    if (value.overflowUint16())
        return insn.setError(scan, OVERFLOW_RANGE);

    size_t unit;
    switch (type) {
    case DATA_WORD:
        unit = sizeof(uint16_t);
        break;
    case DATA_LONG:
        unit = sizeof(uint32_t);
        break;
    default:
        unit = sizeof(uint8_t);
    }
    const auto bytes = value.getUnsigned() * unit;
    const auto addr = insn.address() + (bytes / config().addressUnit());
    insn.setErrorIf(scan, setCurrentLocation(addr, align));
    scan = p;
    return OK;
}

Error Assembler::defineString(StrScanner &scan, Insn &insn, uint16_t) {
    ErrorAt error;
    do {
        const auto delim = *scan.skipSpaces()++;
        auto p = scan;
        while (!p.expect(delim)) {
            const auto c = *p;
            if (c == 0)
                return insn.setErrorIf(p, MISSING_CLOSING_DELIMITER);
            error.setErrorIf(p, insn.emitByte(c));
            ++p;
        }
        scan = p;
    } while (scan.skipSpaces().expect(','));

    return insn.setError(error);
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

    while (*p) {
        if (*p == delim) {
            auto a = p;
            a += 1;  // skip possible delimiter
            if (endOfLine(a) || *a == ',') {
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

void Assembler::generateString(
        StrScanner &scan, const StrScanner &end, Insn &insn, DataType type, ErrorAt &error) const {
    // a string surrounded by string delimiters
    parser().stringPrefix(scan);  // skip prefix if exists
    const auto delim = parser().stringDelimiter(scan);
    uint8_t count = 0;
    while (scan.str() < end.str()) {
        const auto at = scan;
        ErrorAt err;
        const auto c = parser().readLetter(scan, err, delim);
        error.setErrorIf(at, insn.emitByte(c));
        count++;
    }
    scan = end;
    parser().stringDelimiter(scan);  // skip closing delimiter
    // emit padding bytes
    switch (type) {
    case DATA_WORD:
        if (count % 2)
            error.setErrorIf(scan, insn.emitByte(0));
        break;
    case DATA_LONG:
        while (count % 4) {
            error.setErrorIf(scan, insn.emitByte(0));
            count++;
        }
        break;
    default:
        if (config().addressUnit() == ADDRESS_WORD && count % 2)
            error.setErrorIf(scan, insn.emitByte(0));
        break;
    }
}

Error Assembler::defineDataConstant(StrScanner &scan, Insn &insn, uint16_t dataType) {
    // Do alignment if requested.
    if (dataType & DATA_ALIGN2)
        setCurrentLocation(insn.align(2));
    const auto type = static_cast<DataType>(dataType & ~DATA_ALIGN2);
    const auto big = config().endian() == ENDIAN_BIG;
    const auto hasString = !(type == DATA_BYTE_NO_STRING || type == DATA_WORD_NO_STRING);
    ErrorAt error;
    do {
        scan.skipSpaces();
        ErrorAt strErr;
        if (hasString) {
            auto end = scan;
            const auto err = isString(end, strErr);
            if (err == OK) {
                generateString(scan, end, insn, type, strErr);
                if (error.setErrorIf(strErr) == NO_MEMORY)
                    break;
                continue;
            }
        }
        ErrorAt exprErr;
        auto p = scan;
        const auto val = parseExpr(p, exprErr);
        if (!endOfLine(p) && *p != ',') {
            if (strErr.getError()) {
                error.setErrorIf(strErr);
            } else {
                error.setErrorIf(scan, ILLEGAL_CONSTANT);
            }
            break;
        }
        if (exprErr.hasError()) {
            error.setErrorIf(strErr.getError() ? strErr : exprErr);
            break;
        }

        auto v = val.getUnsigned();
        switch (type) {
        case DATA_BYTE_OR_WORD:
            if (val.overflowUint8())
                goto emit_word;
            // Fall-through
        case DATA_BYTE:
        case DATA_BYTE_NO_STRING:
            exprErr.setErrorIf(scan, insn.emitByte(v));
            break;
        case DATA_BYTE_IN_WORD:
            v &= 0xFF;
            // Fall-through
        case DATA_WORD:
        case DATA_WORD_NO_STRING:
        emit_word:
            exprErr.setErrorIf(scan, big ? insn.emitUint16Be(v) : insn.emitUint16Le(v));
            break;
        case DATA_LONG:
            exprErr.setErrorIf(scan, big ? insn.emitUint32Be(v) : insn.emitUint32Le(v));
            break;
        case DATA_FLOAT32:
#if defined(LIBASM_ASM_NOFLOAT)
            exprErr.setErrorIf(scan, FLOAT_NOT_SUPPORTED);
            insn.emitUint32Be(0);
#else
            exprErr.setErrorIf(scan,
                    big ? insn.emitFloat32Be(val.getFloat()) : insn.emitFloat32Le(val.getFloat()));
#endif
            break;
        case DATA_FLOAT64:
#if defined(LIBASM_ASM_NOFLOAT)
            exprErr.setErrorIf(scan, FLOAT_NOT_SUPPORTED);
            insn.emitUint64Be(0);
#else
            exprErr.setErrorIf(scan,
                    big ? insn.emitFloat64Be(val.getFloat()) : insn.emitFloat64Le(val.getFloat()));
#endif
            break;
        case DATA_ALIGN2:
            break;
        }
        scan = p;
        if (error.setErrorIf(exprErr) == NO_MEMORY)
            break;
    } while (scan.skipSpaces().expect(','));
    return insn.setError(error);
}

Error Assembler::parseBoolOption(StrScanner &scan, bool &value) const {
    if (OptionBase::parseBoolOption(scan, value) == OK)
        return OK;
    int32_t val;
    const auto error = parseIntOption(scan, val);
    if (error == OK)
        value = (val != 0);
    return error;
}

Error Assembler::parseIntOption(StrScanner &scan, int32_t &value) const {
    auto p = scan;
    const auto dquote = p.expect('"');
    ErrorAt error;
    value = parseInteger(p, error).getSigned();
    if (error.isOK()) {
        if (dquote && !p.expect('"'))
            return ILLEGAL_CONSTANT;
        scan = p;
    }
    return error.getError();
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
