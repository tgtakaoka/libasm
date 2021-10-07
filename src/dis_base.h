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

#include <ctype.h>

#include "array_memory.h"
#include "config_base.h"
#include "dis_memory.h"
#include "error_reporter.h"
#include "insn_base.h"
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
    virtual ConfigBase &config() = 0;
    virtual void reset() {}

    ValueFormatter &getFormatter() { return _formatter; }
    void setRelativeTarget(bool prefer) { _relativeTarget = prefer; }
    void setUppercase(bool uppercase);
    void setCurrentOriginSymbol(char curSym) { _curSym = curSym; }

    const char *listCpu() const { return _table.listCpu(); }
    bool setCpu(const char *cpu) { return _table.setCpu(cpu); }
    const char *getCpu() const { return _table.getCpu(); }

protected:
    ValueFormatter &_formatter;
    RegBase &_regBase;
    TableBase &_table;
    SymbolTable *_symtab;
    char _curSym;
    bool _relativeTarget = false;

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
    StrBuffer &outDec(StrBuffer &out, T val, int8_t bits) {
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
    StrBuffer &outHex(StrBuffer &out, T val, int8_t bits, bool relax = true) {
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
    StrBuffer &outAbsAddr(StrBuffer &out, Addr val, uint8_t addrWidth = 0) {
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
    StrBuffer &outRelAddr(StrBuffer &out, Addr target, Addr origin, uint8_t deltaBits) {
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
    virtual Error decode(DisMemory &memory, Insn &insn, StrBuffer &out) = 0;
};

}  // namespace libasm

#endif  // __DIS_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
