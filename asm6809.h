/* -*- mode: c++; -*- */
#ifndef __ASM6809_H__
#define __ASM6809_H__

#include "config_6809.h"
#ifndef TARGET_ENUMS_DEFINED
typedef AddrMode6809 AddrMode;
typedef RegName6809 RegName;
#endif

#include "insn.h"

class Asm6809 : public Insn {
public:
    Error encode(target::uintptr_t addr, const char *line, SymbolTable *symtab);
    AddrMode addrMode() const override { return _addrMode; }

protected:
    AddrMode     _addrMode;

    const target::uintptr_t lookup(const char *symbol) const {
        return _symtab ? _symtab->lookup(symbol) : 0;
    };
    void addByte(target::byte_t val) { _bytes[_insnLen++] = val; }
    void addWord(target::word_t val) {
        addByte(target::byte_t(val >> 8));
        addByte(target::byte_t(val & 0xff));
    }
    void addInsnCode() {
        if (hasPrefixCode())
            addByte(target::byte_t(insnCode() >> 8));
        addByte(target::byte_t(insnCode() & 0xff));
    }
    Error determineAddrMode(const char *line, AddrMode &mode);
    Error getOperand16(const char *&line, target::word_t &val);
    Error encodeStackOp(const char *line);
    Error encodeRegisters(const char *line);
    Error encodeRelative(const char *line);
    Error encodeImmediate(const char *line);
    Error encodeDirect(const char *line);
    Error encodeExtended(const char *line);
    Error encodeIndexed(const char *line);
};

#endif // __ASM6809_H__
