/* -*- mode: c++; -*- */
#ifndef __INSN_H__
#define __INSN_H__

#include <cstring>

#include "config_host.h"

class Insn {
public:
    target::uintptr_t address() const { return _address; }
    const target::byte_t *bytes() const { return _bytes; }
    host::uint_t insnLen() const { return _insnLen; }
    host::uint_t oprLen() const { return _oprLen; }
    target::insn_t insnCode() const { return _insnCode; }
    const char *name() const { return _name; }
    AddrMode addrMode() const { return _addrMode; }

private:
    target::uintptr_t _address;
    target::insn_t    _insnCode;
    host::uint_t      _insnLen;
    host::uint_t      _oprLen;
    AddrMode          _addrMode;
    target::byte_t    _bytes[8];
    char              _name[8];

    friend class InsnTable;
    friend class Asm;
    friend class Dis;

    void reset(target::uintptr_t addr) {
        _address = addr;
        _insnLen = 0;
    }
    void addByte(target::byte_t val) {
        _bytes[_insnLen++] = val;
    }
    void addUint16(target::uint16_t val) {
        addByte(target::byte_t(val >> 8));
        addByte(target::byte_t(val & 0xff));
    }
    void addUint32(target::uint32_t val) {
        addUint16(target::uint16_t(val >> 16));
        addUint16(target::uint16_t(val & 0xffff));
    }
    void setName(const char *name, const char *end = nullptr) {
        if (!end) end = name + strlen(name);
        char *p = _name;
        while (name < end && p < _name + sizeof(_name) - 1)
            *p++ = *name++;
        *p = 0;
    }
    void setInsnCode(target::insn_t insnCode) { _insnCode = insnCode; }
    void setOprLen(host::uint_t oprLen) { _oprLen = oprLen; }
    void setAddrMode(AddrMode addrMode) { _addrMode = addrMode; }
};

#endif
