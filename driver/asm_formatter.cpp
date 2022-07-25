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

#include "asm_formatter.h"

#include "bin_memory.h"

namespace libasm {
namespace driver {

AsmFormatter::AsmFormatter(BinMemory &memory)
    : ListFormatter(), _memory(memory), _lineNumber(false) {}

void AsmFormatter::reset() {
    ListFormatter::reset();
    _errorLine = false;
}

void AsmFormatter::setUppercase(bool enable) {
    ListFormatter::setUppercase(enable);
    uppercase = enable;
}

void AsmFormatter::emitByte(uint32_t base, uint8_t val) {
    _memory.writeByte(base + length++, val);
}

bool AsmFormatter::isError() const {
    return error.getError() != OK && error.getError() != END_ASSEMBLE;
}

const char *AsmFormatter::getLine() {
    if (isError() && !_errorLine) {
        _out.reset(_outBuffer, sizeof(_outBuffer));
        // TODO: In file included from...
        _out.text(input_name).letter(':');
        _formatter.formatDec(_out, line_number, 0);
        const char *at = error.errorAt().str();
        const int column =
                (at >= line.str() && at < line.str() + line.size()) ? at - line.str() + 1 : -1;
        if (column >= 0) {
            _out.letter(':');
            formatDec(column);
        }
        _out.letter(':').letter(' ').text_P(error.errorText_P());
        _nextLine = 0;
        _errorLine = true;
        return _outBuffer;
    }

    return ListFormatter::getLine();
}

int AsmFormatter::formatBytes(int base) {
    if (!val.isUndefined()) {
        _out.text(" =0x");
        formatHex(val.getUnsigned(), 0, true);
        return 0;
    }
    return ListFormatter::formatBytes(base);
}

void AsmFormatter::formatLine() {
    if (_lineNumber) {
        if (include_nest) {
            _out.letter('(');
            formatDec(include_nest);
            _out.letter(')');
        } else {
            _out.text("   ");
        }
        formatDec(line_number, 5);
        _out.text("/ ");
    }
    return ListFormatter::formatLine();
}

uint32_t AsmFormatter::startAddress() const {
    return address;
}

int AsmFormatter::generatedSize() const {
    return length;
}

uint8_t AsmFormatter::getByte(int offset) const {
    return _memory.readByte(address * conf->addressUnit() + offset);
}

bool AsmFormatter::hasInstruction() const {
    return instruction.size() != 0;
}

const StrScanner AsmFormatter::getInstruction() const {
    return instruction;
}

bool AsmFormatter::hasOperand() const {
    return operand.size() != 0;
}

const StrScanner AsmFormatter::getOperand() const {
    return operand;
}

bool AsmFormatter::hasLabel() const {
    return label.size() != 0;
}

const StrScanner AsmFormatter::getLabel() const {
    return label;
}

bool AsmFormatter::hasComment() const {
    return comment.size() != 0;
}

const StrScanner AsmFormatter::getComment() const {
    return comment;
}

}  // namespace driver
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
