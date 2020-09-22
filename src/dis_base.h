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

#include "config_base.h"
#include "dis_memory.h"
#include "error_reporter.h"
#include "insn_base.h"
#include "reg_base.h"
#include "symbol_table.h"
#include "table_base.h"
#include "type_traits.h"
#include "value_formatter.h"

#include <ctype.h>

namespace libasm {

class Disassembler
    : public ErrorReporter,
      virtual public ConfigBase {
public:
    Disassembler(ValueFormatter &formatter, RegBase &regs, TableBase &table)
        : _formatter(formatter),
          _regBase(regs),
          _table(table)
    {}

    Error decode(
        DisMemory &memory, Insn &insn, char *operands, SymbolTable *symtab);
    ValueFormatter &getFormatter() { return _formatter; }
    void setRelativeTarget(bool prefer) { _relativeTarget = prefer; }
    void setUppercase(bool uppercase) { _uppercase = uppercase; }
    virtual void reset() {}

    const char *listCpu() const { return _table.listCpu(); }
    bool setCpu(const char *cpu) { return _table.setCpu(cpu); }
    const char *getCpu() const { return _table.getCpu(); }

protected:
    ValueFormatter &_formatter;
    RegBase &_regBase;
    TableBase &_table;
    SymbolTable *_symtab;
    bool _relativeTarget = false;
    bool _uppercase = false;

    char *outText(char *out, const char *text) const {
        while ((*out = *text++) != 0)
            out++;
        return out;
    }
    char *outPstr(char *out, const /*PROGMEM*/ char *pstr) const {
        while ((*out = pgm_read_byte(pstr++)) != 0)
            out++;
        return out;
    }

    template<typename Addr>
    const char *lookup(Addr addr) const {
        const char *symbol = nullptr;
        if (_symtab) {
            symbol = _symtab->lookupValue(addr);
            if (!symbol) {
                auto value = static_cast<
                    typename make_signed<Addr>::type>(addr);
                symbol = _symtab->lookupValue(static_cast<int32_t>(value));
            }
        }
        return symbol;
    }

    template<typename T>
    char *outHex(char *out, T val, int8_t bits) {
        const char *label = lookup(val);
        if (label) return outText(out, label);
        return _formatter.formatHex(out, val, bits, /*relax*/true);
    }

    char *outDec(char *out, uint8_t val, int8_t bits);

    template<typename Addr>
    char *outAbsAddr(
        char *out, Addr val, uint8_t addrWidth = 0,
        const /*PROGMEM*/ char *prefix = nullptr, bool needPrefix = false) {
        const char *label = lookup(val);
        if (label) {
            if (prefix) out = outPstr(out, prefix);
            return outText(out, label);
        }
        if (needPrefix && prefix) out = outPstr(out, prefix);
        if (addrWidth == 0) addrWidth = sizeof(Addr) * 8;
        return _formatter.formatHex(out, val, addrWidth, false);
    }

    template<typename Addr>
    char *outRelAddr(
        char *out, Addr target, Addr origin, uint8_t deltaBits) {
        if (!_relativeTarget)
            return outAbsAddr(out, target, addressWidth());
        *out++ = _formatter.currentOriginSymbol();
        *out = 0;
        const auto delta = static_cast<
            typename make_signed<Addr>::type>(target - origin);
        if (delta == 0) return out;
        Addr val;
        if (delta < 0) {
            *out++ = '-';
            val = static_cast<Addr>(-delta);
        } else {
            *out++ = '+';
            val = static_cast<Addr>(delta);
        }
        if (deltaBits <= 14) {
            out = _formatter.formatDec(out, val, deltaBits);
        } else {
            out = _formatter.formatHex(out, val, deltaBits);
        }
        return out;
    }

private:
    virtual Error decode(DisMemory &memory, Insn &insn, char *out) = 0;
};

} // namespace libasm

#endif // __DIS_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
