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
const char Disassembler::OPT_DESC_RELATIVE[] PROGMEM = "program counter relative branch target";
const char Disassembler::OPT_BOOL_UPPERCASE[] PROGMEM = "uppercase";
const char Disassembler::OPT_DESC_UPPERCASE[] PROGMEM = "uppercase instruction and register name";
const char Disassembler::OPT_BOOL_CSTYLE[] PROGMEM = "c-style";
const char Disassembler::OPT_DESC_CSTYLE[] PROGMEM = "C language style number constant";
const char Disassembler::OPT_CHAR_ORIGIN[] PROGMEM = "origin-char";
const char Disassembler::OPT_DESC_ORIGIN[] PROGMEM = "letter for origin symbol";

Error Disassembler::checkAddress(uint32_t addr) {
    const uint32_t max = 1UL << config().addressWidth();
    if (max && (addr & ~(max - 1)))
        return setError(OVERFLOW_RANGE);
    if (config().opCodeWidth() == OPCODE_16BIT && config().addressUnit() == ADDRESS_BYTE) {
        if (addr % 2)
            return setError(INSTRUCTION_NOT_ALIGNED);
    }
    return setOK();
}

Error Disassembler::decode(
        DisMemory &memory, Insn &insn, char *operands, size_t size, SymbolTable *symtab) {
    _symtab = symtab;
    if (checkAddress(insn.address()))
        return getError();
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
