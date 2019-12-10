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
    Error getError() const { return _error; }
    bool hasError() const { return !(_error == OK); }

protected:
    Error setError(Error error) {
        _error = error;
        return error;
    }
    void resetError() {
        _error = INVALID_STATE;
    }

private:
    Error _error;
};

#endif // __ASM_INTERFACE_H__
