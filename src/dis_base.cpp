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

void Disassembler::setUppercase(bool uppercase) {
    _formatter.setUppercase(uppercase);
    _regBase.setUppercase(uppercase);
}

Error Disassembler::decode(
        DisMemory &memory, Insn &insn, char *operands, SymbolTable *symtab) {
    _symtab = symtab;

    resetError();
    *operands = 0;
    insn.resetAddress(memory.address());
    decode(memory, insn, operands);
    if (!_regBase.isUppercase())
        insn.toLowerName();
    return getError();
}

char *Disassembler::outText(char *out, const char *text) const {
    while ((*out = *text++) != 0)
        out++;
    return out;
}

char *Disassembler::outPstr(char *out, const /*PROGMEM*/ char *pstr) const {
    while ((*out = pgm_read_byte(pstr++)) != 0)
        out++;
    return out;
}

char *Disassembler::outDec(char *out, uint8_t val, int8_t bits) {
    const char *label = lookup(val);
    if (label)
        return outText(out, label);
    return _formatter.formatDec(out, val, bits);
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
