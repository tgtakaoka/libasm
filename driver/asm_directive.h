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

#include <map>
#include <string>

#include "asm_base.h"
#include "asm_sources.h"
#include "bin_encoder.h"
#include "error_reporter.h"
#include "str_scanner.h"

namespace libasm {
namespace driver {

struct AsmDriver;
struct BinEncoder;

struct AsmDirective : ErrorAt {
    struct Context {
        Context(AsmSources &_sources, bool _reportError = true)
            : sources(_sources), reportDuplicate(!_reportError), reportUndefined(_reportError) {}
        AsmSources &sources;
        const bool reportDuplicate;
        const bool reportUndefined;
        StrScanner label;
        Value value;
    };

    Error encode(StrScanner &scan, Insn &insn, Context &context, AsmDriver &drive);

    Assembler &assembler() const { return _assembler; }
    virtual BinEncoder &defaultEncoder() const;

    static bool is8080(const /* PROGMEM */ char *cpu_P);

protected:
    Assembler &_assembler;

    typedef Error (AsmDirective::*PseudoHandler)(
            StrScanner &scan, Context &context, AsmDriver &driver);
    struct icasecmp {
        bool operator()(const std::string &lhs, const std::string &rhs) const {
            return strcasecmp(lhs.c_str(), rhs.c_str()) < 0;
        }
    };
    std::map<std::string, PseudoHandler, icasecmp> _pseudos;

    AsmDirective(Assembler &assembler);

    void registerPseudo(const char *name, PseudoHandler handler);
    void disablePseudo(const char *name);
    Error processPseudo(
            const StrScanner &name, StrScanner &scan, Context &context, AsmDriver &driver);

    // PseudoHandler
    Error defineConstant(StrScanner &scan, Context &context, AsmDriver &driver);
    Error defineVariable(StrScanner &scan, Context &context, AsmDriver &driver);
    Error setVariable(StrScanner &scan, Context &context, AsmDriver &driver);
    Error includeFile(StrScanner &scan, Context &context, AsmDriver &driver);
    Error switchCpu(StrScanner &scan, Context &context, AsmDriver &driver);
    Error switchIntelZilog(StrScanner &scan, Context &context, AsmDriver &driver);
    Error endAssemble(StrScanner &scan, Context &context, AsmDriver &driver);
    Error defineFunction(StrScanner &scan, Context &context, AsmDriver &driver);

    // PseudoHanlder helper
    Error defineSymbol(StrScanner &scan, const StrScanner &symbol, Context &context,
            AsmDriver &driver, bool variable);
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
