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

    void setUppercase(bool enable) override;
    void enableLineNumber(bool enable) { _lineNumber = enable; }

    Error assemble(const StrScanner &line, bool reportError = false);
    bool isError() const;
    const char *getLine() override;

    // Interface to AsmDirective
    void emitByte(uint32_t base, uint8_t val);
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
    bool _errorLine;

    StrScanner _line;
    Value _line_value;
    StrScanner _line_symbol;
    Insn _insn{0};
    uint32_t _address;
    int _length;
    StrScanner _label;
    StrScanner _instruction;
    StrScanner _operand;
    StrScanner _comment;
    ErrorAt _errorAt;
    const ConfigBase *_conf;

    void reset() override;
    int formatBytes(int base) override;
    void formatLine() override;

    // ListFormatter
    uint32_t startAddress() const override { return _address; }
    int generatedSize() const override { return _length; }
    uint8_t getByte(int offset) const override;
    bool hasInstruction() const override { return _instruction.size() != 0; }
    const StrScanner getInstruction() const override { return _instruction; }
    bool hasOperand() const override { return _operand.size() != 0; }
    const StrScanner getOperand() const override { return _operand; }

    bool hasLabel() const override { return _label.size() != 0; }
    const StrScanner getLabel() const override { return _label; }
    bool hasComment() const override { return _comment.size() != 0; }
    const StrScanner getComment() const override { return _comment; }

    // configuration
    const ConfigBase &config() const override { return *_conf; }
    int labelWidth() const override { return 16; }
    int nameWidth() const override { return config().nameMax() < 5 ? 6 : config().nameMax() + 1; }
    int codeBytes() const override { return config().codeMax() < 4 ? config().codeMax() : 4; }
    int operandWidth() const override { return 16; }
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
