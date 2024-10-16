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
#include "pseudos.h"
#include "symbol_table.h"
#include "value_parser.h"

namespace libasm {

struct Assembler {
    Error encode(const char *line, Insn &insn, const SymbolTable *symtab = nullptr);
    virtual const ConfigBase &config() const = 0;
    virtual void reset();

    const ValueParser &parser() const { return _parser; }
    bool endOfLine(StrScanner &scan) const { return _parser.endOfLine(scan); }
    /** Parse |expr| text as an integer expression and get value. */
    Value parseInteger(StrScanner &expr, ErrorAt &error, char delim = 0) const;
    /** Parse |expr| text as an expression and get value. */
    Value parseExpr(StrScanner &expr, ErrorAt &error, char delim = 0) const;

    bool setCpu(const char *name);
    Error setCpu(StrScanner &scan);
    virtual Error setFpu(StrScanner &scan);

    Error setOption(const char *name, const char *text);
    Error setOption(const StrScanner &name, StrScanner &text);
    const Options &commonOptions() const { return _commonOptions; }
    const Options &options() const { return _options; }
    Error parseBoolOption(StrScanner &scan, bool &value) const;
    Error parseIntOption(StrScanner &scan, int32_t &value) const;

    Error setListRadix(int32_t radix);
    Radix listRadix() const { return _listRadix; }
    Error setSmartBranch(bool enable);

    Error setCurrentLocation(uint32_t location, bool align = false);
    uint32_t currentLocation() const { return _currentLocation; }

    virtual Error processPseudo(StrScanner &scan, Insn &insn);
    Error setOption(StrScanner &scan, Insn &insn, uint8_t extra = 0);
    Error defineOrigin(StrScanner &scan, Insn &insn, uint8_t extra = 0);
    enum AlignStep : uint8_t {
        ALIGN_VAR = 0,
        ALIGN_ODD = 1,
        ALIGN_EVEN = 2,
    };
    Error alignOrigin(StrScanner &scan, Insn &insn, uint8_t alignStep = ALIGN_VAR);
    Error defineString(StrScanner &scan, Insn &insn, uint8_t extra = 0);
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
        DATA_FLOAT64,
        DATA_ALIGN2 = 0x80,
    };
    Error defineDataConstant(StrScanner &scan, Insn &insn, uint8_t dataType);
    Error allocateSpaces(StrScanner &scan, Insn &insn, uint8_t dataType);

protected:
    const ValueParser _parser;
    const pseudo::Pseudos *const _pseudos;
    const Options _commonOptions;
    const Options _options;
    const IntOption<Assembler> _opt_listRadix;
    const BoolOption<Assembler> _opt_smartBranch;
    Radix _inputRadix;
    Radix _listRadix;
    bool _smartBranch;
    const SymbolTable *_symtab;
    uint32_t _currentLocation;

    Assembler(const ValueParser::Plugins &plugins, const pseudo::Pseudos &pseudos,
            const OptionBase *option = nullptr);

    Error setInputRadix(Radix radix) {
        _inputRadix = radix;
        return OK;
    }

    int32_t branchDelta(uint32_t base, uint32_t target, ErrorAt &error, const ErrorAt &at) const;

    void generateString(StrScanner &scan, const StrScanner &end, Insn &insn, DataType type,
            ErrorAt &error) const;

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
