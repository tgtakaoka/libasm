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

#ifndef __ASM_DIRECTIVE_H__
#define __ASM_DIRECTIVE_H__

#include <map>
#include <functional>
#include <vector>

#include "asm_base.h"
#include "cli_listing.h"
#include "bin_formatter.h"
#include "cli_memory.h"
#include "error_reporter.h"

namespace libasm {
namespace cli {

class AsmDirective
    : public ErrorReporter,
      public ListingLine,
      protected SymbolTable {
public:
    virtual ~AsmDirective();

    bool setCpu(const char *cpu);
    const char *listCpu() const;

    virtual BinFormatter *defaultFormatter() const = 0;
    Error assembleLine(const char *line, CliMemory &memory);

    void setOrigin(uint32_t origin);
    const char *errorAt() const;
    void setSymbolMode(bool reportUndef, bool reportDuplicate);
    const char *currentSource() const;
    int currentLineno() const;
    Error openSource(const char *input_name, const char *end = nullptr);
    const char *readSourceLine();

protected:
    AsmDirective(Assembler &assembler);

    Assembler &_assembler;
    AsmOperand &_parser;
    size_t _line_len;
    char *_line;
    const char *_scan;
    uint32_t _origin;
    bool _reportUndef;
    bool _reportDuplicate;
    int _labelWidth;
    int _operandWidth;
    static constexpr int max_includes = 4;
    struct Source;
    std::vector<Source> _sources;

    struct Listing {
        uint16_t line_number;
        uint16_t include_nest;
        const char *label;      // if label defined
        int label_len;
        uint32_t address;
        CliMemory *memory;
        int length;
        bool value_defined;
        uint32_t value;
        const char *instruction;
        int instruction_len;
        const char *operand;
        int operand_len;
        const char *comment;
    } _list;

    Error closeSource();
    virtual Error processDirective(
        const char *directive, const char *&label, CliMemory &memory);
    Error processPseudo(
        const char *directive, const char *&label, CliMemory &memory);
    Error defineOrigin();
    Error defineLabel(const char *&label, CliMemory &memory);
    Error includeFile();
    Error defineBytes(CliMemory &memory);
    Error defineWords(CliMemory &memory);
    Error defineSpaces();

    const char *lookup(uint32_t address) override;
    bool hasSymbol(const char *symbol, const char *end = nullptr) override;
    uint32_t lookup(const char *symbol, const char *end = nullptr) override;
    void intern(uint32_t value, const char *symbol, const char *end = nullptr) override;
    uint32_t currentOrigin() override;

    void skipSpaces();

private:
    // SymbolTable
    std::map<std::string, uint32_t, std::less<>> _symbols;

    bool hasSymbol(const std::string &key);
    uint32_t lookup(const std::string &key);
    void intern(uint32_t value, const std::string &key);
    static int trimRight(const char *str, int len);

    // ListingLine
public:
    uint32_t startAddress() const override;
    int generatedSize() const override;
    uint8_t getByte(int offset) const override;
    bool hasInstruction() const override;
    std::string getInstruction() const override;
    bool hasOperand() const override;
    std::string getOperand() const override;
    uint16_t lineNumber() const override;
    uint16_t includeNest() const override;
    bool hasValue() const override;
    uint32_t value() const override;
    bool hasLabel() const override;
    std::string getLabel() const override;
    bool hasComment() const override;
    std::string getComment() const override;
public:
    AddressWidth addressWidth() const override;
private:
    OpCodeWidth opCodeWidth() const override;
    int maxBytes() const override;
    int labelWidth() const override;
    int instructionWidth() const override;
    int operandWidth() const override;
};

class AsmMotoDirective : public AsmDirective {
public:
    AsmMotoDirective(Assembler &assembler);
    BinFormatter *defaultFormatter() const override;
protected:
    Error processDirective(
        const char *directive, const char *&label, CliMemory &memory) override;
};

class AsmMostekDirective : public AsmDirective {
public:
    AsmMostekDirective(Assembler &assembler);
    BinFormatter *defaultFormatter() const override;
protected:
    Error processDirective(
        const char *directive, const char *&label, CliMemory &memory) override;
};

class AsmIntelDirective : public AsmDirective {
public:
    AsmIntelDirective(Assembler &assembler);
    BinFormatter *defaultFormatter() const override;
protected:
    Error processDirective(
        const char *directive, const char *&label, CliMemory &memory) override;
};

} // namespace cli
} // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
