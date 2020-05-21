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

#ifndef __ASM_BASE_H__
#define __ASM_BASE_H__

#include "config_base.h"
#include "error_reporter.h"
#include "insn_base.h"
#include "symbol_table.h"
#include "value_parser.h"

#include <stdint.h>

namespace libasm {

class Assembler
    : public ErrorReporter,
      virtual public ConfigBase {
public:
    Error encode(
        const char *line, Insn &insn, uint32_t addr, SymbolTable *symtab);

    virtual ValueParser *getParser() = 0;
    const char *errorAt() const { return _scan; }

protected:
    const char *_scan;
    SymbolTable  *_symtab;

    void reset(const char *line, SymbolTable *symtab);
    bool hasSymbol(const char *symbol) const;
    uint32_t lookupSymbol(const char *symbol) const;
    bool endOfLine(const char *scan);
    Error checkLineEnd(const char *scan = nullptr);
    static const char *skipSpaces(const char *scan);

    template<typename T>
    Error getOperand(T &val) {
        ValueParser *parser = getParser();
        _scan = parser->eval(_scan, val, _symtab);
        if (setError(*parser) == UNDEFINED_SYMBOL)
            return OK;
        return getError();
    }

private:
    virtual Error encode(Insn &insn) = 0;
};

} // namespace libasm

#endif // __ASM_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
