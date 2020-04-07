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

#ifndef __DIS_INTERFACE_H__
#define __DIS_INTERFACE_H__

#include "error_reporter.h"
#include "symbol_table.h"
#include "dis_operand.h"
#include "type_traits.h"

#include <ctype.h>

namespace libasm {

template<typename Addr>
class Disassembler : public ErrorReporter {
public:
    typedef Addr addr_t;

    Error decode(
        DisMemory<target::uintptr_t> &memory, Insn& insn,
        char *operands, SymbolTable *symtab, bool uppercase = false) {
        insn.resetAddress(memory.address());
        *(_operands = operands) = 0;
        _symtab = symtab;
        this->resetError();
        getFormatter().setUppercase(uppercase);
        getRegister().setUppercase(uppercase);
        decode(memory, insn);
        if (!uppercase) insn.toLowerName();
        return getError();
    }
    virtual DisOperand &getFormatter() = 0;
    virtual bool setCpu(const char *cpu) = 0;
    virtual const char *listCpu() const = 0;
    virtual Endian endian() const = 0;
    virtual host::uint_t maxBytes() const = 0;
    virtual host::uint_t maxName() const = 0;

protected:
    char *_operands;
    SymbolTable *_symtab;

    const char *lookup(uint32_t addr) const {
        const char *symbol = nullptr;
        if (_symtab) {
            symbol = _symtab->lookup(addr);
            if (!symbol) {
                auto value =
                    static_cast<typename make_signed<Addr>::type>(addr);
                symbol = _symtab->lookup(static_cast<int32_t>(value));
            }
        }
        return symbol;
    }
    void outText(const char *text) {
        char *p = _operands;
        while ((*p = *text++) != 0)
            p++;
        _operands = p;
    }

    template<typename T>
    void outConstant(T val, int8_t radix = 16, bool relax = true) {
        if (is_signed<T>::value) radix = -radix;
        _operands = this->getFormatter().output(
            _operands, val, radix, relax, static_cast<uint8_t>(sizeof(T)));
    }

private:
    virtual RegBase &getRegister();
    virtual Error decode(
        DisMemory<target::uintptr_t> &memory, Insn& insn) = 0;
};

} // namespace libasm

#endif // __DIS_INTERFACE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
