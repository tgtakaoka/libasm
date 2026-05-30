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

#ifndef __LIBASM_DIS_BASE_H__
#define __LIBASM_DIS_BASE_H__

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
#include "value.h"
#include "value_formatter.h"

namespace libasm {

struct Disassembler {
    virtual ~Disassembler() {}

    Error decode(DisMemory &memory, Insn &insn, char *operands, size_t size,
            SymbolTable *symtab = nullptr);
    virtual const ConfigBase &config() const = 0;
    virtual void reset();

    const ValueFormatter &formatter() const;
    const /*PROGMEM*/ char *lineComment_P() const { return _formatter.lineComment_P(); }

    bool setCpu(const char *name) { return configSetter().setCpu(name); }

    Error setOption(const char *name, const char *text);
    const Options &commonOptions() const { return _commonOptions; }
    const Options &options() const { return _options; }

    Error setUpperHex(bool enable);
    Error setUppercase(bool enable);
    Error setListRadix(int32_t radix);
    Error setRelativeTarget(bool enable);
    Error setCStyle(bool enable);
    Error setIntelHex(bool enable);
    Error setCurSym(char curSym);
    virtual Error setGnuAs(bool enable);

    Radix listRadix() const { return _listRadix; }

    /**
     * Convert |val| as |addrWidth| bit absolute address. Use default configured address width when
     * |addrWdith| is omitted. Output symbol label when |val| is in symbol table.
     */
    virtual StrBuffer &outAbsAddr(StrBuffer &out, uint32_t val, uint8_t addrWidth = 0) const;

private:
    const ValueFormatter _formatter;

protected:
    const Options _commonOptions;
    const Options _options;
    const BoolOption<Disassembler> _opt_upperHex;
    const BoolOption<Disassembler> _opt_uppercase;
    const IntOption<Disassembler> _opt_listRadix;
    const BoolOption<Disassembler> _opt_relative;
    const BoolOption<Disassembler> _opt_cstyle;
    const BoolOption<Disassembler> _opt_intelhex;
    const CharOption<Disassembler> _opt_curSym;
    const BoolOption<Disassembler> _opt_gnuAs;

    bool _upperHex;
    bool _uppercase;
    Radix _listRadix;
    bool _relativeTarget;
    bool _cstyle;
    bool _intelHex;
    char _curSym;
    bool _gnuAs;
    SymbolTable *_symtab = nullptr;

    Disassembler(const ValueFormatter::Plugins &plugins, const OptionBase *option = nullptr);

    /** Lookup |addr| value and returns symbol. */
    const char *lookup(uint32_t addr, uint8_t addrWidth = 0) const;

    /**
     * Convert |val| as |bits| decimal integer. Treat |val| as signed integer when |bits| is
     * negative.
     */
    StrBuffer &outDec(StrBuffer &out, uint32_t val, int8_t bits) const;

    /**
     * Convert |val| as |bits| hexadecimal integer. Treat |val| as signed integer when |bits| is
     * negative. Output symbol label when |val| is in symbol table.
     */
    StrBuffer &outHex(StrBuffer &out, uint32_t val, int8_t bits, bool relax = true) const;

    /**
     * Convert |target| as relative |deltaBits| offset from |origin|.
     */
    StrBuffer &outRelAddr(
            StrBuffer &out, uint32_t target, uint32_t origin, uint8_t deltaBits) const;

private:
    virtual ConfigSetter &configSetter() = 0;
    virtual Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) const = 0;
};

}  // namespace libasm

#endif  // __LIBASM_DIS_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
