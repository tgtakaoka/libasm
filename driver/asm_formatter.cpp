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

#include "asm_directive.h"
#include "asm_driver.h"
#include "asm_sources.h"
#include "bin_memory.h"

namespace libasm {
namespace driver {

AsmFormatter::AsmFormatter(AsmDriver &driver, AsmSources &sources, BinMemory &memory)
    : ListFormatter(), _driver(driver), _sources(sources), _memory(memory), _lineNumber(false) {}

void AsmFormatter::reset() {
    ListFormatter::reset();
    _errorLine = false;
}

void AsmFormatter::setUppercase(bool uppercase) {
    ListFormatter::setUppercase(uppercase);
}

Error AsmFormatter::assemble(const StrScanner &li, bool reportError) {
    reset();
    _reportError = reportError;
    line_number = _sources.current()->lineno();
    include_nest = _sources.size() - 1;
    line = li;
    address = _driver.origin();
    length = 0;
    val.clear();
    label = StrScanner::EMPTY;
    instruction = StrScanner::EMPTY;
    operand = StrScanner::EMPTY;
    comment = StrScanner::EMPTY;
    line_symbol = StrScanner::EMPTY;
    auto &assembler = _driver.current()->assembler();
    assembler.setOption("uppercase", _uppercase ? "on" : "off");
    conf = &assembler.config();

    StrScanner scan(line);
    auto &parser = assembler.parser();
    parser.setCurrentOrigin(address);

    if (parser.isSymbolLetter(*scan, true)) {
        // setAt(scan);
        label = scan;
        line_symbol = parser.readSymbol(scan);
        scan.expect(':');  // skip optional trailing ':' for label.
        label.trimEndAt(scan);
    }
    scan.skipSpaces();

    if (!assembler.endOfLine(*scan)) {
        StrScanner p(scan);
        p.trimStart([](char s) { return !isspace(s); });
        instruction = scan;
        instruction.trimEndAt(p);
        operand = p.skipSpaces();
        errorAt.setError(_driver.current()->processPseudo(instruction, p, *this, _driver));
        if (errorAt.isOK()) {
            operand.trimEndAt(p).trimEnd(isspace);
            comment = p.skipSpaces();
            if (line_symbol.size()) {
                // If |label| isn't consumed, assign the origin.
                const auto error = _driver.internSymbol(address, line_symbol);
                if (error)
                    return errorAt.setError(line_symbol, error);
            }
            return OK;
        }
        if (errorAt.getError() != UNKNOWN_DIRECTIVE)
            return errorAt.getError();
    }

    if (line_symbol.size()) {
        // If |label| isn't consumed, assign the origin.
        const auto error = _driver.internSymbol(address, line_symbol);
        if (error)
            return errorAt.setError(line_symbol, error);
    }

    if (assembler.endOfLine(*scan)) {
        comment = scan;
        return OK;  // skip comment
    }

    insn.reset(address);
    errorAt.setError(assembler.encode(scan.str(), insn, /*SymbolTable*/ &_driver));
    const bool allowUndef =
            errorAt.getError() == UNDEFINED_SYMBOL && _driver.symbolMode() != REPORT_UNDEFINED;
    if (errorAt.isOK() || allowUndef) {
        StrScanner p(assembler.errorAt());
        instruction = StrScanner(insn.name());
        operand.trimEndAt(p).trimEnd(isspace);
        comment = p.skipSpaces();
        if (insn.length() > 0) {
            const uint8_t unit = assembler.config().addressUnit();
            const uint32_t base = insn.address() * unit;
            for (auto offset = 0; offset < insn.length(); offset++) {
                emitByte(base, insn.bytes()[offset]);
            }
            _driver.setOrigin(address + insn.length() / unit);
        }
        return OK;
    }
    return errorAt.getError();
}

void AsmFormatter::emitByte(uint32_t base, uint8_t val) {
    _memory.writeByte(base + length++, val);
}

bool AsmFormatter::isError() const {
    return _reportError && errorAt.getError() != OK && errorAt.getError() != END_ASSEMBLE;
}

const char *AsmFormatter::getLine() {
    if (isError() && !_errorLine) {
        _out.reset(_outBuffer, sizeof(_outBuffer));
        // TODO: In file included from...
        _out.text(_sources.current()->name().c_str()).letter(':');
        _formatter.formatDec(_out, line_number, 0);
        const char *at = errorAt.errorAt().str();
        const int column =
                (at >= line.str() && at < line.str() + line.size()) ? at - line.str() + 1 : -1;
        if (column >= 0) {
            _out.letter(':');
            formatDec(column);
        }
        _out.letter(':').letter(' ').text_P(errorAt.errorText_P());
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
