/* -*- mode: c++; -*- */
#ifndef __ASM_DIRECTIVE_H__
#define __ASM_DIRECTIVE_H__

#include <map>
#include <functional>

#include "error_reporter.h"
#include "cli_memory.h"
#include "asm_interface.h"

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

    Error assembleLine(
        const char *scan, CliMemory<Addr> &memory, Listing &listing) {
        _scan = scan;
        _listing = &listing;

        listing.address = _origin;
        const char *label = nullptr;
        std::string label_buf;
        if (_parser.isSymbolLetter(*_scan, true)) {
            listing.label = _scan;
            const char *p = _scan;
            while (_parser.isSymbolLetter(*p))
                p++;
            label_buf = std::string(_scan, p);
            if (*p == ':') p++; // optional trailing ':' for label.
            listing.label_len = _scan - listing.label;
            label = label_buf.c_str();
            _scan = p;
        }
        skipSpaces();

        if (*_scan && *_scan != ';') {
            listing.instruction = _scan;
            const char *p = _scan;
            while (*p && !isspace(*p))
                p++;
            std::string directive(_scan, p);
            _scan = p;
            listing.instruction_len = _scan - listing.instruction;
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
                if (label && internSymbol(label, origin, _reportDuplicate)) {
                    _scan = listing.label;
                } else {
                    _scan = listing.operand + listing.operand_len;
                }
                return setError(error);
            }
        }

        if (label) {
            if (internSymbol(label, _origin, _reportDuplicate)) {
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

protected:
    AsmDirective(Asm &assembler)
        : _assembler(assembler),
          _parser(assembler.getParser()),
          _origin(0),
          _scan(nullptr),
          _reportUndef(true),
          _reportDuplicate(true)
    {}

    Asm &_assembler;
    AsmOperand &_parser;
    Addr _origin;
    const char *_scan;
    bool _reportUndef;
    bool _reportDuplicate;
    Listing *_listing;

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
        internSymbol(label, value, false);
        label = nullptr;
        return getError();
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

    bool hasSymbol(const char *symbol) override {
        auto it = _symbols.find(std::string(symbol));
        if (_reportUndef && it == _symbols.cend())
            setError(UNDEFINED_SYMBOL);
        return it != _symbols.cend();
    }

    uint32_t lookup(const char *symbol) override {
        auto it = _symbols.find(std::string(symbol));
        if (_reportUndef && it == _symbols.cend())
            setError(UNDEFINED_SYMBOL);
        return it == _symbols.cend() ? 0 : it->second;
    }

    uint32_t currentAddress() override { return _origin; }

    Error internSymbol(const char *label, uint32_t addr, bool reportDuplicate) {
        if (reportDuplicate && hasSymbol(label)) return setError(DUPLICATE_LABEL);
        _symbols.erase(label);
        _symbols.emplace(label, addr);
        return setError(OK);
    }

    void skipSpaces() {
        while (isspace(*_scan))
            _scan++;
    }

private:
    // SymbolTable
    std::map<std::string, uint32_t, std::less<>> _symbols;
};

#endif
