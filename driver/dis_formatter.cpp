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

#include "dis_formatter.h"

#include "str_buffer.h"

namespace libasm {
namespace driver {

DisFormatter::DisFormatter(Disassembler &disassembler, const char *input_name)
    : ListFormatter(),
      _disassembler(disassembler),
      _input_name(input_name),
      _insn(0),
      _insnBase(_insn) {}

void DisFormatter::setUppercase(bool uppercase) {
    ListFormatter::setUppercase(uppercase);
    _disassembler.setOption("uppercase", _uppercase ? "on" : "off");
}

Error DisFormatter::disassemble(DisMemory &memory, uint32_t addr) {
    reset();
    _insnBase.reset(addr);
    const Error error = _disassembler.decode(memory, _insn, _operands, sizeof(_operands));
    return error;
}

Error DisFormatter::setCpu(const char *cpu) {
    reset();
    _insnBase.reset(_insn.address());
    _insnBase.nameBuffer().text_P(PSTR("CPU"), _uppercase);
    StrBuffer buf(_operands, sizeof(_operands));
    buf.text(cpu, _uppercase);
    return _disassembler.setCpu(cpu) ? UNSUPPORTED_CPU : OK;
}

Error DisFormatter::setOrigin(uint32_t origin) {
    reset();
    StrBuffer buf(_operands, sizeof(_operands));
    if (_disassembler.checkAddress(origin))
        return _disassembler.getError();
    _disassembler.formatter().formatHex(buf, origin, config().addressWidth(), false);
    _insnBase.reset(origin);
    _insnBase.nameBuffer().text_P(PSTR("ORG"), _uppercase);
    return OK;
}

void DisFormatter::reset() {
    ListFormatter::reset();
    _disassembler.resetError();
    _errorContent = _errorLine = false;
}

bool DisFormatter::hasNextContent() const {
    return _nextContent < generatedSize();
}

const char *DisFormatter::getContent() {
    _out.reset(_outBuffer, sizeof(_outBuffer));
    if (isError()) {
        if (!_errorContent) {
            _errorContent = true;
            _out.text("; ").text(_input_name).text(": 0x");
            formatAddress(startAddress(), true);
            _out.letter(' ').text_P(_disassembler.errorText_P());
            _nextContent = 0;
        } else {
            _out.text("; ");
            formatAddress(startAddress(), true);
            _nextContent += formatBytes(_nextContent);
        }
        return _outBuffer;
    }

    formatContent(0);
    _nextContent = generatedSize();
    return _outBuffer;
}

const char *DisFormatter::getLine() {
    if (isError() && !_errorLine) {
        _errorLine = true;
        _out.reset(_outBuffer, sizeof(_outBuffer));
        _out.text(_input_name).text(": 0x");
        formatAddress(startAddress(), true);
        _out.letter(' ').text_P(_disassembler.errorText_P());
        _nextLine = 0;
        return _outBuffer;
    }

    return ListFormatter::getLine();
}

}  // namespace driver
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
