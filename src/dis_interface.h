/* -*- mode: c++; -*- */
#ifndef __DIS_INTERFACE_H__
#define __DIS_INTERFACE_H__

#include "error_reporter.h"
#include "symbol_table.h"
#include "dis_operand.h"
#include "type_traits.h"

template<typename Addr>
class Disassembler : public ErrorReporter {
public:
    typedef Addr addr_t;

    Error decode(
        DisMemory<target::uintptr_t> &memory, Insn& insn,
        char *operands, SymbolTable *symtab) {
        insn.resetAddress(memory.address());
        *(_operands = operands) = 0;
        _symtab = symtab;
        this->resetError();
        return decode(memory, insn);
    }

    virtual DisOperand &getFormatter() = 0;

protected:
    char *_operands;
    SymbolTable *_symtab;

    const char *lookup(Addr addr) const {
        const char *symbol = nullptr;
        if (_symtab) {
            symbol = _symtab->lookup(addr);
            if (!symbol) {
                auto value =
                    static_cast<typename make_signed<Addr>::type>(addr);
                symbol = _symtab->lookup(value);
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
    virtual Error decode(
        DisMemory<target::uintptr_t> &memory, Insn& insn) = 0;
};

#endif // __DIS_INTERFACE_H__
