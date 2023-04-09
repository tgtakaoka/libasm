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

#ifndef __DIS_BASE_H__
#define __DIS_BASE_H__

#include <stddef.h>
#include <stdint.h>

#include "config_base.h"
#include "dis_memory.h"
#include "entry_table.h"
#include "error_reporter.h"
#include "insn_base.h"
#include "option_base.h"
#include "reg_base.h"
#include "str_buffer.h"
#include "symbol_table.h"
#include "type_traits.h"
#include "value_formatter.h"

namespace libasm {

class Disassembler : public ErrorReporter {
public:
    Error decode(DisMemory &memory, Insn &insn, char *operands, size_t size,
            SymbolTable *symtab = nullptr);
    virtual const ConfigBase &config() const = 0;
    virtual void reset();

    ValueFormatter &formatter() { return _formatter; }

    const /*PROGMEM*/ char *listCpu_P() const { return _table.listCpu_P(); }
    const /*PROGMEM*/ char *cpu_P() const { return _table.cpu_P(); }
    bool setCpu(const char *cpu) { return _table.setCpu(cpu); }

    Error setOption(const char *name, const char *text) {
        if (_commonOptions.setOption(name, text) == OK)
            return getError();
        return options().setOption(name, text);
    }
    const Options &commonOptions() const { return _commonOptions; }
    const Options &options() const { return _options; }

    Error setUpperHex(bool enable);
    Error setUppercase(bool enable);
    Error setRelativeTarget(bool enable);
    Error setCStyle(bool enable);
    Error setCurSym(char curSym);

protected:
    ValueFormatter _formatter;
    entry::Table &_table;
    const Options _commonOptions;
    const Options _options;
    const BoolOption<Disassembler> _opt_relative;
    const BoolOption<Disassembler> _opt_cstyle;
    const CharOption<Disassembler> _opt_curSym;
    const char _defaultCurSym;

    char _curSym;
    bool _uppercase;
    bool _relativeTarget;
    SymbolTable *_symtab = nullptr;

    Disassembler(const HexFormatter &hexFormatter, entry::Table &table, char curSym,
            const OptionBase *option = nullptr);

    /** Lookup |addr| value and returns symbol. */
    const char *lookup(uint32_t addr, uint8_t addrWidth = 0) const;

    /**
     * Convert |val| as |bits| decimal integer. Treat |val| as signed
     * integer when |bits| is negative.
     */
    StrBuffer &outDec(StrBuffer &out, uint32_t val, int8_t bits) const;

    /**
     * Convert |val| as |bits| hexadecimal integer. Treat |val| as
     * signed integer when |bits| is negative. Output symbol label when
     * |val| is in symbol table.
     */
    StrBuffer &outHex(StrBuffer &out, uint32_t val, int8_t bits, bool relax = true) const;

    /**
     * Convert |val| as |addrWidth| bit absolute address. Use default
     * configured address width when |addrWdith| is ommitted. Output
     * symbol label when |val| is in symbol table.
     */
    StrBuffer &outAbsAddr(StrBuffer &out, uint32_t val, uint8_t addrWidth = 0) const;

    /**
     * Convert |target| as relative |deltaBits| offset from |origin|.
     */
    StrBuffer &outRelAddr(
            StrBuffer &out, uint32_t target, uint32_t origin, uint8_t deltaBits) const;

    uint32_t branchTarget(uint32_t base, int32_t delta);

private:
    virtual Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) = 0;
};

}  // namespace libasm

#endif  // __DIS_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
