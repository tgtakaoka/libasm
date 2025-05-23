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

#ifndef __LIBASM_ASM_DIRECTIVE_H__
#define __LIBASM_ASM_DIRECTIVE_H__

#include <map>
#include <string>

#include "asm_base.h"
#include "bin_encoder.h"
#include "error_reporter.h"
#include "str_scanner.h"
#include "value.h"

namespace libasm {
namespace driver {

struct AsmSources;
struct BinEncoder;
struct SymbolStore;

struct CpuSwitcher {
    virtual bool setCpu(const char *cpu) = 0;
};

struct AsmDirective : ErrorAt {
    bool setCpu(const char *cpu);
    const /*PROGMEM*/ char *cpu_P() const;
    const /*PROGMEM*/ char *listCpu_P() const;
    const ConfigBase &config() const;
    const Options &commonOptions() const;
    const Options &options() const;

    void reset(CpuSwitcher &switcher);
    void setOption(const char *name, const char *value);
    Radix listRadix() const;
    uint32_t currentLocation() const;

    struct Context {
        Context(AsmSources &_sources, SymbolStore &_symbols, bool _reportError = true)
            : sources(_sources),
              symbols(_symbols),
              reportDuplicate(!_reportError),
              reportUndefined(_reportError) {}
        AsmSources &sources;
        SymbolStore &symbols;
        const bool reportDuplicate;
        const bool reportUndefined;
        StrScanner label;
        Value value;
    };

    Error encode(StrScanner &scan, Insn &insn, Context &context);

    virtual BinEncoder &defaultEncoder() const;

protected:
    Assembler &_assembler;
    CpuSwitcher *_switcher;

    using PseudoHandler = Error (AsmDirective::*)(StrScanner &scan, Context &context);
    struct icasecmp {
        bool operator()(const std::string &lhs, const std::string &rhs) const {
            return strcasecmp(lhs.c_str(), rhs.c_str()) < 0;
        }
    };
    std::map<std::string, PseudoHandler, icasecmp> _pseudos;

    AsmDirective(Assembler &assembler);

    void registerPseudo(const char *name, PseudoHandler handler);
    void disablePseudo(const char *name);
    Error processPseudo(const StrScanner &name, StrScanner &scan, Context &context);

    // PseudoHandler
    Error defineConstant(StrScanner &scan, Context &context);
    Error defineVariable(StrScanner &scan, Context &context);
    Error setVariable(StrScanner &scan, Context &context);
    Error includeFile(StrScanner &scan, Context &context);
    Error switchCpu(StrScanner &scan, Context &context);
    Error endAssemble(StrScanner &scan, Context &context);
    Error defineFunction(StrScanner &scan, Context &context);

    // PseudoHanlder helper
    Error defineSymbol(StrScanner &scan, const StrScanner &symbol, Context &context, bool variable);
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
