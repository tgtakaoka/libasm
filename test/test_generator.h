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

#include <cstdarg>
#include <string>
#include <unordered_map>

#include "dis_base.h"
#include "gen_formatter.h"
#include "tokenized_text.h"

namespace libasm {
namespace gen {

struct GenDebugger {
    virtual void info(const char *fmt, va_list args) = 0;
    void info(const char *fmt, ...) {
        va_list args;
        va_start(args, fmt);
        info(fmt, args);
        va_end(args);
    }
};

struct DataGenerator {
    static DataGenerator *newGenerator(
            uint8_t *buffer, const ConfigBase &config, GenDebugger &debugger);
    DataGenerator *newChild();
    virtual ~DataGenerator();

    uint8_t start() const { return _start; }
    uint8_t length() const;
    OpCodeWidth width() const { return _width; }
    virtual bool hasNext() const = 0;
    void next();
    virtual void nextByte() {}
    virtual void outData(uint16_t) = 0;
    uint16_t data() const { return _data; }

    void debug(const char *fmt, ...) const;
    void dump(const char *fmt, ...) const;

protected:
    uint8_t *const _buffer;
    const OpCodeWidth _width;
    const Endian _endian;
    const uint8_t _start;
    GenDebugger &_debugger;
    uint32_t _count;
    uint16_t _data;

    void genData();
    void dump(const char *msg, int start, int size, va_list args) const;

    DataGenerator(uint8_t *buffer, const ConfigBase &config, GenDebugger &debugger);
    DataGenerator(DataGenerator &parent);
};

struct TestGenerator {
    struct Formatter : public GenDebugger {
        virtual GenFormatter &listing() = 0;
        virtual void printList() = 0;
        virtual void flush() = 0;
        virtual void setOrigin(uint32_t addr) = 0;
    };

    TestGenerator(Formatter &formatter, Disassembler &disassembler, uint32_t addr = 0);
    virtual ~TestGenerator();

    TestGenerator &generate();
    TestGenerator &ignoreSizeVariation();

private:
    Formatter &_formatter;
    Disassembler &_disassembler;
    GenFormatter &_listing;
    const OpCodeWidth _opCodeWidth;
    const Endian _endian;
    const AddressUnit _addressUnit;
    const int _codeMax;
    uint8_t *const _memory;
    uint32_t _address;
    bool _ignoreSizeVariation;

    std::unordered_map<std::string, TokenizedText::Set> _map;
    std::unordered_map<std::string, TokenizedText::Set> _error;

    void printInsn(const libasm::driver::DisFormatter &data);
    const TokenizedText &meaningfulTestData(std::string &name, bool withSize = true);
    const TokenizedText &meaningfulError(std::string &name);
    static constexpr uint8_t MAX_DROP_BYTE = 22;
    static uint8_t calcDrop(const TokenizedText &text);
    uint8_t generateTests(DataGenerator &gen, const bool root);
    void dump() const;
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
