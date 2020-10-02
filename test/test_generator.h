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

#ifndef __TEST_GENERATOR_H__
#define __TEST_GENERATOR_H__

#include "dis_base.h"
#include "dis_memory.h"
#include "text_buffer.h"

#include <ctype.h>
#include <unordered_map>
#include <unordered_set>

#if DEBUG_TEXT || DEBUG_TRACE
#include <stdio.h>
#endif

namespace libasm {
namespace test {

class DataGenerator {
public:
    typedef bool (*Filter)(uint8_t);

    DataGenerator(uint8_t *buffer, Endian endian, int size, Filter filter = nullptr)
        : _buffer(buffer),
          _endian(endian),
          _pos(0),
          _size(size),
          _end(calcEnd(0, size)),
          _filter(filter),
          _i(0)
    {}

    DataGenerator(const DataGenerator &parent, int size, Filter filter = nullptr)
        : _buffer(parent._buffer),
          _endian(parent._endian),
          _pos(parent._pos + parent._size),
          _size(size),
          _end(calcEnd(parent._i, size)),
          _filter(filter),
          _i(parent._i)
    {}

    int pos() const { return _pos; }
    int size() const { return _size; }

    void next() {
        _i = outSizedUint(_i) + 1;
    }
    bool hasNext() {
        if (_i == _end) {
            outSizedUint(0);
            return false;
        }
        return true;
    }

    void outUint8(uint8_t val8, int offset = 0) {
        _buffer[_pos + offset] = val8;
    }

    void debugPrint(const char *msg, const uint8_t *memory) const {
#if DEBUG_TRACE
        printf("%s %d/%d%c: ", msg, _pos, _size, _endian == ENDIAN_BIG? 'B' : 'L');
        for (int i = 0; i < _pos; i++)
            printf(" %02X", memory[i]);
        for (int i = 0; i < _size; i++)
            printf("%c%02X", i == 0 ? '|' : ' ', memory[_pos + i]);
        printf("\n");
        fflush(stdout);
#endif
    }

private:
    uint8_t *const _buffer;
    const Endian _endian;
    int _pos;
    int _size;
    const uint32_t _end;
    Filter _filter;
    uint32_t _i;

    static uint32_t calcEnd(uint32_t start, int size) {
        if (size == 1) return start + 0x100;
        if (size == 2) return start + 0x10000L;
        return start;
    }

    uint32_t outSizedUint(uint32_t val) {
        if (_size == 1) {
            while (_filter && _filter(val))
                val++;
            outUint8(static_cast<uint8_t>(val));
        } else if (_size == 2) {
            outUint16(static_cast<uint16_t>(val));
        } else {
            outUint32(val);
        }
        return val;
    }

    void outUint16(uint16_t val16, int offset = 0) {
        if (_endian == ENDIAN_BIG) {
            outUint8(static_cast<uint8_t>(val16 >> 8), offset + 0);
            outUint8(static_cast<uint8_t>(val16 >> 0), offset + 1);
        } else {
            outUint8(static_cast<uint8_t>(val16 >> 0), offset + 0);
            outUint8(static_cast<uint8_t>(val16 >> 8), offset + 1);
        }
    }

    void outUint32(uint32_t val32, int offset = 0) {
        if (_endian == ENDIAN_BIG) {
            outUint16(static_cast<uint16_t>(val32 >> 16), offset + 0);
            outUint16(static_cast<uint16_t>(val32 >> 0),  offset + 2);
        } else {
            outUint16(static_cast<uint16_t>(val32 >> 0),  offset + 0);
            outUint16(static_cast<uint16_t>(val32 >> 16), offset + 2);
        }
    }
};

template<typename Conf>
class TestData : private DisMemory {
public:
    TestData()
        : DisMemory(0),
          _operands(60)
    {}

    const Insn &insn() const { return _insn; }
    TextBuffer &operands() { return _operands; }
    const TextBuffer &operands() const { return _operands; }
    Error tryGenerate(
        Disassembler &dis,
        typename Conf::uintptr_t addr,
        uint8_t *memory,
        int size) {
        resetAddress(addr);
        _memory = memory;
        _memorySize = size;
        _memoryIndex = 0;

        return dis.decode(*this, _insn, _operands.buffer(), nullptr);
    }

private:
    TextBuffer _operands;
    uint8_t *_memory;
    int _memorySize;
    int _memoryIndex;
    Insn _insn;

    // DisMemory
    bool hasNext() const override { return _memoryIndex < _memorySize; }
    uint8_t nextByte() override { return _memory[_memoryIndex++]; }
};

template<typename Conf>
class TestGenerator {
public:
    TestGenerator(
        Disassembler &disassembler,
        typename Conf::uintptr_t addr = 0)
        : _disassembler(disassembler),
          _memorySize(Conf::MAX_CODE),
          _endian(Conf::ENDIAN),
          _opcodeSize(sizeof(typename Conf::opcode_t)) {
        _memory = new uint8_t[_memorySize];
        _addr = addr;
    }

    virtual ~TestGenerator() {
        delete[] _memory;
    }

    class Printer {
    public:
        virtual void print(const Insn &insn, const char *operands) = 0;
        virtual void origin(typename Conf::uintptr_t addr) = 0;
    };
    typedef bool (*Filter)(uint8_t);

    TestGenerator<Conf> &generate(Printer &printer, Filter filter = nullptr) {
        DataGenerator gen(_memory, _endian, _opcodeSize, filter);
        if (_addr) printer.origin(_addr);
        return generate(printer, gen);
    }

    TestGenerator<Conf> &generate(
        Printer &printer, uint8_t opc1, Filter filter = nullptr) {
        DataGenerator parent(_memory, _endian, _opcodeSize);
        parent.outUint8(opc1);
        DataGenerator gen(parent, _opcodeSize, filter);
        return generate(printer, gen);
    }

    TestGenerator<Conf> &generate(
        Printer &printer, uint8_t opc1, uint8_t opc2) {
        DataGenerator parent(_memory, _endian, _opcodeSize * 2);
        parent.outUint8(opc1, 0);
        parent.outUint8(opc2, 1);
        DataGenerator gen(parent, _opcodeSize);
        return generate(printer, gen);
    }

    TestGenerator<Conf> &generate(
        Printer &printer, uint8_t opc1, uint8_t opc2, uint8_t opc3) {
        DataGenerator parent(_memory, _endian, _opcodeSize * 3);
        parent.outUint8(opc1, 0);
        parent.outUint8(opc2, 1);
        parent.outUint8(opc3, 2);
        DataGenerator gen(parent, _opcodeSize);
        return generate(printer, gen);
    }

    TestGenerator<Conf> &generate(
        Printer &printer, uint8_t opc1, uint8_t opc2, uint8_t opc3, uint8_t opc4) {
        DataGenerator parent(_memory, _endian, _opcodeSize * 4);
        parent.outUint8(opc1, 0);
        parent.outUint8(opc2, 1);
        parent.outUint8(opc3, 2);
        parent.outUint8(opc4, 3);
        DataGenerator gen(parent, _opcodeSize);
        return generate(printer, gen);
    }

private:
    Disassembler &_disassembler;
    const int _memorySize;
    const Endian _endian;
    const size_t _opcodeSize;
    uint8_t *_memory;
    TestData<Conf> _data;
    std::unordered_map<
        std::string,
        std::unordered_set<TokenizedText,
                          TokenizedText::hash,
                          TokenizedText::eq>
        > _map;

    typename Conf::uintptr_t _addr;
    Printer *_printer;

    TestGenerator<Conf> &generate(Printer &printer, DataGenerator &gen) {
        _printer = &printer;
        generateTests(gen);

        return *this;
    }

    void printInsn(const TestData<Conf> &data) {
        _printer->print(data.insn(), data.operands().buffer());
        _addr += data.insn().length();
    }

    int meaningfulTestData() {
        std::string name(_data.insn().name());
        const size_t size = _data.insn().length();
        name += ':';
        name += size + '0';
        auto seen = _map.find(name);
        if (seen == _map.end()) {
            _map.emplace(name, std::unordered_set<TokenizedText, TokenizedText::hash, TokenizedText::eq>());
            seen = _map.find(name);
        }
        auto &variants = seen->second;
        const TokenizedText a(_data.operands().buffer());
        auto found = variants.find(a);
        if (found != variants.end()) {
            if (found->count() >= 300)
                return 2;
            return 0;
        }
        variants.insert(a);
        return -2;
    }

    void generateTests(DataGenerator &gen) {
        gen.debugPrint("@@ enter", _memory);
        do {
            gen.next();
            gen.debugPrint("@@  loop", _memory);
            _data.tryGenerate(_disassembler, _addr, _memory, _memorySize);
            if (_disassembler.getError() == OK) {
                int size = _data.insn().length() - (gen.pos() + gen.size());
                if (size > 0) {
                    if (size % 2 == 0 || size == 1) {
                        DataGenerator child(gen, size);
                        child.debugPrint("@@  long", _memory);
                        generateTests(child);
                        gen.debugPrint("@@  cont", _memory);
                    } else {
                        DataGenerator child(gen, 1);
                        child.debugPrint("@@  odd1", _memory);
                        generateTests(child);
                        gen.debugPrint("@@  cont", _memory);
                    }
                } else if (size < 0) {
                    gen.debugPrint("@@  shrk", _memory);
                    printInsn(_data);
                    return;
                } else {
                    int abort;
                    if ((abort = meaningfulTestData()) < 0) {
                        printInsn(_data);
                    } else if (abort > 0) {
                        gen.debugPrint("@@ abort", _memory);
                        return;
                    }
                }
            }
        } while (gen.hasNext());
        gen.debugPrint("@@ leave", _memory);
    }
};

} // namespace test
} // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
