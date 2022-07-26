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

    void reset() override;
    Error assemble(const StrScanner &line, bool reportError = false);
    void emitByte(uint32_t base, uint8_t val);

    bool isError() const;
    const char *getLine() override;

private:
    AsmDriver &_driver;
    AsmSources &_sources;
    BinMemory &_memory;
    bool _lineNumber;
    bool _reportError;
    bool _errorLine;

    friend class AsmDirective;
    StrScanner line;
    uint32_t address;
    int length;
    uint32_t line_number;
    uint16_t include_nest;
    Insn insn{0};
    Value val;
    StrScanner label;
    StrScanner line_symbol;
    StrScanner instruction;
    StrScanner operand;
    StrScanner comment;
    ErrorAt errorAt;
    const ConfigBase *conf;

    int formatBytes(int base) override;
    void formatLine() override;

    // ListLine
    uint32_t startAddress() const override;
    int generatedSize() const override;
    uint8_t getByte(int offset) const override;
    bool hasInstruction() const override;
    const StrScanner getInstruction() const override;
    bool hasOperand() const override;
    const StrScanner getOperand() const override;

    bool hasLabel() const override;
    const StrScanner getLabel() const override;
    bool hasComment() const override;
    const StrScanner getComment() const override;

    // configuration
    const ConfigBase &config() const override { return *conf; }
    int labelWidth() const override { return 16; }
    int nameWidth() const override { return conf->nameMax() < 5 ? 6 : conf->nameMax() + 1; }
    int codeBytes() const override { return conf->codeMax() < 6 ? conf->codeMax() : 6; }
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
