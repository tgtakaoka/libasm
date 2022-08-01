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

Error AsmFormatter::assemble(const StrScanner &li, bool reportError) {
    reset();
    _reportError = reportError;
    _line = li;
    setStartAddress(_driver.origin());
    _length = 0;
    _line_value.clear();
    _label = StrScanner::EMPTY;
    _instruction = StrScanner::EMPTY;
    _line_symbol = StrScanner::EMPTY;
    auto &assembler = _driver.current()->assembler();
    _conf = &assembler.config();

    StrScanner scan(_line);
    auto &parser = assembler.parser();
    parser.setCurrentOrigin(startAddress());

    if (parser.isSymbolLetter(*scan, true)) {
        // setAt(scan);
        _label = scan;
        _line_symbol = parser.readSymbol(scan);
        scan.expect(':');  // skip optional trailing ':' for label.
        _label.trimEndAt(scan);
    }
    scan.skipSpaces();

    if (!assembler.endOfLine(*scan)) {
        _instruction = scan;
        StrScanner directive = scan;
        StrScanner p(scan);
        p.trimStart([](char s) { return !isspace(s); });
        directive.trimEndAt(p);
        _errorAt.setError(_driver.current()->processPseudo(directive, p.skipSpaces(), *this, _driver));
        if (_errorAt.isOK()) {
            if (_line_symbol.size()) {
                // If |label| isn't consumed, assign the origin.
                const auto error = _driver.internSymbol(startAddress(), _line_symbol);
                if (error)
                    return _errorAt.setError(_line_symbol, error);
            }
            return OK;
        }
        if (_errorAt.getError() != UNKNOWN_DIRECTIVE)
            return _errorAt.getError();
    }

    if (_line_symbol.size()) {
        // If |label| isn't consumed, assign the origin.
        const auto error = _driver.internSymbol(startAddress(), _line_symbol);
        if (error)
            return _errorAt.setError(_line_symbol, error);
    }

    if (assembler.endOfLine(*scan))
        return OK;  // skip comment

    _insn.reset(startAddress());
    _errorAt.setError(assembler.encode(scan.str(), _insn, /*SymbolTable*/ &_driver));
    const bool allowUndef =
            _errorAt.getError() == UNDEFINED_SYMBOL && _driver.symbolMode() != REPORT_UNDEFINED;
    if (_errorAt.isOK() || allowUndef) {
        StrScanner p(assembler.errorAt());
        _instruction = scan;
        if (_insn.length() > 0) {
            const uint8_t unit = assembler.config().addressUnit();
            const uint32_t base = _insn.address() * unit;
            for (auto offset = 0; offset < _insn.length(); offset++) {
                emitByte(base, _insn.bytes()[offset]);
            }
            _driver.setOrigin(startAddress() + _insn.length() / unit);
        }
        return OK;
    }
    return _errorAt.getError();
}

void AsmFormatter::emitByte(uint32_t base, uint8_t val) {
    _memory.writeByte(base + _length++, val);
}

bool AsmFormatter::isError() const {
    return _reportError && _errorAt.getError() != OK && _errorAt.getError() != END_ASSEMBLE;
}

const char *AsmFormatter::getLine() {
    if (isError() && !_errorLine) {
        _out.reset(_outBuffer, sizeof(_outBuffer));
        // TODO: In file included from...
        _out.text(_sources.current()->name().c_str()).letter(':');
        _formatter.formatDec(_out, _sources.current()->lineno(), 0);
        const char *at = _errorAt.errorAt().str();
        const char *line = _line.str();
        const char *line_end = _line.str() + _line.size();
        const int column = (at >= line && at < line_end) ? at - line + 1 : -1;
        if (column >= 0) {
            _out.letter(':');
            formatDec(column);
        }
        _out.letter(':').letter(' ').text_P(_errorAt.errorText_P());
        _nextLine = 0;
        _errorLine = true;
        return _outBuffer;
    }

    return ListFormatter::getLine();
}

int AsmFormatter::formatBytes(int base) {
    if (!_line_value.isUndefined()) {
        _out.text(" =0x");
        formatHex(_line_value.getUnsigned(), 0, true);
        return 0;
    }
    return ListFormatter::formatBytes(base);
}

void AsmFormatter::formatLine() {
    if (_lineNumber) {
        const auto include_nest = _sources.size() - 1;
        if (include_nest) {
            _out.letter('(');
            formatDec(include_nest);
            _out.letter(')');
        } else {
            _out.text("   ");
        }
        formatDec(_sources.current()->lineno(), 5);
        _out.text("/ ");
    }
    return ListFormatter::formatLine();
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
