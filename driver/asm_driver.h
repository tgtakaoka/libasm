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

#ifndef __ASM_DRIVER_H__
#define __ASM_DRIVER_H__

#include <initializer_list>
#include <list>

#include "asm_directive.h"
#include "error_reporter.h"
#include "str_scanner.h"
#include "symbol_store.h"
#include "text_printer.h"

namespace libasm {
namespace driver {

struct AsmSources;
struct BinMemory;

struct AsmDriver final : CpuSwitcher {
    AsmDriver(std::initializer_list<AsmDirective *> directives);

    bool restrictCpu(const char *cpu);
    bool setCpu(const char *cpu) override;
    std::list<std::string> listCpu() const;
    AsmDirective *current() const { return _current; }

    void setUpperHex(bool upperHex);
    void setLineNumber(bool enable);
    void setOption(const char *name, const char *value);

    int assemble(AsmSources &sources, BinMemory &memory, TextPrinter &listout,
            TextPrinter &errorout, bool reportError = true);

    auto begin() const { return _directives.cbegin(); }
    auto end() const { return _directives.cend(); }

    void clearSymbols() { _symbols.clear(); }
    SymbolStoreImpl &symbols() { return _symbols; }

private:
    std::list<AsmDirective *> _directives;
    AsmDirective *_current;

    SymbolStoreImpl _symbols;
    uint32_t _origin;
    bool _upperHex;
    bool _lineNumber;
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
