/* -*- mode: c++; -*- */
#ifndef __ASM_MC6809_H__
#define __ASM_MC6809_H__

#include "config_mc6809.h"
#include "insn.h"
#include "table_mc6809.h"

class AsmMc6809 : public Insn {
public:
    virtual Error encode(target::uintptr_t addr, const char *line, SymbolTable *symtab);
    bool hasPrefixCode() const {
        return _tableMc6809.isPrefixCode(_tableMc6809.prefixCode(insnCode()));
    }

protected:
    static TableMc6809 _tableMc6809;

    void addByte(target::byte_t val) {
        _bytes[_insnLen++] = val;
    }
    void addWord(target::word_t val) {
        addByte(target::byte_t(val >> 8));
        addByte(target::byte_t(val & 0xff));
    }
    void addInsnCode() {
        if (hasPrefixCode()) addByte(_tableMc6809.prefixCode(insnCode()));
        addByte(_tableMc6809.opCode(insnCode()));
    }

    static bool compareRegName(const char *line, RegName regName);
    static host::uint_t regNameLen(RegName regName);
    static host::int_t encodeRegNumber(RegName regName, const RegName *table, host::uint_t len);
    static RegName parseRegName(const char *, const RegName *table, host::uint_t len);

    Error determineAddrMode(const char *line, AddrMode &mode);
    Error getOperand16(const char *&line, target::word_t &val);
    Error encodeStackOp(const char *line);
    Error encodeRegisters(const char *line);
    Error encodeRelative(const char *line);
    virtual Error encodeImmediate(const char *line);
    Error encodeDirect(const char *line);
    Error encodeExtended(const char *line);
    virtual Error encodeIndexed(const char *line);
};

#endif // __ASM_MC6809_H__
