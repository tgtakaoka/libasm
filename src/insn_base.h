/* -*- mode: c++; -*- */
#ifndef __INSN_BASE_H__
#define __INSN_BASE_H__

#include <ctype.h>
#include <string.h>

#include "error_reporter.h"
#include "dis_memory.h"

enum Endian {
    ENDIAN_BIG,
    ENDIAN_LITTLE,
};

template<
    Endian endian,
    int maxBytes,
    int maxName>
class InsnBase {
public:
    target::uintptr_t address() const { return _address; }
    const uint8_t *bytes() const { return _bytes; }
    host::uint_t insnLen() const { return _insnLen; }
    target::insn_t insnCode() const { return _insnCode; }
    const char *name() const { return _name; }
    static bool bigEndian() { return endian == ENDIAN_BIG; }
    static bool littleEndian() { return endian == ENDIAN_LITTLE; }
    static host::uint_t getMaxBytes() { return maxBytes; }
    static host::uint_t getMaxName() { return maxName; }

    void resetAddress(target::uintptr_t addr) {
        _address = addr;
        _insnLen = 0;
    }

    void setInsnCode(target::insn_t insnCode) {
        _insnCode = insnCode;
    }

    void setName(const char *name, const char *end = nullptr) {
        if (!end) end = name + strlen(name);
        char *p = _name;
        while (name < end && p < _name + maxName)
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
        uint8_t msb, lsb;
        if (bigEndian()) {
            if (readByte(memory, msb)) return NO_MEMORY;
            if (readByte(memory, lsb))  return NO_MEMORY;
        }
        if (littleEndian()) {
            if (readByte(memory, lsb))  return NO_MEMORY;
            if (readByte(memory, msb)) return NO_MEMORY;
        }
        val = static_cast<uint16_t>(msb) << 8 | lsb;
        return OK;
    }

    Error readUint32(DisMemory<target::uintptr_t> &memory, uint32_t &val) {
        uint16_t msw, lsw;
        if (bigEndian()) {
            if (readUint16(memory, msw)) return NO_MEMORY;
            if (readUint16(memory, lsw))  return NO_MEMORY;
        }
        if (littleEndian()) {
            if (readUint16(memory, lsw))  return NO_MEMORY;
            if (readUint16(memory, msw)) return NO_MEMORY;
        }
        val = static_cast<uint32_t>(msw) << 16 | lsw;
        return OK;
    }

    void emitByte(uint8_t val) {
        _bytes[_insnLen++] = val;
    }

    void emitUint16(uint16_t val) {
        if (bigEndian()) {
            emitByte(static_cast<uint8_t>(val >> 8));
            emitByte(static_cast<uint8_t>(val));
        }
        if (littleEndian()) {
            emitByte(static_cast<uint8_t>(val));
            emitByte(static_cast<uint8_t>(val >> 8));
        }
    }

    void emitUint32(uint32_t val) {
        if (bigEndian()) {
            emitUint16(static_cast<uint16_t>(val >> 16));
            emitUint16(static_cast<uint16_t>(val));
        }
        if (littleEndian()) {
            emitUint16(static_cast<uint16_t>(val));
            emitUint16(static_cast<uint16_t>(val >> 16));
        }
    }

    void toLower() {
        for (char *p = _name; *p; p++)
            *p = tolower(*p);
    }

protected:
    target::uintptr_t _address;
    target::insn_t _insnCode;
    host::uint_t _insnLen;
    uint8_t _bytes[maxBytes];
    char    _name[maxName + 1];
};

#endif // __INSN_BASE_H__
