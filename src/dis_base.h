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

protected:
    char *_operands;
    SymbolTable *_symtab;
    bool _relativeTarget = false;
    bool _uppercase = false;

    void outText(const char *text);

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
    void outConstant(T val,
                     int8_t radix = 16,
                     bool relax = true,
                     bool symbol = true,
                     uint8_t bitWidth = sizeof(T) * 8) {
        if (symbol) {
            const char *label = lookup(val);
            if (label) {
                outText(label);
                return;
            }
        }
        const int8_t r = is_signed<T>::value ? -radix : radix;
        _operands = this->getFormatter().output(
            _operands, val, r, relax, bitWidth);
    }

    template<typename Addr>
    void outRelativeAddr(Addr target, Addr origin, int8_t deltaBits) {
        const char *label = lookup(target);
        if (label) {
            outText(label);
            return;
        }
        if (!_relativeTarget) {
            outConstant(target, 16, false, true, addressWidth());
            return;
        }
        const auto delta = static_cast<
            typename make_signed<Addr>::type>(target - origin);
        outText(getFormatter().currentOriginSymbol());
        if (delta > 0) *_operands++ = '+';
        if (delta) outConstant(delta, 16, true, true, deltaBits + 1);
    }

private:
    virtual RegBase &getRegister() = 0;
    virtual Error decode(DisMemory &memory, Insn &insn) = 0;
};

} // namespace libasm

#endif // __DIS_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
