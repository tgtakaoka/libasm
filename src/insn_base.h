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

private:
    uint32_t _address;
    uint8_t _length;
    static constexpr size_t MAX_CODE = 24;
    uint8_t _bytes[MAX_CODE];
    static constexpr size_t MAX_NAME = 7;
    char _name[MAX_NAME + 1];

    template <typename Conf>
    friend class InsnBase;

    void emitByte(uint8_t val) { emitByte(val, _length); }

    void emitByte(uint8_t val, uint8_t pos) {
        if (pos < MAX_CODE) {
            _bytes[pos++] = val;
            if (_length < pos)
                _length = pos;
        }
    }

    void setName_P(const /*PROGMEM*/ char *name) {
        strncpy_P(_name, name, MAX_NAME);
        _name[MAX_NAME] = 0;
    }

    void setName(const char *name, const char *end) {
        uint8_t len = end - name;
        if (len >= MAX_NAME)
            len = MAX_NAME;
        strncpy(_name, name, len);
        _name[len] = 0;
    }

    void appendName(const char c) {
        uint8_t len = strlen(_name);
        if (len < MAX_NAME) {
            _name[len++] = c;
            _name[len] = 0;
        }
    }

    friend class Disassembler;

    void toLowerName() {
        for (char *p = _name; *p; p++)
            *p = tolower(*p);
    }
};

template <typename Conf>
class InsnBase : public ErrorReporter {
public:
    typename Conf::uintptr_t address() const { return _insn.address(); }
    const uint8_t *bytes() const { return _insn.bytes(); }
    uint8_t length() const { return _insn.length(); }
    const char *name() const { return _insn.name(); }

    void resetAddress(typename Conf::uintptr_t addr) {
        _insn.resetAddress(addr);
        resetError();
    }

    void setName_P(const /*PROGMEM*/ char *name) { _insn.setName_P(name); }

    void setName(const char *name, const char *end) {
        _insn.setName(name, end);
    }

    void appendName(const char c) { _insn.appendName(c); }

    void emitByte(uint8_t val) { _insn.emitByte(val); }

    void emitByte(uint8_t val, uint8_t pos) { _insn.emitByte(val, pos); }

    void emitUint16Be(uint16_t val) {
        emitByte(val >> 8);
        emitByte(val >> 0);
    }

    void emitUint16Le(uint16_t val) {
        emitByte(val >> 0);
        emitByte(val >> 8);
    }

    void emitUint16Be(uint16_t val, uint8_t pos) {
        emitByte(val >> 8, pos + 0);
        emitByte(val >> 0, pos + 1);
    }

    void emitUint16Le(uint16_t val, uint8_t pos) {
        emitByte(val >> 0, pos + 0);
        emitByte(val >> 8, pos + 1);
    }

    void emitUint32Be(uint32_t val) {
        emitUint16Be(val >> 16);
        emitUint16Be(val >> 0);
    }

    void emitUint32Le(uint32_t val) {
        emitUint16Le(val >> 0);
        emitUint16Le(val >> 16);
    }

    void emitUint32Be(uint32_t val, uint8_t pos) {
        emitUint16Be(val >> 16, pos + 0);
        emitUint16Be(val >> 0, pos + 2);
    }

    void emitUint32Le(uint32_t val, uint8_t pos) {
        emitUint16(val >> 0, pos + 0);
        emitUint16(val >> 16, pos + 2);
    }

    uint8_t readByte(DisMemory &memory) {
        if (!memory.hasNext()) {
            setError(NO_MEMORY);
            return 0;
        }
        const uint8_t val = memory.readByte();
        emitByte(val);
        return val;
    }

    uint16_t readUint16Be(DisMemory &memory) {
        const uint8_t msb = readByte(memory);
        const uint8_t lsb = readByte(memory);
        return static_cast<uint16_t>(msb) << 8 | lsb;
    }

    uint16_t readUint16Le(DisMemory &memory) {
        const uint8_t lsb = readByte(memory);
        const uint8_t msb = readByte(memory);
        return static_cast<uint16_t>(msb) << 8 | lsb;
    }

    uint32_t readUint32Be(DisMemory &memory) {
        const uint16_t msw = readUint16Be(memory);
        const uint16_t lsw = readUint16Be(memory);
        return static_cast<uint32_t>(msw) << 16 | lsw;
    }

    uint32_t readUint32Le(DisMemory &memory) {
        const uint16_t lsw = readUint16Le(memory);
        const uint16_t msw = readUint16Le(memory);
        return static_cast<uint32_t>(msw) << 16 | lsw;
    }

    void emitUint16(uint16_t val) {
        if (Conf::ENDIAN == ENDIAN_BIG) {
            emitUint16Be(val);
        } else {
            emitUint16Le(val);
        }
    }

    void emitUint16(uint16_t val, uint8_t pos) {
        if (Conf::ENDIAN == ENDIAN_BIG) {
            emitUint16Be(val, pos);
        } else {
            emitUint16Le(val, pos);
        }
    }

    void emitUint32(uint32_t val) {
        if (Conf::ENDIAN == ENDIAN_BIG) {
            emitUint32Be(val);
        } else {
            emitUint32Le(val);
        }
    }

    void emitUint32(uint32_t val, uint8_t pos) {
        if (Conf::ENDIAN == ENDIAN_BIG) {
            emitUint32Be(val, pos);
        } else {
            emitUint32Le(val, pos);
        }
    }

    uint16_t readUint16(DisMemory &memory) {
        if (Conf::ENDIAN == ENDIAN_BIG) {
            return readUint16Be(memory);
        } else {
            return readUint16Le(memory);
        }
    }

    uint32_t readUint32(DisMemory &memory) {
        if (Conf::ENDIAN == ENDIAN_BIG) {
            return readUint32Be(memory);
        } else {
            return readUint32Le(memory);
        }
    }

protected:
    InsnBase(Insn &insn) : ErrorReporter(), _insn(insn) {}

private:
    Insn &_insn;
};

}  // namespace libasm

#endif  // __INSN_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
