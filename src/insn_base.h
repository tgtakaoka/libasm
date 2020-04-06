/*
 * Copyright 2020 Tadashi G. Takaoka
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

class InsnBase {
public:
    target::uintptr_t address() const { return _address; }
    const uint8_t *bytes() const { return _bytes; }
    host::uint_t length() const { return _length; }
    const char *name() const { return _name; }

    void resetAddress(target::uintptr_t addr) {
        _address = addr;
        _length = 0;
    }

    target::insn_t insnCode() const { return _insnCode; }
    void setInsnCode(target::insn_t insnCode) {
        _insnCode = insnCode;
    }

    void setName(const char *name, const char *end = nullptr) {
        size_t len = end ? end - name : strlen(name);
        if (len >= sizeof(_name) - 1) len = sizeof(_name) - 1;
        strncpy(_name, name, len);
        _name[len] = 0;
    }

    Error readByte(DisMemory<target::uintptr_t> &memory, uint8_t &val) {
        if (!memory.hasNext()) return NO_MEMORY;
        val = memory.readByte();
        emitByte(val);
        return OK;
    }

    Error readUint16(DisMemory<target::uintptr_t> &memory, uint16_t &val) {
        uint8_t msb, lsb;
        if (endian() == ENDIAN_BIG) {
            if (readByte(memory, msb)) return NO_MEMORY;
            if (readByte(memory, lsb))  return NO_MEMORY;
        } else {
            if (readByte(memory, lsb))  return NO_MEMORY;
            if (readByte(memory, msb)) return NO_MEMORY;
        }
        val = static_cast<uint16_t>(msb) << 8 | lsb;
        return OK;
    }

    Error readUint32(DisMemory<target::uintptr_t> &memory, uint32_t &val) {
        uint16_t msw, lsw;
        if (endian() == ENDIAN_BIG) {
            if (readUint16(memory, msw)) return NO_MEMORY;
            if (readUint16(memory, lsw))  return NO_MEMORY;
        } else {
            if (readUint16(memory, lsw))  return NO_MEMORY;
            if (readUint16(memory, msw)) return NO_MEMORY;
        }
        val = static_cast<uint32_t>(msw) << 16 | lsw;
        return OK;
    }

    void emitByte(uint8_t val) {
        _bytes[_length++] = val;
    }

    void emitUint16(uint16_t val) {
        if (endian() == ENDIAN_BIG) {
            emitByte(static_cast<uint8_t>(val >> 8));
            emitByte(static_cast<uint8_t>(val));
        } else {
            emitByte(static_cast<uint8_t>(val));
            emitByte(static_cast<uint8_t>(val >> 8));
        }
    }

    void emitUint32(uint32_t val) {
        if (endian() == ENDIAN_BIG) {
            emitUint16(static_cast<uint16_t>(val >> 16));
            emitUint16(static_cast<uint16_t>(val));
        } else {
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
    host::uint_t _length;
    uint8_t _bytes[16];
    char    _name[15 + 1];
    target::insn_t _insnCode;

    virtual Endian endian() = 0;
};

#endif // __INSN_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
