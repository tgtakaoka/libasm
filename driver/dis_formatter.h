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

#include "dis_base.h"
#include "list_formatter.h"

#include <cstdint>

namespace libasm {
namespace driver {

struct DisFormatter : ListFormatter::Provider {
    DisFormatter(Disassembler &disassembler, const char *input_name);

    void setUpperHex(bool enable);
    void setUppercase(bool enable);

    Error disassemble(DisMemory &memory, uint32_t addr);
    Error setCpu(const char *cpu);
    Error setOrigin(uint32_t origin);

    bool isError() const { return _disassembler.getError() != OK; }
    const char *name() const { return _insn.name(); }
    const char *operand() const { return _operands; }
    uint8_t bytesSize() const override { return _insn.length(); }

    bool hasNextContent() const;
    const char *getContent();
    bool hasNextLine() const;
    const char *getLine();

protected:
    Disassembler &_disassembler;
    const char *_input_name;
    ListFormatter _formatter;
    Insn _insn;
    bool _uppercase;

    int _nextContent;
    bool _errorContent;
    int _nextLine;
    bool _errorLine;

    char _operands[256];

    void reset();

    // ListFormatter::Provider
    const ConfigBase &config() const override { return _disassembler.config(); }
    uint32_t startAddress() const override { return _insn.address(); }
    uint8_t getByte(uint8_t offset) const override { return _insn.bytes()[offset]; }

    static constexpr int min_nameWidth = 5;
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
