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

struct AsmDriver;
struct BinEncoder;

struct AsmDirective : ErrorAt {
    typedef Error (AsmDirective::*PseudoHandler)(
            StrScanner &scan, AsmFormatter &formatter, AsmDriver &driver);

    Assembler &assembler() const { return _assembler; }
    virtual BinEncoder &defaultEncoder() const;

    Error processPseudo(
            const StrScanner &name, StrScanner &scan, AsmFormatter &formatter, AsmDriver &driver);

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
    Error defineConstant(StrScanner &scan, AsmFormatter &formatter, AsmDriver &driver);
    Error defineVariable(StrScanner &scan, AsmFormatter &formatter, AsmDriver &driver);
    Error setVariable(StrScanner &scan, AsmFormatter &formatter, AsmDriver &driver);
    Error includeFile(StrScanner &scan, AsmFormatter &formatter, AsmDriver &driver);
    Error switchCpu(StrScanner &scan, AsmFormatter &formatter, AsmDriver &driver);
    Error switchIntelZilog(StrScanner &scan, AsmFormatter &formatter, AsmDriver &driver);
    Error endAssemble(StrScanner &scan, AsmFormatter &formatter, AsmDriver &driver);
    Error defineFunction(StrScanner &scan, AsmFormatter &formatter, AsmDriver &driver);

    // PseudoHanlder helper
    Error defineSymbol(StrScanner &scan, AsmFormatter &formatter, AsmDriver &driver,
            const StrScanner &symbol, bool variable);
};

struct MotorolaDirective : AsmDirective {
    MotorolaDirective(Assembler &assembler);
    BinEncoder &defaultEncoder() const override;
};

struct IntelDirective : AsmDirective {
    IntelDirective(Assembler &assembler);
};

struct MostekDirective : MotorolaDirective {
    MostekDirective(Assembler &assembler);
};

struct Z8Directive : IntelDirective {
    Z8Directive(Assembler &assembler);
};

struct ZilogDirective : IntelDirective {
    ZilogDirective(Assembler &assembler);
};

struct RcaDirective : IntelDirective {
    RcaDirective(Assembler &assembler);
};

struct NationalDirective : IntelDirective {
    NationalDirective(Assembler &assembler);
};

struct FairchildDirective : AsmDirective {
    FairchildDirective(Assembler &assembler);
};

struct DecDirective : AsmDirective {
    DecDirective(Assembler &assembler);
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
