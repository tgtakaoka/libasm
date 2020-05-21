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
#include "symbol_table.h"
#include "value_parser.h"

#include <ctype.h>

namespace libasm {

Error Assembler::encode(
    const char *line, Insn &insn, uint32_t addr, SymbolTable *symtab) {
    this->resetError();
    _scan = skipSpaces(line);
    if (checkLineEnd() == OK)
        return setError(NO_INSTRUCTION);
    this->resetError();
    _symtab = symtab;
    insn.resetAddress(addr);
    return encode(insn);
}

void Assembler::reset(const char *line, SymbolTable *symtab) {
    _scan = line;
    _symtab = symtab;
    resetError();
}

bool Assembler::hasSymbol(const char *symbol) const {
    return _symtab && _symtab->hasSymbol(symbol);
}

uint32_t Assembler::lookupSymbol(const char *symbol) const {
    return _symtab ? _symtab->lookupSymbol(symbol) : 0;
}

bool Assembler::endOfLine(const char *scan) {
    return *scan == 0 || *scan == ';';
}

Error Assembler::checkLineEnd(const char *scan) {
    if (scan == nullptr) scan = _scan;
    if (endOfLine(skipSpaces(scan)))
        return setError(getError());
    return setError(GARBAGE_AT_END);
}

const char *Assembler::skipSpaces(const char *scan) {
    while (isspace(*scan))
        scan++;
    return scan;
}

} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
