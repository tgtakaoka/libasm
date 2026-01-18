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

#ifndef __LIBASM_DIS_FORMATTER_H__
#define __LIBASM_DIS_FORMATTER_H__

#include "dis_base.h"
#include "list_formatter.h"
#include "value.h"

#include <cstdint>

namespace libasm {
namespace driver {

struct DisFormatter : ListFormatter::Provider {
    DisFormatter(Disassembler &disassembler, const char *inputName);

    void setUpperHex(bool upperHex);
    void setUppercase(bool uppercase);
    void setListRadix(Radix listRadix);

    void reset();
    void set(const ErrorAt &error, const /*PROGMEM*/ char *mark_P = nullptr);
    void setCpu(const char *cpu);
    Error setOrigin(uint32_t origin);

    Insn &insn() { return _insn; }
    StrBuffer &operands() { return _operands; }
    bool hasNextContent() const;
    StrBuffer &getContent(StrBuffer &out);
    bool hasNextLine() const;
    StrBuffer &getLine(StrBuffer &out);

protected:
    Disassembler &_disassembler;
    const char *_inputName;
    ListFormatter _formatter;
    Insn _insn;
    StrBuffer _operands;
    ErrorAt _error;
    const /*PROGMEM*/ char *_continueMark_P;

    bool _uppercase;
    bool _errorMessage;
    bool _errorContent;
    int _nextContent;
    bool _errorLine;
    int _nextLine;
    char _buffer[128];

    void formatComment(StrBuffer &out);
    void formatError(StrBuffer &out);
    void formatLine(StrBuffer &out, int &next);

    // ListFormatter::Provider
    const ConfigBase &config() const override { return _disassembler.config(); }
    uint32_t startAddress() const override { return _insn.address(); }
    uint8_t bytesSize() const override { return _insn.length(); }
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
