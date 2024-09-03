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
#ifndef __LIBASM_STR_BUFFER_H__
#define __LIBASM_STR_BUFFER_H__

#include <ctype.h>
#include <stdint.h>
#include "error_reporter.h"
#include "str_scanner.h"
#include "type_traits.h"

namespace libasm {

/**
 * Convenient and safe wrapper for C character buffer.
 */
struct StrBuffer : ErrorReporter {
    /** constructor from C char buffer. */
    StrBuffer(char *buffer, size_t size);

    /** copy constructor. */
    explicit StrBuffer(const StrBuffer &o);

    /** Return the head of buffer. */
    const char *str() const { return _str; }
    operator const char *() const { return str(); }

    /** Return character length. */
    int len() const { return static_cast<int>(_out - _str); }

    /** Return remaining length. */
    size_t capacity() const { return _end - _out; }

    /** Return whole length of buffer. */
    size_t size() const { return _end - _str; }

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

    /** Output |v| in hexadecimal. */
    template <typename T>
    StrBuffer &hex(T v, uint_fast8_t prec = 0) {
        using U = typename make_unsigned<T>::type;
        U u = static_cast<U>(v);
        auto *start = mark();
        for (uint_fast8_t i = 0; i < sizeof(T) * 2; ++i) {
            const uint8_t digit = u & 0xF;
            letter(digit + (digit < 10 ? '0' : 'A' - 10));
            u >>= 4;
            if (prec && --prec == 0)
                break;
        }
        for (; prec; --prec)
            letter('0');
        return reverse(start);
    }

    /** Output signed/unsigned |v| in decimal. */
    template <typename T>
    StrBuffer &dec(T v, uint_fast8_t prec = 0) {
        const auto negative = is_signed<T>::value && v < 0;
        if (negative)
            letter('-');
        using U = typename make_unsigned<T>::type;
        U u = negative ? static_cast<U>(-v) : v;
        auto *start = mark();
        do {
            const uint8_t digit = u % 10;
            letter(digit + '0');
            u /= 10;
            if (prec)
                --prec;
        } while (u);
        for (; prec; --prec)
            letter('0');
        return reverse(start);
    }

    /** Output |value| as decimal number. */
    StrBuffer &int16(int16_t i16, uint_fast8_t prec = 0) { return dec(i16, prec); }

    /**
     * Output |value| as 32-bit floating point number.
     * Significand is 23+1 bit and UINT24_MAX is 16777215 (8 digits).
     */
    StrBuffer &float32(float f32, uint_fast8_t prec = 9);

    /**
     * Output |value| as 64-bit floating point number.
     * Significand is 52+1 bit and UINT53_MAX is 9007199254740991 (16 digits)
     */
    StrBuffer &float64(double f64, uint_fast8_t prec = 17);

    /**
     * Output |value| as 80-bit floating point number.
     * Significandis 64 bit and UINT64_MAX is 18446744073709551615 (20 digits)
     */
    StrBuffer &float80(int16_t exponent, uint64_t mantissa, uint_fast8_t prec = 21);

    /** output ", " */
    StrBuffer &comma();

    /** Reverse letters from |start| to |_out|. */
    StrBuffer &reverse(char *start);

private:
    char *_str;
    char *_out;
    char *_end;

    StrBuffer &convert(size_t len);
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

#endif  // __LIBASM_STR_BUFFER_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
