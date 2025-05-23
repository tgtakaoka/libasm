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

namespace {

constexpr char OPT_BOOL_UPPER_HEX[] PROGMEM = "upper-hex";
constexpr char OPT_DESC_UPPER_HEX[] PROGMEM = "use upper case letter for hexadecimal";
constexpr char OPT_BOOL_UPPERCASE[] PROGMEM = "upper-case";
constexpr char OPT_DESC_UPPERCASE[] PROGMEM = "use upper case letter for output";
constexpr char OPT_INT_LIST_RADIX[] PROGMEM = "list-radix";
constexpr char OPT_DESC_LIST_RADIX[] PROGMEM = "set listing radix (8, 16)";
constexpr char OPT_BOOL_RELATIVE[] PROGMEM = "relative";
constexpr char OPT_DESC_RELATIVE[] PROGMEM = "program counter relative branch target";
constexpr char OPT_BOOL_CSTYLE[] PROGMEM = "c-style";
constexpr char OPT_DESC_CSTYLE[] PROGMEM = "C language style number constant";
constexpr char OPT_BOOL_INTELHEX[] PROGMEM = "intel-hex";
constexpr char OPT_DESC_INTELHEX[] PROGMEM = "Intel style hexadecimal";
constexpr char OPT_CHAR_ORIGIN[] PROGMEM = "origin-char";
constexpr char OPT_DESC_ORIGIN[] PROGMEM = "letter for origin symbol";
constexpr char OPT_BOOL_GNU_AS[] PROGMEM = "gnu-as";
constexpr char OPT_DESC_GNU_AS[] PROGMEM = "GNU assembler compatible";

}  // namespace

Disassembler::Disassembler(const ValueFormatter::Plugins &plugins, const OptionBase *option)
    : _formatter(plugins),
      _commonOptions(&_opt_upperHex),
      _options(option),
      _opt_upperHex(this, &Disassembler::setUpperHex, OPT_BOOL_UPPER_HEX, OPT_DESC_UPPER_HEX,
              &_opt_uppercase),
      _opt_uppercase(this, &Disassembler::setUppercase, OPT_BOOL_UPPERCASE, OPT_DESC_UPPERCASE,
              &_opt_listRadix),
      _opt_listRadix(this, &Disassembler::setListRadix, OPT_INT_LIST_RADIX, OPT_DESC_LIST_RADIX,
              &_opt_relative),
      _opt_relative(this, &Disassembler::setRelativeTarget, OPT_BOOL_RELATIVE, OPT_DESC_RELATIVE,
              &_opt_cstyle),
      _opt_cstyle(this, &Disassembler::setCStyle, OPT_BOOL_CSTYLE, OPT_DESC_CSTYLE, &_opt_intelhex),
      _opt_intelhex(
              this, &Disassembler::setIntelHex, OPT_BOOL_INTELHEX, OPT_DESC_INTELHEX, &_opt_curSym),
      _opt_curSym(this, &Disassembler::setCurSym, OPT_CHAR_ORIGIN, OPT_DESC_ORIGIN, &_opt_gnuAs),
      _opt_gnuAs(this, &Disassembler::setGnuAs, OPT_BOOL_GNU_AS, OPT_DESC_GNU_AS) {
    reset();
}

void Disassembler::reset() {
    setUpperHex(true);
    setUppercase(true);
    setListRadix(RADIX_16);
    setRelativeTarget(false);
    setCStyle(false);
    setIntelHex(false);
    setCurSym(0);
    setGnuAs(false);
}

const ValueFormatter &Disassembler::formatter() const {
    if (_cstyle) {
        static ValueFormatter formatter{ValueFormatter::Plugins::cstyle()};
        return formatter;
    }
    if (_intelHex) {
        static ValueFormatter formatter{ValueFormatter::Plugins::intel()};
        return formatter;
    }
    return _formatter;
}

Error Disassembler::setUpperHex(bool enable) {
    _upperHex = enable;
    return OK;
}

Error Disassembler::setUppercase(bool enable) {
    _uppercase = enable;
    return OK;
}

Error Disassembler::setListRadix(int32_t radix) {
    _listRadix = static_cast<Radix>(radix == 8 ? RADIX_8 : RADIX_16);
    return OK;
}

Error Disassembler::setRelativeTarget(bool enable) {
    _relativeTarget = enable;
    return OK;
}

Error Disassembler::setCStyle(bool enable) {
    _cstyle = enable;
    if (_cstyle && _intelHex)
        _intelHex = false;
    return OK;
}

Error Disassembler::setIntelHex(bool enable) {
    _intelHex = enable;
    if (_intelHex && _cstyle)
        _cstyle = false;
    return OK;
}

Error Disassembler::setCurSym(char curSym) {
    _curSym = curSym ? curSym : _formatter.locationSymbol();
    return OK;
}

Error Disassembler::setGnuAs(bool enable) {
    _gnuAs = enable;
    setCStyle(enable);
    setCurSym(enable ? '.' : 0);
    return OK;
}

Error Disassembler::setOption(const char *name, const char *text) {
    StrScanner scan{text};
    if (_commonOptions.setOption(name, scan) == OK)
        return OK;
    return options().setOption(name, scan);
}

Error Disassembler::decode(
        DisMemory &memory, Insn &insn, char *operands, size_t size, SymbolTable *symtab) {
    _symtab = symtab;
    // This setError also reset error of Disassembler.
    if (insn.setError(config().checkAddr(insn.address())))
        return insn.getError();
    StrCaseBuffer out(operands, size, _uppercase);
    insn.nameBuffer().reset();
    decodeImpl(memory, insn, out);
    if (insn.isOK())
        insn.setError(out);
    return insn.getError();
}

const char *Disassembler::lookup(uint32_t addr, uint8_t addrWidth) const {
    const char *symbol = nullptr;
    if (_symtab) {
        Value val;
        val.setUnsigned(addr);
        symbol = _symtab->lookupValue(val);
        if (!symbol) {
            val.setSigned(config().signExtend(addr, addrWidth));
            symbol = _symtab->lookupValue(val);
        }
    }
    return symbol;
}

StrBuffer &Disassembler::outDec(StrBuffer &out, uint32_t val, int8_t bits) const {
    const auto bw = bits >= 0 ? bits : -bits;
    const char *label = lookup(val, bw);
    if (label)
        return out.rtext(label);
    StrCaseBuffer caseOut(out, _upperHex);
    if (_listRadix == RADIX_8)
        return formatter().formatOct(caseOut, val, bits).over(out);
    return formatter().formatDec(caseOut, val, bits).over(out);
}

/**
 * Convert |val| as |bits| octal integer. Treat |val| as signed
 * integer when |bits| is negative. Output symbol label when |val| is
 * in symbol table.
 */
StrBuffer &Disassembler::outHex(StrBuffer &out, uint32_t val, int8_t bits, bool relax) const {
    const auto bw = bits >= 0 ? bits : -bits;
    const char *label = lookup(val, bw);
    if (label)
        return out.rtext(label);
    StrCaseBuffer caseOut(out, _upperHex);
    if (_listRadix == RADIX_8)
        return formatter().formatOct(caseOut, val, bits).over(out);
    if (relax) {
        auto abs = val;
        ValueFormatter::absolute(abs, bits);
        if (abs <= 32)
            return formatter().formatDec(caseOut, val, bits).over(out);
    }
    return formatter().formatHex(caseOut, val, bits).over(out);
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
    StrCaseBuffer caseOut(out, _upperHex);
    if (_listRadix == RADIX_8)
        return formatter().formatOct(caseOut, val, addrWidth).over(out);
    return formatter().formatHex(caseOut, val, addrWidth).over(out);
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
    StrCaseBuffer caseOut(out, _upperHex);
    if (_listRadix == RADIX_8)
        return formatter().formatOct(caseOut, val, deltaBits).over(out);
    if (deltaBits <= 14)
        return formatter().formatDec(caseOut, val, deltaBits).over(out);
    return outHex(caseOut, val, deltaBits, true).over(out);
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
