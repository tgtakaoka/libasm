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
    const StrScanner line = scan;
    ValueParser &parser = assembler().parser();
    Value value = parser.eval(scan, &driver);
    // TODO line end check
    if (setError(parser))
        return getError();
    if (value.isUndefined() && driver.symbolMode() == REPORT_UNDEFINED)
        return setError(parser, UNDEFINED_SYMBOL);
    setAt(line);
    if (assembler().checkAddress(value.getUnsigned(), *this))
        return setError(assembler());
    list.setStartAddress(driver.setOrigin(value.getUnsigned()));
    return setOK();
}

Error AsmDirective::alignOrigin(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    const StrScanner line = scan;
    ValueParser &parser = assembler().parser();
    Value value = parser.eval(scan, &driver);
    // TODO line end check
    if (setError(parser))
        return getError();
    if (value.isUndefined() && driver.symbolMode() == REPORT_UNDEFINED)
        return setError(UNDEFINED_SYMBOL);
    if (value.overflowUint16())
        return setError(OVERFLOW_RANGE);
    const auto alignment = value.getUnsigned();
    if (alignment > 0x1000)
        setError(ILLEGAL_OPERAND);
    const auto origin = driver.origin();
    list.setStartAddress(origin);
    const auto addr = (origin + (alignment - 1)) & ~(alignment - 1);
    setAt(line);
    if (assembler().checkAddress(addr, *this))
        return setError(assembler());
    driver.setOrigin(addr);
    return setOK();
}

Error AsmDirective::defineLabel(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    if (list.lineSymbol().size() == 0)
        return setError(MISSING_LABEL);
    ValueParser &parser = assembler().parser();
    const auto &value = list.lineValue() = parser.eval(scan, &driver);
    if (setError(parser))
        return getError();
    if (value.isUndefined() && driver.symbolMode() == REPORT_UNDEFINED)
        return setError(UNDEFINED_SYMBOL);
    // TODO line end check
    const auto error = driver.internSymbol(value.getUnsigned(), list.lineSymbol());
    if (error) {
        setError(list.lineSymbol(), error);
    } else {
        list.lineSymbol() = StrScanner::EMPTY;
    }
    return getError();
}

Error AsmDirective::includeFile(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    char quote = scan.expect('"');
    if (quote == 0)
        quote = scan.expect('\'');
    StrScanner filename(scan);
    StrScanner p(scan);
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

Error AsmDirective::defineUint8s(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    return defineBytes(scan, list, driver, false);
}

Error AsmDirective::defineString(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    return defineBytes(scan, list, driver, true);
}

Error AsmDirective::defineBytes(
        StrScanner &scan, AsmFormatter &list, AsmDriver &driver, bool delimitor) {
    ValueParser &parser = assembler().parser();
    const uint8_t unit = assembler().config().addressUnit();
    const uint32_t base = driver.origin() * unit;
    for (;;) {
        scan.skipSpaces();
        if (delimitor || *scan == '"') {
            const char delim = *scan++;
            StrScanner p(scan);
            for (;;) {
                if (p.expect(delim))
                    break;
                if (*p == 0) {
                    switch (delim) {
                    case '"':
                        return setError(p, MISSING_CLOSING_DQUOTE);
                    default:
                        return setError(p, MISSING_CLOSING_DELIMITOR);
                    }
                }
                const char c = parser.readChar(p);
                if (setError(parser)) {
                    scan = p;
                    return getError();
                }
                list.emitByte(base, c);
            }
            scan = p;
        } else {
            Value value = parser.eval(scan, &driver);
            if (setError(parser))
                return getError();
            if (value.isUndefined() && driver.symbolMode() == REPORT_UNDEFINED)
                return setError(UNDEFINED_SYMBOL);
            if (value.overflowUint8())
                return setError(OVERFLOW_RANGE);
            const uint8_t val8 = value.getUnsigned();
            list.emitByte(base, val8);
        }
        if (!scan.skipSpaces().expect(','))
            break;
    }
    driver.setOrigin(driver.origin() + ((list.byteLength() + unit - 1) & -unit) / unit);
    return setOK();
}

Error AsmDirective::defineUint16s(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    ValueParser &parser = assembler().parser();
    const uint8_t unit = assembler().config().addressUnit();
    const uint32_t base = driver.origin() * unit;
    const auto endian = assembler().config().endian();
    for (;;) {
        Value value = parser.eval(scan, &driver);
        if (setError(parser))
            return getError();
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
    ValueParser &parser = assembler().parser();
    const uint8_t unit = assembler().config().addressUnit();
    const uint32_t base = driver.origin() * unit;
    const auto endian = assembler().config().endian();
    for (;;) {
        Value value = parser.eval(scan, &driver);
        if (setError(parser))
            return getError();
        if (value.isUndefined() && driver.symbolMode() == REPORT_UNDEFINED)
            return setError(UNDEFINED_SYMBOL);
        uint32_t val32 = value.getUnsigned();
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

Error AsmDirective::allocateUint8s(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    return allocateSpaces(scan, list, driver, sizeof(uint8_t));
}

Error AsmDirective::allocateUint16s(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    return allocateSpaces(scan, list, driver, sizeof(uint16_t));
}

Error AsmDirective::allocateUint32s(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    return allocateSpaces(scan, list, driver, sizeof(uint32_t));
}

Error AsmDirective::allocateSpaces(
        StrScanner &scan, AsmFormatter &list, AsmDriver &driver, size_t unit) {
    ValueParser &parser = assembler().parser();
    Value value = parser.eval(scan, &driver);
    if (setError(parser))
        return getError();
    if (value.isUndefined() && driver.symbolMode() == REPORT_UNDEFINED)
        return setError(UNDEFINED_SYMBOL);
    if (value.overflowUint16())
        return setError(OVERFLOW_RANGE);
    const auto size = value.getUnsigned() * unit;
    const auto origin = driver.origin();
    if (origin + size < origin)
        return setError(OVERFLOW_RANGE);
    driver.setOrigin(origin + ((size + unit - 1) & -unit) / unit);
    return setOK();
}

Error AsmDirective::defineFunction(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    if (list.lineSymbol().size() == 0)
        return setError(MISSING_LABEL);
    if (driver.symbolMode() == REPORT_DUPLICATE && driver.hasSymbol(list.lineSymbol()))
        return setError(DUPLICATE_LABEL);
    ValueParser &parser = assembler().parser();
    std::list<StrScanner> params;
    for (;;) {
        const StrScanner expr = parser.scanExpr(scan.skipSpaces(), ',');
        if (expr.size() == 0) {
            const auto error = driver.internFunction(
                    list.lineSymbol(), params, StrScanner(scan.str(), expr.str()));
            scan = expr;
            list.lineSymbol() = StrScanner::EMPTY;
            return error;
        }
        StrScanner p(expr);
        bool head = true;
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
    StrScanner p(scan);
    p.trimStart([](char s) { return !isspace(s); });
    scan.trimEndAt(p);
    std::string cpu(scan.str(), scan.size());
    if (driver.setCpu(cpu.c_str()) == nullptr)
        return setError(UNSUPPORTED_CPU);
    scan = p;
    return setOK();
}

Error AsmDirective::switchIntelZilog(StrScanner &scan, AsmFormatter &list, AsmDriver &driver) {
    const /* PROGMEM */ char *cpu_P = assembler().cpu_P();
    if (!is8080(cpu_P))
        return setError(UNKNOWN_DIRECTIVE);
    char cpu[strlen_P(cpu_P) + 1];
    strcpy_P(cpu, cpu_P);
    StrScanner option = assembler().parser().readSymbol(scan);
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
    registerPseudo(".equ", &AsmDirective::defineLabel);
    registerPseudo(":=", &AsmDirective::defineLabel);
    registerPseudo("=", &AsmDirective::defineLabel);
    registerPseudo(".org", &AsmDirective::defineOrigin);
    registerPseudo(".align", &AsmDirective::alignOrigin);
    registerPseudo(".string", &AsmDirective::defineString);
    registerPseudo(".ascii", &AsmDirective::defineString);
    registerPseudo(".byte", &AsmDirective::defineUint8s);
    registerPseudo(".word", &AsmDirective::defineUint16s);
    registerPseudo(".long", &AsmDirective::defineUint32s);
    registerPseudo("dc.b", &AsmDirective::defineUint8s);
    registerPseudo("dc.w", &AsmDirective::defineUint16s);
    registerPseudo("dc.l", &AsmDirective::defineUint32s);
    registerPseudo("ds.b", &AsmDirective::allocateUint8s);
    registerPseudo("ds.w", &AsmDirective::allocateUint16s);
    registerPseudo("ds.l", &AsmDirective::allocateUint32s);
    registerPseudo(".function", &AsmDirective::defineFunction);
}

void AsmDirective::registerPseudo(const char *name, PseudoHandler handler) {
    _pseudos.emplace(std::make_pair(std::string(name), handler));
    if (*name++ == '.')
        _pseudos.emplace(std::make_pair(std::string(name), handler));
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
    registerPseudo(".fcb", &AsmDirective::defineUint8s);
    registerPseudo(".fcc", &AsmDirective::defineString);
    registerPseudo(".fdb", &AsmDirective::defineUint16s);
    registerPseudo(".rmb", &AsmDirective::allocateUint8s);
    registerPseudo(".dfs", &AsmDirective::allocateUint8s);
}

BinEncoder &MotorolaDirective::defaultEncoder() {
    return MotoSrec::encoder();
}

IntelDirective::IntelDirective(Assembler &assembler) : AsmDirective(assembler) {
    registerPseudo(".db", &AsmDirective::defineUint8s);
    registerPseudo(".dw", &AsmDirective::defineUint16s);
    registerPseudo(".dd", &AsmDirective::defineUint32s);
    registerPseudo(".ds", &AsmDirective::allocateUint8s);
    registerPseudo(".z80syntax", &AsmDirective::switchIntelZilog);
}

BinEncoder &IntelDirective::defaultEncoder() {
    return IntelHex::encoder();
}

NationalDirective::NationalDirective(Assembler &assembler) : IntelDirective(assembler) {
    registerPseudo(".dbyte", &AsmDirective::defineUint16s);
}

}  // namespace driver
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
