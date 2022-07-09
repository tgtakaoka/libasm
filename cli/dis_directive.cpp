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

#include "dis_directive.h"

namespace libasm {
namespace cli {

DisDirective::DisDirective(Disassembler &disassembler, BinMemory &memory, bool uppercase)
    : _disassembler(disassembler),
      _memory(memory),
      _listing(),
      _labelWidth(8),
      _operandWidth(8),
      _address(0) {
    _listing.setUppercase(uppercase);
    _disassembler.setUppercase(uppercase);
}

Error DisDirective::disassemble(uint32_t addr, Insn &insn) {
    _memory.setAddress(addr);
    const Error error = _disassembler.decode(_memory, insn, _operands, sizeof(_operands));
    _listing.reset(*this);
    _address = insn.address();
    _generated_size = insn.length();
    _instruction = insn.name();
    return error;
}

const char *DisDirective::getCpu(bool withBytes) {
    _listing.reset(*this);
    _generated_size = 0;
    _instruction = "CPU";
    strcpy_P(_operands, _disassembler.cpu_P());
    return withBytes ? _listing.getLine() : _listing.getContent();
}

const char *DisDirective::origin(uint32_t origin, bool withBytes) {
    StrBuffer buf(_operands, sizeof(_operands));
    _disassembler.formatter().formatHex(buf, origin, config().addressWidth(), false);
    _listing.reset(*this);
    _address = origin;
    _generated_size = 0;
    _instruction = "ORG";
    return withBytes ? _listing.getLine() : _listing.getContent();
}

bool DisDirective::hasNext() const {
    return _listing.hasNext();
}

const char *DisDirective::getLine() {
    return _listing.getLine();
}

const char *DisDirective::getContent() {
    return _listing.getContent();
}

// ListLine overrides

uint32_t DisDirective::startAddress() const {
    return _address;
}

int DisDirective::generatedSize() const {
    return _generated_size;
}

uint8_t DisDirective::getByte(int offset) const {
    const uint32_t base = _address * uint8_t(_disassembler.config().addressUnit());
    uint8_t val = 0;
    _memory.readByte(base + offset, val);
    return val;
}

bool DisDirective::hasInstruction() const {
    return _instruction;
}

std::string DisDirective::getInstruction() const {
    return std::string(_instruction);
}

bool DisDirective::hasOperand() const {
    return *_operands;
}

std::string DisDirective::getOperand() const {
    return std::string(_operands);
}

uint32_t DisDirective::lineNumber() const {
    return 0;
}

uint16_t DisDirective::includeNest() const {
    return 0;
}

bool DisDirective::hasValue() const {
    return false;
}

uint32_t DisDirective::value() const {
    return 0;
}

bool DisDirective::hasLabel() const {
    return false;
}

std::string DisDirective::getLabel() const {
    return "";
}

bool DisDirective::hasComment() const {
    return false;
}

std::string DisDirective::getComment() const {
    return "";
}

const ConfigBase &DisDirective::config() const {
    return _disassembler.config();
}

int DisDirective::codeBytes() const {
    const uint8_t codeMax = config().codeMax();
    return codeMax < 6 ? codeMax : 6;
}

int DisDirective::nameWidth() const {
    return config().nameMax() + 1;
}

int DisDirective::labelWidth() const {
    return _labelWidth;
}

int DisDirective::operandWidth() const {
    return _operandWidth;
}

}  // namespace cli
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
