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

#include "config_host.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>

namespace libasm {

StrBuffer &StrBuffer::reset(char *buffer, size_t size) {
    _out = buffer;
    _end = buffer + size - 1;
    _out[0] = 0;
    return *this;
}

StrBuffer &StrBuffer::letter(char letter) {
    if (_out < _end) {
        *_out++ = letter;
        *_out = 0;
    } else {
        setError(BUFFER_OVERFLOW);
    }
    return *this;
}

StrBuffer &StrBuffer::text(const char *text) {
    char c;
    while ((c = *text++) != 0)
        letter(c);
    return *this;
}

StrBuffer &StrBuffer::text(const char *text, bool uppercase) {
    const auto &conv = uppercase ? toupper : tolower;
    char c;
    while ((c = *text++) != 0)
        letter(conv(c));
    return *this;
}

StrBuffer &StrBuffer::text(const StrScanner &scan) {
    StrScanner text(scan);
    char c;
    while ((c = *text++) != 0)
        letter(c);
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

StrBuffer &StrBuffer::text_P(const /*PROGMEM*/ char *text_P, bool uppercase) {
    const auto &conv = uppercase ? toupper : tolower;
    while (true) {
        const char c = pgm_read_byte(text_P++);
        if (c == 0)
            break;
        letter(conv(c));
    }
    return *this;
}

StrBuffer &StrBuffer::format_P(const /*PROGMEM*/ char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char *end = _out + vsnprintf_P(_out, _end - _out, fmt, args);
    if (end < _end) {
        _out = end;
    } else {
        setError(BUFFER_OVERFLOW);
    }
    return *this;
}

StrBuffer &StrBuffer::comma() {
    return letter(',').letter(' ');
}

StrBuffer &StrBuffer::reverse(char *start) {
    for (char *end = _out - 1; start < end;) {
        const char c = *start;
        *start++ = *end;
        *end-- = c;
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
