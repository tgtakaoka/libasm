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

    // pseudoHandlers
    Error defineOrigin();
    Error alignOrigin();
    Error defineSymbol();
    Error includeFile();
    Error defineUint8();
    Error defineUint8WithTerminator();
    Error defineUint16();
    Error defineUint32();
    Error allocateUint8();
    Error allocateUint16();
    Error allocateUint32();
    Error closeSource();
    Error switchCpu();
    Error switchIntelZilog();

    Error defineLabel(const char *&label, CliMemory &memory);
    Error defineUint8s(CliMemory &memory, bool terminator = false);
    Error defineUint16s(CliMemory &memory);
    Error defineUint32s(CliMemory &memory);
    Error allocateSpaces(const uint8_t unit = 1);

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

    // pseudoHandler's context
    const char *_label;
    CliMemory *_memory;

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

    std::map<std::string, Error (AsmCommonDirective::*)()> _pseudos;
    void registerPseudo(const char *name, Error (AsmCommonDirective::*handler)());
    Error processPseudo(const char *name);

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
    Error processPseudo(const char *name, AsmCommonDirective &common) const {
        auto it = _pseudos.find(name);
        return it == _pseudos.end() ? UNKNOWN_DIRECTIVE : (common.*it->second)();
    }
    virtual BinFormatter &defaultFormatter() = 0;

protected:
    Assembler &_assembler;
    std::map<std::string, Error (AsmCommonDirective::*)()> _pseudos;

    AsmDirective(Assembler &assembler) : _assembler(assembler) {}
    void registerPseudo(const char *name, Error (AsmCommonDirective::*handler)()) {
        _pseudos.emplace(std::make_pair(std::string(name), handler));
    }
};

class AsmMotoDirective : public AsmDirective {
public:
    AsmMotoDirective(Assembler &assembler);
    BinFormatter &defaultFormatter() override { return _formatter; }
private:
    MotoSrec _formatter;
};

class AsmIntelDirective : public AsmDirective {
public:
    AsmIntelDirective(Assembler &assembler);
    BinFormatter &defaultFormatter() override { return _formatter; }
private:
    IntelHex _formatter;
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
