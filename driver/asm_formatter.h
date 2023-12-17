/*
 * Copyright 2022 Tadashi G. Takaoka
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

#ifndef __LIBASM_ASM_FORMATTER_H__
#define __LIBASM_ASM_FORMATTER_H__

#include "asm_sources.h"
#include "insn_base.h"
#include "list_formatter.h"
#include "str_scanner.h"
#include "value.h"

#include <cstdint>

namespace libasm {
namespace driver {

struct Assembler;
struct BinMemory;

struct AsmFormatter final : ListFormatter::Provider {
    AsmFormatter(AsmSources &sources);

    void setUpperHex(bool upperHex);
    void setListRadix(Radix listRadix);
    void setLineNumber(bool enable);

    void set(const StrScanner &line, const ErrorAt &error, const ConfigBase &config,
            const Value *value = nullptr);
    bool hasError() const;

    const ConfigBase &config() const override { return *_config; }
    Insn &insn() { return _insn; }

    bool hasNextLine() const;
    StrBuffer &getLine(StrBuffer &out);

private:
    AsmSources &_sources;
    ListFormatter _formatter;
    Insn _insn;
    bool _lineNumber;

    StrScanner _line;
    ErrorAt _error;
    const Value *_value;
    const ConfigBase *_config;

    bool _errorLine;
    int _nextLine;

    void formatLineNumber(StrBuffer &out);

    // ListFormatter
    uint32_t startAddress() const override { return _insn.address(); }
    uint8_t bytesSize() const override { return _insn.length(); }
    uint8_t getByte(uint8_t offset) const override { return _insn.bytes()[offset]; }
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
