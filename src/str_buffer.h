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
#ifndef __STR_BUFFER_H__
#define __STR_BUFFER_H__

#include "error_reporter.h"
#include "str_scanner.h"

#include <ctype.h>
#include <stdint.h>

namespace libasm {

/**
 * Convenient and safe wrapper for C character buffer.
 */
struct StrBuffer : ErrorReporter {
    /** constructor from C char buffer. */
    StrBuffer(char *buffer, size_t size);

    /** copy constructor. */
    StrBuffer(const StrBuffer &o);

    /** Return the head of buffer. */
    constexpr const char *str() const { return _str; }
    constexpr operator const char *() const { return str(); }

    /** Return character length. */
    constexpr int len() const { return static_cast<int>(_out - _str); }

    /** Return remaining length. */
    constexpr size_t capacity() const { return _end - _out; }

    /** Return whole length of buffer. */
    constexpr size_t size() const { return _end - _str; }

    /** Return the current output position */
    char *mark() const { return _out; }

    /** Reset buffer to . */
    StrBuffer &reset();

    /** Hand over output buffer to |heir| and return it. */
    StrBuffer &over(StrBuffer &heir) const;

    /** Output a |letter| as is. */
    StrBuffer &rletter(char c);

    /** Output a |letter| using specific case conversion. */
    virtual StrBuffer &letter(char c);

    /** Output |text| via |letter(char)|. */
    StrBuffer &text(const char *text);
    StrBuffer &text(const StrScanner &scan);
    StrBuffer &text_P(const /*PROGMEM*/ char *text_P);

    /** Output |text| via |rletter(char)| as is. */
    StrBuffer &rtext(const char *text);
    StrBuffer &rtext(const StrScanner &scan);
    StrBuffer &rtext_P(const /*PROGMEM*/ char *text_P);

    /** Output |num| as decimal number. */
    StrBuffer &uint8(uint8_t num);

    /** Output |num| as 32-bit floating point number. */
    StrBuffer &float32(float num);

    /** Output |num| as 64-bit floating point number. */
    StrBuffer &float64(double num);

    /** output ", " */
    StrBuffer &comma();

    /** Reverse letters from |start| to |_out|. */
    StrBuffer &reverse(char *start);

private:
    char *_str;
    char *_out;
    const char *_end;
};

/**
 * Case converting StrBuffer.
 */
struct StrCaseBuffer : StrBuffer {
    /** constructor from C char buffer. */
    StrCaseBuffer(char *buffer, size_t size, bool uppercase)
        : StrBuffer(buffer, size), _converter(uppercase ? toupper : tolower) {}

    /** constroctor from StrBuffer. */
    StrCaseBuffer(const StrBuffer &o, bool uppercase)
        : StrBuffer(o), _converter(uppercase ? toupper : tolower) {}

    /** Output a |letter| with case covert */
    StrBuffer &letter(char c) override {
        StrBuffer::letter(_converter(c));
        return *this;
    }

private:
    int (*const _converter)(int);
};

}  // namespace libasm

#endif  // __STR_BUFFER_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
