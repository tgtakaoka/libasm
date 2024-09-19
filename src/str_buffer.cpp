/*
 * Copyright 2021 Tadashi G. Takaoka
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

#include "str_buffer.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include "config_host.h"

namespace libasm {

StrBuffer::StrBuffer(char *buffer, size_t size)
    : ErrorReporter(), _str(buffer), _out(buffer), _end(buffer + size - 1) {
    *_out = 0;
}

StrBuffer::StrBuffer(const StrBuffer &o)
    : ErrorReporter(), _str(o._str), _out(o._out), _end(o._end) {
    setError(o.getError());
}

StrBuffer &StrBuffer::reset() {
    *(_out = _str) = 0;
    resetError();
    return *this;
}

StrBuffer &StrBuffer::over(StrBuffer &heir) const {
    heir._str = _str;
    heir._out = _out;
    heir._end = _end;
    heir.setError(getError());
    return heir;
}

StrBuffer &StrBuffer::rletter(char letter) {
    if (_out < _end) {
        *_out++ = letter;
        *_out = 0;
    } else {
        setError(BUFFER_OVERFLOW);
    }
    return *this;
}

StrBuffer &StrBuffer::letter(char letter) {
    return rletter(letter);
}

StrBuffer &StrBuffer::text(const char *text) {
    char c;
    while ((c = *text++) != 0)
        letter(c);
    return *this;
}

StrBuffer &StrBuffer::rtext(const char *text) {
    char c;
    while ((c = *text++) != 0)
        rletter(c);
    return *this;
}

StrBuffer &StrBuffer::text(const StrScanner &scan) {
    auto text = scan;
    char c;
    while ((c = *text++) != 0)
        letter(c);
    return *this;
}

StrBuffer &StrBuffer::rtext(const StrScanner &scan) {
    auto text = scan;
    char c;
    while ((c = *text++) != 0)
        rletter(c);
    return *this;
}

StrBuffer &StrBuffer::text_P(const /*PROGMEM*/ char *text_P) {
    while (true) {
        const char c = pgm_read_byte(text_P++);
        if (c == 0)
            break;
        letter(c);
    }
    return *this;
}

StrBuffer &StrBuffer::rtext_P(const /*PROGMEM*/ char *text_P) {
    while (true) {
        const char c = pgm_read_byte(text_P++);
        if (c == 0)
            break;
        rletter(c);
    }
    return *this;
}

StrBuffer &StrBuffer::float32(float f32, uint_fast8_t prec) {
    *_end = 1;  // to check buffer overflow
    const auto len = snprintf_P(_out, capacity() + 1, PSTR("%.*g"), prec, f32);
    return convert(len);
}

StrBuffer &StrBuffer::float64(double f64, uint_fast8_t prec) {
    *_end = 1;  // to check buffer overflow
    const auto len = snprintf_P(_out, capacity() + 1, PSTR("%.*g"), prec, f64);
    return convert(len);
}

StrBuffer &StrBuffer::float80(int16_t exponent, uint64_t significand, uint_fast8_t prec) {
    *_end = 1;  // to check buffer overflow
    // TODO: Implement Dragon4 algorithm
    const auto n = significand * pow(2.0, exponent);
    const auto len = snprintf_P(_out, capacity() + 1, PSTR("%.*g"), prec, n);
    return convert(len);
}

StrBuffer &StrBuffer::convert(size_t len) {
    if (*_end == 0)
        setError(BUFFER_OVERFLOW);
    for (auto c = *_out; len; --len) {
        const auto next = _out[1];
        // may convert letter case.
        letter(c);
        c = next;
    }
    return *this;
}

StrBuffer &StrBuffer::comma() {
    return rletter(',').rletter(' ');
}

StrBuffer &StrBuffer::reverse(char *start) {
    if (start > _out) {
        setError(OVERWRAP_SEGMENT);
    } else {
        for (auto *end = _out - 1; start < end;) {
            const auto c = *start;
            *start++ = *end;
            *end-- = c;
        }
    }
    return *this;
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
