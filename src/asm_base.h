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

#include <stdint.h>

#include "config_base.h"
#include "error_reporter.h"
#include "insn_base.h"
#include "symbol_table.h"
#include "table_base.h"
#include "value_parser.h"

namespace libasm {

class Assembler : public ErrorReporter, virtual public ConfigBase {
public:
    Error encode(
            const char *line, Insn &insn, uint32_t addr, SymbolTable *symtab);

    void setCommentChar(char commentChar) { _commentChar = commentChar; }
    ValueParser &getParser() const { return _parser; }
    const char *errorAt() const { return _scan; }
    bool endOfLine(const char *scan) const;
    virtual void reset() {}

    const char *listCpu() const { return _table.listCpu(); }
    bool setCpu(const char *cpu) { return _table.setCpu(cpu); }
    const char *getCpu() const { return _table.getCpu(); }

protected:
    ValueParser &_parser;
    TableBase &_table;
    char _commentChar;
    const char *_scan;
    SymbolTable *_symtab;

    Assembler(ValueParser &parser, TableBase &table, char commentChar = 0)
        : _parser(parser), _table(table), _commentChar(commentChar) {}

    bool hasSymbol(const char *symbol) const;
    uint32_t lookupSymbol(const char *symbol) const;

    /** Scan |expr| text to find |delim| letter. */
    const char *scanExpr(const char *expr, char delim, uint16_t nesting = 0) const;
    /** Parse |expr| text and get value as unsigned 16 bit. */
    uint16_t parseExpr16(const char *expr, const char *end = nullptr);
    /** Parse |expr| text and get value as unsigned 32 bit. */
    uint32_t parseExpr32(const char *expr, const char *end = nullptr);
    /** Return error caused by |parseExpr16| and |parseExpr32|. */
    Error parserError() { return _parser.error(); }

    static const char *skipSpaces(const char *scan);

private:
    virtual Error encode(Insn &insn) = 0;
};

}  // namespace libasm

#endif  // __ASM_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
