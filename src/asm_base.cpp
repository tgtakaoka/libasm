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

Assembler::Assembler(const OptionBase *option, const ValueParser::Plugins &plugins)
    : ErrorAt(), _options(option), _parser(plugins, *this) {}

Error Assembler::setCurrentLocation(uint32_t location) {
    return config().checkAddr(_currentLocation = location);
}

Error Assembler::encode(const char *line, Insn &insn, SymbolTable *symtab) {
    _symtab = symtab;
    setAt(line);
    const auto err = setCurrentLocation(insn.address());
    if (err)
        return setError(err);

    StrScanner scan(line);
    setError(scan, OK);
    if (_parser.commentLine(scan.skipSpaces()))
        return setError(scan, OK);

    const auto symbol = _parser.readSymbol(scan);
    if (symbol.size() == 0)
        return setError(scan, UNKNOWN_INSTRUCTION);
    insn.clearNameBuffer().text(symbol);
    auto error = processPseudo(scan, insn);
    if (error != UNKNOWN_DIRECTIVE)
        return setError(error);

    error = encodeImpl(scan, insn);
    if (error == UNKNOWN_INSTRUCTION)
        setAt(line);
    return error;
}

Error Assembler::processPseudo(StrScanner &scan, Insn &insn) {
    return UNKNOWN_DIRECTIVE;
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

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
