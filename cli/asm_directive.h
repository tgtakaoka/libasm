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

#include "error_reporter.h"
#include "cli_memory.h"
#include "asm_interface.h"
#include "asm_listing.h"
#include "file_util.h"

namespace libasm {
namespace cli {

template <typename Conf>
class AsmDirective : public ErrorReporter,
                     public AsmLine<typename Conf::uintptr_t>,
                     protected SymbolTable {
    typedef typename Conf::uintptr_t addr_t;

public:
    virtual ~AsmDirective() {
        free(_line);
    }

    bool setCpu(const char *cpu) {
        return _assembler.setCpu(cpu);
    }
    const char *listCpu() const {
        return _assembler.listCpu();
    }

    Error assembleLine(const char *line, CliMemory<Conf> &memory) {
        _scan = line;
        if (_scan == nullptr) {
            return OK;
        }

        _list.line_number = currentLineno();
        _list.include_nest = _sources.size() - 1;
        _list.address = _origin;
        _list.length = 0;
        _list.value_defined = false;
        _list.memory = &memory;
        _list.label_len = 0;
        _list.instruction_len = 0;
        _list.operand_len = 0;
        _list.comment = nullptr;
        const char *label = nullptr;
        std::string label_buf;
        _list.label_len = 0;
        if (_parser.isSymbolLetter(*_scan, true)) {
            _list.label = _scan;
            const char *end = _parser.scanSymbol(_list.label);
            label_buf = std::string(_list.label, end);
            if (*end == ':') end++; // optional trailing ':' for label.
            _list.label_len = end - _list.label;
            label = label_buf.c_str();
            _scan = end;
        }
        skipSpaces();

        if (*_scan && *_scan != ';') {
            _list.instruction = _scan;
            const char *end = _list.instruction;
            while (*end && !isspace(*end)) end++;
            std::string directive(_list.instruction, end);
            _list.instruction_len = end - _list.instruction;
            _scan = end;
            skipSpaces();
            _list.operand = _scan;
            const addr_t origin = _origin;
            const Error error = processPseudo(directive.c_str(), label, memory);
            if (error == UNKNOWN_DIRECTIVE) {
                _scan = _list.instruction;
            } else {
                _list.operand_len = _scan - _list.operand;
                skipSpaces();
                _list.comment = _scan;
                if (label) intern(origin, label);
                if (getError()) {
                    _scan = _list.label;
                } else {
                    _scan = _list.operand + _list.operand_len;
                }
                return setError(error);
            }
        }

        if (label) {
            intern(_origin, label);
            if (getError()) {
                _scan = _list.label;
                return getError();
            }
        }

        if (*_scan == 0 || *_scan == ';') {
            _list.comment = _scan;
            return setError(OK); // skip comment
        }

        Insn insn;
        const Error error = _assembler.encode(_scan, insn, _origin, this);
        const bool allowUndef = !_reportUndef && error == UNDEFINED_SYMBOL;
        _scan = _assembler.errorAt();
        if (error == OK || allowUndef) {
            _list.operand_len = _scan - _list.operand;
            skipSpaces();
            _list.comment = _scan;
            if (insn.length() > 0) {
                memory.writeBytes(insn.address(), insn.bytes(), insn.length());
                _list.address = insn.address();
                _list.length = insn.length();
                _origin += insn.length();
            }
            return setError(OK);
        }
        return setError(error);
    }

    void setOrigin(addr_t origin) { _origin = origin; }
    const char *errorAt() const { return _scan; }
    void setSymbolMode(bool reportUndef, bool reportDuplicate) {
        _reportUndef = reportUndef;
        _reportDuplicate = reportDuplicate;
    }

    const char *currentSource() const {
        return _sources.empty() ? nullptr : _sources.back().name.c_str();
    }

    int currentLineno() const {
        return _sources.empty() ? 0 : _sources.back().lineno;
    }

    Error openSource(const char *input_name, const char *end = nullptr) {
        if (_sources.size() >= max_includes)
            return setError(TOO_MANY_INCLUDE);
        if (end == nullptr) end = input_name + strlen(input_name);
        const auto *parent = _sources.empty() ? nullptr : &_sources.back();
        const size_t pos = parent ? parent->name.find_last_of('/') : std::string::npos;
        if (pos == std::string::npos || *input_name == '/') {
            _sources.emplace_back(input_name, end, parent);
        } else {
            std::string name(parent->name.c_str(), pos + 1);
            name.append(input_name, end);
            _sources.emplace_back(name, parent);
        }
        auto source = &_sources.back();
        if ((source->fp = fopen(source->name.c_str(), "r")) == nullptr) {
            _sources.pop_back();
            return setError(NO_INCLUDE_FOUND);
        }
        return setError(OK);
    }

    const char *readSourceLine() {
        while (!_sources.empty()) {
            auto source = &_sources.back();
            source->lineno++;
            if (getLine(_line, _line_len, source->fp) >= 0)
                return _line;
            closeSource();
        }
        return nullptr;
    }

    // AsmLine
    uint16_t lineNumber() const override { return _list.line_number; }
    uint16_t includeNest() const override { return _list.include_nest; }
    addr_t startAddress() const override { return _list.address; }
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
    int instructionWidth() const override { return _assembler.maxName() + 1; }
    int operandWidth() const override { return _operandWidth; }

    // Error reporting

protected:
    AsmDirective(Assembler<Conf> &assembler)
        : _assembler(assembler),
          _parser(assembler.getParser()),
          _line_len(128),
          _line(static_cast<char *>(malloc(_line_len))),
          _scan(nullptr),
          _origin(0),
          _reportUndef(true),
          _reportDuplicate(true),
          _labelWidth(16),
          _operandWidth(16)
    {}

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

    Assembler<Conf> &_assembler;
    AsmOperand &_parser;
    size_t _line_len;
    char *_line;
    const char *_scan;
    addr_t _origin;
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
        addr_t address;
        CliMemory<Conf> *memory;
        int length;
        bool value_defined;
        uint32_t value;
        const char *instruction;
        int instruction_len;
        const char *operand;
        int operand_len;
        const char *comment;
    } _list;

    Error closeSource() {
        auto source = &_sources.back();
        fclose(source->fp);
        _sources.pop_back();
        return setError(OK);
    }

    virtual Error processDirective(
        const char *directive, const char *&label, CliMemory<Conf> &memory) {
        return UNKNOWN_DIRECTIVE;
    }

    Error processPseudo(
        const char *directive, const char *&label, CliMemory<Conf> &memory) {
        if (processDirective(directive, label, memory) != UNKNOWN_DIRECTIVE)
            return getError();
        if (strcasecmp(directive, "org") == 0)
            return defineOrigin();
        if (strcasecmp(directive, "equ") == 0)
            return defineLabel(label, memory);
        if (strcasecmp(directive, "include") == 0)
            return includeFile();
        // TODO: implement listing after "end".
        if (strcasecmp(directive, "end") == 0)
            return closeSource();
        if (strcasecmp(directive, "cpu") == 0) {
            const char *p = _scan;
            while (*p && !isspace(*p))
                p++;
            std::string cpu(_scan, p);
            if (!_assembler.setCpu(cpu.c_str()))
                return setError(UNSUPPORTED_CPU);
            _scan = p;
            return setError(OK);
        }
        return setError(UNKNOWN_DIRECTIVE);
    }

    Error defineOrigin() {
        addr_t value;
        const char *scan = _parser.eval(_scan, value, this);
        if (_parser.getError())
            return setError(_parser);
        _scan = scan;
        // TODO line end check
        _origin = value;
        _list.address = value;
        return setError(OK);
    }

    Error defineLabel(const char *&label, CliMemory<Conf> &memory) {
        if (label == nullptr)
            return setError(MISSING_LABEL);
        if (_reportDuplicate && hasSymbol(label))
            return setError(DUPLICATE_LABEL);
        uint32_t value;
        const char *scan = _parser.eval(_scan, value, this);
        const Error error = setError(_parser);
        if (!_reportUndef && error == UNDEFINED_SYMBOL) {
            value = 0;
            setError(OK);
        }
        if (getError()) return getError();
        _scan = scan;
        // TODO line end check
        intern(value, label);
        _list.value = value;
        _list.value_defined = true;
        label = nullptr;
        return getError();
    }

    Error includeFile() {
        const char *filename = _scan;
        char c = 0;
        if (*filename == '"' || *filename == '\'')  c = *filename++;
        const char *end = filename;
        while (*end && (!c || *end != c) && !isspace(*end))
            end++;
        if (c && *end != c)
            return setError(c == '"' ? MISSING_CLOSING_DQUOTE : MISSING_CLOSING_QUOTE);
        return openSource(filename, end);
    }

    Error defineBytes(CliMemory<Conf> &memory) {
        _list.address = _origin;
        _list.length = 0;
        do {
            skipSpaces();
            if (*_scan == '"') {
                const char *p = _scan + 1;
                for (;;) {
                    if (*p == 0) return setError(MISSING_CLOSING_DQUOTE);
                    if (*p == '"') break;
                    char c = 0;
                    p = _parser.readChar(p, c);
                    if (setError(_parser)) {
                        _scan = p;
                        return getError();
                    }
                    memory.writeByte(_origin++, c);
                    _list.length++;
                }
                _scan = p + 1;
            } else {
                uint8_t val8;
                _scan = _parser.eval(_scan, val8, this);
                const Error error = setError(_parser);
                if (!_reportUndef && error == UNDEFINED_SYMBOL) {
                    val8 = 0;
                    setError(OK);
                }
                if (getError()) return getError();
                memory.writeByte(_origin++, val8);
                _list.length++;
            }
            const char *save = _scan;
            skipSpaces();
            if (*_scan++ == ',')
                continue;
            _scan = save;
            break;
        } while (true);
        return setError(OK);
    }

    Error defineWords(CliMemory<Conf> &memory) {
        _list.address = _origin;
        _list.length = 0;
        do {
            skipSpaces();
            uint16_t val16;
            _scan = _parser.eval(_scan, val16, this);
            const Error error = setError(_parser);
            if (!_reportUndef && error == UNDEFINED_SYMBOL) {
                val16 = 0;
                setError(OK);
            }
            if (getError()) return getError();
            if (_assembler.endian() == ENDIAN_BIG) {
                memory.writeByte(_origin++, static_cast<uint8_t>(val16 >> 8));
                memory.writeByte(_origin++, static_cast<uint8_t>(val16));
            } else {
                memory.writeByte(_origin++, static_cast<uint8_t>(val16));
                memory.writeByte(_origin++, static_cast<uint8_t>(val16 >> 8));
            }
            _list.length += 2;
            const char *save = _scan;
            skipSpaces();
            if (*_scan++ == ',')
                continue;
            _scan = save;
            break;
        } while (true);
        return setError(OK);
    }

    Error defineSpaces() {
        uint16_t val16;
        _scan = _parser.eval(_scan, val16, this);
        if (setError(_parser)) return getError();
        if (_origin + val16 < _origin) return setError(OVERFLOW_RANGE);
        _origin += val16;
        return setError(OK);
    }

    const char *lookup(uint32_t address) override {
        return nullptr;
    }

    bool hasSymbol(const char *symbol, const char *end = nullptr) override {
        return end ? hasSymbol(std::string(symbol, end - symbol))
            : hasSymbol(std::string(symbol));
    }

    uint32_t lookup(const char *symbol, const char *end = nullptr) override {
        return end ? lookup(std::string(symbol, end - symbol))
            : lookup(std::string(symbol));
    }

    void intern(uint32_t value, const char *symbol, const char *end = nullptr) override {
        if (end) intern(value, std::string(symbol, end - symbol));
        intern(value, std::string(symbol));
    }

    uint32_t currentOrigin() override { return _origin; }

    void skipSpaces() {
        while (isspace(*_scan))
            _scan++;
    }

private:
    // SymbolTable
    std::map<std::string, uint32_t, std::less<>> _symbols;

    bool hasSymbol(const std::string &key) {
        auto it = _symbols.find(key);
        if (_reportUndef && it == _symbols.end())
            setError(UNDEFINED_SYMBOL);
        return it != _symbols.end();
    }

    uint32_t lookup(const std::string &key) {
        auto it = _symbols.find(key);
        if (_reportUndef && it == _symbols.end())
            setError(UNDEFINED_SYMBOL);
        return it == _symbols.end() ? 0 : it->second;
    }

    void intern(uint32_t value, const std::string &key) {
        if (_reportDuplicate && hasSymbol(key)) {
            setError(DUPLICATE_LABEL);
        } else {
            _symbols.erase(key);
            _symbols.emplace(key, value);
            setError(OK);
        }
    }

    static int trimRight(const char *str, int len) {
        while (len > 0 && isspace(str[len - 1]))
            len--;
        return len;
    }
};

template<typename Conf>
class AsmMotoDirective : public AsmDirective<Conf> {
public:
    AsmMotoDirective(Assembler<Conf> &assembler)
        : AsmDirective<Conf>(assembler) {}

protected:
    Error processDirective(
        const char *directive, const char *&label,
        CliMemory<Conf> &memory) override {
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

template<typename Conf>
class AsmMostekDirective : public AsmDirective<Conf> {
public:
    AsmMostekDirective(Assembler<Conf> &assembler)
        : AsmDirective<Conf>(assembler) {}

protected:
    Error processDirective(
        const char *directive, const char *&label,
        CliMemory<Conf> &memory) override {
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

template<typename Conf>
class AsmIntelDirective : public AsmDirective<Conf> {
public:
    AsmIntelDirective(Assembler<Conf> &assembler)
        : AsmDirective<Conf>(assembler) {}

protected:
    Error processDirective(
        const char *directive, const char *&label,
        CliMemory<Conf> &memory) override {
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
