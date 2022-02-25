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
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "config_base.h"
#include "dis_memory.h"
#include "error_reporter.h"
#include "str_scanner.h"

namespace libasm {

/**
 * General instruction code interface for Assembler and Disassembler.
 */
class Insn {
public:
    Insn(uint32_t addr) : _address(addr), _length(0) { _name[0] = 0; }
    uint32_t address() const { return _address; }
    const uint8_t *bytes() const { return _bytes; }
    uint8_t length() const { return _length; }
    const char *name() const { return _name; }
    void clear() { _length = 0; }
    void reset() {
        clear();
        _name[0] = 0;
    }

    /** No copy constructor. */
    Insn(Insn const &) = delete;
    /** No assignment operator. */
    void operator=(Insn const &) = delete;

private:
    const uint32_t _address;
    uint8_t _length;
    static constexpr size_t MAX_CODE = 24;
    uint8_t _bytes[MAX_CODE];
    static constexpr size_t MAX_NAME = 7;
    char _name[MAX_NAME + 1];

    friend class InsnBase;
    friend class Disassembler;

    void emitByte(uint8_t val) { emitByte(val, _length); }

    void emitByte(uint8_t val, uint8_t pos) {
        if (pos < MAX_CODE) {
            _bytes[pos++] = val;
            if (_length < pos)
                _length = pos;
        }
    }

    void setName_P(const /*PROGMEM*/ char *name_P) {
        strncpy_P(_name, name_P, MAX_NAME);
        _name[MAX_NAME] = 0;
    }

    void setName(const StrScanner &name) {
        uint8_t len = name.size();
        if (len >= MAX_NAME)
            len = MAX_NAME;
        strncpy(_name, name, len);
        _name[len] = 0;
    }

    void setName(const char *name) {
        uint8_t len = strlen(name);
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

    void toLowerName() {
        for (char *p = _name; *p; p++)
            *p = tolower(*p);
    }
};

/**
 * Base instruction code class.
 */
class InsnBase : public ErrorReporter {
public:
    const uint8_t *bytes() const { return _insn.bytes(); }
    uint8_t length() const { return _insn.length(); }
    const char *name() const { return _insn.name(); }

    /** Set instruction name from readonly text (Disassembler). */
    void setName_P(const /*PROGMEM*/ char *name_P) { _insn.setName_P(name_P); }

    /** Set instruction name from text (Assembler). */
    void setName(const char *name) { _insn.setName(name); }

    /** Set instruction name from text (Assembler). */
    void setName(const StrScanner &name) { _insn.setName(name); }

    /** Append a letter to instruction name. */
    void appendName(const char c) { _insn.appendName(c); }

    /** Generate 8 bit |data| (Assembler). */
    void emitByte(uint8_t data) { _insn.emitByte(data); }

    /** Generate 8 bit |data| at |pos| (Assembler). */
    void emitByte(uint8_t data, uint8_t pos) { _insn.emitByte(data, pos); }

    /** Generate 16 bit big endian |data| (Assembler). */
    void emitUint16Be(uint16_t data) {
        emitByte(data >> 8);
        emitByte(data >> 0);
    }

    /** Generate 16 bit little endian |data| (Assembler). */
    void emitUint16Le(uint16_t data) {
        emitByte(data >> 0);
        emitByte(data >> 8);
    }

    /** Generate 16 bit big endian |data| at |pos| (Assembler). */
    void emitUint16Be(uint16_t data, uint8_t pos) {
        emitByte(data >> 8, pos + 0);
        emitByte(data >> 0, pos + 1);
    }

    /** Generate 16 bit little endian |data| at |pos| (Assembler). */
    void emitUint16Le(uint16_t data, uint8_t pos) {
        emitByte(data >> 0, pos + 0);
        emitByte(data >> 8, pos + 1);
    }

    /** Generate 32 bit big endian |data| (Assembler). */
    void emitUint32Be(uint32_t data) {
        emitUint16Be(data >> 16);
        emitUint16Be(data >> 0);
    }

    /** Generate 32 bit little endian |data| (Assembler). */
    void emitUint32Le(uint32_t data) {
        emitUint16Le(data >> 0);
        emitUint16Le(data >> 16);
    }

    /** Generate 32 bit big endian |data| at |pos| (Assembler). */
    void emitUint32Be(uint32_t data, uint8_t pos) {
        emitUint16Be(data >> 16, pos + 0);
        emitUint16Be(data >> 0, pos + 2);
    }

    /** Generate 32 bit little endian |data| at |pos| (Assembler). */
    void emitUint32Le(uint32_t data, uint8_t pos) {
        emitUint16Le(data >> 0, pos + 0);
        emitUint16Le(data >> 16, pos + 2);
    }

    /** Generate 64 bit big little |data| (Assembler). */
    void emitUint64Be(uint64_t data) {
        emitUint32Be(data >> 32);
        emitUint32Be(data >> 0);
    }

    /** Generate 64 bit big little |data| (Assembler). */
    void emitUint64Le(uint64_t data) {
        emitUint32Le(data >> 0);
        emitUint32Le(data >> 32);
    }

    /** Generate 64 bit big endian |data| at |pos| (Assembler). */
    void emitUint64Be(uint64_t data, uint8_t pos) {
        emitUint32Be(data >> 32, pos + 0);
        emitUint32Be(data >> 0, pos + 4);
    }

    /** Generate 64 bit big little |data| at |pos| (Assembler). */
    void emitUint64Le(uint64_t data, uint8_t pos) {
        emitUint32Le(data >> 0, pos + 0);
        emitUint32Le(data >> 32, pos + 4);
    }

    /** Read 8 bit data from |memory| (Disassembler). */
    uint8_t readByte(DisMemory &memory) {
        if (!memory.hasNext()) {
            setError(NO_MEMORY);
            return 0;
        }
        const uint8_t data = memory.readByte();
        emitByte(data);
        return data;
    }

    /** Read 16 bit big endian data from |memory| (Disassembler). */
    uint16_t readUint16Be(DisMemory &memory) {
        const uint8_t msb = readByte(memory);
        const uint8_t lsb = readByte(memory);
        return static_cast<uint16_t>(msb) << 8 | lsb;
    }

    /** Read 16 bit little endian data from |memory| (Disassembler). */
    uint16_t readUint16Le(DisMemory &memory) {
        const uint8_t lsb = readByte(memory);
        const uint8_t msb = readByte(memory);
        return static_cast<uint16_t>(msb) << 8 | lsb;
    }

    /** Read 32 bit big endian data from |memory| (Disassembler). */
    uint32_t readUint32Be(DisMemory &memory) {
        const uint16_t msw = readUint16Be(memory);
        const uint16_t lsw = readUint16Be(memory);
        return static_cast<uint32_t>(msw) << 16 | lsw;
    }

    /** Read 32 bit little endian data from |memory| (Disassembler). */
    uint32_t readUint32Le(DisMemory &memory) {
        const uint16_t lsw = readUint16Le(memory);
        const uint16_t msw = readUint16Le(memory);
        return static_cast<uint32_t>(msw) << 16 | lsw;
    }

    /** Read 64 bit big endian data from |memory| (Disassembler). */
    uint64_t readUint64Be(DisMemory &memory) {
        const uint32_t msw = readUint32Be(memory);
        const uint32_t lsw = readUint32Be(memory);
        return static_cast<uint64_t>(msw) << 32 | lsw;
    }

    /** Read 64 bit little endian data from |memory| (Disassembler). */
    uint64_t readUint64Le(DisMemory &memory) {
        const uint32_t lsw = readUint32Le(memory);
        const uint32_t msw = readUint32Le(memory);
        return static_cast<uint64_t>(msw) << 32 | lsw;
    }

protected:
    InsnBase(Insn &insn) : ErrorReporter(), _insn(insn) {}

    uint32_t address() const { return _insn.address(); }

    void clear() {
        _insn.clear();
        resetError();
    }
    void reset() {
        clear();
        _insn.reset();
    }

private:
    Insn &_insn;
};

template <typename Conf, typename Entry>
class InsnImpl : public InsnBase {
public:
    typename Conf::uintptr_t address() const { return InsnBase::address(); }

    void clear() { InsnBase::clear(); }
    void reset() { InsnBase::reset(); }

    void setOpCode(typename Conf::opcode_t opCode, typename Conf::opcode_t prefix = 0) {
        _opCode = opCode;
        _prefix = prefix;
        setPost(0, false);
    }
    typename Conf::opcode_t opCode() const { return _opCode; }
    void embed(typename Conf::opcode_t data) { _opCode |= data; }
    bool hasPrefix() const { return _prefix != 0; }
    typename Conf::opcode_t prefix() const { return _prefix; }
    void setPost(typename Conf::opcode_t post, bool hasPost = true) {
        _post = post;
        _hasPost = hasPost;
    }
    void embedPost(typename Conf::opcode_t data) { _post |= data; }
    bool hasPost() const { return _hasPost; }
    typename Conf::opcode_t post() const { return _post; }
    void setFlags(typename Entry::Flags flags) { _flags = flags; }
    typename Entry::Flags flags() const { return _flags; }
    typename Entry::Flags &flags() { return _flags; }

    /* Generate 16 bit |data| (Assembler). */
    void emitUint16(uint16_t data) {
        if (Conf::ENDIAN == ENDIAN_BIG) {
            emitUint16Be(data);
        } else {
            emitUint16Le(data);
        }
    }

    /* Generate 16 bit |data| at |pos| (Assembler). */
    void emitUint16(uint16_t data, uint8_t pos) {
        if (Conf::ENDIAN == ENDIAN_BIG) {
            emitUint16Be(data, pos);
        } else {
            emitUint16Le(data, pos);
        }
    }

    /* Generate 32 bit |data| (Assembler). */
    void emitUint32(uint32_t data) {
        if (Conf::ENDIAN == ENDIAN_BIG) {
            emitUint32Be(data);
        } else {
            emitUint32Le(data);
        }
    }

    /* Generate 32 bit |data| at |pos| (Assembler). */
    void emitUint32(uint32_t data, uint8_t pos) {
        if (Conf::ENDIAN == ENDIAN_BIG) {
            emitUint32Be(data, pos);
        } else {
            emitUint32Le(data, pos);
        }
    }

    /* Generate 64 bit |data| (Assembler). */
    void emitUint64(uint64_t data) {
        if (Conf::ENDIAN == ENDIAN_BIG) {
            emitUint64Be(data);
        } else {
            emitUint64Le(data);
        }
    }

    /* Generate 64 bit |data| at |pos| (Assembler). */
    void emitUint64(uint32_t data, uint8_t pos) {
        if (Conf::ENDIAN == ENDIAN_BIG) {
            emitUint64Be(data, pos);
        } else {
            emitUint64Le(data, pos);
        }
    }

    /* Generate 32 bit floating point |data| (Assembler). */
    void emitFloat32(float data) {
        union {
            float float32;
            uint32_t data32;
        } bytes;
        bytes.float32 = data;
        if (Conf::ENDIAN == ENDIAN_BIG) {
            emitUint32Be(bytes.data32);
        } else {
            emitUint32Le(bytes.data32);
        }
    }

    /* Generate 32 bit floating point |data| at |pos| (Assembler). */
    void emitFloat32(float data, uint8_t pos) {
        union {
            float float32;
            uint32_t data32;
        } bytes;
        bytes.float32 = data;
        if (Conf::ENDIAN == ENDIAN_BIG) {
            emitUint32Be(bytes.data32, pos);
        } else {
            emitUint32Le(bytes.data32, pos);
        }
    }

    /* Generate 64 bit floating point |data| (Assembler). */
    void emitFloat64(double data) {
        union {
            double float64;
            uint64_t data64;
        } bytes;
        bytes.float64 = data;
        if (Conf::ENDIAN == ENDIAN_BIG) {
            emitUint64Be(bytes.data64);
        } else {
            emitUint64Le(bytes.data64);
        }
    }

    /* Generate 64 bit floating point |data| at |pos| (Assembler). */
    void emitFloat64(double data, uint8_t pos) {
        union {
            double float64;
            uint64_t data64;
        } bytes;
        bytes.float64 = data;
        if (Conf::ENDIAN == ENDIAN_BIG) {
            emitUint64Be(bytes.data64, pos);
        } else {
            emitUint64Le(bytes.data64, pos);
        }
    }

    /** Read 16 bit data from |memory| (Disassembler). */
    uint16_t readUint16(DisMemory &memory) {
        if (Conf::ENDIAN == ENDIAN_BIG) {
            return readUint16Be(memory);
        } else {
            return readUint16Le(memory);
        }
    }

    /** Read 32 bit data from |memory| (Disassembler). */
    uint32_t readUint32(DisMemory &memory) {
        if (Conf::ENDIAN == ENDIAN_BIG) {
            return readUint32Be(memory);
        } else {
            return readUint32Le(memory);
        }
    }

    /** Read 64 bit data from |memory| (Disassembler). */
    uint64_t readUint64(DisMemory &memory) {
        if (Conf::ENDIAN == ENDIAN_BIG) {
            return readUint64Be(memory);
        } else {
            return readUint64Le(memory);
        }
    }

    /** Read 32 bit floating point data from |memory| (Disassembler). */
    float readFloat32(DisMemory &memory) {
        union {
            float float32;
            uint32_t data32;
        } bytes;
        bytes.data32 = readUint32(memory);
        return bytes.float32;
    }

    /** Read 64 bit floating point data from |memory| (Disassembler). */
    double readFloat64(DisMemory &memory) {
        union {
            double float64;
            uint64_t data64;
        } bytes;
        bytes.data64 = readUint64(memory);
        return bytes.float64;
    }

protected:
    InsnImpl(Insn &insn) : InsnBase(insn) {}

private:
    typename Conf::opcode_t _opCode;
    typename Conf::opcode_t _prefix;
    typename Conf::opcode_t _post;
    bool _hasPost;

    typename Entry::Flags _flags;
};

}  // namespace libasm

#endif  // __INSN_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
