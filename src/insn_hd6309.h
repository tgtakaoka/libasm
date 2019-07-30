/* -*- mode: c++; -*- */
#ifndef __INSN_HD6309_H__
#define __INSN_HD6309_H__

#include <string.h>

#include "entry_hd6309.h"

class Insn {
public:
    target::uintptr_t address() const { return _address; }
    const target::byte_t *bytes() const { return _bytes; }
    host::uint_t insnLen() const { return _insnLen; }
    target::insn_t insnCode() const { return _insnCode; }
    const char *name() const { return _name; }
    AddrMode addrMode() const { return _addrMode(_flags); }
    McuMode mcuMode() const { return _mcuMode(_flags); }

    void resetAddress(target::uintptr_t addr) {
        _address = addr;
        _insnLen = 0;
    }
    void setInsnCode(target::insn_t insnCode) {
        _insnCode = insnCode;
    }
    void setFlags(host::uint_t flags) {
        _flags = flags;
    }
    void setAddrMode(AddrMode addrMode) {
        _flags = (_flags & ~Entry::addrMode_mask)
            | host::uint_t(addrMode);
    }
    void setName(const char *name, const char *end = nullptr) {
        if (!end) end = name + strlen(name);
        char *p = _name;
        while (name < end && p < _name + sizeof(_name) - 1)
            *p++ = *name++;
        *p = 0;
    }
    void emitByte(target::byte_t val) {
        _bytes[_insnLen++] = val;
    }
    void emitUint16(target::uint16_t val) {
        emitByte(target::byte_t(val >> 8));
        emitByte(target::byte_t(val & 0xff));
    }
    void emitUint32(target::uint32_t val) {
        emitUint16(target::uint16_t(val >> 16));
        emitUint16(target::uint16_t(val & 0xffff));
    }

private:
    target::uintptr_t _address;
    target::insn_t    _insnCode;
    host::uint_t      _insnLen;
    host::uint_t      _flags;
    target::byte_t    _bytes[8];
    char              _name[6];
};

#endif // __INSN_HD6309_H__
