/* -*- mode: c++; -*- */
#ifndef __DIS_HD6309_H__
#define __DIS_HD6309_H__

#include "config_hd6309.h"

#include "insn_hd6309.h"
#include "dis_memory.h"
#include "reg_hd6309.h"
#include "symbol_table.h"
#include "dis_interface.h"

template<McuType mcuType>
class Dis09 : public Disassembler<target::uintptr_t> {
public:
    Error decode(
        DisMemory<target::uintptr_t> &memory,
        Insn& insn,
        char *operands,
        SymbolTable<target::uintptr_t> *symtab) override;

private:
    RegHd6309<mcuType> _regs;
    char *_operands;
    SymbolTable<target::uintptr_t> *_symtab;

    void reset(char *operands, SymbolTable<target::uintptr_t> *symtab) {
        *(_operands = operands) = 0;
        _symtab = symtab;
        resetError();
    }

    const char *lookup(target::uintptr_t addr) const {
        return _symtab ? _symtab->lookup(addr) : nullptr;
    }

    void outChar(char c) { *_operands++ = c; *_operands = 0; }
    void outText(const char *text);
    void outOpr8Hex(uint8_t val);
    void outOpr16Hex(uint16_t val);
    void outOpr32Hex(uint32_t val);
    void outOpr16Int(int16_t val);
    void outRegister(RegName regName);

    // MC6809
    Error decodeDirectPage(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeIndexed(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeExtended(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeRelative(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeImmediate(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeStackOp(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeRegisters(DisMemory<target::uintptr_t> &memory, Insn &insn);
    // HD6309
    Error decodeImmediatePlus(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeBitOperation(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeTransferMemory(DisMemory<target::uintptr_t> &memory, Insn &insn);
};

#include "dis_hd6309_impl.h"

typedef Dis09<MC6809> DisMc6809;
typedef Dis09<HD6309> DisHd6309;

#endif // __DIS_HD6309_H__
