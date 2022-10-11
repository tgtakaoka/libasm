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

#ifndef __DIS_BASE_H__
#define __DIS_BASE_H__

#include <stddef.h>
#include <stdint.h>

#include "config_base.h"
#include "dis_memory.h"
#include "error_reporter.h"
#include "insn_base.h"
#include "option_base.h"
#include "reg_base.h"
#include "str_buffer.h"
#include "symbol_table.h"
#include "table_base.h"
#include "type_traits.h"
#include "value_formatter.h"

namespace libasm {

class Disassembler : public ErrorReporter {
public:
    Error decode(DisMemory &memory, Insn &insn, char *operands, size_t size,
            SymbolTable *symtab = nullptr);
    virtual const ConfigBase &config() const = 0;
    virtual void reset() {}

    ValueFormatter &formatter() { return _formatter; }

    const /*PROGMEM*/ char *listCpu_P() const { return _table.listCpu_P(); }
    const /*PROGMEM*/ char *cpu_P() const { return _table.cpu_P(); }
    bool setCpu(const char *cpu) { return _table.setCpu(cpu); }

    Error setOption(const char *name, const char *text) {
        if (_commonOptions.setOption(name, text) == OK)
            return getError();
        return options().setOption(name, text);
    }
    const Options &commonOptions() const { return _commonOptions; }
    virtual const Options &options() const { return Options::EMPTY; }

    Error checkAddress(uint32_t addr);

private:
    ValueFormatter &_formatter;

    static const char OPT_BOOL_RELATIVE[] PROGMEM;
    static const char OPT_DESC_RELATIVE[] PROGMEM;
    static const char OPT_BOOL_UPPERCASE[] PROGMEM;
    static const char OPT_DESC_UPPERCASE[] PROGMEM;
    static const char OPT_BOOL_CSTYLE[] PROGMEM;
    static const char OPT_DESC_CSTYLE[] PROGMEM;
    static const char OPT_CHAR_ORIGIN[] PROGMEM;
    static const char OPT_DESC_ORIGIN[] PROGMEM;

protected:
    RegBase &_regBase;
    TableBase &_table;
    char _curSym;
    bool _relativeTarget = false;
    SymbolTable *_symtab = nullptr;
    const CharOption _opt_curSym{OPT_CHAR_ORIGIN, OPT_DESC_ORIGIN, _curSym};
    const struct OptCStyle : public BoolOptionBase {
        OptCStyle(Disassembler *dis, const OptionBase &next)
            : BoolOptionBase(OPT_BOOL_CSTYLE, OPT_DESC_CSTYLE, next), _dis(dis) {}
        Error set(bool value) const override {
            _dis->_formatter.setCStyle(value);
            return OK;
        }
        Disassembler *_dis;
    } _opt_cstyle{this, _opt_curSym};
    const struct OptUppercase : public BoolOptionBase {
        OptUppercase(Disassembler *dis, const OptionBase &next)
            : BoolOptionBase(OPT_BOOL_UPPERCASE, OPT_DESC_UPPERCASE, next), _dis(dis) {}
        Error set(bool value) const override {
            _dis->_formatter.setUppercase(value);
            _dis->_regBase.setUppercase(value);
            return OK;
        }
        Disassembler *_dis;
    } _opt_uppercase{this, _opt_cstyle};
    const BoolOption _opt_relative{
            OPT_BOOL_RELATIVE, OPT_DESC_RELATIVE, _relativeTarget, _opt_uppercase};
    const Options _commonOptions{_opt_relative};

    Disassembler(ValueFormatter &formatter, RegBase &regs, TableBase &table, char curSym)
        : _formatter(formatter), _regBase(regs), _table(table), _curSym(curSym) {}

    /** Lookup |addr| value and returns symbol. */
    template <typename Addr>
    const char *lookup(Addr addr) const {
        const char *symbol = nullptr;
        if (_symtab) {
            symbol = _symtab->lookupValue(addr);
            if (!symbol) {
                auto value = static_cast<typename make_signed<Addr>::type>(addr);
                symbol = _symtab->lookupValue(static_cast<int32_t>(value));
            }
        }
        return symbol;
    }

    /**
     * Convert |val| as |bits| decimal integer. Treat |val| as signed
     * integer when |bits| is negative.
     */
    template <typename T>
    StrBuffer &outDec(StrBuffer &out, T val, int8_t bits) const {
        const char *label = lookup(val);
        if (label)
            return out.text(label);
        return _formatter.formatDec(out, val, bits);
    }

    /**
     * Convert |val| as |bits| hexadecimal integer. Treat |val| as
     * signed integer when |bits| is negative. Output symbol label when
     * |val| is in symbol table.
     */
    template <typename T>
    StrBuffer &outHex(StrBuffer &out, T val, int8_t bits, bool relax = true) const {
        const char *label = lookup(val);
        if (label)
            return out.text(label);
        return _formatter.formatHex(out, val, bits, relax);
    }

    /**
     * Convert |val| as |addrWidth| bit absolute address. Use default
     * configured address width when |addrWdith| is ommitted. Output
     * symbol label when |val| is in symbol table.
     */
    template <typename Addr>
    StrBuffer &outAbsAddr(StrBuffer &out, Addr val, uint8_t addrWidth = 0) const {
        const char *label = lookup(val);
        if (label)
            return out.text(label);
        if (addrWidth == 0)
            addrWidth = uint8_t(config().addressWidth());
        return _formatter.formatHex(out, val, addrWidth, false);
    }

    /**
     * Convert |target| as relative |deltaBits| offset from |origin|.
     */
    template <typename Addr>
    StrBuffer &outRelAddr(StrBuffer &out, Addr target, Addr origin, uint8_t deltaBits) const {
        if (!_relativeTarget)
            return outAbsAddr(out, target);
        out.letter(_curSym);
        const auto delta = static_cast<typename make_signed<Addr>::type>(target - origin);
        if (delta == 0)
            return out;
        Addr val;
        if (delta < 0) {
            out.letter('-');
            val = static_cast<Addr>(-delta);
        } else {
            out.letter('+');
            val = static_cast<Addr>(delta);
        }
        if (deltaBits <= 14) {
            return _formatter.formatDec(out, val, deltaBits);
        } else {
            return _formatter.formatHex(out, val, deltaBits);
        }
    }

private:
    virtual Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) = 0;
};

}  // namespace libasm

#endif  // __DIS_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
