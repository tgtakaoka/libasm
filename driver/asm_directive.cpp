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

#include "asm_driver.h"
#include "asm_sources.h"
#include "intel_hex.h"
#include "moto_srec.h"
#include "str_scanner.h"

#include <algorithm>
#include <cstring>

namespace libasm {
namespace driver {

AsmDirective::AsmDirective(Assembler &a) : ErrorAt(), _assembler(a) {
    registerPseudo(".cpu", &AsmDirective::switchCpu);
    registerPseudo(".include", &AsmDirective::includeFile);
    // TODO: implement listing after "end".
    registerPseudo(".end", &AsmDirective::endAssemble);
    registerPseudo(".equ", &AsmDirective::defineConstant);
    registerPseudo(".set", &AsmDirective::defineVariable);
    registerPseudo(".function", &AsmDirective::defineFunction);
}

bool AsmDirective::setCpu(const char *cpu) {
    return _assembler.setCpu(cpu);
}

const /*PROGMEM*/ char *AsmDirective::cpu_P() const {
    return _assembler.config().cpu_P();
}

const /*PROGMEM*/ char *AsmDirective::listCpu_P() const {
    return _assembler.config().listCpu_P();
}

void AsmDirective::reset(CpuSwitcher &switcher) {
    _assembler.reset();
    _switcher = &switcher;
}

void AsmDirective::setOption(const char *name, const char *value) {
    _assembler.setOption(name, value);
}

const Options &AsmDirective::commonOptions() const {
    return _assembler.commonOptions();
}

const Options &AsmDirective::options() const {
    return _assembler.options();
}

const ConfigBase &AsmDirective::config() const {
    return _assembler.config();
}

Radix AsmDirective::listRadix() const {
    return _assembler.listRadix();
}

uint32_t AsmDirective::currentLocation() const {
    return _assembler.currentLocation();
}

void AsmDirective::setListRadix(Radix listRadix) {
    _assembler.setListRadix(listRadix);
}

Error AsmDirective::encode(StrScanner &scan, Insn &insn, Context &context) {
    setOK();
    _assembler.setCurrentLocation(insn.address());

    const auto &parser = _assembler.parser();
    auto p = scan;
    if (parser.commentLine(p))
        return OK;

    auto error = OK;
    context.value.clear();
    parser.readLabel(scan, context.label);
    if (!parser.endOfLine(scan.skipSpaces())) {
        auto p = scan;
        StrScanner directive;
        parser.readInstruction(p, directive);
        error = processPseudo(directive, p.skipSpaces(), context);
        if (error == OK && !parser.endOfLine(p))
            setError(p, GARBAGE_AT_END);
    }
    if (context.label.size()) {
        if (context.reportDuplicate && context.symbols.hasSymbol(context.label)) {
            setErrorIf(context.label, DUPLICATE_LABEL);
        } else {
            Value addr;
            addr.setUnsigned(insn.address());
            context.symbols.internSymbol(addr, context.label);
        }
    }
    if (error != UNKNOWN_DIRECTIVE)
        return getError();

    if (parser.endOfLine(scan))
        return setOK();  // skip comment

    _assembler.encode(scan.str(), insn, &context.symbols);
    setError(insn);

    error = getError();
    const auto allowUndef = !context.reportUndefined && error == UNDEFINED_SYMBOL;
    if (error == OK || allowUndef)
        setOK();
    return getError();
}

void AsmDirective::registerPseudo(const char *name, PseudoHandler handler) {
    _pseudos.emplace(std::make_pair(std::string(name), handler));
    if (*name++ == '.')
        _pseudos.emplace(std::make_pair(std::string(name), handler));
}

void AsmDirective::disablePseudo(const char *name) {
    _pseudos.erase(std::string(name));
    if (*name++ == '.')
        _pseudos.erase(std::string(name));
}

Error AsmDirective::processPseudo(const StrScanner &name, StrScanner &scan, Context &context) {
    auto it = _pseudos.find(std::string(name.str(), name.size()));
    if (it == _pseudos.end())
        return UNKNOWN_DIRECTIVE;
    const auto fp = it->second;
    return (this->*fp)(scan, context);
}

BinEncoder &AsmDirective::defaultEncoder() const {
    return IntelHex::encoder();
}

// PseudoHandler

Error AsmDirective::defineConstant(StrScanner &scan, Context &context) {
    defineSymbol(scan, context.label, context, /*variable*/ false);
    context.label.empty();
    return getError();
}

Error AsmDirective::defineVariable(StrScanner &scan, Context &context) {
    defineSymbol(scan, context.label, context, /*variable*/ true);
    context.label.empty();
    return getError();
}

Error AsmDirective::setVariable(StrScanner &scan, Context &context) {
    if (context.label.size())
        return setError(ILLEGAL_LABEL);
    auto p = scan;
    StrScanner symbol;
    if (_assembler.parser().readSymbol(p, symbol) != OK)
        return setError(p, MISSING_LABEL);
    if (!p.skipSpaces().expect(','))
        return setError(p, MISSING_COMMA);
    defineSymbol(p.skipSpaces(), symbol, context, /*variable*/ true);
    scan = p;
    return getError();
}

Error AsmDirective::defineSymbol(
        StrScanner &scan, const StrScanner &symbol, Context &context, bool variable) {
    if (context.reportDuplicate) {
        const auto &symbols = context.symbols;
        if (!variable && symbols.hasValue(symbol, false))
            return setError(symbol, DUPLICATE_LABEL);
        if (symbols.hasValue(symbol, !variable))
            return setError(symbol, DUPLICATE_LABEL);
        if (symbols.hasFunction(symbol))
            return setError(symbol, DUPLICATE_LABEL);
    }

    auto &parser = _assembler.parser();
    ErrorAt error;
    context.value = parser.eval(scan, error, &context.symbols);
    if (error.hasError()) {
        context.value.clear();
        return setError(scan, error);
    }
    if (context.reportUndefined && error.getError() == UNDEFINED_SYMBOL)
        return setError(error);

    setErrorIf(symbol, context.symbols.internSymbol(context.value, symbol, variable));
    return getError();
}

Error AsmDirective::includeFile(StrScanner &scan, Context &context) {
    auto quote = scan.expect('"');
    if (quote == 0)
        quote = scan.expect('\'');
    auto filename = scan;
    auto p = scan;
    if (quote) {
        p = filename.takeWhile([quote](char s) { return s != quote; });
        if (!p.expect(quote))
            return setError(p, quote == '"' ? MISSING_CLOSING_DQUOTE : MISSING_CLOSING_QUOTE);
    } else {
        p = filename.takeWhile([](char s) { return !isspace(s); });
    }
    const auto error = context.sources.open(filename);
    if (error)
        setErrorIf(filename, error);
    scan = p;
    return error;
}

Error AsmDirective::defineFunction(StrScanner &scan, Context &context) {
    auto &name = context.label;
    if (name.size() == 0)
        return setError(MISSING_LABEL);

    const auto &parser = _assembler.parser();
    FunctionStore::Parameters params;
    for (;;) {
        auto p = scan.skipSpaces();
        StrScanner param;
        if (parser.readSymbol(p, param) == OK && p.skipSpaces().expect(',')) {
            params.emplace_back(param);
            scan = p;
        } else {
            auto error = OK;
            if (context.reportDuplicate && context.symbols.hasSymbol(name)) {
                error = DUPLICATE_LABEL;
            } else {
                error = context.symbols.internFunction(name, params, scan, parser);
                scan.empty();  // whole body is interned
            }
            if (error)
                setErrorIf(name, error);
            name.empty();
            return getError();
        }
    }
}

Error AsmDirective::switchCpu(StrScanner &scan, Context &context) {
    auto name = scan;
    scan = name.takeWhile([](char s) { return !isspace(s); });
    std::string cpu(name.str(), name.size());
    if (!_switcher->setCpu(cpu.c_str()))
        return setError(UNSUPPORTED_CPU);
    return setOK();
}

bool AsmDirective::is8080(const /* PROGMEM */ char *cpu_P) {
    return strcmp_P("8080", cpu_P) == 0 || strcmp_P("8085", cpu_P) == 0 ||
           strcasecmp_P("V30EMU", cpu_P) == 0;
}

Error AsmDirective::switchIntelZilog(StrScanner &scan, Context &context) {
    const /* PROGMEM */ char *cpu_P = _assembler.config().cpu_P();
    if (!is8080(cpu_P))
        return setError(UNKNOWN_DIRECTIVE);
    char buffer[20];
    StrBuffer cpu{buffer, sizeof(buffer)};
    cpu.text_P(cpu_P);
    StrScanner option;
    _assembler.parser().readSymbol(scan, option);
    if (option.iequals_P(PSTR("on"))) {
        if (!_switcher->setCpu("Z80"))
            return setError(UNSUPPORTED_CPU);
        cpu.text("zilog");
        _switcher->setCpu(cpu.str());
    } else if (option.iequals_P(PSTR("off"))) {
        if (!_switcher->setCpu("8080"))
            return setError(UNSUPPORTED_CPU);
        _switcher->setCpu(cpu.str());
    } else
        return setError(option, UNKNOWN_OPERAND);
    return setOK();
}

Error AsmDirective::endAssemble(StrScanner &scan, Context &context) {
    return END_ASSEMBLE;
}

MotorolaDirective::MotorolaDirective(Assembler &assembler) : AsmDirective(assembler) {}

BinEncoder &MotorolaDirective::defaultEncoder() const {
    return MotoSrec::encoder();
}

IntelDirective::IntelDirective(Assembler &assembler) : AsmDirective(assembler) {
    registerPseudo(".z80syntax", &IntelDirective::switchIntelZilog);
}

MostekDirective::MostekDirective(Assembler &assembler) : MotorolaDirective(assembler) {
    registerPseudo("=", &MostekDirective::defineConstant);
    registerPseudo(":=", &MostekDirective::defineConstant);
}

Z8Directive::Z8Directive(Assembler &assembler) : IntelDirective(assembler) {
    disablePseudo(".set");
    registerPseudo("var", &Z8Directive::setVariable);
    registerPseudo(".set", &Z8Directive::setVariable);
}

ZilogDirective::ZilogDirective(Assembler &assembler) : IntelDirective(assembler) {
    disablePseudo(".set");
    registerPseudo("defl", &ZilogDirective::defineVariable);
    registerPseudo("var", &ZilogDirective::defineVariable);
}

RcaDirective::RcaDirective(Assembler &assembler) : IntelDirective(assembler) {
    registerPseudo("=", &RcaDirective::defineConstant);
}

NationalDirective::NationalDirective(Assembler &assembler) : IntelDirective(assembler) {
    disablePseudo(".set");
    registerPseudo("=", &NationalDirective::defineConstant);
    registerPseudo(".set", &NationalDirective::setVariable);
}

FairchildDirective::FairchildDirective(Assembler &assembler) : AsmDirective(assembler) {}

DecDirective::DecDirective(Assembler &assembler) : AsmDirective(assembler) {
    registerPseudo("=", &DecDirective::defineConstant);
}

}  // namespace driver
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
