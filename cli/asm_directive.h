/* -*- mode: c++; -*- */
#ifndef __ASM_DIRECTIVE_H__
#define __ASM_DIRECTIVE_H__

#include <map>
#include <functional>
#include <vector>

#include "error_reporter.h"
#include "cli_memory.h"
#include "asm_interface.h"
#include "file_util.h"

template <typename Asm>
class AsmDirective : public ErrorReporter, protected SymbolTable {
public:
    typedef typename Asm::addr_t Addr;

    typedef struct {
        const char *label;      // if label defined
        int label_len;
        Addr address;
        int length;
        const char *instruction;
        int instruction_len;
        const char *operand;
        int operand_len;
        const char *comment;
    } Listing;

    virtual ~AsmDirective() {
        free(_line);
    }

    Error assembleLine(const char *line, CliMemory<Addr> &memory, Listing &listing) {
        _scan = line;
        if (_scan == nullptr) {
            return OK;
        }
        _listing = &listing;

        listing.address = _origin;
        const char *label = nullptr;
        std::string label_buf;
        if (_parser.isSymbolLetter(*_scan, true)) {
            listing.label = _scan;
            const char *end = _parser.scanSymbol(listing.label);
            label_buf = std::string(listing.label, end);
            if (*end == ':') end++; // optional trailing ':' for label.
            listing.label_len = end - listing.label;
            label = label_buf.c_str();
            _scan = end;
        }
        skipSpaces();

        if (*_scan && *_scan != ';') {
            listing.instruction = _scan;
            const char *end = listing.instruction;
            while (*end && !isspace(*end)) end++;
            std::string directive(listing.instruction, end);
            listing.instruction_len = end - listing.instruction;
            _scan = end;
            skipSpaces();
            listing.operand = _scan;
            const Addr origin = _origin;
            const Error error = processPseudo(directive.c_str(), label, memory);
            if (error == UNKNOWN_DIRECTIVE) {
                _scan = listing.instruction;
            } else {
                listing.operand_len = _scan - listing.operand;
                skipSpaces();
                listing.comment = _scan;
                if (label) intern(origin, label);
                if (getError()) {
                    _scan = listing.label;
                } else {
                    _scan = listing.operand + listing.operand_len;
                }
                return setError(error);
            }
        }

        if (label) {
            intern(_origin, label);
            if (getError()) {
                _scan = listing.label;
                return getError();
            }
        }

        if (*_scan == 0 || *_scan == ';') {
            listing.comment = _scan;
            return setError(OK); // skip comment
        }

        Insn insn;
        const Error error = _assembler.encode(_scan, insn, _origin, this);
        const bool ok = error == OK || (!_reportUndef && error == UNDEFINED_SYMBOL);
        _scan = _assembler.errorAt();
        if (ok) {
            listing.operand_len = _scan - listing.operand;
            skipSpaces();
            listing.comment = _scan;
            if (insn.insnLen() > 0) {
                memory.writeBytes(insn.address(), insn.bytes(), insn.insnLen());
                listing.address = insn.address();
                listing.length = insn.insnLen();
                _origin += insn.insnLen();
            }
            return setError(OK);
        }
        return setError(error);
    }

    void setOrigin(Addr origin) { _origin = origin; }
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

protected:
    AsmDirective(Asm &assembler)
        : _assembler(assembler),
          _parser(assembler.getParser()),
          _line_len(128),
          _line(static_cast<char *>(malloc(_line_len))),
          _scan(nullptr),
          _origin(0),
          _reportUndef(true),
          _reportDuplicate(true)
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

    Asm &_assembler;
    AsmOperand &_parser;
    size_t _line_len;
    char *_line;
    const char *_scan;
    Addr _origin;
    bool _reportUndef;
    bool _reportDuplicate;
    Listing *_listing;
    static constexpr int max_includes = 4;
    std::vector<Source> _sources;

    Error closeSource() {
        auto source = &_sources.back();
        fclose(source->fp);
        _sources.pop_back();
        return setError(OK);
    }

    virtual Error processDirective(
        const char *directive, const char *&label, CliMemory<Addr> &memory) {
        return UNKNOWN_DIRECTIVE;
    }

    Error processPseudo(
        const char *directive, const char *&label, CliMemory<Addr> &memory) {
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
            if (!_assembler.acceptCpu(cpu.c_str()))
                return setError(UNSUPPORTED_CPU);
            _scan = p;
            return setError(OK);
        }
        return setError(UNKNOWN_DIRECTIVE);
    }

    Error defineOrigin() {
        Addr value;
        const char *scan = _parser.eval(_scan, value, this);
        if (_parser.getError())
            return setError(_parser);
        _scan = scan;
        // TODO line end check
        _origin = value;
        return setError(OK);
    }

    Error defineLabel(const char *&label, CliMemory<Addr> &memory) {
        if (label == nullptr)
            return setError(MISSING_LABEL);
        if (_reportDuplicate && hasSymbol(label))
            return setError(DUPLICATE_LABEL);
        Addr value;
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

    Error defineBytes(CliMemory<Addr> &memory) {
        _listing->address = _origin;
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
                }
                _scan = p + 1;
            } else if (*_scan == '\'') {
                char c;
                const char *p = _parser.readChar(_scan + 1, c);
                if (setError(_parser)) return getError();
                if (*p++ != '\'') return setError(MISSING_CLOSING_QUOTE);
                _scan = p;
                memory.writeByte(_origin++, c);
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
            }
            skipSpaces();
        } while (*_scan++ == ',');
        _listing->length = _origin - _listing->address;
        return setError(OK);
    }

    Error defineWords(CliMemory<Addr> &memory, bool bigEndian) {
        _listing->address = _origin;
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
            if (bigEndian) {
                memory.writeByte(_origin++, static_cast<uint8_t>(val16 >> 8));
                memory.writeByte(_origin++, static_cast<uint8_t>(val16));
            } else {
                memory.writeByte(_origin++, static_cast<uint8_t>(val16));
                memory.writeByte(_origin++, static_cast<uint8_t>(val16 >> 8));
            }
            skipSpaces();
        } while (*_scan++ == ',');
        _listing->length = _origin - _listing->address;
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
        auto it = _symbols.find(std::string(symbol));
        if (_reportUndef && it == _symbols.cend())
            setError(UNDEFINED_SYMBOL);
        return it != _symbols.cend();
    }

    uint32_t lookup(const char *symbol, const char *end = nullptr) override {
        auto it = _symbols.find(std::string(symbol));
        if (_reportUndef && it == _symbols.cend())
            setError(UNDEFINED_SYMBOL);
        return it == _symbols.cend() ? 0 : it->second;
    }

    void intern(uint32_t value, const char *symbol, const char *end = nullptr) override {
        if (_reportDuplicate && hasSymbol(symbol, end)) {
            setError(DUPLICATE_LABEL);
        } else {
            _symbols.erase(symbol);
            _symbols.emplace(symbol, value);
            setError(OK);
        }
    }

    uint32_t currentOrigin() override { return _origin; }

    void skipSpaces() {
        while (isspace(*_scan))
            _scan++;
    }

private:
    // SymbolTable
    std::map<std::string, uint32_t, std::less<>> _symbols;
};

#endif
