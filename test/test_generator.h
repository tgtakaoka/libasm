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

#include <ctype.h>
#include <stdio.h>

#include <unordered_map>
#include <unordered_set>

#include "dis_base.h"
#include "dis_memory.h"
#include "text_buffer.h"

namespace libasm {
namespace test {

class DataGenerator {
public:
    int start() const { return _start; }
    int length() const { return _start + _size; }
    virtual bool hasNext() const = 0;
    virtual void next() = 0;

    void outByte(uint8_t data, int off = 0) { _buffer[_start + off] = data; }

    void debug(FILE *out, const char *msg) const {
#if DEBUG_TRACE
        if (out)
            dump(out, msg, _start, _size);
#endif
    }

    void dump(FILE *out, const char *msg, int start, int size) const {
        fprintf(out, "%s %d/%d: ", msg, start, size);
        for (int pos = 0; pos < start; pos++)
            fprintf(out, " %02X", _buffer[pos]);
        for (int off = 0; off < size; off++)
            fprintf(out, "%c%02X", off == 0 ? '|' : ' ', _buffer[start + off]);
        fprintf(out, "\n");
        fflush(out);
    }

protected:
    DataGenerator(uint8_t *buffer, int bufSize, int start, int size)
        : _buffer(buffer),
          _bufSize(bufSize),
          _start(start),
          _size(size),
          _count(0) {}

    DataGenerator(DataGenerator &parent, int size)
        : _buffer(parent._buffer),
          _bufSize(parent._bufSize),
          _start(parent.length()),
          _size(size),
          _count(0) {}

    uint8_t *_buffer;
    const int _bufSize;
    const int _start;
    const int _size;
    int _count;

    void fill() {
        uint8_t data = _buffer[length() - 1] + 1;
        for (int pos = length(); pos < _bufSize; pos++) {
            _buffer[pos] = data++;
        }
    }
};

class ByteGenerator : public DataGenerator {
public:
    ByteGenerator(uint8_t *buffer, int bufSize, int start, int size = 1)
        : DataGenerator(buffer, bufSize, start, size), _off(0) {
        _data = initData();
    }

    ByteGenerator(DataGenerator &parent, int size)
        : DataGenerator(parent, size), _off(0) {
        _data = initData();
    }

    bool hasNext() const override { return _off < _size; }

    void next() override {
        _buffer[_start + _off] = _data;
        fill();
        _data++;
        _count++;
        if (_count < 0x100)
            return;
        _count = 0;
        _off++;
    }

private:
    int _off;
    uint8_t _data;

    uint8_t initData() const {
        return (_start > 0) ? _buffer[_start - 1] + 1 : 0;
    }
};

class WordGenerator : public DataGenerator {
public:
    WordGenerator(uint8_t *buffer, int bufSize, Endian endian, int start)
        : DataGenerator(buffer, bufSize, start, 2), _endian(endian) {
        _data = initData();
    }

    WordGenerator(DataGenerator &parent, Endian endian)
        : DataGenerator(parent, 2), _endian(endian) {
        _data = initData();
    }

    bool hasNext() const override { return _count < 0x10000; }

    void next() override {
        outWord(_data, 0);
        fill();
        _data++;
        _count++;
    }

    void outWord(uint16_t data, int off = 0) {
        int pos = _start + off;
        if (_endian == ENDIAN_BIG) {
            _buffer[pos++] = data >> 8;
            _buffer[pos] = data;
        } else {
            _buffer[pos++] = data;
            _buffer[pos] = data >> 8;
        }
    }

private:
    const Endian _endian;
    uint16_t _data;

    uint16_t initData() const {
        const uint8_t data = (_start > 0) ? _buffer[_start - 1] + 1 : 0;
        return (_endian == ENDIAN_BIG)
                       ? (static_cast<uint16_t>(data) << 8) + (data + 1)
                       : data + (static_cast<uint16_t>(data + 1) << 8);
    }
};

template <typename Conf>
class TestData : private DisMemory {
public:
    TestData() : DisMemory(0), _textBuffer(256) {}

    const Insn &insn() const { return _insn; }
    const char *name() const { return _insn.name(); }
    const uint8_t length() const { return _insn.length(); }
    const char *operands() const { return _textBuffer.buffer(); }
    Error tryGenerate(Disassembler &dis, typename Conf::uintptr_t addr,
            uint8_t *memory, int size) {
        resetAddress(addr);
        _memory = memory;
        _memorySize = size;
        _memoryIndex = 0;

        return dis.decode(*this, _insn, _textBuffer.buffer(), nullptr);
    }

private:
    TextBuffer _textBuffer;
    uint8_t *_memory;
    int _memorySize;
    int _memoryIndex;
    Insn _insn;

    // DisMemory
    bool hasNext() const override { return _memoryIndex < _memorySize; }
    uint8_t nextByte() override { return _memory[_memoryIndex++]; }
};

template <typename Conf>
class TestGenerator {
public:
    typedef typename Conf::opcode_t opcode_t;

    TestGenerator(Disassembler &disassembler, typename Conf::uintptr_t addr = 0)
        : _disassembler(disassembler),
          _memorySize(Conf::MAX_CODE),
          _endian(Conf::ENDIAN) {
        _memory = new uint8_t[_memorySize];
        _addr = addr;
    }

    virtual ~TestGenerator() { delete[] _memory; }

    class Printer {
    public:
        virtual void print(const Insn &insn, const char *operands) = 0;
        virtual void origin(typename Conf::uintptr_t addr) = 0;
        virtual FILE *dumpOut() = 0;
    };

    TestGenerator<Conf> &generate(Printer &printer) {
        FILE *dumpOut = printer.dumpOut();
        if (dumpOut)
            fprintf(dumpOut, "@@ generate:\n");
        if (_addr)
            printer.origin(_addr);
        if (sizeof(opcode_t) == 1) {
            ByteGenerator gen(_memory, _memorySize, 0);
            return generate(printer, gen);
        } else {
            WordGenerator gen(_memory, _memorySize, _endian, 0);
            return generate(printer, gen);
        }
    }

    TestGenerator<Conf> &generate(
            Printer &printer, uint8_t opc1, uint8_t opc2, uint8_t opc3) {
        FILE *dumpOut = printer.dumpOut();
        if (dumpOut)
            fprintf(dumpOut, "@@ generate: %#02x %#02x %#02x\n", opc1, opc2,
                    opc3);
        ByteGenerator parent(_memory, _memorySize, 0, 3);
        parent.outByte(opc1, 0);
        parent.outByte(opc2, 1);
        parent.outByte(opc3, 2);
        ByteGenerator gen(parent, 1);
        return generate(printer, gen);
    }

private:
    Disassembler &_disassembler;
    const int _memorySize;
    const Endian _endian;
    uint8_t *_memory;
    TestData<Conf> _data;
    std::unordered_map<std::string,
            std::unordered_set<TokenizedText, TokenizedText::hash,
                    TokenizedText::eq> >
            _map;

    typename Conf::uintptr_t _addr;
    Printer *_printer;

    TestGenerator<Conf> &generate(Printer &printer, DataGenerator &gen) {
        _printer = &printer;
        do {
            gen.next();
            ByteGenerator child(gen, 1);
            generateTests(child);
        } while (gen.hasNext());

        return *this;
    }

    void printInsn(const TestData<Conf> &data) {
        _printer->print(data.insn(), data.operands());
        _addr += data.length();
    }

    int meaningfulTestData() {
        std::string name(_data.name());
        const size_t size = _data.length();
        name += ':';
        name += size + '0';
        auto seen = _map.find(name);
        if (seen == _map.end()) {
            _map.emplace(
                    name, std::unordered_set<TokenizedText, TokenizedText::hash,
                                  TokenizedText::eq>());
            seen = _map.find(name);
        }
        auto &variants = seen->second;
        const TokenizedText a(_data.operands());
        auto found = variants.find(a);
        if (found != variants.end()) {
            if (found->count() >= 0x200)
                return -1;
            return 0;
        }
        variants.insert(a);
        return 1;
    }

    void generateTests(DataGenerator &gen) {
        FILE *dumpOut = _printer->dumpOut();
        gen.debug(dumpOut, "@@ enter");
        do {
            gen.next();
            gen.debug(dumpOut, "@@  loop");
            _data.tryGenerate(_disassembler, _addr, _memory, _memorySize);
            if (_disassembler.getError() == OK) {
                const int len = gen.length();
                const int newLen = _data.length();
                const int delta = newLen - len;
                if (delta && dumpOut) {
                    fprintf(dumpOut, "@@  delta: %d (%d <- %d)\n", delta,
                            newLen, len);
                    gen.dump(dumpOut, "@@   ", len, delta);
                    fprintf(dumpOut, "@@       : %s %s\n", _data.name(),
                            _data.operands());
                    fflush(dumpOut);
                }
                const int level = meaningfulTestData();
                if (level < 0 && gen.start() > 0)
                    return;
                if (level > 0)
                    printInsn(_data);
                if (delta > 0) {
                    ByteGenerator child(gen, delta);
                    child.debug(dumpOut, "@@ delta");
                    generateTests(child);
                } else if (delta < 0) {
                    gen.debug(dumpOut, "@@  shrk");
                    return;
                }
            }
        } while (gen.hasNext());
        gen.debug(dumpOut, "@@ leave");
    }
};

}  // namespace test
}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
