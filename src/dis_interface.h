/* -*- mode: c++; -*- */
#ifndef __DIS_INTERFACE_H__
#define __DIS_INTERFACE_H__

template<typename Addr>
class Disassembler {
public:
    virtual Error decode(
        DisMemory<target::uintptr_t> &memory,
        Insn& insn,
        char *operands,
        SymbolTable<Addr> *symtab) = 0;
};

#endif // __DIS_INTERFACE_H__
