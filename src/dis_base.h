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
    Error decode(
        DisMemory &memory, Insn &insn, char *operands, SymbolTable *symtab);

    virtual ValueFormatter &getFormatter() = 0;
    void setRelativeTarget(bool prefer) { _relativeTarget = prefer; }
    void setUppercase(bool uppercase) { _uppercase = uppercase; }
    virtual void reset() {}

    const char *listCpu() const { return getTable().listCpu(); }
    bool setCpu(const char *cpu) { return getTable().setCpu(cpu); }
    const char *getCpu() const { return getTable().getCpu(); }

protected:
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
    char *outConstant(
            char *out,
            T val,
            int8_t radix = 16,
            bool relax = true,
            bool symbol = true,
            uint8_t bitWidth = sizeof(T) * 8) {
        if (symbol) {
            const char *label = lookup(val);
            if (label) return outText(out, label);
        }
        const int8_t r = is_signed<T>::value ? -radix : radix;
        return this->getFormatter().output(out, val, r, relax, bitWidth);
    }

    template<typename Addr>
    char *outAddress(
            char *out,
            Addr val,
            const /*PROGMEM*/ char *prefix = nullptr,
            bool needPrefix = false,
            uint8_t addrWidth = sizeof(Addr) * 8) {
        const char *label = lookup(val);
        if (label) {
            if (prefix) out = outPstr(out, prefix);
            return outText(out, label);
        }
        if (needPrefix && prefix) out = outPstr(out, prefix);
        return this->getFormatter().output(out, val, 16, false, addrWidth);
    }

    template<typename Addr>
    char *outRelativeAddr(
            char *out, Addr target, Addr origin, int8_t deltaBits) {
        if (!_relativeTarget)
            return outAddress(out, target, nullptr, false, addressWidth());
        const int32_t delta = static_cast<
            typename make_signed<Addr>::type>(target - origin);
        out = outText(out, getFormatter().currentOriginSymbol());
        if (delta > 0) *out++ = '+';
        if (delta < 0) *out++ = '-';
        const uint32_t disp = (delta < 0) ? -delta : delta;
        if (disp) out = outConstant(out, disp, 16, true, true, deltaBits);
        return out;
    }

private:
    virtual Error decode(DisMemory &memory, Insn &insn, char *out) = 0;
    virtual TableBase &getTable() const = 0;
    virtual RegBase &getRegister() = 0;
};

} // namespace libasm

#endif // __DIS_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
