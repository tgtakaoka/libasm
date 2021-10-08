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

#include <functional>
#include <map>
#include <vector>

#include "asm_base.h"
#include "bin_formatter.h"
#include "cli_listing.h"
#include "cli_memory.h"
#include "error_reporter.h"

namespace libasm {
namespace cli {

class AsmDirective;

class AsmCommonDirective : public ErrorReporter, public ListingLine, protected SymbolTable {
public:
    AsmCommonDirective(std::vector<AsmDirective *> &directives);
    virtual ~AsmCommonDirective();

    AsmDirective *restrictCpu(const char *cpu);
    AsmDirective *setCpu(const char *cpu);
    std::string listCpu(const char *separator) const;
    AsmDirective *currentDirective();

    Error assembleLine(const char *line, CliMemory &memory);

    void setOrigin(uint32_t origin);
    const char *errorAt() const;
    void setSymbolMode(bool reportUndef, bool reportDuplicate);
    const char *currentSource() const;
    int currentLineno() const;
    Error openSource(const char *input_name, const char *end = nullptr);
    const char *readSourceLine();

    bool compareDirective(const char *name, const char *directive_name) const;
    Error defineOrigin();
    Error alignOrigin();
    Error defineLabel(const char *&label, CliMemory &memory);
    Error includeFile();
    Error defineBytes(CliMemory &memory, bool terminator = false);
    Error defineWords(CliMemory &memory);
    Error defineSpaces();

private:
    std::vector<AsmDirective *> _directives;
    AsmDirective *_asmZ80;
    AsmDirective *_asmI8080;
    AsmDirective *_directive;
    Assembler *_assembler;
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
    std::vector<Source *> _sources;

    struct Listing {
        uint16_t line_number;
        uint16_t include_nest;
        const char *label;  // if label defined
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
    Error processPseudo(const char *directive, const char *&label, CliMemory &memory);

    // SymbolTable
    const char *lookupValue(uint32_t address) const override;
    bool hasSymbol(const char *symbol, const char *end = nullptr) const override;
    uint32_t lookupSymbol(const char *symbol, const char *end = nullptr) const override;
    Error internSymbol(uint32_t value, const char *symbol, const char *end = nullptr);

    void skipSpaces();

private:
    std::map<std::string, uint32_t, std::less<>> _symbols;
    bool symbolExists(const std::string &key) const;
    uint32_t symbolLookup(const std::string &key) const;
    Error symbolIntern(uint32_t value, const std::string &key);

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
    const ConfigBase &config() const override;
    int nameWidth() const override;
    int codeBytes() const override;
    int labelWidth() const override;
    int operandWidth() const override;

    uint8_t addrUnit() const;

private:
    AsmDirective *switchDirective(AsmDirective *);

    static int trimRight(const char *str, int len);
};

class AsmDirective {
public:
    Assembler &assembler() { return _assembler; }
    virtual BinFormatter *defaultFormatter() const = 0;
    virtual Error processDirective(const char *directive, const char *&label, CliMemory &memory,
            AsmCommonDirective &common) = 0;

protected:
    Assembler &_assembler;

    AsmDirective(Assembler &assembler) : _assembler(assembler) {}
};

class AsmMotoDirective : public AsmDirective {
public:
    AsmMotoDirective(Assembler &assembler);
    BinFormatter *defaultFormatter() const override;
    Error processDirective(const char *directive, const char *&label, CliMemory &memory,
            AsmCommonDirective &common) override;
};

class AsmMostekDirective : public AsmDirective {
public:
    AsmMostekDirective(Assembler &assembler);
    BinFormatter *defaultFormatter() const override;
    Error processDirective(const char *directive, const char *&label, CliMemory &memory,
            AsmCommonDirective &common) override;
};

class AsmIntelDirective : public AsmDirective {
public:
    AsmIntelDirective(Assembler &assembler);
    BinFormatter *defaultFormatter() const override;
    Error processDirective(const char *directive, const char *&label, CliMemory &memory,
            AsmCommonDirective &common) override;
};

}  // namespace cli
}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
