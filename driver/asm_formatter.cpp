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

AsmFormatter::AsmFormatter(AsmSources &sources)
    : _sources(sources), _formatter(*this), _insn(0), _lineNumber(false) {}

void AsmFormatter::setUpperHex(bool upperHex) {
    _formatter.setUpperHex(upperHex);
}

void AsmFormatter::setListRadix(Radix radix) {
    _formatter.setListRadix(radix);
}

void AsmFormatter::setLineNumber(bool enable) {
    _lineNumber = enable;
}

void AsmFormatter::set(const StrScanner &line, const ErrorAt &error, const ConfigBase &config,
        const Value *value) {
    _line = line;
    _error.setError(error);
    _value = value;
    _config = &config;
    _errorLine = _error.getError();
    _nextLine = -1;
}

bool AsmFormatter::hasError() const {
    return !_error.isOK() && _error.getError() != END_ASSEMBLE;
}

bool AsmFormatter::hasNextLine() const {
    return _nextLine < bytesSize();
}

StrBuffer &AsmFormatter::getLine(StrBuffer &out) {
    out.reset();
    if (_errorLine) {
        _errorLine = false;
        // TODO: In file included from...
        auto reader = _sources.current();
        out.rtext(reader->name().c_str()).letter(':');
        _formatter.formatDec(out, reader->lineno());
        const auto *line = _line.str();
        const auto *line_end = line + _line.size();
        const auto *at = _error.errorAt();
        const int column = (at >= line && at < line_end) ? at - line + 1 : -1;
        if (column >= 0) {
            out.letter(':');
            _formatter.formatDec(out, column);
        }
        out.rtext_P(PSTR(": error: ")).rtext_P(_error.errorText_P());
        if (_error.errorAt() && *_error.errorAt())
            out.rtext_P(PSTR(R"(: ")")).rtext(_error.errorAt()).rletter('"');
        _nextLine = -1;
    } else {
        if (_nextLine < 0)
            _nextLine = 0;
        formatLineNumber(out);
        _formatter.formatAddress(out, startAddress() + _nextLine);
        const auto pos = out.len();
        uint8_t formatted = 0;
        if (_value && !_value->isUndefined()) {
            out.text(" =");
            _formatter.formatValue(out, _value->getUnsigned());
        } else {
            formatted = _formatter.formatBytes(out, _nextLine);
        }
        if (_nextLine == 0 && _line.size()) {
            _formatter.formatTab(out, pos + _formatter.bytesColumnWidth() + 1);
            out.text(_line);
        }
        _nextLine += formatted;
    }
    return out;
}

void AsmFormatter::formatLineNumber(StrBuffer &out) {
    // Only the first has "(nest) lineno/"
    if (_nextLine <= 0) {
        const auto include_nest = _sources.nest();
        if (include_nest > 1) {
            out.letter('(');
            _formatter.formatDec(out, include_nest - 1);
            out.letter(')');
        } else {
            _formatter.outSpaces(out, 3);
        }
        if (_lineNumber) {
            _formatter.formatDec(out, _sources.current()->lineno(), 5);
            out.letter('/');
        }
    } else {
        _formatter.outSpaces(out, _lineNumber ? 3 + 6 : 3);
    }
}

}  // namespace driver
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
