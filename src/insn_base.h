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
struct Insn final : ErrorAt {
    Insn(uint32_t addr) : ErrorAt(), _address(addr), _length(0) {
        memset(_bytes, 0, sizeof(_bytes));
    }

    uint32_t address() const { return _address; }
    const uint8_t *bytes() const { return _bytes; }
    uint8_t length() const { return _length; }
    const char *name() const { return _name; }
    StrBuffer &nameBuffer() { return _buffer; }
    void reset(uint32_t addr) {
        resetError();
        setAt(StrScanner::EMPTY);
        _address = addr;
        _length = 0;
        memset(_bytes, 0, sizeof(_bytes));
        _buffer.reset();
    }
    uint32_t align(uint8_t step) {
        const uint8_t rem = _address % step;
        if (rem)
            _address += step - rem;
        return _address;
    }

    /** No copy constructor. */
    Insn(Insn const &) = delete;
    /** No assignment operator. */
    void operator=(Insn const &) = delete;

    Error emitByte(uint8_t val) { return emitByte(val, _length); }

    Error emitByte(uint8_t val, uint8_t pos) {
        if (pos >= MAX_CODE)
            return NO_MEMORY;
        _bytes[pos++] = val;
        if (_length < pos)
            _length = pos;
        return OK;
    }

    /** Generate 16 bit big endian |data| (Assembler). */
    Error emitUint16Be(uint16_t data) {
        emitByte(data >> 8);
        return emitByte(data >> 0);
    }

    /** Generate 16 bit little endian |data| (Assembler). */
    Error emitUint16Le(uint16_t data) {
        emitByte(data >> 0);
        return emitByte(data >> 8);
    }

    /** Generate 16 bit big endian |data| at |pos| (Assembler). */
    Error emitUint16Be(uint16_t data, uint8_t pos) {
        emitByte(data >> 8, pos + 0);
        return emitByte(data >> 0, pos + 1);
    }

    /** Generate 16 bit little endian |data| at |pos| (Assembler). */
    Error emitUint16Le(uint16_t data, uint8_t pos) {
        emitByte(data >> 0, pos + 0);
        return emitByte(data >> 8, pos + 1);
    }

    /** Generate 32 bit big endian |data| (Assembler). */
    Error emitUint32Be(uint32_t data) {
        emitUint16Be(data >> 16);
        return emitUint16Be(data >> 0);
    }

    /** Generate 32 bit little endian |data| (Assembler). */
    Error emitUint32Le(uint32_t data) {
        emitUint16Le(data >> 0);
        return emitUint16Le(data >> 16);
    }

    /** Generate 32 bit big endian |data| at |pos| (Assembler). */
    Error emitUint32Be(uint32_t data, uint8_t pos) {
        emitUint16Be(data >> 16, pos + 0);
        return emitUint16Be(data >> 0, pos + 2);
    }

    /** Generate 32 bit little endian |data| at |pos| (Assembler). */
    Error emitUint32Le(uint32_t data, uint8_t pos) {
        emitUint16Le(data >> 0, pos + 0);
        return emitUint16Le(data >> 16, pos + 2);
    }

    /** Generate 64 bit big enditan |data| (Assembler). */
    Error emitUint64Be(uint64_t data) {
        emitUint32Be(data >> 32);
        return emitUint32Be(data >> 0);
    }

    /** Generate 64 bit little endian |data| (Assembler). */
    Error emitUint64Le(uint64_t data) {
        emitUint32Le(data >> 0);
        return emitUint32Le(data >> 32);
    }

    /** Generate 64 bit big endian |data| at |pos| (Assembler). */
    Error emitUint64Be(uint64_t data, uint8_t pos) {
        emitUint32Be(data >> 32, pos + 0);
        return emitUint32Be(data >> 0, pos + 4);
    }

    /** Generate 64 bit little endian |data| at |pos| (Assembler). */
    Error emitUint64Le(uint64_t data, uint8_t pos) {
        emitUint32Le(data >> 0, pos + 0);
        return emitUint32Le(data >> 32, pos + 4);
    }

private:
    uint32_t _address;
    uint8_t _length;

    static constexpr size_t MAX_NAME = 11;
    char _name[MAX_NAME + 1];
    StrBuffer _buffer{_name, sizeof(_name)};

    static constexpr size_t MAX_CODE = 64;
    uint8_t _bytes[MAX_CODE];
};

/**
 * Base for assembler instruction code.
 */
struct AsmInsnBase : ErrorAt {
    uint32_t address() const { return _insn.address(); }
    const uint8_t *bytes() const { return _insn.bytes(); }
    uint8_t length() const { return _insn.length(); }
    const char *name() const { return _insn.name(); }
    StrBuffer &nameBuffer() { return _insn.nameBuffer(); }

    void reset(uint32_t addr) {
        resetError();
        _insn.reset(addr);
    }

    /** Generate 8 bit |data| (Assembler). */
    Error emitByte(uint8_t data) { return setErrorIf(_insn.emitByte(data)); }

    /** Generate 8 bit |data| at |pos| (Assembler). */
    Error emitByte(uint8_t data, uint8_t pos) { return setErrorIf(_insn.emitByte(data, pos)); }

    /** Generate 16 bit big endian |data| (Assembler). */
    Error emitUint16Be(uint16_t data) { return setErrorIf(_insn.emitUint16Be(data)); }

    /** Generate 16 bit little endian |data| (Assembler). */
    Error emitUint16Le(uint16_t data) { return setErrorIf(_insn.emitUint16Le(data)); }

    /** Generate 16 bit big endian |data| at |pos| (Assembler). */
    Error emitUint16Be(uint16_t data, uint8_t pos) {
        return setErrorIf(_insn.emitUint16Be(data, pos));
    }

    /** Generate 16 bit little endian |data| at |pos| (Assembler). */
    Error emitUint16Le(uint16_t data, uint8_t pos) {
        return setErrorIf(_insn.emitUint16Le(data, pos));
    }

    /** Generate 32 bit big endian |data| (Assembler). */
    Error emitUint32Be(uint32_t data) { return setErrorIf(_insn.emitUint32Be(data)); }

    /** Generate 32 bit little endian |data| (Assembler). */
    Error emitUint32Le(uint32_t data) { return setErrorIf(_insn.emitUint32Le(data)); }

    /** Generate 32 bit big endian |data| at |pos| (Assembler). */
    Error emitUint32Be(uint32_t data, uint8_t pos) {
        return setErrorIf(_insn.emitUint32Be(data, pos));
    }

    /** Generate 32 bit little endian |data| at |pos| (Assembler). */
    Error emitUint32Le(uint32_t data, uint8_t pos) {
        return setErrorIf(_insn.emitUint32Le(data, pos));
    }

    /** Generate 64 bit big endian |data| (Assembler). */
    Error emitUint64Be(uint64_t data) { return setErrorIf(_insn.emitUint64Be(data)); }

    /** Generate 64 bit little endian |data| (Assembler). */
    Error emitUint64Le(uint64_t data) { return setErrorIf(_insn.emitUint64Le(data)); }

    /** Generate 64 bit big endian |data| at |pos| (Assembler). */
    Error emitUint64Be(uint64_t data, uint8_t pos) {
        return setErrorIf(_insn.emitUint64Be(data, pos));
    }

    /** Generate 64 bit little endian |data| at |pos| (Assembler). */
    Error emitUint64Le(uint64_t data, uint8_t pos) {
        return setErrorIf(_insn.emitUint64Le(data, pos));
    }

protected:
    AsmInsnBase(Insn &insn) : ErrorAt(), _insn(insn) {}

private:
    Insn &_insn;
};

/**
 * Base for disassembler instruction code.
 */
struct DisInsnBase : ErrorAt {
    uint32_t address() const { return _insn.address(); }
    const uint8_t *bytes() const { return _insn.bytes(); }
    uint8_t length() const { return _insn.length(); }
    const char *name() const { return _insn.name(); }
    StrBuffer &nameBuffer() { return _insn.nameBuffer(); }

    void reset(uint32_t addr) {
        resetError();
        _insn.reset(addr);
    }

    /** Read 8 bit data. */
    uint8_t readByte() {
        if (!_memory.hasNext()) {
            setErrorIf(_out, NO_MEMORY);
            return 0;
        }
        const uint8_t data = _memory.readByte();
        if (_insn.emitByte(data))
            setErrorIf(_out, NO_MEMORY);
        return data;
    }

    /** Read 16 bit big endian data */
    uint16_t readUint16Be() {
        const uint8_t msb = readByte();
        const uint8_t lsb = readByte();
        return static_cast<uint16_t>(msb) << 8 | lsb;
    }

    /** Read 16 bit little endian data */
    uint16_t readUint16Le() {
        const uint8_t lsb = readByte();
        const uint8_t msb = readByte();
        return static_cast<uint16_t>(msb) << 8 | lsb;
    }

    /** Read 32 bit big endian data */
    uint32_t readUint32Be() {
        const uint16_t msw = readUint16Be();
        const uint16_t lsw = readUint16Be();
        return static_cast<uint32_t>(msw) << 16 | lsw;
    }

    /** Read 32 bit little endian data */
    uint32_t readUint32Le() {
        const uint16_t lsw = readUint16Le();
        const uint16_t msw = readUint16Le();
        return static_cast<uint32_t>(msw) << 16 | lsw;
    }

    /** Read 64 bit big endian data */
    uint64_t readUint64Be() {
        const uint32_t msw = readUint32Be();
        const uint32_t lsw = readUint32Be();
        return static_cast<uint64_t>(msw) << 32 | lsw;
    }

    /** Read 64 bit little endian data */
    uint64_t readUint64Le() {
        const uint32_t lsw = readUint32Le();
        const uint32_t msw = readUint32Le();
        return static_cast<uint64_t>(msw) << 32 | lsw;
    }

protected:
    DisInsnBase(Insn &insn, DisMemory &memory, const StrBuffer &out)
        : ErrorAt(), _insn(insn), _memory(memory), _out(out) {}
    DisInsnBase(Insn &insn, DisInsnBase &o, const StrBuffer &out)
        : ErrorAt(), _insn(insn), _memory(o._memory), _out(out) {}
    DisInsnBase(DisInsnBase &o, const StrBuffer &out)
        : ErrorAt(*this), _insn(o._insn), _memory(o._memory), _out(out) {}

private:
    Insn &_insn;
    DisMemory &_memory;
    const StrBuffer &_out;
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

    /** Generate 16 bit |data| */
    Error emitUint16(uint16_t data) { return big ? emitUint16Be(data) : emitUint16Le(data); }

    /** Generate 16 bit |data| at |pos| */
    Error emitUint16(uint16_t data, uint8_t pos) {
        return big ? emitUint16Be(data, pos) : emitUint16Le(data, pos);
    }

    /** Generate 32 bit |data| */
    Error emitUint32(uint32_t data) { return big ? emitUint32Be(data) : emitUint32Le(data); }

    /** Generate 32 bit |data| at |pos| */
    Error emitUint32(uint32_t data, uint8_t pos) {
        return big ? emitUint32Be(data, pos) : emitUint32Le(data, pos);
    }

    /** Generate 64 bit |data| */
    Error emitUint64(uint64_t data) { return big ? emitUint64Be(data) : emitUint64Le(data); }

    /** Generate 64 bit |data| at |pos| */
    Error emitUint64(uint64_t data, uint8_t pos) {
        return big ? emitUint64Be(data, pos) : emitUint64Le(data, pos);
    }

    /** Generate 32 bit floating point |data| */
    Error emitFloat32(float data) {
        union {
            float float32;
            uint32_t data32;
        } bytes;
        bytes.float32 = data;
        return emitUint32(bytes.data32);
    }

    /** Generate 32 bit floating point |data| at |pos| */
    Error emitFloat32(float data, uint8_t pos) {
        union {
            float float32;
            uint32_t data32;
        } bytes;
        bytes.float32 = data;
        return emitUint32(bytes.data32, pos);
    }

    /** Generate 64 bit floating point |data| */
    Error emitFloat64(double data) {
        union {
            double float64;
            uint64_t data64;
        } bytes;
        bytes.float64 = data;
        return emitUint64(bytes.data64);
    }

    /** Generate 64 bit floating point |data| at |pos| */
    Error emitFloat64(double data, uint8_t pos) {
        union {
            double float64;
            uint64_t data64;
        } bytes;
        bytes.float64 = data;
        return emitUint64(bytes.data64, pos);
    }

protected:
    AsmInsnImpl(Insn &insn) : AsmInsnBase(insn) {}

private:
    static constexpr bool big = Conf::ENDIAN == ENDIAN_BIG;
};

template <typename Conf>
struct DisInsnImpl : DisInsnBase {
    typename Conf::uintptr_t address() const { return DisInsnBase::address(); }

    void reset() { DisInsnBase::reset(address()); }

    /** Read 16 bit data */
    uint16_t readUint16() { return big ? readUint16Be() : readUint16Le(); }

    /** Read 32 bit data */
    uint32_t readUint32() { return big ? readUint32Be() : readUint32Le(); }

    /** Read 64 bit data */
    uint64_t readUint64() { return big ? readUint64Be() : readUint64Le(); }

    /** Read 32 bit floating point data */
    float readFloat32() {
        union {
            float float32;
            uint32_t data32;
        } bytes;
        bytes.data32 = readUint32();
        return bytes.float32;
    }

    /** Read 64 bit floating point data */
    double readFloat64() {
        union {
            double float64;
            uint64_t data64;
        } bytes;
        bytes.data64 = readUint64();
        return bytes.float64;
    }

protected:
    DisInsnImpl(Insn &insn, DisMemory &memory, const StrBuffer &out)
        : DisInsnBase(insn, memory, out) {}
    DisInsnImpl(Insn &insn, DisInsnImpl &o, const StrBuffer &out) : DisInsnBase(insn, o, out) {}
    DisInsnImpl(DisInsnImpl &o, const StrBuffer &out) : DisInsnBase(o, out) {}

private:
    static constexpr bool big = Conf::ENDIAN == ENDIAN_BIG;
};

}  // namespace libasm

#endif  // __INSN_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
