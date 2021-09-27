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

DisDirective::DisDirective(Disassembler &disassembler, CliMemory &memory, bool uppercase)
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
    _address = addr;
    _generated_size = insn.length();
    _instruction = insn.name();
    return error;
}

const char *DisDirective::getCpu(bool withBytes) {
    _listing.reset(*this);
    _generated_size = 0;
    _instruction = "CPU";
    strcpy(_operands, _disassembler.getCpu());
    return withBytes ? _listing.getLine() : _listing.getContent();
}

const char *DisDirective::origin(uint32_t origin, bool withBytes) {
    StrBuffer buf(_operands, sizeof(_operands));
    _disassembler.getFormatter().formatHex(
            buf, origin, _disassembler.config().addressWidth(), false);
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

// ListingLine overrides

uint32_t DisDirective::startAddress() const {
    return _address;
}

int DisDirective::generatedSize() const {
    return _generated_size;
}

uint8_t DisDirective::getByte(int offset) const {
    uint8_t val = 0;
    _memory.readByte(_address + offset, val);
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

uint16_t DisDirective::lineNumber() const {
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

AddressWidth DisDirective::addressWidth() const {
    return _disassembler.config().addressWidth();
}

AddressUnit DisDirective::addressUnit() const {
    return _disassembler.config().addressUnit();
}

OpCodeWidth DisDirective::opCodeWidth() const {
    return _disassembler.config().opCodeWidth();
}

Endian DisDirective::endian() const {
    return _disassembler.config().endian();
}

int DisDirective::maxBytes() const {
    const uint8_t codeMax = _disassembler.config().codeMax();
    return codeMax < 6 ? codeMax : 6;
}

int DisDirective::labelWidth() const {
    return _labelWidth;
}

int DisDirective::instructionWidth() const {
    return _disassembler.config().nameMax() + 1;
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
