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

#ifndef __DIS_DIRECTIVE_H__
#define __DIS_DIRECTIVE_H__

#include "cli_listing.h"
#include "cli_memory.h"
#include "config_base.h"
#include "dis_base.h"

#include <stdint.h>
#include <string>

namespace libasm {
namespace cli {

class DisDirective : public ListingLine {
public:
    DisDirective(
        Disassembler &disassembler, CliMemory &memory, bool uppercase = false);

    Error disassemble(uint32_t addr, Insn &insn);
    const char *getCpu(bool withBytes = false);
    const char *origin(uint32_t origin, bool withBytes = false);
    bool hasNext() const;
    const char *getLine();
    const char *getContent();

private:
    Disassembler &_disassembler;
    CliMemory &_memory;
    CliListing _listing;
    int _labelWidth;
    int _operandWidth;
    uint32_t _address;
    int _generated_size;
    const char *_instruction;
    char _operands[40];

    // ListingLine
    uint32_t startAddress() const override;
    int generatedSize() const override;
    uint8_t getByte(int offset) const override;
    bool hasInstruction() const override;
    std::string getInstruction() const override;
    bool hasOperand() const override;
    std::string getOperand() const override;
    uint16_t lineNumber() const override;
    uint16_t includeNest() const override;
    bool hasValue() const override;
    uint32_t value() const override;
    bool hasLabel() const override;
    std::string getLabel() const override;
    bool hasComment() const override;
    std::string getComment() const override;
    AddressWidth addressWidth() const override;
    OpCodeWidth opCodeWidth() const override;
    int maxBytes() const override;
    int labelWidth() const override;
    int instructionWidth() const override;
    int operandWidth() const override;
};

} // namespace cli
} // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
