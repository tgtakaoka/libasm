/* -*- mode: c++; -*- */
#ifndef __DIS_R65C02_H__
#define __DIS_R65C02_H__

#include "config_r65c02.h"

#include "insn_r65c02.h"
#include "dis_memory.h"
#include "symbol_table.h"
#include "dis_interface.h"

template<McuType mcuType>
class Dis6502 : public DisCommon<target::uintptr_t> {
public:
    Error decode(
        DisMemory<target::uintptr_t> &memory,
        Insn& insn,
        char *operands,
        SymbolTable<target::uintptr_t> *symtab) override;

private:
    template<typename T>
    void outConstant(T val, uint8_t radix = 16, bool relax = true);

    Error decodeImmediate(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeAbsolute(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeZeroPage(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeRelative(DisMemory<target::uintptr_t> &memory, Insn &insn);
};

#include "dis_r65c02_impl.h"

typedef Dis6502<R6502> DisR6502;
typedef Dis6502<R65C02> DisR65c02;

#endif // __DIS_R65C02_H__
