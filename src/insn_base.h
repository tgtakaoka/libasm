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

#ifndef __LIBASM_INSN_BASE_H__
#define __LIBASM_INSN_BASE_H__

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "config_base.h"
#include "dis_memory.h"
#include "error_reporter.h"
#include "float80.h"
#include "str_buffer.h"
#include "str_scanner.h"

namespace libasm {

/**
 * General instruction code interface for Assembler and Disassembler.
 */
struct Insn final : ErrorAt {
    Insn(uint32_t addr);

    uint32_t address() const { return _address; }
    const uint8_t *bytes() const { return _bytes; }
    uint8_t length() const { return _length; }
    const char *name() const { return _name; }
    StrBuffer &nameBuffer() { return _buffer; }
    void setAddress(uint32_t addr) { _address = addr; }
    void reset(uint32_t addr, uint8_t length = 0);
    uint32_t align(uint8_t step);

    /** No copy constructor. */
    Insn(Insn const &) = delete;
    /** No assignment operator. */
    void operator=(Insn const &) = delete;

    /** Generate 8 bit |data| (Assembler). */
    Error emitByte(uint8_t val) { return emitByte(val, _length); }

    /** Generate 16 bit big endian |data| (Assembler). */
    Error emitUint16Be(uint16_t data) { return emitUint16Be(data, _length); }

    /** Generate 16 bit little endian |data| (Assembler). */
    Error emitUint16Le(uint16_t data) { return emitUint16Le(data, _length); }

    /** Generate 32 bit big endian |data| (Assembler). */
    Error emitUint32Be(uint32_t data) { return emitUint32Be(data, _length); }

    /** Generate 32 bit little endian |data| (Assembler). */
    Error emitUint32Le(uint32_t data) { return emitUint32Le(data, _length); }

    /** Generate 64 bit big enditan |data| (Assembler). */
    Error emitUint64Be(uint64_t data) { return emitUint64Be(data, _length); }

    /** Generate 64 bit little endian |data| (Assembler). */
    Error emitUint64Le(uint64_t data) { return emitUint64Le(data, _length); }

#if !defined(LIBASM_ASM_NOFLOAT)
    /** Generate 32 bit big endian floating point |data|(Assembler). */
    Error emitFloat32Be(const float80_t &data) { return emitFloat32Be(data, _length); }

    /** Generate 32 bit little endian floating point |data| (Assembler). */
    Error emitFloat32Le(const float80_t &data) { return emitFloat32Le(data, _length); }

    /** Generate 64 bit big endian floating point |data| (Assembler). */
    Error emitFloat64Be(const float80_t &data) { return emitFloat64Be(data, _length); }

    /** Generate 64 bit little endian floating point |data| (Assembler). */
    Error emitFloat64Le(const float80_t &data) { return emitFloat64Le(data, _length); }
#endif

    /** Generate 8 bit |data| at |pos| (Assembler). */
    Error emitByte(uint8_t val, uint8_t pos);

    /** Generate 16 bit big endian |data| at |pos| (Assembler). */
    Error emitUint16Be(uint16_t data, uint8_t pos);

    /** Generate 16 bit little endian |data| at |pos| (Assembler). */
    Error emitUint16Le(uint16_t data, uint8_t pos);

    /** Generate 32 bit big endian |data| at |pos| (Assembler). */
    Error emitUint32Be(uint32_t data, uint8_t pos);

    /** Generate 32 bit little endian |data| at |pos| (Assembler). */
    Error emitUint32Le(uint32_t data, uint8_t pos);

    /** Generate 64 bit big endian |data| at |pos| (Assembler). */
    Error emitUint64Be(uint64_t data, uint8_t pos);

    /** Generate 64 bit little endian |data| at |pos| (Assembler). */
    Error emitUint64Le(uint64_t data, uint8_t pos);

#if !defined(LIBASM_ASM_NOFLOAT)
    /** Generate 32 bit big endian floating point |data| at |pos| (Assembler). */
    Error emitFloat32Be(const float80_t &data, uint8_t pos);

    /** Generate 32 bit little endian floating point |data| at |pos| (Assembler). */
    Error emitFloat32Le(const float80_t &data, uint8_t pos);

    /** Generate 64 bit big endian floating point |data| at |pos| (Assembler). */
    Error emitFloat64Be(const float80_t &data, uint8_t pos);

    /** Generate 64 bit little endian floating point |data| at |pos| (Assembler). */
    Error emitFloat64Le(const float80_t &data, uint8_t pos);
#endif

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
    AsmInsnBase(Insn &insn) : ErrorAt(), _insn(insn) {}

    uint32_t address() const { return _insn.address(); }
    const uint8_t *bytes() const { return _insn.bytes(); }
    uint8_t length() const { return _insn.length(); }
    const char *name() const { return _insn.name(); }
    StrBuffer &nameBuffer() { return _insn.nameBuffer(); }
    Insn &insnBase() { return _insn; }

    void reset(uint32_t addr) {
        resetError();
        _insn.reset(addr);
    }
    void resetBytes() { _insn.reset(address(), 0); }
    void resetAddress(uint32_t addr) { _insn.setAddress(addr); }

    /** Generate 8 bit |data| (Assembler). */
    Error emitByte(uint8_t data) { return _insn.emitByte(data); }

    /** Generate 8 bit |data| at |pos| (Assembler). */
    Error emitByte(uint8_t data, uint8_t pos) { return _insn.emitByte(data, pos); }

    /** Generate 16 bit big endian |data| (Assembler). */
    Error emitUint16Be(uint16_t data) { return _insn.emitUint16Be(data); }

    /** Generate 16 bit little endian |data| (Assembler). */
    Error emitUint16Le(uint16_t data) { return _insn.emitUint16Le(data); }

    /** Generate 16 bit big endian |data| at |pos| (Assembler). */
    Error emitUint16Be(uint16_t data, uint8_t pos) { return _insn.emitUint16Be(data, pos); }

    /** Generate 16 bit little endian |data| at |pos| (Assembler). */
    Error emitUint16Le(uint16_t data, uint8_t pos) { return _insn.emitUint16Le(data, pos); }

    /** Generate 32 bit big endian |data| (Assembler). */
    Error emitUint32Be(uint32_t data) { return _insn.emitUint32Be(data); }

    /** Generate 32 bit little endian |data| (Assembler). */
    Error emitUint32Le(uint32_t data) { return _insn.emitUint32Le(data); }

    /** Generate 32 bit big endian |data| at |pos| (Assembler). */
    Error emitUint32Be(uint32_t data, uint8_t pos) { return _insn.emitUint32Be(data, pos); }

    /** Generate 32 bit little endian |data| at |pos| (Assembler). */
    Error emitUint32Le(uint32_t data, uint8_t pos) { return _insn.emitUint32Le(data, pos); }

    /** Generate 64 bit big endian |data| (Assembler). */
    Error emitUint64Be(uint64_t data) { return _insn.emitUint64Be(data); }

    /** Generate 64 bit little endian |data| (Assembler). */
    Error emitUint64Le(uint64_t data) { return _insn.emitUint64Le(data); }

    /** Generate 64 bit big endian |data| at |pos| (Assembler). */
    Error emitUint64Be(uint64_t data, uint8_t pos) { return _insn.emitUint64Be(data, pos); }

    /** Generate 64 bit little endian |data| at |pos| (Assembler). */
    Error emitUint64Le(uint64_t data, uint8_t pos) { return _insn.emitUint64Le(data, pos); }

#if !defined(LIBASM_ASM_NOFLOAT)
    /** Generate 32 bit big endian floating point |data|(Assembler). */
    Error emitFloat32Be(const float80_t &data) { return _insn.emitFloat32Be(data); }

    /** Generate 32 bit little endian floating point |data| (Assembler). */
    Error emitFloat32Le(const float80_t &data) { return _insn.emitFloat32Le(data); }

    /** Generate 32 bit big endian floating point |data| at |pos| (Assembler). */
    Error emitFloat32Be(const float80_t &data, uint8_t pos) {
        return _insn.emitFloat32Be(data, pos);
    }

    /** Generate 32 bit little endian floating point |data| at |pos| (Assembler). */
    Error emitFloat32Le(const float80_t &data, uint8_t pos) {
        return _insn.emitFloat32Le(data, pos);
    }

    /** Generate 64 bit big endian floating point |data| (Assembler). */
    Error emitFloat64Be(const float80_t &data) { return _insn.emitFloat64Be(data); }

    /** Generate 64 bit little endian floating point |data| (Assembler). */
    Error emitFloat64Le(const float80_t &data) { return _insn.emitFloat64Le(data); }

    /** Generate 64 bit big endian floating point |data| at |pos| (Assembler). */
    Error emitFloat64Be(const float80_t &data, uint8_t pos) {
        return _insn.emitFloat64Be(data, pos);
    }

    /** Generate 64 bit little endian floating point |data| at |pos| (Assembler). */
    Error emitFloat64Le(const float80_t &data, uint8_t pos) {
        return _insn.emitFloat64Le(data, pos);
    }
#endif

    //protected:
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

    void resetLength(uint8_t length = 0);

    /** Read 8 bit data. */
    uint8_t readByte();

    /** Read 16 bit big endian data */
    uint16_t readUint16Be();

    /** Read 16 bit little endian data */
    uint16_t readUint16Le();

    /** Read 32 bit big endian data */
    uint32_t readUint32Be();

    /** Read 32 bit little endian data */
    uint32_t readUint32Le();

    /** Read 64 bit big endian data */
    uint64_t readUint64Be();

    /** Read 64 bit little endian data */
    uint64_t readUint64Le();

#if !defined(LIBASM_DIS_NOFLOAT)
    /** Read 32 bit big endian floating point data */
    float80_t readFloat32Be();

    /** Read 32 bit little endian floating point data */
    float80_t readFloat32Le();

    /** Read 64 bit big endian floating point data */
    float80_t readFloat64Be();

    /** Read 64 bit little endian floating point data */
    float80_t readFloat64Le();
#endif

protected:
    Insn &_insn;
    DisMemory &_memory;
    const StrBuffer &_out;

    DisInsnBase(Insn &insn, DisMemory &memory, const StrBuffer &out)
        : ErrorAt(), _insn(insn), _memory(memory), _out(out) {}
    DisInsnBase(Insn &insn, DisInsnBase &o, const StrBuffer &out)
        : ErrorAt(), _insn(insn), _memory(o._memory), _out(out) {}
    DisInsnBase(DisInsnBase &o, const StrBuffer &out)
        : ErrorAt(*this), _insn(o._insn), _memory(o._memory), _out(out) {}
};

template <typename Conf, typename Entry>
struct EntryInsnBase {
    EntryInsnBase() : _opCode(0), _flags() {}
    using opcode_t = typename Conf::opcode_t;
    using Flags = typename Entry::Flags;
    void setOpCode(opcode_t opCode) { _opCode = opCode; }
    opcode_t opCode() const { return _opCode; }
    void embed(opcode_t data) { _opCode |= data; }
    void setFlags(Flags flags) { _flags = flags; }
    Flags flags() const { return _flags; }
    Flags &flags() { return _flags; }

private:
    opcode_t _opCode;
    Flags _flags;
};

template <typename Conf, typename Entry>
struct EntryInsnPrefix : virtual EntryInsnBase<Conf, Entry> {
    EntryInsnPrefix() : _prefix(0) {}
    using opcode_t = typename Conf::opcode_t;
    void setPrefix(uint16_t prefix) { _prefix = prefix; }
    bool hasPrefix() const { return _prefix != 0; }
    uint16_t prefix() const { return _prefix; }

private:
    uint16_t _prefix;
};

template <typename Conf, typename Entry>
struct EntryInsnPostfix : virtual EntryInsnBase<Conf, Entry> {
    EntryInsnPostfix() : _postfix(0), _hasPostfix(false) {}
    using opcode_t = typename Conf::opcode_t;
    void setPostfix(opcode_t postfix, bool hasPostfix = true) {
        _postfix = postfix;
        _hasPostfix = hasPostfix;
    }
    void embedPostfix(opcode_t data) { setPostfix(_postfix | data); }
    bool hasPostfix() const { return _hasPostfix; }
    opcode_t postfix() const { return _postfix; }

private:
    opcode_t _postfix;
    bool _hasPostfix;
};

template <typename Conf, typename Entry>
struct EntryInsnPrePostfix : EntryInsnPrefix<Conf, Entry>, EntryInsnPostfix<Conf, Entry> {
    EntryInsnPrePostfix() {}
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

protected:
    AsmInsnImpl(Insn &insn) : AsmInsnBase(insn) {}

private:
    static constexpr bool big = Conf::ENDIAN == ENDIAN_BIG;
};

template <typename Conf>
struct DisInsnImpl : DisInsnBase {
    typename Conf::uintptr_t address() const { return DisInsnBase::address(); }

    /** Read 16 bit data */
    uint16_t readUint16() { return big ? readUint16Be() : readUint16Le(); }

    /** Read 32 bit data */
    uint32_t readUint32() { return big ? readUint32Be() : readUint32Le(); }

    /** Read 64 bit data */
    uint64_t readUint64() { return big ? readUint64Be() : readUint64Le(); }

protected:
    DisInsnImpl(Insn &insn, DisMemory &memory, const StrBuffer &out)
        : DisInsnBase(insn, memory, out) {}
    DisInsnImpl(Insn &insn, DisInsnImpl &o, const StrBuffer &out) : DisInsnBase(insn, o, out) {}
    DisInsnImpl(DisInsnImpl &o, const StrBuffer &out) : DisInsnBase(o, out) {}

private:
    static constexpr bool big = Conf::ENDIAN == ENDIAN_BIG;
};

}  // namespace libasm

#endif  // __LIBASM_INSN_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
