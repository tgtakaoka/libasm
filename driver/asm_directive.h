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

#ifndef __ASM_DIRECTIVE_H__
#define __ASM_DIRECTIVE_H__

#include "asm_base.h"
#include "asm_formatter.h"
#include "error_reporter.h"
#include "str_scanner.h"

#include <map>
#include <string>

namespace libasm {
namespace driver {

class AsmDriver;
class BinEncoder;

class AsmDirective : public ErrorAt {
public:
    typedef Error (AsmDirective::*PseudoHandler)(
            StrScanner &scan, AsmFormatter &list, AsmDriver &driver);

    Assembler &assembler() const { return _assembler; }
    virtual BinEncoder &defaultEncoder() = 0;
    Error processPseudo(
            const StrScanner &name, StrScanner &scan, AsmFormatter &list, AsmDriver &driver);

    static bool is8080(const /* PROGMEM */ char *cpu_P);

protected:
    struct icasecmp {
        bool operator()(const std::string &lhs, const std::string &rhs) const {
            return strcasecmp(lhs.c_str(), rhs.c_str()) < 0;
        }
    };

    Assembler &_assembler;
    std::map<std::string, PseudoHandler, icasecmp> _pseudos;

    AsmDirective(Assembler &assembler);

    void registerPseudo(const char *name, PseudoHandler handler);
    void disablePseudo(const char *name);

    // Common PseudoHandler
    Error defineConstant(StrScanner &scan, AsmFormatter &list, AsmDriver &driver);
    Error defineVariable(StrScanner &scan, AsmFormatter &list, AsmDriver &driver);
    Error includeFile(StrScanner &scan, AsmFormatter &list, AsmDriver &driver);
    Error switchCpu(StrScanner &scan, AsmFormatter &list, AsmDriver &driver);
    Error endAssemble(StrScanner &scan, AsmFormatter &list, AsmDriver &driver);
    Error defineFunction(StrScanner &scan, AsmFormatter &list, AsmDriver &driver);

    // PseudoHanlder helper
    Error defineSymbol(StrScanner &scan, AsmFormatter &list, AsmDriver &driver,
            const StrScanner &symbol, bool variable);
};

class MotorolaDirective : public AsmDirective {
public:
    MotorolaDirective(Assembler &assembler);
    BinEncoder &defaultEncoder() override;

private:
    Error defineString(StrScanner &scan, AsmFormatter &list, AsmDriver &driver);
};

class IntelDirective : public AsmDirective {
public:
    IntelDirective(Assembler &assembler);
    BinEncoder &defaultEncoder() override;

private:
    Error switchIntelZilog(StrScanner &scan, AsmFormatter &list, AsmDriver &driver);
};

class MostekDirective : public AsmDirective {
public:
    MostekDirective(Assembler &assembler);
    BinEncoder &defaultEncoder() override;
};

class Z80Directive : public IntelDirective {
public:
    Z80Directive(Assembler &assembler);
};

class RcaDirective : public IntelDirective {
public:
    RcaDirective(Assembler &assembler);
};

class NationalDirective : public IntelDirective {
public:
    NationalDirective(Assembler &assembler);

private:
    Error setVariable(StrScanner &scan, AsmFormatter &list, AsmDriver &driver);
};

class FairchildDirective : public AsmDirective {
public:
    FairchildDirective(Assembler &assembler);
    BinEncoder &defaultEncoder() override;
};

}  // namespace driver
}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
