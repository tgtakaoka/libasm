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

#include "config_base.h"
#include "dis_base.h"
#include "list_formatter.h"

#include <cstdint>
#include <string>

namespace libasm {
namespace driver {

class DisFormatter : public ListFormatter {
public:
    DisFormatter(Disassembler &disassembler, const char *input_name);

    void setUppercase(bool uppercase) override;

    void reset() override;
    Error disassemble(DisMemory &memory, uint32_t addr);
    Error setCpu(const char *cpu);
    Error setOrigin(uint32_t origin);

    bool isError() const { return _disassembler.getError() != OK; }
    int byteLength() const { return generatedSize(); }

    const char *getLine() override;
    bool hasNextContent() const;
    const char *getContent();

protected:
    Disassembler &_disassembler;
    const char *_input_name;
    Insn _insn;
    InsnBase _insnBase;
    int _nextContent;
    bool _errorLine;
    bool _errorContent;
    char _operands[256];

    // ListLine
    uint32_t startAddress() const override { return _insn.address(); }
    int generatedSize() const override { return _insn.length(); }
    uint8_t getByte(int offset) const override { return _insn.bytes()[offset]; }
    bool hasInstruction() const override { return *_insn.name() != 0; }
    const StrScanner getInstruction() const override { return StrScanner(_insn.name()); }
    bool hasOperand() const override { return *_operands; }
    const StrScanner getOperand() const override { return StrScanner(_operands); }

    const ConfigBase &config() const override { return _disassembler.config(); }
    int labelWidth() const override { return 8; }
    int codeBytes() const override { return config().codeMax() < 4 ? config().codeMax() : 4; }
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
