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

    // ListingLine
    AddressWidth addressWidth() const override { return _assembler.addressWidth(); }
    OpCodeWidth opCodeWidth() const override { return _assembler.opCodeWidth(); }
    uint16_t lineNumber() const override { return _list.line_number; }
    uint16_t includeNest() const override { return _list.include_nest; }
    uint32_t startAddress() const override { return _list.address; }
    int generatedSize() const override { return _list.length; }
    uint8_t getByte(int offset) const override {
        uint8_t val = 0;
        _list.memory->readByte(_list.address + offset, val);
        return val;
    }
    bool hasValue() const override { return _list.value_defined; }
    uint32_t value() const override { return _list.value; }
    bool hasLabel() const override { return _list.label_len; }
    bool hasInstruction() const override { return _list.instruction_len; }
    bool hasOperand() const override { return _list.operand_len; }
    bool hasComment() const override { return _list.comment && *_list.comment; }
    std::string getLabel() const override {
        return std::string(_list.label, _list.label_len);
    }
    std::string getInstruction() const override {
        return std::string(_list.instruction, _list.instruction_len);
    }
    std::string getOperand() const override {
        return std::string(_list.operand, trimRight(_list.operand, _list.operand_len));
    }
    std::string getComment() const override {
        return std::string(_list.comment);
    }
    int maxBytes() const override { return 6; }
    int labelWidth() const override { return _labelWidth; }
    int instructionWidth() const override { return _assembler.nameMax() + 1; }
    int operandWidth() const override { return _operandWidth; }

protected:
    AsmDirective(Assembler &assembler);

    struct Source {
        Source(const char *file_name, const char *end,
               const Source *parent)
            : fp(nullptr),
              lineno(0),
              name(file_name, end),
              include_from(parent)
        {}
        Source(const std::string &file_name,
               const Source *parent)
            : fp(nullptr),
              lineno(0),
              name(file_name),
              include_from(parent)
        {}
        FILE *fp;
        int lineno;
        const std::string name;
        const Source *include_from;
    };

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
};

class AsmMotoDirective : public AsmDirective {
public:
    AsmMotoDirective(Assembler &assembler)
        : AsmDirective(assembler) {}

    BinFormatter *defaultFormatter() const override {
        return new MotoSrec(_assembler.addressWidth());
    }

protected:
    Error processDirective(
        const char *directive, const char *&label,
        CliMemory &memory) override {
        if (strcasecmp(directive, "fcb") == 0 ||
            strcasecmp(directive, "fcc") == 0)
            return this->defineBytes(memory);
        if (strcasecmp(directive, "fdb") == 0)
            return this->defineWords(memory);
        if (strcasecmp(directive, "rmb") == 0)
            return this->defineSpaces();
        return UNKNOWN_DIRECTIVE;
    }
};

class AsmMostekDirective : public AsmDirective {
public:
    AsmMostekDirective(Assembler &assembler)
        : AsmDirective(assembler) {}

    BinFormatter *defaultFormatter() const override {
        return new MotoSrec(_assembler.addressWidth());
    }

protected:
    Error processDirective(
        const char *directive, const char *&label,
        CliMemory &memory) override {
        if (strcmp(directive, ":=") == 0
            || strcmp(directive, "=") == 0) {
            return this->defineLabel(label, memory);
        }
        if (strcasecmp(directive, "fcb") == 0)
            return this->defineBytes(memory);
        if (strcasecmp(directive, "fdb") == 0)
            return this->defineWords(memory);
        if (strcasecmp(directive, "rmb") == 0)
            return this->defineSpaces();
        return UNKNOWN_DIRECTIVE;
    }
};

class AsmIntelDirective : public AsmDirective {
public:
    AsmIntelDirective(Assembler &assembler)
        : AsmDirective(assembler) {}

    BinFormatter *defaultFormatter() const override {
        return new IntelHex(_assembler.addressWidth());
    }        

protected:
    Error processDirective(
        const char *directive, const char *&label,
        CliMemory &memory) override {
        this->_parser.isSymbolLetter(0);
        if (strcasecmp(directive, "db") == 0)
            return this->defineBytes(memory);
        if (strcasecmp(directive, "dw") == 0)
            return this->defineWords(memory);
        if (strcasecmp(directive, "ds") == 0)
            return this->defineSpaces();
        return UNKNOWN_DIRECTIVE;
    }
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
