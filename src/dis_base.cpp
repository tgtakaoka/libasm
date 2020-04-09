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

#include "dis_base.h"

namespace libasm {

Error Disassembler::decode(
    DisMemory &memory, Insn &insn,
    char *operands, SymbolTable *symtab, bool uppercase) {
    insn.resetAddress(memory.address());
    *(_operands = operands) = 0;
    _symtab = symtab;
    this->resetError();
    getFormatter().setUppercase(uppercase);
    getRegister().setUppercase(uppercase);
    decode(memory, insn);
    if (!uppercase) insn.toLowerName();
    return getError();
}

void Disassembler::outText(const char *text) {
    char *p = _operands;
    while ((*p = *text++) != 0)
        p++;
    _operands = p;
}

} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
