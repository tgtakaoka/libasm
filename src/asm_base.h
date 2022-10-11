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
#include "option_base.h"
#include "symbol_table.h"
#include "table_base.h"
#include "value_parser.h"

namespace libasm {

struct OperandBase : public ErrorAt {
    bool hasError() const { return getError() && getError() != UNDEFINED_SYMBOL; }
};

class Assembler : public ErrorAt {
public:
    Error encode(const char *line, Insn &insn, SymbolTable *symtab = nullptr);
    virtual const ConfigBase &config() const = 0;
    virtual void reset() {}

    ValueParser &parser() const { return _parser; }
    bool endOfLine(char letter) const;

    const /* PROGMEM */ char *listCpu_P() const { return _table.listCpu_P(); }
    const /* PROGMEM */ char *cpu_P() const { return _table.cpu_P(); }
    bool setCpu(const char *cpu) { return _table.setCpu(cpu); }

    Error setOption(const char *name, const char *text) {
        if (_commonOptions.setOption(name, text) == OK)
            return getError();
        return options().setOption(name, text);
    }
    const Options &commonOptions() const { return _commonOptions; }
    virtual const Options &options() const { return Options::EMPTY; }

    Error checkAddress(uint32_t addr, const ErrorAt &at);

private:
    ValueParser &_parser;

protected:
    TableBase &_table;
    char _commentChar;
    SymbolTable *_symtab;
    const CharOption _opt_commentChar{OPT_CHAR_COMMENT, OPT_DESC_COMMENT, _commentChar};
    const Options _commonOptions{_opt_commentChar};

    Assembler(ValueParser &parser, TableBase &table, char commentChar = 0)
        : ErrorAt(), _parser(parser), _table(table), _commentChar(commentChar) {}

    uint8_t addrUnit() { return uint8_t(config().addressUnit()); }

    /** Parse |expr| text and get value as unsigned 16 bit. */
    uint16_t parseExpr16(StrScanner &expr, ErrorAt &error) const;
    /** Parse |expr| text and get value as unsigned 32 bit. */
    uint32_t parseExpr32(StrScanner &expr, ErrorAt &error) const;
    /** Parse |expr| text and get value. */
    Value parseExpr(StrScanner &expr, ErrorAt &error) const;
    /** Return error caused by |parseExpr16| and |parseExpr32|. */
    Error parserError() const { return _parser.getError(); }

    static bool overflowRel8(int16_t s16) { return Value::overflowRel8(s16); }
    static bool overflowRel8(int32_t s32) { return Value::overflowRel8(s32); }
    static bool overflowRel16(int32_t s32) { return Value::overflowRel16(s32); }
    static bool overflowUint8(uint16_t u16) { return Value::overflowUint8(u16); }
    static bool overflowUint8(uint32_t u32) { return Value::overflowUint8(u32); }
    static bool overflowUint16(uint32_t u32) { return Value::overflowUint16(u32); }
    static bool overflowUint(uint32_t u32, uint8_t bitw) { return Value::overflowUint(u32, bitw); }

    static const char OPT_CHAR_COMMENT[] PROGMEM;
    static const char OPT_DESC_COMMENT[] PROGMEM;

private:
    virtual Error encodeImpl(StrScanner &scan, Insn &insn) = 0;
};

}  // namespace libasm

#endif  // __ASM_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
