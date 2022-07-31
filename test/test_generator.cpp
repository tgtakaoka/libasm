/*
 * Copyright 2022 Tadashi G. Takaoka
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

#include "test_generator.h"

#include "array_memory.h"
#include "dis_base.h"
#include "tokenized_text.h"

#include <cctype>
#include <cstdio>
#include <unordered_map>
#include <unordered_set>

namespace libasm {
namespace gen {

void DataGenerator::debug(const char *msg) const {
#if DEBUG_TRACE
    dump(msg, _start, _size);
#endif
}

void DataGenerator::dump(const char *msg, int start, int size) const {
    _debugger.info("%s %d/%d: ", msg, start, size);
    for (int pos = 0; pos < start; pos++)
        _debugger.info(" %02X", _buffer[pos]);
    for (int off = 0; off < size; off++)
        _debugger.info("%c%02X", off == 0 ? '|' : ' ', _buffer[start + off]);
    _debugger.info("\n");
}

DataGenerator::DataGenerator(
        uint8_t *buffer, int bufSize, int start, int size, GenDebugger &debugger)
    : _buffer(buffer),
      _bufSize(bufSize),
      _start(start),
      _size(size),
      _count(0),
      _debugger(debugger) {}

DataGenerator::DataGenerator(DataGenerator &parent, int size)
    : _buffer(parent._buffer),
      _bufSize(parent._bufSize),
      _start(parent.length()),
      _size(size),
      _count(0),
      _debugger(parent._debugger) {}

void DataGenerator::fill() {
    uint8_t data = _buffer[length() - 1] + 1;
    for (int pos = length(); pos < _bufSize; pos++) {
        _buffer[pos] = data++;
    }
}

class ByteGenerator : public DataGenerator {
public:
    ByteGenerator(uint8_t *buffer, int bufSize, int start, int size, GenDebugger &debugger)
        : DataGenerator(buffer, bufSize, start, size, debugger), _off(0) {
        _data = initData();
    }

    ByteGenerator(DataGenerator &parent, int size = 1) : DataGenerator(parent, size), _off(0) {
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

    uint8_t initData() const { return (_start > 0) ? _buffer[_start - 1] + 1 : 0; }
};

class WordGenerator : public DataGenerator {
public:
    WordGenerator(uint8_t *buffer, int bufSize, Endian endian, int start, GenDebugger &debugger)
        : DataGenerator(buffer, bufSize, start, 2, debugger), _endian(endian) {
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
        const int hi = int(_endian);
        const int lo = 1 - hi;
        const int pos = _start + off;
        _buffer[pos + hi] = data >> 8;
        _buffer[pos + lo] = data;
    }

private:
    const Endian _endian;
    uint16_t _data;

    uint16_t initData() const {
        const uint8_t data = (_start > 0) ? _buffer[_start - 1] + 1 : 0;
        const uint8_t hi = uint8_t(_endian);
        const uint8_t lo = 1 - hi;
        return (static_cast<uint16_t>(data + hi) << 8) | (data + lo);
    }
};

TestGenerator::TestGenerator(Formatter &formatter, Disassembler &disassembler, uint32_t addr)
    : _formatter(formatter),
      _disassembler(disassembler),
      _listing(formatter.listing()),
      _opCodeWidth(disassembler.config().opCodeWidth()),
      _memorySize(disassembler.config().codeMax()),
      _endian(disassembler.config().endian()),
      _addr_unit(disassembler.config().addressUnit()),
      _mem_addr(addr * _addr_unit),
      _memory(new uint8_t[_memorySize]) {}

TestGenerator::~TestGenerator() {
    delete[] _memory;
}

TestGenerator &TestGenerator::generate() {
    _formatter.info("@@ generate:\n");
    if (_mem_addr) {
        const auto addr = _mem_addr / _addr_unit;
        _formatter.setOrigin(addr);
    }
    if (_opCodeWidth == OPCODE_16BIT) {
        WordGenerator gen(_memory, _memorySize, _endian, 0, _formatter);
        return generate(gen);
    }
    ByteGenerator gen(_memory, _memorySize, 0, 1, _formatter);
    return generate(gen);
}

TestGenerator &TestGenerator::generate(uint8_t opc1) {
    _formatter.info("@@ generate: %#02xx\n", opc1);
    ByteGenerator parent(_memory, _memorySize, 0, 1, _formatter);
    parent.outByte(opc1, 0);
    ByteGenerator gen(parent, 1);
    return generate(gen);
}

TestGenerator &TestGenerator::generate(uint8_t opc1, uint8_t opc2, uint8_t opc3) {
    _formatter.info("@@ generate: %#02x %#02x %#02x\n", opc1, opc2, opc3);
    ByteGenerator parent(_memory, _memorySize, 0, 3, _formatter);
    parent.outByte(opc1, 0);
    parent.outByte(opc2, 1);
    parent.outByte(opc3, 2);
    ByteGenerator gen(parent);
    return generate(gen);
}

TestGenerator &TestGenerator::generate(DataGenerator &gen) {
    do {
        gen.next();
        ByteGenerator child(gen);
        generateTests(child);
    } while (gen.hasNext());

    return *this;
}

int TestGenerator::meaningfulTestData() {
    std::string name(_listing.name());
    const size_t size = _listing.length();
    name += ':';
    name += size + '0';
    auto seen = _map.find(name);
    if (seen == _map.end()) {
        _map.emplace(
                name, std::unordered_set<TokenizedText, TokenizedText::hash, TokenizedText::eq>());
        seen = _map.find(name);
    }
    auto &variants = seen->second;
    const TokenizedText a(_listing.operand());
    auto found = variants.find(a);
    if (found != variants.end()) {
        if (found->count() >= 0x4000)
            return -1;
        return 0;
    }
    variants.insert(a);
    return 1;
}

void TestGenerator::generateTests(DataGenerator &gen) {
    gen.debug("@@ enter");
    do {
        gen.next();
        gen.debug("@@  loop");
        const ArrayMemory memory(_mem_addr, _memory, _memorySize);
        auto it = memory.iterator();
        _listing.disassemble(it, _mem_addr / _addr_unit);
        if (_disassembler.isOK()) {
            const int len = gen.length();
            const int newLen = _listing.length();
            const int delta = newLen - len;
            if (delta) {
                _formatter.info("@@  delta: %d (%d <- %d)\n", delta, newLen, len);
                gen.dump("@@   ", len, delta);
                _formatter.info("@@       : %s %s\n", _listing.name(), _listing.operand());
            }
            const int level = meaningfulTestData();
            if (level < 0 && gen.start() > 0)
                return;
            if (level > 0) {
                _formatter.printList();
                _mem_addr += newLen;
            }
            if (delta > 0) {
                ByteGenerator child(gen, delta);
                child.debug("@@ delta");
                generateTests(child);
            } else if (delta < 0) {
                gen.debug("@@  shrk");
                return;
            }
        }
    } while (gen.hasNext());
    gen.debug("@@ leave");
}

}  // namespace gen
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
