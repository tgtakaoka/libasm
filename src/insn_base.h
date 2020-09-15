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

#include "config_base.h"
#include "dis_memory.h"
#include "error_reporter.h"

namespace libasm {

class Insn {
public:
    uint32_t address() const { return _address; }
    const uint8_t *bytes() const { return _bytes; }
    uint8_t length() const { return _length; }
    const char *name() const { return _name; }

    void resetAddress(uint32_t addr) {
        _address = addr;
        _length = 0;
    }

    void setName(const char *name, size_t len) {
        if (len >= NAME_MAX) len = NAME_MAX;
        strncpy(_name, name, len);
        _name[len] = 0;
    }

    void appendName(const char *suffix) {
        strncat(_name, suffix, NAME_MAX - strlen(_name));
    }

    void toLowerName() {
        for (char *p = _name; *p; p++)
            *p = tolower(*p);
    }

    void emitByte(uint8_t val) {
        emitByte(val, _length);
    }

    void emitByte(uint8_t val, uint8_t pos) {
        if (pos < CODE_MAX) {
            _bytes[pos++] = val;
            if (_length < pos) _length = pos;
        }
    }

    Error readByte(DisMemory &memory, uint8_t &val) {
        if (!memory.hasNext()) return NO_MEMORY;
        val = memory.readByte();
        emitByte(val);
        return OK;
    }

    Error readUint16Be(DisMemory &memory, uint16_t &val) {
        uint8_t msb, lsb;
        if (readByte(memory, msb)) return NO_MEMORY;
        if (readByte(memory, lsb)) return NO_MEMORY;
        val = static_cast<uint16_t>(msb) << 8 | lsb;
        return OK;
    }

    Error readUint16Le(DisMemory &memory, uint16_t &val) {
        uint8_t msb, lsb;
        if (readByte(memory, lsb)) return NO_MEMORY;
        if (readByte(memory, msb)) return NO_MEMORY;
        val = static_cast<uint16_t>(msb) << 8 | lsb;
        return OK;
    }

    Error readUint32Be(DisMemory &memory, uint32_t &val) {
        uint16_t msw, lsw;
        if (readUint16Be(memory, msw)) return NO_MEMORY;
        if (readUint16Be(memory, lsw)) return NO_MEMORY;
        val = static_cast<uint32_t>(msw) << 16 | lsw;
        return OK;
    }

    Error readUint32Le(DisMemory &memory, uint32_t &val) {
        uint16_t msw, lsw;
        if (readUint16Le(memory, lsw)) return NO_MEMORY;
        if (readUint16Le(memory, msw)) return NO_MEMORY;
        val = static_cast<uint32_t>(msw) << 16 | lsw;
        return OK;
    }

private:
    uint32_t _address;
    uint8_t  _length;
    static constexpr size_t CODE_MAX = 24;
    uint8_t  _bytes[CODE_MAX];
    static constexpr size_t NAME_MAX = 7;
    char     _name[NAME_MAX + 1];

    friend class TableBase;
};

template<typename Conf>
class InsnBase {
public:
    Insn &insn() { return _insn; }
    typename Conf::uintptr_t address() const { return _insn.address(); }
    const uint8_t *bytes() const { return _insn.bytes(); }
    uint8_t length() const { return _insn.length(); }
    const char *name() const { return _insn.name(); }

    void resetAddress(typename Conf::uintptr_t addr) {
        _insn.resetAddress(addr);
    }
    void setName(const char *name, const char *end = nullptr) {
        _insn.setName(name, end ? end - name : strlen(name));
    }

    Error readByte(DisMemory &memory, uint8_t &val) {
        return _insn.readByte(memory, val);
    }

    Error readUint16(DisMemory &memory, uint16_t &val) {
        return (Conf::ENDIAN == ENDIAN_BIG)
            ? _insn.readUint16Be(memory, val)
            : _insn.readUint16Le(memory, val);
    }
    Error readUint16Be(DisMemory &memory, uint16_t &val) {
        return _insn.readUint16Be(memory, val);
    }
    Error readUint16Le(DisMemory &memory, uint16_t &val) {
        return _insn.readUint16Le(memory, val);
    }

    Error readUint32(DisMemory &memory, uint32_t &val) {
        return (Conf::ENDIAN == ENDIAN_BIG)
            ? _insn.readUint32Be(memory, val)
            : _insn.readUint32Le(memory, val);
    }

    void emitByte(uint8_t val) {
        _insn.emitByte(val);
    }

    void emitByte(uint8_t val, uint8_t pos) {
        _insn.emitByte(val, pos);
    }

    void emitUint16(uint16_t val) {
        if (Conf::ENDIAN == ENDIAN_BIG) {
            emitByte(val >> 8);
            emitByte(val >> 0);
        } else {
            emitByte(val >> 0);
            emitByte(val >> 8);
        }
    }

    void emitUint16(uint16_t val, uint8_t pos) {
        if (Conf::ENDIAN == ENDIAN_BIG) {
            emitByte(val >> 8, pos + 0);
            emitByte(val >> 0, pos + 1);
        } else {
            emitByte(val >> 0, pos + 0);
            emitByte(val >> 8, pos + 1);
        }
    }

    void emitUint32(uint32_t val) {
        if (Conf::ENDIAN == ENDIAN_BIG) {
            emitUint16(val >> 16);
            emitUint16(val >>  0);
        } else {
            emitUint16(val >>  0);
            emitUint16(val >> 16);
        }
    }

    void emitUint32(uint32_t val, uint8_t pos) {
        if (Conf::ENDIAN == ENDIAN_BIG) {
            emitUint16(val >> 16, pos + 0);
            emitUint16(val >>  0, pos + 2);
        } else {
            emitUint16(val >>  0, pos + 0);
            emitUint16(val >> 16, pos + 2);
        }
    }

protected:
    InsnBase(Insn &insn) : _insn(insn) {}

    Insn &_insn;
};

} // namespace libasm

#endif // __INSN_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
