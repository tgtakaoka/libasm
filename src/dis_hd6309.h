/* -*- mode: c++; -*- */
#ifndef __DIS_HD6309_H__
#define __DIS_HD6309_H__

#include "config_hd6309.h"

#include "error_reporter.h"
#include "insn_hd6309.h"
#include "dis_memory.h"
#include "reg_hd6309.h"
#include "symbol_table.h"

template<McuType mcuType>
class Dis09 : public ErrorReporter {
public:
    Error decode(
        DisMemory &memory, Insn& insn, char *operands, SymbolTable *symtab);

private:
    RegHd6309<mcuType> _regs;
    char *_operands;
    SymbolTable *_symtab;

    void reset(char *operands, SymbolTable *symtab) {
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
    Error decodeDirectPage(DisMemory &memory, Insn &insn);
    Error decodeIndexed(DisMemory &memory, Insn &insn);
    Error decodeExtended(DisMemory &memory, Insn &insn);
    Error decodeRelative(DisMemory &memory, Insn &insn);
    Error decodeImmediate(DisMemory &memory, Insn &insn);
    Error decodeStackOp(DisMemory &memory, Insn &insn);
    Error decodeRegisters(DisMemory &memory, Insn &insn);
    // HD6309
    Error decodeImmediatePlus(DisMemory &memory, Insn &insn);
    Error decodeBitOperation(DisMemory &memory, Insn &insn);
    Error decodeTransferMemory(DisMemory &memory, Insn &insn);
};

#include "dis_hd6309_impl.h"

typedef Dis09<MC6809> DisMc6809;
typedef Dis09<HD6309> DisHd6309;

#endif // __DIS_HD6309_H__
