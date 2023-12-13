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

#include <cctype>
#include <cstdio>

#include "array_memory.h"
#include "dis_base.h"
#include "tokenized_text.h"

namespace libasm {
namespace gen {

void DataGenerator::debug(const char *fmt, ...) const {
#if DEBUG_TRACE
    va_list args;
    va_start(args, fmt);
    _debugger.info(fmt, args);
    va_end(args);
#endif
}

void DataGenerator::dump(const char *fmt, ...) const {
#if DEBUG_TRACE
    va_list args;
    va_start(args, fmt);
    dump(fmt, _start, length() - _start, args);
    va_end(args);
#endif
}

void DataGenerator::dump(const char *fmt, int start, int size, va_list args) const {
    _debugger.info(fmt, args);
    for (int pos = 0; pos < start + size; pos++) {
        const auto sep = pos == start ? '|' : ' ';
        _debugger.info("%c%02X", sep, _buffer[pos]);
    }
    _debugger.info("\n");
}

DataGenerator::DataGenerator(uint8_t *buffer, const ConfigBase &config, GenDebugger &debugger)
    : _buffer(buffer),
      _width(config.opCodeWidth()),
      _endian(config.endian()),
      _start(0),
      _debugger(debugger),
      _count(0) {}

DataGenerator::DataGenerator(DataGenerator &parent)
    : _buffer(parent._buffer),
      _width(parent._width),
      _endian(parent._endian),
      _start(parent.length()),
      _debugger(parent._debugger),
      _count(0) {}

uint8_t DataGenerator::length() const {
    if (_width == OPCODE_8BIT)
        return _start + 1;
    if (_width == OPCODE_16BIT)
        return _start + 2;
    if (_width == OPCODE_12BIT)
        return _start + 2;
    return 100;
}

void DataGenerator::next() {
    genData();
    _data++;
    _count++;
}

void DataGenerator::genData() {
    outData(_data);
}

DataGenerator::~DataGenerator() {}

struct ByteGenerator : DataGenerator {
    ByteGenerator(uint8_t *buffer, const ConfigBase &config, GenDebugger &debugger)
        : DataGenerator(buffer, config, debugger) {
        _data = 0;
    }

    ByteGenerator(DataGenerator &parent) : DataGenerator(parent) { _data = parent.data(); }

    bool hasNext() const override { return _count < 0x100; }

    void outData(uint16_t data) override { _buffer[_start] = data; }
};

struct WordGenerator : DataGenerator {
    WordGenerator(uint8_t *buffer, const ConfigBase &config, GenDebugger &debugger)
        : DataGenerator(buffer, config, debugger) {
        _data = 0;
    }

    WordGenerator(DataGenerator &parent) : DataGenerator(parent) { _data = parent.data() + 0x100; }

    bool hasNext() const override { return _count < 0x10000; }

    void nextByte() override {
        _data += 0x101;
        _count += 0x101;
    }

    void outData(uint16_t data) override {
        if (_endian == ENDIAN_BIG) {
            _buffer[_start + 0] = data >> 8;
            _buffer[_start + 1] = data;
        } else {
            _buffer[_start + 0] = data;
            _buffer[_start + 1] = data >> 8;
        }
    }
};

DataGenerator *DataGenerator::newGenerator(
        uint8_t *buffer, const ConfigBase &config, GenDebugger &debugger) {
    const auto width = config.opCodeWidth();
    if (width == OPCODE_8BIT)
        return new ByteGenerator(buffer, config, debugger);
    if (width == OPCODE_16BIT)
        return new WordGenerator(buffer, config, debugger);
    if (width == OPCODE_12BIT)
        return new WordGenerator(buffer, config, debugger);
    return nullptr;
}

DataGenerator *DataGenerator::newChild() {
    if (_width == OPCODE_8BIT)
        return new ByteGenerator(*this);
    if (_width == OPCODE_16BIT)
        return new WordGenerator(*this);
    if (_width == OPCODE_12BIT)
        return new WordGenerator(*this);
    return nullptr;
}

TestGenerator::TestGenerator(Formatter &formatter, Disassembler &disassembler, uint32_t addr)
    : _formatter(formatter),
      _disassembler(disassembler),
      _disFormatter(formatter.formatter()),
      _opCodeWidth(disassembler.config().opCodeWidth()),
      _endian(disassembler.config().endian()),
      _addressUnit(disassembler.config().addressUnit()),
      _codeMax(disassembler.config().codeMax()),
      _memory(new uint8_t[_codeMax]),
      _address(addr * _addressUnit),
      _ignoreSizeVariation(false) {}

TestGenerator::~TestGenerator() {
    delete[] _memory;
}

TestGenerator &TestGenerator::ignoreSizeVariation() {
    _ignoreSizeVariation = true;
    return *this;
}

TestGenerator &TestGenerator::generate() {
    if (_address) {
        const auto addr = _address / _addressUnit;
        _formatter.setOrigin(addr);
    }
    auto *gen = DataGenerator::newGenerator(_memory, _disassembler.config(), _formatter);
    gen->dump("@@ generate:");
    generateTests(*gen, true);
    delete gen;
    dump();
    return *this;
}

TestGenerator &TestGenerator::generate(uint16_t opc1) {
    if (_address) {
        const auto addr = _address / _addressUnit;
        _formatter.setOrigin(addr);
    }
    auto *parent = DataGenerator::newGenerator(_memory, _disassembler.config(), _formatter);
    parent->outData(opc1);
    auto *gen = parent->newChild();
    gen->dump("@@ generate:");
    generateTests(*gen, true);
    delete gen;
    delete parent;
    dump();
    return *this;
}

TestGenerator &TestGenerator::generate(uint16_t opc1, uint16_t opc2) {
    if (_address) {
        const auto addr = _address / _addressUnit;
        _formatter.setOrigin(addr);
    }
    auto *parent = DataGenerator::newGenerator(_memory, _disassembler.config(), _formatter);
    parent->outData(opc1);
    auto *parent2 = parent->newChild();
    parent2->outData(opc2);
    auto *gen = parent2->newChild();
    gen->dump("@@ generate:");
    generateTests(*gen, true);
    delete gen;
    delete parent2;
    delete parent;
    dump();
    return *this;
}

void TestGenerator::dump() const {
    _formatter.info("@@ ===== dump =====\n");
    for (auto it = _map.begin(); it != _map.end(); it++) {
        const auto &name = it->first;
        for (auto t = it->second.begin(); t != it->second.end(); t++) {
            const auto &opr = t->tokens();
            _formatter.info("%10u %s %s\n", t->count(), name.c_str(), opr.c_str());
        }
    }
}

const TokenizedText &TestGenerator::meaningfulTestData(std::string &name, bool withSize) {
    const auto &insn = _disFormatter.insn();
    name = insn.name();
    if (withSize) {
        const auto size = insn.length();
        name += ':';
        name += size + '0';
    }
    auto seen = _map.find(name);
    if (seen == _map.end())
        seen = _map.emplace(name, TokenizedText::Set()).first;
    auto &oprVariants = seen->second;
    const TokenizedText opr{_disFormatter.operands().str()};
    auto found = oprVariants.find(opr);
    if (found == oprVariants.end())
        found = oprVariants.insert(opr).first;
    found->increment();
    return *found;
}

const TokenizedText &TestGenerator::meaningfulError(std::string &name) {
    const auto &insn = _disFormatter.insn();
    name = insn.name();
    const auto size = insn.length();
    name += ':';
    name += size + '0';
    auto seen = _error.find(name);
    if (seen == _error.end())
        seen = _error.emplace(name, TokenizedText::Set()).first;
    auto &oprVariants = seen->second;
    const TokenizedText opr{_disFormatter.operands().str()};
    auto found = oprVariants.find(opr);
    if (found == oprVariants.end())
        found = oprVariants.insert(opr).first;
    found->increment();
    return *found;
}

uint8_t TestGenerator::calcDrop(const TokenizedText &text) {
    for (int8_t drop = MAX_DROP_BYTE; drop >= 0; drop--) {
        if (text.count() >= (1U << 16) + (1U << 8) * drop)
            return drop + 2;
    }
    return 1;
}

uint8_t TestGenerator::generateTests(DataGenerator &gen, const bool root) {
    do {
        gen.next();
        const ArrayMemory memory(_address, _memory, gen.length());
        auto it = memory.iterator();
        _disFormatter.reset();
        auto &insn = _disFormatter.insn();
        auto &operands = _disFormatter.operands();
        insn.reset(_address / _addressUnit);
        const auto error = _disassembler.decode(it, insn, operands.mark(), operands.capacity());
        _disFormatter.set(insn);
        if (insn.isOK()) {
            const int len = gen.length();
            const int newLen = _disFormatter.insn().length();
            std::string name;
            const auto &found = meaningfulTestData(name);
            if (found.count() == 1) {
                // Found the first occurence of name/size/operand combination
                if (_ignoreSizeVariation) {
                    std::string mnemonic;
                    const auto &uniq = meaningfulTestData(mnemonic, false);
                    if (uniq.count() == 1) {
                        _formatter.printList();
                        _address += newLen;
                    }
                } else {
                    _formatter.printList();
                    _address += newLen;
                }
            }
            if (newLen < len && !root) {
                // Byte/word sequence is correct instruction but there are unnecessary bytes. Drop
                // these.
                gen.dump("@@ shrink length=%d count=%#x: %s %s", newLen, found.count(),
                        name.c_str(), found.tokens().c_str());
                return len - newLen;
            }
            // This iteration is the first byte/word of instruction or doesn't iterate over all
            // possible operand combination yet. Try next sequence.
            if (root || found.count() < (1U << 16))
                continue;
            // This byte/word seems unnecessary to iterate remaining. Drop some last bytes/words.
            const auto drop = calcDrop(found);
            gen.dump("@@ break drop=%d count=%#x: %s %s", drop, found.count(), name.c_str(),
                    found.tokens().c_str());
            return drop;
        } else if (error == NO_MEMORY) {
            // Byte/word sequence seems a correct instruction but needs more bytes/words.
            auto *child = gen.newChild();
            const auto drop = generateTests(*child, false);
            _formatter.flush();
            // This iteration is the first byte/word of instruction or a child iterattion just
            // finishes without any hiccup, continue current iteration.
            if (root || drop == 0) {
                delete child;
                continue;
            }
            delete child;
            // Here a byte/word is dropped.
            if (_opCodeWidth == OPCODE_16BIT || _opCodeWidth == OPCODE_12BIT) {
                // further words needs to be droppped.
                if (drop >= 4) {
                    gen.dump("@@ drop=%d", drop);
                    return drop - 2;
                } else if (drop >= 3) {
                    gen.dump("@@ drop=%d", drop);
                    gen.nextByte();
                }
            } else if (_opCodeWidth == OPCODE_8BIT) {
                // further bytes needs to be dropped.
                if (drop >= 2) {
                    gen.dump("@@ drop=%d", drop);
                    return drop - 1;
                }
            }
        } else if (error != UNKNOWN_INSTRUCTION && error != OPERAND_NOT_ALIGNED) {
            std::string name;
            const auto &error = meaningfulError(name);
            if (error.count() == 1)
                gen.dump("@@ error: %s %s: %s", name.c_str(), error.tokens().c_str(),
                        insn.errorText_P());
            if (root || error.count() < (1U << 16))
                continue;
            const auto drop = calcDrop(error);
            gen.dump("@@ break drop=%d error=%#x: %s %s: %s", drop, error.count(), name.c_str(),
                    error.tokens().c_str(), insn.errorText_P());
            return drop;
        }
    } while (gen.hasNext());
    _formatter.flush();
    return 0;
}

}  // namespace gen
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
