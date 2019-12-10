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

#endif // __DIS_INTERFACE_H__
