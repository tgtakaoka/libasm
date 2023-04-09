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

static const char OPT_BOOL_UPPER_HEX[] PROGMEM = "upper-hex";
static const char OPT_DESC_UPPER_HEX[] PROGMEM = "use upper case letter for hexadecimal";
static const char OPT_BOOL_UPPERCASE[] PROGMEM = "uppercase";
static const char OPT_DESC_UPPERCASE[] PROGMEM = "use upper case letter for output";
static const char OPT_BOOL_RELATIVE[] PROGMEM = "relative";
static const char OPT_DESC_RELATIVE[] PROGMEM = "program counter relative branch target";
static const char OPT_BOOL_CSTYLE[] PROGMEM = "c-style";
static const char OPT_DESC_CSTYLE[] PROGMEM = "C language style number constant";
static const char OPT_CHAR_ORIGIN[] PROGMEM = "origin-char";
static const char OPT_DESC_ORIGIN[] PROGMEM = "letter for origin symbol";

Disassembler::Disassembler(const HexFormatter &hexFormatter, entry::Table &table, char curSym,
        const OptionBase *option)
    : _formatter(hexFormatter),
      _table(table),
      _commonOptions(&_opt_relative),
      _options(option),
      _opt_relative(this, &Disassembler::setRelativeTarget, OPT_BOOL_RELATIVE, OPT_DESC_RELATIVE,
              _opt_cstyle),
      _opt_cstyle(this, &Disassembler::setCStyle, OPT_BOOL_CSTYLE, OPT_DESC_CSTYLE, _opt_curSym),
      _opt_curSym(this, &Disassembler::setCurSym, OPT_CHAR_ORIGIN, OPT_DESC_ORIGIN),
      _defaultCurSym(curSym) {
    reset();
}

void Disassembler::reset() {
    setUpperHex(true);
    setUppercase(true);
    setRelativeTarget(false);
    setCStyle(false);
    setCurSym(0);
}

Error Disassembler::setUpperHex(bool enable) {
    _formatter.setUpperHex(enable);
    return OK;
}

Error Disassembler::setUppercase(bool enable) {
    _uppercase = enable;
    return OK;
}

Error Disassembler::setRelativeTarget(bool enable) {
    _relativeTarget = enable;
    return OK;
}

Error Disassembler::setCStyle(bool enable) {
    formatter().setCStyle(enable);
    return OK;
}

Error Disassembler::setCurSym(char curSym) {
    _curSym = curSym ? curSym : _defaultCurSym;
    return OK;
}

Error Disassembler::decode(
        DisMemory &memory, Insn &insn, char *operands, size_t size, SymbolTable *symtab) {
    _symtab = symtab;
    // This setError also reset error of Disassembler.
    if (setError(config().checkAddr(insn.address())))
        return getError();
    UppercaseBuffer upper(operands, size);
    LowercaseBuffer lower(operands, size);
    StrBuffer *out = _uppercase ? upper.ptr() : lower.ptr();
    decodeImpl(memory, insn, *out);
    if (isOK())
        setError(*out);
    return getError();
}

const char *Disassembler::lookup(uint32_t addr, uint8_t addrWidth) const {
    const char *symbol = nullptr;
    if (_symtab) {
        symbol = _symtab->lookupValue(addr);
        if (!symbol)
            symbol = _symtab->lookupValue(config().signExtend(addr, addrWidth));
    }
    return symbol;
}

StrBuffer &Disassembler::outDec(StrBuffer &out, uint32_t val, int8_t bits) const {
    const auto bw = bits >= 0 ? bits : -bits;
    const char *label = lookup(val, bw);
    if (label)
        return out.rtext(label);
    return _formatter.formatDec(out, val, bits);
}

/**
 * Convert |val| as |bits| hexadecimal integer. Treat |val| as
 * signed integer when |bits| is negative. Output symbol label when
 * |val| is in symbol table.
 */
StrBuffer &Disassembler::outHex(StrBuffer &out, uint32_t val, int8_t bits, bool relax) const {
    const auto bw = bits >= 0 ? bits : -bits;
    const char *label = lookup(val, bw);
    if (label)
        return out.rtext(label);
    return _formatter.formatHex(out, val, bits, relax);
}

/**
 * Convert |val| as |addrWidth| bit absolute address. Use default
 * configured address width when |addrWdith| is ommitted. Output
 * symbol label when |val| is in symbol table.
 */
StrBuffer &Disassembler::outAbsAddr(StrBuffer &out, uint32_t val, uint8_t addrWidth) const {
    const char *label = lookup(val, addrWidth);
    if (label)
        return out.rtext(label);
    if (addrWidth == 0)
        addrWidth = uint8_t(config().addressWidth());
    return _formatter.formatHex(out, val, addrWidth, false);
}

/**
 * Convert |target| as relative |deltaBits| offset from |origin|.
 */
StrBuffer &Disassembler::outRelAddr(
        StrBuffer &out, uint32_t target, uint32_t origin, uint8_t deltaBits) const {
    if (!_relativeTarget)
        return outAbsAddr(out, target);
    const char *label = lookup(target, config().addressWidth());
    if (label)
        return out.rtext(label);
    out.letter(_curSym);
    const auto delta = static_cast<int32_t>(target - origin);
    if (delta == 0)
        return out;
    uint32_t val;
    if (delta < 0) {
        out.letter('-');
        val = static_cast<uint32_t>(-delta);
    } else {
        out.letter('+');
        val = static_cast<uint32_t>(delta);
    }
    if (deltaBits <= 14) {
        return _formatter.formatDec(out, val, deltaBits);
    } else {
        return _formatter.formatHex(out, val, deltaBits);
    }
}

uint32_t Disassembler::branchTarget(uint32_t base, int32_t delta) {
    const auto target = base + delta;
    const auto err = config().checkAddr(target);
    if (err)
        setErrorIf(err);
    return target;
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
