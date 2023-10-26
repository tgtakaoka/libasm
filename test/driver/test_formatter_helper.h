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

#ifndef __TEST_FORMATTER_HELPER_H__
#define __TEST_FORMATTER_HELPER_H__

#include "array_memory.h"
#include "asm_directive.h"
#include "asm_driver.h"
#include "asm_formatter.h"
#include "bin_memory.h"
#include "dis_driver.h"
#include "dis_formatter.h"
#include "test_driver_helper.h"
#include "test_reader.h"
#include "test_sources.h"

struct AsmFormatterHelper {
    AsmFormatterHelper() : upperHex(true), lineNumber(false) {}
    void setUpperHex(bool enable) { upperHex = enable; }
    void setLineNumber(bool enable) { lineNumber = enable; }
    bool upperHex;
    bool lineNumber;
};

#define PREP_ASM_DRIVER(reportDuplicate, ...)                          \
    AsmDirective *dirs[] = {__VA_ARGS__};                              \
    TestSources sources;                                               \
    AsmDriver driver(&dirs[0], &dirs[sizeof(dirs) / sizeof(dirs[0])]); \
    char buffer[256];                                                  \
    StrBuffer out(buffer, sizeof(buffer));                             \
    const bool reportError = !reportDuplicate;                         \
    AsmFormatterHelper formatter

#define PREP_ASM_SYMBOL(typeof_asm, typeof_directive) \
    typeof_asm assembler;                             \
    typeof_directive directive{assembler};            \
    PREP_ASM_DRIVER(true, &directive)

#define PREP_ASM(typeof_asm, typeof_directive) \
    typeof_asm assembler;                      \
    typeof_directive directive{assembler};     \
    PREP_ASM_DRIVER(false, &directive)

#define ASM(_cpu, _source, _expected)                                       \
    do {                                                                    \
        uint32_t origin = 0;                                                \
        TestReader expected("expected");                                    \
        expected.add(_expected);                                            \
        TestReader source(_cpu);                                            \
        source.add(_source);                                                \
        sources.add(source);                                                \
        sources.open(source.name().c_str());                                \
        AsmFormatter fmt{sources};                                          \
        fmt.setUpperHex(formatter.upperHex);                                \
        fmt.setLineNumber(formatter.lineNumber);                            \
        StrScanner *line;                                                   \
        while ((line = sources.readLine()) != nullptr) {                    \
            auto &directive = *driver.current();                            \
            auto &insn = fmt.insn();                                        \
            insn.reset(origin);                                             \
            AsmDirective::Context context{sources, reportError};            \
            auto scan = *line;                                              \
            directive.encode(scan, insn, context, driver);                  \
            const auto &config = directive.assembler().config();            \
            origin = insn.address() + insn.length() / config.addressUnit(); \
            if (insn.length() == 0)                                         \
                origin = directive.assembler().currentLocation();           \
            fmt.set(*line, directive, config, &context.value);              \
            fmt.setListRadix(driver.current()->assembler().listRadix());    \
            while (fmt.hasNextLine())                                       \
                EQ("line", expected.readLine(), fmt.getLine(out).str());    \
        }                                                                   \
        EQ("line eor", nullptr, expected.readLine());                       \
    } while (0)

#define PREP_DIS(typeof_disassembler)      \
    typeof_disassembler disassembler;      \
    Disassembler *dis = &disassembler;     \
    DisDriver driver(&dis, &dis + 1);      \
    char buffer[256];                      \
    StrBuffer out(buffer, sizeof(buffer)); \
    DisFormatter formatter(disassembler, "test.bin")

#define DIS(_cpu, _org, _contents, _lines, _memory)                                  \
    do {                                                                             \
        TestReader contents(_cpu);                                                   \
        contents.add(_contents);                                                     \
        TestReader lines(_cpu);                                                      \
        lines.add(_lines);                                                           \
        const auto unit = disassembler.addressUnit();                                \
        auto reader = _memory.iterator();                                            \
        TRUE("cpu", disassembler.setCpu(_cpu));                                      \
        formatter.setCpu(_cpu);                                                      \
        EQ("cpu content", contents.readLine(), formatter.getContent(out).str());     \
        EQ("cpu line", lines.readLine(), formatter.getLine(out).str());              \
        formatter.setOrigin(_org);                                                   \
        EQ("org content", contents.readLine(), formatter.getContent(out).str());     \
        EQ("org line", lines.readLine(), formatter.getLine(out).str());              \
        while (reader.hasNext()) {                                                   \
            formatter.reset();                                                       \
            auto &opr = formatter.operands();                                        \
            auto &insn = formatter.insn();                                           \
            insn.reset(reader.address() / unit);                                     \
            disassembler.decode(reader, insn, opr.mark(), opr.capacity());           \
            formatter.set(insn);                                                     \
            while (formatter.hasNextContent())                                       \
                EQ("content", contents.readLine(), formatter.getContent(out).str()); \
            while (formatter.hasNextLine())                                          \
                EQ("line", lines.readLine(), formatter.getLine(out).str());          \
            FALSE("line eor", formatter.hasNextLine());                              \
        }                                                                            \
        EQ("expected content eor", nullptr, contents.readLine());                    \
        EQ("expected line eor", nullptr, lines.readLine());                          \
    } while (0)

#define DIS16(_cpu, _org, _contents, _expected, ...)                            \
    do {                                                                        \
        const auto unit = disassembler.addressUnit();                           \
        const auto endian = disassembler.endian();                              \
        const uint16_t _memory[] = {__VA_ARGS__};                               \
        const ArrayMemory memory(_org *unit, _memory, sizeof(_memory), endian); \
        DIS(_cpu, _org, _contents, _expected, memory);                          \
    } while (0)

#define DIS8(_cpu, _org, _contents, _expected, ...)               \
    do {                                                          \
        const uint8_t _memory[] = {__VA_ARGS__};                  \
        const ArrayMemory memory(_org, _memory, sizeof(_memory)); \
        DIS(_cpu, _org, _contents, _expected, memory);            \
    } while (0)

#endif  // __TEST_FORMATTER_HELPER_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
