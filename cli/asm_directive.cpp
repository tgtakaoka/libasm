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

#include <string.h>
#include <set>
#include "file_util.h"

namespace libasm {
namespace cli {

AsmCommonDirective::AsmCommonDirective(
        std::vector<AsmDirective *> &directives) {
    _directives.reserve(directives.size());
    _directives.insert(
            _directives.begin(), directives.begin(), directives.end());
    _asmZ80 = _asmI8080 = nullptr;
    for (auto directive : _directives) {
        if (directive->assembler().setCpu("Z80")) {
            _asmZ80 = directive;
        } else if (directive->assembler().setCpu("8080")) {
            _asmI8080 = directive;
        }
    }
    _directive = _directives.front();
    _assembler = &_directive->assembler();
    _line_len = 128;
    _line = static_cast<char *>(malloc(_line_len));
    _scan = nullptr;
    _origin = 0;
    _reportUndef = true;
    _reportDuplicate = true;
    _labelWidth = 16;
    _operandWidth = 16;
}

AsmCommonDirective::~AsmCommonDirective() {
    free(_line);
}

AsmDirective *AsmCommonDirective::restrictCpu(const char *cpu) {
    AsmDirective *dir = setCpu(cpu);
    if (dir) {
        _directives.clear();
        _directives.push_back(dir);
        if (dir == _asmI8080)
            _directives.push_back(_asmZ80);
        if (dir == _asmZ80)
            _directives.push_back(_asmI8080);
    }
    return dir;
}

AsmDirective *AsmCommonDirective::switchDirective(AsmDirective *dir) {
    _directive = dir;
    _assembler = &dir->assembler();
    _assembler->reset();
    return dir;
}

AsmDirective *AsmCommonDirective::setCpu(const char *cpu) {
    for (auto dir : _directives) {
        if (dir->assembler().setCpu(cpu))
            return switchDirective(dir);
    }
    return nullptr;
}

static void appendTo(std::string &list, const std::string &cpu,
        std::set<std::string> &cpuSet) {
    if (cpuSet.find(cpu) == cpuSet.end()) {
        cpuSet.insert(cpu);
        if (!list.empty())
            list += ", ";
        list.append(cpu);
    }
}

static std::string filter(const char *text, std::set<std::string> &cpuSet) {
    std::string list;
    while (true) {
        const char *del = strchr(text, ',');
        if (del == nullptr) {
            const std::string cpu(text);
            appendTo(list, cpu, cpuSet);
            return list;
        }
        const std::string cpu(text, del);
        appendTo(list, cpu, cpuSet);
        for (text = del + 1; *text == ' '; text++)
            ;
    }
}

std::string AsmCommonDirective::listCpu(const char *separator) const {
    std::set<std::string> cpuSet;
    std::string cpuList;
    std::string buf = "";
    for (auto dir : _directives) {
        const std::string list(filter(dir->assembler().listCpu(), cpuSet));
        if (buf.size() + list.size() < 47) {
            if (buf.size())
                buf += ", ";
            buf += list;
        } else {
            cpuList += separator;
            cpuList += buf;
            buf = list;
        }
    }
    cpuList += separator + buf;
    return cpuList;
}

AsmDirective *AsmCommonDirective::currentDirective() {
    return _directive;
}

Error AsmCommonDirective::assembleLine(const char *line, CliMemory &memory) {
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
    ValueParser &parser = _assembler->getParser();
    if (parser.isSymbolLetter(*_scan, true)) {
        _list.label = _scan;
        const char *end = parser.scanSymbol(_list.label);
        label_buf = std::string(_list.label, end);
        if (*end == ':')
            end++;  // optional trailing ':' for label.
        _list.label_len = end - _list.label;
        label = label_buf.c_str();
        _scan = end;
    }
    skipSpaces();

    if (!_assembler->endOfLine(_scan)) {
        _list.instruction = _scan;
        const char *end = _list.instruction;
        while (*end && !isspace(*end))
            end++;
        std::string directive(_list.instruction, end);
        _list.instruction_len = end - _list.instruction;
        _scan = end;
        skipSpaces();
        _list.operand = _scan;
        const uint32_t origin = _origin;
        const Error error = processPseudo(directive.c_str(), label, memory);
        if (error == UNKNOWN_DIRECTIVE) {
            _scan = _list.instruction;
        } else if (error) {
            return error;
        } else {
            _list.operand_len = _scan - _list.operand;
            skipSpaces();
            _list.comment = _scan;
            if (label)
                internSymbol(origin, label);
            if (getError()) {
                _scan = _list.label;
            } else {
                _scan = _list.operand + _list.operand_len;
            }
            return getError();
        }
    }

    if (label) {
        internSymbol(_origin, label);
        if (getError()) {
            _scan = _list.label;
            return getError();
        }
    }

    if (_assembler->endOfLine(_scan)) {
        _list.comment = _scan;
        return setError(OK);  // skip comment
    }

    Insn insn;
    const Error error = _assembler->encode(_scan, insn, _origin, this);
    const bool allowUndef = !_reportUndef && error == UNDEFINED_SYMBOL;
    _scan = _assembler->errorAt();
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

void AsmCommonDirective::setOrigin(uint32_t origin) {
    _origin = origin;
}

const char *AsmCommonDirective::errorAt() const {
    return _scan;
}

void AsmCommonDirective::setSymbolMode(bool reportUndef, bool reportDuplicate) {
    _reportUndef = reportUndef;
    _reportDuplicate = reportDuplicate;
}

struct AsmCommonDirective::Source {
    Source(const char *file_name, const char *end, const Source *parent)
        : fp(nullptr), lineno(0), name(file_name, end), include_from(parent) {}
    Source(const std::string &file_name, const Source *parent)
        : fp(nullptr), lineno(0), name(file_name), include_from(parent) {}
    FILE *fp;
    int lineno;
    const std::string name;
    const Source *include_from;
};

const char *AsmCommonDirective::currentSource() const {
    return _sources.empty() ? nullptr : _sources.back()->name.c_str();
}

int AsmCommonDirective::currentLineno() const {
    return _sources.empty() ? 0 : _sources.back()->lineno;
}

Error AsmCommonDirective::openSource(const char *input_name, const char *end) {
    if (_sources.size() >= max_includes)
        return setError(TOO_MANY_INCLUDE);
    if (end == nullptr)
        end = input_name + strlen(input_name);
    const Source *parent = _sources.empty() ? nullptr : _sources.back();
    const size_t pos =
            parent ? parent->name.find_last_of('/') : std::string::npos;
    Source *source;
    if (pos == std::string::npos || *input_name == '/') {
        source = new Source(input_name, end, parent);
    } else {
        std::string name(parent->name.c_str(), pos + 1);
        name.append(input_name, end);
        source = new Source(name, parent);
    }
    _sources.push_back(source);
    if ((source->fp = fopen(source->name.c_str(), "r")) == nullptr) {
        _sources.pop_back();
        return setError(NO_INCLUDE_FOUND);
    }
    return setError(OK);
}

const char *AsmCommonDirective::readSourceLine() {
    while (!_sources.empty()) {
        Source *source = _sources.back();
        source->lineno++;
        if (getLine(_line, _line_len, source->fp) >= 0)
            return _line;
        closeSource();
    }
    return nullptr;
}

Error AsmCommonDirective::closeSource() {
    Source *source = _sources.back();
    fclose(source->fp);
    delete source;
    _sources.pop_back();
    return setError(OK);
}

bool AsmCommonDirective::compareDirective(
        const char *name, const char *directive_name) const {
    if (strcasecmp(name, directive_name) == 0)
        return true;
    return *directive_name == '.' && strcasecmp(name, directive_name + 1) == 0;
}

Error AsmCommonDirective::processPseudo(
        const char *directive, const char *&label, CliMemory &memory) {
    if (_directive->processDirective(directive, label, memory, *this) !=
            UNKNOWN_DIRECTIVE)
        return getError();
    if (compareDirective(directive, ".org"))
        return defineOrigin();
    if (compareDirective(directive, ".equ"))
        return defineLabel(label, memory);
    if (compareDirective(directive, ".include"))
        return includeFile();
    // TODO: implement listing after "end".
    if (compareDirective(directive, ".end"))
        return closeSource();
    if (compareDirective(directive, ".align"))
        return alignOrigin();
    if (compareDirective(directive, ".byte"))
        return defineBytes(memory);
    if (compareDirective(directive, ".ascii"))
        return defineBytes(memory, /* terminator */ true);
    if (compareDirective(directive, ".word"))
        return defineWords(memory);
    if (compareDirective(directive, ".space"))
        return defineSpaces();
    if (compareDirective(directive, ".cpu")) {
        const char *p = _scan;
        while (*p && !isspace(*p))
            p++;
        std::string cpu(_scan, p);
        if (setCpu(cpu.c_str()) == nullptr)
            return setError(UNSUPPORTED_CPU);
        _scan = p;
        return setError(OK);
    }
    if (compareDirective(directive, ".z80syntax")) {
        const char *cpu = _assembler->getCpu();
        if (strcmp(cpu, "8080") && strcmp(cpu, "8085"))
            return setError(UNKNOWN_DIRECTIVE);
        const char *p = _assembler->getParser().scanSymbol(_scan);
        std::string val(_scan, p);
        bool value = false;
        if (strcasecmp(val.c_str(), "on") == 0) {
            value = true;
        } else if (strcasecmp(val.c_str(), "off") == 0) {
            value = false;
        } else
            return setError(UNKNOWN_OPERAND);
        _scan = p;
        if (_directive == _asmI8080 && value)
            switchDirective(_asmZ80)->assembler().setCpu(cpu);
        if (_directive == _asmZ80 && !value)
            switchDirective(_asmI8080)->assembler().setCpu(cpu);
        return setError(OK);
    }
    return setError(UNKNOWN_DIRECTIVE);
}

Error AsmCommonDirective::defineOrigin() {
    ValueParser &parser = _assembler->getParser();
    Value value;
    const char *scan = parser.eval(_scan, nullptr, value, this);
    if (setError(parser.error()))
        return getError();
    if (_reportUndef && value.isUndefined())
        return setError(UNDEFINED_SYMBOL);
    _scan = scan;
    // TODO line end check
    _list.address = _origin = value.getUnsigned();
    return setError(OK);
}

Error AsmCommonDirective::alignOrigin() {
    ValueParser &parser = _assembler->getParser();
    Value value;
    const char *scan = parser.eval(_scan, nullptr, value, this);
    if (setError(parser.error()))
        return getError();
    if (_reportUndef && value.isUndefined())
        return setError(UNDEFINED_SYMBOL);
    if (value.overflowUint16())
        return setError(OVERFLOW_RANGE);
    const uint16_t val16 = value.getUnsigned();
    if (val16 > 0x1000)
        setError(ILLEGAL_OPERAND);
    _scan = scan;
    // TODO line end check
    _list.address = _origin;
    _origin += val16 - 1;
    _origin -= _origin % val16;
    return setError(OK);
}

Error AsmCommonDirective::defineLabel(const char *&label, CliMemory &memory) {
    if (label == nullptr)
        return setError(MISSING_LABEL);
    if (_reportDuplicate && hasSymbol(label))
        return setError(DUPLICATE_LABEL);
    ValueParser &parser = _assembler->getParser();
    Value value;
    const char *scan = parser.eval(_scan, nullptr, value, this);
    if (setError(parser.error()))
        return getError();
    if (_reportUndef && value.isUndefined())
        return setError(UNDEFINED_SYMBOL);
    _scan = scan;
    // TODO line end check
    internSymbol(value.getUnsigned(), label);
    _list.value = value.getUnsigned();
    _list.value_defined = true;
    label = nullptr;
    return getError();
}

Error AsmCommonDirective::includeFile() {
    const char *filename = _scan;
    char c = 0;
    if (*filename == '"' || *filename == '\'')
        c = *filename++;
    const char *end = filename;
    while (*end && (!c || *end != c) && !isspace(*end))
        end++;
    if (c && *end != c)
        return setError(
                c == '"' ? MISSING_CLOSING_DQUOTE : MISSING_CLOSING_QUOTE);
    return openSource(filename, end);
}

Error AsmCommonDirective::defineBytes(CliMemory &memory, bool terminator) {
    _list.address = _origin;
    _list.length = 0;
    ValueParser &parser = _assembler->getParser();
    do {
        skipSpaces();
        if (terminator || *_scan == '"') {
            const char delim = *_scan;
            const char *p = _scan + 1;
            for (;;) {
                if (*p == 0)
                    return setError(MISSING_CLOSING_DQUOTE);
                if (*p == delim)
                    break;
                char c = 0;
                p = parser.readChar(p, c);
                if (setError(parser.error())) {
                    _scan = p;
                    return getError();
                }
                memory.writeByte(_origin++, c);
                _list.length++;
            }
            _scan = p + 1;
        } else {
            Value value;
            _scan = parser.eval(_scan, nullptr, value, this);
            if (setError(parser.error()))
                return getError();
            if (_reportUndef && value.isUndefined())
                return setError(UNDEFINED_SYMBOL);
            if (value.overflowUint8())
                return setError(OVERFLOW_RANGE);
            memory.writeByte(_origin++, value.getUnsigned());
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

Error AsmCommonDirective::defineWords(CliMemory &memory) {
    _list.address = _origin;
    _list.length = 0;
    ValueParser &parser = _assembler->getParser();
    do {
        skipSpaces();
        Value value;
        _scan = parser.eval(_scan, nullptr, value, this);
        if (setError(parser.error()))
            return getError();
        if (_reportUndef && value.isUndefined())
            return setError(UNDEFINED_SYMBOL);
        if (value.overflowUint16())
            return setError(OVERFLOW_RANGE);
        const uint16_t val16 = value.getUnsigned();
        if (_assembler->endian() == ENDIAN_BIG) {
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

Error AsmCommonDirective::defineSpaces() {
    ValueParser &parser = _assembler->getParser();
    Value value;
    _scan = parser.eval(_scan, nullptr, value, this);
    if (setError(parser.error()))
        return getError();
    if (_reportUndef && value.isUndefined())
        return setError(UNDEFINED_SYMBOL);
    if (value.overflowUint16())
        return setError(OVERFLOW_RANGE);
    const uint16_t val16 = value.getUnsigned();
    if (_origin + val16 < _origin)
        return setError(OVERFLOW_RANGE);
    _origin += val16;
    return setError(OK);
}

const char *AsmCommonDirective::lookupValue(uint32_t address) {
    return nullptr;
}

bool AsmCommonDirective::hasSymbol(const char *symbol, const char *end) {
    return end ? symbolExists(std::string(symbol, end - symbol))
               : symbolExists(std::string(symbol));
}

uint32_t AsmCommonDirective::lookupSymbol(const char *symbol, const char *end) {
    return end ? symbolLookup(std::string(symbol, end - symbol))
               : symbolLookup(std::string(symbol));
}

uint32_t AsmCommonDirective::currentOrigin() {
    return _origin;
}

Error AsmCommonDirective::internSymbol(
        uint32_t value, const char *symbol, const char *end) {
    if (end)
        return symbolIntern(value, std::string(symbol, end - symbol));
    return symbolIntern(value, std::string(symbol));
}

void AsmCommonDirective::skipSpaces() {
    while (isspace(*_scan))
        _scan++;
}

bool AsmCommonDirective::symbolExists(const std::string &key) const {
    auto it = _symbols.find(key);
    return it != _symbols.end();
}

uint32_t AsmCommonDirective::symbolLookup(const std::string &key) const {
    auto it = _symbols.find(key);
    return it == _symbols.end() ? 0 : it->second;
}

Error AsmCommonDirective::symbolIntern(uint32_t value, const std::string &key) {
    if (_reportDuplicate && symbolExists(key))
        return setError(DUPLICATE_LABEL);
    _symbols.erase(key);
    _symbols.emplace(key, value);
    return setError(OK);
}

int AsmCommonDirective::trimRight(const char *str, int len) {
    while (len > 0 && isspace(str[len - 1]))
        len--;
    return len;
}

// ListingLine oevrrides

uint32_t AsmCommonDirective::startAddress() const {
    return _list.address;
}

int AsmCommonDirective::generatedSize() const {
    return _list.length;
}

uint8_t AsmCommonDirective::getByte(int offset) const {
    uint8_t val = 0;
    _list.memory->readByte(_list.address + offset, val);
    return val;
}

bool AsmCommonDirective::hasInstruction() const {
    return _list.instruction_len;
}

std::string AsmCommonDirective::getInstruction() const {
    return std::string(_list.instruction, _list.instruction_len);
}

bool AsmCommonDirective::hasOperand() const {
    return _list.operand_len;
}

std::string AsmCommonDirective::getOperand() const {
    return std::string(
            _list.operand, trimRight(_list.operand, _list.operand_len));
}

uint16_t AsmCommonDirective::lineNumber() const {
    return _list.line_number;
}

uint16_t AsmCommonDirective::includeNest() const {
    return _list.include_nest;
}

bool AsmCommonDirective::hasValue() const {
    return _list.value_defined;
}

uint32_t AsmCommonDirective::value() const {
    return _list.value;
}

bool AsmCommonDirective::hasLabel() const {
    return _list.label_len;
}

std::string AsmCommonDirective::getLabel() const {
    return std::string(_list.label, _list.label_len);
}

bool AsmCommonDirective::hasComment() const {
    return _list.comment && *_list.comment;
}

std::string AsmCommonDirective::getComment() const {
    return std::string(_list.comment);
}

AddressWidth AsmCommonDirective::addressWidth() const {
    return _assembler->addressWidth();
}

OpCodeWidth AsmCommonDirective::opCodeWidth() const {
    return _assembler->opCodeWidth();
}

int AsmCommonDirective::maxBytes() const {
    const uint8_t codeMax = _assembler->codeMax();
    return codeMax < 6 ? codeMax : 6;
}

int AsmCommonDirective::labelWidth() const {
    return _labelWidth;
}

int AsmCommonDirective::instructionWidth() const {
    return _assembler->nameMax() + 1;
}

int AsmCommonDirective::operandWidth() const {
    return _operandWidth;
}

// Motorola type directives

AsmMotoDirective::AsmMotoDirective(Assembler &assembler)
    : AsmDirective(assembler) {}

BinFormatter *AsmMotoDirective::defaultFormatter() const {
    return new MotoSrec(_assembler.addressWidth());
}

Error AsmMotoDirective::processDirective(const char *directive,
        const char *&label, CliMemory &memory, AsmCommonDirective &common) {
    if (common.compareDirective(directive, ".fcb"))
        return common.defineBytes(memory);
    if (common.compareDirective(directive, ".fcc"))
        return common.defineBytes(memory, /* terminator */ true);
    if (common.compareDirective(directive, ".fdb"))
        return common.defineWords(memory);
    if (common.compareDirective(directive, ".rmb"))
        return common.defineSpaces();
    return UNKNOWN_DIRECTIVE;
}

AsmMostekDirective::AsmMostekDirective(Assembler &assembler)
    : AsmDirective(assembler) {}

BinFormatter *AsmMostekDirective::defaultFormatter() const {
    return new MotoSrec(_assembler.addressWidth());
}

Error AsmMostekDirective::processDirective(const char *directive,
        const char *&label, CliMemory &memory, AsmCommonDirective &common) {
    if (common.compareDirective(directive, ":=") ||
            common.compareDirective(directive, "="))
        return common.defineLabel(label, memory);
    if (common.compareDirective(directive, ".fcb"))
        return common.defineBytes(memory);
    if (common.compareDirective(directive, ".fcc"))
        return common.defineBytes(memory, /* terminator */ true);
    if (common.compareDirective(directive, ".fdb"))
        return common.defineWords(memory);
    if (common.compareDirective(directive, ".rmb"))
        return common.defineSpaces();
    return UNKNOWN_DIRECTIVE;
}

AsmIntelDirective::AsmIntelDirective(Assembler &assembler)
    : AsmDirective(assembler) {}

BinFormatter *AsmIntelDirective::defaultFormatter() const {
    return new IntelHex(_assembler.addressWidth());
}

Error AsmIntelDirective::processDirective(const char *directive,
        const char *&label, CliMemory &memory, AsmCommonDirective &common) {
    if (common.compareDirective(directive, ".db"))
        return common.defineBytes(memory);
    if (common.compareDirective(directive, ".dw"))
        return common.defineWords(memory);
    if (common.compareDirective(directive, ".ds"))
        return common.defineSpaces();
    return UNKNOWN_DIRECTIVE;
}

}  // namespace cli
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
