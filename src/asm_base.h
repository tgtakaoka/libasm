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

#ifndef __LIBASM_ASM_BASE_H__
#define __LIBASM_ASM_BASE_H__

#include <stdint.h>

#include "config_base.h"
#include "entry_table.h"
#include "error_reporter.h"
#include "insn_base.h"
#include "option_base.h"
#include "parsers.h"
#include "pseudos.h"
#include "symbol_table.h"
#include "value_parser.h"

namespace libasm {

struct Assembler : private ValueParser::Locator {
    Error encode(const char *line, Insn &insn, SymbolTable *symtab = nullptr);
    virtual const ConfigBase &config() const = 0;
    virtual void reset();

    const ValueParser &parser() const { return _parser; }
    bool endOfLine(StrScanner &scan) const { return _parser.endOfLine(scan); }
    /** Parse |expr| text and get value. */
    Value parseExpr(StrScanner &expr, ErrorAt &error, char delim = 0) const;

    bool setCpu(const char *name);
    Error setCpu(StrScanner &scan);
    virtual Error setFpu(StrScanner &scan);

    Error setOption(const char *name, const char *text);
    Error setOption(const StrScanner &name, StrScanner &text);
    const Options &commonOptions() const { return _commonOptions; }
    const Options &options() const { return _options; }

    Error setListRadix(int32_t radix);
    Radix listRadix() const { return _listRadix; }
    Error setSmartBranch(bool enable);

    Error setCurrentLocation(uint32_t location, bool align = false);
    uint32_t currentLocation() const { return _currentLocation; }

    virtual Error processPseudo(StrScanner &scan, Insn &insn);
    Error setOption(StrScanner &scan, Insn &insn, uint8_t extra = 0);
    Error defineOrigin(StrScanner &scan, Insn &insn, uint8_t extra = 0);
    Error alignOrigin(StrScanner &scan, Insn &insn, uint8_t step = 0);
    enum StringType : uint8_t {
        STR_ASCII = 0,
        STR_DEC_6BIT = 1,
    };
    Error defineString(StrScanner &scan, Insn &insn, uint8_t stringType);
    Error isString(StrScanner &scan, ErrorAt &error) const;
    enum DataType : uint8_t {
        DATA_BYTE,
        DATA_BYTE_OR_WORD,
        DATA_BYTE_IN_WORD,
        DATA_BYTE_NO_STRING,
        DATA_WORD,
        DATA_WORD_NO_STRING,
        DATA_LONG,
        DATA_FLOAT32,
        DATA_FLOAT32_LONG,
        DATA_FLOAT64,
        DATA_FLOAT64_QUAD,
        DATA_FLOAT80_BCD,
        DATA_FLOAT96,
        DATA_PACKED_BCD96,
        DATA_ALIGN2 = 0x80,
    };
    Error defineDataConstant(StrScanner &scan, Insn &insn, uint8_t dataType);
    Error allocateSpaces(StrScanner &scan, Insn &insn, uint8_t dataType);

protected:
    const ValueParser _parser;
    const pseudo::Pseudos _pseudos;
    const Options _commonOptions;
    const Options _options;
    const IntOption<Assembler> _opt_listRadix;
    const BoolOption<Assembler> _opt_smartBranch;
    const TextOption<Assembler> _opt_fpu;

    Radix _listRadix;
    bool _smartBranch;
    SymbolTable *_symtab;
    uint32_t _currentLocation;

    Assembler(const ValueParser::Plugins &plugins, const pseudo::Pseudos &pseudos,
            const OptionBase *option = nullptr);

    int32_t branchDelta(uint32_t base, uint32_t target, ErrorAt &error, const ErrorAt &at) const;

    /** Parse |expr| text and get value as unsigned 16 bit. */
    uint16_t parseExpr16(StrScanner &expr, ErrorAt &error, char delim = 0) const;
    /** Parse |expr| text and get value as unsigned 32 bit. */
    uint32_t parseExpr32(StrScanner &expr, ErrorAt &error, char delim = 0) const;

    void generateString(StrScanner &scan, const StrScanner &end, Insn &insn, DataType type,
            ErrorAt &error) const;
    void generatePackedBcd80Le(int64_t value, Insn &insn) const;
    void generateFloat80Le(double value, Insn &insn) const;
    void generateFloat96Be(double value, Insn &insn, uint8_t pos) const;
    void generatePackedBcd96Be(double value, Insn &insn, uint8_t pos) const;

private:
    virtual ConfigSetter &configSetter() = 0;
    virtual Error encodeImpl(StrScanner &scan, Insn &insn) const = 0;
};

}  // namespace libasm

#endif  // __LIBASM_ASM_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
