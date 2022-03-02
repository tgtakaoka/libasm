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
#include "value_parser.h"

#include <ctype.h>

namespace libasm {

bool Value::overflowRel8(int16_t s16) {
    return s16 < -128 || s16 >= 128;
}

bool Value::overflowRel8(int32_t s32) {
    return s32 < -128 || s32 >= 128;
}

bool Value::overflowRel16(int32_t s32) {
    return s32 < -32768L || s32 >= 32768L;
}

bool Value::overflowUint8(uint16_t u16) {
    const int16_t s16 = static_cast<int16_t>(u16);
    return s16 < -128 || (s16 >= 0 && u16 >= 256);
}

bool Value::overflowUint8(uint32_t u32) {
    const int32_t s32 = static_cast<int32_t>(u32);
    return s32 < -128 || (s32 >= 0 && u32 >= 0x100);
}

bool Value::overflowUint16(uint32_t u32) {
    const int32_t s32 = static_cast<int32_t>(u32);
    return s32 < -32768L || (s32 >= 0 && u32 >= 0x10000L);
}

static bool isoctal(char c) {
    return c >= '0' && c < '8';
}

char ValueParser::readChar(StrScanner &scan) {
    setOK();
    StrScanner p(scan);
    char c = *p++;
    if (c != '\\') {
        scan = p;
        return c;
    }
    uint8_t base = 0;
    if (p.iexpect('x')) {
        base = 16;
    } else if (isoctal(*p)) {
        base = 8;
    } else {
        switch (c = *p++) {
        case '\'':
        case '"':
        case '?':
        case '\\':
            break;
        case 'b':
            c = 0x08;
            break;
        case 't':
            c = 0x09;
            break;
        case 'n':
            c = 0x0a;
            break;
        case 'r':
            c = 0x0d;
            break;
        default:
            setError(scan, UNKNOWN_ESCAPE_SEQUENCE);
            return 0;
        }
        scan = p;
        return c;
    }
    Value value;
    parseNumber(p, value, base);
    if (getError())
        return 0;
    if (value.overflowUint8()) {
        setError(scan, OVERFLOW_RANGE);
        return 0;
    }
    scan = p;
    return value.getUnsigned();
}

static bool isValidDigit(const char c, const uint8_t base) {
    if (base == 16)
        return isxdigit(c);
    return c >= '0' && c < '0' + base;
}

static uint8_t toNumber(const char c, const uint8_t base) {
    if (base == 16 && c >= 'A')
        return (c & ~0x20) - 'A' + 10;
    return c - '0';
}

Error ValueParser::parseNumber(
        StrScanner &scan, Value &val, const uint8_t base, const char suffix) {
    StrScanner p(scan);
    if (!isValidDigit(*p, base))
        return setError(scan, ILLEGAL_CONSTANT);
    uint32_t v = 0;
    while (isValidDigit(*p, base)) {
        const uint32_t prev = v;
        v *= base;
        v += toNumber(*p, base);
        if (v < prev)
            return setError(scan, OVERFLOW_RANGE);
        ++p;
    }
    if (isalpha(suffix)) {
        // Alphabet suffix is mandatory
        if (toupper(*p++) != suffix)
            return setError(scan, ILLEGAL_CONSTANT);
    } else if (suffix) {
        // Non-laphabet suffix is optional, such as H'xx'
        p.expect(suffix);
    }
    if (isSymbolLetter(*p))
        return setError(scan, ILLEGAL_CONSTANT);
    scan = p;
    val.setValue(v);
    return setOK();
}

Value ValueParser::eval(StrScanner &expr, const SymbolTable *symtab) {
    setOK();
    Stack<OprAndLval> stack;
    return parseExpr(expr, stack, symtab);
}

Value ValueParser::parseExpr(
        StrScanner &scan, Stack<OprAndLval> &stack, const SymbolTable *symtab) {
    if (stack.full()) {
        setError(scan, TOO_COMPLEX_EXPRESSION);
        return Value();
    }
    stack.push(OprAndLval());
    Value value(readAtom(scan, stack, symtab));
    if (getError())
        return Value();
    while (!stack.empty()) {
        Operator opr(readOperator(scan));
        while (opr._precedence <= stack.top().precedence()) {
            if (stack.top().isEnd()) {
                stack.pop();
                return value;
            }
            value = evalExpr(stack.top()._opr._op, stack.top()._value, value);
            stack.pop();
        }
        if (stack.full()) {
            setError(scan, TOO_COMPLEX_EXPRESSION);
            return Value();
        }
        stack.push(OprAndLval(opr, value));
        value = readAtom(scan, stack, symtab);
        if (getError())
            return Value();
    }
    return Value();
}

Value ValueParser::readAtom(StrScanner &scan, Stack<OprAndLval> &stack, const SymbolTable *symtab) {
    StrScanner p(scan);
    if (*p.skipSpaces() == 0) {
        setError(scan, ILLEGAL_CONSTANT);
        return Value();
    }
    if (p.expect('(')) {
        Value value(parseExpr(p, stack, symtab));
        if (isOK()) {
            if (!p.skipSpaces().expect(')')) {
                setError(p, MISSING_CLOSING_PAREN);
                return Value();
            }
            scan = p;
            return value;
        }
        return value;
    }
    if (p.expect('\'')) {
        Value value(readCharacterConstant(p));
        if (isOK() && !p.expect('\''))
            setError(p, MISSING_CLOSING_QUOTE);
        scan = p;
        return value;
    }
    if (p.expect('~')) {
        Value value(readAtom(p, stack, symtab));
        scan = p;
        if (!value.isUndefined())
            return value.setValue(~value.getUnsigned());
        return value;
    }
    if (*p == '-' || *p == '+') {
        const char c = *p;
        if (*++p == '-' || *p == '+') {
            setError(scan, UNKNOWN_EXPR_OPERATOR);
            return Value();
        }
        Value value(readAtom(p, stack, symtab));
        if (c == '-' && !value.isUndefined()) {
            if (value.isUnsigned() && value.getUnsigned() > 0x80000000)
                setError(scan, OVERFLOW_RANGE);
            scan = p;
            return Value::makeSigned(-value.getSigned());
        }
        scan = p;
        return value;
    }

    if ((p.expect(_curSym) || p.expect('.')) && !isSymbolLetter(*p)) {
        scan = p;
        return Value::makeUnsigned(_origin);
    }

    Value val;
    if (numberPrefix(p)) {
        if (readNumber(p, val) == OK) {
            scan = p;
            return val;
        }
        return Value();
    }

    if (isSymbolLetter(*p, true)) {
        if (_funcParser)
            _funcParser->setAt(p);
        const StrScanner symbol = readSymbol(p);
        if (*p.skipSpaces() == '(' && _funcParser) {
            StrScanner f(p);
            if (_funcParser->parseFunc(*this, symbol, ++f, val, symtab) == OK) {
                if (!f.expect(')')) {
                    setError(p, MISSING_CLOSING_PAREN);
                    return Value();
                }
                scan = f;
                return val;
            } else if (_funcParser->getError() != UNKNOWN_FUNCTION) {
                setError(*_funcParser);
                return Value();
            }
        }
        if (symtab) {
            scan = p;
            if (symtab->hasSymbol(symbol)) {
                const uint32_t v = symtab->lookupSymbol(symbol);
                return Value().setValue(v);
            }
            return Value();
        }
    }

    if (readNumber(p, val) == OK) {
        scan = p;
        return val;
    }

    return Value();
}

Value ValueParser::readCharacterConstant(StrScanner &scan) {
    const auto c = readChar(scan);
    if (getError())
        return Value();
    return Value::makeSigned(c);
}

// Operator precedence (larger value means higher precedence).
// The same order of C/C++ language.
ValueParser::Operator ValueParser::readOperator(StrScanner &scan) {
    if (*scan.skipSpaces() == 0)
        return Operator(OP_NONE, 0);
    const char c = *scan++;
    switch (c) {
    case '*':
        return Operator(OP_MUL, 13);
    case '/':
        return Operator(OP_DIV, 13);
    case '%':
        return Operator(OP_MOD, 13);
    case '+':
        return Operator(OP_ADD, 12);
    case '-':
        return Operator(OP_SUB, 12);
    case '<':
        if (scan.expect('<'))
            return Operator(OP_BIT_SHL, 11);
        setError(scan, UNKNOWN_EXPR_OPERATOR);
        break;
    case '>':
        if (scan.expect('>'))
            return Operator(OP_BIT_SHR, 11);
        setError(scan, UNKNOWN_EXPR_OPERATOR);
        break;
    case '&':
        return Operator(OP_BIT_AND, 8);
    case '^':
        return Operator(OP_BIT_XOR, 7);
    case '|':
        return Operator(OP_BIT_OR, 6);
    default:
        break;
    }
    --scan;
    return Operator(OP_NONE, 0);
}

bool ValueParser::isSymbolLetter(char c, bool head) const {
    if (isalpha(c) || c == '_')
        return true;
    if (head && c == '.')
        return true;
    return !head && isdigit(c);
}

StrScanner ValueParser::readSymbol(StrScanner &scan) const {
    StrScanner p(scan);
    p.trimStart([this](char c) { return this->isSymbolLetter(c); });
    StrScanner symbol = StrScanner(scan, p);
    scan = p;
    return symbol;
}

static uint32_t shift_left(uint32_t value, uint8_t count) {
    for (unsigned i = 0; i <= 32 && i < count; i++)
        value <<= 1;
    return value;
}

static uint32_t shift_right(uint32_t value, uint8_t count) {
    for (unsigned i = 0; i <= 32 && i < count; i++)
        value >>= 1;
    return value;
}

static int32_t shift_right_negative(int32_t value, uint8_t count) {
    for (unsigned i = 0; i <= 32 && i < count; i++) {
        value >>= 1;
        value |= 0x80000000;
    }
    return value;
}

Value ValueParser::evalExpr(const Op op, const Value lhs, const Value rhs) {
    if (lhs.isUndefined() || rhs.isUndefined())
        return Value();

    const bool bsigned = lhs.isSigned() && rhs.isSigned();

    switch (op) {
    case OP_ADD:
        return Value::makeUnsigned(lhs.getUnsigned() + rhs.getUnsigned()).setSign(bsigned);
    case OP_SUB:
        return Value::makeUnsigned(lhs.getUnsigned() - rhs.getUnsigned()).setSign(bsigned);
    case OP_MUL:
        return bsigned ? Value::makeSigned(lhs.getSigned() * rhs.getSigned())
                       : Value::makeUnsigned(lhs.getUnsigned() * rhs.getUnsigned());
    case OP_DIV:
        if (rhs.getUnsigned() == 0) {
            setError(DIVIDE_BY_ZERO);
            return Value();
        }
        return bsigned ? Value::makeSigned(lhs.getSigned() / rhs.getSigned())
                       : Value::makeUnsigned(lhs.getUnsigned() / rhs.getUnsigned());
    case OP_MOD:
        if (rhs.getUnsigned() == 0) {
            setError(DIVIDE_BY_ZERO);
            return Value();
        }
        return bsigned ? Value::makeSigned(lhs.getSigned() % rhs.getSigned())
                       : Value::makeUnsigned(lhs.getUnsigned() % rhs.getUnsigned());
    case OP_BIT_AND:
        return Value::makeUnsigned(lhs.getUnsigned() & rhs.getUnsigned());
    case OP_BIT_XOR:
        return Value::makeUnsigned(lhs.getUnsigned() ^ rhs.getUnsigned());
    case OP_BIT_OR:
        return Value::makeUnsigned(lhs.getUnsigned() | rhs.getUnsigned());
    case OP_BIT_SHL:
        return Value::makeUnsigned(shift_left(lhs.getUnsigned(), rhs.getUnsigned()));
    case OP_BIT_SHR:
        if (lhs.isSigned() && lhs.getSigned() < 0)
            return Value::makeSigned(shift_right_negative(lhs.getSigned(), rhs.getUnsigned()));
        return Value::makeUnsigned(shift_right(lhs.getUnsigned(), rhs.getUnsigned()));
    default:
        return Value();
    }
}

Error ValueParser::scanNumberEnd(const StrScanner &scan, const uint8_t base, char suffix) {
    StrScanner p(scan);
    if (suffix == 'B') {
        // Check whether intel binary or C-style binary
        if (*p++ == '0' && toupper(*p++) == 'B' && (*p == '0' || *p == '1'))
            return ILLEGAL_CONSTANT;  // expect intel but found C-style
        p = scan;
    }
    p.trimStart([base](char c) { return isValidDigit(c, base); });
    if (suffix && toupper(*p) != suffix)
        return ILLEGAL_CONSTANT;
    return OK;
}

bool ValueParser::numberPrefix(const StrScanner &scan) const {
    return isdigit(*scan);
}

Error ValueParser::readNumber(StrScanner &scan, Value &val) {
    const StrScanner save(scan);
    if (scan.expect('0')) {
        const char c = toupper(*scan);
        if (isoctal(c) && scanNumberEnd(scan, 8) == OK)
            return parseNumber(scan, val, 8);
        ++scan;
        if (c == 'X' && scanNumberEnd(scan, 16) == OK)
            return parseNumber(scan, val, 16);
        if (c == 'B' && scanNumberEnd(scan, 2) == OK)
            return parseNumber(scan, val, 2);
        scan = save;
    }
    if (isdigit(*scan) && scanNumberEnd(scan, 10) == OK)
        return parseNumber(scan, val, 10);
    return setError(scan, ILLEGAL_CONSTANT);
}


bool MotorolaValueParser::numberPrefix(const StrScanner &scan) const {
    const char c = *scan;
    if (c == '$' || c == '@' || c == '%')
        return true;
    return ValueParser::numberPrefix(scan);
}

Error MotorolaValueParser::readNumber(StrScanner &scan, Value &val) {
    const StrScanner save(scan);
    const char c = *scan++;
    if (c == '$' && scanNumberEnd(scan, 16) == OK)
        return parseNumber(scan, val, 16);
    if (c == '@' && scanNumberEnd(scan, 8) == OK)
        return parseNumber(scan, val, 8);
    if (c == '%' && scanNumberEnd(scan, 2) == OK)
        return parseNumber(scan, val, 2);
    scan = save;
    return ValueParser::readNumber(scan, val);
}

Error IntelValueParser::readNumber(StrScanner &scan, Value &val) {
    if (scanNumberEnd(scan, 16, 'H') == OK)
        return parseNumber(scan, val, 16, 'H');
    if (scanNumberEnd(scan, 8, 'O') == OK)
        return parseNumber(scan, val, 8, 'O');
    if (scanNumberEnd(scan, 2, 'B') == OK)
        return parseNumber(scan, val, 2, 'B');
    return ValueParser::readNumber(scan, val);
}

bool NationalValueParser::isSymbolLetter(char c, bool head) const {
    if (head && c == '$')
        return true;
    return ValueParser::isSymbolLetter(c, head);
}

bool NationalValueParser::numberPrefix(const StrScanner &scan) const {
    const char c = toupper(*scan);
    const char q = scan[1];
    if (q == '\'')
        return c == 'H' || c == 'X' || c == 'D' || c == 'O' || c == 'Q' || c == 'B';
    return ValueParser::numberPrefix(scan);
}

Error NationalValueParser::readNumber(StrScanner &scan, Value &val) {
    const StrScanner save(scan);
    const char c = toupper(*scan++);
    const char q = *scan++;
    if (q == '\'') {
        if ((c == 'H' || c == 'X') && scanNumberEnd(scan, 16) == OK)
            return parseNumber(scan, val, 16, '\'');
        if (c == 'D' && scanNumberEnd(scan, 10) == OK)
            return parseNumber(scan, val, 10, '\'');
        if ((c == 'O' || c == 'Q') && scanNumberEnd(scan, 8) == OK)
            return parseNumber(scan, val, 8, '\'');
        if (c == 'B' && scanNumberEnd(scan, 2) == OK)
            return parseNumber(scan, val, 2, '\'');
    }
    // TODO: Support Decimal(0[fFlL]) and Hexadecimal([fFlL]'/0[yYzZ]) floating ponit constant
    scan = save;
    return IntelValueParser::readNumber(scan, val);
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
