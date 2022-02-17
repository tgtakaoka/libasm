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

namespace libasm {

Error Assembler::encode(const char *line, Insn &insn, SymbolTable *symtab) {
    _symtab = symtab;
    _parser.setCurrentOrigin(insn.address());
    resetError();
    StrScanner scan(line);
    setAt(scan.skipSpaces());
    if (endOfLine(*scan))
        return OK;
    const auto error = encode(scan, insn);
    if (error == OK)
        setAt(scan);
    return error;
}

bool Assembler::hasSymbol(const char *symbol) const {
    return _symtab && _symtab->hasSymbol(symbol);
}

uint32_t Assembler::lookupSymbol(const char *symbol) const {
    return _symtab ? _symtab->lookupSymbol(symbol) : 0;
}

bool Assembler::endOfLine(char letter) const {
    return letter == 0 || letter == ';' || letter == _commentChar;
}

uint16_t Assembler::parseExpr16(StrScanner &expr, ErrorReporter &error) {
    Value value = _parser.eval(expr, _symtab);
    setError(_parser);
    if (value.overflowUint16())
        error.setErrorIf(expr, OVERFLOW_RANGE);
    if (value.isUndefined())
        error.setErrorIf(expr, UNDEFINED_SYMBOL);
    return value.getUnsigned();
}

uint32_t Assembler::parseExpr32(StrScanner &expr, ErrorReporter &error) {
    Value value = _parser.eval(expr, _symtab);
    setError(_parser);
    if (value.isUndefined())
        error.setErrorIf(expr, UNDEFINED_SYMBOL);
    return value.getUnsigned();
}

StrScanner Assembler::scanExpr(const StrScanner &expr, char delim) const {
    StrScanner p(expr);
    while (!endOfLine(*p)) {
        if (*p == delim)
            return StrScanner(expr, p);
        if (p.expect('\'')) {
            _parser.readChar(p);
            if (!p.expect('\''))
                break;
        } else if (*p == '(' || *p == '[') {
            const char close = (*p++ == '(') ? ')' : ']';
            const StrScanner atom = scanExpr(p, close);
            if (atom.size() == 0)
                break;
            p += atom.size() + 1;
        } else {
            ++p;
        }
    }
    return StrScanner::EMPTY;
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
