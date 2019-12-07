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
    virtual Error getError() const = 0;
    virtual const char *errorAt() const = 0;
};

template<typename Addr>
class AsmCommon : public Assembler<Addr> {
public:
    Error getError() const override { return _error; }
    const char *errorAt() const override { return _scan; }

protected:
    const char *_scan;
    Error _error;
    SymbolTable<Addr>  *_symtab;

    Error setError(Error error) {
        _error = error;
        return error;
    }
    void resetError() {
        _error = INVALID_STATE;
    }

    void reset(const char *line, SymbolTable<Addr> *symtab) {
        _scan = line;
        _symtab = symtab;
        resetError();
    }
    bool hasSymbol(const char *symbol) const {
        return _symtab && _symtab->hasSymbol(symbol);
    }
    Addr lookup(const char *symbol) const {
        return _symtab ? _symtab->lookup(symbol) : 0;
    }
};

#endif // __ASM_INTERFACE_H__
