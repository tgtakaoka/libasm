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
#include "str_scanner.h"

#include <string.h>
#include <algorithm>
#include <list>

namespace libasm {
namespace cli {

AsmCommonDirective::AsmCommonDirective(std::vector<AsmDirective *> &directives) {
    _directives.reserve(directives.size());
    _directives.insert(_directives.begin(), directives.begin(), directives.end());
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

    registerPseudo(".cpu", &AsmCommonDirective::switchCpu);
    registerPseudo(".include", &AsmCommonDirective::includeFile);
    // TODO: implement listing after "end".
    registerPseudo(".end", &AsmCommonDirective::closeSource);
    registerPseudo(".equ", &AsmCommonDirective::defineSymbol);
    registerPseudo(":=", &AsmCommonDirective::defineSymbol);
    registerPseudo("=", &AsmCommonDirective::defineSymbol);
    registerPseudo(".org", &AsmCommonDirective::defineOrigin);
    registerPseudo(".align", &AsmCommonDirective::alignOrigin);
    registerPseudo(".string", &AsmCommonDirective::defineUint8WithTerminator);
    registerPseudo(".ascii", &AsmCommonDirective::defineUint8WithTerminator);
    registerPseudo(".byte", &AsmCommonDirective::defineUint8);
    registerPseudo(".word", &AsmCommonDirective::defineUint16);
    registerPseudo(".long", &AsmCommonDirective::defineUint32);
    registerPseudo("dc.b", &AsmCommonDirective::defineUint8);
    registerPseudo("dc.w", &AsmCommonDirective::defineUint16);
    registerPseudo("dc.l", &AsmCommonDirective::defineUint32);
    registerPseudo("ds.b", &AsmCommonDirective::allocateUint8);
    registerPseudo("ds.w", &AsmCommonDirective::allocateUint16);
    registerPseudo("ds.l", &AsmCommonDirective::allocateUint32);
    registerPseudo(".z80syntax", &AsmCommonDirective::switchIntelZilog);
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

static void appendTo(const std::string &cpu, std::list<std::string> &list) {
    if (std::find(list.begin(), list.end(), cpu) == list.end())
        list.push_back(cpu);
}

static void filter(const char *text, std::list<std::string> &list) {
    while (*text) {
        const char *del = strchr(text, ',');
        if (del == nullptr) {
            appendTo(std::string(text), list);
            return;
        }
        const std::string cpu(text, del);
        appendTo(cpu, list);
        for (text = del + 1; *text == ' '; text++)
            ;
    }
}

std::string AsmCommonDirective::listCpu(const char *separator) const {
    std::list<std::string> list;
    for (auto dir : _directives) {
        filter(dir->assembler().listCpu(), list);
    }
    std::string cpuList;
    std::string buf;
    for (auto &cpu : list) {
        if (buf.size() + cpu.size() <= 58) {
            if (buf.size())
                buf += ", ";
            buf += cpu;
        } else {
            cpuList += separator;
            cpuList += buf;
            buf = cpu;
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
    ValueParser &parser = _assembler->parser();
    parser.setCurrentOrigin(_origin);
    if (parser.isSymbolLetter(*_scan, true)) {
        _list.label = _scan;
        StrScanner scan(_scan);
        const StrScanner sym = parser.readSymbol(scan);
        const char *end = scan;
        label_buf = std::string(sym, sym.size());
        if (*end == ':')
            end++;  // optional trailing ':' for label.
        _list.label_len = end - _list.label;
        label = label_buf.c_str();
        _scan = end;
    }
    skipSpaces();

    if (!_assembler->endOfLine(*_scan)) {
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
        // setup context
        _label = label;
        _memory = &memory;
        const Error error = processPseudo(directive.c_str());
        label = _label;
        if (error == UNKNOWN_DIRECTIVE) {
            _scan = _list.instruction;
        } else if (error) {
            return error;
        } else {
            _list.operand_len = _scan - _list.operand;
            skipSpaces();
            _list.comment = _scan;
            if (label)
                internSymbol(origin / addrUnit(), label);
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

    if (_assembler->endOfLine(*_scan)) {
        _list.comment = _scan;
        return setError(OK);  // skip comment
    }

    Insn insn(_origin);
    const Error error = _assembler->encode(_scan, insn, this);
    const bool allowUndef = !_reportUndef && error == UNDEFINED_SYMBOL;
    if (error == OK || allowUndef) {
        _scan = _assembler->errorAt();
        _list.operand_len = _scan - _list.operand;
        skipSpaces();
        _list.comment = _scan;
        if (insn.length() > 0) {
            const uint32_t addr = insn.address() * addrUnit();
            memory.writeBytes(addr, insn.bytes(), insn.length());
            _list.address = addr;
            _list.length = insn.length();
            _origin += insn.length() / addrUnit();
        }
        return setError(OK);
    }
    return setError(error);
}

void AsmCommonDirective::reset() {
    currentDirective()->assembler().reset();
    _origin = 0;
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
    const size_t pos = parent ? parent->name.find_last_of('/') : std::string::npos;
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

void AsmCommonDirective::registerPseudo(const char *name, Error (AsmCommonDirective::*handler)()) {
    _pseudos.emplace(std::make_pair(std::string(name), handler));
    if (*name == '.')
        _pseudos.emplace(std::make_pair(std::string(name + 1), handler));
}

Error AsmCommonDirective::processPseudo(const char *name) {
    if (_directive->processPseudo(name, *this) != UNKNOWN_DIRECTIVE)
        return getError();
    auto it = _pseudos.find(name);
    if (it != _pseudos.end())
        return (this->*it->second)();
    return setError(UNKNOWN_DIRECTIVE);
}

// PseudoHandler

Error AsmCommonDirective::defineOrigin() {
    ValueParser &parser = _assembler->parser();
    StrScanner scan(_scan);
    Value value = parser.eval(scan, this);
    _scan = scan;
    if (setError(parser))
        return getError();
    if (_reportUndef && value.isUndefined())
        return setError(UNDEFINED_SYMBOL);
    // TODO line end check
    _list.address = _origin = value.getUnsigned();
    return setError(OK);
}

Error AsmCommonDirective::alignOrigin() {
    ValueParser &parser = _assembler->parser();
    StrScanner scan(_scan);
    Value value = parser.eval(scan, this);
    _scan = scan;
    if (setError(parser))
        return getError();
    if (_reportUndef && value.isUndefined())
        return setError(UNDEFINED_SYMBOL);
    if (value.overflowUint16())
        return setError(OVERFLOW_RANGE);
    const uint16_t val16 = value.getUnsigned();
    if (val16 > 0x1000)
        setError(ILLEGAL_OPERAND);
    // TODO line end check
    _list.address = _origin;
    _origin += val16 - 1;
    _origin -= _origin % val16;
    return setError(OK);
}

Error AsmCommonDirective::defineSymbol() {
    return defineLabel(_label, *_memory);
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
        return setError(c == '"' ? MISSING_CLOSING_DQUOTE : MISSING_CLOSING_QUOTE);
    return openSource(filename, end);
}

Error AsmCommonDirective::defineUint8() {
    return defineUint8s(*_memory, false);
}

Error AsmCommonDirective::defineUint8WithTerminator() {
    return defineUint8s(*_memory, true);
}

Error AsmCommonDirective::defineUint16() {
    return defineUint16s(*_memory);
}

Error AsmCommonDirective::defineUint32() {
    return defineUint32s(*_memory);
}

Error AsmCommonDirective::allocateUint8() {
    return allocateSpaces(sizeof(uint8_t));
}

Error AsmCommonDirective::allocateUint16() {
    return allocateSpaces(sizeof(uint16_t));
}

Error AsmCommonDirective::allocateUint32() {
    return allocateSpaces(sizeof(uint32_t));
}

Error AsmCommonDirective::switchCpu() {
    const char *p = _scan;
    while (*p && !isspace(*p))
        p++;
    std::string cpu(_scan, p);
    if (setCpu(cpu.c_str()) == nullptr)
        return setError(UNSUPPORTED_CPU);
    _scan = p;
    return setError(OK);
}

Error AsmCommonDirective::switchIntelZilog() {
    const char *cpu = _assembler->getCpu();
    if (strcmp(cpu, "8080") && strcmp(cpu, "8085"))
        return setError(UNKNOWN_DIRECTIVE);
    StrScanner scan(_scan);
    StrScanner option = _assembler->parser().readSymbol(scan);
    std::string val(option);
    _scan = scan;
    bool value = false;
    if (strcasecmp(val.c_str(), "on") == 0) {
        value = true;
    } else if (strcasecmp(val.c_str(), "off") == 0) {
        value = false;
    } else
        return setError(UNKNOWN_OPERAND);
    if (_directive == _asmI8080 && value)
        switchDirective(_asmZ80)->assembler().setCpu(cpu);
    if (_directive == _asmZ80 && !value)
        switchDirective(_asmI8080)->assembler().setCpu(cpu);
    return setError(OK);
}

Error AsmCommonDirective::defineLabel(const char *&label, CliMemory &memory) {
    if (label == nullptr)
        return setError(MISSING_LABEL);
    if (_reportDuplicate && hasSymbol(label))
        return setError(DUPLICATE_LABEL);
    ValueParser &parser = _assembler->parser();
    StrScanner scan(_scan);
    Value value = parser.eval(scan, this);
    _scan = scan;
    if (setError(parser))
        return getError();
    if (_reportUndef && value.isUndefined())
        return setError(UNDEFINED_SYMBOL);
    // TODO line end check
    internSymbol(value.getUnsigned(), label);
    _list.value = value.getUnsigned();
    _list.value_defined = true;
    label = nullptr;
    return getError();
}

Error AsmCommonDirective::defineUint8s(CliMemory &memory, bool terminator) {
    _list.address = _origin;
    ValueParser &parser = _assembler->parser();
    const uint32_t base = _origin * addrUnit();
    auto &len = _list.length;
    len = 0;
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
                StrScanner scan(p);
                const char c = parser.readChar(scan);
                p = scan;
                if (setError(parser)) {
                    _scan = p;
                    return getError();
                }
                memory.writeByte(base + len, c);
                len++;
            }
            _scan = p + 1;
        } else {
            StrScanner scan(_scan);
            Value value = parser.eval(scan, this);
            _scan = scan;
            if (setError(parser))
                return getError();
            if (_reportUndef && value.isUndefined())
                return setError(UNDEFINED_SYMBOL);
            if (value.overflowUint8())
                return setError(OVERFLOW_RANGE);
            const uint8_t val8 = value.getUnsigned();
            memory.writeByte(base + len, val8);
            len++;
        }
        const char *save = _scan;
        skipSpaces();
        if (*_scan++ == ',')
            continue;
        _scan = save;
        break;
    } while (true);
    _origin += ((len + addrUnit() - 1) & -addrUnit()) / addrUnit();
    return setError(OK);
}

Error AsmCommonDirective::defineUint16s(CliMemory &memory) {
    _list.address = _origin;
    ValueParser &parser = _assembler->parser();
    const uint32_t base = _origin * addrUnit();
    auto &len = _list.length;
    len = 0;
    const uint8_t hi = uint8_t(config().endian());
    const uint8_t lo = 1 - hi;
    do {
        skipSpaces();
        StrScanner scan(_scan);
        Value value = parser.eval(scan, this);
        _scan = scan;
        if (setError(parser))
            return getError();
        if (_reportUndef && value.isUndefined())
            return setError(UNDEFINED_SYMBOL);
        if (value.overflowUint16())
            return setError(OVERFLOW_RANGE);
        const uint16_t val16 = value.getUnsigned();
        const uint8_t val8lo = val16;
        const uint8_t val8hi = val16 >> 8;
        memory.writeByte(base + len + hi, val8hi);
        memory.writeByte(base + len + lo, val8lo);
        len += 2;
        const char *save = _scan;
        skipSpaces();
        if (*_scan++ == ',')
            continue;
        _scan = save;
        break;
    } while (true);
    _origin += ((len + addrUnit() - 1) & -addrUnit()) / addrUnit();
    return setError(OK);
}

Error AsmCommonDirective::defineUint32s(CliMemory &memory) {
    _list.address = _origin;
    ValueParser &parser = _assembler->parser();
    const uint32_t base = _origin * addrUnit();
    auto &len = _list.length;
    len = 0;
    do {
        skipSpaces();
        StrScanner scan(_scan);
        Value value = parser.eval(scan, this);
        _scan = scan;
        if (setError(parser))
            return getError();
        if (_reportUndef && value.isUndefined())
            return setError(UNDEFINED_SYMBOL);
        uint32_t val32 = value.getUnsigned();
        for (auto i = 0; i < 4; i++) {
            const auto pos = config().endian() == ENDIAN_LITTLE ? i : 3 - i;
            memory.writeByte(base + len + pos, val32);
            val32 >>= 8;
        }
        len += 4;
        const char *save = _scan;
        skipSpaces();
        if (*_scan++ == ',')
            continue;
        _scan = save;
        break;
    } while (true);
    _origin += ((len + addrUnit() - 1) & -addrUnit()) / addrUnit();
    return setError(OK);
}

Error AsmCommonDirective::allocateSpaces(const uint8_t unit) {
    ValueParser &parser = _assembler->parser();
    StrScanner scan(_scan);
    Value value = parser.eval(scan, this);
    _scan = scan;
    if (setError(parser))
        return getError();
    if (_reportUndef && value.isUndefined())
        return setError(UNDEFINED_SYMBOL);
    if (value.overflowUint16())
        return setError(OVERFLOW_RANGE);
    const auto size = value.getUnsigned() * unit;
    if (_origin + size < _origin)
        return setError(OVERFLOW_RANGE);
    _origin += ((size + addrUnit() - 1) & -addrUnit()) / addrUnit();
    return setError(OK);
}

const char *AsmCommonDirective::lookupValue(uint32_t address) const {
    return nullptr;
}

bool AsmCommonDirective::hasSymbol(const char *symbol, const char *end) const {
    return end ? symbolExists(std::string(symbol, end - symbol))
               : symbolExists(std::string(symbol));
}

uint32_t AsmCommonDirective::lookupSymbol(const char *symbol, const char *end) const {
    return end ? symbolLookup(std::string(symbol, end - symbol))
               : symbolLookup(std::string(symbol));
}

Error AsmCommonDirective::internSymbol(uint32_t value, const char *symbol, const char *end) {
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
    auto addr = _list.address * addrUnit() + offset;
    _list.memory->readByte(addr, val);
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
    return std::string(_list.operand, trimRight(_list.operand, _list.operand_len));
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

const ConfigBase &AsmCommonDirective::config() const {
    return _assembler->config();
}

int AsmCommonDirective::codeBytes() const {
    const uint8_t codeMax = config().codeMax();
    return codeMax < 6 ? codeMax : 6;
}

int AsmCommonDirective::labelWidth() const {
    return _labelWidth;
}

int AsmCommonDirective::nameWidth() const {
    return config().nameMax() + 1;
}

int AsmCommonDirective::operandWidth() const {
    return _operandWidth;
}

uint8_t AsmCommonDirective::addrUnit() const {
    return static_cast<uint8_t>(config().addressUnit());
}

AsmMotoDirective::AsmMotoDirective(Assembler &assembler)
    : AsmDirective(assembler), _formatter(assembler.config().addressWidth()) {
    registerPseudo("fcb", &AsmCommonDirective::defineUint8);
    registerPseudo("fcc", &AsmCommonDirective::defineUint8WithTerminator);
    registerPseudo("fdb", &AsmCommonDirective::defineUint16);
    registerPseudo("fdb", &AsmCommonDirective::defineUint16);
    registerPseudo("rmb", &AsmCommonDirective::allocateUint8);
    registerPseudo("dfs", &AsmCommonDirective::allocateUint8);
}

AsmIntelDirective::AsmIntelDirective(Assembler &assembler)
    : AsmDirective(assembler), _formatter(assembler.config().addressWidth()) {
    registerPseudo("db", &AsmCommonDirective::defineUint8);
    registerPseudo("dw", &AsmCommonDirective::defineUint16);
    registerPseudo("dd", &AsmCommonDirective::defineUint32);
    registerPseudo("ds", &AsmCommonDirective::allocateUint8);
}

}  // namespace cli
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
