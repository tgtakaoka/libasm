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

#ifndef __ASM_FORMATTER_H__
#define __ASM_FORMATTER_H__

#include "asm_sources.h"
#include "insn_base.h"
#include "list_formatter.h"
#include "str_scanner.h"
#include "value.h"

#include <cstdint>

namespace libasm {
namespace driver {

struct AsmDriver;
struct BinMemory;

struct AsmFormatter final : ListFormatter::Provider {
    AsmFormatter(AsmDriver &driver, AsmSources &sources, BinMemory &memory);

    void setUpperHex(bool upperHex) { _formatter.setUpperHex(upperHex); }
    void enableLineNumber(bool enable);

    Error assemble(const StrScanner &line, bool reportError = false);
    bool isError() const;
    bool hasNextLine() const;
    const char *getLine();

    // Interface to AsmDirective
    void setStartAddress(uint32_t addr) { _address = addr; }
    uint8_t byteLength() const { return bytesSize(); }
    StrScanner &lineSymbol() { return _line_symbol; }
    Value &lineValue() { return _line_value; }

    // TODO: Remove these
    Error openSource(const StrScanner &filename) { return _sources.open(filename); }
    Error closeCurrent() { return _sources.closeCurrent(); }

private:
    AsmDriver &_driver;
    AsmSources &_sources;
    BinMemory &_memory;
    ListFormatter _formatter;
    bool _lineNumber;
    bool _reportError;
    int _nextLine;
    bool _errorLine;

    StrScanner _line;
    Value _line_value;
    StrScanner _line_symbol;
    Insn _insn{0};
    uint32_t _address;
    uint8_t _length;
    ErrorAt _errorAt;
    const ConfigBase *_conf;

    void reset();
    void formatLineNumber();

    // ListFormatter
    uint32_t startAddress() const override { return _address; }
    uint8_t bytesSize() const override { return _length; }
    uint8_t getByte(uint8_t offset) const override;
    const ConfigBase &config() const override { return *_conf; }
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
