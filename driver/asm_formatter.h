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

#include "insn_base.h"
#include "list_formatter.h"
#include "str_scanner.h"
#include "value.h"

#include <cstdint>

namespace libasm {
namespace driver {

class AsmDriver;
class AsmSources;
class BinMemory;

class AsmFormatter : public ListFormatter {
public:
    AsmFormatter(AsmDriver &driver, AsmSources &sources, BinMemory &memory);

    void enableLineNumber(bool enable) { _lineNumber = enable; }

    Error assemble(const StrScanner &line, bool reportError = false);
    bool isError() const;
    bool hasNextLine() const override;
    const char *getLine() override;

    // Interface to AsmDirective
    void setStartAddress(uint32_t addr) { _address = addr; }
    int byteLength() const { return generatedSize(); }
    StrScanner &lineSymbol() { return _line_symbol; }
    Value &lineValue() { return _line_value; }

private:
    AsmDriver &_driver;
    AsmSources &_sources;
    BinMemory &_memory;
    bool _lineNumber;
    bool _reportError;
    int _nextLine;
    bool _errorLine;

    StrScanner _line;
    Value _line_value;
    StrScanner _line_symbol;
    Insn _insn{0};
    uint32_t _address;
    int _length;
    ErrorAt _errorAt;
    const ConfigBase *_conf;

    void reset();
    void formatLineNumber();

    // ListFormatter
    uint32_t startAddress() const override { return _address; }
    int generatedSize() const override { return _length; }
    uint8_t getByte(int offset) const override;
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
