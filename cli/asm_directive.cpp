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

namespace libasm {
namespace cli {

AsmCommonDirective::Directives::Directives(AsmDirective **begin, AsmDirective **end)
    : _current(nullptr) {
    _directives.insert(_directives.begin(), begin, end);
    switchDirective(_directives.front());
}

Assembler &AsmCommonDirective::Directives::assembler() const {
    return current()->assembler();
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

std::list<std::string> AsmCommonDirective::Directives::listCpu() const {
    std::list<std::string> list;
    for (auto dir : _directives) {
        const char *list_P = dir->assembler().listCpu();
        char buffer[strlen_P(list_P) + 1];
        strcpy_P(buffer, list_P);
        filter(buffer, list);
    }
    return list;
}

std::string AsmCommonDirective::Directives::currentCpu() const {
    const char *cpu_P = assembler().getCpu();
    char buffer[strlen_P(cpu_P) + 1];
    strcpy_P(buffer, cpu_P);
    return std::string(buffer);
}

bool AsmCommonDirective::Directives::is8080() const {
    const std::string cpu(currentCpu());
    return cpu == "8080" || cpu == "8085";
}

AsmDirective *AsmCommonDirective::Directives::setCpu(const char *cpu) {
    for (auto dir : _directives) {
        if (dir->assembler().setCpu(cpu))
            return switchDirective(dir);
    }
    return nullptr;
}

AsmDirective *AsmCommonDirective::Directives::restrictCpu(const char *cpu) {
    AsmDirective *z80 = is8080() ? setCpu("Z80") : nullptr;
    AsmDirective *dir = setCpu(cpu);
    if (dir) {
        _directives.clear();
        _directives.push_back(dir);
        if (z80)
            _directives.push_back(z80);
    }
    return dir;
}

AsmDirective *AsmCommonDirective::Directives::switchDirective(AsmDirective *dir) {
    _current = dir;
    assembler().reset();
    return dir;
}

AsmCommonDirective::AsmCommonDirective(AsmDirective **begin, AsmDirective **end)
    : _directives(begin, end) {
    _line_len = 128;
    _line = static_cast<char *>(malloc(_line_len));
    _origin = 0;
    _reportUndef = true;
    _reportDuplicate = true;
    _labelWidth = 16;
    _operandWidth = 16;

    registerPseudo(".cpu", &AsmCommonDirective::switchCpu);
    registerPseudo(".include", &AsmCommonDirective::includeFile);
    // TODO: implement listing after "end".
    registerPseudo(".end", &AsmCommonDirective::endAssemble);
    registerPseudo(".equ", &AsmCommonDirective::defineLabel);
    registerPseudo(":=", &AsmCommonDirective::defineLabel);
    registerPseudo("=", &AsmCommonDirective::defineLabel);
    registerPseudo(".org", &AsmCommonDirective::defineOrigin);
    registerPseudo(".align", &AsmCommonDirective::alignOrigin);
    registerPseudo(".string", &AsmCommonDirective::defineString);
    registerPseudo(".ascii", &AsmCommonDirective::defineString);
    registerPseudo(".byte", &AsmCommonDirective::defineUint8s);
    registerPseudo(".word", &AsmCommonDirective::defineUint16s);
    registerPseudo(".long", &AsmCommonDirective::defineUint32s);
    registerPseudo("dc.b", &AsmCommonDirective::defineUint8s);
    registerPseudo("dc.w", &AsmCommonDirective::defineUint16s);
    registerPseudo("dc.l", &AsmCommonDirective::defineUint32s);
    registerPseudo("ds.b", &AsmCommonDirective::allocateUint8s);
    registerPseudo("ds.w", &AsmCommonDirective::allocateUint16s);
    registerPseudo("ds.l", &AsmCommonDirective::allocateUint32s);
    registerPseudo(".z80syntax", &AsmCommonDirective::switchIntelZilog);
}

AsmCommonDirective::~AsmCommonDirective() {
    free(_line);
}

std::string AsmCommonDirective::listCpu(const char *separator) const {
    std::list<std::string> list(_directives.listCpu());
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

Error AsmCommonDirective::assembleLine(const char *line, CliMemory &memory) {
    if (line == nullptr)
        return OK;
    StrScanner scan(line);

    _list.line_number = currentLineno();
    _list.include_nest = _sources.size() - 1;
    _list.address = _origin;
    _list.length = 0;
    _list.value.clear();
    _list.memory = &memory;
    _list.label = StrScanner::EMPTY;
    _list.instruction = StrScanner::EMPTY;
    _list.operand = StrScanner::EMPTY;
    _list.comment = StrScanner::EMPTY;

    ValueParser &parser = _directives.parser();
    parser.setCurrentOrigin(_origin);

    StrScanner label;
    if (parser.isSymbolLetter(*scan, true)) {
        setAt(scan);
        _list.label = scan;
        label = parser.readSymbol(scan);
        scan.expect(':');  // skip optional trailing ':' for label.
        _list.label.trimEndAt(scan);
    }
    scan.skipSpaces();

    if (!_directives.assembler().endOfLine(*scan)) {
        StrScanner p(scan);
        p.trimStart([](char s) { return !isspace(s); });
        _list.instruction = scan;
        _list.instruction.trimEndAt(p);
        _list.operand = p.skipSpaces();
        const uint32_t origin = _origin;
        const Error error = processPseudo(_list.instruction, p, label, memory);
        if (error == OK) {
            _list.operand.trimEndAt(p).trimEnd(isspace);
            _list.comment = p.skipSpaces();
            if (label.size()) {
                // If |label| isn't consumed, assign the origin.
                internSymbol(origin, label);
            }
            return getError();
        }
        if (error != UNKNOWN_DIRECTIVE)
            return error;
    }

    if (label.size()) {
        // If |label| isn't consumed, assign the origin.
        internSymbol(_origin, label);
        if (getError())
            return getError();
    }

    if (_directives.assembler().endOfLine(*scan)) {
        _list.comment = scan;
        return setError(OK);  // skip comment
    }

    Insn insn(_origin);
    const Error error = _directives.assembler().encode(scan, insn, this);
    const bool allowUndef = !_reportUndef && error == UNDEFINED_SYMBOL;
    if (error == OK || allowUndef) {
        StrScanner p(_directives.assembler().errorAt());
        _list.operand.trimEndAt(p).trimEnd(isspace);
        _list.comment = p.skipSpaces();
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
    current()->assembler().reset();
    _origin = 0;
}

void AsmCommonDirective::setSymbolMode(bool reportUndef, bool reportDuplicate) {
    _reportUndef = reportUndef;
    _reportDuplicate = reportDuplicate;
}

struct AsmCommonDirective::Source {
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

Error AsmCommonDirective::openSource(const StrScanner &input_name) {
    if (_sources.size() >= max_includes)
        return setError(TOO_MANY_INCLUDE);
    const Source *parent = _sources.empty() ? nullptr : _sources.back();
    const size_t pos = parent ? parent->name.find_last_of('/') : std::string::npos;
    Source *source;
    if (pos == std::string::npos || *input_name == '/') {
        source = new Source(std::string(input_name, input_name.size()), parent);
    } else {
        std::string name(parent->name.c_str(), pos + 1);
        name.append(input_name, input_name.size());
        source = new Source(name, parent);
    }
    _sources.push_back(source);
    if ((source->fp = fopen(source->name.c_str(), "r")) == nullptr) {
        _sources.pop_back();
        return setError(NO_INCLUDE_FOUND);
    }
    return setError(OK);
}

StrScanner *AsmCommonDirective::readSourceLine() {
    while (!_sources.empty()) {
        Source *source = _sources.back();
        source->lineno++;
        if (getLine(_line, _line_len, source->fp) >= 0) {
            _line_scan = _line;
            return &_line_scan;
        }
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

void AsmCommonDirective::registerPseudo(const char *name, PseudoHandler handler) {
    _pseudos.emplace(std::make_pair(std::string(name), handler));
    if (*name++ == '.')
        _pseudos.emplace(std::make_pair(std::string(name), handler));
}

Error AsmCommonDirective::processPseudo(
        const StrScanner &name, StrScanner &scan, StrScanner &label, CliMemory &memory) {
    if (_directives.current()->processPseudo(name, *this, scan, label, memory) != UNKNOWN_DIRECTIVE)
        return getError();
    auto it = _pseudos.find(std::string(name, name.size()));
    if (it != _pseudos.end())
        return (this->*it->second)(scan, label, memory);
    return setError(UNKNOWN_DIRECTIVE);
}

// PseudoHandler

Error AsmCommonDirective::defineOrigin(StrScanner &scan, StrScanner &label, CliMemory &memory) {
    ValueParser &parser = _directives.parser();
    Value value = parser.eval(scan, this);
    if (setError(parser))
        return getError();
    if (_reportUndef && value.isUndefined())
        return setError(UNDEFINED_SYMBOL);
    // TODO line end check
    _list.address = _origin = value.getUnsigned();
    return setError(OK);
}

Error AsmCommonDirective::alignOrigin(StrScanner &scan, StrScanner &label, CliMemory &memory) {
    ValueParser &parser = _directives.parser();
    Value value = parser.eval(scan, this);
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

Error AsmCommonDirective::defineLabel(StrScanner &scan, StrScanner &label, CliMemory &memory) {
    if (label.size() == 0)
        return setError(MISSING_LABEL);
    if (_reportDuplicate && hasSymbol(label))
        return setError(DUPLICATE_LABEL);
    ValueParser &parser = _directives.parser();
    _list.value = parser.eval(scan, this);
    if (setError(parser))
        return getError();
    if (_reportUndef && _list.value.isUndefined())
        return setError(UNDEFINED_SYMBOL);
    // TODO line end check
    internSymbol(_list.value.getUnsigned(), label);
    label = StrScanner::EMPTY;
    return getError();
}

Error AsmCommonDirective::includeFile(StrScanner &scan, StrScanner &label, CliMemory &memory) {
    char quote = scan.expect('"');
    if (quote == 0)
        quote = scan.expect('\'');
    StrScanner filename(scan);
    StrScanner p(scan);
    if (quote) {
        p.trimStart([quote](char s) { return s != quote; });
        if (*p != quote)
            return setError(p, quote == '"' ? MISSING_CLOSING_DQUOTE : MISSING_CLOSING_QUOTE);
    } else {
        p.trimStart([](char s) { return !isspace(s); });
    }
    scan = p;
    return openSource(filename.trimEndAt(p));
}

Error AsmCommonDirective::defineUint8s(StrScanner &scan, StrScanner &label, CliMemory &memory) {
    return defineUint8s(scan, memory, false);
}

Error AsmCommonDirective::defineString(StrScanner &scan, StrScanner &label, CliMemory &memory) {
    return defineUint8s(scan, memory, true);
}

Error AsmCommonDirective::defineUint8s(StrScanner &scan, CliMemory &memory, bool terminator) {
    _list.address = _origin;
    ValueParser &parser = _directives.parser();
    const uint32_t base = _origin * addrUnit();
    auto &len = _list.length;
    len = 0;
    for (;;) {
        scan.skipSpaces();
        if (terminator || *scan == '"') {
            const char delim = *scan++;
            StrScanner p(scan);
            for (;;) {
                if (p.expect(delim))
                    break;
                if (*p == 0)
                    return setError(p, MISSING_CLOSING_DQUOTE);
                const char c = parser.readChar(p);
                if (setError(parser)) {
                    scan = p;
                    return getError();
                }
                memory.writeByte(base + len, c);
                len++;
            }
            scan = p;
        } else {
            Value value = parser.eval(scan, this);
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
        if (!scan.skipSpaces().expect(','))
            break;
    }
    _origin += ((len + addrUnit() - 1) & -addrUnit()) / addrUnit();
    return setError(OK);
}

Error AsmCommonDirective::defineUint16s(StrScanner &scan, StrScanner &label, CliMemory &memory) {
    _list.address = _origin;
    ValueParser &parser = _directives.parser();
    const uint32_t base = _origin * addrUnit();
    auto &len = _list.length;
    len = 0;
    const uint8_t hi = uint8_t(config().endian());
    const uint8_t lo = 1 - hi;
    for (;;) {
        Value value = parser.eval(scan, this);
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
        if (!scan.skipSpaces().expect(','))
            break;
    }
    _origin += ((len + addrUnit() - 1) & -addrUnit()) / addrUnit();
    return setError(OK);
}

Error AsmCommonDirective::defineUint32s(StrScanner &scan, StrScanner &label, CliMemory &memory) {
    _list.address = _origin;
    ValueParser &parser = _directives.parser();
    const uint32_t base = _origin * addrUnit();
    auto &len = _list.length;
    len = 0;
    for (;;) {
        Value value = parser.eval(scan, this);
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
        if (!scan.skipSpaces().expect(','))
            break;
    }
    _origin += ((len + addrUnit() - 1) & -addrUnit()) / addrUnit();
    return setError(OK);
}

Error AsmCommonDirective::allocateUint8s(StrScanner &scan, StrScanner &label, CliMemory &memory) {
    return allocateSpaces(scan, sizeof(uint8_t));
}

Error AsmCommonDirective::allocateUint16s(StrScanner &scan, StrScanner &label, CliMemory &memory) {
    return allocateSpaces(scan, sizeof(uint16_t));
}

Error AsmCommonDirective::allocateUint32s(StrScanner &scan, StrScanner &label, CliMemory &memory) {
    return allocateSpaces(scan, sizeof(uint32_t));
}

Error AsmCommonDirective::allocateSpaces(StrScanner &scan, size_t unit) {
    ValueParser &parser = _directives.parser();
    Value value = parser.eval(scan, this);
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

Error AsmCommonDirective::switchCpu(StrScanner &scan, StrScanner &label, CliMemory &memory) {
    StrScanner p(scan);
    p.trimStart([](char s) { return !isspace(s); });
    scan.trimEndAt(p);
    std::string cpu(scan, scan.size());
    if (_directives.setCpu(cpu.c_str()) == nullptr)
        return setError(UNSUPPORTED_CPU);
    scan = p;
    return setError(OK);
}

Error AsmCommonDirective::switchIntelZilog(StrScanner &scan, StrScanner &label, CliMemory &memory) {
    if (!_directives.is8080())
        return setError(UNKNOWN_DIRECTIVE);
    const std::string currentCpu(_directives.currentCpu());
    StrScanner option = _directives.parser().readSymbol(scan);
    if (option.iequals_P(PSTR("on"))) {
        if (_directives.setCpu("z80") == nullptr)
            return setError(UNSUPPORTED_CPU);
        _directives.assembler().setCpu(currentCpu.c_str());
    } else if (option.iequals_P(PSTR("off"))) {
        if (_directives.setCpu("8080") == nullptr)
            return setError(UNSUPPORTED_CPU);
        _directives.assembler().setCpu(currentCpu.c_str());
    } else
        return setError(option, UNKNOWN_OPERAND);
    return setError(OK);
}

Error AsmCommonDirective::endAssemble(StrScanner &scan, StrScanner &label, CliMemory &memory) {
    return closeSource();
}

const char *AsmCommonDirective::lookupValue(uint32_t address) const {
    return nullptr;
}

bool AsmCommonDirective::hasSymbol(const StrScanner &symbol) const {
    return symbolExists(std::string(symbol, symbol.size()));
}

uint32_t AsmCommonDirective::lookupSymbol(const StrScanner &symbol) const {
    return symbolLookup(std::string(symbol, symbol.size()));
}

Error AsmCommonDirective::internSymbol(uint32_t value, const StrScanner &symbol) {
    return symbolIntern(value, std::string(symbol, symbol.size()));
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
    return _list.instruction.size() != 0;
}

std::string AsmCommonDirective::getInstruction() const {
    return std::string(_list.instruction, _list.instruction.size());
}

bool AsmCommonDirective::hasOperand() const {
    return _list.operand.size() != 0;
}

std::string AsmCommonDirective::getOperand() const {
    return std::string(_list.operand, _list.operand.size());
}

uint16_t AsmCommonDirective::lineNumber() const {
    return _list.line_number;
}

uint16_t AsmCommonDirective::includeNest() const {
    return _list.include_nest;
}

bool AsmCommonDirective::hasValue() const {
    return !_list.value.isUndefined();
}

uint32_t AsmCommonDirective::value() const {
    return _list.value.getUnsigned();
}

bool AsmCommonDirective::hasLabel() const {
    return _list.label.size() != 0;
}

std::string AsmCommonDirective::getLabel() const {
    return std::string(_list.label, _list.label.size());
}

bool AsmCommonDirective::hasComment() const {
    return _list.comment.size() != 0;
}

std::string AsmCommonDirective::getComment() const {
    return std::string(_list.comment, _list.comment.size());
}

const ConfigBase &AsmCommonDirective::config() const {
    return _directives.assembler().config();
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

void AsmDirective::registerPseudo(const char *name, AsmCommonDirective::PseudoHandler handler) {
    _pseudos.emplace(std::make_pair(std::string(name), handler));
    if (*name++ == '.')
        _pseudos.emplace(std::make_pair(std::string(name), handler));
}

Error AsmDirective::processPseudo(const StrScanner &name, AsmCommonDirective &common,
        StrScanner &scan, StrScanner &label, CliMemory &memory) const {
    auto it = _pseudos.find(std::string(name, name.size()));
    return it == _pseudos.end() ? UNKNOWN_DIRECTIVE : (common.*it->second)(scan, label, memory);
}

MotorolaDirective::MotorolaDirective(Assembler &assembler)
    : AsmDirective(assembler), _formatter(assembler.config().addressWidth()) {
    registerPseudo(".fcb", &AsmCommonDirective::defineUint8s);
    registerPseudo(".fcc", &AsmCommonDirective::defineString);
    registerPseudo(".fdb", &AsmCommonDirective::defineUint16s);
    registerPseudo(".fdb", &AsmCommonDirective::defineUint16s);
    registerPseudo(".rmb", &AsmCommonDirective::allocateUint8s);
    registerPseudo(".dfs", &AsmCommonDirective::allocateUint8s);
}

IntelDirective::IntelDirective(Assembler &assembler)
    : AsmDirective(assembler), _formatter(assembler.config().addressWidth()) {
    registerPseudo(".db", &AsmCommonDirective::defineUint8s);
    registerPseudo(".dw", &AsmCommonDirective::defineUint16s);
    registerPseudo(".dd", &AsmCommonDirective::defineUint32s);
    registerPseudo(".ds", &AsmCommonDirective::allocateUint8s);
}

NationalDirective::NationalDirective(Assembler &assembler) : IntelDirective(assembler) {
    registerPseudo(".dbyte", &AsmCommonDirective::defineUint16s);
}

}  // namespace cli
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
