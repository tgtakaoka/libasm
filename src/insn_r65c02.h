/* -*- mode: c++; -*- */
#ifndef __INSN_R65C02_H__
#define __INSN_R65C02_H__

#include <string.h>

#include "entry_r65c02.h"
#include "error_reporter.h"
#include "dis_memory.h"

class Insn {
public:
    target::uintptr_t address() const { return _address; }
    const uint8_t *bytes() const { return _bytes; }
    host::uint_t insnLen() const { return _insnLen; }
    target::insn_t insnCode() const { return _insnCode; }
    const char *name() const { return _name; }
    AddrMode addrMode() const { return _addrMode(_flags); }
    McuType mcuType() const { return _mcuType(_flags); }

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
        _flags = ::_flags(_mcuType(_flags), _addrMode(addrMode));
    }
    void setName(const char *name, const char *end = nullptr) {
        if (!end) end = name + strlen(name);
        char *p = _name;
        while (name < end && p < _name + sizeof(_name) - 1)
            *p++ = *name++;
        *p = 0;
    }

    Error readByte(DisMemory<target::uintptr_t> &memory, uint8_t &val) {
        if (!memory.hasNext()) return NO_MEMORY;
        val = memory.readByte();
        emitByte(val);
        return OK;
    }
    Error readUint16(DisMemory<target::uintptr_t> &memory, uint16_t &val) {
        if (!memory.hasNext()) return NO_MEMORY;
        val = memory.readByte();
        if (!memory.hasNext()) return NO_MEMORY;
        val |= static_cast<uint16_t>(memory.readByte()) << 8;
        emitUint16(val);
        return OK;
    }

    void emitByte(uint8_t val) {
        _bytes[_insnLen++] = val;
    }
    void emitUint16(uint16_t val) {
        emitByte(static_cast<uint8_t>(val & 0xff));
        emitByte(static_cast<uint8_t>(val >> 8));
    }

private:
    target::uintptr_t _address;
    target::insn_t    _insnCode;
    host::uint_t      _insnLen;
    host::uint_t      _flags;
    uint8_t    _bytes[3];
    char              _name[5];
};

#endif // __INSN_R65C02_H__
