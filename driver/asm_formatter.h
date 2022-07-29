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

#include "bin_memory.h"
#include "list_formatter.h"
#include "str_scanner.h"
#include "value_parser.h"

#include <stdint.h>

namespace libasm {
namespace driver {

class AsmFormatter : public ListFormatter {
private:
    BinMemory &_memory;
    bool _lineNumber;

public:
    AsmFormatter(BinMemory &memory) : ListFormatter(), _memory(memory), _lineNumber(false) {}

    void setUppercase(bool enable) override {
        ListFormatter::setUppercase(enable);
        uppercase = enable;
    }
    void enableLineNumber(bool enable) { _lineNumber = enable; }

    bool uppercase;
    uint32_t line_number;
    uint16_t include_nest;
    uint32_t address;
    int length;
    Value val;
    StrScanner label;
    StrScanner instruction;
    StrScanner operand;
    StrScanner comment;

    StrScanner line_symbol;
    const ConfigBase *conf;

    void generateByte(uint32_t base, uint8_t val) { _memory.writeByte(base + length++, val); }

private:
    // ListLine
    uint32_t startAddress() const override { return address; }
    int generatedSize() const override { return length; }

    uint8_t getByte(int offset) const override {
        return _memory.readByte(address * conf->addressUnit() + offset);
    }
    bool hasInstruction() const override { return instruction.size() != 0; }
    const StrScanner getInstruction() const override { return instruction; }
    bool hasOperand() const override { return operand.size() != 0; }
    const StrScanner getOperand() const override { return operand; }

    uint32_t lineNumber() const override { return line_number; }
    uint16_t includeNest() const override { return include_nest; }
    bool hasValue() const override { return !val.isUndefined(); }
    uint32_t value() const override { return val.getUnsigned(); }
    bool hasLabel() const override { return label.size() != 0; }
    const StrScanner getLabel() const override { return label; }
    bool hasComment() const override { return comment.size() != 0; }
    const StrScanner getComment() const override { return comment; }

    // configuration
    const ConfigBase &config() const override { return *conf; }
    int labelWidth() const override { return 16; }
    int nameWidth() const override { return conf->nameMax() < 5 ? 6 : conf->nameMax() + 1; }
    int codeBytes() const override { return conf->codeMax() < 6 ? conf->codeMax() : 6; }
    int operandWidth() const override { return 16; }

    void formatLine() override {
        if (_lineNumber) {
            auto nest = includeNest();
            if (nest) {
                formatDec(nest);
            } else {
                _out.text("   ");
            }
            formatDec(lineNumber(), 5);
            _out.text("/ ");
        }
        ListFormatter::formatLine();
    }
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
