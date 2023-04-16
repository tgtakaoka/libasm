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

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "config_base.h"
#include "dis_memory.h"
#include "error_reporter.h"
#include "str_buffer.h"
#include "str_scanner.h"

namespace libasm {

/**
 * General instruction code interface for Assembler and Disassembler.
 */
class Insn final {
public:
    Insn(uint32_t addr) : _address(addr), _length(0), _buffer(_name, sizeof(_name)) {}
    uint32_t address() const { return _address; }
    const uint8_t *bytes() const { return _bytes; }
    uint8_t length() const { return _length; }
    const char *name() const { return _name; }
    StrBuffer &nameBuffer() { return _buffer; }
    StrBuffer &clearNameBuffer() { return _buffer.reset(_name, sizeof(_name)); }
    void reset(uint32_t addr) {
        _address = addr;
        _length = 0;
    }

    /** No copy constructor. */
    Insn(Insn const &) = delete;
    /** No assignment operator. */
    void operator=(Insn const &) = delete;

    void emitByte(uint8_t val) { emitByte(val, _length); }

    void emitByte(uint8_t val, uint8_t pos) {
        if (pos < MAX_CODE) {
            _bytes[pos++] = val;
            if (_length < pos)
                _length = pos;
        }
    }

private:
    uint32_t _address;
    uint8_t _length;
    StrBuffer _buffer;

    static constexpr size_t MAX_CODE = 24;
    uint8_t _bytes[MAX_CODE];

    static constexpr size_t MAX_NAME = 11;
    char _name[MAX_NAME + 1];
};

/**
 * Base instruction code class.
 */
struct AsmInsnBase : ErrorReporter {
    AsmInsnBase(Insn &insn) : ErrorReporter(), _insn(insn) {}

    uint32_t address() const { return _insn.address(); }
    const uint8_t *bytes() const { return _insn.bytes(); }
    uint8_t length() const { return _insn.length(); }
    const char *name() const { return _insn.name(); }
    StrBuffer &nameBuffer() { return _insn.nameBuffer(); }
    StrBuffer &clearNameBuffer() { return _insn.clearNameBuffer(); }

    void reset(uint32_t addr) {
        resetError();
        _insn.reset(addr);
        clearNameBuffer();
    }

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

private:
    Insn &_insn;
};

/**
 * Base instruction code class.
 */
struct DisInsnBase : ErrorReporter {
    DisInsnBase(Insn &insn, DisMemory &memory) : ErrorReporter(), _insn(insn), _memory(memory) {}

    uint32_t address() const { return _insn.address(); }
    const uint8_t *bytes() const { return _insn.bytes(); }
    uint8_t length() const { return _insn.length(); }
    const char *name() const { return _insn.name(); }
    StrBuffer &nameBuffer() { return _insn.nameBuffer(); }
    StrBuffer &clearNameBuffer() { return _insn.clearNameBuffer(); }

    void reset(uint32_t addr) {
        resetError();
        _insn.reset(addr);
        clearNameBuffer();
    }

    /** Read 8 bit data from |memory| (Disassembler). */
    uint8_t readByte() {
        if (!_memory.hasNext()) {
            setError(NO_MEMORY);
            return 0;
        }
        const uint8_t data = _memory.readByte();
        _insn.emitByte(data);
        return data;
    }

    /** Read 16 bit big endian data from |memory| (Disassembler). */
    uint16_t readUint16Be() {
        const uint8_t msb = readByte();
        const uint8_t lsb = readByte();
        return static_cast<uint16_t>(msb) << 8 | lsb;
    }

    /** Read 16 bit little endian data from |memory| (Disassembler). */
    uint16_t readUint16Le() {
        const uint8_t lsb = readByte();
        const uint8_t msb = readByte();
        return static_cast<uint16_t>(msb) << 8 | lsb;
    }

    /** Read 32 bit big endian data from |memory| (Disassembler). */
    uint32_t readUint32Be() {
        const uint16_t msw = readUint16Be();
        const uint16_t lsw = readUint16Be();
        return static_cast<uint32_t>(msw) << 16 | lsw;
    }

    /** Read 32 bit little endian data from |memory| (Disassembler). */
    uint32_t readUint32Le() {
        const uint16_t lsw = readUint16Le();
        const uint16_t msw = readUint16Le();
        return static_cast<uint32_t>(msw) << 16 | lsw;
    }

    /** Read 64 bit big endian data from |memory| (Disassembler). */
    uint64_t readUint64Be() {
        const uint32_t msw = readUint32Be();
        const uint32_t lsw = readUint32Be();
        return static_cast<uint64_t>(msw) << 32 | lsw;
    }

    /** Read 64 bit little endian data from |memory| (Disassembler). */
    uint64_t readUint64Le() {
        const uint32_t lsw = readUint32Le();
        const uint32_t msw = readUint32Le();
        return static_cast<uint64_t>(msw) << 32 | lsw;
    }

protected:
    Insn &_insn;
    DisMemory &_memory;
};

template <typename Conf, typename Entry>
struct EntryInsnBase {
    EntryInsnBase() : _opCode(0), _prefix(0), _post(0), _hasPost(false), _flags() {}
    void setOpCode(typename Conf::opcode_t opCode, typename Conf::opcode_t prefix = 0) {
        _opCode = opCode;
        _prefix = prefix;
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

private:
    typename Conf::opcode_t _opCode;
    typename Conf::opcode_t _prefix;
    typename Conf::opcode_t _post;
    bool _hasPost;
    typename Entry::Flags _flags;
};

template <typename Conf>
struct AsmInsnImpl : AsmInsnBase {
    typename Conf::uintptr_t address() const { return AsmInsnBase::address(); }

    void reset() { AsmInsnBase::reset(address()); }

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

protected:
    AsmInsnImpl(Insn &insn) : AsmInsnBase(insn) {}
};

template <typename Conf>
struct DisInsnImpl : DisInsnBase {
    typename Conf::uintptr_t address() const { return DisInsnBase::address(); }

    void reset() { DisInsnBase::reset(address()); }

    /** Read 16 bit data from |memory| (Disassembler). */
    uint16_t readUint16() {
        if (Conf::ENDIAN == ENDIAN_BIG) {
            return readUint16Be();
        } else {
            return readUint16Le();
        }
    }

    /** Read 32 bit data from |memory| (Disassembler). */
    uint32_t readUint32() {
        if (Conf::ENDIAN == ENDIAN_BIG) {
            return readUint32Be();
        } else {
            return readUint32Le();
        }
    }

    /** Read 64 bit data from |memory| (Disassembler). */
    uint64_t readUint64() {
        if (Conf::ENDIAN == ENDIAN_BIG) {
            return readUint64Be();
        } else {
            return readUint64Le();
        }
    }

    /** Read 32 bit floating point data from |memory| (Disassembler). */
    float readFloat32() {
        union {
            float float32;
            uint32_t data32;
        } bytes;
        bytes.data32 = readUint32();
        return bytes.float32;
    }

    /** Read 64 bit floating point data from |memory| (Disassembler). */
    double readFloat64() {
        union {
            double float64;
            uint64_t data64;
        } bytes;
        bytes.data64 = readUint64();
        return bytes.float64;
    }

protected:
    DisInsnImpl(Insn &insn, DisMemory &memory) : DisInsnBase(insn, memory) {}
};

}  // namespace libasm

#endif  // __INSN_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
