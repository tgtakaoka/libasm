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

#ifndef __DIS_FORMATTER_H__
#define __DIS_FORMATTER_H__

#include "bin_memory.h"
#include "list_formatter.h"
#include "config_base.h"
#include "dis_base.h"
#include "str_buffer.h"

#include <stdint.h>

#include <string>

namespace libasm {
namespace driver {

class DisFormatter : public ListFormatter, public ListLine {
public:
    DisFormatter(Disassembler &disassembler)
        : ListFormatter(), _disassembler(disassembler), _address(0) {}

    Error disassemble(BinMemory &memory, uint32_t addr, Insn &insn) {
        memory.setAddress(addr);
        _disassembler.setOption("uppercase", _uppercase ? "on" : "off");
        const Error error = _disassembler.decode(memory, insn, _operands, sizeof(_operands));
        reset(*this);
        _address = insn.address();
        _generated_bytes = insn.bytes();
        _generated_size = insn.length();
        _instruction = insn.name();
        return error;
    }

    void setCpu() {
        reset(*this);
        _generated_size = 0;
        _instruction = _uppercase ? "CPU" : "cpu";
        strcpy_P(_operands, _disassembler.cpu_P());
    }

    void setOrigin(uint32_t origin) {
        StrBuffer buf(_operands, sizeof(_operands));
        _disassembler.formatter().formatHex(buf, origin, config().addressWidth(), false);
        reset(*this);
        _address = origin;
        _generated_size = 0;
        _instruction = _uppercase ? "ORG" : "org";
    }

private:
    Disassembler &_disassembler;
    uint32_t _address;
    char _operands[128];
    const uint8_t *_generated_bytes;
    int _generated_size;
    const char *_instruction;

    // ListLine
    uint32_t startAddress() const override { return _address; }
    int generatedSize() const override { return _generated_size; }
    uint8_t getByte(int offset) const override { return _generated_bytes[offset]; }
    bool hasInstruction() const override { return _instruction; }
    std::string getInstruction() const override { return std::string(_instruction); }
    bool hasOperand() const override { return *_operands; }
    std::string getOperand() const override { return std::string(_operands); }
    uint32_t lineNumber() const override { return 0; }
    uint16_t includeNest() const override { return 0; }
    bool hasValue() const override { return false; }
    uint32_t value() const override { return 0; }
    bool hasLabel() const override { return false; }
    std::string getLabel() const override { return ""; }
    bool hasComment() const override { return false; }
    std::string getComment() const override { return ""; }
    const ConfigBase &config() const override { return _disassembler.config(); }
    int labelWidth() const override { return 8; }
    int codeBytes() const override {
        const uint8_t codeMax = config().codeMax();
        return codeMax < 6 ? codeMax : 6;
    }
    int nameWidth() const override { return config().nameMax() + 1; }
    int operandWidth() const override { return 8; }
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
