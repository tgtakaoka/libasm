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

StrBuffer &StrBuffer::uint8(uint8_t num) {
    auto *start = mark();
    do {
        letter(num % 10 + '0');
        num /= 10;
    } while (num);
    return reverse(start);
}

StrBuffer &StrBuffer::float32(double num) {
    *_end = 1;  // to check buffer overflow
    const auto len = snprintf_P(_out, _end - _out + 1, PSTR("%.9g"), num);
    return convert(len);
}

StrBuffer &StrBuffer::float64(double num) {
    *_end = 1;  // to check buffer overflow
    const auto len = snprintf_P(_out, _end - _out + 1, PSTR("%.17g"), num);
    return convert(len);
}

StrBuffer &StrBuffer::float80(int16_t exponent, uint64_t significand) {
    *_end = 1;  // to check buffer overflow
    // TODO: Implement Dragon4 algorithm
    const auto num = significand * pow(2.0, exponent);
    const auto len = snprintf_P(_out, _end - _out + 1, PSTR("%.17g"), num);
    return convert(len);
}

StrBuffer &StrBuffer::convert(size_t len) {
    if (*_end == 0) {
        // Possible buffer overflow, though it may be just the right length.
        setError(BUFFER_OVERFLOW);
    }
    for (auto c = *_out; len; --len) {
        const auto next = _out[1];
        letter(c);
        c = next;
    }
    return *this;
}

StrBuffer &StrBuffer::comma() {
    return rletter(',').rletter(' ');
}

StrBuffer &StrBuffer::reverse(char *start) {
    if (start >= _out) {
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
