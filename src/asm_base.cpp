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

#include <math.h>
#include <stdlib.h>

#ifndef powl
#define powl(a, b) pow(a, b)
#endif
#ifndef log10l
#define log10l(a) log10(a)
#endif
#ifndef log2l
#define log2l(a) (log10(a) / log10(2))
#endif
#ifndef strtold
#define strtold(a, b) strtod(a, b)
#endif
#ifndef modfl
#define modfl(a, b) modf(a, b)
#endif

#include "asm_base.h"

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
constexpr char OPT_TEXT_FPU[] PROGMEM = "fpu";
constexpr char OPT_DESC_FPU[] PROGMEM = "floating point co-processor";

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
    : _parser(plugins, *this),
      _pseudos(pseudos),
      _commonOptions(&_opt_listRadix),
      _options(option),
      _opt_listRadix(this, &Assembler::setListRadix, OPT_INT_LIST_RADIX, OPT_DESC_LIST_RADIX,
              &_opt_smartBranch),
      _opt_smartBranch(this, &Assembler::setSmartBranch, OPT_BOOL_SMART_BRANCH,
              OPT_DESC_SMART_BRANCH, &_opt_fpu),
      _opt_fpu(this, &Assembler::setFpu, OPT_TEXT_FPU, OPT_DESC_FPU) {
    Assembler::reset();
}

void Assembler::reset() {
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

Error Assembler::encode(const char *line, Insn &insn, SymbolTable *symtab) {
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
    if (!_parser.endOfLine(scan.skipSpaces()))
        return insn.setErrorIf(scan, GARBAGE_AT_END);
    return insn.getError();
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

bool Assembler::setCpu(const char *name) {
    StrScanner scan{name};
    return setCpu(scan) == OK;
}

Error Assembler::setCpu(StrScanner &scan) {
    return configSetter().setCpuName(scan);
}

Error Assembler::setFpu(StrScanner &scan) {
    UNUSED(scan);
    return UNSUPPORTED_CPU;
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

Error Assembler::setOption(StrScanner &scan, Insn &insn, uint8_t extra) {
    UNUSED(insn);
    UNUSED(extra);
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

Error Assembler::defineOrigin(StrScanner &scan, Insn &insn, uint8_t extra) {
    UNUSED(extra);
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

Error Assembler::alignOrigin(StrScanner &scan, Insn &insn, uint8_t step) {
    auto p = scan;
    Value value;
    if (step) {
        value.setUnsigned(step);
    } else {
        ErrorAt error;
        value = parseExpr(p, error);
        if (error.getError())
            return insn.setError(error);
    }
    if (value.overflowUint8())
        return insn.setError(scan, OVERFLOW_RANGE);
    insn.align(value.getUnsigned());
    if (insn.setErrorIf(scan, setCurrentLocation(insn.address())))
        return insn.getError();
    scan = p;
    return OK;
}

Error Assembler::allocateSpaces(StrScanner &scan, Insn &insn, uint8_t dataType) {
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

    uint8_t unit;
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
                return insn.setErrorIf(p, MISSING_CLOSING_DELIMITER);
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
    case DATA_FLOAT32_LONG:
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

Error Assembler::defineDataConstant(StrScanner &scan, Insn &insn, uint8_t dataType) {
    // Do alignment if requested.
    if (dataType & DATA_ALIGN2)
        setCurrentLocation(insn.align(2));
    const auto type = static_cast<DataType>(dataType & ~DATA_ALIGN2);
    const auto big = config().endian() == ENDIAN_BIG;
    const auto hasString = !(type == DATA_BYTE_NO_STRING || type == DATA_WORD_NO_STRING);
    ErrorAt error;
    do {
        auto p = scan.skipSpaces();
        ErrorAt strErr;
        if (hasString && isString(p, strErr) == OK) {
            generateString(scan, p, insn, type, error);
            continue;
        }

        const auto at = p;
        ErrorAt exprErr;
        const auto value = parseExpr(p, exprErr);
        if (!endOfLine(p.skipSpaces()) && *p != ',')
            exprErr.setErrorIf(at, ILLEGAL_CONSTANT);
        if (!exprErr.hasError()) {
            auto v = value.getUnsigned();
            switch (type) {
            case DATA_LONG:
                big ? insn.emitUint32Be(v) : insn.emitUint32Le(v);
                break;
            case DATA_BYTE_IN_WORD:
                v &= 0xFF;
                // Fall-through
            case DATA_WORD:
            case DATA_WORD_NO_STRING:
            emit_word:
                big ? insn.emitUint16Be(v) : insn.emitUint16Le(v);
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
            if (insn.getError())
                error.setErrorIf(at, insn);
            scan = p;
            if (exprErr.isOK())
                continue;
        }

        return insn.setError(strErr.getError() ? strErr : exprErr);
    } while (scan.skipSpaces().expect(','));

    return insn.setError(error);
}

namespace {
auto convert2pbcd(uint64_t bin, uint8_t digits) {
    uint64_t pbcd = 0;
    uint8_t shift = 0;
    while (digits-- > 0) {
        pbcd |= (bin % 10) << shift;
        bin /= 10;
        shift += 4;
    }
    return pbcd;
}
}  // namespace

void Assembler::generateFloat80Le(bool negative, long double value, Insn &insn) const {
    if (negative)
        value = -value;
    const uint16_t sign = negative ? 0x8000 : 0;
    if (isinf(value)) {
        insn.emitUint64Le(0);
        insn.emitUint16Le(sign | 0x7FFF);
    } else if (isnan(value)) {
        insn.emitUint64Le(static_cast<uint64_t>(1) << 63);
        insn.emitUint16Le(sign | 0x7FFF);
    } else {
        const int16_t exponent = log2l(value);
        const auto significand = value / powl(2.0, exponent);
        insn.emitUint64Le(significand * powl(2.0, 63));
        insn.emitUint16Le(sign | (exponent + 16383));
    }
}

void Assembler::generateFloat96Be(
        bool negative, long double value, AsmInsnBase &insn, uint8_t pos, DataType type) const {
    if (negative)
        value = -value;
    uint16_t sign = negative ? 0x8000 : 0;
    if (isinf(value)) {
        insn.emitUint16Be(sign | 0x7FFF, pos);
        insn.emitUint16Be(0, pos += 2);
        insn.emitUint64Be(0, pos += 2);
    } else if (isnan(value)) {
        insn.emitUint16Be(sign | 0x7FFF, pos);
        insn.emitUint16Be(0, pos += 2);
        insn.emitUint64Be(static_cast<uint64_t>(1) << 63, pos += 2);
    } else if (type == DATA_FLOAT96) {
        const int16_t exponent = log2l(value);
        const auto significand = value / powl(2.0, exponent);
        insn.emitUint16Be(sign | (exponent + 16383), pos);
        insn.emitUint16Be(0, pos += 2);
        insn.emitUint64Be(significand * powl(2.0, 63), pos += 2);
    } else if (type == DATA_PACKED_BCD96) {
        int16_t exponent = log10l(value);
        if (exponent < 0 || (exponent == 0 && value < 1.0))
            --exponent;
        uint8_t digit = value * powl(10.0, -exponent);
        value *= powl(10.0, 16 - exponent);
        if (exponent < 0) {
            sign |= 0x4000;
            exponent = -exponent;
        }
        insn.emitUint16Be(sign | convert2pbcd(exponent, 3), pos);
        insn.emitUint16Be(digit, pos += 2);
        insn.emitUint64Be(convert2pbcd(value, 16), pos += 2);
    }
}

Error Assembler::defineFloatConstant(StrScanner &scan, Insn &insn, uint8_t dataType) {
    // Do alignment if requested.
    if (dataType & DATA_ALIGN2)
        setCurrentLocation(insn.align(2));
    const auto type = static_cast<DataType>(dataType & ~DATA_ALIGN2);
    const auto big = config().endian() == ENDIAN_BIG;
    const auto hasString = (type == DATA_FLOAT32_LONG);
    ErrorAt error;
    do {
        auto p = scan.skipSpaces();
        ErrorAt strErr;
        if (hasString && isString(p, strErr) == OK) {
            generateString(scan, p, insn, type, error);
            continue;
        }

        const auto at = p;
        const auto negative = p.expect('-') != 0;
        ErrorAt exprErr;
        long double value = 0;
        uint64_t val64 = parseExpr(p, exprErr).getUnsigned();
        const auto fpnum = (*p == '.' || *p == 'e' || *p == 'E');
        auto integer = exprErr.isOK() && !fpnum;
        if (!integer) {
            p = at;
            p.expect('-');
            const auto error = Value::parseNumber(p, RADIX_10, val64);
            integer = (error == OK && !fpnum);
            if (integer) {
                value = negative ? -static_cast<int64_t>(val64) : val64;
                exprErr.setOK();
            } else {
                char *end;
                value = strtold(at.str(), &end);
                if (end != at.str()) {
                    exprErr.setOK();
                } else if (exprErr.hasError()) {
                    exprErr.setError(at, ILLEGAL_CONSTANT);
                }
                p = end;
            }
        }
        if (!endOfLine(p.skipSpaces()) && *p != ',')
            exprErr.setErrorIf(at, ILLEGAL_CONSTANT);
        if (!exprErr.hasError()) {
            switch (type) {
            case DATA_FLOAT32_LONG:
                if (integer && val64 <= UINT32_MAX) {
                    if (negative)
                        val64 = -static_cast<int64_t>(val64);
                    big ? insn.emitUint32Be(val64) : insn.emitUint32Le(val64);
                    break;
                }
                /* Fall-through */
            case DATA_FLOAT32:
                big ? insn.emitFloat32Be(value) : insn.emitFloat32Le(value);
                break;
            case DATA_FLOAT64_QUAD:
                if (integer) {
                    if (negative)
                        val64 = -static_cast<int64_t>(val64);
                    big ? insn.emitUint64Be(val64) : insn.emitUint64Le(val64);
                    break;
                }
                /* Fall-through */
            case DATA_FLOAT64:
                big ? insn.emitFloat64Be(value) : insn.emitFloat64Le(value);
                break;
            case DATA_FLOAT80_BCD:
                if (integer && val64 <= 1e18 - 1) {  // i8087
                    for (auto i = 0; i < 9; ++i) {
                        uint8_t d = val64 % 10;
                        val64 /= 10;
                        d |= (val64 % 10) << 4;
                        val64 /= 10;
                        insn.emitByte(d);
                    }
                    const uint8_t sign = negative ? 0x80 : 0;
                    insn.emitByte(sign);
                } else if (!big) {  // i8087
                    generateFloat80Le(negative, value, insn);
                }
                break;
            case DATA_FLOAT96:
            case DATA_PACKED_BCD96:
                if (big) {  // MC68881
                    AsmInsnBase i{insn};
                    generateFloat96Be(negative, value, i, insn.length(), type);
                }
                break;
            default:
                break;
            }
            if (insn.getError())
                error.setErrorIf(at, insn);
            scan = p;
            if (exprErr.isOK())
                continue;
        }

        return insn.setError(strErr.getError() ? strErr : exprErr);
    } while (scan.skipSpaces().expect(','));

    return insn.setError(error);
}

Error OptionBase::parseBoolOption(StrScanner &scan, bool &value, const Assembler &assembler) {
    if (parseBoolOption(scan, value) == OK)
        return OK;
    int32_t val;
    const auto error = parseIntOption(scan, val, assembler);
    if (error == OK)
        value = (val != 0);
    return error;
}

Error OptionBase::parseIntOption(StrScanner &scan, int32_t &value, const Assembler &assembler) {
    auto p = scan;
    const auto dquote = p.expect('"');
    ErrorAt error;
    value = assembler.parseExpr(p, error).getSigned();
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
