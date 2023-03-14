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

static const char OPT_BOOL_RELATIVE[] PROGMEM = "relative";
static const char OPT_DESC_RELATIVE[] PROGMEM = "program counter relative branch target";
static const char OPT_BOOL_UPPERCASE[] PROGMEM = "uppercase";
static const char OPT_DESC_UPPERCASE[] PROGMEM = "uppercase instruction and register name";
static const char OPT_BOOL_CSTYLE[] PROGMEM = "c-style";
static const char OPT_DESC_CSTYLE[] PROGMEM = "C language style number constant";
static const char OPT_CHAR_ORIGIN[] PROGMEM = "origin-char";
static const char OPT_DESC_ORIGIN[] PROGMEM = "letter for origin symbol";

Disassembler::Disassembler(ValueFormatter &formatter, RegBase &regs, TableBase &table, char curSym)
    : _formatter(formatter),
      _regBase(regs),
      _table(table),
      _opt_curSym(OPT_CHAR_ORIGIN, OPT_DESC_ORIGIN, _curSym),
      _opt_cstyle(this, _opt_curSym),
      _opt_uppercase(this, _opt_cstyle),
      _opt_relative(OPT_BOOL_RELATIVE, OPT_DESC_RELATIVE, _relativeTarget, _opt_uppercase),
      _commonOptions(_opt_relative),
      _curSym(curSym) {}

Disassembler::OptCStyle::OptCStyle(Disassembler *dis, const OptionBase &next)
    : BoolOptionBase(OPT_BOOL_CSTYLE, OPT_DESC_CSTYLE, next), DisassemblerOption(dis) {}

Error Disassembler::OptCStyle::set(bool value) const {
    _dis->_formatter.setCStyle(value);
    return OK;
}

Disassembler::OptUppercase::OptUppercase(Disassembler *dis, const OptionBase &next)
    : BoolOptionBase(OPT_BOOL_UPPERCASE, OPT_DESC_UPPERCASE, next), DisassemblerOption(dis) {}

Error Disassembler::OptUppercase::set(bool value) const {
    _dis->_formatter.setUppercase(value);
    _dis->_regBase.setUppercase(value);
    return OK;
}

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
    char *mark = insn.clearNameBuffer().mark();
    decodeImpl(memory, insn, out);
    if (!_regBase.isUppercase())
        insn.nameBuffer().lowercase(mark);
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
