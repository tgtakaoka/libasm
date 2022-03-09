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
#include <list>
#include <map>

#include "asm_base.h"
#include "bin_formatter.h"
#include "cli_listing.h"
#include "cli_memory.h"
#include "error_reporter.h"
#include "function_store.h"

namespace libasm {
namespace cli {

class AsmDirective;

enum SymbolMode {
    REPORT_UNDEFINED = 0,
    REPORT_DUPLICATE = 1,
};

class AsmCommonDirective : public ErrorReporter, public ListingLine, protected SymbolTable {
public:
    AsmCommonDirective(AsmDirective **begin, AsmDirective **end);
    virtual ~AsmCommonDirective();

    AsmDirective *restrictCpu(const char *cpu) { return _directives.restrictCpu(cpu); }
    AsmDirective *setCpu(const char *cpu) { return _directives.setCpu(cpu); }
    std::string listCpu(const char *separator) const;
    AsmDirective *current() { return _directives.current(); }

    Error assembleLine(const char *line, CliMemory &memory);

    void reset();
    void setSymbolMode(SymbolMode mode) { _symbolMode = mode; }
    const char *currentSource() const;
    int currentLineno() const;
    Error openSource(const StrScanner &input_name);
    StrScanner *readSourceLine();
    Error closeSource();

    // pseudoHandlers
    typedef Error (AsmCommonDirective::*PseudoHandler)(
            StrScanner &scan, StrScanner &label, CliMemory &memory);
    Error defineOrigin(StrScanner &scan, StrScanner &label, CliMemory &memory);
    Error alignOrigin(StrScanner &scan, StrScanner &label, CliMemory &memory);
    Error defineLabel(StrScanner &scan, StrScanner &label, CliMemory &memory);
    Error includeFile(StrScanner &scan, StrScanner &label, CliMemory &memory);
    Error defineUint8s(StrScanner &scan, StrScanner &label, CliMemory &memory);
    Error defineString(StrScanner &scan, StrScanner &label, CliMemory &memory);
    Error defineUint16s(StrScanner &scan, StrScanner &label, CliMemory &memory);
    Error defineUint32s(StrScanner &scan, StrScanner &label, CliMemory &memory);
    Error allocateUint8s(StrScanner &scan, StrScanner &label, CliMemory &memory);
    Error allocateUint16s(StrScanner &scan, StrScanner &label, CliMemory &memory);
    Error allocateUint32s(StrScanner &scan, StrScanner &label, CliMemory &memory);
    Error switchCpu(StrScanner &scan, StrScanner &label, CliMemory &memory);
    Error switchIntelZilog(StrScanner &scan, StrScanner &label, CliMemory &memory);
    Error endAssemble(StrScanner &scan, StrScanner &label, CliMemory &memory);
    Error defineFunction(StrScanner &scan, StrScanner &label, CliMemory &memory);

    Error defineUint8s(StrScanner &scan, CliMemory &memory, bool delimitor);
    Error allocateSpaces(StrScanner &scan, size_t unit);

private:
    struct Directives {
        Directives(AsmDirective **begin, AsmDirective **end);
        void setFunctionStore(FunctionStore *functionStore);
        AsmDirective *current() const { return _current; }
        Assembler &assembler() const;
        ValueParser &parser() const { return assembler().parser(); }
        std::list<std::string> listCpu() const;
        std::string currentCpu() const;
        bool is8080() const;
        AsmDirective *setCpu(const char *cpu);
        AsmDirective *restrictCpu(const char *cpu);

    private:
        std::list<AsmDirective *> _directives;
        AsmDirective *_current;
        ValueParser::FuncParser *_savedFuncParser;
        FunctionStore *_functionStore;
        AsmDirective *switchDirective(AsmDirective *dir);
    };
    Directives _directives;
    size_t _line_len;
    char *_line;
    StrScanner _line_scan;
    uint32_t _origin;
    SymbolMode _symbolMode;
    int _labelWidth;
    int _operandWidth;

    FunctionStore _functions;

    static constexpr int max_includes = 4;
    struct Source;
    std::list<Source *> _sources;

    struct Listing {
        uint16_t line_number;
        uint16_t include_nest;
        uint32_t address;
        CliMemory *memory;
        int length;
        Value value;
        StrScanner label;
        StrScanner instruction;
        StrScanner operand;
        StrScanner comment;
    } _list;

    std::map<std::string, PseudoHandler, icasecmp> _pseudos;
    void registerPseudo(const char *name, PseudoHandler handler);
    Error processPseudo(
            const StrScanner &name, StrScanner &scan, StrScanner &label, CliMemory &memory);

    // SymbolTable
    const char *lookupValue(uint32_t address) const override;
    bool hasSymbol(const StrScanner &symbol) const override;
    uint32_t lookupSymbol(const StrScanner &symbol) const override;
    Error internSymbol(uint32_t value, const StrScanner &symbol);

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
};

class AsmDirective {
public:
    Assembler &assembler() const { return _assembler; }
    Error processPseudo(const StrScanner &name, AsmCommonDirective &common, StrScanner &scan,
            StrScanner &label, CliMemory &memory) const;
    virtual BinFormatter &binFormatter() = 0;

protected:
    Assembler &_assembler;
    std::map<std::string, AsmCommonDirective::PseudoHandler, icasecmp> _pseudos;

    AsmDirective(Assembler &assembler) : _assembler(assembler) {}
    void registerPseudo(const char *name, AsmCommonDirective::PseudoHandler handler);
};

class MotorolaDirective : public AsmDirective {
public:
    MotorolaDirective(Assembler &assembler);
    BinFormatter &binFormatter() override { return _formatter; }

private:
    MotoSrec _formatter;
};

class IntelDirective : public AsmDirective {
public:
    IntelDirective(Assembler &assembler);
    BinFormatter &binFormatter() override { return _formatter; }

private:
    IntelHex _formatter;
};

class NationalDirective : public IntelDirective {
public:
    NationalDirective(Assembler &assembler);
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
