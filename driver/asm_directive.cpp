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
#include "intel_hex.h"
#include "moto_srec.h"
#include "str_scanner.h"

#include <algorithm>
#include <cstring>

namespace libasm {
namespace driver {

bool AsmDirective::is8080(const /* PROGMEM */ char *cpu_P) {
    return strcmp_P("8080", cpu_P) == 0 || strcmp_P("8085", cpu_P) == 0 ||
           strcasecmp_P("V30EMU", cpu_P) == 0;
}

// PseudoHandler

Error AsmDirective::defineConstant(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    return defineSymbol(scan, list, driver, list.lineSymbol(), /*variable*/ false);
}

Error AsmDirective::defineVariable(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    return defineSymbol(scan, list, driver, list.lineSymbol(), /*variable*/ true);
}

Error AsmDirective::setVariable(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    if (list.lineSymbol().size())
        return setError(ILLEGAL_LABEL);
    StrScanner symbol;
    if (assembler().parser().readSymbol(scan, symbol) != OK)
        return setError(MISSING_LABEL);
    if (scan.skipSpaces().expect(','))
        return defineSymbol(scan.skipSpaces(), list, driver, symbol, /*variable*/ true);
    return setError(MISSING_COMMA);
}

Error AsmDirective::defineSymbol(StrScanner &scan, AsmFormatter &list, AsmDriver &driver,
        const StrScanner &symbol, bool variable) {
    if (symbol.size() == 0)
        return setError(MISSING_LABEL);
    if (driver.symbolMode() == REPORT_DUPLICATE && driver.hasFunction(symbol))
        return setError(symbol, DUPLICATE_LABEL);
    auto &parser = assembler().parser();
    ErrorAt error;
    auto &value = list.lineValue() = parser.eval(scan, error, &driver);
    if (error.getError()) {
        value.clear();
        return setError(error);
    }
    if (value.isUndefined() && driver.symbolMode() == REPORT_UNDEFINED)
        return setError(symbol, UNDEFINED_SYMBOL);
    // TODO line end check
    const auto err = driver.internSymbol(value.getUnsigned(), symbol, variable);
    if (err) {
        setError(symbol, err);
        value.clear();
    }
    list.lineSymbol() = StrScanner::EMPTY;
    return getError();
}

Error AsmDirective::includeFile(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
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
    scan = p;
    return setError(driver.openSource(filename));
}

Error AsmDirective::defineFunction(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    if (list.lineSymbol().size() == 0)
        return setError(MISSING_LABEL);
    if (driver.symbolMode() == REPORT_DUPLICATE && driver.symbolInTable(list.lineSymbol()))
        return setError(DUPLICATE_LABEL);
    const auto &parser = assembler().parser();
    FunctionStore::Parameters params;
    for (;;) {
        auto p = scan.skipSpaces();
        StrScanner param;
        if (parser.readSymbol(p, param) == OK && p.skipSpaces().expect(',')) {
            params.emplace_back(param);
            scan = p;
        } else {
            const auto error = driver.internFunction(list.lineSymbol(), params, scan, parser);
            list.lineSymbol() = StrScanner::EMPTY;
            return error;
        }
    }
}

Error AsmDirective::switchCpu(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    auto name = scan;
    scan = name.takeWhile([](char s) { return !isspace(s); });
    std::string cpu(name.str(), name.size());
    if (driver.setCpu(cpu.c_str()) == nullptr)
        return setError(UNSUPPORTED_CPU);
    return setOK();
}

Error AsmDirective::switchIntelZilog(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    const /* PROGMEM */ char *cpu_P = assembler().cpu_P();
    if (!is8080(cpu_P))
        return setError(UNKNOWN_DIRECTIVE);
    char cpu[strlen_P(cpu_P) + 1];
    strcpy_P(cpu, cpu_P);
    StrScanner option;
    assembler().parser().readSymbol(scan, option);
    if (option.iequals_P(PSTR("on"))) {
        auto zilog = driver.setCpu("Z80");
        if (zilog == nullptr)
            return setError(UNSUPPORTED_CPU);
        zilog->assembler().setCpu(cpu);
    } else if (option.iequals_P(PSTR("off"))) {
        auto intel = driver.setCpu("8080");
        if (intel == nullptr)
            return setError(UNSUPPORTED_CPU);
        intel->assembler().setCpu(cpu);
    } else
        return setError(option, UNKNOWN_OPERAND);
    return setOK();
}

Error AsmDirective::endAssemble(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    return END_ASSEMBLE;
}

AsmDirective::AsmDirective(Assembler &a) : ErrorAt(), _assembler(a) {
    registerPseudo(".cpu", &AsmDirective::switchCpu);
    registerPseudo(".include", &AsmDirective::includeFile);
    // TODO: implement listing after "end".
    registerPseudo(".end", &AsmDirective::endAssemble);
    registerPseudo(".equ", &AsmDirective::defineConstant);
    registerPseudo(".set", &AsmDirective::defineVariable);
    registerPseudo(".function", &AsmDirective::defineFunction);
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

Error AsmDirective::processPseudo(
        const StrScanner &name, StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    auto it = _pseudos.find(std::string(name.str(), name.size()));
    if (it == _pseudos.end())
        return UNKNOWN_DIRECTIVE;
    const auto fp = it->second;
    return (this->*fp)(scan, list, driver);
}

BinEncoder &AsmDirective::defaultEncoder() const {
    return IntelHex::encoder();
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
