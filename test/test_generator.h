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
#include "gen_formatter.h"
#include "tokenized_text.h"

#include <string>
#include <unordered_map>
#include <unordered_set>

namespace libasm {
namespace gen {

struct GenDebugger {
    virtual void info(const char *fmt, ...) = 0;
};

class DataGenerator {
public:
    int start() const { return _start; }
    int length() const { return _start + _size; }
    virtual bool hasNext() const = 0;
    virtual void next() = 0;

    void outByte(uint8_t data, int off = 0) { _buffer[_start + off] = data; }

    void debug(const char *msg) const;
    void dump(const char *msg, int start, int size) const;

protected:
    uint8_t *_buffer;
    const int _bufSize;
    const int _start;
    const int _size;
    int _count;
    GenDebugger &_debugger;

    DataGenerator(uint8_t *buffer, int bufSize, int start, int size, GenDebugger &debugger);
    DataGenerator(DataGenerator &parent, int size);
    void fill();
};

class TestGenerator {
public:
    struct Formatter : public GenDebugger {
        virtual GenFormatter &listing() = 0;
        virtual void printList() = 0;
        virtual void setOrigin(uint32_t addr) = 0;
    };

    TestGenerator(Formatter &formatter, Disassembler &disassembler, uint32_t addr = 0);
    virtual ~TestGenerator();

    TestGenerator &generate();
    TestGenerator &generate(uint8_t opc1);
    TestGenerator &generate(uint8_t opc1, uint8_t opc2, uint8_t opc3);

private:
    Formatter &_formatter;
    Disassembler &_disassembler;
    GenFormatter &_listing;
    const OpCodeWidth _opCodeWidth;
    const int _memorySize;
    const Endian _endian;
    const uint8_t _addrUnit;
    uint32_t _addr;
    uint8_t *_memory;
    std::unordered_map<std::string,
            std::unordered_set<TokenizedText, TokenizedText::hash, TokenizedText::eq> >
            _map;

    TestGenerator &generate(DataGenerator &gen);
    void printInsn(const libasm::driver::DisFormatter &data);
    int meaningfulTestData();
    void generateTests(DataGenerator &gen);
};

}  // namespace gen
}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
