/* -*- mode: c++; -*- */
#ifndef __INSN_TMS9995_H__
#define __INSN_TMS9995_H__

#include <string.h>

#include "entry_tms9995.h"
#include "memory.h"

class Insn {
public:
    target::uintptr_t address() const { return _address; }
    const target::byte_t *bytes() const { return _bytes; }
    host::uint_t insnLen() const { return _insnLen; }
    target::insn_t insnCode() const { return _insnCode; }
    const char *name() const { return _name; }
    AddrMode addrMode() const { return _addrMode(_flags); }

    void resetAddress(target::uintptr_t addr) {
        _address = addr;
        _insnLen = 0;
    }
    void setInsnCode(target::insn_t insnCode) { _insnCode = insnCode; }
    void setName(const char *name, const char *end = nullptr) {
        if (!end) end = name + strlen(name);
        char *p = _name;
        while (name < end && p < _name + sizeof(_name) - 1)
            *p++ = *name++;
        *p = 0;
    }
    void setFlags(host::uint_t flags) { _flags = flags; }

    Error readUint16(Memory &memory, target::uint16_t &val) {
        if (!memory.hasNext()) return NO_MEMORY;
        val = (target::uint16_t)memory.readByte() << 8;
        if (!memory.hasNext()) return NO_MEMORY;
        val |= memory.readByte();
        appendUint16(val);
        return OK;
    }

    void emitInsn() {
        emitUint16(_insnCode, 0);
        if (_insnLen == 0) _insnLen = 2;
    }
    void emitOperand(target::uint16_t val) {
        if (_insnLen == 0) _insnLen = 2;
        appendUint16(val);
    }
    void appendUint16(target::uint16_t val) {
        emitUint16(val, _insnLen);
        _insnLen += 2;
    }

private:
    target::uintptr_t _address;
    target::insn_t    _insnCode;
    host::uint_t      _insnLen;
    host::uint_t      _flags;
    char              _name[5];
    target::byte_t    _bytes[6];

    void emitUint16(target::uint16_t val, host::uint_t pos) {
        _bytes[pos++] = target::byte_t(val >> 8);
        _bytes[pos] = target::byte_t(val);
    }
};

#endif // __INSN_TMS9995_H__
