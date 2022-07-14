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

const char Disassembler::OPT_BOOL_RELATIVE[] PROGMEM = "relative";
const char Disassembler::OPT_BOOL_UPPERCASE[] PROGMEM = "uppercase";
const char Disassembler::OPT_BOOL_CSTYLE[] PROGMEM = "c-style";
const char Disassembler::OPT_CHAR_ORIGIN[] PROGMEM = "origin-char";

Error Disassembler::decode(
        DisMemory &memory, Insn &insn, char *operands, size_t size, SymbolTable *symtab) {
    _symtab = symtab;
    resetError();
    StrBuffer out(operands, size);
    decode(memory, insn, out);
    if (!_regBase.isUppercase())
        insn.toLowerName();
    if (isOK())
        setError(out);
    return getError();
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
