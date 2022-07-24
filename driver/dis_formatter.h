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

#include "list_formatter.h"
#include "config_base.h"
#include "dis_base.h"
#include "list_formatter.h"
#include "str_buffer.h"

#include <stdint.h>

#include <string>

namespace libasm {
namespace driver {

class DisFormatter : public ListFormatter, public ListLine {
public:

    DisFormatter(Disassembler &disassembler, const char *input_name)
        : ListFormatter(), _disassembler(disassembler), _input_name(input_name), _insn(0), _insnBase(_insn) {}

    void setUppercase(bool uppercase) override {
        ListFormatter::setUppercase(uppercase);
        _disassembler.setOption("uppercase", _uppercase ? "on" : "off");
    }

    Error disassemble(DisMemory &memory, uint32_t addr) {
        reset(*this);
        _insnBase.reset(addr);
        const Error error = _disassembler.decode(memory, _insn, _operands, sizeof(_operands));
        return error;
    }

    bool setCpu(const char *cpu) {
        reset(*this);
        _insnBase.reset(_insn.address());
        _insnBase.nameBuffer().text_P(PSTR("CPU"), _uppercase);
        StrBuffer buf(_operands, sizeof(_operands));
        buf.text(cpu, _uppercase);
        return _disassembler.setCpu(cpu);
    }

    void setOrigin(uint32_t origin) {
        reset(*this);
        StrBuffer buf(_operands, sizeof(_operands));
        _disassembler.formatter().formatHex(buf, origin, config().addressWidth(), false);
        _insnBase.reset(origin);
        _insnBase.nameBuffer().text_P(PSTR("ORG"), _uppercase);
    }

    // ListLine
    int generatedSize() const override { return _insn.length(); }
    bool isError() const override { return _disassembler.getError() != OK; }

protected:
    Disassembler &_disassembler;
    const char *_input_name;
    Insn _insn;
    InsnBase _insnBase;
    char _operands[128];

    // ListLine
    uint32_t startAddress() const override { return _insn.address(); }
    uint8_t getByte(int offset) const override { return _insn.bytes()[offset]; }
    bool hasInstruction() const override { return *_insn.name() != 0; }
    std::string getInstruction() const override { return std::string(_insn.name()); }
    bool hasOperand() const override { return *_operands; }
    std::string getOperand() const override { return std::string(_operands); }

    std::string inputName() const { return std::string(_input_name); }
    std::string errorText() const override { return std::string(_disassembler.errorText_P()); }

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
