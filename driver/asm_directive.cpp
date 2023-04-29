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
    const auto err = assembler().config().checkAddr(value.getUnsigned());
    if (err)
        return setError(err);
    list.setStartAddress(driver.setOrigin(value.getUnsigned()));
    return setOK();
}

Error AsmDirective::setAlignment(uint32_t alignment, AsmFormatter &list, AsmDriver &driver) {
    const auto addr = (driver.origin() + (alignment - 1)) & ~(alignment - 1);
    const auto err = assembler().config().checkAddr(addr);
    if (err)
        return setError(err);
    driver.setOrigin(addr);
    list.setStartAddress(driver.origin());
    return setOK();
}

Error AsmDirective::alignOrigin(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    const auto line = scan;
    auto &parser = assembler().parser();
    ErrorAt error;
    const auto value = parser.eval(scan, error, &driver);
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
    return defineSymbol(scan, list, driver, list.lineSymbol(), /*variable*/ false);
}

Error AsmDirective::defineVariable(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    return defineSymbol(scan, list, driver, list.lineSymbol(), /*variable*/ true);
}

Error NationalDirective::setVariable(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    if (list.lineSymbol().size())
        return setError(ILLEGAL_LABEL);
    StrScanner symbol = assembler().parser().readSymbol(scan);
    if (symbol.size() == 0)
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
    const uint8_t unit = assembler().config().addressUnit();
    const uint32_t base = driver.origin() * unit;
    do {
        const auto delim = *scan.skipSpaces()++;
        auto p = scan;
        while (!p.expect(delim)) {
            if (*p == 0)
                return setError(p, MISSING_CLOSING_DELIMITER);
            const auto c = *p++;
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

static bool isString(StrScanner &scan, const ValueParser &parser) {
    auto p = scan;
    const auto delim = *p++;
    if (delim == '"' || delim == '\'') {
        while (true) {
            if (parser.endOfLine(p))
                break;
            ErrorAt error;
            parser.readLetter(p, error);
            if (error.getError())
                break;
            if (*p == delim) {
                auto a = p;
                a += 1;  // skip delim
                if (parser.endOfLine(a.skipSpaces()) || *a == ',') {
                    scan = p;
                    return true;
                }
            }
        }
    }
    return false;
}

Error AsmDirective::defineData8s(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    return defineBytes(scan, list, driver, true);
}

Error AsmDirective::defineUint8s(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    return defineBytes(scan, list, driver, false);
}

Error AsmDirective::defineBytes(
        StrScanner &scan, AsmFormatter &list, AsmDriver &driver, bool acceptString) {
    auto &parser = assembler().parser();
    const uint8_t unit = assembler().config().addressUnit();
    const uint32_t base = driver.origin() * unit;
    do {
        auto p = scan.skipSpaces();
        ErrorAt error;
        auto value = parser.eval(p, error, &driver);
        if (error.isOK() && (parser.endOfLine(p.skipSpaces()) || *p == ',')) {
            // a byte expression.
            if (value.isUndefined() && driver.symbolMode() == REPORT_UNDEFINED)
                return setError(UNDEFINED_SYMBOL);
            if (value.overflowUint8())
                return setError(OVERFLOW_RANGE);
            const uint8_t val8 = value.getUnsigned();
            list.emitByte(base, val8);
            scan = p;
            continue;
        }
        if (acceptString) {
            p = scan;
            if (isString(p, parser)) {
                // a string surrounded by double quotes, single quotes
                ++scan;  // skip delimitor
                while (scan.str() < p.str()) {
                    ErrorAt error;
                    const auto c = parser.readLetter(scan, error);
                    list.emitByte(base, c);
                }
                ++scan;  // skip delimitor
                continue;
            }
        }
        return setError(error);
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
    if (driver.symbolMode() == REPORT_DUPLICATE && driver.symbolInTable(list.lineSymbol()))
        return setError(DUPLICATE_LABEL);
    const auto &parser = assembler().parser();
    FunctionStore::Parameters params;
    for (;;) {
        auto p = scan.skipSpaces();
        const auto param = parser.readSymbol(p);
        if (param.size() && p.skipSpaces().expect(',')) {
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
    registerPseudo(".set", &AsmDirective::defineVariable);
    if (a.hasSetInstruction())
        disablePseudo("set");
    registerPseudo(".org", &AsmDirective::defineOrigin);
    registerPseudo(".align", &AsmDirective::alignOrigin);
    registerPseudo(".string", &AsmDirective::defineData8s);
    registerPseudo(".ascii", &AsmDirective::defineData8s);
    registerPseudo(".byte", &AsmDirective::defineData8s);
    registerPseudo(".word", &AsmDirective::defineUint16s);
    registerPseudo(".long", &AsmDirective::defineUint32s);
    registerPseudo("dc.b", &AsmDirective::defineData8s);
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
    registerPseudo(".fcc", static_cast<PseudoHandler>(&MotorolaDirective::defineString));
    registerPseudo(".fdb", &MotorolaDirective::defineUint16s);
    registerPseudo(".rmb", &MotorolaDirective::allocateUint8s);
    registerPseudo(".dfs", &MotorolaDirective::allocateUint8s);
}

BinEncoder &MotorolaDirective::defaultEncoder() {
    return MotoSrec::encoder();
}

IntelDirective::IntelDirective(Assembler &assembler) : AsmDirective(assembler) {
    registerPseudo(".db", &IntelDirective::defineData8s);
    registerPseudo(".dw", &IntelDirective::defineUint16s);
    registerPseudo(".dd", &IntelDirective::defineUint32s);
    registerPseudo(".ds", &IntelDirective::allocateUint8s);
    registerPseudo(".z80syntax", static_cast<PseudoHandler>(&IntelDirective::switchIntelZilog));
}

BinEncoder &IntelDirective::defaultEncoder() {
    return IntelHex::encoder();
}

MostekDirective::MostekDirective(Assembler &assembler) : AsmDirective(assembler) {
    registerPseudo("=", &MostekDirective::defineConstant);
    registerPseudo("*=", &MostekDirective::defineOrigin);
}

BinEncoder &MostekDirective::defaultEncoder() {
    return MotoSrec::encoder();
}

Z80Directive::Z80Directive(Assembler &assembler) : IntelDirective(assembler) {
    registerPseudo("defb", &Z80Directive::defineData8s);
    registerPseudo("defw", &Z80Directive::defineUint16s);
    registerPseudo("defm", &Z80Directive::defineData8s);  // TODO: DEFT 'str' which has length byte
    registerPseudo("defs", &Z80Directive::allocateUint8s);
    registerPseudo("defl", &Z80Directive::defineVariable);
}

RcaDirective::RcaDirective(Assembler &assembler) : IntelDirective(assembler) {
    registerPseudo("=", &RcaDirective::defineConstant);
}

NationalDirective::NationalDirective(Assembler &assembler) : IntelDirective(assembler) {
    disablePseudo(".set");
    disablePseudo("set");
    registerPseudo("=", &NationalDirective::defineConstant);
    registerPseudo(".=", &NationalDirective::defineOrigin);
    registerPseudo(".set", static_cast<PseudoHandler>(&NationalDirective::setVariable));
    registerPseudo(".dbyte", &NationalDirective::defineUint16s);
}

FairchildDirective::FairchildDirective(Assembler &assembler) : AsmDirective(assembler) {
    registerPseudo(".dc", &FairchildDirective::defineData8s);
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
