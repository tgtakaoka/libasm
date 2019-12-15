/* -*- mode: c++; -*- */
#ifndef __DIS_TMS9995_H__
#define __DIS_TMS9995_H__

#include "config_tms9995.h"

#include "insn_tms9995.h"
#include "dis_memory.h"
#include "symbol_table.h"
#include "dis_interface.h"

class DisTms9995 : public DisCommon<target::uintptr_t> {
public:
    Error decode(
        DisMemory<target::uintptr_t> &memory,
        Insn& insn,
        char *operands,
        SymbolTable<target::uintptr_t> *symtab) override;

private:
    template<typename T>
    void outConstant(T val, uint8_t radix = 16, bool relax = true);
    void outAddress(target::uintptr_t addr, bool relax = true);
    void outRegister(host::uint_t regno);

    Error decodeOperand(
        DisMemory<target::uintptr_t> &memory, Insn& insn, host::uint_t opr);
    Error decodeImmediate(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeRelative(Insn &insn);
};

#endif // __DIS_TMS9995_H__
