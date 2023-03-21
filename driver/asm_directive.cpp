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

Error AsmDirective::defineOrigin(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    const auto line = scan;
    auto &parser = assembler().parser();
    ErrorAt error;
    auto value = parser.eval(scan, error, &driver);
    // TODO line end check
    if (error.getError())
        return setError(error);
    if (value.isUndefined() && driver.symbolMode() == REPORT_UNDEFINED)
        return setError(UNDEFINED_SYMBOL);
    setAt(line);
    if (assembler().checkAddress(value.getUnsigned(), *this))
        return setError(assembler());
    list.setStartAddress(driver.setOrigin(value.getUnsigned()));
    return setOK();
}

Error AsmDirective::setAlignment(uint32_t alignment, AsmFormatter &list, AsmDriver &driver) {
    const auto addr = (driver.origin() + (alignment - 1)) & ~(alignment - 1);
    if (assembler().checkAddress(addr, *this))
        return setError(assembler());
    driver.setOrigin(addr);
    list.setStartAddress(driver.origin());
    return setOK();
}

Error AsmDirective::alignOrigin(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    const auto line = scan;
    auto &parser = assembler().parser();
    ErrorAt error;
    auto value = parser.eval(scan, error, &driver);
    // TODO line end check
    if (error.getError())
        return setError(error);
    if (value.isUndefined() && driver.symbolMode() == REPORT_UNDEFINED)
        return setError(UNDEFINED_SYMBOL);
    if (value.overflowUint16())
        return setError(OVERFLOW_RANGE);
    setAt(line);
    const auto alignment = value.getUnsigned();
    if (alignment > 0x1000)
        setError(ILLEGAL_OPERAND);
    return setAlignment(alignment, list, driver);
}

Error AsmDirective::defineConstant(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    return defineSymbol(scan, list, driver, /*variable*/ false);
}

Error AsmDirective::defineVariable(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    return defineSymbol(scan, list, driver, /*variable*/ true);
}

Error AsmDirective::defineSymbol(
        StrScanner &scan, AsmFormatter &list, AsmDriver &driver, bool variable) {
    if (list.lineSymbol().size() == 0)
        return setError(MISSING_LABEL);
    auto &parser = assembler().parser();
    ErrorAt error;
    auto &value = list.lineValue() = parser.eval(scan, error, &driver);
    if (error.getError()) {
        value.clear();
        return setError(error);
    }
    if (value.isUndefined() && driver.symbolMode() == REPORT_UNDEFINED)
        return setError(UNDEFINED_SYMBOL);
    // TODO line end check
    const auto err = driver.internSymbol(value.getUnsigned(), list.lineSymbol(), variable);
    if (err) {
        setError(list.lineSymbol(), err);
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
        p.trimStart([quote](char s) { return s != quote; });
        filename.trimEndAt(p);
        if (!p.expect(quote))
            return setError(p, quote == '"' ? MISSING_CLOSING_DQUOTE : MISSING_CLOSING_QUOTE);
    } else {
        p.trimStart([](char s) { return !isspace(s); });
        filename.trimEndAt(p);
    }
    scan = p;
    return setError(driver.openSource(filename));
}

Error MotorolaDirective::defineString(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    auto &parser = assembler().parser();
    const uint8_t unit = assembler().config().addressUnit();
    const uint32_t base = driver.origin() * unit;
    do {
        const auto delim = *scan.skipSpaces()++;
        auto p = scan;
        while (!p.expect(delim)) {
            if (*p == 0)
                return setError(p, MISSING_CLOSING_DELIMITOR);
            ErrorAt error;
            const auto c = parser.readChar(p, error);
            if (error.getError()) {
                scan = p;
                return setError(error);
            }
            list.emitByte(base, c);
        }
        scan = p;
    } while (scan.skipSpaces().expect(','));
    scan.skipSpaces();
    if (!assembler().endOfLine(scan))
        return setError(scan, GARBAGE_AT_END);
    driver.setOrigin(driver.origin() + ((list.byteLength() + unit - 1) & -unit) / unit);
    return setOK();
}

Error AsmDirective::defineUint8s(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    auto &parser = assembler().parser();
    const uint8_t unit = assembler().config().addressUnit();
    const uint32_t base = driver.origin() * unit;
    do {
        auto p = scan.skipSpaces();
        ErrorAt error;
        auto value = parser.eval(p, error, &driver);
        if (error.isOK() && !(*scan == '\'' && *p == '\'')) {
            // a byte expression, though a single 'c' constant is handled as a string
            scan = p;
            if (value.isUndefined() && driver.symbolMode() == REPORT_UNDEFINED)
                return setError(UNDEFINED_SYMBOL);
            if (value.overflowUint8())
                return setError(OVERFLOW_RANGE);
            const uint8_t val8 = value.getUnsigned();
            list.emitByte(base, val8);
        } else if (*scan == '"' || *scan == '\'') {
            // a string surrounded by double quotes, single quotes
            const auto delim = *scan++;
            auto p = scan;
            for (;;) {
                if (*p == '\'' && p[1] == '\'') {
                    ++p;  // inside a string, two successive apostrophe is an apostrophe
                } else if (p.expect(delim)) {
                    break;
                }
                if (*p == 0)
                    return setError(
                            p, delim == '"' ? MISSING_CLOSING_DQUOTE : MISSING_CLOSING_QUOTE);
                ErrorAt error;
                const auto c = parser.readChar(p, error);
                if (error.getError()) {
                    scan = p;
                    return setError(error);
                }
                list.emitByte(base, c);
            }
            scan = p;
        } else {
            return setError(error);
        }
    } while (scan.skipSpaces().expect(','));
    scan.skipSpaces();
    if (!assembler().endOfLine(scan))
        return setError(scan, GARBAGE_AT_END);
    driver.setOrigin(driver.origin() + ((list.byteLength() + unit - 1) & -unit) / unit);
    return setOK();
}

Error AsmDirective::defineUint16s(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    auto &parser = assembler().parser();
    const uint8_t unit = assembler().config().addressUnit();
    const uint32_t base = driver.origin() * unit;
    const auto endian = assembler().config().endian();
    for (;;) {
        ErrorAt error;
        auto value = parser.eval(scan, error, &driver);
        if (error.getError())
            return setError(error);
        if (value.isUndefined() && driver.symbolMode() == REPORT_UNDEFINED)
            return setError(UNDEFINED_SYMBOL);
        if (value.overflowUint16())
            return setError(OVERFLOW_RANGE);
        const uint16_t val16 = value.getUnsigned();
        const uint8_t val8lo = val16;
        const uint8_t val8hi = val16 >> 8;
        list.emitByte(base, endian == ENDIAN_BIG ? val8hi : val8lo);
        list.emitByte(base, endian == ENDIAN_BIG ? val8lo : val8hi);
        if (!scan.skipSpaces().expect(','))
            break;
    }
    driver.setOrigin(driver.origin() + ((list.byteLength() + unit - 1) & -unit) / unit);
    return setOK();
}

Error AsmDirective::defineUint32s(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    auto &parser = assembler().parser();
    const uint8_t unit = assembler().config().addressUnit();
    const uint32_t base = driver.origin() * unit;
    const auto endian = assembler().config().endian();
    for (;;) {
        ErrorAt error;
        auto value = parser.eval(scan, error, &driver);
        if (error.getError())
            return setError(error);
        if (value.isUndefined() && driver.symbolMode() == REPORT_UNDEFINED)
            return setError(UNDEFINED_SYMBOL);
        auto val32 = value.getUnsigned();
        for (auto i = 0; i < 4; i++) {
            if (endian == ENDIAN_BIG) {
                list.emitByte(base, val32 >> 24);
                val32 <<= 8;
            } else {
                list.emitByte(base, val32);
                val32 >>= 8;
            }
        }
        if (!scan.skipSpaces().expect(','))
            break;
    }
    driver.setOrigin(driver.origin() + ((list.byteLength() + unit - 1) & -unit) / unit);
    return setOK();
}

Error AsmDirective::defineUint16sAligned(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    setAlignment(2, list, driver);
    return defineUint16s(scan, list, driver);
}

Error AsmDirective::defineUint32sAligned(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    setAlignment(2, list, driver);
    return defineUint32s(scan, list, driver);
}

Error AsmDirective::allocateUint8s(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    return allocateSpaces(scan, list, driver, sizeof(uint8_t));
}

Error AsmDirective::allocateUint16s(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    return allocateSpaces(scan, list, driver, sizeof(uint16_t));
}

Error AsmDirective::allocateUint32s(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    return allocateSpaces(scan, list, driver, sizeof(uint32_t));
}

Error AsmDirective::allocateUint16sAligned(
        StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    setAlignment(2, list, driver);
    return allocateUint16s(scan, list, driver);
}

Error AsmDirective::allocateUint32sAligned(
        StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    setAlignment(2, list, driver);
    return allocateUint32s(scan, list, driver);
}

Error AsmDirective::allocateSpaces(
        StrScanner &scan, AsmFormatter &list, AsmDriver &driver, int spaceUnit) {
    auto &parser = assembler().parser();
    auto value = parser.eval(scan, *this, &driver);
    if (getError())
        return getError();
    if (value.isUndefined() && driver.symbolMode() == REPORT_UNDEFINED)
        return setError(UNDEFINED_SYMBOL);
    if (value.overflowUint16())
        return setError(OVERFLOW_RANGE);
    const auto size = value.getUnsigned() * spaceUnit;
    const auto origin = driver.origin();
    if (origin + size < origin)
        return setError(OVERFLOW_RANGE);
    const uint8_t unit = assembler().config().addressUnit();
    driver.setOrigin(origin + ((size + unit - 1) & -unit) / unit);
    return setOK();
}

Error AsmDirective::defineFunction(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    if (list.lineSymbol().size() == 0)
        return setError(MISSING_LABEL);
    if (driver.symbolMode() == REPORT_DUPLICATE && driver.hasSymbol(list.lineSymbol()))
        return setError(DUPLICATE_LABEL);
    auto &parser = assembler().parser();
    std::list<StrScanner> params;
    for (;;) {
        const auto expr = parser.scanExpr(scan.skipSpaces(), *this, ',');
        if (expr.size() == 0) {
            const auto error = driver.internFunction(
                    list.lineSymbol(), params, StrScanner(scan.str(), expr.str()));
            scan = expr;
            list.lineSymbol() = StrScanner::EMPTY;
            return error;
        }
        auto p = expr;
        auto head = true;
        while (p.size()) {
            if (!parser.symbolLetter(*p, head))
                return setError(expr, SYMBOL_REQUIRE);
            head = false;
            ++p;
        }
        params.emplace_back(expr);
        scan += expr.size() + 1;
    }
}

Error AsmDirective::switchCpu(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    auto p = scan;
    p.trimStart([](char s) { return !isspace(s); });
    scan.trimEndAt(p);
    std::string cpu(scan.str(), scan.size());
    if (driver.setCpu(cpu.c_str()) == nullptr)
        return setError(UNSUPPORTED_CPU);
    scan = p;
    return setOK();
}

Error IntelDirective::switchIntelZilog(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    const /* PROGMEM */ char *cpu_P = assembler().cpu_P();
    if (!is8080(cpu_P))
        return setError(UNKNOWN_DIRECTIVE);
    char cpu[strlen_P(cpu_P) + 1];
    strcpy_P(cpu, cpu_P);
    auto option = assembler().parser().readSymbol(scan);
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
    registerPseudo("=", &AsmDirective::defineConstant);
    registerPseudo(".set", &AsmDirective::defineVariable);
    if (a.hasSetInstruction())
        disablePseudo("set");
    registerPseudo(":=", &AsmDirective::defineVariable);
    registerPseudo(".org", &AsmDirective::defineOrigin);
    registerPseudo(".align", &AsmDirective::alignOrigin);
    registerPseudo(".string", &AsmDirective::defineUint8s);
    registerPseudo(".ascii", &AsmDirective::defineUint8s);
    registerPseudo(".byte", &AsmDirective::defineUint8s);
    registerPseudo(".word", &AsmDirective::defineUint16s);
    registerPseudo(".long", &AsmDirective::defineUint32s);
    registerPseudo("dc.b", &AsmDirective::defineUint8s);
    registerPseudo("dc.w", &AsmDirective::defineUint16sAligned);
    registerPseudo("dc.l", &AsmDirective::defineUint32sAligned);
    registerPseudo("ds.b", &AsmDirective::allocateUint8s);
    registerPseudo("ds.w", &AsmDirective::allocateUint16sAligned);
    registerPseudo("ds.l", &AsmDirective::allocateUint32sAligned);
    registerPseudo(".function", &AsmDirective::defineFunction);
}

void AsmDirective::registerPseudo(const char *name, PseudoHandler handler) {
    _pseudos.emplace(std::make_pair(std::string(name), handler));
    if (*name++ == '.')
        _pseudos.emplace(std::make_pair(std::string(name), handler));
}

void AsmDirective::disablePseudo(const char *name) {
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

MotorolaDirective::MotorolaDirective(Assembler &assembler) : AsmDirective(assembler) {
    registerPseudo(".fcb", &MotorolaDirective::defineUint8s);
    registerPseudo(".fcc", reinterpret_cast<PseudoHandler>(&MotorolaDirective::defineString));
    registerPseudo(".fdb", &MotorolaDirective::defineUint16s);
    registerPseudo(".rmb", &MotorolaDirective::allocateUint8s);
    registerPseudo(".dfs", &MotorolaDirective::allocateUint8s);
}

BinEncoder &MotorolaDirective::defaultEncoder() {
    return MotoSrec::encoder();
}

IntelDirective::IntelDirective(Assembler &assembler) : AsmDirective(assembler) {
    registerPseudo(".db", &IntelDirective::defineUint8s);
    registerPseudo(".dw", &IntelDirective::defineUint16s);
    registerPseudo(".dd", &IntelDirective::defineUint32s);
    registerPseudo(".ds", &IntelDirective::allocateUint8s);
    registerPseudo(
            ".z80syntax", reinterpret_cast<PseudoHandler>(&IntelDirective::switchIntelZilog));
}

BinEncoder &IntelDirective::defaultEncoder() {
    return IntelHex::encoder();
}

NationalDirective::NationalDirective(Assembler &assembler) : IntelDirective(assembler) {
    registerPseudo(".dbyte", &NationalDirective::defineUint16s);
}

FairchildDirective::FairchildDirective(Assembler &assembler) : AsmDirective(assembler) {
    registerPseudo(".dc", &FairchildDirective::defineUint8s);
    registerPseudo(".da", &FairchildDirective::defineUint16s);
    registerPseudo(".rs", &FairchildDirective::allocateUint8s);
}

BinEncoder &FairchildDirective::defaultEncoder() {
    return IntelHex::encoder();
}

}  // namespace driver
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
