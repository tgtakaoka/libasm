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

#ifndef __LIBASM_TEST_GENERATOR_H__
#define __LIBASM_TEST_GENERATOR_H__

#include <cstdarg>
#include <string>
#include <unordered_map>

#include "array_memory.h"
#include "dis_base.h"
#include "dis_formatter.h"
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
    // |width| is how much of an instruction one step of the scan covers, which
    // is normally the CPU's opcode width but may be narrowed, see
    // TestGenerator::scanByByte.
    static DataGenerator *newGenerator(uint8_t *buffer, const ConfigBase &config,
            GenDebugger &debugger, OpCodeWidth width);
    DataGenerator *newChild();
    virtual ~DataGenerator();

    uint8_t start() const { return _start; }
    uint8_t length() const;
    OpCodeWidth width() const { return _width; }
    virtual bool hasNext() const = 0;
    void next();
    virtual void nextByte() {}
    virtual void outData(uint32_t) = 0;
    uint32_t data() const { return _data; }

    void debug(const char *fmt, ...) const;
    void dump(const char *fmt, ...) const;

protected:
    uint8_t *const _buffer;
    const OpCodeWidth _width;
    const Endian _endian;
    const uint8_t _start;
    GenDebugger &_debugger;
    uint64_t _count;
    uint32_t _data;

    void genData();
    void dump(const char *msg, int start, int size, va_list args) const;

    DataGenerator(
            uint8_t *buffer, const ConfigBase &config, GenDebugger &debugger, OpCodeWidth width);
    DataGenerator(DataGenerator &parent);
};

struct TestGenerator {
    struct Formatter : public GenDebugger {
        virtual driver::DisFormatter &formatter() = 0;
        virtual void printList() = 0;
        virtual void flush() = 0;
        virtual void setOrigin(uint32_t addr) = 0;
    };

    // Each arch supplies its own tokenizer list, built from
    // standardTokenizers<HexPolicy>(dis.curSym(), {index tokenizers}).
    TestGenerator(Formatter &formatter, Disassembler &disassembler, uint32_t addr,
            TokenizerList tokenizers);
    virtual ~TestGenerator();

    TestGenerator &generate();
    TestGenerator &generate(uint16_t opc1);
    TestGenerator &generate(uint16_t opc1, uint16_t opc2);
    TestGenerator &ignoreSizeVariation();
    // Walk the opcode space a byte at a time rather than a whole opcode at a
    // time.  A CPU whose instruction is one wide word never asks for more bytes
    // while decoding, so it never spawns a child generator, and the drop
    // heuristic which abandons an exhausted subtree never runs -- the scan then
    // has to visit every one of the 2^32 encodings.  Scanning by byte builds
    // the same child chain a narrower CPU gets, which lets whole subtrees be
    // dropped once an opcode has shown all the operand variety it has.
    TestGenerator &scanByByte();

private:
    Formatter &_formatter;
    Disassembler &_disassembler;
    driver::DisFormatter &_disFormatter;
    const TokenizerList _tokenizers;
    const OpCodeWidth _opCodeWidth;
    OpCodeWidth _scanWidth;
    const Endian _endian;
    const AddressUnit _addressUnit;
    const int _codeMax;
    uint8_t *const _memory;
    uint32_t _address;
    bool _ignoreSizeVariation;

    std::unordered_map<std::string, TokenizedText::Set> _map;
    std::unordered_map<std::string, TokenizedText::Set> _error;

    void printInsn(const libasm::driver::DisFormatter &data);
    // Disassemble the bytes in _memory at _address into _disFormatter.insn().
    // mark_P==nullptr: decode a fresh instruction (reads the opcode, resets state);
    // mark_P!=nullptr: decode the continuation half (reuses the opcode bytes).
    Error disasm(const ArrayMemory &memory, const char *mark_P = nullptr);
    // Dedup key = namePrefix + insn.name() [+ ":size"], and namePrefix's operand
    // counterpart. For a continuation the caller passes the first half as the
    // prefix and leaves the second half in _disFormatter.
    const TokenizedText *_meaningfulTestData(std::string &name, int size,
            const std::string &namePrefix, const std::string &oprPrefix);
    const TokenizedText *meaningfulTestData(std::string &name, int size,
            const std::string &namePrefix = "", const std::string &oprPrefix = "");
    const TokenizedText *meaningfulError(std::string &name);
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
