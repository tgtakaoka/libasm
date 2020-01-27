/* -*- mode: c++; -*- */
#ifndef __INSN_BASE_H__
#define __INSN_BASE_H__

#include <string.h>

#include "error_reporter.h"
#include "dis_memory.h"

template<typename Addr, typename InsnCode, int maxBytes, int maxName>
class InsnBase {
public:
    Addr address() const { return _address; }
    const uint8_t *bytes() const { return _bytes; }
    host::uint_t insnLen() const { return _insnLen; }
    InsnCode insnCode() const { return _insnCode; }
    const char *name() const { return _name; }

    void resetAddress(Addr addr) {
        _address = addr;
        _insnLen = 0;
    }

    void setInsnCode(InsnCode insnCode) {
        _insnCode = insnCode;
    }

    void setName(const char *name, const char *end = nullptr) {
        if (!end) end = name + strlen(name);
        char *p = _name;
        while (name < end && p < _name + maxName)
            *p++ = *name++;
        *p = 0;
    }

    Error readByte(DisMemory<Addr> &memory, uint8_t &val) {
        if (!memory.hasNext()) return NO_MEMORY;
        val = memory.readByte();
        emitByte(val);
        return OK;
    }

    void emitByte(uint8_t val) {
        _bytes[_insnLen++] = val;
    }

protected:
    Addr          _address;
    InsnCode      _insnCode;
    host::uint_t  _insnLen;
    uint8_t       _bytes[maxBytes];
    char          _name[maxName + 1];
};

#endif // __INSN_BASE_H__
