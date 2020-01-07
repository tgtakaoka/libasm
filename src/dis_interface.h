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
    virtual Error decode(
        DisMemory<target::uintptr_t> &memory,
        Insn& insn,
        char *operands,
        SymbolTable *symtab) = 0;
};

template<typename Addr>
class DisCommon : public Disassembler<Addr> {
protected:
    char *_operands;
    SymbolTable *_symtab;

    void reset(char *operands, SymbolTable *symtab) {
        *(_operands = operands) = 0;
        _symtab = symtab;
        ErrorReporter::resetError();
    }
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

    virtual DisOperand *getEncoder() = 0;
    template<typename T>
    void outConstant(T val, int8_t radix = 16, bool relax = true) {
        if (is_signed<T>::value) radix = -radix;
        _operands = getEncoder()->output(
            _operands, val, radix, relax, static_cast<uint8_t>(sizeof(T)));
    }
};

#endif // __DIS_INTERFACE_H__
