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

Error Assembler::checkAddress(uint32_t addr, const ErrorAt &at) {
    const uint32_t max = 1UL << config().addressWidth();
    if (max && (addr & ~(max - 1)))
        return setError(at, OVERFLOW_RANGE);
    if (config().opCodeWidth() == OPCODE_16BIT && config().addressUnit() == ADDRESS_BYTE) {
        if (addr % 2)
            return setError(at, INSTRUCTION_NOT_ALIGNED);
    }
    return OK;
}

Error Assembler::encode(const char *line, Insn &insn, SymbolTable *symtab) {
    _symtab = symtab;
    _parser.setCurrentOrigin(insn.address());
    setAt(line);
    if (checkAddress(insn.address(), *this))
        return getError();
    StrScanner scan(line);
    setError(scan, OK);
    if (_parser.commentLine(scan.skipSpaces()))
        return setError(scan, OK);

    const auto symbol = _parser.readSymbol(scan);
    if (symbol.size() == 0)
        return setError(scan, UNKNOWN_INSTRUCTION);
    insn.clearNameBuffer().text(symbol);
    auto error = _pseudos.processPseudo(scan, insn, *this);
    if (error != UNKNOWN_DIRECTIVE)
        return setError(error);

    error = encodeImpl(scan, insn);
    if (error == UNKNOWN_INSTRUCTION)
        setAt(line);
    return error;
}

uint16_t Assembler::parseExpr16(StrScanner &expr, ErrorAt &error) const {
    auto p = expr;
    const auto value = _parser.eval(p, error, _symtab);
    if (value.overflowUint16())
        error.setErrorIf(expr, OVERFLOW_RANGE);
    expr = p;
    return value.getUnsigned();
}

uint32_t Assembler::parseExpr32(StrScanner &expr, ErrorAt &error) const {
    return _parser.eval(expr, error, _symtab).getUnsigned();
}

Value Assembler::parseExpr(StrScanner &expr, ErrorAt &error) const {
    return _parser.eval(expr, error, _symtab);
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
