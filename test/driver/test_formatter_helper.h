/*
 * Copyright 2023 Tadashi G. Takaoka
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

#ifndef __LIBASM_TEST_FORMATTER_HELPER_H__
#define __LIBASM_TEST_FORMATTER_HELPER_H__

#include <map>
#include <string>
#include "asm_directive.h"
#include "asm_driver.h"
#include "dis_driver.h"
#include "stored_printer.h"
#include "test_driver_helper.h"
#include "test_memory.h"
#include "test_reader.h"
#include "test_sources.h"

namespace libasm {
namespace driver {
namespace test {
struct Options {
    void set(const char *name, const char *value) { _opts.emplace(name, value); }
    void set(AsmDriver &driver) const {
        for (auto &it : _opts)
            driver.setOption(it.first.c_str(), it.second.c_str());
    };

private:
    std::map<std::string, std::string> _opts;
};
}  // namespace test
}  // namespace driver
}  // namespace libasm

#define PREP_ASM(type_of_assembler, type_of_directive) \
    type_of_assembler assembler;                       \
    type_of_directive directive{assembler};            \
    AsmDriver driver{&directive};                      \
    TestSources sources;                               \
    Options options

#define ASM(_cpu, _source, _expected)                                                   \
    do {                                                                                \
        TestReader expected("expected");                                                \
        expected.add(_expected);                                                        \
        TestReader source(_cpu);                                                        \
        sources.add(source.add(_source));                                               \
        BinMemory memory, prev;                                                         \
        SymbolStoreImpl symbols;                                                        \
        StoredPrinter list, error;                                                      \
        bool reportError = false;                                                       \
        do {                                                                            \
            sources.open(source.name().c_str());                                        \
            prev.swap(memory);                                                          \
            symbols.copy(driver.symbols());                                             \
            memory.clear();                                                             \
            list.clear();                                                               \
            error.clear();                                                              \
            driver.reset();                                                             \
            options.set(driver);                                                        \
            driver.setCpu(_cpu);                                                        \
            driver.assemble(sources, memory, list, error, reportError);                 \
            reportError = true;                                                         \
        } while (error.size() == 0 && (memory != prev || symbols != driver.symbols())); \
        for (size_t lineno = 1; lineno <= list.size(); lineno++)                        \
            EQ("line", expected.readLine(), list.line(lineno));                         \
        EQ("line eor", nullptr, expected.readLine());                                   \
    } while (0)

#define PREP_DIS(typeof_disassembler) \
    typeof_disassembler disassembler; \
    DisDriver driver {                \
        &disassembler                 \
    }

#define _DIS(_cpu, _contents, _lines, _memory)                    \
    do {                                                          \
        driver.setCpu(_cpu);                                      \
        TestReader contents(_cpu);                                \
        contents.add(_contents);                                  \
        TestReader lines(_cpu);                                   \
        lines.add(_lines);                                        \
        StoredPrinter out, list, err;                             \
        driver.disassemble(_memory, "test.bin", out, list, err);  \
        for (size_t lineno = 1; lineno <= out.size(); lineno++)   \
            EQ("content", contents.readLine(), out.line(lineno)); \
        EQ("expected content eor", nullptr, contents.readLine()); \
        for (size_t lineno = 1; lineno <= list.size(); lineno++)  \
            EQ("content", lines.readLine(), list.line(lineno));   \
        EQ("expected line eor", nullptr, lines.readLine());       \
    } while (0)

#define DIS16(_cpu, _org, _contents, _expected, ...)      \
    do {                                                  \
        const auto unit = disassembler.addressUnit();     \
        const auto endian = disassembler.endian();        \
        TestMemory memory;                                \
        auto writer = memory.writer(_org * unit, endian); \
        const uint16_t _memory[] = {__VA_ARGS__};         \
        for (auto byte : _memory)                         \
            writer.add(byte);                             \
        _DIS(_cpu, _contents, _expected, memory);         \
    } while (0)

#define DIS8(_cpu, _org, _contents, _expected, ...) \
    do {                                            \
        TestMemory memory;                          \
        auto writer = memory.writer(_org);          \
        const uint8_t _memory[] = {__VA_ARGS__};    \
        for (auto byte : _memory)                   \
            writer.add(byte);                       \
        _DIS(_cpu, _contents, _expected, memory);   \
    } while (0)

#endif  // __TEST_FORMATTER_HELPER_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
