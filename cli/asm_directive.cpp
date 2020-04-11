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

#include "asm_directive.h"
#include "file_util.h"

namespace libasm {
namespace cli {

AsmDirective::AsmDirective(Assembler &assembler)
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

AsmDirective::~AsmDirective() {
    free(_line);
}

bool AsmDirective::setCpu(const char *cpu) {
    return _assembler.setCpu(cpu);
}

const char *AsmDirective::listCpu() const {
    return _assembler.listCpu();
}

Error AsmDirective::assembleLine(const char *line, CliMemory &memory) {
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
        const uint32_t origin = _origin;
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

void AsmDirective::setOrigin(uint32_t origin) { _origin = origin; }

const char *AsmDirective::errorAt() const { return _scan; }

void AsmDirective::setSymbolMode(bool reportUndef, bool reportDuplicate) {
    _reportUndef = reportUndef;
    _reportDuplicate = reportDuplicate;
}

struct AsmDirective::Source {
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

const char *AsmDirective::currentSource() const {
    return _sources.empty() ? nullptr : _sources.back().name.c_str();
}

int AsmDirective::currentLineno() const {
    return _sources.empty() ? 0 : _sources.back().lineno;
}

Error AsmDirective::openSource(const char *input_name, const char *end) {
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

const char *AsmDirective::readSourceLine() {
    while (!_sources.empty()) {
        auto source = &_sources.back();
        source->lineno++;
        if (getLine(_line, _line_len, source->fp) >= 0)
            return _line;
        closeSource();
    }
    return nullptr;
}

Error AsmDirective::closeSource() {
    auto source = &_sources.back();
    fclose(source->fp);
    _sources.pop_back();
    return setError(OK);
}

// protected

Error AsmDirective::processDirective(
    const char *directive, const char *&label, CliMemory &memory) {
    return UNKNOWN_DIRECTIVE;
}

Error AsmDirective::processPseudo(
    const char *directive, const char *&label, CliMemory &memory) {
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

Error AsmDirective::defineOrigin() {
    uint32_t value;
    const char *scan = _parser.eval(_scan, value, this);
    if (_parser.getError())
        return setError(_parser);
    _scan = scan;
    // TODO line end check
    _origin = value;
    _list.address = value;
    return setError(OK);
}

Error AsmDirective::defineLabel(const char *&label, CliMemory &memory) {
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

Error AsmDirective::includeFile() {
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

Error AsmDirective::defineBytes(CliMemory &memory) {
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

Error AsmDirective::defineWords(CliMemory &memory) {
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

Error AsmDirective::defineSpaces() {
    uint16_t val16;
    _scan = _parser.eval(_scan, val16, this);
    if (setError(_parser)) return getError();
    if (_origin + val16 < _origin) return setError(OVERFLOW_RANGE);
    _origin += val16;
    return setError(OK);
}

const char *AsmDirective::lookup(uint32_t address) {
    return nullptr;
}

bool AsmDirective::hasSymbol(const char *symbol, const char *end) {
    return end ? hasSymbol(std::string(symbol, end - symbol))
        : hasSymbol(std::string(symbol));
}

uint32_t AsmDirective::lookup(const char *symbol, const char *end) {
    return end ? lookup(std::string(symbol, end - symbol))
        : lookup(std::string(symbol));
}

void AsmDirective::intern(
    uint32_t value, const char *symbol, const char *end) {
    if (end) intern(value, std::string(symbol, end - symbol));
    intern(value, std::string(symbol));
}

uint32_t AsmDirective::currentOrigin() { return _origin; }

void AsmDirective::skipSpaces() {
    while (isspace(*_scan))
        _scan++;
}

bool AsmDirective::hasSymbol(const std::string &key) {
    auto it = _symbols.find(key);
    if (_reportUndef && it == _symbols.end())
        setError(UNDEFINED_SYMBOL);
    return it != _symbols.end();
}

uint32_t AsmDirective::lookup(const std::string &key) {
    auto it = _symbols.find(key);
    if (_reportUndef && it == _symbols.end())
        setError(UNDEFINED_SYMBOL);
    return it == _symbols.end() ? 0 : it->second;
}

void AsmDirective::intern(uint32_t value, const std::string &key) {
    if (_reportDuplicate && hasSymbol(key)) {
        setError(DUPLICATE_LABEL);
    } else {
        _symbols.erase(key);
        _symbols.emplace(key, value);
        setError(OK);
    }
}

int AsmDirective::trimRight(const char *str, int len) {
    while (len > 0 && isspace(str[len - 1]))
        len--;
    return len;
}

// ListingLine oevrrides

uint32_t AsmDirective::startAddress() const {
    return _list.address;
}

int AsmDirective::generatedSize() const {
    return _list.length;
}

uint8_t AsmDirective::getByte(int offset) const {
    uint8_t val = 0;
    _list.memory->readByte(_list.address + offset, val);
    return val;
}

bool AsmDirective::hasInstruction() const {
    return _list.instruction_len;
}

std::string AsmDirective::getInstruction() const {
    return std::string(_list.instruction, _list.instruction_len);
}

bool AsmDirective::hasOperand() const {
    return _list.operand_len;
}

std::string AsmDirective::getOperand() const {
    return std::string(_list.operand,
                       trimRight(_list.operand, _list.operand_len));
}

uint16_t AsmDirective::lineNumber() const {
    return _list.line_number;
}

uint16_t AsmDirective::includeNest() const {
    return _list.include_nest;
}

bool AsmDirective::hasValue() const {
    return _list.value_defined;
}

uint32_t AsmDirective::value() const {
    return _list.value;
}

bool AsmDirective::hasLabel() const {
    return _list.label_len;
}

std::string AsmDirective::getLabel() const {
    return std::string(_list.label, _list.label_len);
}

bool AsmDirective::hasComment() const {
    return _list.comment && *_list.comment;
}

std::string AsmDirective::getComment() const {
    return std::string(_list.comment);
}

AddressWidth AsmDirective::addressWidth() const {
    return _assembler.addressWidth();
}

OpCodeWidth AsmDirective::opCodeWidth() const {
    return _assembler.opCodeWidth();
}

int AsmDirective::maxBytes() const {
    return 6;
}

int AsmDirective::labelWidth() const {
    return _labelWidth;
}

int AsmDirective::instructionWidth() const {
    return _assembler.nameMax() + 1;
}

int AsmDirective::operandWidth() const {
    return _operandWidth;
}

// Motorola type directives

AsmMotoDirective::AsmMotoDirective(Assembler &assembler)
    : AsmDirective(assembler)
{}

BinFormatter *AsmMotoDirective::defaultFormatter() const {
    return new MotoSrec(_assembler.addressWidth());
}

Error AsmMotoDirective::processDirective(
    const char *directive, const char *&label, CliMemory &memory) {
    if (strcasecmp(directive, "fcb") == 0 ||
        strcasecmp(directive, "fcc") == 0)
        return this->defineBytes(memory);
    if (strcasecmp(directive, "fdb") == 0)
        return this->defineWords(memory);
    if (strcasecmp(directive, "rmb") == 0)
        return this->defineSpaces();
    return UNKNOWN_DIRECTIVE;
}

AsmMostekDirective::AsmMostekDirective(Assembler &assembler)
    : AsmDirective(assembler)
{}

BinFormatter *AsmMostekDirective::defaultFormatter() const {
    return new MotoSrec(_assembler.addressWidth());
}

Error AsmMostekDirective::processDirective(
    const char *directive, const char *&label, CliMemory &memory) {
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

AsmIntelDirective::AsmIntelDirective(Assembler &assembler)
    : AsmDirective(assembler)
{}

BinFormatter *AsmIntelDirective::defaultFormatter() const {
    return new IntelHex(_assembler.addressWidth());
}        

Error AsmIntelDirective::processDirective(
    const char *directive, const char *&label, CliMemory &memory) {
    this->_parser.isSymbolLetter(0);
    if (strcasecmp(directive, "db") == 0)
        return this->defineBytes(memory);
    if (strcasecmp(directive, "dw") == 0)
        return this->defineWords(memory);
    if (strcasecmp(directive, "ds") == 0)
        return this->defineSpaces();
    return UNKNOWN_DIRECTIVE;
}

} // namespace cli
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
