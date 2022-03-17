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

bool Assembler::endOfLine(char letter) const {
    return letter == 0 || letter == ';' || letter == _commentChar;
}

uint16_t Assembler::parseExpr16(StrScanner &expr, ErrorAt &error) {
    const Value value = _parser.eval(expr, _symtab);
    setError(_parser);
    if (value.overflowUint16())
        error.setErrorIf(expr, OVERFLOW_RANGE);
    if (value.isUndefined())
        error.setErrorIf(expr, UNDEFINED_SYMBOL);
    return value.getUnsigned();
}

uint32_t Assembler::parseExpr32(StrScanner &expr, ErrorAt &error) {
    const Value value = _parser.eval(expr, _symtab);
    setError(_parser);
    if (value.isUndefined())
        error.setErrorIf(expr, UNDEFINED_SYMBOL);
    return value.getUnsigned();
}

Value Assembler::parseExpr(StrScanner &expr, ErrorAt &error) {
    const Value value = _parser.eval(expr, _symtab);
    setError(_parser);
    if (value.isUndefined())
        error.setErrorIf(expr, UNDEFINED_SYMBOL);
    return value;
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
