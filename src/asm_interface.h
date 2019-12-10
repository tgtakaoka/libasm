/* -*- mode: c++; -*- */
#ifndef __ASM_INTERFACE_H__
#define __ASM_INTERFACE_H__

template<typename Addr>
class Assembler {
public:
    virtual Error encode(
        const char *line,
        Insn &insn,
        Addr addr,
        SymbolTable<Addr> *symtab) = 0;
};

#endif // __ASM_INTERFACE_H__
