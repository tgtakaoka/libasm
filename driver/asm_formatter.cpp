#include <stdio.h>
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

#include <ctype.h>

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
    _nextLine = -1;
    _errorLine = false;
    _errorAt.setOK();
    _driver.current()->setOK();
}

void AsmFormatter::enableLineNumber(bool enable) {
    _lineNumber = enable;
}

Error AsmFormatter::assemble(const StrScanner &li, bool reportError) {
    auto &assembler = _driver.current()->assembler();
    auto &parser = assembler.parser();

    reset();
    _reportError = reportError;
    _conf = &assembler.config();
    auto scan = _line = li;
    _length = 0;
    _line_value.clear();
    setStartAddress(_driver.origin());
    assembler.setCurrentLocation(_driver.origin());

    if (parser.commentLine(scan))
        return OK;

    parser.readLabel(scan, _line_symbol);
    _driver.setLineSymbol(_line_symbol);

    if (!parser.endOfLine(scan.skipSpaces())) {
        auto p = scan;
        StrScanner directive;
        parser.readInstruction(p, directive);
        auto error = _driver.current()->processPseudo(directive, p.skipSpaces(), *this, _driver);
        setListRadix(assembler.listRadix());
        if (&_driver.current()->assembler() != &assembler) {
            // Assembler may be switched by "cpu" directive
            _driver.current()->assembler().setListRadix(assembler.listRadix());
        }
        if (error == OK) {
            if (_line_symbol.size()) {
                // If |label| isn't consumed, assign the origin.
                const auto error = _driver.internLineSymbol(startAddress());
                if (error) {
                    _line_value.clear();
                    return _errorAt.setError(_line_symbol, error);
                }
            }
            return OK;
        }
        if (error != UNKNOWN_DIRECTIVE)
            return _errorAt.setError(error);
    }

    if (_line_symbol.size()) {
        // If |label| isn't consumed, assign the origin.
        const auto error = _driver.internLineSymbol(startAddress());
        if (error)
            return _errorAt.setError(_line_symbol, error);
    }

    if (parser.endOfLine(scan))
        return OK;  // skip comment

    _insn.reset(startAddress());
    assembler.encode(scan.str(), _insn, /*SymbolTable*/ &_driver);
    _errorAt.setError(assembler);
    setListRadix(assembler.listRadix());
    // maybe be modified because of alignment required by constant generating pseudos.
    setStartAddress(_insn.address());
    const auto allowUndef =
            _errorAt.getError() == UNDEFINED_SYMBOL && _driver.symbolMode() != REPORT_UNDEFINED;
    if (_errorAt.isOK() || allowUndef) {
        if (_insn.length() == 0) {
            // set location or allocate memory space.
            _driver.setOrigin(assembler.currentLocation());
        } else {
            const uint8_t unit = assembler.config().addressUnit();
            const uint32_t base = _insn.address() * unit;
            for (auto offset = 0; offset < _insn.length(); offset++) {
                _memory.writeByte(base + _length, _insn.bytes()[offset]);
                _length++;
            }
            _driver.setOrigin(startAddress() + _insn.length() / unit);
        }
        return OK;
    }
    return _errorAt.getError();
}

bool AsmFormatter::isError() const {
    return _reportError && _errorAt.getError() != OK && _errorAt.getError() != END_ASSEMBLE;
}

bool AsmFormatter::hasNextLine() const {
    return _nextLine < generatedSize();
}

const char *AsmFormatter::getLine() {
    resetOut();
    if (isError() && !_errorLine) {
        // TODO: In file included from...
        _out.text(_sources.current()->name().c_str()).letter(':');
        formatDec(_sources.current()->lineno());
        const auto *line = _line.str();
        const auto *line_end = line + _line.size();
        const auto *at = _errorAt.errorAt();
        const int column = (at >= line && at < line_end) ? at - line + 1 : -1;
        if (column >= 0) {
            _out.letter(':');
            formatDec(column);
        }
        _out.text(": error: ").text_P(_errorAt.errorText_P());
        _nextLine = -1;
        _errorLine = true;
    } else {
        if (_nextLine < 0)
            _nextLine = 0;
        formatLineNumber();
        formatAddress(startAddress() + _nextLine);
        const auto pos = outLength();
        auto formatted = 0;
        if (!_line_value.isUndefined()) {
            _out.text(" =");
            formatValue(_line_value.getUnsigned());
        } else {
            formatted = formatBytes(_nextLine);
        }
        if (_nextLine == 0 && _line.size()) {
            formatTab(pos + bytesColumnWidth() + 1);
            _out.text(_line);
        }
        _nextLine += formatted;
    }
    return outBuffer();
}

void AsmFormatter::formatLineNumber() {
    // Only the first has "(nest) lineno/"
    if (_nextLine <= 0) {
        const auto include_nest = _sources.nest();
        if (include_nest > 1) {
            _out.letter('(');
            formatDec(include_nest - 1);
            _out.letter(')');
        } else {
            outSpaces(3);
        }
        if (_lineNumber) {
            formatDec(_sources.current()->lineno(), 5);
            _out.letter('/');
        }
    } else {
        outSpaces(_lineNumber ? 3 + 6 : 3);
    }
}

uint8_t AsmFormatter::getByte(int offset) const {
    return _memory.readByte(_address * config().addressUnit() + offset);
}

}  // namespace driver
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
